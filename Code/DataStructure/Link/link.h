#ifndef __LINK_H__
#define __LINK_H__

#include <iostream>

#include "node.h"

template <typename T> class Link {
public:
  Link() = default;
  ~Link();

  void AddNode(const T &t);
  bool GetValue(T *const t);
  void Iterate();
  void Reverse();
  void CrusionReverse() { head_ = RecrusionReverse(head_); }
  bool IsEmpty() const { return head_ == nullptr; }

private:
  Node<T> *RecrusionReverse(Node<T> *head);

  Node<T> *head_{nullptr};
};

template <typename T> Link<T>::~Link() {
  Node<T> *next = nullptr;
  while (head_) {
    next = head_->next;
    head_->Release();
    head_ = next;
  }
}

template <typename T> void Link<T>::AddNode(const T &t) {
  Node<T> *node = new Node<T>(t);
  node->Release();
  node->next = head_;
  head_ = node;
}

template <typename T> void Link<T>::Iterate() {
  Node<T> *node = head_;
  while (node) {
    std::cout << node->value << " ";
    node = node->next;
  }
  std::cout << std::endl;
}

template <typename T> bool Link<T>::GetValue(T *const t) {
  if (nullptr == head_) {
    return false;
  }
  auto node = head_;
  head_ = head_->next;
  *t = std::move(node->value);
  node->Release();
  return true;
}

template <typename T> void Link<T>::Reverse() {
  Node<T> *prev = nullptr;
  Node<T> *node = head_;
  Node<T> *next = nullptr;
  while (node) {
    next = node->next;
    node->next = prev;
    prev = node;
    node = next;
  }
  head_ = prev;
}

template <typename T> Node<T> *Link<T>::RecrusionReverse(Node<T> *head) {
  if (!head || !head->next) {
    return head;
  }
  auto prev = RecrusionReverse(head->next);
  head->next->next = head;
  head->next = nullptr;
  return prev;
}

#endif
