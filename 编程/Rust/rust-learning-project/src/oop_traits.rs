// ============================================================
// oop_traits.rs - OOP与Trait模块
// ============================================================
// Rust不是传统的面向对象语言，但它具有很多面向对象的特性
// Trait是Rust实现代码复用和多态的核心机制
//
// Trait（特征）：定义一组方法签名，描述类型必须实现的行为
//   - 类似于其他语言的接口（interface），但更强大
//   - 可以有默认实现
//   - 可以为外部类型实现（孤儿规则限制）
//
// 主要内容：
//   - Trait定义与实现
//   - 默认实现
//   - Trait作为参数（impl Trait / Trait bound）
//   - 返回实现Trait的类型
//   - Trait对象（dyn Trait）
//   - 泛型与trait bound
//   - 运算符重载（通过trait）
//   - Drop trait
//   - Deref trait
// ============================================================

/// OOP与Trait模块的入口演示函数
pub fn run_oop_traits_demo() {
    println!("\n{}", "=".repeat(60));
    println!("OOP与Trait演示");
    println!("{}", "=".repeat(60));

    trait_definition_demo();
    default_impl_demo();
    trait_as_parameter_demo();
    return_trait_demo();
    trait_object_demo();
    generic_trait_bound_demo();
    operator_overloading_demo();
    drop_trait_demo();
    deref_trait_demo();
}

// ============================================================
// 1. Trait定义与实现
// ============================================================
// Trait定义了一组方法签名（行为），其他类型可以实现这些方法
// 使用 trait 关键字定义 trait
// 使用 impl Trait for Type 实现 trait

/// Trait定义与实现演示函数
fn trait_definition_demo() {
    println!("\n--- 1. Trait定义与实现 ---");

    // ---- 定义一个 Trait ----
    // Summary trait 定义了一个 summarize 方法
    // 任何实现了 Summary trait 的类型都必须提供 summarize 方法
    pub trait Summary {
        // 方法签名（没有实现，只有声明）
        // 实现这个 trait 的类型必须提供这个方法的具体实现
        fn summarize(&self) -> String;
    }

    // ---- 为结构体实现 Trait ----
    // 文章类型
    pub struct NewsArticle {
        pub headline: String,    // 标题
        pub location: String,    // 地点
        pub author: String,      // 作者
        pub content: String,     // 内容
    }

    // 为 NewsArticle 实现 Summary trait
    impl Summary for NewsArticle {
        fn summarize(&self) -> String {
            format!("{} - {} by {}", self.headline, self.location, self.author)
        }
    }

    // 推文类型
    pub struct Tweet {
        pub username: String,    // 用户名
        pub content: String,     // 内容
        pub reply: bool,         // 是否是回复
        pub retweet: bool,       // 是否是转发
    }

    // 为 Tweet 实现 Summary trait
    impl Summary for Tweet {
        fn summarize(&self) -> String {
            format!("{}: {}", self.username, self.content)
        }
    }

    // ---- 使用实现了Trait的类型 ----
    let article = NewsArticle {
        headline: String::from("Rust 1.75 发布！"),
        location: String::from("全球"),
        author: String::from("Rust团队"),
        content: String::from("Rust新版本带来了许多新特性..."),
    };
    println!("文章摘要: {}", article.summarize());

    let tweet = Tweet {
        username: String::from("rust_lover"),
        content: String::from("Rust 太酷了！"),
        reply: false,
        retweet: false,
    };
    println!("推文摘要: {}", tweet.summarize());

    // ---- 孤儿规则（Orphan Rule） ----
    // 规则：只能在以下情况为类型实现 trait：
    //   1. trait 是在当前 crate 中定义的，或者
    //   2. 类型是在当前 crate 中定义的
    // 也就是说：不能为外部类型实现外部 trait
    // 这样保证了别人的代码不会破坏你的代码，反之亦然
    // （newtype 模式可以绕过这个限制，详见 advanced.rs）

    println!("Trait的特点：");
    println!("  - 定义一组方法签名，描述行为");
    println!("  - 不同类型可以实现同一个trait");
    println!("  - 受孤儿规则保护，确保一致性");
}

// ============================================================
// 2. 默认实现
// ============================================================
// Trait中的方法可以有默认实现
// 实现trait时可以选择保留默认实现，或者覆盖它
// 默认实现可以调用trait中的其他方法（即使那些方法没有默认实现）

/// 默认实现演示函数
fn default_impl_demo() {
    println!("\n--- 2. 默认实现 ---");

    // 定义带默认实现的 trait
    pub trait Summary {
        // 必须实现的方法（没有默认实现）
        fn summarize_author(&self) -> String;

        // 有默认实现的方法
        // 默认实现可以调用 trait 中的其他方法
        fn summarize(&self) -> String {
            format!("(阅读更多来自 {} 的内容...)", self.summarize_author())
        }
    }

    // 结构体1：只实现必需的方法，summarize 使用默认实现
    pub struct Tweet {
        pub username: String,
        pub content: String,
    }

    impl Summary for Tweet {
        // 只需要实现 summarize_author
        fn summarize_author(&self) -> String {
            format!("@{}", self.username)
        }
        // summarize 使用默认实现
    }

    // 结构体2：覆盖默认实现
    pub struct NewsArticle {
        pub headline: String,
        pub author: String,
    }

    impl Summary for NewsArticle {
        fn summarize_author(&self) -> String {
            self.author.clone()
        }

        // 覆盖默认实现
        fn summarize(&self) -> String {
            format!("【{}】- {}", self.headline, self.author)
        }
    }

    // 使用
    let tweet = Tweet {
        username: String::from("rust_lover"),
        content: String::from("Rust 真好用！"),
    };
    println!("推文（默认实现）: {}", tweet.summarize());

    let article = NewsArticle {
        headline: String::from("Rust 新版本发布"),
        author: String::from("张小明"),
    };
    println!("文章（覆盖默认实现）: {}", article.summarize());

    println!("默认实现的好处：");
    println!("  - 减少重复代码");
    println!("  - 可以选择性覆盖");
    println!("  - 默认实现可以调用其他trait方法");
}

// ============================================================
// 3. Trait作为参数
// ============================================================
// 有两种方式使用 trait 作为参数类型：
//
// 方式1：impl Trait 语法（简洁写法）
//   fn foo(item: &impl Summary) { ... }
//
// 方式2：Trait bound 语法（完整写法，更灵活）
//   fn foo<T: Summary>(item: &T) { ... }
//
// 当参数简单时，impl Trait 更简洁
// 当有多个参数或复杂约束时，trait bound 更灵活

/// Trait作为参数演示函数
fn trait_as_parameter_demo() {
    println!("\n--- 3. Trait作为参数 ---");

    // ---- 先定义一个 trait 和两个实现 ----
    pub trait Summary {
        fn summarize(&self) -> String;
    }

    pub struct NewsArticle {
        pub headline: String,
        pub author: String,
    }
    impl Summary for NewsArticle {
        fn summarize(&self) -> String {
            format!("{} - {}", self.headline, self.author)
        }
    }

    pub struct Tweet {
        pub username: String,
        pub content: String,
    }
    impl Summary for Tweet {
        fn summarize(&self) -> String {
            format!("@{}: {}", self.username, self.content)
        }
    }

    // ---- impl Trait 语法 ----
    // 参数 item 可以是任何实现了 Summary trait 的类型
    // 这是语法糖，内部还是泛型实现
    fn notify(item: &impl Summary) {
        println!("突发新闻！{}", item.summarize());
    }

    let article = NewsArticle {
        headline: String::from("地震了"),
        author: String::from("记者"),
    };
    let tweet = Tweet {
        username: String::from("witness"),
        content: String::from("我感觉到了震动！"),
    };
    notify(&article);  // 可以传入 NewsArticle
    notify(&tweet);    // 也可以传入 Tweet

    // ---- Trait Bound 语法 ----
    // 完整的泛型约束写法
    // T: Summary 表示类型 T 必须实现 Summary trait
    fn notify_generic<T: Summary>(item: &T) {
        println!("泛型通知：{}", item.summarize());
    }
    notify_generic(&article);

    // ---- 多个 Trait Bound ----
    // 使用 + 连接多个 trait 约束
    // 表示类型必须同时实现这些 trait
    use std::fmt::Display;

    fn notify_multiple<T: Summary + Display>(item: &T) {
        // item 必须同时实现 Summary 和 Display
        println!("多约束通知：{}", item.summarize());
    }
    // NewsArticle 没有实现 Display，所以不能传
    // notify_multiple(&article);  // 编译错误

    // ---- where 子句 ----
    // 当 trait bound 太多时，可以用 where 子句让代码更清晰
    fn some_function<T, U>(t: &T, u: &U) -> String
    where
        T: Summary + Display,
        U: Summary + Clone,
    {
        format!("t: {}, u: {}", t.summarize(), u.summarize())
    }
    // 上面的函数使用 where 子句，比把约束都写在 <> 里更清晰

    println!("Trait作为参数的两种方式：");
    println!("  - impl Trait：简洁，适合简单情况");
    println!("  - Trait bound：灵活，适合复杂约束");
    println!("  - where 子句：多约束时更清晰");
}

// ============================================================
// 4. 返回实现Trait的类型
// ============================================================
// 函数可以返回实现了某个 trait 的类型，而不是具体类型
// 使用 impl Trait 语法指定返回类型
//
// 注意：impl Trait 在返回位置的限制：
//   - 函数所有可能的返回值必须是同一个具体类型
//   - 不能根据条件返回不同的具体类型（即使都实现了同一个trait）
//   - 如果需要返回不同类型，应该使用 trait 对象（dyn Trait）

/// 返回实现Trait的类型演示函数
fn return_trait_demo() {
    println!("\n--- 4. 返回实现Trait的类型 ---");

    use std::fmt::Display;

    // ---- 返回 impl Trait ----
    // 函数返回值类型是"实现了 Display trait 的某个类型"
    // 但实际返回的是具体类型（这里是 String）
    fn returns_display() -> impl Display {
        String::from("这是一个字符串")
    }

    let val = returns_display();
    println!("返回 impl Display: {}", val);

    // ---- 闭包与 impl Trait ----
    // impl Trait 常用于返回闭包（因为闭包类型无法直接写出）
    // 闭包实现了 Fn/FnMut/FnOnce trait
    fn returns_closure() -> impl Fn(i32) -> i32 {
        |x| x + 1
    }

    let add_one = returns_closure();
    println!("闭包结果: {}", add_one(5));  // 6

    // ---- 注意：返回 impl Trait 必须是同一具体类型 ----
    // 下面的代码不能编译，因为可能返回 String 或 i32 两种不同类型
    // fn maybe_string_or_number(flag: bool) -> impl Display {
    //     if flag {
    //         String::from("hello")
    //     } else {
    //         42  // 错误：expected struct `String`, found integer
    //     }
    // }
    // 如果需要返回不同类型，使用 trait 对象（dyn Trait）

    println!("返回 impl Trait 的特点：");
    println!("  - 隐藏具体类型，只暴露接口");
    println!("  - 常用于返回闭包");
    println!("  - 所有返回路径必须是同一具体类型");
    println!("  - 如需返回不同类型，使用 trait 对象");
}

// ============================================================
// 5. Trait对象（dyn Trait）
// ============================================================
// Trait对象（Trait Object）：指向实现了某个 trait 的类型的指针
// 写法：dyn TraitName（dyn 是 dynamic 的缩写）
//
// Trait对象允许运行时多态：
//   - 在运行时确定具体调用哪个方法（动态分发）
//   - 可以在同一个集合中存储不同类型的对象（只要都实现了同一个trait）
//
// 与泛型的区别：
//   - 泛型：编译时确定具体类型（静态分发），零成本抽象
//   - Trait对象：运行时确定具体类型（动态分发），有少量运行时开销
//
// 注意：只有对象安全（object-safe）的 trait 才能作为 trait 对象
// 对象安全规则：
//   - 方法的返回类型不能是 Self
//   - 方法不能有泛型类型参数

/// Trait对象演示函数
fn trait_object_demo() {
    println!("\n--- 5. Trait对象（dyn Trait）---");

    // ---- 定义一个 trait ----
    pub trait Draw {
        fn draw(&self) -> String;
    }

    // ---- 实现 trait 的不同类型 ----
    pub struct Circle {
        pub radius: f64,
    }
    impl Draw for Circle {
        fn draw(&self) -> String {
            format!("画一个半径为{}的圆", self.radius)
        }
    }

    pub struct Rectangle {
        pub width: f64,
        pub height: f64,
    }
    impl Draw for Rectangle {
        fn draw(&self) -> String {
            format!("画一个{}x{}的矩形", self.width, self.height)
        }
    }

    pub struct Triangle {
        pub base: f64,
        pub height: f64,
    }
    impl Draw for Triangle {
        fn draw(&self) -> String {
            format!("画一个底{}高{}的三角形", self.base, self.height)
        }
    }

    // ---- 使用 Trait 对象 ----
    // Vec<Box<dyn Draw>> 存储不同类型，但都实现了 Draw trait
    // Box<dyn Draw> 是一个 trait 对象，指向堆上实现了 Draw 的值
    pub struct Screen {
        pub components: Vec<Box<dyn Draw>>,
    }

    impl Screen {
        pub fn run(&self) {
            for component in &self.components {
                println!("  {}", component.draw());
            }
        }
    }

    // 创建一个屏幕，包含多种图形
    let screen = Screen {
        components: vec![
            Box::new(Circle { radius: 5.0 }),
            Box::new(Rectangle { width: 10.0, height: 20.0 }),
            Box::new(Triangle { base: 8.0, height: 12.0 }),
        ],
    };

    println!("屏幕上的图形：");
    screen.run();
    // 所有不同类型的图形都存在同一个 Vec 中
    // 调用 draw() 时会动态分发到具体类型的实现

    // ---- 静态分发 vs 动态分发 ----
    // 泛型（静态分发）：
    //   - 编译时生成具体类型的代码（单态化 monomorphization）
    //   - 运行时零开销
    //   - 一个 Vec 只能存同一种类型
    //
    // Trait对象（动态分发）：
    //   - 运行时通过虚函数表（vtable）查找方法
    //   - 有少量运行时开销
    //   - 一个 Vec 可以存不同类型（只要实现了同一trait）

    println!("Trait对象的特点：");
    println!("  - 运行时多态（动态分发）");
    println!("  - 可以存储不同类型的对象");
    println!("  - 有少量运行时开销（vtable查找）");
    println!("  - trait必须是对象安全的");
}

// ============================================================
// 6. 泛型与trait bound
// ============================================================
// 泛型（Generics）：可以处理多种类型的代码
// Trait bound：约束泛型类型必须实现的行为
//
// 泛型函数：
//   fn largest<T: PartialOrd>(list: &[T]) -> &T { ... }
//
// 泛型结构体：
//   struct Point<T> { x: T, y: T }
//
// Rust的泛型是零成本抽象：编译时单态化（monomorphization）
// 为每个具体类型生成对应的代码，运行时无开销

/// 泛型与trait bound演示函数
fn generic_trait_bound_demo() {
    println!("\n--- 6. 泛型与trait bound ---");

    // ---- 泛型函数 ----
    // 找出切片中最大的元素
    // T: PartialOrd 表示 T 必须实现 PartialOrd trait（可以比较大小）
    fn largest<T: PartialOrd>(list: &[T]) -> &T {
        let mut largest = &list[0];  // 假设第一个元素最大

        for item in list.iter() {    // 遍历所有元素
            if item > largest {      // 比较（需要 PartialOrd trait）
                largest = item;      // 更新最大值
            }
        }

        largest
    }

    let numbers = vec![34, 50, 25, 100, 65];
    let result = largest(&numbers);
    println!("最大的数字: {}", result);  // 100

    let chars = vec!['y', 'm', 'a', 'q'];
    let result = largest(&chars);
    println!("最大的字符: {}", result);  // y

    // ---- 泛型结构体 ----
    // Point 结构体可以使用任何类型作为坐标
    #[derive(Debug)]
    struct Point<T> {
        x: T,
        y: T,
    }

    let integer_point = Point { x: 5, y: 10 };
    let float_point = Point { x: 1.0, y: 4.0 };
    println!("整数点: {:?}", integer_point);
    println!("浮点点点: {:?}", float_point);

    // 多类型参数的泛型结构体
    #[derive(Debug)]
    struct MultiPoint<T, U> {
        x: T,
        y: U,
    }

    let mixed_point = MultiPoint { x: 5, y: 10.5 };
    println!("混合类型点: {:?}", mixed_point);

    // ---- 泛型方法 ----
    impl<T> Point<T> {
        // 获取 x 的引用
        fn x(&self) -> &T {
            &self.x
        }
    }

    // 只为特定类型实现方法
    // 只有当 T 是 f64 时才有这个方法
    impl Point<f64> {
        fn distance_from_origin(&self) -> f64 {
            (self.x.powi(2) + self.y.powi(2)).sqrt()
        }
    }

    let p = Point { x: 3.0, y: 4.0 };
    println!("距离原点: {}", p.distance_from_origin());  // 5.0

    // let p_int = Point { x: 3, y: 4 };
    // p_int.distance_from_origin();  // 编译错误：i32 没有这个方法

    // ---- 带 trait bound 的泛型方法 ----
    use std::fmt::Display;

    #[derive(Debug)]
    struct Pair<T> {
        x: T,
        y: T,
    }

    impl<T> Pair<T> {
        // 所有 Pair<T> 都有 new 方法
        fn new(x: T, y: T) -> Self {
            Self { x, y }
        }
    }

    // 只有当 T 实现了 Display 和 PartialOrd 时，
    // Pair<T> 才有 cmp_display 方法
    impl<T: Display + PartialOrd> Pair<T> {
        fn cmp_display(&self) {
            if self.x >= self.y {
                println!("较大的成员是 x = {}", self.x);
            } else {
                println!("较大的成员是 y = {}", self.y);
            }
        }
    }

    let pair = Pair::new(3, 8);
    pair.cmp_display();  // 输出：较大的成员是 y = 8

    // ----  blanket implementations（全面实现）----
    // 为所有满足某个 trait bound 的类型实现另一个 trait
    // 这是标准库中常用的模式
    // 例如：标准库中，所有实现了 Display 的类型都会自动实现 ToString trait
    let s = 5.to_string();  // i32 实现了 Display，所以自动有 to_string()
    println!("i32 转字符串: {}", s);

    println!("泛型与trait bound：");
    println!("  - 泛型：编写可以处理多种类型的代码");
    println!("  - Trait bound：约束泛型类型的行为");
    println!("  - 单态化：编译时生成具体类型代码，零运行时开销");
}

// ============================================================
// 7. 运算符重载
// ============================================================
// Rust不允许自定义任意运算符，但可以重载 std::ops 中定义的 trait 对应的运算符
// 运算符重载就是实现对应的 trait
//
// 常用运算符 trait：
//   - Add：+ 运算符
//   - Sub：- 运算符
//   - Mul：* 运算符
//   - Div：/ 运算符
//   - AddAssign：+= 运算符
//   - Index：[] 索引运算符
//   - Deref：* 解引用运算符

/// 运算符重载演示函数
fn operator_overloading_demo() {
    println!("\n--- 7. 运算符重载（通过trait）---");

    use std::ops::Add;

    // ---- 定义一个点结构体 ----
    #[derive(Debug, PartialEq, Copy, Clone)]
    struct Point {
        x: i32,
        y: i32,
    }

    // ---- 实现 Add trait 来重载 + 运算符 ----
    impl Add for Point {
        // 关联类型：指定相加后的输出类型
        type Output = Point;

        // add 方法定义 + 运算符的行为
        fn add(self, other: Point) -> Point {
            Point {
                x: self.x + other.x,
                y: self.y + other.y,
            }
        }
    }

    // 现在可以使用 + 运算符了
    let p1 = Point { x: 1, y: 2 };
    let p2 = Point { x: 3, y: 4 };
    let p3 = p1 + p2;
    println!("{:?} + {:?} = {:?}", p1, p2, p3);
    // 输出：Point { x: 1, y: 2 } + Point { x: 3, y: 4 } = Point { x: 4, y: 6 }

    // ---- 带类型参数的 Add ----
    // Add trait 有一个泛型参数 Rhs（右手操作数），默认为 Self
    // 可以指定不同的类型来实现异构相加

    #[derive(Debug)]
    struct Millimeters(u32);  // 毫米
    #[derive(Debug)]
    struct Meters(u32);       // 米

    // 实现 Millimeters + Meters
    impl Add<Meters> for Millimeters {
        type Output = Millimeters;

        fn add(self, other: Meters) -> Millimeters {
            Millimeters(self.0 + other.0 * 1000)  // 米转毫米后相加
        }
    }

    let mm = Millimeters(500);
    let m = Meters(2);
    let result = mm + m;
    println!("500毫米 + 2米 = {:?}", result);  // Millimeters(2500)

    println!("运算符重载：");
    println!("  - 通过实现 std::ops 中的 trait 实现");
    println!("  - 不能重载任意运算符，只能重载预定义trait的运算符");
    println!("  - 可以重载不同类型间的运算");
}

// ============================================================
// 8. Drop trait
// ============================================================
// Drop trait 允许我们自定义值离开作用域时的行为
// 类似于其他语言的析构函数
//
// Drop trait 只有一个方法：drop(&mut self)
// 当值离开作用域时，Rust会自动调用 drop 方法
//
// 用途：
//   - 释放文件句柄
//   - 释放网络连接
//   - 释放锁
//   - 等等...
//
// Rust不允许手动调用 drop 方法（会导致双重释放）
// 如果需要提前释放，使用 std::mem::drop 函数

/// Drop trait演示函数
fn drop_trait_demo() {
    println!("\n--- 8. Drop trait ---");

    // ---- 实现 Drop trait ----
    struct CustomSmartPointer {
        data: String,
    }

    impl Drop for CustomSmartPointer {
        /// drop 方法：值离开作用域时自动调用
        /// &mut self：可变借用，可以进行清理工作
        fn drop(&mut self) {
            println!("释放 CustomSmartPointer，数据: `{}`", self.data);
        }
    }

    {
        let _c = CustomSmartPointer {
            data: String::from("my stuff"),
        };
        let _d = CustomSmartPointer {
            data: String::from("other stuff"),
        };
        println!("创建了两个 CustomSmartPointer");
    }  // 在这里，_d 和 _c 离开作用域，自动调用 drop
    // drop 调用顺序与创建顺序相反（先创建的后释放）
    println!("两个指针已释放");

    // ---- 提前释放值：std::mem::drop ----
    // 不能直接调用 .drop() 方法（会编译错误）
    // 但可以使用标准库的 std::mem::drop 函数提前释放值
    let c = CustomSmartPointer {
        data: String::from("提前释放"),
    };
    println!("创建了一个 CustomSmartPointer");
    drop(c);  // 手动提前释放
    println!("在作用域结束前就释放了");

    // ---- Drop 的常见用途 ----
    // - RAII 模式（资源获取即初始化）
    // - 智能指针（Box, Rc, Arc 等都实现了 Drop）
    // - 文件、网络连接等资源的自动释放

    println!("Drop trait：");
    println!("  - 值离开作用域时自动调用");
    println!("  - 用于资源清理（RAII模式）");
    println!("  - 不能手动调用.drop()，用 std::mem::drop() 提前释放");
}

// ============================================================
// 9. Deref trait
// ============================================================
// Deref trait 用于自定义解引用运算符 * 的行为
// 实现了 Deref 的类型可以像引用一样使用
//
// Deref trait 定义：
//   trait Deref {
//       type Target: ?Sized;
//       fn deref(&self) -> &Self::Target;
//   }
//
// 解引用强制转换（Deref coercion）：
//   - 当 T: Deref<Target=U> 时，&T 会自动转换为 &U
//   - 这样函数接受 &U 时，可以直接传入 &T
//   - 例如：String 实现了 Deref<Target=str>，所以 &String 可以自动转为 &str

/// Deref trait演示函数
fn deref_trait_demo() {
    println!("\n--- 9. Deref trait ---");

    use std::ops::Deref;

    // ---- 自定义智能指针 ----
    struct MyBox<T>(T);  // 元组结构体

    impl<T> MyBox<T> {
        fn new(x: T) -> MyBox<T> {
            MyBox(x)
        }
    }

    // 为 MyBox 实现 Deref trait
    impl<T> Deref for MyBox<T> {
        // Target 关联类型：指定解引用后得到的类型
        type Target = T;

        // deref 方法：返回对内部值的引用
        // Rust 会自动调用这个方法来实现 * 解引用
        fn deref(&self) -> &Self::Target {
            &self.0  // 返回元组的第一个元素的引用
        }
    }

    // 使用解引用运算符
    let x = 5;
    let y = MyBox::new(x);

    assert_eq!(5, x);
    assert_eq!(5, *y);  // 等价于 *(y.deref())
    println!("*y = {}", *y);
    // 解释：当我们写 *y 时，Rust 实际上调用了 y.deref() 然后解引用结果

    // ---- 解引用强制转换（Deref coercion） ----
    // Rust会自动进行解引用强制转换，让代码更简洁
    // 规则：当 T: Deref<Target=U> 时，&T 自动转换为 &U

    // 这个函数接受 &str 类型
    fn hello(name: &str) {
        println!("你好，{}！", name);
    }

    let m = MyBox::new(String::from("Rust"));
    // 这里发生了两次解引用强制转换：
    // 1. &MyBox<String> → &String（因为 MyBox 实现了 Deref<Target=String>）
    // 2. &String → &str（因为 String 实现了 Deref<Target=str>）
    hello(&m);
    // 如果没有解引用强制转换，我们需要写成：
    // hello(&(*m)[..]);

    // ---- DerefMut trait ----
    // DerefMut 用于可变引用的解引用
    // 类似于 Deref，但返回 &mut Target
    use std::ops::DerefMut;

    impl<T> DerefMut for MyBox<T> {
        fn deref_mut(&mut self) -> &mut Self::Target {
            &mut self.0
        }
    }

    let mut m2 = MyBox::new(String::from("hello"));
    m2.push_str(" world");  // 通过 DerefMut 自动解引用并调用 String 的方法
    println!("修改后: {}", *m2);

    println!("Deref trait：");
    println!("  - 自定义解引用运算符 * 的行为");
    println!("  - 解引用强制转换：&T 自动转为 &U（当 T: Deref<Target=U>）");
    println!("  - 让智能指针可以像普通引用一样使用");
    println!("  - DerefMut 用于可变引用");

    println!("\nOOP与Trait模块演示完毕！");
    println!("核心要点：");
    println!("  - Trait定义行为，类型实现Trait");
    println!("  - 泛型+Trait bound：编译时多态（静态分发）");
    println!("  - Trait对象(dyn)：运行时多态（动态分发）");
    println!("  - 运算符重载通过实现对应trait完成");
    println!("  - Drop/Deref等trait是Rust智能指针的基础");
}
