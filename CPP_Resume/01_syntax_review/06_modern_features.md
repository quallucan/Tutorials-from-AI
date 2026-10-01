# 1.6 现代 C++ 新特性总览

## 📖 本节概述

自 C++11 以来，C++ 经历了巨大的现代化变革。本节将快速概览 C++11/14/17/20 的关键新特性，帮助你了解现代 C++ 的全貌。更深入的讲解将在第二章展开。

---

本教程统一按 **C++20** 编译。下文按引入版本回顾演进，旧版本标题不表示要切换编译标准。Concepts、Ranges、`span`、`format`、`jthread` 是本教程可直接使用的工具。

| 实践主题 | 完整示例 |
|----------|----------|
| Concepts / requires | [template_demo.cpp](./examples/template_demo.cpp) |
| span、三路比较、指定初始化、format、source_location | [modern_features_demo.cpp](./examples/modern_features_demo.cpp) |
| 协程帧与 co_yield | [coroutine_demo.cpp](./examples/coroutine_demo.cpp) |
| 模板 Lambda | [lambda_demo.cpp](../02_modern_paradigms/examples/lambda_demo.cpp) |
| consteval、constinit、编译期 vector 与排序 | [constexpr_demo.cpp](../02_modern_paradigms/examples/constexpr_demo.cpp) |
| jthread、停止请求和同步工具 | [thread_demo.cpp](../02_modern_paradigms/examples/thread_demo.cpp) |
| Ranges、视图、投影 | [stl_algo_demo.cpp](../03_algorithm_design/examples/stl_algo_demo.cpp) |


## 1. C++11 - 现代 C++ 的起点

C++11 是一个里程碑式的版本，引入了大量改变语言面貌的特性。

### 1.1 自动类型推断 (auto)

```cpp
// 之前：冗长的类型声明
std::vector<std::map<std::string, int>>::iterator it = container.begin();

// C++11：简洁的 auto
auto it = container.begin();
auto x = 42;        // int
auto y = 3.14;      // double
auto z = "hello";   // const char*
```

### 1.2 范围 for 循环

```cpp
std::vector<int> nums = {1, 2, 3, 4, 5};

// 之前
for (std::vector<int>::iterator it = nums.begin(); it != nums.end(); ++it) {
    std::cout << *it << " ";
}

// C++11
for (int n : nums) {
    std::cout << n << " ";
}

// 配合 auto 和引用
for (const auto& n : nums) {
    std::cout << n << " ";
}
```

### 1.3 Lambda 表达式

```cpp
// 基本语法
auto add = [](int a, int b) { return a + b; };
std::cout << add(3, 5) << std::endl;  // 8

// 捕获变量
int multiplier = 3;
auto times = [multiplier](int x) { return x * multiplier; };
auto times_ref = [&multiplier](int x) { return x * multiplier; };  // 引用捕获

// 在算法中使用
std::vector<int> nums = {3, 1, 4, 1, 5};
std::sort(nums.begin(), nums.end(), [](int a, int b) { 
    return a > b;  // 降序
});
```

### 1.4 智能指针

```cpp
#include <memory>

// unique_ptr：独占所有权
std::unique_ptr<int> p1 = std::make_unique<int>(42);  // C++14

// shared_ptr：共享所有权
std::shared_ptr<int> p2 = std::make_shared<int>(42);
std::shared_ptr<int> p3 = p2;  // 引用计数 +1

// weak_ptr：弱引用，不增加引用计数
std::weak_ptr<int> wp = p2;

// 不需要手动 delete！
```

### 1.5 右值引用和移动语义

```cpp
#include <utility>

// 移动而非拷贝
std::vector<int> create_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v;  // 自动移动
}

std::vector<int> v1 = create_vector();  // 移动构造

std::string s1 = "Hello";
std::string s2 = std::move(s1);  // s1 的资源被移动到 s2
// s1 现在为空
```

### 1.6 nullptr

```cpp
int* p1 = nullptr;    // 类型安全的空指针
int* p2 = NULL;       // 旧方式，可能有歧义
int* p3 = 0;          // 更旧的方式

void foo(int);
void foo(int*);

foo(NULL);      // 可能调用 foo(int)！
foo(nullptr);   // 明确调用 foo(int*)
```

### 1.7 constexpr

```cpp
// 编译期计算
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int result = factorial(5);  // 编译期计算为 120

// 编译期数组大小
constexpr int size = 10;
int arr[size];  // OK
```

### 1.8 统一初始化

```cpp
// 使用 {} 初始化一切
int a{42};
std::vector<int> v{1, 2, 3};
std::map<int, std::string> m{{1, "one"}, {2, "two"}};

class Point {
public:
    int x, y;
};
Point p{10, 20};

// 防止窄化转换
int x{3.14};  // 错误！double 到 int 的窄化
```

### 1.9 类型别名 (using)

```cpp
// 替代 typedef
typedef std::vector<std::pair<int, int>> PairVector;  // 旧方式
using PairVector = std::vector<std::pair<int, int>>;  // C++11

// 支持模板别名
template<typename T>
using Vec = std::vector<T>;

Vec<int> v;  // std::vector<int>
```

### 1.10 其他 C++11 特性

```cpp
// override 和 final
class Base {
    virtual void foo();
};
class Derived : public Base {
    void foo() override;  // 确保是重写
};

// 枚举类
enum class Color { Red, Green, Blue };
Color c = Color::Red;  // 需要作用域限定

// default 和 delete
class NonCopyable {
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

// 类内成员初始化
class Widget {
    int value_ = 0;  // 默认值
};

// 可变参数模板
template<typename... Args>
void print(Args... args);

// static_assert
static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");

// noexcept
void safe_function() noexcept;

// 原始字符串字面量
std::string path = R"(C:\Users\Name\Documents)";
```

---

## 2. C++14 - 完善和改进

C++14 主要是对 C++11 的完善，修复了一些问题并添加了便利特性。

### 2.1 泛型 Lambda

```cpp
// C++11：必须指定参数类型
auto add11 = [](int a, int b) { return a + b; };

// C++14：auto 参数
auto add14 = [](auto a, auto b) { return a + b; };

add14(1, 2);        // int
add14(1.5, 2.5);    // double
add14(std::string("a"), std::string("b"));  // string
```

### 2.2 返回类型推断

```cpp
// C++14：函数返回类型自动推断
auto add(int a, int b) {
    return a + b;  // 推断返回 int
}

// 递归函数需要小心
auto factorial(int n) -> int {  // 需要指定返回类型
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

### 2.3 变量模板

```cpp
template<typename T>
constexpr T pi = T(3.14159265358979);

double d = pi<double>;
float f = pi<float>;
```

### 2.4 make_unique

```cpp
// C++11 漏掉了 make_unique
std::unique_ptr<int> p1(new int(42));  // C++11

// C++14 补上了
auto p2 = std::make_unique<int>(42);   // C++14，更安全
```

### 2.5 二进制字面量和数字分隔符

```cpp
int binary = 0b1010'1100;    // 二进制：172
int big = 1'000'000'000;     // 十亿，易读
double pi = 3.14159'26535;   // 也适用于浮点数
```

### 2.6 [[deprecated]] 属性

```cpp
[[deprecated("Use newFunction instead")]]
void oldFunction() {
    // ...
}

oldFunction();  // 编译器警告
```

---

## 3. C++17 - 实用特性大爆发

C++17 带来了大量实用的语言和库特性。

### 3.1 结构化绑定

```cpp
// 解构 pair/tuple
std::pair<int, std::string> p = {1, "one"};
auto [id, name] = p;

// 解构 map 元素
std::map<int, std::string> m = {{1, "one"}, {2, "two"}};
for (const auto& [key, value] : m) {
    std::cout << key << ": " << value << std::endl;
}

// 解构数组
int arr[] = {1, 2, 3};
auto [a, b, c] = arr;

// 解构结构体
struct Point { int x, y; };
Point pt = {10, 20};
auto [x, y] = pt;
```

### 3.2 if/switch 初始化语句

```cpp
// if 中的初始化
if (auto it = map.find(key); it != map.end()) {
    // 使用 it
}
// it 在此作用域外不可见

// switch 中的初始化
switch (auto value = get_value(); value) {
    case 1: break;
    case 2: break;
    default: break;
}
```

### 3.3 if constexpr - 编译期条件

```cpp
template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return value / 2.0;
    } else {
        return value;
    }
}

// 在编译期选择分支，未选中的分支不会编译
```

### 3.4 折叠表达式

```cpp
// 可变参数模板更简洁
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // 折叠求和
}

template<typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

sum(1, 2, 3, 4, 5);         // 15
print_all(1, "hello", 3.14); // 1 hello 3.14
```

### 3.5 类模板参数推断 (CTAD)

```cpp
// C++17 之前
std::pair<int, double> p1{1, 3.14};
std::vector<int> v1{1, 2, 3};

// C++17：自动推断
std::pair p2{1, 3.14};      // pair<int, double>
std::vector v2{1, 2, 3};    // vector<int>
std::tuple t{1, "hello", 3.14};  // tuple<int, const char*, double>
```

### 3.6 std::optional

```cpp
#include <optional>

std::optional<int> find_value(const std::vector<int>& v, int target) {
    for (int x : v) {
        if (x == target) return x;
    }
    return std::nullopt;  // 没找到
}

auto result = find_value({1, 2, 3}, 2);
if (result) {
    std::cout << *result << std::endl;  // 2
}

// 或者使用 value_or
int value = find_value({1, 2, 3}, 5).value_or(-1);  // -1
```

### 3.7 std::variant

```cpp
#include <variant>

std::variant<int, double, std::string> v;

v = 42;
std::cout << std::get<int>(v) << std::endl;

v = 3.14;
std::cout << std::get<double>(v) << std::endl;

v = "hello";
std::cout << std::get<std::string>(v) << std::endl;

// 访问器模式
std::visit([](auto&& arg) {
    std::cout << arg << std::endl;
}, v);
```

### 3.8 std::string_view

```cpp
#include <string_view>

void process(std::string_view sv) {
    // 不拷贝，只是视图
    std::cout << sv << std::endl;
}

process("hello");              // const char* 直接使用
process(std::string("world")); // string 也可以

std::string_view sv = "Hello, World!";
std::string_view sub = sv.substr(0, 5);  // "Hello"，无拷贝
```

### 3.9 std::filesystem

```cpp
#include <filesystem>
namespace fs = std::filesystem;

// 路径操作
fs::path p = "/home/user/documents/file.txt";
std::cout << p.filename() << std::endl;   // file.txt
std::cout << p.extension() << std::endl;  // .txt
std::cout << p.parent_path() << std::endl; // /home/user/documents

// 文件操作
if (fs::exists(p)) {
    std::cout << fs::file_size(p) << " bytes" << std::endl;
}

// 遍历目录
for (const auto& entry : fs::directory_iterator("/home/user")) {
    std::cout << entry.path() << std::endl;
}

// 创建目录
fs::create_directories("/tmp/a/b/c");
```

### 3.10 其他 C++17 特性

```cpp
// inline 变量
class Widget {
    inline static int count = 0;  // 可以在头文件中定义
};

// [[nodiscard]]
[[nodiscard]] int compute() { return 42; }
compute();  // 警告：返回值被忽略

// [[maybe_unused]]
void foo([[maybe_unused]] int x) {
    // x 可能未使用，不会警告
}

// 嵌套命名空间
namespace A::B::C {  // 等价于 namespace A { namespace B { namespace C { ...
    void foo() {}
}

// std::any
#include <any>
std::any a = 42;
a = std::string("hello");
a = 3.14;
```

---

## 4. C++20 - 重大革新

C++20 是自 C++11 以来最大的更新，引入了四大特性：Concepts、Ranges、Coroutines、Modules。

### 4.1 Concepts - 约束模板

```cpp
#include <concepts>
#include <type_traits>

// 定义概念
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// 使用概念约束模板
template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// requires 子句
template<typename T>
    requires Addable<T>
T add2(T a, T b) {
    return a + b;
}

// 简洁语法
auto add3(Numeric auto a, Numeric auto b) {
    return a + b;
}
```

### 4.2 Ranges - 现代化算法

```cpp
#include <ranges>
#include <iostream>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// 传统方式
std::vector<int> result;
for (int x : nums) {
    if (x % 2 == 0) {
        result.push_back(x * x);
    }
}

// C++20 Ranges
auto result2 = nums 
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; });

for (int x : result2) {
    std::cout << x << " ";  // 4 16 36 64 100
}

// 延迟计算，不创建中间容器！
```

### 4.3 协程 (Coroutines)

C++20 提供 `co_await`、`co_yield`、`co_return` 及 `<coroutine>` 基础设施，但没有现成的标准 `generator` 或 `task` 类型。协程也不会自动创建线程或调度异步任务。

下面片段使用本项目自定义的 `IntGenerator`，需结合 [coroutine_demo.cpp](./examples/coroutine_demo.cpp) 中的完整定义阅读：

```cpp
IntGenerator range(int first, int last) {
    for (int i = first; i < last; ++i) {
        co_yield i;  // 保存状态，暂停到调用者下一次恢复
    }
}

void demo_coroutine() {
    auto values = range(1, 5);
    int value = 0;
    while (values.next(value)) std::cout << value << ' ';  // 1 2 3 4
}
```

返回类型的 `promise_type` 定义开始、产出、异常与结束行为；拥有协程句柄的对象必须管理协程帧的销毁。示例采用不可复制、可移动的 RAII 包装，在结束或提前退出时释放帧，并将协程中的异常传播给调用者。`std::generator` 是 C++23 的内容，不属于这里的基线。

### 4.4 Modules（模块）

以下是两个文件的语法介绍，不能拼成一个 `.cpp` 编译。模块的扫描、接口编译及链接命令依赖编译器和构建系统；它不纳入本教程的单文件 CMake 示例。`import std;` 不是 C++20 标准库的要求，普通头文件仍使用 `#include`。

```cpp
// math.ixx（模块接口）
export module math;

export int add(int a, int b) {
    return a + b;
}

export int multiply(int a, int b) {
    return a * b;
}

// main.cpp（独立翻译单元）
#include <iostream>
import math;

int main() {
    std::cout << add(2, 3) << std::endl;
    return 0;
}

// 模块提供显式导出边界；构建时间收益取决于工具链与项目组织
```

### 4.5 三路比较运算符 (<=>)

```cpp
#include <compare>

struct Point {
    int x, y;
    
    // 默认化 <=> 支持关系比较，并隐式声明默认化的 ==
    auto operator<=>(const Point&) const = default;
};

Point p1{1, 2}, p2{1, 3};
if (p1 < p2) {   // 自动生成
    std::cout << "p1 < p2" << std::endl;
}
if (p1 == p2) {  // 自动生成
    std::cout << "p1 == p2" << std::endl;
}
```

这里是 `= default` 的规则；手写 `<=>` 的函数体不会自动获得 `==`，需要另行定义。比较按成员声明顺序进行；包含浮点成员时还要考虑 NaN 对应的无序结果。

### 4.6 std::format

```cpp
#include <format>
#include <string>

std::string name = "Alice";
int age = 30;
double score = 95.5;

// 类似 Python 的格式化
std::string msg = std::format("Name: {}, Age: {}, Score: {:.1f}", 
                              name, age, score);
// "Name: Alice, Age: 30, Score: 95.5"

// 位置参数
std::string s = std::format("{1} is {0} years old", age, name);
// "Alice is 30 years old"

// 格式说明符
std::format("{:>10}", 42);     // "        42"（右对齐）
std::format("{:<10}", 42);     // "42        "（左对齐）
std::format("{:^10}", 42);     // "    42    "（居中）
std::format("{:#x}", 255);     // "0xff"
std::format("{:b}", 42);       // "101010"
```

### 4.7 编译期容器：临时分配，不保留动态存储

```cpp
#include <numeric>
#include <vector>

constexpr int sum_values() {
    std::vector<int> values{1, 2, 3};
    return std::accumulate(values.begin(), values.end(), 0);
}  // 动态存储在本次常量求值结束前释放
static_assert(sum_values() == 6);
// constexpr std::vector<int> values{1, 2, 3};  // 错误：动态存储逃逸出常量求值
```

需要持久保存编译期序列时使用 `std::array`。`consteval` 强制立即调用产生常量表达式；`constinit` 要求静态或线程存储期对象常量初始化，但不使对象只读，也不自动使它可用于常量表达式。详见 [2.6 编译期计算](../02_modern_paradigms/06_constexpr_compile.md)。

### 4.8 std::span：借用连续序列

```cpp
#include <iostream>
#include <span>
#include <vector>

void print_values(std::span<const int> data) {
    for (int x : data) std::cout << x << ' ';
}

void demo_span() {
    int array[]{1, 2, 3};
    std::vector<int> values{4, 5, 6};
    print_values(array);
    print_values(values);
    print_values(std::span{values}.first(2));
}
```

`span` 包含首地址和长度，不拥有数据，不延长底层对象的生命周期。`span<const int>` 限制通过该视图修改元素；`const span<int>` 仅限制视图本身，仍能修改元素。C++20 中 `operator[]` 不保证运行时边界检查，访问前要确保索引有效；容器重新分配后旧视图可能悬空。

### 4.9 日常语法与库工具

- `map`、`set` 及对应无序关联容器的 `contains`：检查键是否存在；不是所有容器都有该成员。
- `std::erase` / `std::erase_if`：统一删除接口；关联容器按条件删除使用 `std::erase_if`。
- `std::string::starts_with` / `ends_with`：前后缀判断；`std::string::contains` 属于 C++23。
- 指定初始化：`Point{.x = 1, .y = 2}`，只用于聚合类型，顺序必须与成员声明一致。
- `std::source_location`：通过默认参数记录调用位置，见配套示例。
- `<bit>` 中的 `std::popcount`、`std::has_single_bit` 与 `<numbers>` 中的数学常量。
- 范围 for 初始化语句：`for (auto values = make_values(); auto& x : values)`，让拥有数据的对象存活到循环结束。
- `std::jthread` / `std::stop_token`：自动回收线程与协作式停止；`std::osyncstream` 避免多线程消息相互穿插，见 [2.8 并发编程](../02_modern_paradigms/08_concurrency_intro.md)。

---

## 5. 特性速查表

| 特性 | C++11 | C++14 | C++17 | C++20 |
|------|-------|-------|-------|-------|
| auto | ✅ | 增强 | - | - |
| Lambda | ✅ | 泛型 | - | 模板 |
| 智能指针 | ✅ | make_unique | - | - |
| 移动语义 | ✅ | - | - | - |
| constexpr | 基础 | 增强 | if constexpr | 更强 |
| 范围 for | ✅ | - | - | 初始化 |
| nullptr | ✅ | - | - | - |
| 变参模板 | ✅ | - | 折叠 | - |
| 结构化绑定 | - | - | ✅ | - |
| optional | - | - | ✅ | - |
| variant | - | - | ✅ | - |
| string_view | - | - | ✅ | - |
| filesystem | - | - | ✅ | - |
| Concepts | - | - | - | ✅ |
| Ranges | - | - | - | ✅ |
| Coroutines | - | - | - | ✅ |
| Modules | - | - | - | ✅ |
| format | - | - | - | ✅ |
| <=> | - | - | - | ✅ |

---

## 📝 下一步学习建议

1. **立即开始使用**：auto、范围 for、智能指针、lambda
2. **深入学习**：移动语义、RAII（见第二章）
3. **提升效率**：string_view、optional、结构化绑定
4. **C++20 日常实践**：Concepts、Ranges、span、format、consteval、jthread
5. **进阶机制**：协程帧的生命周期和模块的构建边界

---

## 💡 要点总结

1. **C++11 是分水岭**：现代 C++ 从这里开始
2. **优先使用现代特性**：auto、智能指针、lambda、范围 for
3. **编译期编程**：constexpr、if constexpr 让代码更高效
4. **类型安全**：optional、variant 替代裸指针和 union
5. **Concepts 替代 SFINAE**：更清晰的模板约束
6. **Ranges 改变算法使用**：链式调用，延迟计算

---

## ⏭️ 下一章

[第二章：现代编程范式](../02_modern_paradigms/README.md) - 深入学习 RAII、智能指针、移动语义等核心概念

