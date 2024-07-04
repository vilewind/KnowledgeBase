#include <iostream>

class Node {
public:
  Node() { std::cout << "default" << std::endl; }
  explicit Node(int a) : a_(a) { std::cout << "Initailizer list" << std::endl; }

  void operator=(const Node &a) {
    a_ = a.a_;
    std::cout << "Operator =" << std::endl;
  }

private:
  int a_ = 0;
};

class Link {
public:
  Link(const Node &input) : node(input) {}
  Link(const Node &input, const int b) {
    node = input;
    b_ = b;
  }

private:
  Node node;
  int b_ = 0;
};

int main() {
  Node node(1);

  Link link(node);
  std::cout << "another ctor" << std::endl;
  Link link1(node, 1);

  return 0;
}
