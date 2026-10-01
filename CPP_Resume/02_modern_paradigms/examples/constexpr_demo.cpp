/**
 * @file constexpr_demo.cpp
 * @brief 编译期计算示例
 * 
 * 编译：g++ -std=c++20 -Wall -o constexpr_demo constexpr_demo.cpp
 * 运行：./constexpr_demo
 */

#include <iostream>
#include <array>
#include <string_view>
#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// 1. constexpr 变量
// ============================================================

constexpr int MAX_SIZE = 100;
constexpr double PI = 3.14159265358979;
constexpr int DOUBLED = MAX_SIZE * 2;

void demo_constexpr_variables() {
    std::cout << "\n=== constexpr 变量 ===\n";
    
    std::cout << "MAX_SIZE = " << MAX_SIZE << "\n";
    std::cout << "PI = " << PI << "\n";
    std::cout << "DOUBLED = " << DOUBLED << "\n";
    
    // 用于数组大小
    int arr[MAX_SIZE];
    std::cout << "arr 大小: " << sizeof(arr) / sizeof(arr[0]) << "\n";
    
    // 用于 std::array
    std::array<int, MAX_SIZE> std_arr;
    std::cout << "std_arr 大小: " << std_arr.size() << "\n";
}

// ============================================================
// 2. constexpr 函数
// ============================================================

constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

constexpr bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

void demo_constexpr_functions() {
    std::cout << "\n=== constexpr 函数 ===\n";
    
    // 编译期计算
    constexpr int fact5 = factorial(5);
    constexpr int fib10 = fibonacci(10);
    constexpr bool prime7 = is_prime(7);
    constexpr bool prime9 = is_prime(9);
    
    std::cout << "5! = " << fact5 << "\n";
    std::cout << "fib(10) = " << fib10 << "\n";
    std::cout << "7 是质数: " << std::boolalpha << prime7 << "\n";
    std::cout << "9 是质数: " << prime9 << "\n";
    
    // static_assert 验证
    static_assert(factorial(5) == 120, "阶乘计算错误");
    static_assert(fibonacci(10) == 55, "斐波那契计算错误");
    static_assert(is_prime(7), "7 应该是质数");
    
    // 也可以在运行时使用
    int n = 8;
    std::cout << n << "! = " << factorial(n) << " (运行时)\n";
}

// ============================================================
// 3. constexpr 类
// ============================================================

class Point {
public:
    int x, y;
    
    constexpr Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    constexpr Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    constexpr Point operator*(int scalar) const {
        return Point(x * scalar, y * scalar);
    }
    
    constexpr int distance_squared(const Point& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return dx * dx + dy * dy;
    }
};

void demo_constexpr_class() {
    std::cout << "\n=== constexpr 类 ===\n";
    
    constexpr Point p1(3, 4);
    constexpr Point p2(1, 2);
    constexpr Point p3 = p1 + p2;
    constexpr Point p4 = p1 * 2;
    constexpr int dist = p1.distance_squared(p2);
    
    std::cout << "p1 = (" << p1.x << ", " << p1.y << ")\n";
    std::cout << "p2 = (" << p2.x << ", " << p2.y << ")\n";
    std::cout << "p1 + p2 = (" << p3.x << ", " << p3.y << ")\n";
    std::cout << "p1 * 2 = (" << p4.x << ", " << p4.y << ")\n";
    std::cout << "p1 到 p2 距离平方 = " << dist << "\n";
    
    static_assert(p3.x == 4 && p3.y == 6);
    static_assert(p4.x == 6 && p4.y == 8);
}

// ============================================================
// 4. 编译期数组
// ============================================================

constexpr std::array<int, 10> create_squares() {
    std::array<int, 10> arr{};
    for (int i = 0; i < 10; ++i) {
        arr[i] = i * i;
    }
    return arr;
}

constexpr std::array<int, 10> create_fibonacci_table() {
    std::array<int, 10> arr{};
    arr[0] = 0;
    arr[1] = 1;
    for (int i = 2; i < 10; ++i) {
        arr[i] = arr[i-1] + arr[i-2];
    }
    return arr;
}

void demo_constexpr_arrays() {
    std::cout << "\n=== 编译期数组 ===\n";
    
    constexpr auto squares = create_squares();
    constexpr auto fibs = create_fibonacci_table();
    
    std::cout << "平方表: ";
    for (int x : squares) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    std::cout << "斐波那契表: ";
    for (int x : fibs) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    static_assert(squares[5] == 25);
    static_assert(fibs[9] == 34);
}

// ============================================================
// 5. if constexpr (C++17)
// ============================================================

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

void demo_if_constexpr() {
    std::cout << "\n=== if constexpr ===\n";
    
    auto int_result = process(10);
    auto double_result = process(10.0);
    auto string_result = process(std::string("hello"));
    
    std::cout << "process(10) = " << int_result << " (int * 2)\n";
    std::cout << "process(10.0) = " << double_result << " (double / 2)\n";
    std::cout << "process(\"hello\") = " << string_result << " (原样返回)\n";
}

// ============================================================
// 6. constexpr 字符串处理
// ============================================================

constexpr size_t string_length(const char* s) {
    size_t len = 0;
    while (s[len] != '\0') ++len;
    return len;
}

constexpr bool string_equal(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        ++a;
        ++b;
    }
    return *a == *b;
}

constexpr unsigned int string_hash(const char* s) {
    unsigned int hash = 0;
    while (*s) {
        hash = hash * 31 + static_cast<unsigned int>(*s);
        ++s;
    }
    return hash;
}

void demo_constexpr_strings() {
    std::cout << "\n=== constexpr 字符串处理 ===\n";
    
    constexpr auto len = string_length("Hello, World!");
    constexpr auto eq = string_equal("hello", "hello");
    constexpr auto neq = string_equal("hello", "world");
    constexpr auto hash = string_hash("test");
    
    std::cout << "\"Hello, World!\" 长度: " << len << "\n";
    std::cout << "\"hello\" == \"hello\": " << std::boolalpha << eq << "\n";
    std::cout << "\"hello\" == \"world\": " << neq << "\n";
    std::cout << "hash(\"test\") = " << hash << "\n";
    
    static_assert(len == 13);
    static_assert(eq == true);
    static_assert(neq == false);
}

// ============================================================
// 7. 编译期查找表
// ============================================================

constexpr std::array<unsigned char, 256> create_lookup_table() {
    std::array<unsigned char, 256> table{};
    for (int i = 0; i < 256; ++i) {
        // 简单转换：大写 <-> 小写
        if (i >= 'a' && i <= 'z') {
            table[i] = static_cast<unsigned char>(i - 32);
        } else if (i >= 'A' && i <= 'Z') {
            table[i] = static_cast<unsigned char>(i + 32);
        } else {
            table[i] = static_cast<unsigned char>(i);
        }
    }
    return table;
}

constexpr auto case_table = create_lookup_table();

char swap_case(char c) {
    return static_cast<char>(case_table[static_cast<unsigned char>(c)]);
}

void demo_lookup_table() {
    std::cout << "\n=== 编译期查找表 ===\n";
    
    std::string s = "Hello, World!";
    std::cout << "原始: " << s << "\n";
    
    for (char& c : s) {
        c = swap_case(c);
    }
    std::cout << "转换: " << s << "\n";
    
    static_assert(case_table['A'] == 'a');
    static_assert(case_table['z'] == 'Z');
}

// ============================================================
// 8. static_assert
// ============================================================

template<typename T>
class OnlyArithmetic {
    static_assert(std::is_arithmetic_v<T>, "T 必须是算术类型");
public:
    T value;
    OnlyArithmetic(T v) : value(v) {}
};

void demo_static_assert() {
    std::cout << "\n=== static_assert ===\n";
    
    // 编译期检查
    static_assert(sizeof(int) >= 4, "int 必须至少 4 字节");
    static_assert(sizeof(char) == 1, "char 的大小按定义为 1 字节");
    
    OnlyArithmetic<int> a(42);
    OnlyArithmetic<double> b(3.14);
    // OnlyArithmetic<std::string> c("hi");  // 编译错误
    
    std::cout << "a.value = " << a.value << "\n";
    std::cout << "b.value = " << b.value << "\n";
    std::cout << "static_assert 检查通过\n";
}

// ============================================================
// 9. C++20：立即函数、常量初始化和编译期容器
// ============================================================

consteval int square_immediate(int value) {
    return value * value;
}

// 必须常量初始化，但不是只读变量；只能用于静态或线程存储期变量。
constinit int request_count = square_immediate(3);

constexpr int sorted_minimum() {
    std::vector<int> values{5, 3, 8, 1};
    std::ranges::sort(values);
    return values.front();
}  // vector 的动态存储必须在本次常量求值结束前释放

constexpr int evaluation_mode() {
    // 普通 if 才能区分这一次调用的求值环境；不要写 if constexpr。
    if (std::is_constant_evaluated()) return 1;
    return 2;
}

void demo_cpp20_constexpr() {
    std::cout << "\n=== C++20 编译期计算 ===\n";
    constexpr int square = square_immediate(6);
    static_assert(square == 36);
    static_assert(sorted_minimum() == 1);
    static_assert(evaluation_mode() == 1);
    ++request_count;
    std::cout << "consteval square(6) = " << square << '\n';
    std::cout << "constinit 变量修改后 = " << request_count << '\n';
    std::cout << "运行时求值标记 = " << evaluation_mode() << '\n';
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "        编译期计算示例\n";
    std::cout << "========================================\n";
    
    demo_constexpr_variables();
    demo_constexpr_functions();
    demo_constexpr_class();
    demo_constexpr_arrays();
    demo_if_constexpr();
    demo_constexpr_strings();
    demo_lookup_table();
    demo_static_assert();
    demo_cpp20_constexpr();
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

