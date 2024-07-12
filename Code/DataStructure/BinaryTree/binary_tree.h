#ifndef __BINARY_TRR_H__
#define __BINARY_TRR_H__

#include "tree_node.h"

namespace dl {
namespace binary_tree {

class BinaryTree {
public:
  using Node = TreeNode<int>;

  BinaryTree(const std::vector<int> &val) : root_(Node::BuildTree(val)) {}
  ~BinaryTree() { Node::Clear(root_); }

  void PreOrderIter(std::vector<int> &ans);
  void PreOrderRec(std::vector<int> &ans);

  void InOrderIter(std::vector<int> &ans);
  void InOrderRec(std::vector<int> &ans);
  void InOrderMorris(std::vector<int> &ans);

  void PostOrderIter(std::vector<int> &ans);
  void PostOrderRec(std::vector<int> &ans);

  void LevelTraversal(std::vector<int> &ans);

private:
  Node *root_;
};
} // namespace binary_tree
} // namespace dl

#endif
