# C#学习项目

## 项目简介

本项目是一套完整的 C# 学习教程，基于 .NET 6.0（LTS 长期支持版）与 C# 10，涵盖基础语法、面向对象编程、集合与泛型、高级特性、设计模式五大核心模块。项目采用 SDK 风格的控制台应用，使用顶级语句（Top-level statements）实现交互式菜单，可自由选择运行各模块的演示。每个 `.cs` 文件都配有极其详细的中文注释与 XML 文档注释（`/// <summary>`），从 `var` 类型推导到 `async/await`，从属性封装到事件委托都逐行说明，适合零基础入门到进阶提升。

通过本项目的学习，你可以掌握 C# 的核心知识体系：值类型与引用类型、属性与索引器、封装/继承/多态、抽象类与接口、委托与事件、泛型与 LINQ、异常处理与反射、异步编程，以及单例、工厂、观察者、策略、装饰器、建造者六种经典设计模式的 C# 实现。

## 目录结构

```
csharp-learning-project/
├── Program.cs                      # 主程序入口（顶级语句实现交互菜单，switch分发到各模块）
├── CSharpLearning.csproj           # 项目文件（.NET 6.0，启用可空引用类型/隐式using/不安全代码）
├── Basics/
│   └── BasicsDemo.cs               # 基础语法演示（数据类型、常量const/readonly、枚举、运算符、控制流、方法、数组、字符串、可空类型）
├── OOP/
│   ├── OopDemo.cs                  # OOP演示入口（类与对象、封装、继承、多态、抽象类、接口、属性、索引器、委托事件等）
│   ├── Person.cs                   # 人类基类（私有字段封装、属性、构造函数、析构函数/终结器、索引器）
│   ├── Student.cs                  # 学生类（继承Person、base关键字、override重写、sealed密封、多态演示）
│   ├── Shapes.cs                   # 形状抽象类（abstract抽象属性Area、抽象方法Draw、多态基类）
│   ├── Interfaces.cs               # 接口定义与实现（IAnimal接口、接口契约、显式接口实现）
│   ├── EventsDemo.cs               # 委托与事件（delegate委托定义、EventArgs事件参数、事件发布与订阅）
│   ├── StaticAndExtensions.cs      # 静态类与扩展方法（static静态构造函数、静态成员、this扩展方法）
│   └── StructsAndPartials.cs       # 结构体与分部类（struct值类型Point、partial分部类、内部类）
├── Collections/
│   └── CollectionsDemo.cs          # 集合与泛型演示（泛型基础、List/Dictionary/HashSet/Queue/Stack/LinkedList、Lambda、LINQ查询/方法语法）
├── Advanced/
│   └── AdvancedDemo.cs             # 高级特性演示（异常处理、特性Attribute、反射、async/await异步、线程与Task、unsafe指针、模式匹配、record记录类型）
├── Patterns/
│   ├── PatternsDemo.cs             # 设计模式演示入口（依次运行六种模式演示）
│   ├── SingletonPattern.cs         # 单例模式（确保一个类只有一个实例）
│   ├── FactoryPattern.cs           # 工厂模式（封装对象创建过程）
│   ├── ObserverPattern.cs          # 观察者模式（一对多依赖，事件通知）
│   ├── StrategyPattern.cs          # 策略模式（算法封装，运行时切换）
│   ├── DecoratorPattern.cs         # 装饰器模式（动态为对象添加职责）
│   └── BuilderPattern.cs           # 建造者模式（分步构建复杂对象）
└── README_CSHARP.md                # 本文件
```

> 说明：`bin/` 与 `obj/` 为 `dotnet build` 自动生成的编译产物目录，未列入目录树。

## 学习路径建议

### 第一阶段：基础语法（Basics）
- `Basics/BasicsDemo.cs` - C# 语法地基

核心知识点：
- 文件范围命名空间（C# 10）：`namespace CSharpLearning.Basics;` 一行搞定
- 值类型 vs 引用类型：栈与堆的存储差异、赋值时的复制语义
- `const`（编译时常量）vs `readonly`（运行时常量）的区别
- 枚举 `enum`：一组命名的整型常量，默认底层类型 `int`
- `var` 隐式类型推导：编译期确定类型，非动态类型
- 可空类型：`int?`（可空值类型）与 `string?`（可空引用类型）、`??` 空合并运算符、`?.` 空条件运算符
- 字符串：`string` 不可变性、字符串插值 `$"{name}"`、`@` 逐字字符串
- 数组与 `foreach` 遍历、方法参数 `ref`/`out`/`params`

### 第二阶段：面向对象编程（OOP）
- `OOP/OopDemo.cs` → `Person.cs` → `Student.cs` → `Shapes.cs` → `Interfaces.cs` → `EventsDemo.cs` → `StaticAndExtensions.cs` → `StructsAndPartials.cs`

核心知识点：
- 封装：私有字段 + 公共属性（`get`/`set` 访问器），属性中做合法性校验
- 自动属性 `public string Name { get; set; }` 与完整属性写法
- 构造函数链：派生类用 `base(...)` 调用基类构造，先基后派
- 继承与多态：`virtual`/`override` 实现运行时多态，`sealed` 阻止继承/重写
- 抽象类 `abstract`：不能被实例化，可含抽象成员与非抽象成员
- 接口 `interface`：纯契约（命名以 I 开头），C# 类单继承但可实现多接口
- 委托 `delegate`：类型安全的方法引用（"方法的指针"），`Action`/`Func` 泛型委托
- 事件 `event`：基于委托的发布-订阅机制，`EventArgs` 传递事件数据
- 静态类与静态构造函数（只执行一次）、扩展方法（第一个参数加 `this`）
- 结构体 `struct`（值类型，轻量数据）与分部类 `partial`（类定义拆到多文件）

### 第三阶段：集合与泛型（Collections）
- `Collections/CollectionsDemo.cs` - 泛型、集合、LINQ

核心知识点：
- 泛型基础：类型参数 `<T>`，编译期类型安全，避免装箱拆箱
- `List<T>`：动态数组，增删查改首选
- `Dictionary<TKey, TValue>`：哈希表键值对，O(1) 查找
- `HashSet<T>`：去重集合；`Queue<T>` 先进先出；`Stack<T>` 后进先出；`LinkedList<T>` 双向链表
- 数组 vs `List<T>` 的性能对比与选型
- Lambda 表达式：`x => x * 2`，委托的简洁写法
- LINQ 查询语法（`from ... where ... select`）与方法语法（`.Where().Select()`）
- 常用 LINQ 操作符：`Where`、`Select`、`OrderBy`、`GroupBy`、`FirstOrDefault`、`Any`、`Count`

### 第四阶段：高级特性（Advanced）
- `Advanced/AdvancedDemo.cs` - 异常、反射、异步、新模式

核心知识点：
- 异常处理：`try`/`catch`/`finally`，`throw` 抛出，自定义异常，异常的抛出与捕获代价
- 特性 `Attribute`：给代码附加元数据，`[Obsolete]`、自定义特性
- 反射：`System.Reflection`，运行时获取类型信息、动态调用方法
- 异步编程：`async`/`await` 基于 `Task`，不阻塞线程的 IO 密集操作利器
- 线程与任务：`Thread` vs `Task`，线程池，`Task.Run` 并行计算
- 不安全代码 `unsafe` 与指针（需开启 `AllowUnsafeBlocks`，本项目 csproj 已配置）
- 模式匹配：`switch` 表达式、`is` 类型模式、属性模式
- 记录类型 `record`（C# 9+）：不可变数据载体，值相等语义

### 第五阶段：设计模式（Patterns）
- `Patterns/PatternsDemo.cs` 及六个模式文件

核心知识点：
- 单例模式：全局唯一实例（`static` + 私有构造，线程安全实现）
- 工厂模式：把 `new` 的细节收拢到工厂，面向接口创建对象
- 观察者模式：与 C# 事件机制天然契合（`event` 就是观察者模式的语法糖）
- 策略模式：算法族封装为接口实现，运行时切换
- 装饰器模式：通过组合层层包装，动态扩展功能（对比继承的僵化）
- 建造者模式：链式调用（Fluent API）分步构建复杂对象

## 开发环境

### 1. .NET SDK（必选）
- **用途**：包含 C# 编译器、运行时与 `dotnet` 命令行工具，编译运行本项目只需它
- **下载地址**：https://dotnet.microsoft.com/download
- **版本说明**：免费开源、跨平台（Windows/Linux/macOS），本项目目标框架为 .NET 6.0（LTS），也可安装更高版本 SDK（向后兼容）
- **特点**：`dotnet CLI` 一条命令完成还原、编译、运行、发布全流程

### 2. dotnet CLI 常用命令
| 命令 | 作用 |
|------|------|
| `dotnet restore` | 还原 NuGet 依赖（首次或更换环境后执行） |
| `dotnet build` | 编译项目，输出到 `bin/Debug/net6.0/` |
| `dotnet run` | 编译并运行（开发期最常用） |
| `dotnet run -c Release` | 以 Release 配置编译并运行 |
| `dotnet publish -c Release` | 发布可部署产物 |
| `dotnet --version` | 查看 SDK 版本 |

### 3. IDE 推荐
- **Visual Studio 2022 Community**（Windows 首选）：https://visualstudio.microsoft.com/ ，免费，C# 开发体验天花板：智能感知、调试器、诊断工具、重构一应俱全
- **JetBrains Rider**：https://www.jetbrains.com/rider/ ，跨平台、响应快、ReSharper 级代码分析（收费，学生免费）
- **VS Code + C# Dev Kit 插件**：https://code.visualstudio.com/ ，轻量免费，配合 `dotnet` CLI 足够完成本项目全部学习

## C#基础知识

### 值类型 vs 引用类型
| 对比项 | 值类型 | 引用类型 |
|--------|--------|----------|
| 代表 | `int`、`double`、`bool`、`struct`、`enum` | `string`、`class`、数组、委托 |
| 存储 | 栈（或内联在所属对象中） | 堆，变量存引用 |
| 赋值语义 | 复制整个值，互不影响 | 复制引用，指向同一对象 |
| 默认值 | 各字段零值 | `null` |
| 可空性 | 需 `int?`（`Nullable<T>`） | 直接可为 `null`（C# 8 起有可空引用类型检查） |

### 属性（Property）三种写法
| 写法 | 示例 | 说明 |
|------|------|------|
| 自动属性 | `public string Name { get; set; }` | 编译器自动生成幕后字段 |
| 只读自动属性 | `public int Age { get; }` | 只能在构造函数中赋值 |
| 完整属性 | 私有字段 + `get`/`set` 访问器 | 可在访问器中添加校验、通知等逻辑 |

### 委托与事件
- **委托（delegate）**：类型安全的方法引用，签名必须与目标方法一致；内置泛型委托 `Action`（无返回值）、`Func`（有返回值）、`Predicate`（返回 bool）
- **多播委托**：`+=` 订阅多个方法，依次调用
- **事件（event）**：对委托的封装，外部只能 `+=`/`-=`，不能随意触发，是观察者模式的语言级实现
- **Lambda 表达式**：`=>` 语法，委托的匿名简写，如 `btn.Click += (s, e) => Save();`

### 常用泛型集合速查
| 集合 | 特点 | 查找复杂度 | 适用场景 |
|------|------|------------|----------|
| `List<T>` | 动态数组，有序可重复 | O(n)（按下标 O(1)） | 通用列表，默认首选 |
| `Dictionary<K,V>` | 哈希键值对，键唯一 | O(1) | 按键快速检索 |
| `HashSet<T>` | 自动去重 | O(1) | 存在性判断、交并集 |
| `Queue<T>` | 先进先出 | — | 排队任务、BFS |
| `Stack<T>` | 后进先出 | — | 撤销、表达式求值 |
| `LinkedList<T>` | 双向链表 | O(n) | 频繁首尾插删 |

### LINQ 核心概念
- LINQ（Language Integrated Query）：把查询能力集成进语言，可查询集合、数据库（EF Core）、XML 等
- 两种等价写法：查询语法（`from s in students where s.Age > 18 select s`）与方法语法（`students.Where(s => s.Age > 18)`）
- **延迟执行**：LINQ 查询在枚举（`foreach`/`ToList()`）时才真正执行，注意多次枚举的重复计算
- 常用操作符：`Where`（过滤）、`Select`（投影）、`OrderBy/ThenBy`（排序）、`GroupBy`（分组）、`Join`（连接）、`Any/All`（存在性）、`FirstOrDefault/Single`（取元素）、`Count/Sum/Average`（聚合）

## 学习方法建议

1. **先跑通再深入**：在项目目录执行 `dotnet run`，通过交互菜单把五个模块的演示完整看一遍，建立全景认识。
2. **逐行阅读注释**：本项目每个 `.cs` 文件都有详细中文注释和 XML 文档注释，`Person.cs` → `Student.cs` 的继承链是理解 OOP 的最佳路线。
3. **善用 XML 文档**：IDE 中把鼠标悬停在类/方法上即可看到 `/// <summary>` 注释，养成阅读与编写文档注释的习惯。
4. **动手改写演示**：给 `Person` 加一个属性、给 LINQ 演示换一个查询条件、给事件再加一个订阅者，在修改中巩固理解。
5. **对比学习**：与 C/C++ 学习项目对比，体会属性 vs 手写 getter/setter、GC vs 手动内存管理、LINQ vs 手写循环的效率差异。
6. **循序渐进**：严格按 5 个阶段推进，委托事件不懂先看懂 `EventsDemo.cs` 的发布-订阅流程，再进入 LINQ 与异步。

## 注意事项

### 1. null 安全
- 空引用异常（`NullReferenceException`）是 C# 第一大运行时错误
- 本项目已启用可空引用类型（`Nullable: enable`），`string?` 表示可能为 null，编译器会给出警告，务必逐条消除
- 防御性写法：方法参数校验、`?.` 空条件调用、`??` 提供默认值、`ArgumentNullException.ThrowIfNull(x)`

### 2. 值类型的装箱与拆箱
- 值类型赋值给 `object` 或非泛型集合（`ArrayList`）时发生装箱（stack → heap），反向为拆箱
- 频繁装箱拆箱有显著性能开销，**优先使用泛型集合**（`List<int>` 而非 `ArrayList`）

### 3. 字符串不可变性
- `string` 一经创建不可修改，拼接会生成新字符串
- 循环中大量拼接请用 `StringBuilder`，避免产生大量临时对象加重 GC 负担
- 字符串比较用 `==` 或 `Equals`（C# 中 `==` 对 string 已重载为值比较），区分大小写需求用 `string.Equals(a, b, StringComparison.OrdinalIgnoreCase)`

### 4. 资源释放与 using
- GC 自动管理内存，但文件句柄、网络连接等非托管资源需要主动释放
- 实现 `IDisposable` 的对象（`FileStream`、`SqlConnection` 等）用 `using` 语句或 `using` 声明确保及时释放
- 析构函数（终结器）不能保证执行时机，不要依赖它释放关键资源

### 5. async/await 使用纪律
- 异步方法约定以 `Async` 结尾，返回 `Task`/`Task<T>`，避免 `async void`（除事件处理外）
- 异步要"一路到底"：调用链上不要混用 `.Result`/`.Wait()` 同步阻塞，容易死锁
- `async/await` 不创建线程，适合 IO 密集；CPU 密集并行用 `Task.Run` 或 `Parallel`

## 学习资源推荐

### 推荐书籍
1. 《C# 10 和 .NET 6 入门与跨平台开发》（第 6 版）- Mark J. Price（配合本项目 .NET 6 版本）
2. 《C# 高级编程》- Christian Nagel（大部头参考书，查缺补漏）
3. 《深入理解 C#》（第 4 版）- Jon Skeet（语言细节权威，进阶必读）
4. 《CLR via C#》（第 4 版）- Jeffrey Richter（深入 .NET 运行时原理）
5. 《设计模式：可复用面向对象软件的基础》（GoF）- 设计模式经典原著

### 在线教程网址
- 微软官方 C# 文档（最权威，有中文）：https://learn.microsoft.com/zh-cn/dotnet/csharp/
- .NET 官方教程与交互式练习：https://dotnet.microsoft.com/learn
- C# 中文教程（菜鸟教程）：https://www.runoob.com/csharp/csharp-tutorial.html
- dotnet CLI 命令参考：https://learn.microsoft.com/zh-cn/dotnet/core/tools/
- C# 语言规范（英文，进阶）：https://learn.microsoft.com/dotnet/csharp/language-reference/language-specification/
- B 站 C#/.NET 视频教程：https://www.bilibili.com/

---

**祝学习愉快！从第一个 `Console.WriteLine` 到优雅的设计模式，C# 会带给你愉悦的开发体验。如有问题请参考各文件中的详细注释。**
