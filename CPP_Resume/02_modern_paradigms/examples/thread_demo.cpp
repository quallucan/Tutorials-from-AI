/**
 * @file thread_demo.cpp
 * @brief 并发编程入门示例
 * 
 * 编译：g++ -std=c++20 -Wall -pthread -o thread_demo thread_demo.cpp
 * 运行：./thread_demo
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <chrono>
#include <barrier>
#include <cassert>
#include <latch>
#include <semaphore>
#include <stop_token>
#include <string>
#include <syncstream>

// ============================================================
// 1. 创建线程
// ============================================================

void simple_function() {
    std::cout << "  [Thread] 简单函数在线程中执行\n";
    std::cout << "  [Thread] 线程 ID: " << std::this_thread::get_id() << "\n";
}

void demo_create_threads() {
    std::cout << "\n=== 创建线程 ===\n";
    
    std::cout << "主线程 ID: " << std::this_thread::get_id() << "\n";
    
    // 使用普通函数
    std::thread t1(simple_function);
    
    // 使用 Lambda
    std::thread t2([]() {
        std::cout << "  [Lambda Thread] Lambda 线程\n";
    });
    
    // 带参数
    std::thread t3([](int x, const std::string& msg) {
        std::cout << "  [Param Thread] " << msg << ": " << x << "\n";
    }, 42, "参数值");
    
    t1.join();
    t2.join();
    t3.join();
    
    std::cout << "所有线程已结束\n";
}

// ============================================================
// 2. 数据竞争问题
// ============================================================

int safe_counter = 0;
std::mutex counter_mutex;

// 反例（不执行）：两个线程同时 ++ 普通 int 会产生数据竞争和未定义行为。
// 不能把一次运行的输出当作数据竞争的可靠演示。

void increment_safe(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(counter_mutex);
        ++safe_counter;
    }
}

void demo_data_race() {
    std::cout << "\n=== 数据竞争问题 ===\n";
    
    const int iterations = 100000;
    
    std::cout << "未加锁的 ++counter 是未定义行为，反例仅作说明。\n";
    
    // 安全版本
    safe_counter = 0;
    std::thread t3(increment_safe, iterations);
    std::thread t4(increment_safe, iterations);
    t3.join();
    t4.join();
    std::cout << "安全计数器 (期望 " << iterations * 2 << "): " << safe_counter << "\n";
    assert(safe_counter == iterations * 2);
}

// ============================================================
// 3. 互斥锁
// ============================================================

std::mutex print_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << msg << "\n";
}

void demo_mutex() {
    std::cout << "\n=== 互斥锁 ===\n";
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            for (int j = 0; j < 3; ++j) {
                safe_print("  线程 " + std::to_string(i) + " 输出 " + std::to_string(j));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
}

// ============================================================
// 4. 条件变量
// ============================================================

std::mutex cv_mutex;
std::condition_variable cv;
bool ready = false;

void worker_thread(int id) {
    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, []{ return ready; });
    std::cout << "  工作线程 " << id << " 收到信号，开始工作\n";
}

void demo_condition_variable() {
    std::cout << "\n=== 条件变量 ===\n";
    
    ready = false;
    
    std::vector<std::thread> workers;
    for (int i = 0; i < 3; ++i) {
        workers.emplace_back(worker_thread, i);
    }
    
    std::cout << "主线程：准备发送信号...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    {
        std::lock_guard<std::mutex> lock(cv_mutex);
        ready = true;
    }
    cv.notify_all();
    
    for (auto& w : workers) {
        w.join();
    }
}

// ============================================================
// 5. 生产者-消费者
// ============================================================

std::queue<int> buffer;
std::mutex buffer_mutex;
std::condition_variable buffer_cv;
bool producer_done = false;

void producer(int count) {
    for (int i = 0; i < count; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            buffer.push(i);
            std::cout << "  [生产者] 生产: " << i << "\n";
        }
        buffer_cv.notify_one();
    }
    
    {
        std::lock_guard<std::mutex> lock(buffer_mutex);
        producer_done = true;
    }
    buffer_cv.notify_all();
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        buffer_cv.wait(lock, []{ return !buffer.empty() || producer_done; });
        
        if (buffer.empty() && producer_done) {
            break;
        }
        
        int value = buffer.front();
        buffer.pop();
        lock.unlock();
        
        std::cout << "  [消费者 " << id << "] 消费: " << value << "\n";
    }
}

void demo_producer_consumer() {
    std::cout << "\n=== 生产者-消费者 ===\n";
    
    producer_done = false;
    while (!buffer.empty()) buffer.pop();
    
    std::thread prod(producer, 5);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);
    
    prod.join();
    cons1.join();
    cons2.join();
}

// ============================================================
// 6. async 和 future
// ============================================================

int compute(int x) {
    std::cout << "  [Async] 开始计算 " << x << "^2\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

void demo_async_future() {
    std::cout << "\n=== async 和 future ===\n";
    
    // 异步启动任务
    std::future<int> f1 = std::async(std::launch::async, compute, 5);
    std::future<int> f2 = std::async(std::launch::async, compute, 7);
    
    std::cout << "任务已启动，做其他工作...\n";
    
    // 获取结果（会阻塞等待）
    int result1 = f1.get();
    int result2 = f2.get();
    
    std::cout << "结果: 5^2 = " << result1 << ", 7^2 = " << result2 << "\n";
}

// ============================================================
// 7. promise
// ============================================================

void worker_with_promise(std::promise<int>& prom) {
    std::cout << "  [Promise Worker] 开始工作...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    prom.set_value(42);
    std::cout << "  [Promise Worker] 已设置结果\n";
}

void demo_promise() {
    std::cout << "\n=== promise ===\n";
    
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    
    std::thread t(worker_with_promise, std::ref(prom));
    
    std::cout << "等待结果...\n";
    int result = fut.get();
    std::cout << "收到结果: " << result << "\n";
    
    t.join();
}

// ============================================================
// 8. 原子操作
// ============================================================

std::atomic<int> atomic_counter{0};

void increment_atomic(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        ++atomic_counter;  // 原子操作，无需锁
    }
}

void demo_atomic() {
    std::cout << "\n=== 原子操作 ===\n";
    
    const int iterations = 100000;
    atomic_counter = 0;
    
    std::thread t1(increment_atomic, iterations);
    std::thread t2(increment_atomic, iterations);
    
    t1.join();
    t2.join();
    
    std::cout << "原子计数器 (期望 " << iterations * 2 << "): " << atomic_counter << "\n";
    assert(atomic_counter == iterations * 2);
}

// ============================================================
// 9. 硬件并发
// ============================================================

void demo_hardware_concurrency() {
    std::cout << "\n=== 硬件并发信息 ===\n";
    
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "硬件并发线程数: " << n << "\n";
}

// ============================================================
// 10. C++20：自动回收线程与协作式停止
// ============================================================

void demo_jthread_stop() {
    std::cout << "\n=== C++20 jthread 与 stop_token ===\n";
    std::mutex mutex;
    std::condition_variable_any wakeup;
    std::latch started{1};
    bool observed_stop = false;
    {
        std::jthread worker([&](std::stop_token stop) {
            std::unique_lock lock(mutex);
            started.count_down();
            // 带 stop_token 的等待可被停止请求唤醒；普通 condition_variable 不支持。
            wakeup.wait(lock, stop, [] { return false; });
            observed_stop = stop.stop_requested();
            std::osyncstream(std::cout) << "工作线程收到停止请求\n";
        });
        started.wait();
        worker.request_stop();  // 协作式请求，不是强制终止
    }  // 析构时 request_stop + join；被引用的状态此时仍然存活
    assert(observed_stop);  // join 之后读取，无数据竞争
}

// ============================================================
// 11. C++20：latch、barrier、semaphore 与原子等待
// ============================================================

void demo_cpp20_synchronization() {
    constexpr int worker_count = 3;
    std::latch completed{worker_count};
    std::barrier phase{worker_count};
    std::binary_semaphore permit{1};
    int total = 0;
    {
        std::vector<std::jthread> workers;
        for (int i = 0; i < worker_count; ++i) {
            workers.emplace_back([&] {
                for (int round = 0; round < 2; ++round) {
                    permit.acquire();
                    ++total;  // 此处不会抛出异常，随后必定释放许可
                    permit.release();
                    phase.arrive_and_wait();  // 可重复使用的阶段屏障
                }
                completed.count_down();
            });
        }
        completed.wait();  // 一次性等待所有工作完成
    }  // 所有线程已 join，之后才销毁同步对象
    assert(total == worker_count * 2);
    std::cout << "两轮同步后的计数: " << total << '\n';

    std::atomic<bool> published{false};
    int answer = 0;
    std::jthread publisher([&] {
        answer = 42;
        published.store(true, std::memory_order_release);
        published.notify_one();
    });
    published.wait(false, std::memory_order_acquire);
    assert(answer == 42);  // release/acquire 发布了非原子数据
    std::cout << "原子等待收到的结果: " << answer << '\n';
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "        并发编程入门示例\n";
    std::cout << "========================================\n";
    
    demo_create_threads();
    demo_data_race();
    demo_mutex();
    demo_condition_variable();
    demo_producer_consumer();
    demo_async_future();
    demo_promise();
    demo_atomic();
    demo_hardware_concurrency();
    demo_jthread_stop();
    demo_cpp20_synchronization();
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

