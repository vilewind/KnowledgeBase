#ifndef __TREENODE_H__
#define __TREENODE_H__

#include <queue>
#include <vector>

namespace dl {
namespace binary_tree {

template <typename Value> struct TreeNode {
  Value val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(const Value &val) : val(val), left(nullptr), right(nullptr) {}

  static TreeNode<Value> *BuildTree(const std::vector<Value> &val);
  static void Clear(TreeNode *root);
};

template <typename Value>
TreeNode<Value> *TreeNode<Value>::BuildTree(const std::vector<Value> &val) {
  if (val.empty()) {
    return nullptr;
  }

  TreeNode<Value> *root = new TreeNode<Value>(val.front());
  std::queue<TreeNode<Value> *> que;
  que.push(root);
  for (int i = 1; i < val.size();) {
    auto cur = que.front();
    que.pop();
    if (i < val.size()) {
      cur->left = new TreeNode<Value>(val[i++]);
      que.push(cur->left);
    }
    if (i < val.size()) {
      cur->right = new TreeNode<Value>(val[i++]);
      que.push(cur->right);
    }
  }
  return root;
}

template <typename Value> void TreeNode<Value>::Clear(TreeNode<Value> *root) {
  if (!root)
    return;
  Clear(root->left);
  Clear(root->right);
  delete root;
  root = nullptr;
}

} // namespace binary_tree
} // namespace dl

#endif
