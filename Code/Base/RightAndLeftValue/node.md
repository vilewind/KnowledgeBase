---
左值与右值：描述value的生命周期和存储
---

# 左值与右值

## 左值

> 能够标识特定内存位置的表达式，可以出现在赋值操作符的左边，通常有名字且持久存在。

特点：

- 可以取地址
- 可以在赋值操作符的左边
- 生命周期通常超过当前表达式

```c++
int x = 10;
int *p = &x;
x = 20;
```



## 右值

> 不能标识特定内存位置的表达式，只在表达式求值过程中存在

特点：

- 不可以取地址
- 通常不能在赋值操作符号左边
- 生命周期只在当前表达式中

```c++
int y = x + 5; // x + 5 是一个右值
/*
int *q = &(x + 5);
 Cannot take the address of an rvalue of type 'int' (clang typecheck_inva    lid_lvalue_addrof)
*/
```



> 区分左值右值的规则：
>
> - 左值在内存中有确定位置的对象
> - 右值：临时值或者字面值，不持久
> - **能不能对表达式取值，如果能，则是左值，否则为右值。所有具名变量或对象都是左值，右值不具名。**
> - 其中，右值又有两个概念，将亡值和纯右值。**非引用返回的临时变量、运算表达式产生的临时变量、原始字面量和lambda表达式都是纯右值**。**将亡值在C++11中引进，将要移动的对象、T &&函数返回值、std::move返回值和转换为T &&的类型转换函数的返回值都是将亡值。**



# 左值引用与右值引用

> **const 左值引用可以接受左值、右值、const 左值、const 右值**

```c++
/*
int &lvalue = 10;
Non-const lvalue reference to type 'int' cannot bind to a temporary of type 'int' (clang lvalue_reference_bind_to_temporary)
*/
const int& lvalue = 10;
int &&rvalue = 10;
```

> 右值引用使用场景：用于优化临时对象的操作，如在函数返回值优化和移动构造函数中。

右值引用，通过定义移动构造和移动赋值实现资源窃取

```c++
#include <iostream>
#include <utility> // for std::move

class MyClass {
public:
    int* data;

    // 构造函数
    MyClass(size_t size) : data(new int[size]) {
        std::cout << "Constructing MyClass\n";
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept : data(other.data) {
        other.data = nullptr; // 将源对象置于有效但未定义状态
        std::cout << "Moving MyClass\n";
    }

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            delete[] data;    // 释放当前对象的资源
            data = other.data; // 转移资源
            other.data = nullptr; // 将源对象置于有效但未定义状态
            std::cout << "Move-assigning MyClass\n";
        }
        return *this;
    }

    // 析构函数
    ~MyClass() {
        delete[] data; // 释放资源
        std::cout << "Destructing MyClass\n";
    }
};

int main() {
    MyClass a(10);       // 普通构造函数
    MyClass b = std::move(a); // 移动构造函数，a 的资源被转移到 b

    MyClass c(5);
    c = std::move(b); // 移动赋值运算符，b 的资源被转移到 c

    return 0;
}
```

# 模版引用与完美转发

## T&

> Int& 或者模版&都只能接收左值

```c++
template <typename T> void foo1(T &arg) {}

int x = 10;
foo1(x);
/*
foo1(10);
No matching function for call to 'foo1' (clang ovl_no_viable_function_in_call)
*/
```

## T&& 模版中的万能引用

> 当模版参数类型是一个推倒类型时， T&&可以绑定右值，也可以绑定左值；但T是一个具体类型时（如int &&），T&& 表示右值，只能绑定右值；**引用折叠的缘故，所以有万能引用的概念**。引用折叠式微了解决模版参数推导出现双重引用的情况。

```c++
template <typename T>
void bar(T&& arg) {
    // arg 是一个万能引用
}

int main() {
    int x = 10;
    bar(x);  // 可以传递左值
    bar(10); // 也可以传递右值
}
```

## 完美转发

> T&&实现完美转发

```c++
int &&a = 10;
int &&b = a; // Rvalue reference to type 'int' cannot bind to lvalue of type 'int' (clang int &&r lvalue_to_rvalue_ref)
int &&c = std::forward<int>(a);
int &&d = std::move(a);
```

完美转发实现原理

```c++
namespace std {
    template<typename T>
    T&& forward(typename std::remove_reference<T>::type& t) noexcept {
        return static_cast<T&&>(t);
    }

    template<typename T>
    T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward call");
        return static_cast<T&&>(t);
    }
}
```

move实现原理

```c++
namespace std {
    template<typename T>
    typename remove_reference<T>::type&& move(T&& t) noexcept {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }
}
```



DEMO：参数为万能引用，且使用这个参数提供移动构造时使用forward

```c++
 Entry(K key, const V &value) : key(key) {
      value_ptr.store(new V(value), std::memory_order_release);
 }
 Entry(K key, V &&value) : key(key) {
   value_ptr.store(new V(std::forward<V>(value)), std::memory_order_release);
 }
```



# Ref

[右值的作用](https://www.cnblogs.com/ChinaHook/p/7684122.html)
