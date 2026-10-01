/**
 * @file error_demo.cpp
 * @brief 现代错误处理示例
 * 
 * 编译：g++ -std=c++20 -Wall -o error_demo error_demo.cpp
 * 运行：./error_demo
 */

#include <iostream>
#include <climits>
#include <optional>
#include <variant>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

// ============================================================
// 1. 异常处理基础
// ============================================================

double divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("除数不能为零");
    }
    return a / b;
}

void demo_exceptions() {
    std::cout << "\n=== 异常处理基础 ===\n";
    
    try {
        std::cout << "10 / 2 = " << divide(10, 2) << "\n";
        std::cout << "10 / 0 = " << divide(10, 0) << "\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "捕获 invalid_argument: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "捕获 exception: " << e.what() << "\n";
    } catch (...) {
        std::cout << "捕获未知异常\n";
    }
}

// ============================================================
// 2. 自定义异常
// ============================================================

class NetworkError : public std::runtime_error {
public:
    int error_code;
    
    NetworkError(const std::string& msg, int code)
        : std::runtime_error(msg), error_code(code) {}
};

void connect_to_server(bool should_fail) {
    if (should_fail) {
        throw NetworkError("连接被拒绝", 111);
    }
    std::cout << "连接成功\n";
}

void demo_custom_exception() {
    std::cout << "\n=== 自定义异常 ===\n";
    
    try {
        connect_to_server(true);
    } catch (const NetworkError& e) {
        std::cout << "网络错误 [" << e.error_code << "]: " << e.what() << "\n";
    }
}

// ============================================================
// 3. noexcept
// ============================================================

void may_throw() {
    throw std::runtime_error("抛出异常");
}

void no_throw() noexcept {
    // 保证不抛出异常
    // 如果抛出，程序会调用 std::terminate
}

void demo_noexcept() {
    std::cout << "\n=== noexcept ===\n";
    
    std::cout << "may_throw() noexcept: " << std::boolalpha << noexcept(may_throw()) << "\n";
    std::cout << "no_throw() noexcept: " << noexcept(no_throw()) << "\n";
}

// ============================================================
// 4. std::optional (C++17)
// ============================================================

std::optional<int> find_index(const std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] == target) {
            return static_cast<int>(i);
        }
    }
    return std::nullopt;
}

std::optional<int> parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;
    }
}

void demo_optional() {
    std::cout << "\n=== std::optional (C++17) ===\n";
    
    std::vector<int> v = {10, 20, 30, 40, 50};
    
    // 查找存在的元素
    auto idx1 = find_index(v, 30);
    if (idx1) {
        std::cout << "找到 30 在索引: " << *idx1 << "\n";
    }
    
    // 查找不存在的元素
    auto idx2 = find_index(v, 99);
    if (!idx2) {
        std::cout << "未找到 99\n";
    }
    
    // value_or 提供默认值
    int idx3 = find_index(v, 100).value_or(-1);
    std::cout << "find_index(v, 100).value_or(-1) = " << idx3 << "\n";
    
    // 解析整数
    auto num1 = parse_int("42");
    auto num2 = parse_int("abc");
    
    std::cout << "parse_int(\"42\"): " << (num1 ? std::to_string(*num1) : "无效") << "\n";
    std::cout << "parse_int(\"abc\"): " << (num2 ? std::to_string(*num2) : "无效") << "\n";
}

// ============================================================
// 5. optional 的更多用法
// ============================================================

struct Config {
    std::optional<int> timeout;
    std::optional<std::string> host;
    int port = 8080;
};

void demo_optional_advanced() {
    std::cout << "\n=== optional 高级用法 ===\n";
    
    Config cfg;
    cfg.host = "localhost";
    // timeout 未设置
    
    std::cout << "host: " << cfg.host.value_or("未设置") << "\n";
    std::cout << "timeout: " << cfg.timeout.value_or(30) << " (默认)\n";
    std::cout << "port: " << cfg.port << "\n";
    
    // emplace
    std::optional<std::string> opt;
    opt.emplace("Hello");
    std::cout << "emplace 后: " << *opt << "\n";
    
    // reset
    opt.reset();
    std::cout << "reset 后 has_value: " << std::boolalpha << opt.has_value() << "\n";
}

// ============================================================
// 6. C++20 错误处理：用 variant 表示值或错误（variant 自 C++17 引入）
// ============================================================

enum class ParseError {
    Empty,
    InvalidFormat,
    OutOfRange
};

std::string error_to_string(ParseError e) {
    switch (e) {
        case ParseError::Empty: return "空字符串";
        case ParseError::InvalidFormat: return "格式无效";
        case ParseError::OutOfRange: return "超出范围";
    }
    return "未知错误";
}

// 简单的 Result 类型
template<typename T, typename E>
using Result = std::variant<T, E>;

template<typename T, typename E>
bool is_ok(const Result<T, E>& r) {
    return std::holds_alternative<T>(r);
}

template<typename T, typename E>
const T& get_value(const Result<T, E>& r) {
    return std::get<T>(r);
}

template<typename T, typename E>
const E& get_error(const Result<T, E>& r) {
    return std::get<E>(r);
}

Result<int, ParseError> safe_parse_int(const std::string& s) {
    if (s.empty()) {
        return ParseError::Empty;
    }
    
    try {
        size_t pos;
        long value = std::stol(s, &pos);
        if (pos != s.length()) {
            return ParseError::InvalidFormat;
        }
        if (value < INT_MIN || value > INT_MAX) {
            return ParseError::OutOfRange;
        }
        return static_cast<int>(value);
    } catch (const std::invalid_argument&) {
        return ParseError::InvalidFormat;
    } catch (const std::out_of_range&) {
        return ParseError::OutOfRange;
    }
}

void demo_variant_result() {
    std::cout << "\n=== 使用 variant 模拟 Result ===\n";
    
    std::vector<std::string> inputs = {"42", "", "abc", "999999999999999"};
    
    for (const auto& input : inputs) {
        auto result = safe_parse_int(input);
        
        std::cout << "parse(\"" << input << "\"): ";
        if (is_ok(result)) {
            std::cout << "成功 = " << get_value(result) << "\n";
        } else {
            std::cout << "失败 = " << error_to_string(get_error(result)) << "\n";
        }
    }
}

// ============================================================
// 7. 异常安全保证
// ============================================================

class Resource {
public:
    Resource() { std::cout << "  [Resource] 创建\n"; }
    ~Resource() { std::cout << "  [Resource] 销毁\n"; }
};

void risky_operation(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("操作失败");
    }
}

void demo_exception_safety() {
    std::cout << "\n=== 异常安全保证 ===\n";
    
    std::cout << "-- 使用智能指针保证异常安全 --\n";
    try {
        auto res = std::make_unique<Resource>();
        risky_operation(true);
        // 即使抛出异常，res 也会被正确释放
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }
    std::cout << "资源已正确释放\n";
}

// ============================================================
// 8. 错误处理最佳实践
// ============================================================

// 使用 optional 的查找函数
std::optional<std::string> find_user_name(int id) {
    static std::vector<std::pair<int, std::string>> users = {
        {1, "Alice"}, {2, "Bob"}, {3, "Charlie"}
    };
    
    for (const auto& [uid, name] : users) {
        if (uid == id) {
            return name;
        }
    }
    return std::nullopt;
}

// 使用异常的验证函数
void validate_age(int age) {
    if (age < 0) {
        throw std::invalid_argument("年龄不能为负");
    }
    if (age > 150) {
        throw std::invalid_argument("年龄不合理");
    }
}

void demo_best_practices() {
    std::cout << "\n=== 错误处理最佳实践 ===\n";
    
    // optional：结果可能不存在
    std::cout << "-- 使用 optional --\n";
    for (int id : {1, 2, 99}) {
        auto name = find_user_name(id);
        std::cout << "用户 " << id << ": " << name.value_or("未找到") << "\n";
    }
    
    // 异常：编程错误或异常情况
    std::cout << "\n-- 使用异常 --\n";
    for (int age : {25, -5, 200}) {
        try {
            validate_age(age);
            std::cout << "年龄 " << age << ": 有效\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "年龄 " << age << ": " << e.what() << "\n";
        }
    }
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "        现代错误处理示例\n";
    std::cout << "========================================\n";
    
    demo_exceptions();
    demo_custom_exception();
    demo_noexcept();
    demo_optional();
    demo_optional_advanced();
    demo_variant_result();
    demo_exception_safety();
    demo_best_practices();
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

