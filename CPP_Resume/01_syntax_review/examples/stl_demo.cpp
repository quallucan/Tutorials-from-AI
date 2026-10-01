/**
 * @file stl_demo.cpp
 * @brief STL 容器示例
 * 
 * 演示内容：
 * - vector
 * - array
 * - map 和 unordered_map
 * - set 和 unordered_set
 * - stack, queue, priority_queue
 * - 迭代器使用
 * 
 * 编译：g++ -std=c++20 -Wall -o stl_demo stl_demo.cpp
 * 运行：./stl_demo
 */

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>

// ============================================================
// 1. vector - 动态数组
// ============================================================

void demo_vector() {
    std::cout << "=== vector 示例 ===" << std::endl;
    
    // 创建和初始化
    std::vector<int> v1;                    // 空
    std::vector<int> v2(5, 10);             // 5个10
    std::vector<int> v3 = {1, 2, 3, 4, 5};  // 初始化列表
    
    // 添加元素
    v1.push_back(1);
    v1.push_back(2);
    v1.emplace_back(3);  // 原地构造
    
    // 访问元素
    std::cout << "  v1[0] = " << v1[0] << std::endl;
    std::cout << "  v1.at(1) = " << v1.at(1) << std::endl;
    std::cout << "  v1.front() = " << v1.front() << std::endl;
    std::cout << "  v1.back() = " << v1.back() << std::endl;
    
    // 遍历 - 范围 for
    std::cout << "  v3 内容: ";
    for (const auto& x : v3) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // 大小和容量
    std::cout << "  v3.size() = " << v3.size() << std::endl;
    std::cout << "  v3.capacity() = " << v3.capacity() << std::endl;
    
    // 删除元素
    v3.pop_back();
    v3.erase(v3.begin());  // 删除第一个
    
    std::cout << "  删除后 v3: ";
    for (const auto& x : v3) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 2. array - 固定大小数组 (C++11)
// ============================================================

void demo_array() {
    std::cout << "=== array 示例 ===" << std::endl;
    
    std::array<int, 5> arr = {10, 20, 30, 40, 50};
    
    std::cout << "  arr.size() = " << arr.size() << std::endl;
    std::cout << "  arr[2] = " << arr[2] << std::endl;
    
    // 遍历
    std::cout << "  arr 内容: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // 填充
    std::array<int, 3> arr2;
    arr2.fill(42);
    std::cout << "  arr2.fill(42): ";
    for (const auto& x : arr2) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 3. list 和 deque
// ============================================================

void demo_list_deque() {
    std::cout << "=== list 和 deque 示例 ===" << std::endl;
    
    // list: 双向链表
    std::list<int> lst = {3, 1, 4, 1, 5};
    lst.push_front(0);
    lst.push_back(9);
    
    std::cout << "  list 内容: ";
    for (const auto& x : lst) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    lst.sort();
    std::cout << "  排序后: ";
    for (const auto& x : lst) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    lst.unique();  // 删除相邻重复
    std::cout << "  去重后: ";
    for (const auto& x : lst) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // deque: 双端队列
    std::deque<int> dq = {2, 3, 4};
    dq.push_front(1);
    dq.push_back(5);
    
    std::cout << "  deque 内容: ";
    for (const auto& x : dq) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 4. map - 有序键值对
// ============================================================

void demo_map() {
    std::cout << "=== map 示例 ===" << std::endl;
    
    std::map<std::string, int> scores;
    
    // 插入
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores.insert({"Charlie", 92});
    scores.emplace("David", 88);
    
    // 访问
    std::cout << "  Alice 的分数: " << scores["Alice"] << std::endl;
    
    // 查找
    if (auto it = scores.find("Bob"); it != scores.end()) {
        std::cout << "  找到 Bob: " << it->second << std::endl;
    }
    
    // C++20 contains
    if (scores.contains("Charlie")) {
        std::cout << "  Charlie 存在于 map 中" << std::endl;
    }
    
    // 遍历 (C++17 结构化绑定)
    std::cout << "  所有分数:" << std::endl;
    for (const auto& [name, score] : scores) {
        std::cout << "    " << name << ": " << score << std::endl;
    }
    
    // 删除
    scores.erase("David");
    std::cout << "  删除 David 后大小: " << scores.size() << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 5. unordered_map - 哈希表
// ============================================================

void demo_unordered_map() {
    std::cout << "=== unordered_map 示例 ===" << std::endl;
    
    std::unordered_map<std::string, int> word_count;
    
    // 统计词频
    std::vector<std::string> words = {"apple", "banana", "apple", "cherry", "banana", "apple"};
    for (const auto& word : words) {
        word_count[word]++;
    }
    
    std::cout << "  词频统计:" << std::endl;
    for (const auto& [word, count] : word_count) {
        std::cout << "    " << word << ": " << count << std::endl;
    }
    
    // 哈希表特性
    std::cout << "  桶数量: " << word_count.bucket_count() << std::endl;
    std::cout << "  负载因子: " << word_count.load_factor() << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 6. set 和 unordered_set
// ============================================================

void demo_set() {
    std::cout << "=== set 和 unordered_set 示例 ===" << std::endl;
    
    // set: 有序集合
    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};  // 自动去重和排序
    
    std::cout << "  set 内容 (自动排序去重): ";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // 插入
    auto [it, success] = s.insert(7);
    std::cout << "  插入 7: " << (success ? "成功" : "已存在") << std::endl;
    
    auto [it2, success2] = s.insert(3);
    std::cout << "  插入 3: " << (success2 ? "成功" : "已存在") << std::endl;
    
    // unordered_set: 哈希集合
    std::unordered_set<std::string> names = {"Alice", "Bob", "Charlie"};
    names.insert("David");
    
    std::cout << "  unordered_set 内容: ";
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // 查找
    if (names.count("Alice") > 0) {
        std::cout << "  Alice 在集合中" << std::endl;
    }
    
    std::cout << std::endl;
}

// ============================================================
// 7. 容器适配器: stack, queue, priority_queue
// ============================================================

void demo_adapters() {
    std::cout << "=== 容器适配器示例 ===" << std::endl;
    
    // stack: 栈 (LIFO)
    std::stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    
    std::cout << "  stack (后进先出): ";
    while (!stk.empty()) {
        std::cout << stk.top() << " ";
        stk.pop();
    }
    std::cout << std::endl;
    
    // queue: 队列 (FIFO)
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    
    std::cout << "  queue (先进先出): ";
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
    
    // priority_queue: 优先队列 (默认最大堆)
    std::priority_queue<int> pq;
    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(1);
    pq.push(5);
    
    std::cout << "  priority_queue (最大堆): ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << std::endl;
    
    // 最小堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(3);
    min_pq.push(1);
    min_pq.push(4);
    
    std::cout << "  priority_queue (最小堆): ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 8. 迭代器和算法
// ============================================================

void demo_iterators() {
    std::cout << "=== 迭代器和算法示例 ===" << std::endl;
    
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // 使用迭代器遍历
    std::cout << "  原始数据: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 排序
    std::sort(v.begin(), v.end());
    std::cout << "  排序后: ";
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // 反向迭代
    std::cout << "  反向遍历: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 查找
    auto it = std::find(v.begin(), v.end(), 5);
    if (it != v.end()) {
        std::cout << "  找到 5 在位置: " << std::distance(v.begin(), it) << std::endl;
    }
    
    // 二分查找（需要已排序）
    bool found = std::binary_search(v.begin(), v.end(), 7);
    std::cout << "  二分查找 7: " << (found ? "找到" : "未找到") << std::endl;
    
    // 求和
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "  总和: " << sum << std::endl;
    
    // 计数
    v.push_back(5);
    v.push_back(5);
    int count = std::count(v.begin(), v.end(), 5);
    std::cout << "  5 出现次数: " << count << std::endl;
    
    // transform
    std::vector<int> squared(v.size());
    std::transform(v.begin(), v.end(), squared.begin(), [](int x) { return x * x; });
    std::cout << "  平方后: ";
    for (const auto& x : squared) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // filter (使用 copy_if)
    std::vector<int> evens;
    std::copy_if(v.begin(), v.end(), std::back_inserter(evens), 
                 [](int x) { return x % 2 == 0; });
    std::cout << "  偶数: ";
    for (const auto& x : evens) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

// ============================================================
// 9. C++17/20 新特性
// ============================================================

void demo_modern_stl() {
    std::cout << "=== 现代 STL 特性 ===" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // C++17: 结构化绑定
    std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
    for (const auto& [key, value] : m) {
        std::cout << "  " << key << " -> " << value << std::endl;
    }
    
    // C++20: erase_if (统一的删除方式)
    std::erase_if(v, [](int x) { return x % 2 == 0; });
    std::cout << "  删除偶数后: ";
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    
    // C++20: contains
    std::set<int> s = {1, 2, 3};
    if (s.contains(2)) {
        std::cout << "  set 包含 2" << std::endl;
    }
    
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "       C++ STL 容器示例程序" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    demo_vector();
    demo_array();
    demo_list_deque();
    demo_map();
    demo_unordered_map();
    demo_set();
    demo_adapters();
    demo_iterators();
    demo_modern_stl();
    
    std::cout << "========================================" << std::endl;
    std::cout << "            示例结束" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

