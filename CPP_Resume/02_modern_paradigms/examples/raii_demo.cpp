/**
 * @file raii_demo.cpp
 * @brief RAII 与资源管理示例
 * 
 * 编译：g++ -std=c++20 -Wall -pthread -o raii_demo raii_demo.cpp
 * 运行：./raii_demo
 */

#include <iostream>
#include <thread>
#include <fstream>
#include <memory>
#include <string>
#include <functional>

// ============================================================
// 1. RAII 文件包装器
// ============================================================

class FileHandle {
private:
    FILE* file_;
    std::string filename_;

public:
    explicit FileHandle(const std::string& filename, const char* mode)
        : file_(fopen(filename.c_str(), mode)), filename_(filename) {
        if (!file_) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::cout << "[FileHandle] 打开文件: " << filename << std::endl;
    }
    
    ~FileHandle() {
        if (file_) {
            fclose(file_);
            std::cout << "[FileHandle] 关闭文件: " << filename_ << std::endl;
        }
    }
    
    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // 允许移动
    FileHandle(FileHandle&& other) noexcept
        : file_(other.file_), filename_(std::move(other.filename_)) {
        other.file_ = nullptr;
    }
    
    void write(const std::string& text) {
        if (file_) {
            fprintf(file_, "%s", text.c_str());
        }
    }
    
    std::string read_line() {
        if (!file_) return "";
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file_)) {
            return std::string(buffer);
        }
        return "";
    }
};

void demo_file_handle() {
    std::cout << "\n=== RAII 文件包装器 ===\n";
    
    try {
        FileHandle file("raii_test.txt", "w");
        file.write("Hello, RAII!\n");
        file.write("This is a test.\n");
        // 离开作用域时自动关闭
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    // 读取刚写入的文件
    try {
        FileHandle file("raii_test.txt", "r");
        std::string line;
        while (!(line = file.read_line()).empty()) {
            std::cout << "Read: " << line;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    std::cout << "文件已自动关闭\n";
}

// ============================================================
// 2. ScopeGuard 模式
// ============================================================

class ScopeGuard {
private:
    std::function<void()> cleanup_;
    bool active_;

public:
    explicit ScopeGuard(std::function<void()> cleanup)
        : cleanup_(std::move(cleanup)), active_(true) {}
    
    ~ScopeGuard() {
        if (active_) {
            cleanup_();
        }
    }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    
    void dismiss() { active_ = false; }
};

void demo_scope_guard() {
    std::cout << "\n=== ScopeGuard 模式 ===\n";
    
    {
        std::cout << "进入作用域...\n";
        ScopeGuard guard([]() {
            std::cout << "ScopeGuard: 清理操作执行!\n";
        });
        
        std::cout << "做一些工作...\n";
        // 离开作用域时自动执行清理
    }
    
    std::cout << "已离开作用域\n";
    
    // 使用 dismiss 取消清理
    {
        ScopeGuard guard([]() {
            std::cout << "这条消息不会显示\n";
        });
        
        guard.dismiss();  // 取消清理
    }
    std::cout << "dismiss 后清理不执行\n";
}

// ============================================================
// 3. RAII 计时器
// ============================================================

#include <chrono>

class Timer {
private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;

public:
    explicit Timer(const std::string& name)
        : name_(name), start_(std::chrono::high_resolution_clock::now()) {
        std::cout << "[Timer] 开始计时: " << name_ << std::endl;
    }
    
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "[Timer] " << name_ << " 耗时: " << duration.count() << " μs" << std::endl;
    }
    
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
};

void demo_timer() {
    std::cout << "\n=== RAII 计时器 ===\n";
    
    {
        Timer timer("简单循环");
        int sum = 0;
        for (int i = 0; i < 1000000; ++i) {
            sum += i;
        }
        std::cout << "Sum = " << sum << std::endl;
    }
    
    {
        Timer timer("睡眠测试");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// ============================================================
// 4. 与智能指针配合
// ============================================================

class Resource {
public:
    int id;
    
    Resource(int id) : id(id) {
        std::cout << "[Resource] 创建资源 #" << id << std::endl;
    }
    
    ~Resource() {
        std::cout << "[Resource] 销毁资源 #" << id << std::endl;
    }
    
    void use() {
        std::cout << "[Resource] 使用资源 #" << id << std::endl;
    }
};

void demo_smart_pointers() {
    std::cout << "\n=== 智能指针 RAII ===\n";
    
    std::cout << "\n-- unique_ptr --\n";
    {
        auto p1 = std::make_unique<Resource>(1);
        p1->use();
        
        auto p2 = std::make_unique<Resource>(2);
        p2->use();
        
        // p1 和 p2 离开作用域时自动释放
    }
    std::cout << "资源已自动释放\n";
    
    std::cout << "\n-- shared_ptr --\n";
    {
        auto p3 = std::make_shared<Resource>(3);
        std::cout << "引用计数: " << p3.use_count() << std::endl;
        
        {
            auto p4 = p3;
            std::cout << "引用计数: " << p3.use_count() << std::endl;
        }
        
        std::cout << "p4 离开作用域后引用计数: " << p3.use_count() << std::endl;
    }
    std::cout << "资源已自动释放\n";
}

// ============================================================
// 5. 异常安全演示
// ============================================================

void risky_operation(bool should_throw) {
    if (should_throw) {
        throw std::runtime_error("操作失败!");
    }
    std::cout << "操作成功!\n";
}

void demo_exception_safety() {
    std::cout << "\n=== 异常安全 ===\n";
    
    try {
        auto resource = std::make_unique<Resource>(100);
        std::cout << "准备执行风险操作...\n";
        risky_operation(true);  // 这会抛出异常
        std::cout << "这行不会执行\n";
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }
    
    std::cout << "即使异常发生，资源也已正确释放\n";
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "      RAII 与资源管理示例\n";
    std::cout << "========================================\n";
    
    demo_file_handle();
    demo_scope_guard();
    demo_timer();
    demo_smart_pointers();
    demo_exception_safety();
    
    // 清理测试文件
    std::remove("raii_test.txt");
    
    std::cout << "\n========================================\n";
    std::cout << "            示例结束\n";
    std::cout << "========================================\n";
    
    return 0;
}

