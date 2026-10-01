/**
 * @file sort_search_demo.cpp
 * @brief 排序与查找算法示例
 * 
 * 编译：g++ -std=c++20 -O2 -Wall -o sort_search_demo sort_search_demo.cpp
 * 运行：./sort_search_demo
 */

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

//==============================================================================
// 辅助函数
//==============================================================================

template<typename Container>
void print(const std::string& label, const Container& c, int limit = 20) {
    std::cout << label << ": [";
    int count = 0;
    for (const auto& x : c) {
        if (count > 0) std::cout << ", ";
        if (count >= limit) {
            std::cout << "...";
            break;
        }
        std::cout << x;
        count++;
    }
    std::cout << "]" << std::endl;
}

std::vector<int> generate_random(int n, int max_val = 1000) {
    std::vector<int> v(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, max_val);
    for (int& x : v) x = dis(gen);
    return v;
}

//==============================================================================
// 1. 冒泡排序
//==============================================================================

void bubble_sort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//==============================================================================
// 2. 选择排序
//==============================================================================

void selection_sort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

//==============================================================================
// 3. 插入排序
//==============================================================================

void insertion_sort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

//==============================================================================
// 4. 归并排序
//==============================================================================

void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (int i = 0; i < k; ++i) {
        arr[left + i] = temp[i];
    }
}

void merge_sort_impl(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    merge_sort_impl(arr, left, mid);
    merge_sort_impl(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void merge_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    merge_sort_impl(arr, 0, arr.size() - 1);
}

//==============================================================================
// 5. 快速排序
//==============================================================================

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort_impl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_impl(arr, low, pi - 1);
        quick_sort_impl(arr, pi + 1, high);
    }
}

void quick_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quick_sort_impl(arr, 0, arr.size() - 1);
}

//==============================================================================
// 6. 堆排序
//==============================================================================

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
    
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

//==============================================================================
// 7. 二分查找
//==============================================================================

int binary_search(const std::vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// 查找第一个等于 target 的位置
int find_first(const std::vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            result = mid;
            right = mid - 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

// 查找最后一个等于 target 的位置
int find_last(const std::vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            result = mid;
            left = mid + 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

//==============================================================================
// 8. 性能测试
//==============================================================================

template<typename SortFunc>
double benchmark_sort(SortFunc sort_func, int n, int iterations = 5) {
    double total = 0;
    for (int i = 0; i < iterations; ++i) {
        auto arr = generate_random(n);
        auto start = std::chrono::high_resolution_clock::now();
        sort_func(arr);
        auto end = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::milli>(end - start).count();
    }
    return total / iterations;
}

//==============================================================================
// 主函数
//==============================================================================

int main() {
    std::cout << "===== 排序与查找算法示例 =====" << std::endl;
    
    // ========== 1. 排序演示 ==========
    std::cout << "\n【1. 排序演示】" << std::endl;
    
    std::vector<int> sample = {64, 34, 25, 12, 22, 11, 90};
    print("原数组", sample);
    
    auto test = sample;
    bubble_sort(test);
    print("冒泡排序", test);
    
    test = sample;
    selection_sort(test);
    print("选择排序", test);
    
    test = sample;
    insertion_sort(test);
    print("插入排序", test);
    
    test = sample;
    merge_sort(test);
    print("归并排序", test);
    
    test = sample;
    quick_sort(test);
    print("快速排序", test);
    
    test = sample;
    heap_sort(test);
    print("堆排序", test);
    
    // ========== 2. 排序性能比较 ==========
    std::cout << "\n【2. 排序性能比较】" << std::endl;
    std::cout << std::setw(12) << "算法" 
              << std::setw(12) << "n=1000" 
              << std::setw(12) << "n=5000" 
              << std::setw(12) << "n=10000" << std::endl;
    std::cout << std::string(48, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(2);
    
    // O(n²) 算法只测小规模
    std::cout << std::setw(12) << "冒泡" 
              << std::setw(12) << benchmark_sort(bubble_sort, 1000) 
              << std::setw(12) << benchmark_sort(bubble_sort, 5000) 
              << std::setw(12) << "-" << " ms" << std::endl;
    
    std::cout << std::setw(12) << "选择" 
              << std::setw(12) << benchmark_sort(selection_sort, 1000) 
              << std::setw(12) << benchmark_sort(selection_sort, 5000) 
              << std::setw(12) << "-" << " ms" << std::endl;
    
    std::cout << std::setw(12) << "插入" 
              << std::setw(12) << benchmark_sort(insertion_sort, 1000) 
              << std::setw(12) << benchmark_sort(insertion_sort, 5000) 
              << std::setw(12) << "-" << " ms" << std::endl;
    
    // O(n log n) 算法
    std::cout << std::setw(12) << "归并" 
              << std::setw(12) << benchmark_sort(merge_sort, 1000) 
              << std::setw(12) << benchmark_sort(merge_sort, 5000) 
              << std::setw(12) << benchmark_sort(merge_sort, 10000) << " ms" << std::endl;
    
    std::cout << std::setw(12) << "快速" 
              << std::setw(12) << benchmark_sort(quick_sort, 1000) 
              << std::setw(12) << benchmark_sort(quick_sort, 5000) 
              << std::setw(12) << benchmark_sort(quick_sort, 10000) << " ms" << std::endl;
    
    std::cout << std::setw(12) << "堆排序" 
              << std::setw(12) << benchmark_sort(heap_sort, 1000) 
              << std::setw(12) << benchmark_sort(heap_sort, 5000) 
              << std::setw(12) << benchmark_sort(heap_sort, 10000) << " ms" << std::endl;
    
    std::cout << std::setw(12) << "std::sort" 
              << std::setw(12) << benchmark_sort([](std::vector<int>& v) { std::sort(v.begin(), v.end()); }, 1000) 
              << std::setw(12) << benchmark_sort([](std::vector<int>& v) { std::sort(v.begin(), v.end()); }, 5000) 
              << std::setw(12) << benchmark_sort([](std::vector<int>& v) { std::sort(v.begin(), v.end()); }, 10000) << " ms" << std::endl;
    
    // ========== 3. 二分查找演示 ==========
    std::cout << "\n【3. 二分查找演示】" << std::endl;
    
    std::vector<int> sorted = {1, 2, 3, 4, 5, 5, 5, 6, 7, 8, 9, 10};
    print("有序数组", sorted);
    
    int target = 5;
    std::cout << "查找 " << target << ":" << std::endl;
    std::cout << "  binary_search: 位置 " << binary_search(sorted, target) << std::endl;
    std::cout << "  find_first: 位置 " << find_first(sorted, target) << std::endl;
    std::cout << "  find_last: 位置 " << find_last(sorted, target) << std::endl;
    
    target = 100;
    std::cout << "查找 " << target << " (不存在): 位置 " << binary_search(sorted, target) << std::endl;
    
    // ========== 4. 二分查找性能 ==========
    std::cout << "\n【4. 二分查找 vs 线性查找】" << std::endl;
    
    int n = 10000000;
    std::vector<int> big_array(n);
    std::iota(big_array.begin(), big_array.end(), 0);
    
    int search_target = n - 1;  // 最坏情况
    
    // 线性查找
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        auto it = std::find(big_array.begin(), big_array.end(), search_target);
        (void)it;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double linear_time = std::chrono::duration<double, std::milli>(end - start).count() / 100;
    
    // 二分查找
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) {
        int pos = binary_search(big_array, search_target);
        (void)pos;
    }
    end = std::chrono::high_resolution_clock::now();
    double binary_time = std::chrono::duration<double, std::milli>(end - start).count() / 100000;
    
    std::cout << "数组大小: " << n << std::endl;
    std::cout << "线性查找: " << linear_time << " ms" << std::endl;
    std::cout << "二分查找: " << binary_time << " ms" << std::endl;
    std::cout << "加速比: " << linear_time / binary_time << "x" << std::endl;
    
    std::cout << "\n===== 完成 =====" << std::endl;
    return 0;
}

