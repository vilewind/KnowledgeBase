#include "binary_tree.h"

#include <stack>

namespace dl {
namespace binary_tree {

void BinaryTree::InOrderIter(std::vector<int> &ans) {
  if (!root_)
    return;
  Node *node = root_;
  std::stack<Node *> stk;
  while (!stk.empty() || node) {
    while (node) {
      stk.push(node);
      node = node->left;
    }
    node = stk.top();
    stk.pop();
    ans.push_back(node->val);
    node = node->right;
  }
}

static void InOrderRecInner(BinaryTree::Node *node, std::vector<int> &ans) {
  if (!node)
    return;
  InOrderRecInner(node->left, ans);
  ans.push_back(node->val);
  InOrderRecInner(node->right, ans);
}

void BinaryTree::InOrderRec(std::vector<int> &ans) {
  InOrderRecInner(root_, ans);
}

void BinaryTree::InOrderMorris(std::vector<int> &ans) {
  Node *node = root_;
  while (node) {
    if (node->left) {
      Node *prev = node->left;
      while (prev->right && prev->right != node) {
        prev = prev->right;
      }
      if (!prev->right) {
        prev->right = node;
        node = node->left;
      } else {
        ans.push_back(node->val);
        prev->right = nullptr;
        node = node->right;
      }
    } else {
      ans.push_back(node->val);
      node = node->right;
    }
  }
}

} // namespace binary_tree
} // namespace dl
