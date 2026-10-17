# C++学习项目

## 项目简介

本项目是一套完整的 C++ 学习教程，基于 C++17 标准，采用"头文件声明 + 源文件实现"的模块化架构，涵盖基础语法、面向对象编程、STL 容器与算法、高级特性、设计模式五大核心模块。项目提供交互式菜单主程序，可自由选择运行各模块的演示。每个 `.cpp` 文件都配有极其详细的中文注释，从初始化列表到移动语义，从虚表原理到智能指针都逐行说明，适合从 C 语言过渡到 C++、系统掌握现代 C++ 的学习者。

通过本项目的学习，你可以掌握现代 C++ 的核心知识体系：引用与右值引用、类的构造/析构/拷贝/移动、继承与多态、STL 容器与迭代器、智能指针与 RAII、Lambda 表达式与模板编程，以及单例、工厂、观察者、策略等经典设计模式的 C++ 实现。

## 目录结构

```
cpp-learning-project/
├── include/                        # 头文件目录（类与函数声明）
│   ├── basics.h                    # 基础语法头文件（数据类型、auto/decltype、引用、范围for、命名空间演示函数）
│   ├── oop.h                       # OOP头文件（Point类：构造/析构/拷贝/移动/运算符重载，封装/继承/多态演示）
│   ├── stl_containers.h            # STL头文件（顺序容器、关联容器、string操作、算法演示函数，stl_demo命名空间）
│   ├── advanced.h                  # 高级特性头文件（智能指针、Lambda、移动语义、模板、异常，advanced命名空间）
│   └── patterns.h                  # 设计模式头文件（饿汉/懒汉单例、工厂、观察者等，patterns命名空间）
├── src/                            # 源文件目录（实现细节）
│   ├── main.cpp                    # 主程序入口（交互式菜单，横幅打印，选项分发到各模块）
│   ├── basics/
│   │   └── basics.cpp              # 基础语法实现（类型推导、初始化列表、左右值引用、结构化绑定、内联命名空间）
│   ├── oop/
│   │   └── oop.cpp                 # OOP实现（Point类全套特殊成员函数、继承多态、抽象类、多重继承、虚继承、友元）
│   ├── stl/
│   │   └── stl_containers.cpp      # STL实现（vector/list/deque/array、map/set/unordered系列、string、算法与迭代器）
│   ├── advanced/
│   │   └── advanced.cpp            # 高级特性实现（unique_ptr/shared_ptr/weak_ptr、Lambda闭包、移动语义、模板编程）
│   └── patterns/
│       └── patterns.cpp            # 设计模式实现（饿汉/懒汉单例、线程安全单例、工厂、观察者、策略等）
├── CMakeLists.txt                  # CMake构建脚本（C++17标准，禁用编译器扩展保证可移植性）
├── Makefile                        # Make构建脚本（适用于GCC/MinGW环境）
├── build.bat                       # Windows一键编译批处理脚本
└── README_CPP.md                   # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（basics）
- `include/basics.h` - 基础语法模块声明
- `src/basics/basics.cpp` - 从 C 到 C++ 的语法升级

核心知识点：
- `auto` 类型推导与 `decltype` 的区别与使用场景
- 统一初始化（列表初始化 `{}`）与 `initializer_list`
- 左值引用 `T&` 与右值引用 `T&&` 的本质区别
- 引用 vs 指针：必须初始化、不可改指向、无空引用
- 范围 `for` 循环（`for (auto& x : container)`）替代传统下标遍历
- 结构化绑定（C++17）：`auto [key, value] = pair;`
- 命名空间 `namespace` 与内联命名空间，解决命名冲突
- `using` 声明 vs `using namespace` 的利弊（头文件中禁用后者）

### 第二阶段：面向对象编程（oop）
- `include/oop.h` - Point 类与 OOP 演示声明
- `src/oop/oop.cpp` - 类与对象、封装继承多态

核心知识点：
- 类的特殊成员函数：默认构造、带参构造、析构、拷贝构造、移动构造、拷贝赋值、移动赋值
- 初始化列表：比在函数体赋值更高效，顺序必须与成员声明顺序一致
- `this` 指针：指向当前对象的隐含指针
- 拷贝构造的参数必须是 `const` 引用，否则无限递归
- 深拷贝 vs 浅拷贝：含裸指针成员的类必须自定义拷贝语义
- 运算符重载（`+`、`<<`、`==` 等）的两种形式：成员函数 vs 友元函数
- 继承三方式：`public`/`protected`/`private` 继承对访问权限的影响
- 虚函数与多态：基类指针/引用调用派生类实现，虚表（vtable）原理
- 纯虚函数与抽象类（接口）：`= 0` 声明，不可实例化
- 多重继承的菱形问题与虚继承 `virtual` 解决方案
- 静态成员、const 成员函数、友元函数/友元类

### 第三阶段：STL 容器与算法（stl）
- `include/stl_containers.h` - STL 演示声明（`stl_demo` 命名空间）
- `src/stl/stl_containers.cpp` - 容器、迭代器、算法

核心知识点：
- 顺序容器选型：`vector`（首选，连续内存）/ `list`（双向链表，任意位置 O(1) 插入）/ `deque`（双端高效）/ `array`（定长）
- `vector` 的扩容机制：容量通常 2 倍增长，`reserve` 预分配避免反复搬迁
- 关联容器：`map`/`set` 基于红黑树（有序，O(log n)）；`unordered_map`/`unordered_set` 基于哈希表（无序，平均 O(1)）
- 迭代器：容器与算法之间的桥梁，begin()/end() 半开区间
- `std::string` 的常用操作：拼接、查找、替换、截取、比较
- 常用算法：`sort`、`find`、`for_each`、`accumulate`（`<numeric>`）、`transform`
- 迭代器失效问题：`vector` 插入导致全部失效，`list` 仅被删元素失效

### 第四阶段：高级特性（advanced）
- `include/advanced.h` - 高级特性声明（`advanced` 命名空间）
- `src/advanced/advanced.cpp` - 智能指针、Lambda、移动语义、模板

核心知识点：
- RAII 思想：资源获取即初始化，析构函数自动释放
- `unique_ptr`：独占所有权，不可拷贝可移动，开销为零的首选
- `shared_ptr`：共享所有权，引用计数归零自动释放
- `weak_ptr`：弱引用，打破 `shared_ptr` 循环引用
- `make_unique`/`make_shared`：比直接 `new` 更安全高效的创建方式
- Lambda 表达式：`[捕获列表](参数) -> 返回类型 { 函数体 }`，值捕获/引用捕获/泛型 Lambda
- 移动语义：移动构造 + `std::move`，避免深拷贝的性能利器
- 模板编程：函数模板、类模板、模板特化，泛型编程基础
- 异常处理：`try`/`catch`/`throw`，标准异常类体系（`<stdexcept>`）

### 第五阶段：设计模式（patterns）
- `include/patterns.h` - 模式类声明（`patterns` 命名空间）
- `src/patterns/patterns.cpp` - 经典设计模式的现代 C++ 实现

核心知识点：
- 单例模式：饿汉式（静态初始化，天然线程安全）vs 懒汉式（延迟加载，需处理线程安全）
- 用 `= delete` 禁用拷贝构造与赋值，是现代 C++ 单例的标准写法
- 工厂模式：用 `unique_ptr` 返回产品对象，所有权清晰
- 观察者模式：`vector` 存储观察者回调，事件发生时批量通知
- 策略模式：算法封装为可替换对象，运行时动态切换
- 设计模式不是目的而是手段：理解"面向接口编程、组合优于继承"的设计思想

## 开发环境

### 1. GCC / MinGW-w64 或 MSVC（编译器）
- **用途**：C++ 编译。GCC 跨平台，MSVC 是 Windows 原生，Clang 跨平台且报错友好
- **下载地址**：GCC/MinGW-w64：https://www.mingw-w64.org/ 或 https://winlibs.com/ ；MSVC 随 Visual Studio 安装
- **版本说明**：建议 GCC 9+ / MSVC 2019+，完整支持 C++17
- **特点**：GCC 对标准跟进快；MSVC 调试体验最佳；Clang 错误提示最人性化

### 2. CMake（构建工具）
- **用途**：跨平台构建脚本生成器，本项目 `CMakeLists.txt` 设置 C++17 标准并禁用编译器扩展
- **下载地址**：https://cmake.org/download/
- **使用方式**：`mkdir build && cd build && cmake .. && cmake --build .`
- **特点**：`CMAKE_CXX_STANDARD 17` 一行指定标准；`CMAKE_CXX_EXTENSIONS OFF` 保证代码可移植

### 3. IDE 推荐
- **Visual Studio 2022 Community**（Windows 首选）：https://visualstudio.microsoft.com/ ，免费，MSVC + 强大调试器，直接打开 CMake 项目
- **CLion**：https://www.jetbrains.com/clion/ ，原生 CMake 支持，代码分析与重构顶级（收费，学生免费）
- **VS Code + C/C++ 插件**：https://code.visualstudio.com/ ，轻量免费，搭配 CMake Tools 插件体验良好

### 4. 在线编译器（免安装快速验证）
- **Compiler Explorer（godbolt）**：https://godbolt.org/ ，可切换编译器版本、查看汇编输出
- **Wandbox**：https://wandbox.org/ ，支持几乎所有 C++ 标准版本

## C++基础知识

### C++ 与 C 的关键差异
| 特性 | C | C++ |
|------|---|-----|
| 内存管理 | `malloc`/`free` | `new`/`delete`、智能指针（推荐） |
| 输入输出 | `printf`/`scanf` | `cout`/`cin`（iostream） |
| 字符串 | 字符数组 + 手动管理 | `std::string` 自动管理 |
| 函数传参 | 值传递 / 指针 | 值传递 / 指针 / 引用（推荐） |
| 类型推导 | 无 | `auto`、`decltype` |
| 泛型 | `void*` + 宏 | 模板（类型安全） |
| 面向对象 | 结构体 + 函数指针模拟 | 原生类、继承、多态 |

### 常用 STL 容器速查
| 容器 | 底层结构 | 随机访问 | 插入/删除 | 适用场景 |
|------|----------|----------|-----------|----------|
| `vector` | 动态数组 | O(1) | 尾部 O(1)，中间 O(n) | 默认首选 |
| `list` | 双向链表 | 不支持 | 任意位置 O(1) | 频繁中间插删 |
| `deque` | 分段连续 | O(1) | 双端 O(1) | 双端队列 |
| `array` | 定长数组 | O(1) | 不可增删 | 编译期定长 |
| `map` | 红黑树 | 按键 O(log n) | O(log n) | 有序键值对 |
| `unordered_map` | 哈希表 | 按键平均 O(1) | 平均 O(1) | 高频查找 |
| `set` | 红黑树 | — | O(log n) | 有序去重 |
| `unordered_set` | 哈希表 | — | 平均 O(1) | 快速去重 |

### 智能指针对比
| 智能指针 | 所有权 | 可否拷贝 | 开销 | 典型用途 |
|----------|--------|----------|------|----------|
| `unique_ptr` | 独占 | 否（可移动） | 零开销 | 默认首选，唯一持有者 |
| `shared_ptr` | 共享（引用计数） | 是 | 控制块 + 原子计数 | 多方共享同一对象 |
| `weak_ptr` | 观察（不计数） | 是 | 同 shared 控制块 | 打破循环引用、缓存 |

### 左值与右值
- **左值（lvalue）**：有名字、可取地址的持久对象，如变量、解引用结果
- **右值（rvalue）**：临时对象、字面量，如 `a + b` 的结果、`std::move(x)` 的返回值
- 左值引用 `T&` 只能绑左值；`const T&` 可绑一切；右值引用 `T&&` 绑右值，是移动语义的基础
- `std::move` 不移动任何东西，只是无条件转换为右值引用，"搬运"由移动构造函数完成

### 构造与析构调用顺序
- 构造：基类构造 → 成员对象构造（按声明顺序）→ 自身构造函数体
- 析构：自身析构函数体 → 成员对象析构（声明逆序）→ 基类析构
- 基类析构函数务必声明为 `virtual`，否则通过基类指针 `delete` 派生对象时派生部分不会析构（内存泄漏）

## 学习方法建议

1. **先读后跑再改**：先读头文件了解类与函数设计，编译运行看菜单输出，再修改参数观察变化。
2. **逐行研读注释**：本项目每个 `.cpp` 都有逐行中文注释，特别是 `oop.cpp` 中构造/析构的打印语句，跑一遍就能看到对象生命周期全貌。
3. **对照 C 语言学习**：本项目与 C 语言学习项目结构呼应，对比 C 的虚表模拟与 C++ 的原生虚函数，能深刻理解多态本质。
4. **优先使用现代特性**：写练习代码时坚持用 `std::string`、`vector`、`unique_ptr`，不要退回裸 `new`/`delete` 和字符数组。
5. **用调试器观察对象**：在构造/析构/拷贝处打断点，观察成员值与地址，亲手验证深拷贝与移动的区别。
6. **循序渐进**：按 5 个阶段推进，STL 不熟时先别急着学模板，智能指针不熟时先别碰设计模式。

## 注意事项

### 1. 对象生命周期与拷贝陷阱
- 含裸指针成员的类必须自定义拷贝构造与赋值运算符（深拷贝），否则默认浅拷贝会导致双重释放
- "三五法则"：自定义了析构/拷贝构造/拷贝赋值之一，通常三个都要自定义；C++11 后还要考虑移动构造/移动赋值
- 函数参数优先传 `const T&`，避免不必要的拷贝

### 2. 引用使用的边界
- 引用必须在定义时初始化，且终身不可改指向
- 不要返回局部变量的引用/指针（悬垂引用，未定义行为）
- 范围 `for` 中修改元素要用 `auto&`，`auto` 只是拷贝

### 3. 迭代器失效
- `vector` 的 `push_back` 触发扩容后，所有迭代器、指针、引用全部失效
- 遍历中删除元素：顺序容器用 `it = vec.erase(it);` 的返回值继续；关联容器 C++11 起同样支持
- 不要在遍历 `vector` 的同时无保护地 `push_back`

### 4. 智能指针使用纪律
- 不要用同一个裸指针构造两个 `shared_ptr`（双重计数导致 double free）
- 类成员间相互持有要用 `weak_ptr` 打破循环引用，否则内存永不释放
- `shared_ptr` 不是线程安全的银弹：计数本身线程安全，所指对象不保证

### 5. 头文件与命名空间纪律
- 头文件中永远不要写 `using namespace std;`，会污染所有包含者的命名空间
- 头文件用 `#ifndef`/`#define`/`#endif` 或 `#pragma once` 防止重复包含
- 类内定义的成员函数隐式内联；模板实现必须放在头文件中

## 学习资源推荐

### 推荐书籍
1. 《C++ Primer》（第 5 版）- Stanley B. Lippman（系统全面的入门圣经）
2. 《Effective C++》（第 3 版）- Scott Meyers（55 条黄金准则，进阶必读）
3. 《Effective Modern C++》- Scott Meyers（C++11/14 现代特性 42 条准则）
4. 《STL 源码剖析》- 侯捷（深入容器与迭代器实现原理）
5. 《深度探索 C++ 对象模型》- Stanley B. Lippman（理解虚表、继承的底层布局）

### 在线教程网址
- cppreference（C++ 权威参考，常备手册）：https://zh.cppreference.com/
- C++ 中文教程（菜鸟教程）：https://www.runoob.com/cplusplus/cpp-tutorial.html
- 现代 C++ 教程（Change Log，免费开源）：https://changkun.de/modern-cpp/
- GCC 官方文档：https://gcc.gnu.org/onlinedocs/
- Compiler Explorer（在线看汇编）：https://godbolt.org/
- B 站 C++ 视频教程：https://www.bilibili.com/

---

**祝学习愉快！从 C 到现代 C++ 的蜕变，就从每一个演示开始。如有问题请参考各文件中的详细注释。**
