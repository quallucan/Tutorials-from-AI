# 2.1 RAII 与资源管理

## 📖 本节概述

RAII（Resource Acquisition Is Initialization，资源获取即初始化）是 C++ 最重要的编程范式之一。它不仅仅是一种技术，更是一种思维方式——**让对象的生命周期管理资源的生命周期**。

理解 RAII 是掌握现代 C++ 的关键第一步。

---

## 1. 什么是资源？

在编程中，"资源"是指需要手动获取和释放的东西：

| 资源类型 | 获取操作 | 释放操作 |
|----------|----------|----------|
| 动态内存 | `new` | `delete` |
| 文件句柄 | `fopen()` / `open()` | `fclose()` / `close()` |
| 网络连接 | `connect()` | `close()` |
| 互斥锁 | `lock()` | `unlock()` |
| 数据库连接 | `connect()` | `disconnect()` |
| 线程 | 创建 | `join()` / `detach()` |

**问题**：如果获取了资源但忘记释放，就会导致资源泄漏。

---

## 2. 传统资源管理的问题

### 2.1 手动管理的困境

```cpp
void process_file_bad() {
    FILE* file = fopen("data.txt", "r");
    if (file == nullptr) {
        return;  // 错误处理
    }
    
    // 处理文件...
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == nullptr) {
        // ❌ 忘记 fclose！
        return;
    }
    
    // 更多处理...
    if (some_error_condition) {
        // ❌ 又忘记 fclose！
        throw std::runtime_error("Error!");
    }
    
    fclose(file);  // 只有正常路径会执行到这里
}
```

### 2.2 问题分析

1. **多个退出点**：每个 `return` 或 `throw` 都需要记得释放资源
2. **异常安全**：抛出异常后，后续的释放代码不会执行
3. **代码膨胀**：重复的清理代码
4. **容易遗忘**：人总会犯错

### 2.3 传统的 "解决方案"

```cpp
// 方法1：goto（不推荐）
void process_file_goto() {
    FILE* file = fopen("data.txt", "r");
    if (!file) return;
    
    if (error1) goto cleanup;
    if (error2) goto cleanup;
    // ...
    
cleanup:
    fclose(file);
}

// 方法2：标志变量（繁琐）
void process_file_flag() {
    FILE* file = fopen("data.txt", "r");
    bool success = false;
    
    do {
        if (!file) break;
        if (error1) break;
        if (error2) break;
        success = true;
    } while (false);
    
    if (file) fclose(file);
}
```

这些方法都很丑陋，而且仍然容易出错。

---

## 3. RAII：优雅的解决方案

### 3.1 核心思想

> **资源的生命周期与对象的生命周期绑定**
> - 在**构造函数**中获取资源
> - 在**析构函数**中释放资源

由于 C++ 保证：
- 对象离开作用域时，析构函数**一定会被调用**
- 即使发生异常，栈上对象的析构函数也会被调用（栈展开）

因此，资源的释放是**自动且保证的**！

### 3.2 RAII 示例：文件包装器

```cpp
class FileHandle {
private:
    FILE* file_;

public:
    // 构造函数：获取资源
    explicit FileHandle(const char* filename, const char* mode)
        : file_(fopen(filename, mode)) {
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
    }
    
    // 析构函数：释放资源
    ~FileHandle() {
        if (file_) {
            fclose(file_);
            std::cout << "File closed automatically" << std::endl;
        }
    }
    
    // 禁止拷贝（避免双重释放）
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // 允许移动
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }
    
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }
    
    // 提供访问接口
    FILE* get() const { return file_; }
    
    // 读取一行
    std::string read_line() {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file_)) {
            return std::string(buffer);
        }
        return "";
    }
};

// 使用
void process_file_raii() {
    FileHandle file("data.txt", "r");  // 构造时打开
    
    // 处理文件...
    std::string line = file.read_line();
    
    if (some_error) {
        throw std::runtime_error("Error!");
        // 不需要手动 close！析构函数会处理
    }
    
    // 更多处理...
    
}  // 离开作用域，析构函数自动关闭文件
```

### 3.3 RAII 的优势

1. **自动释放**：无论如何退出作用域，资源都会被释放
2. **异常安全**：即使抛出异常，资源也会正确释放
3. **代码简洁**：不需要重复的清理代码
4. **不会遗忘**：释放是自动的，不依赖人的记忆

---

## 4. RAII 在标准库中的应用

### 4.1 智能指针

```cpp
#include <memory>

void demo_smart_pointers() {
    // unique_ptr：独占所有权
    std::unique_ptr<int> p1 = std::make_unique<int>(42);
    // 离开作用域自动 delete
    
    // shared_ptr：共享所有权
    std::shared_ptr<int> p2 = std::make_shared<int>(100);
    std::shared_ptr<int> p3 = p2;  // 引用计数 = 2
    // 最后一个 shared_ptr 离开作用域时 delete
}
```

### 4.2 容器

```cpp
#include <vector>
#include <string>

void demo_containers() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    // vector 内部使用动态数组
    // 离开作用域时自动释放内存
    
    std::string s = "Hello, World!";
    // string 也是 RAII 包装器
}
```

### 4.3 锁

```cpp
#include <mutex>

std::mutex mtx;

void demo_locks() {
    std::lock_guard<std::mutex> lock(mtx);  // 自动加锁
    
    // 临界区代码...
    if (error) {
        throw std::runtime_error("Error!");
        // 不需要手动 unlock！
    }
    
}  // 离开作用域自动解锁
```

### 4.4 文件流

```cpp
#include <fstream>

void demo_fstream() {
    std::ifstream file("data.txt");  // 自动打开
    
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    
}  // 自动关闭
```

---

## 5. 实现自己的 RAII 类

### 5.1 通用模板

```cpp
template<typename T, typename Deleter>
class RAIIWrapper {
private:
    T resource_;
    Deleter deleter_;
    bool owns_;

public:
    explicit RAIIWrapper(T resource, Deleter deleter)
        : resource_(resource), deleter_(deleter), owns_(true) {}
    
    ~RAIIWrapper() {
        if (owns_) {
            deleter_(resource_);
        }
    }
    
    // 禁止拷贝
    RAIIWrapper(const RAIIWrapper&) = delete;
    RAIIWrapper& operator=(const RAIIWrapper&) = delete;
    
    // 允许移动
    RAIIWrapper(RAIIWrapper&& other) noexcept
        : resource_(other.resource_), deleter_(std::move(other.deleter_)), owns_(other.owns_) {
        other.owns_ = false;
    }
    
    RAIIWrapper& operator=(RAIIWrapper&& other) noexcept {
        if (this != &other) {
            if (owns_) deleter_(resource_);
            resource_ = other.resource_;
            deleter_ = std::move(other.deleter_);
            owns_ = other.owns_;
            other.owns_ = false;
        }
        return *this;
    }
    
    T get() const { return resource_; }
    
    T release() {
        owns_ = false;
        return resource_;
    }
};

// 使用示例
void demo_wrapper() {
    // 包装 malloc/free
    auto ptr = RAIIWrapper<void*, decltype(&free)>(
        malloc(100), free
    );
    
    // 使用...
}  // 自动 free
```

### 5.2 ScopeGuard 模式

```cpp
#include <functional>

class ScopeGuard {
private:
    std::function<void()> cleanup_;
    bool active_;

public:
    explicit ScopeGuard(std::function<void()> cleanup)
        : cleanup_(std::move(cleanup)), active_(true) {}
    
    ~ScopeGuard() {
        if (active_) {
            cleanup_();
        }
    }
    
    // 禁止拷贝
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
    // 取消清理（用于成功路径）
    void dismiss() {
        active_ = false;
    }
};

// 使用示例
void demo_scope_guard() {
    FILE* file = fopen("data.txt", "r");
    ScopeGuard guard([&]() { 
        if (file) fclose(file);
        std::cout << "Cleanup executed" << std::endl;
    });
    
    // 处理文件...
    if (error) {
        throw std::runtime_error("Error!");
        // guard 会自动清理
    }
    
    // 如果一切正常，可以取消自动清理
    // guard.dismiss();
    
}  // 离开作用域，guard 执行清理
```

### 5.3 C++20 的作用域清理选择

C++20 标准库没有 `std::scope_exit`，也没有标准头文件 `<scope>`。优先使用 `std::unique_ptr` 的自定义删除器、文件流、锁等已有 RAII 类型；一般回调可以使用上节的 ScopeGuard，清理回调必须保证不抛出异常。

`std::experimental::scope_exit` 属于 Library Fundamentals TS v3，见 [WG21 文档](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/n4920)。TS 扩展不是 C++20 或 C++23 的标准库保证，本教程不依赖 `<experimental/scope>`。

---

## 6. 所有权语义

### 6.1 独占所有权 vs 共享所有权

```cpp
// 独占所有权：只有一个对象拥有资源
class UniqueOwner {
    std::unique_ptr<Resource> resource_;
    // 不能拷贝，只能移动
};

// 共享所有权：多个对象共享资源
class SharedOwner {
    std::shared_ptr<Resource> resource_;
    // 可以拷贝，引用计数管理生命周期
};
```

### 6.2 所有权转移

```cpp
#include <memory>

std::unique_ptr<int> create_resource() {
    return std::make_unique<int>(42);  // 工厂函数返回所有权
}

void take_ownership(std::unique_ptr<int> ptr) {
    // 获取所有权，函数结束时释放
}

void demo_ownership_transfer() {
    auto ptr = create_resource();     // 获取所有权
    take_ownership(std::move(ptr));   // 转移所有权
    // ptr 现在为空
}
```

### 6.3 借用（不转移所有权）

```cpp
// 使用原始指针或引用表示"借用"
void use_resource(int* ptr) {     // 借用，不拥有
    std::cout << *ptr << std::endl;
}

void use_resource_ref(int& ref) {  // 引用方式借用
    std::cout << ref << std::endl;
}

void demo_borrowing() {
    auto ptr = std::make_unique<int>(42);
    
    use_resource(ptr.get());   // 借用
    use_resource_ref(*ptr);    // 借用
    
    // ptr 仍然拥有资源
}
```

---

## 7. 异常安全性

RAII 与异常安全性密切相关。C++ 定义了三个级别的异常安全保证：

### 7.1 基本保证（Basic Guarantee）

> 如果异常发生，程序处于有效状态，没有资源泄漏

```cpp
void basic_guarantee(std::vector<int>& v) {
    v.push_back(1);  // 可能抛出异常
    v.push_back(2);  // 如果这里抛出异常
    // v 可能只有一个元素，但不会泄漏
}
```

### 7.2 强保证（Strong Guarantee）

> 如果异常发生，程序状态回滚到调用前

```cpp
void strong_guarantee(std::vector<int>& v) {
    std::vector<int> temp = v;  // 拷贝
    temp.push_back(1);
    temp.push_back(2);
    v = std::move(temp);  // noexcept，只在成功时修改原数据
}
```

### 7.3 不抛出保证（Nothrow Guarantee）

> 操作保证不抛出异常

```cpp
void nothrow_guarantee() noexcept {
    // 这里的代码保证不抛出异常
    // 如果抛出，程序终止（std::terminate）
}
```

### 7.4 RAII 提供的保证

```cpp
class Transaction {
private:
    Database& db_;
    bool committed_ = false;

public:
    explicit Transaction(Database& db) : db_(db) {
        db_.begin_transaction();
    }
    
    ~Transaction() {
        if (!committed_) {
            db_.rollback();  // 自动回滚
        }
    }
    
    void commit() {
        db_.commit();
        committed_ = true;
    }
};

void do_work(Database& db) {
    Transaction tx(db);  // 开始事务
    
    db.execute("INSERT ...");
    db.execute("UPDATE ...");
    
    if (error) {
        throw std::runtime_error("Error!");
        // 自动回滚
    }
    
    tx.commit();  // 成功则提交
}  // 如果没有 commit，自动回滚
```

---

## 8. 最佳实践

### 8.1 永远使用 RAII 管理资源

```cpp
// ❌ 不好
void bad() {
    int* p = new int(42);
    // ... 使用 p ...
    delete p;  // 容易忘记或在异常时跳过
}

// ✅ 好
void good() {
    auto p = std::make_unique<int>(42);
    // ... 使用 p ...
}  // 自动释放
```

### 8.2 使用标准库提供的 RAII 类

```cpp
// 优先使用标准库
std::unique_ptr<T>    // 动态内存
std::shared_ptr<T>    // 共享所有权
std::lock_guard       // 互斥锁
std::unique_lock      // 灵活的锁
std::fstream          // 文件
std::thread           // 线程（配合 join/detach）
```

### 8.3 自定义 RAII 类的规则

1. **析构函数不抛出异常**：标记为 `noexcept`
2. **遵循五法则**：定义析构函数时，考虑拷贝/移动操作
3. **禁止拷贝或正确实现**：避免双重释放
4. **使用 explicit**：防止隐式转换

```cpp
class Resource {
public:
    explicit Resource(int id);  // explicit 防止隐式转换
    ~Resource() noexcept;       // noexcept
    
    Resource(const Resource&) = delete;             // 禁止拷贝
    Resource& operator=(const Resource&) = delete;
    
    Resource(Resource&&) noexcept;                  // 移动
    Resource& operator=(Resource&&) noexcept;
};
```

---

## 📝 练习题

### 练习1：实现 Timer RAII 类
创建一个 `Timer` 类，在构造时记录开始时间，析构时打印经过的时间。可用于测量代码段的执行时间。

### 练习2：实现 SocketHandle
创建一个 `SocketHandle` 类，包装 socket 文件描述符，自动关闭连接。

### 练习3：实现 MemoryMappedFile
创建一个类，使用 `mmap` 映射文件到内存，析构时自动 `munmap`。

### 练习4：事务回滚
实现一个简单的事务类，支持多个操作，如果任一操作失败则回滚所有已执行的操作。

---

## 💡 要点总结

1. **RAII 是 C++ 的核心范式**：资源生命周期绑定到对象生命周期
2. **析构函数保证执行**：即使发生异常
3. **优先使用标准库 RAII 类**：智能指针、锁、流等
4. **自定义 RAII 类要正确处理拷贝/移动**：通常禁止拷贝，允许移动
5. **析构函数不抛异常**：标记为 noexcept
6. **RAII 提供异常安全**：是实现异常安全代码的基础

---

## ⏭️ 下一节

[2.2 智能指针详解](./02_smart_pointers.md) - 深入学习 unique_ptr、shared_ptr、weak_ptr

