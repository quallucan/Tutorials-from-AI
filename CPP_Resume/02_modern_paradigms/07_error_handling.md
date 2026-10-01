# 2.7 现代错误处理

## 📖 本节概述

错误处理是编程中的核心问题。C++ 提供了多种错误处理机制：返回值、异常、`std::optional`，以及用 `std::variant` 表示值或错误的 Result。本节将探讨这些机制的使用场景和最佳实践。

---

## 1. 错误处理方式概览

### 1.1 历史演变

| 方式 | 时代 | 特点 |
|------|------|------|
| 返回错误码 | C | 简单，但容易忽略 |
| 异常 | C++98 | 强制处理，但有性能争议 |
| `std::optional` | C++17 | 表示"可能没有值" |
| `std::variant` / Result | C++17 引入，C++20 可用 | 表示"值或错误" |
| `std::expected` | C++23（延伸阅读） | 标准化的"值或错误"接口，不属于本教程基线 |

### 1.2 选择指南

```
函数可能失败吗？
├─ 否 → 普通返回值
└─ 是 → 失败是否"异常"（罕见且严重）？
        ├─ 是 → 抛出异常
        └─ 否 → 函数可能没有结果？
                ├─ 是 → std::optional
                └─ 否 → 需要返回错误信息？
                        ├─ 是 → 基于 std::variant 的 Result
                        └─ 否 → std::optional
```

---

## 2. 异常处理

### 2.1 基本语法

```cpp
#include <stdexcept>

double divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

void demo() {
    try {
        double result = divide(10, 0);
        std::cout << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
}
```

### 2.2 标准异常层次

```cpp
// 异常类层次结构
std::exception
├── std::logic_error          // 逻辑错误（编程错误）
│   ├── std::invalid_argument
│   ├── std::out_of_range
│   ├── std::length_error
│   └── std::domain_error
├── std::runtime_error        // 运行时错误（外部因素）
│   ├── std::overflow_error
│   ├── std::underflow_error
│   └── std::range_error
└── std::bad_alloc           // 内存分配失败
```

### 2.3 自定义异常

```cpp
class NetworkError : public std::runtime_error {
public:
    int error_code;
    
    NetworkError(const std::string& message, int code)
        : std::runtime_error(message), error_code(code) {}
};

void connect() {
    // ...
    throw NetworkError("Connection refused", 111);
}

void demo() {
    try {
        connect();
    } catch (const NetworkError& e) {
        std::cerr << "Network error " << e.error_code 
                  << ": " << e.what() << std::endl;
    }
}
```

### 2.4 异常安全

```cpp
class Widget {
    int* data_;
    
public:
    Widget() : data_(new int[100]) {}
    ~Widget() { delete[] data_; }
    
    // 基本保证：可能修改状态，但不会泄漏
    void basic_safe(int value) {
        data_[0] = value;  // 可能抛出
        // 即使抛出，对象仍然有效
    }
    
    // 强保证：失败时状态回滚
    void strong_safe(std::vector<int>& v, int value) {
        std::vector<int> temp = v;  // 拷贝
        temp.push_back(value);       // 可能抛出
        v = std::move(temp);         // noexcept，只在成功时生效
    }
    
    // 不抛出保证
    int get_value() const noexcept {
        return data_[0];
    }
};
```

### 2.5 noexcept

```cpp
// noexcept：声明函数不抛出异常
void safe_function() noexcept {
    // 如果抛出异常，程序调用 std::terminate
}

// 条件 noexcept
template<typename T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// 检查是否 noexcept
static_assert(noexcept(safe_function()));
```

### 2.6 异常的问题

```cpp
// 1. 性能开销（争议性）
// 现代编译器：零成本异常（不抛时无开销，抛时有开销）

// 2. 难以追踪控制流
void complex_function() {
    operation1();  // 可能抛出
    operation2();  // 可能抛出
    operation3();  // 可能抛出
    // 哪个会抛？抛什么？
}

// 3. 异常不安全的遗留代码
extern "C" void c_function();  // C 函数不理解异常

// 4. 某些环境禁用异常（嵌入式、游戏引擎）
```

---

## 3. std::optional (C++17)

### 3.1 基本用法

```cpp
#include <optional>

// 返回可能不存在的值
std::optional<int> find_value(const std::vector<int>& v, int target) {
    for (int x : v) {
        if (x == target) return x;
    }
    return std::nullopt;  // 没找到
}

void demo() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // 方式1：检查并访问
    auto result = find_value(v, 3);
    if (result) {
        std::cout << "Found: " << *result << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }
    
    // 方式2：使用 has_value()
    if (result.has_value()) {
        std::cout << result.value() << std::endl;
    }
    
    // 方式3：value_or 提供默认值
    int val = find_value(v, 99).value_or(-1);  // -1
}
```

### 3.2 optional 的操作

```cpp
std::optional<int> opt;

// 创建
std::optional<int> o1;                    // 空
std::optional<int> o2 = std::nullopt;     // 空
std::optional<int> o3 = 42;               // 有值
std::optional<int> o4{std::in_place, 42}; // 原地构造

// 检查
if (opt) { }               // 转换为 bool
if (opt.has_value()) { }   // 显式检查

// 访问
int a = *opt;              // 不检查，未定义行为如果为空
int b = opt.value();       // 检查，为空时抛出 std::bad_optional_access
int c = opt.value_or(0);   // 为空时返回默认值

// 修改
opt = 100;                 // 赋值
opt.emplace(200);          // 原地构造
opt.reset();               // 置空
opt = std::nullopt;        // 置空
```

### 3.3 optional 与引用

```cpp
// optional<T&> 不被允许，使用 optional<reference_wrapper<T>>
int x = 42;
std::optional<std::reference_wrapper<int>> opt_ref = x;

if (opt_ref) {
    opt_ref->get() = 100;  // 修改 x
}
std::cout << x << std::endl;  // 100
```

### 3.4 optional 的适用场景

```cpp
// 1. 可能没有结果的查找
std::optional<User> find_user(int id);

// 2. 可选参数
void configure(std::optional<int> timeout = std::nullopt);

// 3. 延迟初始化
class Widget {
    std::optional<ExpensiveResource> resource_;
    
public:
    void ensure_initialized() {
        if (!resource_) {
            resource_.emplace();  // 第一次使用时初始化
        }
    }
};

// 4. 解析可能失败的输入
std::optional<int> parse_int(const std::string& s);
```

---

## 4. C++20：用 std::variant 表示值或错误

### 4.1 Result 类型

C++20 没有 `std::expected`。需要返回错误详情时，可以用 `std::variant<T, E>` 存储成功值或错误。下面的简化别名要求 T 与 E 不同；它不是完整的 expected 实现，不提供 `and_then` 等接口，通用 variant 还可能因异常进入无值状态。

```cpp
#include <climits>
#include <stdexcept>
#include <string>
#include <variant>

template<typename T, typename E>
using Result = std::variant<T, E>;

enum class ParseError { Empty, InvalidFormat, OutOfRange };

Result<int, ParseError> parse_int(const std::string& s) {
    if (s.empty()) return ParseError::Empty;
    try {
        std::size_t pos = 0;
        long value = std::stol(s, &pos);
        if (pos != s.size()) return ParseError::InvalidFormat;
        if (value < INT_MIN || value > INT_MAX) return ParseError::OutOfRange;
        return static_cast<int>(value);
    } catch (const std::invalid_argument&) {
        return ParseError::InvalidFormat;
    } catch (const std::out_of_range&) {
        return ParseError::OutOfRange;
    }
}
```

`std::stol` 接受前导空白和正负号；检查 `pos` 可以拒绝 `"42xyz"` 这样的尾随内容。完整程序见 [error_demo.cpp](./examples/error_demo.cpp)。

### 4.2 显式检查结果

```cpp
#include <iostream>

void demo_result() {
    auto result = parse_int("42");
    if (const auto* value = std::get_if<int>(&result)) {
        std::cout << "Value: " << *value << '\n';
    } else {
        switch (std::get<ParseError>(result)) {
            case ParseError::Empty: std::cout << "Empty string\n"; break;
            case ParseError::InvalidFormat: std::cout << "Invalid format\n"; break;
            case ParseError::OutOfRange: std::cout << "Out of range\n"; break;
        }
    }
}
```

`std::get_if` 在活动类型不匹配时返回空指针；`std::get` 则抛出 `std::bad_variant_access`。这个 Result 仅含 int 和枚举，两种类型都不会在构造或赋值时抛出异常。

### 4.3 多步骤错误传播

```cpp
Result<int, ParseError> parse_nonnegative(const std::string& s) {
    auto parsed = parse_int(s);
    if (const auto* error = std::get_if<ParseError>(&parsed)) {
        return *error;
    }
    const int value = std::get<int>(parsed);
    if (value < 0) return ParseError::OutOfRange;
    return value;
}
```

先处理失败分支，再执行下一步，适用于 C++20 中的多步骤操作。

### 4.4 延伸阅读：C++23 std::expected

C++23 在 `<expected>` 中提供 `std::expected<T, E>`、`std::unexpected` 及 `and_then`、`transform` 等组合操作。它们不是 C++20 标准库的一部分，本教程的可编译示例与练习均不依赖它们。`std::optional` 的 `and_then`、`transform`、`or_else` 同样属于 C++23，C++20 中使用普通条件分支。

---

## 5. 错误码

### 5.1 std::error_code

```cpp
#include <system_error>

std::error_code read_file(const std::string& path, std::string& content) {
    std::ifstream file(path);
    if (!file) {
        return std::make_error_code(std::errc::no_such_file_or_directory);
    }
    
    std::stringstream ss;
    ss << file.rdbuf();
    content = ss.str();
    
    return {};  // 成功，返回空 error_code
}

void demo() {
    std::string content;
    std::error_code ec = read_file("test.txt", content);
    
    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
    } else {
        std::cout << content << std::endl;
    }
}
```

### 5.2 自定义错误类别

```cpp
// 定义错误枚举
enum class FileError {
    Success = 0,
    NotFound,
    PermissionDenied,
    IOError
};

// 错误类别
class FileErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "FileError";
    }
    
    std::string message(int ev) const override {
        switch (static_cast<FileError>(ev)) {
            case FileError::Success: return "Success";
            case FileError::NotFound: return "File not found";
            case FileError::PermissionDenied: return "Permission denied";
            case FileError::IOError: return "I/O error";
        }
        return "Unknown error";
    }
};

const FileErrorCategory& file_error_category() {
    static FileErrorCategory instance;
    return instance;
}

std::error_code make_error_code(FileError e) {
    return {static_cast<int>(e), file_error_category()};
}
```

---

## 6. 最佳实践

### 6.1 选择正确的机制

```cpp
// 使用异常：真正的异常情况
void parse_config(const std::string& path) {
    if (path.empty()) {
        throw std::invalid_argument("Config path cannot be empty");
    }
    // 配置路径为空是编程错误，应该用异常
}

// 使用 optional：结果可能不存在
std::optional<User> find_user(int id);
// 用户不存在是正常情况，不是错误

// 使用第 4 节的 Result：需要返回错误信息
Result<File, FileError> open_file(const std::string& path);
// 文件打开可能失败，需要知道原因

// 使用 error_code：与 C 接口或系统调用配合
std::error_code connect(const std::string& host);
```

### 6.2 异常处理原则

```cpp
// 1. 只捕获能处理的异常
try {
    operation();
} catch (const SpecificException& e) {
    // 只捕获知道如何处理的异常
    handle_error(e);
}

// 2. 按引用捕获
try {
    operation();
} catch (const std::exception& e) {  // 使用 const&
    // 避免切片
}

// 3. 不要在析构函数中抛出异常
class Resource {
public:
    ~Resource() noexcept {
        try {
            cleanup();
        } catch (...) {
            // 记录日志，但不传播
        }
    }
};

// 4. 使用 RAII 保证清理
void safe_function() {
    auto resource = std::make_unique<Resource>();
    // 即使抛出异常，resource 也会被正确释放
    may_throw();
}
```

### 6.3 optional 使用原则

```cpp
// ✅ 好的使用
std::optional<int> find_index(const std::vector<int>& v, int value);
std::optional<Config> load_optional_config();

// ❌ 不好的使用：当需要知道失败原因时
std::optional<User> get_user(int id);  // 为什么失败？网络？不存在？

// ❌ 不好的使用：返回默认值更合适时
std::optional<int> get_count();  // 如果没有，返回 0 可能更好
```

### 6.4 组合使用

```cpp
// 结合 optional 和异常
std::optional<User> find_user(int id) {
    if (id < 0) {
        throw std::invalid_argument("Invalid user ID");
    }
    
    auto it = users.find(id);
    if (it != users.end()) {
        return it->second;
    }
    return std::nullopt;  // 没找到是正常情况
}

// 结合 Result 和 optional：空输入表示“未提供”，非空但无效表示错误
Result<std::optional<int>, ParseError> parse_optional_int(const std::string& s) {
    if (s.empty()) return std::optional<int>{};
    auto result = parse_int(s);
    if (const auto* error = std::get_if<ParseError>(&result)) return *error;
    return std::optional<int>{std::get<int>(result)};
}
```

---

## 7. 总结对比

| 机制 | 优点 | 缺点 | 适用场景 |
|------|------|------|----------|
| 异常 | 强制处理、可跨层传播 | 性能、控制流复杂 | 真正的异常情况 |
| optional | 简单、类型安全 | 不携带错误信息 | 结果可能不存在 |
| variant / Result | 携带错误信息、类型安全 | 需自行设计接口和传播逻辑 | C++20 中需要错误详情 |
| error_code | 轻量、无异常 | 容易忽略 | 系统调用、C 接口 |

---

## 📝 练习题

### 练习1：异常安全的 Vector
实现一个简化版 Vector，确保 push_back 提供强异常安全保证。

### 练习2：可选配置解析
使用 `std::optional` 实现一个配置解析器，处理可选的配置项。

### 练习3：实现简单的 Result 类型
在 C++20 中使用 `std::variant` 封装 Result 类型，区分成功值与错误，并测试空输入、格式错误和越界。

### 练习4：错误处理链
使用普通条件分支组合 optional 或 Result，实现一个多步骤操作的错误处理链。

---

## 💡 要点总结

1. **异常用于真正的异常情况**：罕见且严重的错误
2. **optional 用于"可能没有"**：查找未找到、可选参数
3. **Result 用于"可能失败"**：C++20 中用 variant 保存错误详情；expected 是 C++23 延伸内容
4. **使用 RAII 保证异常安全**
5. **析构函数不应抛出异常**：标记为 noexcept
6. **按 const 引用捕获异常**
7. **根据场景选择合适的错误处理机制**

---

## ⏭️ 下一节

[2.8 并发编程入门](./08_concurrency_intro.md) - 探索多线程编程的世界

