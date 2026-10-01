/**
 * @file template_demo.cpp
 * @brief 模板示例
 * 
 * 演示内容：
 * - 函数模板
 * - 类模板
 * - 模板特化
 * - 可变参数模板
 * - C++20 Concepts（本教程的基线特性）
 * 
 * 编译：g++ -std=c++20 -Wall -o template_demo template_demo.cpp
 * 运行：./template_demo
 */

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <concepts>
#include <limits>

// ============================================================
// 1. 函数模板基础
// ============================================================

// 简单函数模板
template<typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

// 多类型参数模板
template<typename T, typename U>
auto add(T a, U b) {
    return a + b;
}

void demo_function_templates() {
    std::cout << "=== 函数模板基础 ===" << std::endl;
    
    // 自动类型推断
    std::cout << "  max_value(3, 5) = " << max_value(3, 5) << std::endl;
    std::cout << "  max_value(3.14, 2.71) = " << max_value(3.14, 2.71) << std::endl;
    std::cout << "  max_value(\"abc\", \"xyz\") = " << max_value(std::string("abc"), std::string("xyz")) << std::endl;
    
    // 显式指定类型
    std::cout << "  max_value<double>(3, 5.5) = " << max_value<double>(3, 5.5) << std::endl;
    
    // 不同类型参数
    std::cout << "  add(1, 2.5) = " << add(1, 2.5) << std::endl;
    std::cout << "  add(\"Hello \", \"World\") = " << add(std::string("Hello "), std::string("World")) << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 2. 非类型模板参数
// ============================================================

template<typename T, int N>
class FixedArray {
private:
    T data_[N];
    
public:
    constexpr int size() const { return N; }
    
    T& operator[](int index) { return data_[index]; }
    const T& operator[](int index) const { return data_[index]; }
    
    T* begin() { return data_; }
    T* end() { return data_ + N; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + N; }
    
    void fill(const T& value) {
        for (int i = 0; i < N; ++i) {
            data_[i] = value;
        }
    }
};

template<int N>
int multiply(int x) {
    return x * N;
}

void demo_non_type_params() {
    std::cout << "=== 非类型模板参数 ===" << std::endl;
    
    FixedArray<int, 5> arr;
    arr.fill(0);
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    
    std::cout << "  FixedArray<int, 5> 大小: " << arr.size() << std::endl;
    std::cout << "  内容: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  multiply<5>(10) = " << multiply<5>(10) << std::endl;
    std::cout << "  multiply<3>(7) = " << multiply<3>(7) << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 3. 类模板
// ============================================================

template<typename T>
class Stack {
private:
    std::vector<T> data_;

public:
    void push(const T& value) {
        data_.push_back(value);
    }
    
    void pop() {
        if (!empty()) {
            data_.pop_back();
        }
    }
    
    T& top() {
        return data_.back();
    }
    
    const T& top() const {
        return data_.back();
    }
    
    bool empty() const {
        return data_.empty();
    }
    
    size_t size() const {
        return data_.size();
    }
};

void demo_class_templates() {
    std::cout << "=== 类模板 ===" << std::endl;
    
    Stack<int> int_stack;
    int_stack.push(1);
    int_stack.push(2);
    int_stack.push(3);
    
    std::cout << "  int_stack (后进先出): ";
    while (!int_stack.empty()) {
        std::cout << int_stack.top() << " ";
        int_stack.pop();
    }
    std::cout << std::endl;
    
    Stack<std::string> str_stack;
    str_stack.push("Hello");
    str_stack.push("World");
    
    std::cout << "  str_stack 顶部: " << str_stack.top() << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 4. 模板特化
// ============================================================

// 通用模板
template<typename T>
class TypeInfo {
public:
    static std::string name() {
        return "Unknown";
    }
};

// 完全特化
template<>
class TypeInfo<int> {
public:
    static std::string name() {
        return "int";
    }
};

template<>
class TypeInfo<double> {
public:
    static std::string name() {
        return "double";
    }
};

template<>
class TypeInfo<std::string> {
public:
    static std::string name() {
        return "std::string";
    }
};

// 部分特化：指针类型
template<typename T>
class TypeInfo<T*> {
public:
    static std::string name() {
        return TypeInfo<T>::name() + "*";
    }
};

// 部分特化：vector
template<typename T>
class TypeInfo<std::vector<T>> {
public:
    static std::string name() {
        return "std::vector<" + TypeInfo<T>::name() + ">";
    }
};

void demo_specialization() {
    std::cout << "=== 模板特化 ===" << std::endl;
    
    std::cout << "  TypeInfo<int>: " << TypeInfo<int>::name() << std::endl;
    std::cout << "  TypeInfo<double>: " << TypeInfo<double>::name() << std::endl;
    std::cout << "  TypeInfo<std::string>: " << TypeInfo<std::string>::name() << std::endl;
    std::cout << "  TypeInfo<int*>: " << TypeInfo<int*>::name() << std::endl;
    std::cout << "  TypeInfo<double*>: " << TypeInfo<double*>::name() << std::endl;
    std::cout << "  TypeInfo<std::vector<int>>: " << TypeInfo<std::vector<int>>::name() << std::endl;
    std::cout << "  TypeInfo<float>: " << TypeInfo<float>::name() << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 5. 可变参数模板
// ============================================================

// 基础情况
void print() {
    std::cout << std::endl;
}

// 递归情况
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    print(rest...);
}

// C++17 折叠表达式
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // 一元左折叠
}

template<typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);  // 逗号折叠
    std::cout << std::endl;
}

// 检查所有参数是否为正数
template<typename... Args>
bool all_positive(Args... args) {
    return (... && (args > 0));
}

void demo_variadic_templates() {
    std::cout << "=== 可变参数模板 ===" << std::endl;
    
    std::cout << "  print(1, 2.5, \"hello\", 'c'): ";
    print(1, 2.5, "hello", 'c');
    
    std::cout << "  sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "  sum(1.1, 2.2, 3.3) = " << sum(1.1, 2.2, 3.3) << std::endl;
    
    std::cout << "  print_all(\"A\", \"B\", \"C\"): ";
    print_all("A", "B", "C");
    
    std::cout << "  all_positive(1, 2, 3): " << std::boolalpha << all_positive(1, 2, 3) << std::endl;
    std::cout << "  all_positive(1, -2, 3): " << all_positive(1, -2, 3) << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 6. SFINAE 与 enable_if
// ============================================================

// 只对整数类型启用
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
safe_divide(T a, T b) {
    if (b == 0) {
        std::cout << "  警告: 除以零，返回0" << std::endl;
        return 0;
    }
    return a / b;
}

// 只对浮点类型启用
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
safe_divide(T a, T b) {
    if (b == 0.0) {
        std::cout << "  警告: 除以零，返回无穷大" << std::endl;
        return std::numeric_limits<T>::infinity();
    }
    return a / b;
}

void demo_sfinae() {
    std::cout << "=== SFINAE 与 enable_if ===" << std::endl;
    
    std::cout << "  safe_divide(10, 3) = " << safe_divide(10, 3) << std::endl;
    std::cout << "  safe_divide(10, 0) = " << safe_divide(10, 0) << std::endl;
    std::cout << "  safe_divide(10.0, 3.0) = " << safe_divide(10.0, 3.0) << std::endl;
    std::cout << "  safe_divide(10.0, 0.0) = " << safe_divide(10.0, 0.0) << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 7. C++20 Concepts
// ============================================================

// 定义概念
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept Printable = requires(T a) {
    { std::cout << a } -> std::same_as<std::ostream&>;
};

// 使用概念约束
template<Numeric T>
T square(T x) {
    return x * x;
}

// requires 子句
template<typename T>
    requires Printable<T>
void print_value(const T& value) {
    std::cout << "  值: " << value << std::endl;
}

// 简洁语法
auto multiply_numeric(Numeric auto a, Numeric auto b) {
    return a * b;
}

void demo_concepts() {
    std::cout << "=== C++20 Concepts ===" << std::endl;
    
    std::cout << "  square(5) = " << square(5) << std::endl;
    std::cout << "  square(3.14) = " << square(3.14) << std::endl;
    
    print_value(42);
    print_value("Hello");
    print_value(3.14159);
    
    std::cout << "  multiply_numeric(3, 4) = " << multiply_numeric(3, 4) << std::endl;
    std::cout << "  multiply_numeric(2.5, 4.0) = " << multiply_numeric(2.5, 4.0) << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 8. 类模板参数推断 (CTAD) - C++17
// ============================================================

template<typename T, typename U>
class Pair {
public:
    T first;
    U second;
    
    Pair(T f, U s) : first(f), second(s) {}
    
    void print() const {
        std::cout << "  Pair(" << first << ", " << second << ")" << std::endl;
    }
};

// 推断指南（可选，但有助于自定义推断）
template<typename T, typename U>
Pair(T, U) -> Pair<T, U>;

void demo_ctad() {
    std::cout << "=== 类模板参数推断 (CTAD) ===" << std::endl;
    
    // C++17 之前需要这样写
    Pair<int, std::string> p1(1, "one");
    p1.print();
    
    // C++17 可以自动推断
    Pair p2(2, 3.14);  // Pair<int, double>
    p2.print();
    
    Pair p3(std::string("hello"), 42);  // Pair<std::string, int>
    p3.print();
    
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "        C++ 模板示例程序" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    demo_function_templates();
    demo_non_type_params();
    demo_class_templates();
    demo_specialization();
    demo_variadic_templates();
    demo_sfinae();
    demo_concepts();
    demo_ctad();
    
    std::cout << "========================================" << std::endl;
    std::cout << "            示例结束" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

