#include "binary_tree.h"

#include <queue>

namespace dl {
namespace binary_tree {

void BinaryTree::LevelTraversal(std::vector<int> &ans) {
  if (!root_)
    return;
  std::queue<Node *> que;
  que.push(root_);
  while (que.empty()) {
    auto node = que.front();
    ans.push_back(node->val);
    que.pop();
    if (node->left) {
      que.push(node->left);
    }
    if (node->right) {
      que.push(node->right);
    }
  }
}

} // namespace binary_tree
} // namespace dl
