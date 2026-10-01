/**
 * @file lambda_demo.cpp
 * @brief Lambda 与函数式编程示例
 * 
 * 编译：g++ -std=c++20 -Wall -o lambda_demo lambda_demo.cpp
 * 运行：./lambda_demo
 */

#include <iostream>
#include <concepts>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

// ============================================================
// 1. Lambda 基础语法
// ============================================================

void demo_lambda_basics() {
    std::cout << "\n=== Lambda 基础语法 ===\n";
    
    // 最简单的 Lambda
    auto hello = []() { std::cout << "  Hello, Lambda!\n"; };
    hello();
    
    // 带参数
    auto add = [](int a, int b) { return a + b; };
    std::cout << "  add(3, 5) = " << add(3, 5) << "\n";
    
    // 显式返回类型
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };
    std::cout << "  divide(10, 3) = " << divide(10, 3) << "\n";
    
    // 立即调用
    int result = [](int x) { return x * x; }(7);
    std::cout << "  立即调用: 7^2 = " << result << "\n";
}

// ============================================================
// 2. 捕获列表
// ============================================================

void demo_captures() {
    std::cout << "\n=== 捕获列表 ===\n";
    
    int x = 10, y = 20;
    
    // 值捕获
    auto by_value = [x, y]() {
        std::cout << "  值捕获: x=" << x << ", y=" << y << "\n";
    };
    x = 100;  // 修改不影响 lambda 内的值
    by_value();  // 仍然输出 10, 20
    
    // 引用捕获
    auto by_ref = [&x, &y]() {
        std::cout << "  引用捕获: x=" << x << ", y=" << y << "\n";
        x = 200;  // 可以修改外部变量
    };
    by_ref();
    std::cout << "  修改后 x = " << x << "\n";
    
    // 混合捕获
    int a = 1, b = 2;
    auto mixed = [a, &b]() {
        std::cout << "  混合捕获: a=" << a << ", b=" << b << "\n";
    };
    mixed();
    
    // 全部值捕获
    auto all_by_value = [=]() {
        std::cout << "  全部值捕获: x=" << x << ", a=" << a << "\n";
    };
    all_by_value();
    
    // 全部引用捕获
    auto all_by_ref = [&]() {
        std::cout << "  全部引用捕获: x=" << x << "\n";
    };
    all_by_ref();
}

// ============================================================
// 3. mutable Lambda
// ============================================================

void demo_mutable() {
    std::cout << "\n=== mutable Lambda ===\n";
    
    int counter = 0;
    
    // 默认不能修改值捕获的变量
    auto inc = [counter]() mutable {
        ++counter;  // mutable 允许修改
        return counter;
    };
    
    std::cout << "  inc() = " << inc() << "\n";  // 1
    std::cout << "  inc() = " << inc() << "\n";  // 2
    std::cout << "  inc() = " << inc() << "\n";  // 3
    std::cout << "  外部 counter = " << counter << " (未改变)\n";
}

// ============================================================
// 4. 初始化捕获 (C++14)
// ============================================================

void demo_init_capture() {
    std::cout << "\n=== 初始化捕获 (C++14) ===\n";
    
    // 移动捕获
    auto ptr = std::make_unique<int>(42);
    auto lambda = [p = std::move(ptr)]() {
        std::cout << "  移动捕获的值: " << *p << "\n";
    };
    lambda();
    std::cout << "  ptr " << (ptr ? "非空" : "已移动") << "\n";
    
    // 自定义名称
    int x = 10;
    auto custom = [doubled = x * 2]() {
        std::cout << "  doubled = " << doubled << "\n";
    };
    custom();
}

// ============================================================
// 5. 泛型 Lambda (C++14)
// ============================================================

void demo_generic_lambda() {
    std::cout << "\n=== 泛型 Lambda (C++14) ===\n";
    
    auto print = [](const auto& x) {
        std::cout << "  print: " << x << "\n";
    };
    
    print(42);
    print(3.14);
    print("Hello");
    print(std::string("World"));
    
    // 泛型算术
    auto multiply = [](auto a, auto b) { return a * b; };
    std::cout << "  multiply(3, 4) = " << multiply(3, 4) << "\n";
    std::cout << "  multiply(2.5, 3.0) = " << multiply(2.5, 3.0) << "\n";
}

// ============================================================
// 6. Lambda 与 STL 算法
// ============================================================

void demo_stl_algorithms() {
    std::cout << "\n=== Lambda 与 STL 算法 ===\n";
    
    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // for_each
    std::cout << "  原始数据: ";
    std::for_each(nums.begin(), nums.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cout << "\n";
    
    // sort
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;  // 降序
    });
    std::cout << "  降序排序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    
    // find_if
    auto it = std::find_if(nums.begin(), nums.end(), [](int n) {
        return n < 5;
    });
    if (it != nums.end()) {
        std::cout << "  第一个 < 5 的数: " << *it << "\n";
    }
    
    // count_if
    int even_count = std::count_if(nums.begin(), nums.end(), [](int n) {
        return n % 2 == 0;
    });
    std::cout << "  偶数个数: " << even_count << "\n";
    
    // transform
    std::vector<int> squares(nums.size());
    std::transform(nums.begin(), nums.end(), squares.begin(), [](int n) {
        return n * n;
    });
    std::cout << "  平方: ";
    for (int n : squares) std::cout << n << " ";
    std::cout << "\n";
    
    // accumulate
    int sum = std::accumulate(nums.begin(), nums.end(), 0, [](int acc, int n) {
        return acc + n;
    });
    std::cout << "  总和: " << sum << "\n";
}

// ============================================================
// 7. std::function
// ============================================================

void demo_std_function() {
    std::cout << "\n=== std::function ===\n";
    
    // 存储 Lambda
    std::function<int(int, int)> op;
    
    op = [](int a, int b) { return a + b; };
    std::cout << "  op(3, 5) = " << op(3, 5) << " (加法)\n";
    
    op = [](int a, int b) { return a * b; };
    std::cout << "  op(3, 5) = " << op(3, 5) << " (乘法)\n";
    
    // 回调函数
    auto apply = [](std::function<int(int)> f, int x) {
        return f(x);
    };
    
    std::cout << "  apply(square, 7) = " << apply([](int x) { return x * x; }, 7) << "\n";
}

// ============================================================
// 8. 高阶函数
// ============================================================

void demo_higher_order() {
    std::cout << "\n=== 高阶函数 ===\n";
    
    // 返回 Lambda
    auto make_multiplier = [](int factor) {
        return [factor](int x) { return x * factor; };
    };
    
    auto times2 = make_multiplier(2);
    auto times10 = make_multiplier(10);
    
    std::cout << "  times2(5) = " << times2(5) << "\n";
    std::cout << "  times10(5) = " << times10(5) << "\n";
    
    // 函数组合
    auto compose = [](auto f, auto g) {
        return [=](auto x) { return f(g(x)); };
    };
    
    auto add1 = [](int x) { return x + 1; };
    auto square = [](int x) { return x * x; };
    
    auto add1_then_square = compose(square, add1);
    std::cout << "  (x+1)^2 where x=4: " << add1_then_square(4) << "\n";
    
    // 柯里化
    auto curry_add = [](int a) {
        return [a](int b) {
            return a + b;
        };
    };
    
    auto add5 = curry_add(5);
    std::cout << "  curry_add(5)(3) = " << add5(3) << "\n";
}

// ============================================================
// 9. 捕获 this
// ============================================================

class Counter {
    int value_ = 0;
    
public:
    auto get_incrementer() {
        // 捕获 this
        return [this]() {
            ++value_;
            return value_;
        };
    }
    
    auto get_value_copy() {
        // C++17: 捕获 *this（拷贝整个对象）
        return [*this]() {
            return value_;
        };
    }
    
    int value() const { return value_; }
};

void demo_capture_this() {
    std::cout << "\n=== 捕获 this ===\n";
    
    Counter c;
    auto inc = c.get_incrementer();
    
    std::cout << "  inc() = " << inc() << "\n";
    std::cout << "  inc() = " << inc() << "\n";
    std::cout << "  inc() = " << inc() << "\n";
    std::cout << "  c.value() = " << c.value() << "\n";
}

// ============================================================
// 10. C++20：显式模板参数与无捕获 Lambda 的默认构造
// ============================================================

void demo_cpp20_lambda() {
    std::cout << "\n=== C++20 模板 Lambda ===\n";
    auto add_same_type = []<std::integral T>(T a, T b) { return a + b; };
    std::cout << "同类型整数相加: " << add_same_type(2, 3) << '\n';
    // add_same_type(2, 3.0);  // 约束要求整数，且两个参数推导为同一个 T

    auto descending = [](int a, int b) { return a > b; };
    std::set<int, decltype(descending)> values{1, 3, 2};
    for (int value : values) std::cout << value << ' ';  // 3 2 1
    std::cout << '\n';
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "     Lambda 与函数式编程示例\n";
    std::cout << "========================================\n";
    
    demo_lambda_basics();
    demo_captures();
    demo_mutable();
    demo_init_capture();
    demo_generic_lambda();
    demo_stl_algorithms();
    demo_std_function();
    demo_higher_order();
    demo_capture_this();
    demo_cpp20_lambda();
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

