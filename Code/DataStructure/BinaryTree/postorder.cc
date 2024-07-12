#include "binary_tree.h"

#include <stack>

namespace dl {
namespace binary_tree {

void BinaryTree::PostOrderIter(std::vector<int> &ans) {
  if (!root_)
    return;
  std::stack<Node *> stk;
  Node *node = root_;
  Node *prev = nullptr;
  while (!stk.empty() || node) {
    while (node) {
      stk.push(node);
      node = node->left;
    }
    node = stk.top();
    if (!node->right || node->right == prev) {
      stk.pop();
      ans.push_back(node->val);
      prev = node;
      node = nullptr;
    } else {
      node = node->right;
    }
  }
}

static void PostOrderRecInner(BinaryTree::Node *node, std::vector<int> &ans) {
  if (!node)
    return;
  PostOrderRecInner(node->left, ans);
  PostOrderRecInner(node->right, ans);
  ans.push_back(node->val);
}

void BinaryTree::PostOrderRec(std::vector<int> &ans) {
  PostOrderRecInner(root_, ans);
}

} // namespace binary_tree
} // namespace dl
