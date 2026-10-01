# C++ 现代编程教程

## 📖 项目简介

本教程专为有 C++ 基础但长期未接触代码的开发者设计，旨在帮助你：

**内容与示例统一以 C++20 为基线。** C++11/14/17 标注用于说明特性的引入版本；学习和编译时均使用 C++20。C++23 内容仅作为明确标注的延伸阅读，不是本教程的依赖。

- 🔄 **回顾核心语法**：快速恢复 C++ 编程手感
- 🚀 **学习现代特性**：掌握 C++20 的 Concepts、Ranges、编译期计算和并发工具，理解 C++11/14/17 的演进
- 🎯 **理解编程范式**：深入理解 RAII、智能指针、移动语义等现代编程范式
- 🧮 **算法设计入门**：建立基础算法设计与实现能力

---

## 🎯 适用对象

- ✅ 曾学习过 C++ 基础语法
- ✅ 多年未编写 C++ 代码
- ✅ 缺乏完整项目开发经验
- ✅ 希望学习现代 C++ 的开发者

---

## 📚 教程目录

### 第一章：C++ 语法回顾
> 📁 [01_syntax_review/](./01_syntax_review/)

快速回顾 C++ 核心语法，为学习现代特性打下基础。

| 小节 | 内容 | 链接 |
|------|------|------|
| 1.1 | 基础语法回顾 | [01_basic_syntax.md](./01_syntax_review/01_basic_syntax.md) |
| 1.2 | 指针与引用 | [02_pointers_references.md](./01_syntax_review/02_pointers_references.md) |
| 1.3 | 类与对象 | [03_classes_objects.md](./01_syntax_review/03_classes_objects.md) |
| 1.4 | 模板基础 | [04_templates_basics.md](./01_syntax_review/04_templates_basics.md) |
| 1.5 | STL 容器 | [05_stl_containers.md](./01_syntax_review/05_stl_containers.md) |
| 1.6 | 现代 C++ 新特性总览 | [06_modern_features.md](./01_syntax_review/06_modern_features.md) |

### 第二章：现代编程范式
> 📁 [02_modern_paradigms/](./02_modern_paradigms/)

深入学习现代 C++ 的核心编程范式和最佳实践。

| 小节 | 内容 | 链接 |
|------|------|------|
| 2.1 | RAII 与资源管理 | [01_raii_resource.md](./02_modern_paradigms/01_raii_resource.md) |
| 2.2 | 智能指针详解 | [02_smart_pointers.md](./02_modern_paradigms/02_smart_pointers.md) |
| 2.3 | 移动语义与完美转发 | [03_move_semantics.md](./02_modern_paradigms/03_move_semantics.md) |
| 2.4 | Lambda与函数式编程 | [04_lambda_functional.md](./02_modern_paradigms/04_lambda_functional.md) |
| 2.5 | 类型推断 | [05_type_deduction.md](./02_modern_paradigms/05_type_deduction.md) |
| 2.6 | 编译期计算 | [06_constexpr_compile.md](./02_modern_paradigms/06_constexpr_compile.md) |
| 2.7 | 现代错误处理 | [07_error_handling.md](./02_modern_paradigms/07_error_handling.md) |
| 2.8 | 并发编程入门 | [08_concurrency_intro.md](./02_modern_paradigms/08_concurrency_intro.md) |

### 第三章：算法设计初步
> 📁 [03_algorithm_design/](./03_algorithm_design/)

掌握基础算法设计思想和实现技巧。

| 小节 | 内容 | 链接 |
|------|------|------|
| 3.1 | 复杂度分析 | [01_complexity_analysis.md](./03_algorithm_design/01_complexity_analysis.md) |
| 3.2 | STL算法库 | [02_stl_algorithms.md](./03_algorithm_design/02_stl_algorithms.md) |
| 3.3 | 排序与查找 | [03_sorting_searching.md](./03_algorithm_design/03_sorting_searching.md) |
| 3.4 | 递归与动态规划 | [04_recursion_dp.md](./03_algorithm_design/04_recursion_dp.md) |
| 3.5 | 常用数据结构 | [05_data_structures.md](./03_algorithm_design/05_data_structures.md) |
| 3.6 | 实战练习题 | [06_practical_problems.md](./03_algorithm_design/06_practical_problems.md) |

---

## 🛠️ 环境要求

### 编译器

需要同时支持所用 C++20 特性的编译器和标准库，尤其是 `<format>`、`<ranges>`、`<coroutine>` 和 `std::jthread`。建议使用以下工具链：

| 平台/工具链 | 建议配置 |
|------------|----------|
| GCC | GCC 13+，配套 libstdc++ 13+ |
| Clang（Linux） | Clang 18+，搭配 libstdc++ 13+ |
| MSVC（Windows） | Visual Studio 2022 17.10+，安装“使用 C++ 的桌面开发”及配套标准库 |

这不是对完整 C++20 实现的承诺。Clang 的语言版本与标准库版本是两回事；使用 libc++ 或 Apple Clang 时也需确认所需库特性。CMake 配置会检查 `std::format` 和 `std::jthread`，其余示例在构建时检查，不会静默降级或跳过特性。具体支持情况可查 [libstdc++ 官方状态表](https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2020)、[libc++ 官方状态表](https://libcxx.llvm.org/Status/Cxx20.html)和 [MSVC 官方一致性文档](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance)。

### 编译命令

```bash
# GCC / Clang：编译单个文件
g++ -std=c++20 -Wall -Wextra -o output example.cpp

# 使用线程的示例（thread_demo.cpp、raii_demo.cpp）还需 -pthread
g++ -std=c++20 -Wall -Wextra -pthread -o thread_demo thread_demo.cpp
```

Windows 可在 Visual Studio 的开发者命令提示符中编译：

```bat
cl /std:c++20 /EHsc /W4 /permissive- /utf-8 /Zc:__cplusplus example.cpp /Fe:output.exe
```

### 构建并验证全部示例

项目提供 CMake 3.21+ 配置，从项目根目录执行：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure
```

每个 `.cpp` 对应一个独立程序；CTest 会在构建目录内分别运行它们。Debug 构建启用示例里的运行期断言；编译期断言始终生效。模块章节是多文件语法介绍，需要工具链专用构建流程，不包含在这些单文件示例中。

### 开发环境（建议）
- **编辑器**：VSCode + C/C++ 扩展 或 CLion
- **调试器**：GDB 或 LLDB

---

## 📅 学习路线

### 🚀 快速回顾路线（约 2 周）
适合基础较好、时间有限的学习者：

1. 第一章快速浏览，重点看 1.6 现代特性总览
2. 第二章重点学习 RAII、智能指针、移动语义、模板 Lambda、`consteval` 和 `std::jthread`
3. 第三章学习复杂度分析、STL 算法库和 Ranges

### 📖 完整学习路线（约 6-8 周）
适合系统性学习的开发者：

| 周次 | 内容 |
|------|------|
| 第 1-2 周 | 完成第一章全部内容 |
| 第 3-5 周 | 完成第二章全部内容 |
| 第 6-8 周 | 完成第三章内容 + 综合项目 |

### 🔧 项目驱动路线
适合喜欢动手实践的学习者：

1. 快速浏览第一章
2. 边学第二章边做项目
3. 边学第三章边练习算法题

---

## 📝 学习建议

1. **动手实践**
   - 不要只是阅读，务必编译运行每个示例代码
   - 尝试修改代码，观察结果变化

2. **做笔记**
   - 记录不熟悉或容易遗忘的知识点
   - 总结每章的核心要点

3. **循序渐进**
   - 按顺序学习，每节内容都是后续的基础
   - 不理解的地方可以反复阅读

4. **练习为主**
   - 完成每节末尾的练习题
   - 尝试用学到的知识解决实际问题

---

## 📂 目录结构

```
CPP_Resume/
├── README.md                 # 本文件
├── DESIGN.md                 # 设计文档
├── CMakeLists.txt            # C++20 构建与 CTest 验证
│
├── 01_syntax_review/         # 第一章：语法回顾
│   ├── README.md             # 章节导读
│   ├── 01_basic_syntax.md    # 基础语法
│   ├── 02_pointers_references.md  # 指针与引用
│   ├── 03_classes_objects.md # 类与对象
│   ├── 04_templates_basics.md # 模板基础
│   ├── 05_stl_containers.md  # STL容器
│   ├── 06_modern_features.md # 现代C++特性
│   └── examples/             # 示例代码
│       ├── basic_demo.cpp
│       ├── pointer_demo.cpp
│       ├── class_demo.cpp
│       ├── template_demo.cpp
│       ├── stl_demo.cpp
│       ├── modern_features_demo.cpp # span、比较、format 等
│       └── coroutine_demo.cpp # 自定义协程生成器
│
├── 02_modern_paradigms/      # 第二章：现代编程范式
│   ├── README.md             # 章节导读
│   ├── 01_raii_resource.md   # RAII与资源管理
│   ├── 02_smart_pointers.md  # 智能指针
│   ├── 03_move_semantics.md  # 移动语义
│   ├── 04_lambda_functional.md # Lambda与函数式
│   ├── 05_type_deduction.md  # 类型推断
│   ├── 06_constexpr_compile.md # 编译期计算
│   ├── 07_error_handling.md  # 错误处理
│   ├── 08_concurrency_intro.md # 并发编程
│   └── examples/             # 示例代码
│
└── 03_algorithm_design/      # 第三章：算法设计
    ├── README.md             # 章节导读
    ├── 01_complexity_analysis.md # 复杂度分析
    ├── 02_stl_algorithms.md  # STL算法库
    ├── 03_sorting_searching.md # 排序与查找
    ├── 04_recursion_dp.md    # 递归与动态规划
    ├── 05_data_structures.md # 数据结构
    ├── 06_practical_problems.md # 实战练习
    └── examples/             # 示例代码
```

---

## 🚀 快速开始

```bash
# 进入项目目录
cd CPP_Resume

# 编译第一个示例
cd 01_syntax_review/examples
g++ -std=c++20 -Wall -o basic_demo basic_demo.cpp

# 运行
./basic_demo
```

---

## 📄 许可

本教程仅供学习使用。

---

*创建日期：2025年12月*

