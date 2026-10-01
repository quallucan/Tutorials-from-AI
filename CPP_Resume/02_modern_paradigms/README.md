# 第二章：现代 C++ 编程范式

## 📖 章节概述

本章是整个教程的核心，将深入讲解现代 C++ 的编程范式和最佳实践。如果你只能学习一章，那就是这一章——它涵盖了区分"传统 C++"和"现代 C++"的关键内容。

我们将从 RAII（资源获取即初始化）这一 C++ 最重要的设计原则开始，逐步深入到智能指针、移动语义、Lambda 表达式等核心特性，最后介绍并发编程的基础。

所有示例采用 **C++20**。模板 Lambda、`consteval`、`constinit`、`std::jthread` 与协作式停止都属于本章实践内容；携带错误信息使用 `std::variant` 构建 Result。

## 🎯 学习目标

完成本章学习后，你将能够：

- ✅ 理解并应用 RAII 原则管理资源
- ✅ 熟练使用 `unique_ptr`、`shared_ptr`、`weak_ptr`
- ✅ 理解移动语义，编写高效的资源管理代码
- ✅ 使用 Lambda 表达式进行函数式编程
- ✅ 掌握 `auto`、`decltype` 等类型推断机制
- ✅ 使用 `constexpr`、`consteval` 和 `constinit` 区分编译期计算与初始化
- ✅ 采用现代方式处理错误（异常、optional、基于 variant 的 Result）
- ✅ 使用 `std::jthread`、`std::stop_token` 和 C++20 同步工具

## 📚 章节目录

| 序号 | 文档 | 主题 | 预计时长 |
|------|------|------|----------|
| 2.1 | [RAII与资源管理](./01_raii_resource.md) | RAII原则、资源所有权、作用域管理 | 2小时 |
| 2.2 | [智能指针详解](./02_smart_pointers.md) | unique_ptr、shared_ptr、weak_ptr | 3小时 |
| 2.3 | [移动语义与完美转发](./03_move_semantics.md) | 右值引用、std::move、std::forward | 3小时 |
| 2.4 | [Lambda与函数式编程](./04_lambda_functional.md) | Lambda语法、捕获、高阶函数 | 2小时 |
| 2.5 | [类型推断](./05_type_deduction.md) | auto、decltype、decltype(auto) | 1.5小时 |
| 2.6 | [编译期计算](./06_constexpr_compile.md) | constexpr、consteval、constinit、编译期容器 | 2小时 |
| 2.7 | [现代错误处理](./07_error_handling.md) | 异常、std::optional、std::variant / Result | 2小时 |
| 2.8 | [并发编程入门](./08_concurrency_intro.md) | jthread、stop_token、同步工具、future/promise | 3小时 |

## 💻 示例代码

每节都配有可运行的示例代码，位于 `examples/` 目录：

```
examples/
├── raii_demo.cpp           # RAII 示例
├── smart_ptr_demo.cpp      # 智能指针示例
├── move_demo.cpp           # 移动语义示例
├── lambda_demo.cpp         # Lambda 示例
├── type_deduction_demo.cpp # 类型推断示例
├── constexpr_demo.cpp      # constexpr 示例
├── error_demo.cpp          # 错误处理示例
└── thread_demo.cpp         # 并发编程示例
```

### 编译运行示例

```bash
# 进入示例目录
cd examples

# 编译（使用 C++20 标准）
g++ -std=c++20 -Wall -pthread -o raii_demo raii_demo.cpp

# 并发示例需要链接 pthread
g++ -std=c++20 -Wall -pthread -o thread_demo thread_demo.cpp

# 运行
./raii_demo
```

## 🔑 核心概念预览

### RAII（资源获取即初始化）
> "构造函数获取资源，析构函数释放资源"

这是 C++ 最重要的编程范式，是智能指针、容器、锁等一切资源管理的基础。

### 移动语义
> "转移而非拷贝"

C++11 引入的移动语义让我们可以高效地转移资源所有权，避免不必要的深拷贝。

### 值类别
> "左值 vs 右值"

理解左值和右值是掌握移动语义的前提。

### 智能指针
> "永远不要使用裸 new/delete"

`unique_ptr`、`shared_ptr` 让内存管理自动化，杜绝内存泄漏。

## 📝 学习建议

1. **理解原理比记忆语法更重要**：本章的概念相对抽象，请务必理解"为什么"
2. **多看编译器输出**：添加 `-fno-elide-constructors` 关闭拷贝消除，观察对象生命周期
3. **调试观察**：使用调试器单步执行，观察对象的构造和析构
4. **动手实验**：修改示例代码，验证你的理解

## ⚠️ 本章难度提示

本章内容是 C++ 进阶的核心，难度相对较高。如果第一遍没有完全理解，这是正常的。建议：

1. 先通读一遍，建立整体印象
2. 结合示例代码动手实践
3. 遇到困难时回顾第一章的相关基础
4. 多次复习，逐步深入理解

## ⏭️ 开始学习

准备好了吗？让我们从 [2.1 RAII与资源管理](./01_raii_resource.md) 开始！

---

*预计总学习时长：18.5 小时*

