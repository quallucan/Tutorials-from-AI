/**
 * @file type_deduction_demo.cpp
 * @brief 类型推断示例
 * 
 * 编译：g++ -std=c++20 -Wall -o type_deduction_demo type_deduction_demo.cpp
 * 运行：./type_deduction_demo
 */

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <type_traits>

// 辅助宏：打印类型信息
#define PRINT_TYPE(x) std::cout << "  " #x " 类型: " << typeid(x).name() << "\n"

// ============================================================
// 1. auto 基础
// ============================================================

void demo_auto_basics() {
    std::cout << "\n=== auto 基础 ===\n";
    
    auto x = 42;           // int
    auto y = 3.14;         // double
    auto z = 'c';          // char
    auto s = "hello";      // const char*
    auto str = std::string("world");  // std::string
    
    std::cout << "auto x = 42;         -> int: " << x << "\n";
    std::cout << "auto y = 3.14;       -> double: " << y << "\n";
    std::cout << "auto z = 'c';        -> char: " << z << "\n";
    std::cout << "auto s = \"hello\";    -> const char*: " << s << "\n";
    std::cout << "auto str = string(); -> string: " << str << "\n";
}

// ============================================================
// 2. auto 与引用
// ============================================================

void demo_auto_references() {
    std::cout << "\n=== auto 与引用 ===\n";
    
    int x = 10;
    const int cx = 20;
    const int& rx = x;
    
    // auto 会忽略引用和顶层 const
    auto a = x;     // int（不是引用）
    auto b = cx;    // int（忽略 const）
    auto c = rx;    // int（忽略引用和 const）
    
    a = 100;  // 不影响 x
    b = 200;  // 可以修改（不是 const）
    c = 300;  // 可以修改
    
    std::cout << "原始 x = " << x << "\n";
    std::cout << "a = x 后修改 a, x = " << x << " (不变)\n";
    
    // 显式使用引用
    auto& d = x;    // int&
    d = 500;
    std::cout << "auto& d = x 后修改 d, x = " << x << "\n";
    
    // const auto&
    const auto& e = x;  // const int&
    // e = 600;  // 错误：不能修改
    std::cout << "const auto& e = x; e = " << e << "\n";
    
    // auto&&（转发引用）
    auto&& f = x;    // int&（x 是左值）
    auto&& g = 42;   // int&&（42 是右值）
    
    std::cout << "auto&& f = x; f 是左值引用\n";
    std::cout << "auto&& g = 42; g 是右值引用\n";
}

// ============================================================
// 3. auto 与容器
// ============================================================

void demo_auto_containers() {
    std::cout << "\n=== auto 与容器 ===\n";
    
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    
    // 迭代器
    auto it = vec.begin();  // std::vector<int>::iterator
    std::cout << "vec.begin() 指向: " << *it << "\n";
    
    // 范围 for（值拷贝）
    std::cout << "值遍历: ";
    for (auto x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // 范围 for（const 引用，推荐只读）
    std::cout << "const 引用遍历: ";
    for (const auto& x : vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // map 遍历
    std::cout << "map 遍历:\n";
    for (const auto& [name, score] : scores) {  // C++17 结构化绑定
        std::cout << "  " << name << ": " << score << "\n";
    }
}

// ============================================================
// 4. decltype
// ============================================================

void demo_decltype() {
    std::cout << "\n=== decltype ===\n";
    
    int x = 10;
    const int cx = 20;
    int& rx = x;
    
    // decltype 保留引用和 const
    decltype(x) a = 0;     // int
    decltype(cx) b = 0;    // const int
    decltype(rx) c = x;    // int&
    
    std::cout << "decltype(x) -> int\n";
    std::cout << "decltype(cx) -> const int\n";
    std::cout << "decltype(rx) -> int&\n";
    
    // 表达式的 decltype
    decltype(x + 1) d = 0;   // int
    decltype(x * 1.0) e = 0; // double
    
    std::cout << "decltype(x + 1) -> int\n";
    std::cout << "decltype(x * 1.0) -> double\n";
    
    // 注意括号的影响
    decltype(x) f = 0;    // int
    decltype((x)) g = x;  // int&（注意！）
    
    g = 100;
    std::cout << "decltype((x)) 是引用，修改 g 后 x = " << x << "\n";
}

// ============================================================
// 5. decltype(auto) (C++14)
// ============================================================

int& get_ref(int& x) { return x; }
int get_value(int x) { return x; }

auto return_auto() {
    int x = 42;
    return get_ref(x);  // 返回 int（丢失引用）
}

// decltype(auto) return_decltype_auto() {
//     int x = 42;
//     return get_ref(x);  // 返回 int&（保留引用）-- 但这里会悬垂！
// }

void demo_decltype_auto() {
    std::cout << "\n=== decltype(auto) ===\n";
    
    int x = 10;
    
    // 变量推断
    auto a = get_ref(x);          // int（丢失引用）
    decltype(auto) b = get_ref(x); // int&（保留引用）
    
    a = 20;
    std::cout << "auto a = get_ref(x); a = 20; x = " << x << " (不变)\n";
    
    b = 30;
    std::cout << "decltype(auto) b = get_ref(x); b = 30; x = " << x << "\n";
}

// ============================================================
// 6. 结构化绑定 (C++17)
// ============================================================

std::pair<int, std::string> get_person() {
    return {25, "Alice"};
}

struct Point {
    int x, y;
};

void demo_structured_bindings() {
    std::cout << "\n=== 结构化绑定 (C++17) ===\n";
    
    // pair
    auto [age, name] = get_person();
    std::cout << "pair: " << name << " is " << age << " years old\n";
    
    // tuple
    std::tuple<int, double, std::string> t{1, 3.14, "hello"};
    auto [i, d, s] = t;
    std::cout << "tuple: " << i << ", " << d << ", " << s << "\n";
    
    // 数组
    int arr[] = {10, 20, 30};
    auto [a, b, c] = arr;
    std::cout << "array: " << a << ", " << b << ", " << c << "\n";
    
    // 结构体
    Point pt{100, 200};
    auto [x, y] = pt;
    std::cout << "struct: (" << x << ", " << y << ")\n";
    
    // map 遍历
    std::map<std::string, int> scores = {{"A", 90}, {"B", 80}};
    std::cout << "map:\n";
    for (const auto& [key, value] : scores) {
        std::cout << "  " << key << " -> " << value << "\n";
    }
    
    // 引用绑定（可修改）
    auto& [px, py] = pt;
    px = 500;
    py = 600;
    std::cout << "修改后 struct: (" << pt.x << ", " << pt.y << ")\n";
}

// ============================================================
// 7. CTAD (C++17)
// ============================================================

template<typename T, typename U>
class Pair {
public:
    T first;
    U second;
    Pair(T f, U s) : first(f), second(s) {}
};

template<typename T, typename U>
Pair(T, U) -> Pair<T, U>;  // 推断指南

void demo_ctad() {
    std::cout << "\n=== CTAD (C++17) ===\n";
    
    // 标准库类型
    std::pair p1{1, 3.14};        // pair<int, double>
    std::vector v1{1, 2, 3};      // vector<int>
    std::tuple t1{1, 2.0, "hi"};  // tuple<int, double, const char*>
    
    std::cout << "std::pair{1, 3.14} -> (" << p1.first << ", " << p1.second << ")\n";
    std::cout << "std::vector{1, 2, 3} -> size=" << v1.size() << "\n";
    
    // 自定义类型
    Pair p2{42, std::string("answer")};  // Pair<int, string>
    std::cout << "Pair{42, \"answer\"} -> (" << p2.first << ", " << p2.second << ")\n";
    
    // array
    std::array arr{1, 2, 3, 4, 5};  // array<int, 5>
    std::cout << "std::array{1,2,3,4,5} -> size=" << arr.size() << "\n";
}

// ============================================================
// 8. 类型特征
// ============================================================

template<typename T>
void describe_type() {
    std::cout << "  is_integral: " << std::boolalpha << std::is_integral_v<T> << "\n";
    std::cout << "  is_floating_point: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_reference: " << std::is_reference_v<T> << "\n";
    std::cout << "  is_const: " << std::is_const_v<std::remove_reference_t<T>> << "\n";
}

void demo_type_traits() {
    std::cout << "\n=== 类型特征 ===\n";
    
    std::cout << "int:\n";
    describe_type<int>();
    
    std::cout << "double:\n";
    describe_type<double>();
    
    std::cout << "int*:\n";
    describe_type<int*>();
    
    std::cout << "const int&:\n";
    describe_type<const int&>();
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "          类型推断示例\n";
    std::cout << "========================================\n";
    
    demo_auto_basics();
    demo_auto_references();
    demo_auto_containers();
    demo_decltype();
    demo_decltype_auto();
    demo_structured_bindings();
    demo_ctad();
    demo_type_traits();
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

