# 3.2 STL 算法库

## 📖 本节概述

STL 算法库提供了大量通用算法，涵盖排序、查找、变换、归约等常见操作。掌握这些算法可以让你写出更简洁、高效、可读的代码，避免重复造轮子。

---

## 1. 算法库概览

### 1.1 头文件

```cpp
#include <algorithm>   // 大多数算法
#include <numeric>     // 数值算法
#include <functional>  // 函数对象
```

### 1.2 算法分类

| 类别 | 示例 |
|------|------|
| 非修改算法 | `find`, `count`, `search`, `all_of` |
| 修改算法 | `copy`, `fill`, `transform`, `replace` |
| 排序算法 | `sort`, `stable_sort`, `partial_sort` |
| 二分查找 | `lower_bound`, `upper_bound`, `binary_search` |
| 集合算法 | `set_union`, `set_intersection` |
| 堆算法 | `make_heap`, `push_heap`, `pop_heap` |
| 数值算法 | `accumulate`, `inner_product`, `partial_sum` |

---

## 2. 非修改算法

### 2.1 查找算法

```cpp
#include <algorithm>
#include <vector>

void demo_find() {
    std::vector<int> v = {1, 2, 3, 4, 5, 3, 6};
    
    // find：查找第一个等于给定值的元素
    auto it = std::find(v.begin(), v.end(), 3);
    if (it != v.end()) {
        std::cout << "找到 3 在位置: " << (it - v.begin()) << std::endl;
    }
    
    // find_if：查找第一个满足条件的元素
    auto it2 = std::find_if(v.begin(), v.end(), [](int x) {
        return x > 4;
    });
    if (it2 != v.end()) {
        std::cout << "第一个 > 4 的元素: " << *it2 << std::endl;
    }
    
    // find_if_not：查找第一个不满足条件的元素
    auto it3 = std::find_if_not(v.begin(), v.end(), [](int x) {
        return x < 3;
    });
}
```

### 2.2 计数算法

```cpp
void demo_count() {
    std::vector<int> v = {1, 2, 3, 3, 3, 4, 5};
    
    // count：统计等于给定值的元素个数
    int cnt = std::count(v.begin(), v.end(), 3);
    std::cout << "3 出现了 " << cnt << " 次" << std::endl;
    
    // count_if：统计满足条件的元素个数
    int even_cnt = std::count_if(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });
    std::cout << "偶数有 " << even_cnt << " 个" << std::endl;
}
```

### 2.3 条件检查

```cpp
void demo_predicates() {
    std::vector<int> v = {2, 4, 6, 8, 10};
    
    // all_of：所有元素都满足条件？
    bool all_even = std::all_of(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });
    std::cout << "全是偶数: " << std::boolalpha << all_even << std::endl;
    
    // any_of：存在元素满足条件？
    bool has_big = std::any_of(v.begin(), v.end(), [](int x) {
        return x > 100;
    });
    std::cout << "有 > 100 的: " << has_big << std::endl;
    
    // none_of：没有元素满足条件？
    bool no_negative = std::none_of(v.begin(), v.end(), [](int x) {
        return x < 0;
    });
    std::cout << "没有负数: " << no_negative << std::endl;
}
```

### 2.4 遍历

```cpp
void demo_for_each() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // for_each：对每个元素执行操作
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << std::endl;
    
    // 带状态的 for_each
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) {
        sum += x;
    });
    std::cout << "Sum: " << sum << std::endl;
}
```

---

## 3. 修改算法

### 3.1 复制

```cpp
void demo_copy() {
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst(5);
    
    // copy：复制元素
    std::copy(src.begin(), src.end(), dst.begin());
    
    // copy_if：有条件复制
    std::vector<int> evens;
    std::copy_if(src.begin(), src.end(), std::back_inserter(evens),
                 [](int x) { return x % 2 == 0; });
    
    // copy_n：复制前 n 个
    std::vector<int> first3(3);
    std::copy_n(src.begin(), 3, first3.begin());
    
    // copy_backward：从后向前复制（处理重叠区域）
    std::vector<int> v = {1, 2, 3, 4, 5, 0, 0};
    std::copy_backward(v.begin(), v.begin() + 5, v.end());
}
```

### 3.2 变换

```cpp
void demo_transform() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // transform：原地变换
    std::transform(v.begin(), v.end(), v.begin(), [](int x) {
        return x * x;
    });
    // v = {1, 4, 9, 16, 25}
    
    // transform：输出到新容器
    std::vector<int> src = {1, 2, 3};
    std::vector<int> dst(3);
    std::transform(src.begin(), src.end(), dst.begin(), [](int x) {
        return x * 2;
    });
    
    // transform：二元操作
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {10, 20, 30};
    std::vector<int> c(3);
    std::transform(a.begin(), a.end(), b.begin(), c.begin(),
                   [](int x, int y) { return x + y; });
    // c = {11, 22, 33}
}
```

### 3.3 填充和生成

```cpp
void demo_fill_generate() {
    std::vector<int> v(5);
    
    // fill：填充相同值
    std::fill(v.begin(), v.end(), 42);
    // v = {42, 42, 42, 42, 42}
    
    // fill_n：填充前 n 个
    std::fill_n(v.begin(), 3, 0);
    // v = {0, 0, 0, 42, 42}
    
    // generate：用生成器填充
    int n = 0;
    std::generate(v.begin(), v.end(), [&n]() { return n++; });
    // v = {0, 1, 2, 3, 4}
    
    // iota：填充递增序列（<numeric>）
    std::iota(v.begin(), v.end(), 10);
    // v = {10, 11, 12, 13, 14}
}
```

### 3.4 替换和删除

```cpp
void demo_replace_remove() {
    std::vector<int> v = {1, 2, 3, 2, 5, 2};
    
    // replace：替换等于给定值的元素
    std::replace(v.begin(), v.end(), 2, 99);
    // v = {1, 99, 3, 99, 5, 99}
    
    // replace_if：条件替换
    std::replace_if(v.begin(), v.end(), [](int x) { return x > 50; }, 0);
    
    // remove：移除元素（不真正删除，返回新的逻辑末尾）
    std::vector<int> v2 = {1, 2, 3, 2, 5, 2};
    auto new_end = std::remove(v2.begin(), v2.end(), 2);
    v2.erase(new_end, v2.end());  // 真正删除
    // v2 = {1, 3, 5}
    
    // remove_if + erase（常用模式）
    std::vector<int> v3 = {1, 2, 3, 4, 5, 6};
    v3.erase(
        std::remove_if(v3.begin(), v3.end(), [](int x) { return x % 2 == 0; }),
        v3.end()
    );
    // v3 = {1, 3, 5}
    
    // C++20：std::erase_if（更简洁）
    // std::erase_if(v3, [](int x) { return x % 2 == 0; });
}
```

### 3.5 反转和旋转

```cpp
void demo_reverse_rotate() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // reverse：反转
    std::reverse(v.begin(), v.end());
    // v = {5, 4, 3, 2, 1}
    
    // rotate：旋转
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    std::rotate(v2.begin(), v2.begin() + 2, v2.end());
    // v2 = {3, 4, 5, 1, 2}（将前两个元素移到末尾）
    
    // shuffle：随机打乱
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
}
```

---

## 4. 排序算法

### 4.1 基本排序

```cpp
void demo_sort() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3};
    
    // sort：默认升序
    std::sort(v.begin(), v.end());
    // v = {1, 2, 3, 5, 8, 9}
    
    // sort：自定义比较
    std::sort(v.begin(), v.end(), std::greater<int>());
    // v = {9, 8, 5, 3, 2, 1}（降序）
    
    // sort：Lambda 比较
    std::sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
    });
    
    // stable_sort：稳定排序（保持相等元素的相对顺序）
    std::stable_sort(v.begin(), v.end());
}
```

### 4.2 部分排序

```cpp
void demo_partial_sort() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // partial_sort：只排序前 k 个
    std::partial_sort(v.begin(), v.begin() + 3, v.end());
    // 前 3 个是最小的 3 个（有序），其余无序
    
    // nth_element：将第 n 个元素放到正确位置
    std::vector<int> v2 = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::nth_element(v2.begin(), v2.begin() + 4, v2.end());
    // v2[4] 是第 5 小的元素，左边都小于等于它，右边都大于等于它
}
```

### 4.3 检查排序状态

```cpp
void demo_is_sorted() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {1, 3, 2, 4, 5};
    
    // is_sorted：是否已排序
    bool sorted1 = std::is_sorted(v1.begin(), v1.end());  // true
    bool sorted2 = std::is_sorted(v2.begin(), v2.end());  // false
    
    // is_sorted_until：返回第一个破坏顺序的位置
    auto it = std::is_sorted_until(v2.begin(), v2.end());
    // *it == 2（第一个不满足升序的元素）
}
```

---

## 5. 二分查找

**前提**：容器必须已排序！

```cpp
void demo_binary_search() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // binary_search：是否存在
    bool found = std::binary_search(v.begin(), v.end(), 5);
    
    // lower_bound：第一个 >= target 的位置
    auto lb = std::lower_bound(v.begin(), v.end(), 5);
    // *lb == 5
    
    // upper_bound：第一个 > target 的位置
    auto ub = std::upper_bound(v.begin(), v.end(), 5);
    // *ub == 6
    
    // equal_range：返回 {lower_bound, upper_bound}
    auto [lo, hi] = std::equal_range(v.begin(), v.end(), 5);
    // 区间 [lo, hi) 内都是 5
    
    // 应用：统计某值出现次数
    std::vector<int> v2 = {1, 2, 2, 2, 3, 4};
    auto [l, h] = std::equal_range(v2.begin(), v2.end(), 2);
    int count = h - l;  // 3
}
```

---

## 6. 数值算法

```cpp
#include <numeric>

void demo_numeric() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // accumulate：累加
    int sum = std::accumulate(v.begin(), v.end(), 0);
    // sum = 15
    
    // accumulate：自定义操作
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    // product = 120
    
    // inner_product：内积
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    // dot = 1*4 + 2*5 + 3*6 = 32
    
    // partial_sum：前缀和
    std::vector<int> prefix(v.size());
    std::partial_sum(v.begin(), v.end(), prefix.begin());
    // prefix = {1, 3, 6, 10, 15}
    
    // adjacent_difference：相邻差
    std::vector<int> diff(v.size());
    std::adjacent_difference(v.begin(), v.end(), diff.begin());
    // diff = {1, 1, 1, 1, 1}
    
    // reduce (C++17)：并行友好的累加
    // int sum2 = std::reduce(v.begin(), v.end());
}
```

---

## 7. 最值算法

```cpp
void demo_minmax() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // min_element / max_element：最小/最大元素的迭代器
    auto min_it = std::min_element(v.begin(), v.end());
    auto max_it = std::max_element(v.begin(), v.end());
    std::cout << "最小: " << *min_it << ", 最大: " << *max_it << std::endl;
    
    // minmax_element：同时获取最小和最大
    auto [min_iter, max_iter] = std::minmax_element(v.begin(), v.end());
    
    // min / max / minmax（值）
    int a = 3, b = 5;
    int smaller = std::min(a, b);
    int larger = std::max(a, b);
    auto [lo, hi] = std::minmax(a, b);
    
    // clamp (C++17)：限制在范围内
    int val = 15;
    int clamped = std::clamp(val, 0, 10);  // 10
}
```

---

## 8. 集合算法

**前提**：两个输入范围必须已排序！

```cpp
void demo_set_operations() {
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::vector<int> b = {3, 4, 5, 6, 7};
    std::vector<int> result;
    
    // set_union：并集
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::back_inserter(result));
    // result = {1, 2, 3, 4, 5, 6, 7}
    
    result.clear();
    
    // set_intersection：交集
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(result));
    // result = {3, 4, 5}
    
    result.clear();
    
    // set_difference：差集（a - b）
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(result));
    // result = {1, 2}
    
    result.clear();
    
    // set_symmetric_difference：对称差（并集 - 交集）
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                   std::back_inserter(result));
    // result = {1, 2, 6, 7}
    
    // includes：a 是否包含 b
    std::vector<int> c = {3, 4};
    bool contains = std::includes(a.begin(), a.end(), c.begin(), c.end());
    // true
}
```

---

## 9. 堆算法

```cpp
void demo_heap() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // make_heap：建堆（默认最大堆）
    std::make_heap(v.begin(), v.end());
    // v[0] 是最大元素
    
    std::cout << "堆顶: " << v.front() << std::endl;  // 9
    
    // pop_heap：移除堆顶
    std::pop_heap(v.begin(), v.end());
    v.pop_back();  // 真正删除
    
    // push_heap：插入新元素
    v.push_back(8);
    std::push_heap(v.begin(), v.end());
    
    // sort_heap：堆排序
    std::sort_heap(v.begin(), v.end());
    
    // is_heap：是否是堆
    bool is_heap = std::is_heap(v.begin(), v.end());
}
```

---

## 10. C++20 Ranges

本教程以 C++20 为基线：接受整个范围的 `std::ranges` 算法、`std::views` 惰性适配器和投影是日常工具。前面的迭代器写法仍可使用，尤其适用于需要子区间或没有范围版本的算法。

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

void demo_ranges() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 链式调用
    auto result = v 
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });
    
    for (int x : result) {
        std::cout << x << " ";  // 4 16 36 64 100
    }
    std::cout << std::endl;
    
    // 惰性求值
    auto first3_evens = v 
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::take(3);
    
    // 范围算法
    std::ranges::sort(v);
    auto it = std::ranges::find(v, 5);
    bool found = std::ranges::binary_search(v, 5);
}
```

### 10.1 视图的生命周期与实体化

视图通常借用原容器，管道本身不会预先计算所有结果。保持底层容器存活，并避免使迭代器失效的修改；也不要通过修改元素破坏 `filter` 的筛选条件或依赖其缓存结果。

```cpp
std::vector<int> values{1, 2, 3, 4};
auto squares = values
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * x; });
std::vector<int> saved;
std::ranges::copy(squares, std::back_inserter(saved));  // 4, 16
```

`saved` 拥有结果，可独立于 `values` 存活。C++20 不提供 `std::ranges::to`、`std::views::zip` 或 `std::ranges::fold_left`，这些属于 C++23；累加仍可使用 `<numeric>` 中的 `std::accumulate`。

### 10.2 投影：按成员排序和查找

```cpp
struct Student { std::string name; int score; };
std::vector<Student> students{{"Alice", 88}, {"Bob", 95}, {"Carol", 72}};
std::ranges::sort(students, std::ranges::greater{}, &Student::score);
auto found = std::ranges::find(students, "Bob", &Student::name);
if (found != students.end()) std::cout << found->score;
```

投影指定“比较哪个字段”，比较器指定“如何比较”。算法返回迭代器时，要考虑原范围的寿命；对非 borrowed range 的临时容器调用 `std::ranges::find` 会得到 `std::ranges::dangling`，不能解引用。完整程序见 [stl_algo_demo.cpp](./examples/stl_algo_demo.cpp)。

---

## 📝 练习题

### 练习1：统计词频
使用 STL 算法统计字符串中每个单词出现的次数。

### 练习2：找出 Top K
使用 `partial_sort` 或 `nth_element` 找出数组中最大的 K 个数。

### 练习3：合并有序数组
使用 `merge` 合并两个已排序的数组。

### 练习4：实现 unique
手动实现 `std::unique` 的功能。

---

## 💡 要点总结

1. **STL 算法操作迭代器**：通用性强，适用于各种容器
2. **常用算法**：`find`、`sort`、`transform`、`accumulate`
3. **二分查找前提**：容器必须已排序
4. **remove 不真正删除**：需要配合 `erase`；按条件删除容器元素优先使用 `std::erase_if`
5. **优先使用 STL 算法**：比手写循环更安全、可读
6. **C++20 Ranges**：更优雅的链式调用

---

## ⏭️ 下一节

[3.3 排序与查找](./03_sorting_searching.md) - 深入理解经典算法

