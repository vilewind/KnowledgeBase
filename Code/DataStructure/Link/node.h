#ifndef __NODE_H__
#define __NODE_H__

#include <atomic>

template <typename T> struct Node {
public:
  T value;
  Node<T> *next;
  virtual Node<T> *prev() { return nullptr; }
  explicit Node(const T &t) : value(t), next(nullptr) { ref_count_.store(2); }
  Node(const T &t, Node *const next) : value(t), next(next) {
    ref_count_.store(2);
  }
  void Release();

private:
  std::atomic<uint32_t> ref_count_;
};

template <typename T> struct DoublyNode : public Node<T> {
public:
  Node<T> *prev_ptr;
  explicit DoublyNode(const T &&t) : Node<T>(t), prev_ptr(nullptr) {}
  DoublyNode(const T &&t, Node<T> *next, Node<T> *prev)
      : Node<T>(t, next), prev_ptr(prev) {}
  Node<T> *prev() override { return prev; }
};

template <typename T> void Node<T>::Release() {
  ref_count_.fetch_sub(1);
  if (0 == ref_count_.load()) {
    delete this;
  }
}

#endif
