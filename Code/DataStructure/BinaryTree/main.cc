#include "binary_tree.h"
#include <iostream>

using namespace ::dl::binary_tree;

void Cout(std::vector<int> &vec) {
  for (int i : vec) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  vec.clear();
}

int main() {
  std::vector<int> vec{1, 2, 3, 4, 5};
  BinaryTree bt(vec);
  std::cout << "PreOrderRec" << std::endl;
  std::vector<int> ans;
  bt.PreOrderIter(ans);
  Cout(ans);

  bt.PreOrderRec(ans);
  Cout(ans);

  bt.InOrderIter(ans);
  Cout(ans);

  bt.InOrderMorris(ans);
  Cout(ans);

  bt.InOrderRec(ans);
  Cout(ans);

  bt.PostOrderIter(ans);
  Cout(ans);
  bt.PostOrderRec(ans);
  Cout(ans);

  bt.LevelTraversal(ans);
  Cout(ans);

  return 0;
}
