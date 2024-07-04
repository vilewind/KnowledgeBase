---
CPP DevOps
---

# class 与 struct

> 可以将struct当作默认权限为public的类

## 构造函数

- **构造函数初始化列表**与**函数体内赋值**的差异

  > 函数体内赋值会先调用默认构造函数，再调用赋值构造函数；
  >
  > 初始化列表直接调用合适的构造函数

  ```c++
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
    std::cout << "Another ctor" << std::endl;
    Link link1(node, 1);
  
    return 0;
  }
  
  /**
  * @output
  	Initailizer list
  	another ctor
  	default
  	Operator =
  */
  ```

  - c++ 初始化的方式 “（）”与 “ {} ”

    > 在 C++ 中，花括号 `{}` 和圆括号 `()` 用于变量初始化和赋值时，有一些重要的差异。下面分别介绍这两种方式的不同点：
    >
    > ### 花括号 `{}` 初始化（列表初始化/统一初始化）
    >
    > 花括号初始化（列表初始化）在 C++11 中引入，目的是为了统一各种初始化方式，并且能够避免某些潜在的错误。它有以下几个特点：
    >
    > 1. **禁止隐式窄化转换**：列表初始化会阻止从大类型向小类型的隐式转换，比如从 `double` 到 `int`，这可以避免精度损失的问题。
    > 2. **可以初始化数组和聚合类型**：花括号可以用于初始化数组和结构体等聚合类型。
    > 3. **初始化为空**：可以通过 `{}` 进行默认初始化或空初始化。
    >
    > 示例：
    > ```cpp
    > int a = {10};         // OK
    > int b{10};            // OK
    > double d = {1.1};     // OK
    > int c = {1.1};        // 错误，窄化转换
    > 
    > int arr[3] = {1, 2, 3}; // 初始化数组
    > struct MyStruct {
    >     int x;
    >     double y;
    > };
    > MyStruct s = {1, 2.0};  // 初始化结构体
    > ```
    >
    > ### 圆括号 `()` 初始化（构造函数调用）
    >
    > 圆括号初始化通常用于调用构造函数进行初始化，适用于类类型和内置类型。它有以下几个特点：
    >
    > 1. **允许隐式窄化转换**：圆括号初始化允许从大类型向小类型的隐式转换。
    > 2. **无法直接初始化数组**：不能用于直接初始化数组。
    > 3. **调用构造函数**：对于类类型，圆括号初始化会调用对应的构造函数。
    >
    > 示例：
    > ```cpp
    > int a = 10;          // OK
    > int b(10);           // OK
    > double d = 1.1;      // OK
    > int c = 1.1;         // OK，隐式窄化转换
    > 
    > class MyClass {
    > public:
    >     MyClass(int x) {}
    > };
    > 
    > MyClass obj(10);    // 调用 MyClass 的构造函数
    > ```
    >
    > ### 总结
    >
    > - **列表初始化（{}）**：更加严格，禁止隐式窄化转换，适用于数组和聚合类型的初始化。
    > - **构造函数初始化（()）**：允许隐式窄化转换，主要用于类对象的构造。
    >
    > 选择哪种初始化方式取决于具体的使用场景和对隐式转换的控制需求。在 C++11 及更高版本中，推荐使用列表初始化 `{}`，以减少潜在的错误和不必要的转换。

- 拷贝构造与移动构造

  > 拷贝构造需要考虑深拷贝与浅拷贝的区别

  ```c++
  #include <iostream>
  
  class Node {
  public:
    Node() { std::cout << "default ctor" << std::endl; }
    explicit Node(const int a) : val_(a) {
      std::cout << "Init list ctor" << std::endl;
    }
  
    Node(const Node &rhs) : val_(rhs.val_) {
      std::cout << "Copy ctor" << std::endl;
    }
  
    Node &operator=(const Node &rhs) {
      if (this != &rhs) {
        val_ = rhs.val_;
      }
      std::cout << "CopyAssgin ctor" << std::endl;
      return *this;
    }
  
    Node(const Node &&rhs) : val_(rhs.val_) {
      std::cout << "Move Copy ctor" << std::endl;
    }
  
    Node &operator=(const Node &&rhs) {
      if (this != &rhs) {
        val_ = rhs.val_;
      }
      std::cout << "Move CopyAssgin ctor" << std::endl;
      return *this;
    }
  
  private:
    int val_{0};
  };
  
  class LinkWithPtr {
  public:
    LinkWithPtr() { node = new Node; }
    LinkWithPtr(LinkWithPtr &rhs) {
      if (this != &rhs) {
        delete node;
        node = new Node(*(rhs.node));
      }
    }
    LinkWithPtr(LinkWithPtr &&rhs) {
      std::cout << "LinkWithPtr Move Copy ctor" << std::endl;
      if (this != &rhs) {
        delete node;
        node = rhs.node;
        rhs.node = nullptr;
      }
    }
  
    ~LinkWithPtr() { delete node; }
  
  private:
    Node *node{nullptr};
  };
  
  class Link {
  public:
    Link() = default;
    Link(Link &rhs) : link(rhs.link) {}
    Link(Link &&rhs) : link(rhs.link) {}  // 此时调用拷贝构造，右值的成员是左值
    // Link(Link &&rhs) : link(std::move(rhs.link)) {}  // 此时调用移动拷贝
  
  private:
    LinkWithPtr link;
    int b{0};
  };
  
  int main() {
    Link link1;
    std::cout << "1" << std::endl;
    Link link2(link1);
    std::cout << "2" << std::endl;
    Link link3(std::move(link1));
    return 0;
  }
  ```

  