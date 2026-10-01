/**
 * @file coroutine_demo.cpp
 * @brief C++20 协程：拥有协程帧的最小整数生成器（不是 std::generator）
 * 编译：g++ -std=c++20 -Wall -Wextra -o coroutine_demo coroutine_demo.cpp
 * 运行：./coroutine_demo
 */

#include <cassert>
#include <coroutine>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

class IntGenerator {
public:
    struct promise_type {
        int value = 0;
        std::exception_ptr error;

        IntGenerator get_return_object() {
            return IntGenerator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int next) noexcept {
            value = next;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() noexcept { error = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    IntGenerator(const IntGenerator&) = delete;
    IntGenerator& operator=(const IntGenerator&) = delete;
    IntGenerator(IntGenerator&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}
    IntGenerator& operator=(IntGenerator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    ~IntGenerator() {
        if (handle_) handle_.destroy();  // RAII：包括提前停止遍历的情况
    }

    bool next(int& value) {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        if (handle_.promise().error) std::rethrow_exception(handle_.promise().error);
        if (handle_.done()) return false;
        value = handle_.promise().value;
        return true;
    }

private:
    explicit IntGenerator(Handle handle) : handle_(handle) {}
    Handle handle_;
};

IntGenerator range(int first, int last) {
    if (first > last) throw std::invalid_argument("first must not exceed last");
    for (int i = first; i < last; ++i) co_yield i;
}

int main() {
    auto source = range(1, 5);
    auto values = std::move(source);  // 协程帧只有一个所有者
    int value = 0;
    std::vector<int> result;
    while (values.next(value)) {
        result.push_back(value);
        std::cout << value << ' ';
    }
    assert((result == std::vector{1, 2, 3, 4}));
    assert(!values.next(value));  // 完成后不能再次 resume
    assert(!source.next(value)); // 移动后的对象仍可安全使用
    auto empty = range(2, 2);
    assert(!empty.next(value));
    bool caught = false;
    try {
        auto invalid = range(2, 1);
        invalid.next(value);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "\nC++20 coroutine demo complete\n";
}
