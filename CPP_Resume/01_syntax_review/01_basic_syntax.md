# 1.1 基础语法回顾

## 📖 本节概述

本节将回顾C++的基础语法，包括数据类型、变量、运算符、控制流和函数。这些是C++编程的基石，确保你对这些内容有清晰的理解将为后续学习打下坚实基础。

---

## 1. 数据类型

### 1.1 基本数据类型

C++提供了丰富的内置数据类型：

| 类型 | 描述 | 典型大小 | 示例 |
|------|------|----------|------|
| `bool` | 布尔类型 | 1字节 | `true`, `false` |
| `char` | 字符类型 | 1字节 | `'a'`, `'Z'` |
| `int` | 整数类型 | 4字节 | `42`, `-100` |
| `long` | 长整型 | 4/8字节 | `100000L` |
| `long long` | 更长整型 | 8字节 | `9999999999LL` |
| `float` | 单精度浮点 | 4字节 | `3.14f` |
| `double` | 双精度浮点 | 8字节 | `3.14159265` |

### 1.2 类型修饰符

- `signed` / `unsigned`：控制是否有符号
- `short` / `long`：控制大小
- `const`：常量，值不可修改

```cpp
unsigned int positive_only = 42;      // 只能存储非负数
const double PI = 3.14159265358979;   // 常量，不可修改
long long big_number = 9223372036854775807LL;
```

### 1.3 类型别名（现代C++推荐）

```cpp
// C++11 推荐使用 using
using Integer = int;
using StringPtr = std::string*;

// 传统方式（不推荐）
typedef int Integer;
```

### 1.4 自动类型推断（C++11）

```cpp
auto x = 42;          // x 是 int
auto y = 3.14;        // y 是 double
auto s = "hello";     // s 是 const char*
auto str = std::string("hello");  // str 是 std::string
```

> 💡 **现代C++提示**：`auto` 是C++11引入的重要特性，可以让编译器自动推断变量类型。在后续章节中会详细讨论。

---

## 2. 变量与常量

### 2.1 变量声明与初始化

C++提供多种初始化方式：

```cpp
// 传统方式
int a = 10;
int b(20);           // 直接使用值初始化，类似调用构造函数

// C++11 统一初始化（推荐）
int c{30};           // 列表初始化
int d = {40};        // 拷贝列表初始化

// 列表初始化的优势：防止窄化转换
int e{3.14};         // 错误！不允许 double 到 int 的窄化
int f = 3.14;        // 允许，但丢失精度（不推荐）
```

### 2.2 常量

```cpp
// const 常量
const int MAX_SIZE = 100;

// constexpr 编译期常量（C++11）
constexpr int ARRAY_SIZE = 50;
constexpr double PI = 3.14159265358979;

// const vs constexpr
const int runtime_const = get_value();      // 运行时确定
constexpr int compile_const = 100;          // 必须编译期确定
```

### 2.3 作用域

```cpp
int global_var = 100;  // 全局变量

void example() {
    int local_var = 10;  // 局部变量
    
    {
        int block_var = 20;  // 块作用域变量
        // 可以访问 local_var 和 global_var
    }
    // block_var 在这里不可访问
    
    static int static_var = 0;  // 静态局部变量，函数调用间保持值
    static_var++;
}
```

---

## 3. 运算符

### 3.1 算术运算符

```cpp
int a = 10, b = 3;

int sum = a + b;      // 13
int diff = a - b;     // 7
int prod = a * b;     // 30
int quot = a / b;     // 3 (整数除法)
int rem = a % b;      // 1 (取模)

// 注意浮点除法
double precise = 10.0 / 3.0;  // 3.333...
```

### 3.2 比较运算符

```cpp
a == b   // 等于
a != b   // 不等于
a < b    // 小于
a > b    // 大于
a <= b   // 小于等于
a >= b   // 大于等于

// C++20 三路比较运算符
auto result = a <=> b;  // 整数比较返回 std::strong_ordering；浮点比较返回 std::partial_ordering
```

### 3.3 逻辑运算符

```cpp
bool x = true, y = false;

x && y   // 逻辑与 (false)
x || y   // 逻辑或 (true)
!x       // 逻辑非 (false)

// 短路求值
if (ptr != nullptr && ptr->value > 0) {
    // 如果 ptr 为空，不会执行 ptr->value
}
```

### 3.4 位运算符

```cpp
int a = 0b1010;  // 10 (二进制字面量，C++14)
int b = 0b1100;  // 12

a & b    // 0b1000 = 8  (按位与)
a | b    // 0b1110 = 14 (按位或)
a ^ b    // 0b0110 = 6  (按位异或)
~a       // 按位取反
a << 2   // 0b101000 = 40 (左移)
a >> 1   // 0b0101 = 5    (右移)
```

### 3.5 复合赋值运算符

```cpp
int x = 10;
x += 5;   // x = x + 5
x -= 3;   // x = x - 3
x *= 2;   // x = x * 2
x /= 4;   // x = x / 4
x %= 3;   // x = x % 3
x <<= 1;  // x = x << 1
x >>= 1;  // x = x >> 1
```

### 3.6 自增自减

```cpp
int i = 5;

++i;  // 前置：先加1，再使用 (i = 6)
i++;  // 后置：先使用，再加1 (i = 7)

// 在循环中通常使用前置（效率略高）
for (int j = 0; j < 10; ++j) {
    // ...
}
```

---

## 4. 控制流

### 4.1 条件语句

```cpp
// if-else
if (score >= 90) {
    grade = 'A';
} else if (score >= 80) {
    grade = 'B';
} else if (score >= 70) {
    grade = 'C';
} else {
    grade = 'D';
}

// C++17 if 语句中的初始化
if (auto it = map.find(key); it != map.end()) {
    // 使用 it
    std::cout << it->second << std::endl;
}
// it 在此作用域外不可见

// 三元运算符
int max = (a > b) ? a : b;
```

### 4.2 switch 语句

```cpp
switch (day) {
    case 1:
        std::cout << "Monday";
        break;
    case 2:
        std::cout << "Tuesday";
        break;
    case 6:
    case 7:
        std::cout << "Weekend";
        break;
    default:
        std::cout << "Weekday";
        break;
}

// C++17 属性：[[fallthrough]]
switch (value) {
    case 1:
        do_something();
        [[fallthrough]];  // 明确表示故意不break
    case 2:
        do_something_else();
        break;
}
```

### 4.3 循环语句

```cpp
// for 循环
for (int i = 0; i < 10; ++i) {
    std::cout << i << " ";
}

// 范围 for 循环（C++11，强烈推荐）
std::vector<int> numbers = {1, 2, 3, 4, 5};
for (int n : numbers) {
    std::cout << n << " ";
}

// 引用方式遍历（可修改元素）
for (int& n : numbers) {
    n *= 2;  // 每个元素翻倍
}

// const 引用（避免拷贝，不修改）
for (const auto& n : numbers) {
    std::cout << n << " ";
    n *= 2;  // 这行会报错
}

// while 循环
while (condition) {
    // ...
}

// do-while 循环（至少执行一次）
do {
    // ...
} while (condition);
```

### 4.4 跳转语句

```cpp
// break：跳出当前循环
for (int i = 0; i < 100; ++i) {
    if (i == 50) break;
}

// continue：跳过本次迭代
for (int i = 0; i < 10; ++i) {
    if (i % 2 == 0) continue;  // 跳过偶数
    std::cout << i << " ";     // 只打印奇数
}

// return：从函数返回
int calculate() {
    if (error) return -1;
    return result;
}

// goto：避免使用（除非特殊情况如跳出多层循环）
```

---

## 5. 函数

### 5.1 函数定义

```cpp
// 基本函数定义
返回类型 函数名(参数列表) {
    // 函数体
    return 返回值;
}

// 示例
int add(int a, int b) {
    return a + b;
}

// void 函数（无返回值）
void print_hello() {
    std::cout << "Hello!" << std::endl;
}
```

### 5.2 参数传递

```cpp
// 值传递（拷贝）
void by_value(int x) {
    x = 100;  // 不影响原变量
}

// 引用传递
void by_reference(int& x) {
    x = 100;  // 修改原变量
}

// const 引用（避免拷贝，不修改）
void by_const_ref(const std::string& s) {
    std::cout << s << std::endl;
    // s = "new";  // 错误：不能修改
}

// 指针传递
void by_pointer(int* x) {
    if (x != nullptr) {
        *x = 100;  // 修改原变量
    }
}

// 使用示例
int main() {
    int a = 10;
    
    by_value(a);       // a 仍为 10
    by_reference(a);   // a 变为 100
    by_pointer(&a);    // a 变为 100
    
    return 0;
}
```

### 5.3 默认参数

```cpp
// 默认参数从右向左提供
void greet(const std::string& name, 
           const std::string& greeting = "Hello",
           bool formal = false) {
    if (formal) {
        std::cout << greeting << ", Mr./Ms. " << name << std::endl;
    } else {
        std::cout << greeting << ", " << name << std::endl;
    }
}

// 调用
greet("Alice");                    // Hello, Alice
greet("Bob", "Hi");                // Hi, Bob
greet("Charlie", "Good day", true); // Good day, Mr./Ms. Charlie
```

### 5.4 函数重载

```cpp
// 同名函数，不同参数
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

std::string add(const std::string& a, const std::string& b) {
    return a + b;
}

// 编译器根据参数类型选择正确的重载
add(1, 2);           // 调用 int 版本
add(1.5, 2.5);       // 调用 double 版本
add("Hello", "!");   // 调用 string 版本
```

### 5.5 内联函数

```cpp
// 建议编译器内联展开（减少函数调用开销）
inline int square(int x) {
    return x * x;
}

// 现代C++中，定义在类内的成员函数隐式内联
// constexpr 函数隐式内联
```

### 5.6 尾置返回类型（C++11）

```cpp
// 传统方式
int add(int a, int b) {
    return a + b;
}

// 尾置返回类型（在模板中特别有用）
auto add(int a, int b) -> int {
    return a + b;
}

// 配合 decltype 使用
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// C++14：可以省略尾置返回类型
auto add(int a, int b) {
    return a + b;  // 编译器自动推断返回类型
}
```

---

## 6. 命名空间

### 6.1 基本使用

```cpp
// 定义命名空间
namespace MyLib {
    int value = 42;
    
    void print() {
        std::cout << "MyLib::print()" << std::endl;
    }
    
    namespace Inner {
        void nested_function() {
            std::cout << "Nested!" << std::endl;
        }
    }
}

// 使用命名空间成员
int main() {
    // 完全限定名
    std::cout << MyLib::value << std::endl;
    MyLib::print();
    MyLib::Inner::nested_function();
    
    // using 声明（引入单个名称）
    using MyLib::print;
    print();  // 不需要 MyLib:: 前缀
    
    // using 指令（引入整个命名空间，谨慎使用）
    using namespace MyLib;
    std::cout << value << std::endl;
    
    return 0;
}
```

### 6.2 匿名命名空间

```cpp
// 匿名命名空间中的内容只在当前编译单元可见
// 相当于 static 的现代替代
namespace {
    int internal_value = 100;
    
    void internal_function() {
        // 只能在当前文件中使用
    }
}
```

### 6.3 命名空间别名（C++11）

```cpp
namespace Very_Long_Namespace_Name {
    void do_something() {}
}

// 创建别名
namespace VL = Very_Long_Namespace_Name;
VL::do_something();  // 使用别名
```

---

## 7. 输入输出

### 7.1 标准输入输出

```cpp
#include <iostream>
#include <string>

int main() {
    // 输出
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Value: " << 42 << ", Pi: " << 3.14 << '\n';
    
    // 输入
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;
    
    // 读取整行
    std::string line;
    std::getline(std::cin, line);
    
    // 错误输出
    std::cerr << "Error occurred!" << std::endl;
    
    return 0;
}
```

### 7.2 格式化输出

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double pi = 3.14159265358979;
    
    // 设置精度
    std::cout << std::setprecision(4) << pi << std::endl;  // 3.142
    
    // 固定小数点
    std::cout << std::fixed << std::setprecision(2) << pi << std::endl;  // 3.14
    
    // 设置宽度和填充
    std::cout << std::setw(10) << std::setfill('0') << 42 << std::endl;  // 0000000042
    
    // 十六进制、八进制
    std::cout << std::hex << 255 << std::endl;  // ff
    std::cout << std::oct << 64 << std::endl;   // 100
    std::cout << std::dec << 42 << std::endl;   // 42（恢复十进制）
    
    return 0;
}
```

### 7.3 C++20 格式化库（std::format）

```cpp
#include <format>  // C++20
#include <iostream>
#include <string>

int main() {
    std::string name = "Alice";
    int age = 30;
    double score = 95.5;
    
    // 类似 Python 的格式化
    std::string msg = std::format("Name: {}, Age: {}, Score: {:.1f}", 
                                   name, age, score);
    std::cout << msg << std::endl;
    // 输出：Name: Alice, Age: 30, Score: 95.5
    
    // 位置参数
    std::cout << std::format("{1} is {0} years old", age, name) << std::endl;
    // 输出：Alice is 30 years old
    
    return 0;
}
```

---

### 7.4 C++20 的 UTF-8 字面量

C++20 的 `u8"hello"` 使用 `char8_t`，不能再隐式当作 `const char*` 传给旧接口，也不能直接用 `std::cout` 输出。`std::u8string` 保存 UTF-8 码元；与接受 `char` 的接口交互时需要明确编码和转换方式。本教程的控制台示例使用普通字符串字面量，并在 MSVC 编译时启用 `/utf-8`。

```cpp
#include <string>

std::u8string utf8 = u8"你好";
// const char* text = u8"hello";  // C++20 中类型不匹配
```

---

## 📝 练习题

### 练习1：温度转换
编写一个函数，将摄氏温度转换为华氏温度。公式：F = C × 9/5 + 32

### 练习2：判断闰年
编写一个函数，判断给定年份是否为闰年。

### 练习3：斐波那契数列
使用循环打印前20个斐波那契数。

### 练习4：统计字符
编写程序，统计用户输入字符串中的字母、数字和空格数量。

---

## 💡 要点总结

1. **优先使用现代初始化方式**：使用 `{}` 进行列表初始化
2. **使用 `auto` 简化代码**：让编译器推断类型
3. **优先使用 `constexpr`**：对于编译期常量
4. **使用范围 for 循环**：遍历容器更安全、更清晰
5. **参数传递原则**：
   - 基本类型：值传递
   - 大对象（只读）：const 引用
   - 需要修改：非const引用
6. **避免 `using namespace std;`**：在头文件中尤其危险

---

## ⏭️ 下一节

[1.2 指针与引用](./02_pointers_references.md) - 深入理解C++的核心概念

