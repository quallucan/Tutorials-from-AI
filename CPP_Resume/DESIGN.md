# C++ 现代编程教程设计文档

## 📋 项目概述

本教程面向有C++基础但长期未接触代码的学习者，旨在帮助其快速回顾语法要点、掌握现代C++编程范式，并建立算法设计的基础能力。

### 目标读者
- 曾学习过C++基础语法
- 多年未编写C++代码
- 缺乏完整项目开发经验
- 希望以 C++20 为基线学习现代 C++ 的开发者

### 学习目标
1. 回顾并巩固C++核心语法
2. 理解现代C++的新特性和最佳实践
3. 掌握现代C++编程范式
4. 具备基础算法设计与实现能力

---

## 📁 目录结构

```
CPP_Resume/
├── DESIGN.md                      # 本设计文档
├── README.md                      # 教程简介与使用指南
│
├── 01_syntax_review/              # 第一章：语法回顾
│   ├── README.md                  # 章节导读
│   ├── 01_basic_syntax.md         # 基础语法回顾
│   ├── 02_pointers_references.md  # 指针与引用
│   ├── 03_classes_objects.md      # 类与对象
│   ├── 04_templates_basics.md     # 模板基础
│   ├── 05_stl_containers.md       # STL容器
│   ├── 06_modern_features.md      # 现代C++新特性总览
│   └── examples/                  # 示例代码目录
│       ├── basic_demo.cpp
│       ├── pointer_demo.cpp
│       ├── class_demo.cpp
│       ├── template_demo.cpp
│       ├── stl_demo.cpp
│       ├── modern_features_demo.cpp
│       └── coroutine_demo.cpp
│
├── 02_modern_paradigms/           # 第二章：现代编程范式
│   ├── README.md                  # 章节导读
│   ├── 01_raii_resource.md        # RAII与资源管理
│   ├── 02_smart_pointers.md       # 智能指针详解
│   ├── 03_move_semantics.md       # 移动语义与完美转发
│   ├── 04_lambda_functional.md    # Lambda与函数式编程
│   ├── 05_type_deduction.md       # 类型推断（auto/decltype）
│   ├── 06_constexpr_compile.md    # 编译期计算与constexpr
│   ├── 07_error_handling.md       # 现代错误处理
│   ├── 08_concurrency_intro.md    # 并发编程入门
│   └── examples/                  # 示例代码目录
│       ├── raii_demo.cpp
│       ├── smart_ptr_demo.cpp
│       ├── move_demo.cpp
│       ├── lambda_demo.cpp
│       ├── type_deduction_demo.cpp
│       ├── constexpr_demo.cpp
│       ├── error_demo.cpp
│       └── thread_demo.cpp
│
├── 03_algorithm_design/           # 第三章：算法设计初步
│   ├── README.md                  # 章节导读
│   ├── 01_complexity_analysis.md  # 复杂度分析
│   ├── 02_stl_algorithms.md       # STL算法库
│   ├── 03_sorting_searching.md    # 排序与查找
│   ├── 04_recursion_dp.md         # 递归与动态规划
│   ├── 05_data_structures.md      # 常用数据结构实现
│   ├── 06_practical_problems.md   # 实战练习题
│   └── examples/                  # 示例代码目录
│       ├── complexity_demo.cpp
│       ├── stl_algo_demo.cpp
│       ├── sort_search_demo.cpp
│       ├── recursion_demo.cpp
│       ├── data_struct_demo.cpp
│       └── solutions/             # 练习题解答
│           └── ...
│
└── projects/                      # 综合练习项目
    ├── README.md
    ├── project_01_calculator/     # 项目1：命令行计算器
    ├── project_02_todo_list/      # 项目2：待办事项管理
    └── project_03_mini_stl/       # 项目3：迷你STL实现
```

---

## 📚 章节详细设计

### 第一章：C++ 语法回顾 (`01_syntax_review/`)

**目标**：快速回顾C++核心语法，为学习现代特性打下基础

| 节号 | 标题 | 主要内容 | 预计时长 |
|------|------|----------|----------|
| 1.1 | 基础语法回顾 | 数据类型、变量、运算符、控制流、函数 | 2小时 |
| 1.2 | 指针与引用 | 指针操作、引用、const修饰符、指针算术 | 2小时 |
| 1.3 | 类与对象 | 类定义、构造/析构、继承、多态、虚函数 | 3小时 |
| 1.4 | 模板基础 | 函数模板、类模板、Concepts 与 requires | 2小时 |
| 1.5 | STL容器 | vector、map、set、unordered_map等 | 2小时 |
| 1.6 | 现代C++新特性总览 | C++11/14/17/20 关键特性概述 | 1小时 |

---

### 第二章：现代编程范式 (`02_modern_paradigms/`)

**目标**：深入理解现代C++的核心编程范式和最佳实践

| 节号 | 标题 | 主要内容 | 预计时长 |
|------|------|----------|----------|
| 2.1 | RAII与资源管理 | RAII原则、资源所有权、作用域管理 | 2小时 |
| 2.2 | 智能指针详解 | unique_ptr、shared_ptr、weak_ptr | 3小时 |
| 2.3 | 移动语义与完美转发 | 右值引用、std::move、std::forward | 3小时 |
| 2.4 | Lambda与函数式编程 | Lambda语法、捕获列表、高阶函数 | 2小时 |
| 2.5 | 类型推断 | auto、decltype、decltype(auto) | 1.5小时 |
| 2.6 | 编译期计算 | constexpr、consteval、constinit、编译期容器 | 2小时 |
| 2.7 | 现代错误处理 | 异常、std::optional、std::variant / Result | 2小时 |
| 2.8 | 并发编程入门 | jthread、stop_token、同步工具、future/promise | 3小时 |

---

### 第三章：算法设计初步 (`03_algorithm_design/`)

**目标**：掌握基础算法设计思想和实现技巧

| 节号 | 标题 | 主要内容 | 预计时长 |
|------|------|----------|----------|
| 3.1 | 复杂度分析 | 时间/空间复杂度、大O表示法 | 1.5小时 |
| 3.2 | STL算法库 | 范围算法、views、投影及传统迭代器算法 | 2小时 |
| 3.3 | 排序与查找 | 经典排序算法、二分查找 | 3小时 |
| 3.4 | 递归与动态规划 | 递归思想、记忆化、DP基础 | 4小时 |
| 3.5 | 常用数据结构 | 链表、栈、队列、树、图的实现 | 4小时 |
| 3.6 | 实战练习题 | 综合练习与LeetCode风格题目 | 持续 |

---

## 🎯 教学设计原则

### 1. 渐进式学习
- 从熟悉的知识开始，逐步引入新概念
- 每个新特性都与旧方法对比，突出优势

### 2. 代码优先
- 每个概念配套可运行的示例代码
- 鼓励动手实践，边学边练

### 3. 现代化优先
- 优先教授现代C++的最佳实践
- 避免过时的编程风格

### 4. 实用性导向
- 聚焦实际开发中常用的特性
- 避免过于晦涩的语言角落

---

## 🛠️ 技术要求

### 编译器要求
- **统一基线**：C++20；C++11/14/17 标签仅说明引入版本，C++23 仅作延伸阅读。
- **建议工具链**：GCC 13+ / libstdc++ 13+、Clang 18+ / libstdc++ 13+（Linux），或 Visual Studio 2022 17.10+。
- 编译时使用 `-std=c++20`，MSVC 使用 `/std:c++20 /utf-8 /Zc:__cplusplus`。
- 标准库必须提供 `std::format`、Ranges、协程支持及 `std::jthread`。能力检查与官方支持表见[主 README](./README.md)。
- CMake 设置标准为 20、要求该标准并禁用扩展，不提供旧标准回退。

### 开发环境建议
- **编辑器**：VSCode + C/C++扩展 或 CLion
- **构建工具**：CMake 3.21+
- **调试器**：GDB 或 LLDB

### 示例代码编译方式
```bash
# 单文件编译
g++ -std=c++20 -Wall -Wextra -o output example.cpp

# 使用CMake（推荐）
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure
```

---

## 📅 学习路线建议

### 快速回顾路线（约2周）
适合基础较好、时间有限的学习者：
1. 第一章：快速浏览，重点看1.6现代特性总览
2. 第二章：重点学习2.1-2.4
3. 第三章：学习3.1-3.2

### 完整学习路线（约6-8周）
适合系统性学习的开发者：
1. **第1-2周**：完成第一章全部内容
2. **第3-5周**：完成第二章全部内容
3. **第6-8周**：完成第三章内容 + 综合项目

### 项目驱动路线
适合喜欢动手实践的学习者：
1. 快速浏览第一章
2. 边学第二章边做项目1
3. 边学第三章边做项目2、3

---

## ✅ 下一步计划

1. [ ] 创建目录结构
2. [ ] 编写主README.md
3. [ ] 第一章：语法回顾
   - [ ] 章节导读
   - [ ] 各小节内容
   - [ ] 示例代码
4. [ ] 第二章：现代编程范式
   - [ ] 章节导读
   - [ ] 各小节内容
   - [ ] 示例代码
5. [ ] 第三章：算法设计初步
   - [ ] 章节导读
   - [ ] 各小节内容
   - [ ] 示例代码
6. [ ] 综合练习项目

---

## 📝 备注

- 所有 examples/*.cpp 均可按 C++20 独立编译运行，并纳入 CMake / CTest。
- 文档片段会省略上下文；协程提供完整实现链接，模块为多文件语法介绍。
- 项目与 solutions 目录为后续规划，当前仓库尚未提供。
- 每个章节末尾提供练习题和思考题
- 关键概念配有图解说明
- 参考资料和延伸阅读链接

---

*设计版本：v2.0（C++20 基线）*

*创建日期：2025年12月13日*

