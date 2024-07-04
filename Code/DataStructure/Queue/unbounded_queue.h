#ifndef __UNBOUNEDED_QUEUE_H__
#define __UNBOUNEDED_QUEUE_H__

#include <atomic>
#include <iostream>

template <typename T> class UnboundedQueue {
public:
  UnboundedQueue() {
    Node *node = new Node(-1);
    size_.store(0);
    head_.store(node);
    tail_.store(node);
  }

  ~UnboundedQueue() {
    auto iter = head_.load();
    Node *next = nullptr;
    while (iter) {
      next = iter->next;
      delete iter;
      iter = next;
    }
  }

  void Enqueue(T &&t) {
    auto old_tail = tail_.load(std::memory_order_acquire);
    T loacl_t = t;
    Node *node = new Node(std::forward<T>(t));
    while (true) {
      if (tail_.compare_exchange_strong(old_tail, node,
                                        std::memory_order_acq_rel,
                                        std::memory_order_relaxed)) {
        old_tail->next = node;
        node->Release();
        size_.fetch_add(1);
        break;
      }
    }
  }

  bool Dequeue(T *const t) {
    if (0 == size_.load()) {
      return false;
    }
    auto old_head = head_.load(std::memory_order_acquire);
    Node *next = nullptr;
    do {
      next = old_head->next;
      if (!next) {
        return false;
      }
    } while (!head_.compare_exchange_strong(
        old_head, next, std::memory_order_acq_rel, std::memory_order_relaxed));
    old_head->Release();
    *t = std::move(next->value);
    return true;
  }

private:
  struct Node {
    std::atomic<uint8_t> ref_count;
    T value;
    Node *next{nullptr};
    Node(T &&value) : value(value) { ref_count.store(2); }
    void Release() {
      ref_count.fetch_sub(1);
      if (0 == ref_count.load()) {
        delete this;
      }
    }
  };

  std::atomic<Node *> head_;
  std::atomic<Node *> tail_;
  std::atomic<int> size_;
};

#endif
