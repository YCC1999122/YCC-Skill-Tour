# Java学习项目

## 项目简介

本项目是一套完整的 Java 学习教程，基于 Java 11（LTS 长期支持版）与 Maven 构建，采用标准 Maven 目录结构与包（package）分层组织代码，涵盖基础语法、面向对象编程、集合框架、并发编程、高级特性、设计模式六大核心模块。项目提供交互式菜单主程序（`Main.java`），可自由选择运行各模块的演示。每个 `.java` 文件都配有极其详细的中文注释与 Javadoc 文档注释（`/** ... */`），从 `package` 声明到垃圾回收，从继承多态到线程池都逐行说明，适合零基础入门到进阶提升。

通过本项目的学习，你可以掌握 Java 的核心知识体系：JVM/JRE/JDK 的关系、基本类型与面向对象、封装/继承/多态/抽象四大特性、接口与内部类、Java 集合框架（JCF）、异常处理、多线程与并发工具包（JUC）、Lambda 与函数式接口、反射与注解，以及单例、工厂、观察者、策略、代理、建造者六种经典设计模式的 Java 实现。

## 目录结构

```
java-learning-project/
├── pom.xml                                     # Maven项目配置文件（Java 11、UTF-8编码、指定主类com.learning.Main）
└── src/
    └── main/
        └── java/
            └── com/
                └── learning/                   # 项目根包
                    ├── Main.java               # 主程序入口（Scanner交互菜单，分发到各模块演示）
                    ├── basics/
                    │   └── BasicsDemo.java     # 基础语法演示（基本类型、运算符、控制流、数组、字符串、方法）
                    ├── oop/
                    │   ├── OopDemo.java        # OOP演示入口（封装、继承、多态、接口、static/final、内部类、枚举）
                    │   ├── Shape.java          # 形状抽象类（abstract抽象方法，多态基类）
                    │   ├── Circle.java         # 圆形类（继承Shape，实现面积/周长计算）
                    │   ├── Rectangle.java      # 矩形类（继承Shape，实现面积/周长计算）
                    │   └── Animal.java         # 动物相关类（继承体系与多态演示）
                    ├── collections/
                    │   └── CollectionsDemo.java # 集合框架演示（List/Set/Queue/Map体系、Stream流式操作）
                    ├── concurrency/
                    │   └── ConcurrencyDemo.java # 并发编程演示（Thread、Runnable、synchronized、Lock、原子类、线程池）
                    ├── advanced/
                    │   └── AdvancedDemo.java   # 高级特性演示（异常处理、注解、反射、Lambda与函数式接口、Optional、日期时间API、序列化）
                    └── patterns/
                        ├── PatternsDemo.java   # 设计模式演示入口（依次调用各模式的demonstrate方法）
                        ├── SingletonPattern.java # 单例模式（饿汉/懒汉/双重检查/枚举/静态内部类五种实现）
                        ├── FactoryPattern.java   # 工厂模式（简单工厂/工厂方法/抽象工厂）
                        ├── ObserverPattern.java  # 观察者模式（发布-订阅机制）
                        ├── StrategyPattern.java  # 策略模式（算法封装与运行时切换）
                        ├── ProxyPattern.java     # 代理模式（静态代理/JDK动态代理）
                        └── BuilderPattern.java   # 建造者模式（复杂对象分步构建）
```

> 说明：`target/` 为 Maven 编译输出目录（`mvn compile/package` 自动生成），未列入目录树。

## 学习路径建议

### 第一阶段：基础语法（basics）
- `basics/BasicsDemo.java` - Java 语法地基

核心知识点：
- `package` 声明必须是源文件第一行有效代码；一个文件最多一个 `public` 类且类名与文件名一致
- 8 种基本类型（`byte`/`short`/`int`/`long`/`float`/`double`/`char`/`boolean`）与对应包装类
- 自动装箱/拆箱（`int` ↔ `Integer`）及其性能与 `==` 比较陷阱
- 运算符、类型转换（自动 widening 与强制 narrowing）
- 控制流：`if/else`、`switch`、`for`、增强 `for`（for-each）、`while`/`do-while`
- 数组：声明、初始化、`Arrays` 工具类
- `String` 不可变性、字符串常量池、`StringBuilder` 高效拼接
- 方法：参数按值传递（Java 只有值传递）、方法重载（Overload）

### 第二阶段：面向对象编程（oop）
- `oop/OopDemo.java` → `Shape.java` / `Circle.java` / `Rectangle.java` / `Animal.java`

核心知识点：
- OOP 四大特性：封装、继承、多态、抽象
- 封装：`private` 字段 + `public` getter/setter，数据隐藏
- 构造方法、`this` 关键字（区分字段与参数、调用本类其他构造）
- `static`：类成员共享、静态块；`final`：常量、不可重写方法、不可继承类
- 继承：`extends` 单继承，`super` 调用父类构造与方法，构造链先父后子
- 多态：父类引用指向子类对象，运行时动态绑定（重写 Override）
- `@Override` 注解、重载（编译期）vs 重写（运行期）的区别
- 抽象类 `abstract`：不可实例化，可含抽象方法与具体实现（`Shape` 体系）
- 接口 `interface`：纯契约，Java 8 起支持 `default`/`static` 方法
- 内部类：成员内部类、静态嵌套类、局部内部类、匿名内部类
- 枚举 `enum`：类型安全的常量集合，可实现接口、定义字段和方法

### 第三阶段：集合框架（collections）
- `collections/CollectionsDemo.java` - Java 集合框架（JCF）

核心知识点：
- 集合 vs 数组：长度可变、只能存引用类型（基本类型靠装箱）
- 两大接口体系：`Collection`（单列：`List`/`Set`/`Queue`）与 `Map`（键值对）
- `List`：`ArrayList`（数组，随机访问快）vs `LinkedList`（链表，插删快）
- `Set`：`HashSet`（哈希去重）、`LinkedHashSet`（保插入序）、`TreeSet`（红黑树排序）
- `Map`：`HashMap`（哈希表）、`LinkedHashMap`（保序）、`TreeMap`（键排序）、`Hashtable`（线程安全旧类）
- `hashCode()` 与 `equals()` 契约：相等的对象必须有相等的哈希码
- 迭代器 `Iterator` 与 fail-fast 机制（遍历中结构性修改抛 `ConcurrentModificationException`）
- Java 8 Stream 流式操作：`filter`/`map`/`collect(Collectors.toList())`

### 第四阶段：并发编程（concurrency）
- `concurrency/ConcurrencyDemo.java` - 多线程与 JUC 工具包

核心知识点：
- 进程 vs 线程：资源分配单位 vs CPU 调度单位，线程共享堆、独立栈
- 创建线程：`extends Thread` vs `implements Runnable`（推荐）
- 并发三大问题：原子性、可见性、有序性
- `synchronized` 关键字：对象锁/类锁，保证原子性与可见性
- `volatile`：保证可见性、禁止指令重排，但不保证原子性
- JUC 包：`ReentrantLock`（可重入锁，比 synchronized 更灵活）
- 原子类 `AtomicInteger`：CAS 无锁实现原子自增
- 线程池 `ExecutorService`：复用线程、控制并发数，避免频繁创建线程
- 线程安全的单例实现（双重检查锁定，为设计模式阶段铺垫）

### 第五阶段：高级特性（advanced）
- `advanced/AdvancedDemo.java` - 异常、注解、反射、Lambda 等

核心知识点：
- 异常体系：`Throwable` → `Error`（不可恢复）/`Exception`（可查/运行时）
- `try/catch/finally`、`throw`/`throws`、try-with-resources 自动关闭资源
- 注解 `Annotation`：`@Override`、`@Deprecated`、自定义注解与元注解
- 反射 `Class`/`Field`/`Method`/`Constructor`：运行时探查与动态调用，框架（Spring 等）的基石
- Lambda 表达式与函数式接口（`@FunctionalInterface`）：`Consumer`/`Function`/`Predicate`/`Supplier`
- `Optional`：显式表达"可能为空"，减少 NPE
- 日期时间 API（Java 8+）：`LocalDate`/`LocalDateTime`/`DateTimeFormatter`/`ChronoUnit`
- 序列化 `Serializable`：对象与字节流的互转

### 第六阶段：设计模式（patterns）
- `patterns/PatternsDemo.java` 及六个模式文件

核心知识点：
- 单例模式五种写法：饿汉式、懒汉式、双重检查锁定（DCL + volatile）、静态内部类、枚举（最推荐，防反射防序列化）
- 工厂模式三层次：简单工厂（一个工厂判类型）、工厂方法（每产品一工厂）、抽象工厂（产品族）
- 观察者模式：主题维护观察者列表，状态变化批量通知
- 策略模式：算法接口 + 多实现，运行时注入切换
- 代理模式：静态代理 vs JDK 动态代理（`Proxy` + `InvocationHandler`，AOP 原理基础）
- 建造者模式：链式调用构建复杂对象（`StringBuilder` 就是身边的例子）

## 开发环境

### 1. JDK（Java Development Kit，必选）
- **用途**：包含 Java 编译器（javac）、运行时（JRE/JVM）与标准类库，是 Java 开发的基础
- **下载地址**：Oracle JDK：https://www.oracle.com/java/technologies/downloads/ ；开源 OpenJDK（推荐）：https://adoptium.net/
- **版本说明**：本项目使用 Java 11（LTS），也可安装 Java 17/21（LTS，向下兼容）
- **特点**：一次编写到处运行（Write Once, Run Anywhere），跨平台靠 JVM 实现

### 2. Maven（构建工具）
- **用途**：依赖管理与项目构建，`pom.xml` 是核心配置文件
- **下载地址**：https://maven.apache.org/download.cgi
- **使用方式**：`mvn compile` 编译、`mvn package` 打 jar 包、`mvn clean` 清理
- **特点**：约定优于配置（标准目录结构）、自动从中央仓库下载依赖、插件体系丰富

### 3. 常用命令
| 命令 | 作用 |
|------|------|
| `mvn compile` | 编译源代码到 `target/classes/` |
| `mvn package` | 编译并打包为 `target/java-learning.jar` |
| `java -jar target/java-learning.jar` | 运行打包后的程序（pom 已配置主类） |
| `mvn clean` | 删除 `target/` 目录 |
| `java -version` / `mvn -version` | 检查 JDK / Maven 安装是否成功 |

### 4. IDE 推荐
- **IntelliJ IDEA Community**（首选）：https://www.jetbrains.com/idea/ ，免费社区版足够学习，Java 开发体验业界第一，原生支持 Maven
- **Eclipse**：https://www.eclipse.org/downloads/ ，老牌免费 IDE，企业存量项目常见
- **VS Code + Extension Pack for Java**：https://code.visualstudio.com/ ，轻量免费，微软官方 Java 插件包

## Java基础知识

### JDK / JRE / JVM 的关系
| 名词 | 全称 | 包含内容 | 面向人群 |
|------|------|----------|----------|
| JVM | Java Virtual Machine | 执行字节码的虚拟机，跨平台的核心 | （被使用） |
| JRE | Java Runtime Environment | JVM + 运行所需核心类库 | 只运行 Java 程序的用户 |
| JDK | Java Development Kit | JRE + 编译器 javac + 调试/打包工具 | Java 开发者（必须安装） |

- Java 源文件 `.java` 经 `javac` 编译为字节码 `.class`，由 JVM 解释/JIT 编译执行
- "一次编写，到处运行"的本质：各平台有各自的 JVM，字节码是统一中间层

### 8 种基本类型
| 类型 | 字节数 | 默认值 | 包装类 | 说明 |
|------|--------|--------|--------|------|
| `byte` | 1 | 0 | `Byte` | -128 ~ 127 |
| `short` | 2 | 0 | `Short` | ±3.2 万 |
| `int` | 4 | 0 | `Integer` | ±21 亿，整型默认 |
| `long` | 8 | 0L | `Long` | 字面量加 `L` 后缀 |
| `float` | 4 | 0.0f | `Float` | 字面量加 `f` 后缀 |
| `double` | 8 | 0.0d | `Double` | 浮点默认 |
| `char` | 2 | '\u0000' | `Character` | UTF-16 编码 |
| `boolean` | 1（虚拟机视实现） | false | `Boolean` | 只有 true/false |

### 常用集合类速查
| 集合 | 底层结构 | 有序性 | 允许重复 | 查找复杂度 | 适用场景 |
|------|----------|--------|----------|------------|----------|
| `ArrayList` | 动态数组 | 插入序 | 是 | 按下标 O(1) | 通用列表，默认首选 |
| `LinkedList` | 双向链表 | 插入序 | 是 | O(n) | 频繁首尾插删、可当队列 |
| `HashSet` | 哈希表 | 无 | 否 | O(1) | 去重、存在性判断 |
| `LinkedHashSet` | 哈希表+链表 | 插入序 | 否 | O(1) | 去重且保序 |
| `TreeSet` | 红黑树 | 排序 | 否 | O(log n) | 需要排序的集合 |
| `HashMap` | 哈希表（数组+链表/红黑树） | 无 | 键唯一 | O(1) | 键值查找，默认首选 |
| `LinkedHashMap` | 哈希表+链表 | 插入序 | 键唯一 | O(1) | 保序键值对、LRU 缓存 |
| `TreeMap` | 红黑树 | 键排序 | 键唯一 | O(log n) | 需要排序的键值对 |

### 垃圾回收（GC）基础
- JVM 自动回收不再被引用的堆对象，无需（也不能）手动释放内存
- 对象进入老年代前先在新生代 Eden 区，经历 Minor GC 存活后晋升
- 可达性分析：从 GC Roots（栈引用、静态引用等）出发不可达的对象才被回收
- `null` 赋值只是断开引用，GC 时机由 JVM 决定；`System.gc()` 只是建议，不保证执行
- 内存泄漏在 Java 中仍会发生：静态集合无限增长、监听器未注销、缓存无淘汰策略

### 继承与多态要点
- Java 类单继承（`extends` 一个父类），可多实现接口（`implements A, B`）
- 构造顺序：父类静态块 → 子类静态块 → 父类构造 → 子类构造
- 方法重写三同：同名、同参、返回值兼容（协变返回）；访问权限只能更宽
- 向上转型自动，向下转型需强转且先用 `instanceof` 判断，否则 `ClassCastException`

## 学习方法建议

1. **先跑起来**：执行 `mvn compile` 编译，再用 IDE 运行 `Main.main()`，通过菜单把六大模块的演示完整看一遍。
2. **逐行阅读注释**：本项目每个 `.java` 文件都有详尽的中文注释与 Javadoc，尤其是包声明、访问修饰符、集合框架的体系说明，注释本身就是教材。
3. **用 IDE 的调试器**：在 `OopDemo` 的多态调用、`ConcurrencyDemo` 的线程竞争处打断点单步观察，比看十遍概念都有效。
4. **动手扩展演示**：给 `Shape` 体系加一个 `Triangle`、给单例模式写一个破坏测试、给线程池换一种拒绝策略，在改动中加深理解。
5. **对比学习**：与 C/C++/C# 学习项目对照，体会 GC 与手动内存管理、接口与抽象类、Stream 与 LINQ 的异同。
6. **循序渐进**：并发编程是难点，务必在集合与面向对象扎实后再进入；设计模式放在最后，先理解语法再理解设计。

## 注意事项

### 1. 空指针异常（NullPointerException）
- NPE 是 Java 第一大运行时异常，链式调用 `a.getB().getC()` 任何一环为 null 都会炸
- 防御手段：入参校验、`Objects.requireNonNull`、返回 `Optional`、Java 8+ 的 `Objects` 工具类
- 自动拆箱是隐蔽的 NPE 来源：`Integer i = null; int j = i;` 直接抛异常

### 2. `==` 与 `equals` 的区别
- `==` 比较基本类型的值、引用类型的地址
- `equals` 默认等价于 `==`，`String`/`Integer` 等重写了它用于值比较
- 自定义类作为 `HashMap` 键或放入 `HashSet` 前，必须同时重写 `equals` 和 `hashCode`
- `Integer` 缓存 -128~127，该范围外 `new Integer`/`valueOf` 结果 `==` 为 false，比较请用 `equals`

### 3. 字符串与浮点数的坑
- `String` 不可变，循环拼接用 `StringBuilder`（非线程安全）/`StringBuffer`（线程安全）
- 不要用 `float`/`double` 做精确金额计算，用 `BigDecimal`（构造时传字符串而非 double）
- `switch` 从 Java 7 起支持 `String`，Java 14 起有箭头 `switch` 表达式

### 4. 集合使用的纪律
- 不要在 `foreach` 遍历中调用 `add`/`remove`（fail-fast 抛 `ConcurrentModificationException`），删除元素用 `Iterator.remove()` 或 `removeIf`
- `ArrayList` 初始容量合理预估可减少扩容拷贝；`HashMap` 初始容量 = 预期元素数 / 0.75 + 1
- 多线程环境不要直接用 `HashMap`（可能死循环/丢数据），用 `ConcurrentHashMap`

### 5. 线程安全的意识
- 共享可变数据才需要同步；优先用不可变对象（`final` 字段、无 setter）从源头规避
- `synchronized` 锁对象不能是 `null`，锁粒度越小越好但不要小于原子操作范围
- 优先使用 JUC 的高层工具（线程池、原子类、`ConcurrentHashMap`），少手写 `wait`/`notify`

## 学习资源推荐

### 推荐书籍
1. 《Java 核心技术·卷 I》（第 12 版）- Cay S. Horstmann（系统全面的入门经典）
2. 《Effective Java》（第 3 版）- Joshua Bloch（90 条黄金准则，进阶必读）
3. 《Java 编程思想》（第 4 版）- Bruce Eckel（深入语言设计哲学）
4. 《深入理解 Java 虚拟机》（第 3 版）- 周志明（JVM 原理与调优权威）
5. 《Java 并发编程实战》- Brian Goetz（并发编程圣经）
6. 《Head First 设计模式》- Eric Freeman（设计模式入门首选，示例即 Java）

### 在线教程网址
- Oracle 官方 Java 教程：https://docs.oracle.com/javase/tutorial/
- Java 11 API 官方文档：https://docs.oracle.com/en/java/javase/11/docs/api/
- Java 中文教程（菜鸟教程）：https://www.runoob.com/java/java-tutorial.html
- 廖雪峰 Java 教程（免费优质）：https://liaoxuefeng.com/books/java/introduction/index.html
- Maven 官方指南：https://maven.apache.org/guides/
- B 站 Java 视频教程：https://www.bilibili.com/

---

**祝学习愉快！从 `Hello World` 到设计模式，愿 JVM 成为你可靠的伙伴。如有问题请参考各文件中的详细注释。**
