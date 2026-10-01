/**
 * @file modern_features_demo.cpp
 * @brief C++20 日常语言与标准库特性
 * 编译：g++ -std=c++20 -Wall -Wextra -o modern_features_demo modern_features_demo.cpp
 * 运行：./modern_features_demo
 */

#include <array>
#include <bit>
#include <cassert>
#include <compare>
#include <format>
#include <iostream>
#include <numbers>
#include <numeric>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <vector>

struct Point {
    int x;
    int y;
    // 默认化 <=> 按成员声明顺序比较，并隐式声明默认化的 ==。
    auto operator<=>(const Point&) const = default;
};

int sum(std::span<const int> values) {
    // span 只借用连续内存，不拥有元素，也不延长其生命周期。
    return std::accumulate(values.begin(), values.end(), 0);
}

void log(std::string_view message,
         std::source_location where = std::source_location::current()) {
    std::cout << std::format("{}:{} {}\n", where.file_name(), where.line(), message);
}

int main() {
    constexpr Point a{.x = 1, .y = 2};  // 指定初始化按成员声明顺序书写
    constexpr Point b{.x = 1, .y = 3};
    static_assert(a < b && a != b);

    int raw[] = {1, 2, 3};
    std::array fixed{4, 5, 6};
    std::vector dynamic{7, 8, 9};
    assert(sum(raw) == 6);
    assert(sum(fixed) == 15);
    assert(sum(dynamic) == 24);
    assert(sum(std::span<const int>{}) == 0);
    std::span<int> borrowed = dynamic;
    borrowed.front() = 10;  // 修改原容器；不能在容器重分配后继续使用此 span
    assert(dynamic.front() == 10);

    const std::string filename = "lesson.cpp";
    assert(filename.starts_with("lesson") && filename.ends_with(".cpp"));
    static_assert(std::popcount(0b101101u) == 4);
    static_assert(std::has_single_bit(16u));

    const auto message = std::format("C++{}: sum={}, pi={:.3f}",
                                     20, sum(raw), std::numbers::pi);
    assert(message == "C++20: sum=6, pi=3.142");
    std::cout << message << '\n';
    log("span、三路比较、指定初始化与格式化示例完成");
}
