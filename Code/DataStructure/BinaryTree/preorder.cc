#include "binary_tree.h"

#include <stack>

namespace dl {
namespace binary_tree {

void BinaryTree::PreOrderIter(std::vector<int> &ans) {
  if (!root_)
    return;
  std::stack<Node *> stk;
  Node *node = root_;
  while (!stk.empty() || node) {
    while (node) {
      ans.push_back(node->val);
      stk.push(node);
      node = node->left;
    }
    node = stk.top();
    stk.pop();
    node = node->right;
  }
}

static void PreOrderRecInner(BinaryTree::Node *node, std::vector<int> &ans) {
  if (!node)
    return;
  ans.push_back(node->val);
  PreOrderRecInner(node->left, ans);
  PreOrderRecInner(node->right, ans);
}

void BinaryTree::PreOrderRec(std::vector<int> &ans) {
  PreOrderRecInner(root_, ans);
}

} // namespace binary_tree
} // namespace dl
