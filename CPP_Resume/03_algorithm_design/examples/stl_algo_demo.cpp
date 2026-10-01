/**
 * @file stl_algo_demo.cpp
 * @brief STL 算法库示例
 * 
 * 编译：g++ -std=c++20 -O2 -Wall -o stl_algo_demo stl_algo_demo.cpp
 * 运行：./stl_algo_demo
 */

#include <iostream>
#include <cassert>
#include <iterator>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>
#include <random>
#include <iterator>

//==============================================================================
// 辅助函数：打印容器
//==============================================================================

template<typename Container>
void print(const std::string& label, const Container& c) {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& x : c) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }
    std::cout << "]" << std::endl;
}

//==============================================================================
// 1. 非修改算法
//==============================================================================

void demo_non_modifying() {
    std::cout << "\n===== 非修改算法 =====" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5, 3, 6, 3};
    print("原数组", v);
    
    // find
    auto it = std::find(v.begin(), v.end(), 3);
    if (it != v.end()) {
        std::cout << "find(3): 位置 " << (it - v.begin()) << std::endl;
    }
    
    // find_if
    auto it2 = std::find_if(v.begin(), v.end(), [](int x) { return x > 4; });
    if (it2 != v.end()) {
        std::cout << "find_if(>4): " << *it2 << std::endl;
    }
    
    // count / count_if
    int cnt = std::count(v.begin(), v.end(), 3);
    std::cout << "count(3): " << cnt << std::endl;
    
    int even_cnt = std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "count_if(偶数): " << even_cnt << std::endl;
    
    // all_of / any_of / none_of
    bool all_positive = std::all_of(v.begin(), v.end(), [](int x) { return x > 0; });
    bool has_six = std::any_of(v.begin(), v.end(), [](int x) { return x == 6; });
    bool no_negative = std::none_of(v.begin(), v.end(), [](int x) { return x < 0; });
    
    std::cout << std::boolalpha;
    std::cout << "all_of(>0): " << all_positive << std::endl;
    std::cout << "any_of(==6): " << has_six << std::endl;
    std::cout << "none_of(<0): " << no_negative << std::endl;
}

//==============================================================================
// 2. 修改算法
//==============================================================================

void demo_modifying() {
    std::cout << "\n===== 修改算法 =====" << std::endl;
    
    // transform
    std::vector<int> v = {1, 2, 3, 4, 5};
    print("原数组", v);
    
    std::transform(v.begin(), v.end(), v.begin(), [](int x) { return x * x; });
    print("transform(x²)", v);
    
    // fill
    std::vector<int> v2(5);
    std::fill(v2.begin(), v2.end(), 42);
    print("fill(42)", v2);
    
    // iota
    std::iota(v2.begin(), v2.end(), 1);
    print("iota(1, 2, 3...)", v2);
    
    // generate
    int n = 0;
    std::generate(v2.begin(), v2.end(), [&n]() { return (n++) * 2; });
    print("generate(0, 2, 4...)", v2);
    
    // replace
    std::vector<int> v3 = {1, 2, 3, 2, 4, 2};
    print("替换前", v3);
    std::replace(v3.begin(), v3.end(), 2, 99);
    print("replace(2->99)", v3);
    
    // C++20：直接删除满足条件的元素
    std::vector<int> v4 = {1, 2, 3, 4, 5, 6};
    print("删除前", v4);
    std::erase_if(v4, [](int x) { return x % 2 == 0; });
    print("erase_if(偶数)", v4);
    
    // reverse
    std::reverse(v4.begin(), v4.end());
    print("reverse", v4);
    
    // rotate
    std::vector<int> v5 = {1, 2, 3, 4, 5};
    std::rotate(v5.begin(), v5.begin() + 2, v5.end());
    print("rotate(左移2)", v5);
}

//==============================================================================
// 3. 排序算法
//==============================================================================

void demo_sorting() {
    std::cout << "\n===== 排序算法 =====" << std::endl;
    
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    print("原数组", v);
    
    // sort
    std::sort(v.begin(), v.end());
    print("sort(升序)", v);
    
    std::sort(v.begin(), v.end(), std::greater<int>());
    print("sort(降序)", v);
    
    // partial_sort
    std::vector<int> v2 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::partial_sort(v2.begin(), v2.begin() + 3, v2.end());
    print("partial_sort(前3小)", v2);
    
    // nth_element
    std::vector<int> v3 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::nth_element(v3.begin(), v3.begin() + 4, v3.end());
    std::cout << "nth_element(第5小): " << v3[4] << std::endl;
    
    // is_sorted
    std::vector<int> v4 = {1, 2, 3, 5, 4};
    std::cout << std::boolalpha;
    std::cout << "is_sorted({1,2,3,5,4}): " << std::is_sorted(v4.begin(), v4.end()) << std::endl;
}

//==============================================================================
// 4. 二分查找
//==============================================================================

void demo_binary_search() {
    std::cout << "\n===== 二分查找 =====" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5, 5, 5, 6, 7, 8, 9};
    print("有序数组", v);
    
    // binary_search
    bool found = std::binary_search(v.begin(), v.end(), 5);
    std::cout << "binary_search(5): " << std::boolalpha << found << std::endl;
    
    // lower_bound / upper_bound
    auto lb = std::lower_bound(v.begin(), v.end(), 5);
    auto ub = std::upper_bound(v.begin(), v.end(), 5);
    std::cout << "lower_bound(5): 位置 " << (lb - v.begin()) << std::endl;
    std::cout << "upper_bound(5): 位置 " << (ub - v.begin()) << std::endl;
    std::cout << "5 出现次数: " << (ub - lb) << std::endl;
    
    // equal_range
    auto [lo, hi] = std::equal_range(v.begin(), v.end(), 5);
    std::cout << "equal_range(5): [" << (lo - v.begin()) << ", " << (hi - v.begin()) << ")" << std::endl;
}

//==============================================================================
// 5. 数值算法
//==============================================================================

void demo_numeric() {
    std::cout << "\n===== 数值算法 =====" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5};
    print("原数组", v);
    
    // accumulate
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "accumulate(sum): " << sum << std::endl;
    
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::cout << "accumulate(product): " << product << std::endl;
    
    // inner_product
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "inner_product({1,2,3} · {4,5,6}): " << dot << std::endl;
    
    // partial_sum
    std::vector<int> prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());
    print("partial_sum(前缀和)", prefix);
    
    // adjacent_difference
    std::vector<int> diff(v.size());
    std::adjacent_difference(v.begin(), v.end(), diff.begin());
    print("adjacent_difference", diff);
}

//==============================================================================
// 6. 最值算法
//==============================================================================

void demo_minmax() {
    std::cout << "\n===== 最值算法 =====" << std::endl;
    
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    print("原数组", v);
    
    auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());
    std::cout << "min: " << *min_it << " at " << (min_it - v.begin()) << std::endl;
    std::cout << "max: " << *max_it << " at " << (max_it - v.begin()) << std::endl;
    
    // min / max / minmax
    std::cout << "min(3, 7): " << std::min(3, 7) << std::endl;
    std::cout << "max(3, 7): " << std::max(3, 7) << std::endl;
    
    auto [lo, hi] = std::minmax({5, 2, 8, 1, 9});
    std::cout << "minmax({5,2,8,1,9}): [" << lo << ", " << hi << "]" << std::endl;
    
    // clamp (C++17)
    std::cout << "clamp(15, 0, 10): " << std::clamp(15, 0, 10) << std::endl;
    std::cout << "clamp(-5, 0, 10): " << std::clamp(-5, 0, 10) << std::endl;
    std::cout << "clamp(5, 0, 10): " << std::clamp(5, 0, 10) << std::endl;
}

//==============================================================================
// 7. 集合算法
//==============================================================================

void demo_set_operations() {
    std::cout << "\n===== 集合算法 =====" << std::endl;
    
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::vector<int> b = {3, 4, 5, 6, 7};
    print("集合 A", a);
    print("集合 B", b);
    
    std::vector<int> result;
    
    // 并集
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
    print("A ∪ B", result);
    result.clear();
    
    // 交集
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
    print("A ∩ B", result);
    result.clear();
    
    // 差集
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
    print("A - B", result);
    result.clear();
    
    // 对称差
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
    print("A △ B", result);
}

//==============================================================================
// 8. 堆算法
//==============================================================================

void demo_heap() {
    std::cout << "\n===== 堆算法 =====" << std::endl;
    
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    print("原数组", v);
    
    // 建堆
    std::make_heap(v.begin(), v.end());
    print("make_heap", v);
    std::cout << "堆顶: " << v.front() << std::endl;
    
    // 弹出堆顶
    std::pop_heap(v.begin(), v.end());
    int top = v.back();
    v.pop_back();
    std::cout << "pop_heap: 取出 " << top << std::endl;
    print("弹出后", v);
    
    // 压入新元素
    v.push_back(8);
    std::push_heap(v.begin(), v.end());
    print("push_heap(8)", v);
    
    // 堆排序
    std::sort_heap(v.begin(), v.end());
    print("sort_heap", v);
}

//==============================================================================
// 9. C++20 Ranges：惰性视图、范围算法与投影
//==============================================================================

void demo_ranges() {
    std::cout << "\n===== C++20 Ranges =====\n";
    std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8};
    auto squares = values
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);

    // C++20 用 copy 实体化；std::ranges::to 属于 C++23。
    std::vector<int> result;
    std::ranges::copy(squares, std::back_inserter(result));
    assert((result == std::vector{4, 16, 36}));
    print("前 3 个偶数的平方", result);

    struct Student { std::string name; int score; };
    std::vector<Student> students{{"Alice", 88}, {"Bob", 95}, {"Carol", 72}};
    std::ranges::sort(students, std::ranges::greater{}, &Student::score);
    const auto found = std::ranges::find(students, "Bob", &Student::name);
    assert(found != students.end() && found->score == 95);
    assert(students.front().name == "Bob");
    for (const auto& student : students) {
        std::cout << student.name << ": " << student.score << '\n';
    }
    // squares 借用了 values；使用视图期间保持原容器有效。
}

//==============================================================================
// 主函数
//==============================================================================

int main() {
    std::cout << "===== STL 算法库示例 =====" << std::endl;
    
    demo_non_modifying();
    demo_modifying();
    demo_sorting();
    demo_binary_search();
    demo_numeric();
    demo_minmax();
    demo_set_operations();
    demo_heap();
    demo_ranges();
    
    std::cout << "\n===== 完成 =====" << std::endl;
    return 0;
}

