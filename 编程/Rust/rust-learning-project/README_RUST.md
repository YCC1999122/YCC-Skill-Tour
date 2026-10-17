# Rust 学习项目

## 项目简介

本项目是一套完整的 Rust 语言学习教程，基于 Rust 2021 edition 与 Cargo 工具链，从基础语法、所有权与借用、结构体与枚举、集合与迭代器、Trait 面向对象到智能指针、无畏并发、设计模式逐步进阶。每个 `.rs` 文件都配有极其详细的逐行中文注释，从 `let` 绑定到生命周期标注、从 `Box<T>` 到 `Arc<Mutex<T>>` 都一一说明，适合零基础入门到进阶学习。

项目提供 `main.rs` 交互式菜单主程序，运行 `cargo run` 后可按数字选择进入任意模块的演示。通过本项目的学习，你可以掌握 Rust 的核心知识体系：所有权三法则、借用检查、生命周期、模式匹配、`Option`/`Result` 错误处理、Trait 系统、泛型、闭包、智能指针、`Send`/`Sync` 无畏并发等，并能够独立编写内存安全、零成本抽象的中型 Rust 程序。

## 目录结构

```
rust-learning-project/
├── Cargo.toml                      # 项目配置文件（包元信息、edition、依赖、编译优化配置）
├── src/
│   ├── main.rs                     # 主程序入口（mod 声明各模块，交互式菜单，std::io 读取输入）
│   ├── basics.rs                   # 第1章：基础语法（let/mut、标量复合类型、函数、控制流、match、String vs &str）
│   ├── ownership.rs                # 第2章：所有权与借用（三法则、移动语义、引用、借用规则、生命周期）
│   ├── structs_enums.rs            # 第3章：结构体与枚举（三种结构体、枚举变体、Option、Result）
│   ├── collections.rs              # 第4章：集合与迭代器（Vec、String、HashMap、迭代器适配器）
│   ├── oop_traits.rs               # 第5章：OOP 与 Trait（trait 定义、默认实现、trait bound、dyn Trait、运算符重载）
│   ├── concurrency.rs              # 第6章：并发编程（thread、mpsc 通道、Arc+Mutex、Send/Sync 无畏并发）
│   ├── advanced.rs                 # 第7章：高级特性（闭包 Fn/FnMut/FnOnce、Box/Rc/RefCell 智能指针、内部可变性、宏、模块系统）
│   └── patterns.rs                 # 第8章：设计模式（单例、工厂、策略、观察者、建造者、RAII）
└── README_RUST.md                  # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（basics）
- `basics.rs` - 变量与可变性、数据类型、函数、控制流、模式匹配、字符串

核心知识点：
- `let` 绑定默认**不可变**（immutable），加 `mut` 才可变——与多数语言相反
- `const` 编译期常量与 `static` 静态变量的区别
- 标量类型（i32/f64/bool/char）与复合类型（元组、数组、切片）
- 语句（无返回值）与表达式（有返回值）的区别；函数体最后一个表达式即返回值（无分号）
- `if` 是表达式可以返回值；`loop`/`while`/`for` 三种循环
- `match` 穷尽式模式匹配，`if let`/`while let` 简化单分支匹配
- `String`（堆上、可变、拥有所有权）vs `&str`（字符串切片、借用）的区别是新手第一关

### 第二阶段：所有权与借用（ownership）—— Rust 的灵魂
- `ownership.rs` - 所有权三法则、移动、借用、借用规则、生命周期

核心知识点：
- **所有权三法则**：每个值有且仅有一个所有者；所有者离开作用域时值被 drop
- 赋值/传参默认**移动**（move）所有权，栈上类型（实现 Copy）则拷贝
- 借用：`&T` 不可变引用、`&mut T` 可变引用，不夺取所有权
- **借用规则**：同一时刻只能有一个可变引用，或任意多个不可变引用；引用必须始终有效
- 借用检查器（Borrow Checker）在**编译期**杜绝悬垂引用与数据竞争
- 生命周期（Lifetime）：引用的有效范围，多数情况编译器自动推导（生命周期省略规则），复杂场景手动标注 `'a`

### 第三阶段：结构体与枚举（structs_enums）
- `structs_enums.rs` - 三种结构体、方法、枚举变体、Option、Result

核心知识点：
- 命名字段结构体、元组结构体、类单元结构体三种形态
- `impl` 块为类型定义方法与关联函数（`new` 惯例）
- Rust 枚举是代数数据类型（ADT），每个变体可携带不同类型数据
- `Option<T>` 取代 null：编译器强制处理 None 情况，根除空指针异常
- `Result<T, E>` 表达可失败操作；`?` 运算符自动传播错误
- 模式匹配解构枚举：`match`、`if let`、`unwrap`/`expect`/`unwrap_or` 家族

### 第四阶段：集合与迭代器（collections）
- `collections.rs` - Vec、String、HashMap、VecDeque、迭代器

核心知识点：
- `Vec<T>` 动态数组、`HashMap<K,V>` 哈希表、`VecDeque<T>` 双端队列、`HashSet`/`BTreeMap` 等
- 集合存储在堆上，所有权规则同样适用；遍历时注意 `iter()`（借用）与 `into_iter()`（夺取所有权）的区别
- 迭代器是**惰性**的：不调用消费器（collect/sum/for_each）就不会执行
- 常用适配器：map/filter/fold/enumerate/zip/take；链式调用是 Rust 惯用风格
- 迭代器是零成本抽象：编译后与手写循环性能相当

### 第五阶段：OOP 与 Trait（oop_traits）
- `oop_traits.rs` - trait 定义、默认实现、trait bound、trait 对象、运算符重载、Drop/Deref

核心知识点：
- Trait 定义行为契约，类似接口但更强大：支持默认实现、关联类型
- 一个类型可以实现多个 trait；可为本地类型实现外部 trait，但受**孤儿规则**限制
- 静态分发：`impl Trait` 参数与泛型 + trait bound，编译期单态化，零成本
- 动态分发：`dyn Trait` trait 对象，运行时虚表查找，支持异构集合
- 运算符重载通过实现 std::ops 中的 trait（如 Add、Index）完成
- `Drop` trait 自定义析构逻辑；`Deref` trait 实现智能指针解引用

### 第六阶段：并发编程（concurrency）
- `concurrency.rs` - 线程、mpsc 通道、Arc+Mutex、Send/Sync

核心知识点：
- **无畏并发**（Fearless Concurrency）：编译器在编译期保证无数据竞争
- `thread::spawn` 创建 OS 线程；闭包常用 `move` 关键字夺取所有权
- 消息传递：`std::sync::mpsc` 多生产者单消费者通道，"通过通信共享内存"
- 共享状态：`Arc<T>`（原子引用计数，线程安全版 Rc）+ `Mutex<T>`（互斥锁）
- `Send` trait（可跨线程转移所有权）与 `Sync` trait（可跨线程共享引用）是编译期并发安全的基石
- 锁保护的是数据而非代码：`Mutex` 直接包裹被保护数据，不可能忘记加锁

### 第七阶段：高级特性（advanced）
- `advanced.rs` - 闭包、智能指针、内部可变性、宏、模块系统、泛型

核心知识点：
- 闭包三种 trait：`Fn`（只读捕获）、`FnMut`（可变捕获）、`FnOnce`（消耗捕获）
- `Box<T>` 堆分配；`Rc<T>` 单线程引用计数；`RefCell<T>` 运行时借用检查
- **内部可变性**（Interior Mutability）：`RefCell`/`Cell` 允许在不可变引用下修改数据，借用规则从编译期移到运行时
- `Rc<RefCell<T>>` 组合实现"多所有者 + 可变"（注意循环引用导致内存泄漏）
- `macro_rules!` 声明式宏基础；`#[derive(...)]` 派生宏自动生成 trait 实现
- 模块系统：`mod`/`use`/`crate`/`self`/`super`/`pub` 可见性控制

### 第八阶段：设计模式（patterns）
- `patterns.rs` - 单例、工厂、策略、观察者、建造者、RAII

核心知识点：
- Rust 的所有权与 trait 系统让设计模式的实现与传统 OOP 语言（Java/C++）明显不同
- 单例模式：`std::sync::Once` 保证一次性初始化
- 策略模式：trait 对象或泛型 + trait bound 两种实现路径
- 建造者模式（Builder）：链式调用构造复杂对象，Rust 社区极常用
- **RAII**：资源获取即初始化，利用 Drop trait 自动释放资源，是 Rust 资源管理的根基

## 开发环境

### 1. rustup 工具链管理器
- **用途**：Rust 官方工具链安装与管理工具，管理 rustc、cargo、rustfmt、clippy 及多版本切换
- **下载地址**：https://rustup.rs/（Windows 下载 rustup-init.exe 运行）
- **常用命令**：
  - `rustup update` - 更新工具链
  - `rustup default stable` - 切换 stable/beta/nightly 通道
  - `rustup component add rustfmt clippy` - 安装组件
- **特点**：官方推荐安装方式，自带 MSVC（Windows）或 GNU 链接器配置引导
- **验证安装**：`rustc --version` 与 `cargo --version`

### 2. Cargo 包管理器与构建工具
- **用途**：Rust 官方构建系统与包管理器（类似 npm + make 合体），随 rustup 自动安装
- **常用命令**：
  - `cargo new 项目名` - 创建新项目
  - `cargo build` / `cargo build --release` - 调试/发布编译
  - `cargo run` - 编译并运行（本项目的主要使用方式）
  - `cargo check` - 只检查不生成二进制，速度最快
  - `cargo test` / `cargo doc` / `cargo fmt` / `cargo clippy` - 测试/文档/格式化/静态检查
- **特点**：统一的 `Cargo.toml` 配置、从 crates.io 自动拉取依赖、语义化版本解析
- **说明**：本项目 [dependencies] 为空，全部使用标准库

### 3. rustc 编译器
- **用途**：Rust 编译器本体，日常开发由 cargo 调用，一般无需直接使用
- **特点**：错误信息极其友好（带颜色标注与修复建议），编译期检查严格是"编译通过≈运行正确"的来源
- **学习提示**：认真读编译器报错是学 Rust 的最佳途径之一，`rustc --explain E0382` 查看错误码详解

### 4. rust-analyzer 与 IDE
- **rust-analyzer**（https://rust-analyzer.github.io/）：官方语言服务器（LSP），提供补全、跳转、内联类型提示、重构，已内置为 rustup 组件
- **VS Code**（https://code.visualstudio.com/）：安装 rust-analyzer 扩展即可获得一流体验，轻量免费，社区主流选择
- **RustRover**（https://www.jetbrains.com/rust/）：JetBrains 出品的 Rust 专业 IDE，调试与重构能力更强
- **配置建议**：开启"保存时自动 cargo fmt"，问题面板实时显示 clippy 建议

### 5. Rust Playground（在线试验）
- **网址**：https://play.rust-lang.org/
- 无需安装环境即可在浏览器中编译运行 Rust 代码，支持切换 edition 与查看汇编输出

## Rust 基础知识

### Rust 语言特点速览
| 特性 | 说明 |
|------|------|
| 类型系统 | 静态类型、强类型，编译期检查；类型推导强大（多数情况无需标注） |
| 内存管理 | **无 GC**：所有权系统 + RAII 在编译期静态管理内存 |
| 安全保证 | 内存安全、线程安全在编译期强制保证（unsafe 块除外） |
| 性能 | 与 C/C++ 同级；零成本抽象（Zero-Cost Abstraction） |
| 并发模型 | "无畏并发"：Send/Sync 编译期杜绝数据竞争 |
| 错误处理 | 无异常：`Option<T>` 表示可空，`Result<T,E>` 表示可失败 |
| 版本机制 | Edition（2015/2018/2021/2024）隔离破坏性语法变更，本项目用 2021 |
| 设计哲学 | 安全、并发、实用；"if it compiles, it works" |

### 所有权三大规则
1. Rust 中的每个值都有一个被称为**所有者**（owner）的变量
2. 值在任一时刻**有且只有一个**所有者
3. 当所有者离开作用域，这个值将被**丢弃**（drop）

- 违反任何一条都无法通过编译——这就是 Rust 内存安全的根基

### 借用规则与生命周期
- 同一作用域内：要么**一个** `&mut T`，要么**任意多个** `&T`，二者不可共存
- 引用必须总是有效的：不允许返回指向已 drop 值的引用（悬垂引用）
- 生命周期标注 `'a` 不改变引用的存活时间，只是向编译器**描述**引用间的关系
- 生命周期省略三规则让多数函数无需手动标注；方法中的 `&self` 引用默认与输出同生命周期

### Result 错误处理
| 写法 | 含义 |
|------|------|
| `Ok(value)` | 操作成功，携带结果值 |
| `Err(error)` | 操作失败，携带错误信息 |
| `?` 运算符 | 出错时提前 return 传播错误，成功时解包取值 |
| `.unwrap()` | 成功取值，失败 panic（仅用于原型/测试） |
| `.expect("消息")` | 同 unwrap 但带自定义 panic 信息 |
| `.unwrap_or(默认值)` | 失败时返回默认值 |
| `match result { ... }` | 穷尽处理两种分支（最完整的方式） |

- 库代码应返回 `Result`；`main` 函数也可返回 `Result<(), Box<dyn Error>>` 以使用 `?`

### 常用智能指针速查
| 智能指针 | 所有者模型 | 线程安全 | 可变性 | 典型场景 |
|----------|------------|----------|--------|----------|
| `Box<T>` | 单一所有者 | - | 正常借用规则 | 堆分配、递归类型、trait 对象 |
| `Rc<T>` | 多所有者（引用计数） | 否 | 不可变 | 单线程共享只读数据 |
| `Arc<T>` | 多所有者（原子计数） | 是 | 不可变 | 跨线程共享只读数据 |
| `RefCell<T>` | 单一所有者 | 否 | 内部可变（运行时检查） | 单线程内部可变性 |
| `Mutex<T>` | 互斥访问 | 是 | 锁内可变 | 跨线程共享可变数据 |
| `Rc<RefCell<T>>` | 多所有者+可变 | 否 | 可变 | 图结构、双向链表（单线程） |
| `Arc<Mutex<T>>` | 多所有者+可变 | 是 | 锁内可变 | 多线程共享可变状态 |

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.rs` 文件的逐行中文注释，不要跳过，特别是文件开头阐述设计理念（如所有权三法则、无畏并发）的块注释。
2. **拥抱编译器**：Rust 编译器的报错是教科书级的，认真阅读每一条错误与 help 建议，比看任何教程都快；`cargo check` 快速迭代。
3. **先跑起来再改**：先 `cargo run` 通过菜单运行各模块观察输出，再故意制造错误（如同时持有一个可变引用和一个不可变引用）观察编译器如何拦截，理解规则为什么存在。
4. **动手默写**：看完一个模块后，合上书把核心代码（如 Result 传播、迭代器链、Arc<Mutex<T>> 共享计数器）默写一遍。
5. **循序渐进**：务必按 8 个阶段顺序学习，**所有权（第 2 阶段）是全书的地基**，不理解所有权直接学后面必然处处碰壁。
6. **读官方书与源码**：《The Rust Programming Language》（有官方中文译本）配合本项目食用最佳；标准库源码（`rustup component add rust-src`）注释规范，是进阶读物。

## 注意事项

### 1. 默认不可变与 move 语义
- `let x = 5;` 的 x 不可变，想修改必须 `let mut x = 5;`
- 赋值、传参、返回默认**移动**所有权，之后原变量失效；基础类型（i32、bool 等实现 Copy 的类型）除外
- 只想临时使用值而不夺取所有权：传引用 `&x`，或显式 `x.clone()`

### 2. String 与 &str 的选择
- 函数参数优先用 `&str`（可接受 `String` 引用与字符串字面量，最通用）
- 结构体字段存拥有的字符串用 `String`；存引用需标注生命周期，结构更复杂
- `format!` 拼接、`to_string()`/`String::from()` 创建；`s.len()` 是**字节数**而非字符数（UTF-8 中文字符占 3 字节）
- 字符串不支持按下标索引（UTF-8 变长编码），按字符遍历用 `s.chars()`

### 3. 不要与借用检查器对抗
- 新手常见错误：迭代时修改集合、返回局部变量的引用、结构体自引用
- 正确思路：改用索引循环、返回拥有的值、用 Rc/Arc 共享、用索引代替引用
- 实在需要运行时灵活借用：`RefCell`（单线程）/`Mutex`（多线程），代价是运行时 panic 风险
- **不要用 unsafe 绕过检查**——那是把安全隐患埋进未来

### 4. 锁与死锁
- `Mutex` 锁通过 `lock().unwrap()` 获取，守卫（Guard）离开作用域自动释放（RAII）
- 锁的粒度尽量小，持锁时间尽量短；避免持锁等待 IO
- 多个锁按固定顺序获取可防死锁；Rust 编译器能防数据竞争，但**不能防死锁**
- 能用消息传递（mpsc）就不用共享状态：通信优于共享

### 5. 性能认知
- 迭代器链、闭包、泛型都是零成本抽象，编译后与手写循环等价，放心使用
- 频繁 `clone()` 是性能杀手，优先考虑借用或调整所有权结构
- `cargo build --release` 开启优化（本项目 Cargo.toml 已配置 opt-level=3 + LTO），基准测试必须用 release 构建
- 数组 `[]` 栈上定长、`Vec` 堆上变长；能用数组/切片的场景不必上 Vec

## 学习资源推荐

### 推荐书籍
1. 《Rust 程序设计语言》（The Rust Programming Language，官方书，有免费中文版）- Steve Klabnik & Carol Nichols
2. 《Rust 权威指南》（Programming Rust, 2nd Edition）- Jim Blandy 等
3. 《Rust 实战》（Rust in Action）- Tim McNamara
4. 《Rust 编程：入门、实战与进阶》- 朱春雷（中文原创）
5. 《Rust for Rustaceans》- Jon Gjengset（进阶：从会用到精通）

### 在线教程网址
- Rust 官方网站：https://www.rust-lang.org/zh-CN/
- 官方书中文版（在线免费阅读）：https://kaisery.github.io/trpl-zh-cn/
- Rust 标准库文档：https://doc.rust-lang.org/std/
- Rust 语言圣经（Rust Course，中文社区优秀教程）：https://course.rs/
- Rust By Example（示例驱动学习）：https://doc.rust-lang.org/rust-by-example/（中文版 https://rustwiki.org/zh-CN/rust-by-example/）
- Rustlings（交互式小练习）：https://github.com/rust-lang/rustlings
- crates.io 包索引：https://crates.io/

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
