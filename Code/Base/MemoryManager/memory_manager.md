---
MemoryManager
---



# C/C++ 内存管理

![img](https://img-blog.csdnimg.cn/20210610102054691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5sb25nX2N4eQ==,size_16,color_FFFFFF,t_70)

- 栈：非静态局部变量/函数参数/返回值
- 内存映射段：文件映射、动态库以及匿名映射
- 堆：运行时动态内存分类
- 数据段（静态区）：全局数据和静态数据
- 代码段（常量区）：存放可执行代码和只读常量，如字面值

## c语言内存管理

malloc，calloc和recalloc以及free

## C++内存管理

![img](https://img-blog.csdnimg.cn/20210610153826397.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5sb25nX2N4eQ==,size_16,color_FFFFFF,t_70)

### new

```c++
void* operator new(std::size_t size) {
    if (size == 0) size = 1;  // Ensure at least 1 byte is allocated
    while (true) {
        void* ptr = std::malloc(size);
        if (ptr) return ptr;

        // If malloc fails, try to free up memory and retry
        std::new_handler handler = std::set_new_handler(0);
        std::set_new_handler(handler);
        if (!handler) {
            throw std::bad_alloc();  // If no new handler is set, throw exception
        }
        handler();  // Call the new handler to free up memory
    }
}

void* ptr = ::operator new(sizeof(int));
int * a = new (prt) int(10);
```

### delete

```c++
void operator delete(void* ptr) noexcept {
	free(ptr);
}

ptr->~Dtor();
::operator delete(ptr);
```

### Allocator

> 分配连续内存；释放时直接free
>
> 提供connstruct函数，调用placement new
>
> 提供destory函数，执行对象的析构函数

```c++
#include <iostream>
#include <memory>   // For std::allocator_traits

template <typename T>
class SimpleAllocator {
public:
    // 类型定义
    using value_type = T;

    SimpleAllocator() noexcept {}
    template <typename U>
    SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    // 分配内存
    T* allocate(std::size_t n) {
        if (n == 0) return nullptr;
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        void* ptr = std::malloc(n * sizeof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    // 释放内存
    void deallocate(T* ptr, std::size_t) noexcept {
        std::free(ptr);
    }

    // 构造对象
    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    // 销毁对象
    template <typename U>
    void destroy(U* ptr) noexcept {
        ptr->~U();
    }
};

// 比较两个分配器是否相等
template <typename T, typename U>
bool operator==(const SimpleAllocator<T>&, const SimpleAllocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
bool operator!=(const SimpleAllocator<T>& a, const SimpleAllocator<U>& b) noexcept {
    return !(a == b);
}

int main() {
    // 使用 SimpleAllocator 分配和管理 int 类型的内存
    SimpleAllocator<int> alloc;

    // 分配内存
    int* ptr = alloc.allocate(1);
    std::cout << "Allocated memory for one int" << std::endl;

    // 构造对象
    alloc.construct(ptr, 42);
    std::cout << "Constructed int with value: " << *ptr << std::endl;

    // 销毁对象
    alloc.destroy(ptr);
    std::cout << "Destroyed int" << std::endl;

    // 释放内存
    alloc.deallocate(ptr, 1);
    std::cout << "Deallocated memory for one int" << std::endl;

    return 0;
}

```

