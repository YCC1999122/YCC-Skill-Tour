# C语言学习项目

## 项目简介

本项目是一套完整的 C 语言学习教程，基于 C11 标准，采用模块化架构组织代码，涵盖基础语法、指针与字符串、数据结构、内存管理、文件 IO、设计模式六大核心模块。项目提供交互式菜单主程序，可自由选择运行各模块的演示。每个 `.c` 文件都配有极其详细的中文注释，从 `#include` 到头文件守卫，从指针运算到内存池实现都一一说明，适合从零基础入门到进阶提升。

通过本项目的学习，你可以掌握 C 语言的核心知识体系：数据类型与运算符、控制流与函数、指针与数组、结构体与联合体、动态内存管理（malloc/free）、内存池与引用计数、文件读写、以及用纯 C 模拟面向对象和设计模式（虚函数表、单例、观察者、策略、工厂、状态机）的高级技巧。

## 目录结构

```
c-learning-project/
├── include/                        # 头文件目录（对外接口声明）
│   ├── basics.h                    # 基础语法头文件（宏定义MAX/MIN、状态码枚举、函数声明、内联函数、函数指针类型）
│   ├── pointers.h                  # 指针模块头文件（指针演示、动态数组、自实现字符串函数my_strlen/my_strcpy等）
│   ├── datastruct.h                # 数据结构头文件（枚举Weekday、位域StatusRegister、结构体Date/Variant、链表/栈/队列）
│   ├── memory.h                    # 内存管理头文件（内存池MemoryPool、引用计数RefCounted、调试版MALLOC/FREE宏）
│   ├── file_io.h                   # 文件IO头文件（文本/二进制读写、文件大小、文件存在性检查）
│   └── patterns.h                  # 设计模式头文件（Shape虚函数表模拟OOP、单例、观察者、策略、工厂、状态机）
├── src/                            # 源文件目录（实现细节）
│   ├── main.c                      # 主程序入口（交互式菜单，MenuItem结构体+函数指针表驱动）
│   ├── basics/
│   │   └── basics.c                # 基础语法实现（数据类型、运算符、控制流、递归/可变参数、存储类、类型转换、位运算）
│   ├── pointers/
│   │   └── pointers.c              # 指针实现（指针运算、多级指针、指针与数组、自实现字符串库、二维数组、回调表）
│   ├── datastruct/
│   │   ├── datastruct.c            # 数据结构实现（结构体/联合体/枚举/位域、单链表、栈、队列）
│   │   └── memory.c                # 内存管理实现（malloc/calloc/realloc、内存池、引用计数、泄漏检测）
│   ├── io/
│   │   └── file_io.c               # 文件IO实现（文本写入/读取、二进制写入/读取、格式化输出）
│   └── patterns/
│       └── patterns.c              # 设计模式实现（虚表模拟多态、Rectangle/Circle派生、单例、观察者、策略、状态机）
├── tests/                          # 测试代码目录（预留，可自行编写单元测试）
├── CMakeLists.txt                  # CMake构建脚本（C11标准，-Wall -Wextra警告）
├── Makefile                        # Make构建脚本（适用于GCC/MinGW环境）
├── build.bat                       # Windows一键编译批处理脚本
└── README_C.md                     # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（basics）
- `include/basics.h` - 先读头文件，了解模块对外接口与宏定义
- `src/basics/basics.c` - 数据类型、运算符、控制流、函数

核心知识点：
- 基本数据类型及占用字节数（`char`/`short`/`int`/`long`/`long long`/`float`/`double`）
- C99 的 `bool` 类型（`stdbool.h`）与枚举类型 `enum`
- `sizeof` 运算符与类型极限值宏（`INT_MAX`/`SHRT_MAX` 等，`limits.h`/`float.h`）
- 运算符优先级、自增自减的前置/后置区别
- 控制流：`if/else`、`switch/case`、`for`/`while`/`do-while`、`break`/`continue`
- 函数：递归（阶乘）、内联函数（`inline`）、可变参数函数（`stdarg.h` 的 `va_list`）
- 存储类关键字：`static`、`extern`、`register`、`volatile`
- 类型转换（隐式/显式）与整型提升规则
- 位运算：`&`、`|`、`^`、`~`、`<<`、`>>` 及常见位操作技巧

### 第二阶段：指针与字符串（pointers）
- `include/pointers.h` - 指针模块接口
- `src/pointers/pointers.c` - 指针、数组、字符串

核心知识点：
- 指针的本质：存储内存地址的变量，`&` 取地址与 `*` 解引用
- 空指针 `NULL` 与野指针的危害
- 指针运算：`p++`、`p+=n` 按类型大小步进
- 多级指针（`int **pp`）与指针链
- 指针与数组的关系：数组名退化为首元素指针
- 动态数组的创建、反转、查找最大值
- 自实现字符串函数：`my_strlen`、`my_strcpy`、`my_strcat`、`my_strcmp`、`my_strchr`
- 二维数组与指针数组的区别
- 函数指针数组实现回调表（菜单驱动的基础）
- `void*` 通用指针与 `const` 指针的三种写法

### 第三阶段：数据结构（datastruct）
- `include/datastruct.h` - 数据结构类型定义
- `src/datastruct/datastruct.c` - 结构体、联合体、枚举、链表、栈、队列

核心知识点：
- 结构体 `struct` 的定义、初始化与 C99 指定初始化器（`.year = 2025`）
- 结构体内存对齐与 `sizeof` 结果的关系
- 位域（`StatusRegister`）压缩存储标志位，嵌入式开发常用
- 联合体 `union`：成员共用内存，大小取最大成员
- 枚举在状态表示中的应用（`Weekday`、`SystemState`）
- 单链表：节点定义、头插/尾插、删除、遍历、销毁
- 栈（LIFO）与队列（FIFO）的实现与应用场景
- 函数指针作为结构体成员实现"方法"

### 第四阶段：内存管理（memory）
- `include/memory.h` - 内存管理接口
- `src/datastruct/memory.c` - malloc/free、内存池、引用计数、泄漏检测

核心知识点：
- 堆与栈的区别：栈自动管理、堆手动管理
- `malloc`（未初始化）、`calloc`（清零）、`realloc`（扩容）的区别
- `free` 的正确使用：释放后指针置 `NULL`
- 四大内存错误：重复释放、释放后使用、内存泄漏、缓冲区溢出
- `memcpy`、`memset`、`memmove` 的用法与区别
- 内存池原理：预分配固定大小块，减少碎片、提高分配速度
- 引用计数模拟智能指针：`retain`/`release` 自动释放
- 调试版 `MALLOC`/`FREE` 宏：记录 `__FILE__`/`__LINE__` 追踪泄漏

### 第五阶段：文件IO（io）
- `include/file_io.h` - 文件操作接口
- `src/io/file_io.c` - 文本/二进制文件读写

核心知识点：
- `FILE*` 文件指针与 `fopen`/`fclose` 的配对使用
- 打开模式：`"r"`/`"w"`/`"a"`/`"rb"`/`"wb"` 的区别
- 文本文件读写：`fputs`/`fgets`、整文件读取的缓冲区分配
- 二进制文件读写：`fwrite`/`fread` 按字节块操作
- `fseek`/`ftell` 获取文件大小
- 格式化输出：`fprintf`/`snprintf` 防止缓冲区溢出
- 每次打开文件后检查返回值是否为 `NULL`

### 第六阶段：设计模式（patterns）
- `include/patterns.h` - 虚表与模式接口定义
- `src/patterns/patterns.c` - OOP 模拟与经典设计模式

核心知识点：
- 用结构体+函数指针表（vtable）模拟 C++ 的虚函数与多态
- 基类子对象放在派生结构体第一个成员，实现指针安全转换（继承模拟）
- 单例模式：全局唯一实例的 C 实现
- 观察者模式：回调函数注册与事件通知
- 策略模式：运行时切换算法（函数指针替换）
- 工厂模式：统一创建接口隐藏具体构造细节
- 状态机（FSM）：状态表驱动的事件处理，嵌入式与协议解析常用

## 开发环境

### 1. GCC / MinGW-w64（编译器）
- **用途**：C 语言编译的主流编译器，Linux 原生、Windows 通过 MinGW-w64 使用
- **下载地址**：https://www.mingw-w64.org/ 或 https://winlibs.com/（免安装压缩包）
- **版本说明**：免费开源，建议 GCC 11 及以上，完整支持 C11 标准
- **特点**：编译速度快、警告信息详细（`-Wall -Wextra`）、跨平台

### 2. CMake（构建工具）
- **用途**：跨平台构建脚本生成器，本项目 `CMakeLists.txt` 用它组织多文件编译
- **下载地址**：https://cmake.org/download/
- **使用方式**：`mkdir build && cd build && cmake .. && cmake --build .`
- **特点**：一份脚本生成 VS 工程 / Makefile / Ninja 文件，是中大型 C/C++ 项目的事实标准

### 3. Make（构建工具，可选）
- **用途**：直接执行项目根目录的 `Makefile`，一条 `make` 命令完成编译
- **获取方式**：Windows 下 MinGW-w64 自带 `mingw32-make`；Linux 自带 `make`
- **特点**：轻量直接，适合快速编译验证

### 4. IDE 推荐
- **Visual Studio 2022 Community**（Windows）：https://visualstudio.microsoft.com/ ，免费、调试器强大，自带 MSVC 编译器
- **CLion**：https://www.jetbrains.com/clion/ ，JetBrains 出品，原生支持 CMake，重构与代码分析优秀（收费，学生免费）
- **VS Code + C/C++ 插件**：https://code.visualstudio.com/ ，轻量免费，配合 GCC + CMake 插件体验良好

## C语言基础知识

### 基本数据类型（64 位系统典型值）
| 类型 | 字节数 | 取值范围/说明 |
|------|--------|----------------|
| `char` | 1 | -128 ~ 127，字符以 ASCII 码存储 |
| `unsigned char` | 1 | 0 ~ 255，常用于字节操作 |
| `short` | 2 | -32768 ~ 32767 |
| `int` | 4 | 约 ±21 亿 |
| `unsigned int` | 4 | 0 ~ 约 42 亿 |
| `long` | 4（Windows）/ 8（Linux） | 注意跨平台差异 |
| `long long` | 8 | 约 ±9.2×10^18 |
| `float` | 4 | 约 7 位有效数字 |
| `double` | 8 | 约 15~16 位有效数字 |
| `bool`（C99） | 1 | `true`/`false`，需包含 `stdbool.h` |
| 指针 | 8（64 位） | 与所指类型无关，统一为地址宽度 |

### 指针要点速查
| 声明 | 含义 |
|------|------|
| `int *p` | 指向 int 的指针 |
| `int **pp` | 指向 int 指针的指针（二级指针） |
| `int *arr[10]` | 含 10 个 int 指针的数组 |
| `int (*arr)[10]` | 指向含 10 个 int 数组的指针 |
| `int *f(int)` | 返回 int 指针的函数 |
| `int (*f)(int)` | 指向"接收 int 返回 int"函数的指针 |
| `const int *p` | 指向的内容不可通过 p 修改 |
| `int * const p` | 指针 p 本身不可改指向 |

### 动态内存函数
| 函数 | 作用 | 注意 |
|------|------|------|
| `malloc(n)` | 分配 n 字节，内容未初始化 | 必须检查返回值是否为 NULL |
| `calloc(cnt, size)` | 分配 cnt×size 字节并清零 | 比 malloc 稍慢 |
| `realloc(p, n)` | 调整已分配内存大小 | 可能移动内存，原指针会失效 |
| `free(p)` | 释放堆内存 | 释放后应立即 `p = NULL` |
| `memcpy(d, s, n)` | 复制 n 字节（区域不可重叠） | 重叠时用 `memmove` |
| `memset(p, v, n)` | 按字节填充 | 第二个参数按字节截断 |

### 预处理器
- `#include`：`<>` 包含系统头文件，`""` 包含项目头文件
- 头文件守卫三件套：`#ifndef XXX_H` / `#define XXX_H` / `#endif`，防止重复包含
- `#define` 宏：常量与宏函数，宏函数务必给参数加括号（如 `#define MAX(a,b) ((a)>(b)?(a):(b))`）
- 条件编译：`#ifdef` / `#if` / `#else` / `#endif`，用于调试开关与跨平台适配
- 预定义宏：`__FILE__`、`__LINE__`、`__func__`，调试与日志利器

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.c` 文件的中文注释，从头文件到实现都不要跳过。
2. **先跑起来再改**：先用 `build.bat` 或 CMake 编译运行，通过交互菜单把每个模块的演示都看一遍，建立整体印象。
3. **动手修改验证**：把指针偏移量、内存池块大小、链表操作顺序改一改，观察输出变化，比单纯看书有效得多。
4. **善用调试器**：在 VS / CLion / VS Code 中单步跟踪指针变化和内存分配，亲眼看到 `*ptr = 100` 如何修改变量。
5. **循序渐进**：按 6 个阶段的顺序学习，指针和内存管理是 C 语言的两大难关，值得反复研读。
6. **对照头文件学习**：头文件是模块的"说明书"，先看 `.h` 了解接口，再看 `.c` 理解实现，养成工程化阅读习惯。

## 注意事项

### 1. 指针三大陷阱
- **野指针**：声明后未初始化就使用，指向随机地址，务必初始化为 `NULL` 或有效地址
- **空指针解引用**：对 `NULL` 指针使用 `*` 会导致段错误（程序崩溃），解引用前必须判空
- **悬垂指针**：`free` 后指针仍指向原地址，再次使用属未定义行为，`free` 后立即置 `NULL`

### 2. 内存泄漏与 double free
- 每次 `malloc` 必须有对应的 `free`，函数的所有返回路径（含出错分支）都要检查
- 同一块内存只能 `free` 一次，重复释放会破坏堆管理结构导致崩溃
- 本项目的 `MALLOC`/`FREE` 调试宏可自动记录分配位置，用 `debug_memory_leak_check()` 排查泄漏

### 3. 缓冲区溢出
- `strcpy`/`strcat`/`sprintf` 不检查目标缓冲区大小，是安全漏洞的重灾区
- 优先使用 `strncpy`/`snprintf` 等带长度限制的版本
- 数组越界写入不会立刻报错，但会破坏相邻内存，属于最难排查的 bug 类型

### 4. 未定义行为（UB）
- 有符号整数溢出、移位超过位宽、解引用无效指针、修改字符串常量等都是未定义行为
- UB 的表现不确定：可能正常运行、可能崩溃、可能输出奇怪结果，换编译器结果可能完全不同
- 开启 `-Wall -Wextra` 警告并逐条消除，是规避 UB 的有效手段

### 5. 跨平台差异
- `long` 在 Windows（LLP64）为 4 字节，在 Linux（LP64）为 8 字节
- 需要确定宽度的整数请使用 `stdint.h` 的 `int32_t`、`uint64_t` 等类型
- 文件路径分隔符、换行符（`\r\n` vs `\n`）也存在平台差异

## 学习资源推荐

### 推荐书籍
1. 《C 程序设计语言》（K&R）- Brian W. Kernighan（C 语言圣经，必读）
2. 《C Primer Plus》（第 6 版）- Stephen Prata（系统全面，适合入门）
3. 《C 和指针》- Kenneth A. Reek（指针专题，进阶必读）
4. 《C 陷阱与缺陷》- Andrew Koenig（避开常见坑）
5. 《C 专家编程》- Peter van der Linden（深入理解编译器视角）

### 在线教程网址
- C 语言中文教程（菜鸟教程）：https://www.runoob.com/cprogramming/c-tutorial.html
- cppreference（C 标准库权威参考）：https://zh.cppreference.com/w/c
- C 语言中文网：http://c.biancheng.net/c/
- GCC 官方文档：https://gcc.gnu.org/onlinedocs/
- CMake 官方教程：https://cmake.org/cmake/help/latest/guide/tutorial/
- B 站 C 语言视频教程：https://www.bilibili.com/

---

**祝学习愉快！指针虽难，跨越之后便是坦途。如有问题请参考各文件中的详细注释。**
