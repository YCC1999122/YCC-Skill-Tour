// ============================================================
// structs_enums.rs - 结构体与枚举模块
// ============================================================
// 结构体（Struct）：自定义数据类型，将多个相关的值组合在一起
// 枚举（Enum）：定义一个类型可以是哪些变体的集合
//
// 结构体类型：
//   - 命名字段结构体（Named-field struct）：每个字段有名字
//   - 元组结构体（Tuple struct）：没有字段名，只有类型
//   - 类单元结构体（Unit-like struct）：没有字段
//
// 枚举类型：
//   - 每个变体可以有不同类型的数据
//   - Option<T>：可能为空的值
//   - Result<T, E>：可能失败的操作
// ============================================================

/// 结构体与枚举模块的入口演示函数
pub fn run_structs_enums_demo() {
    println!("\n{}", "=".repeat(60));
    println!("结构体与枚举演示");
    println!("{}", "=".repeat(60));

    struct_definition_demo();
    struct_methods_demo();
    enum_definition_demo();
    option_demo();
    result_demo();
    struct_update_syntax_demo();
    unit_struct_demo();
}

// ============================================================
// 1. 结构体定义
// ============================================================
// 结构体使用 struct 关键字定义
// 结构体有三种形式：
//   1. 命名字段结构体：每个字段有名字和类型，最常用
//   2. 元组结构体：没有字段名，只有类型序列
//   3. 类单元结构体：没有任何字段，类似于单元类型 ()

/// 结构体定义演示函数
fn struct_definition_demo() {
    println!("\n--- 1. 结构体定义 ---");

    // ---- 命名字段结构体 ----
    // 最常见的结构体形式，每个字段都有名字
    // 访问字段使用 . 操作符
    #[derive(Debug)]  // 派生 Debug trait，用于打印调试信息
    struct User {
        // username 字段：String 类型，表示用户名
        username: String,
        // email 字段：String 类型，表示邮箱
        email: String,
        // sign_in_count 字段：u64 类型，表示登录次数
        sign_in_count: u64,
        // active 字段：bool 类型，表示是否激活
        active: bool,
    }

    // 创建结构体实例
    // 注意：字段顺序可以和定义时不同（因为有名字）
    let user1 = User {
        email: String::from("someone@example.com"),
        username: String::from("someusername123"),
        active: true,
        sign_in_count: 1,
    };
    println!("用户1: {:?}", user1);  // {:?} 用于调试输出

    // 访问结构体字段
    println!("用户名: {}", user1.username);
    println!("邮箱: {}", user1.email);

    // 创建可变结构体实例
    // 整个实例必须是可变的，Rust不允许只将某个字段标记为可变
    let mut user2 = User {
        email: String::from("user2@example.com"),
        username: String::from("user2"),
        active: true,
        sign_in_count: 0,
    };
    user2.email = String::from("newemail@example.com");  // 修改字段
    println!("修改后的用户2邮箱: {}", user2.email);

    // 使用字段初始化简写语法
    // 当变量名和字段名相同时，可以简写
    let email = String::from("shorthand@example.com");
    let username = String::from("shorthand");
    let user3 = User {
        email,        // 等同于 email: email
        username,     // 等同于 username: username
        active: true,
        sign_in_count: 1,
    };
    println!("字段初始化简写: user3.username = {}", user3.username);

    // ---- 元组结构体 ----
    // 元组结构体没有字段名，只有字段类型
    // 适用于简单的、需要命名的元组
    #[derive(Debug)]
    struct Color(i32, i32, i32);  // RGB颜色
    #[derive(Debug)]
    struct Point(i32, i32, i32);  // 三维坐标

    // 虽然 Color 和 Point 有相同的字段类型，但它们是不同的类型
    let black = Color(0, 0, 0);
    let origin = Point(0, 0, 0);
    println!("黑色: {:?}", black);
    println!("原点: {:?}", origin);
    // black 和 origin 不能互相赋值，因为类型不同

    // 访问元组结构体的字段：使用 . 加索引
    println!("红色通道: {}", black.0);
    println!("绿色通道: {}", black.1);
    println!("蓝色通道: {}", black.2);

    // ---- 类单元结构体 ----
    // 没有任何字段的结构体，类似于单元类型 ()
    // 常用于在某个类型上实现 trait 但不需要存储数据的场景
    #[derive(Debug)]
    struct AlwaysEqual;

    let subject = AlwaysEqual;
    println!("类单元结构体: {:?}", subject);
}

// ============================================================
// 2. 结构体方法
// ============================================================
// 方法（Method）与函数类似，但它是在结构体（或枚举、trait对象）的上下文中定义的
// 方法的第一个参数总是 self，表示调用该方法的结构体实例
//
// 方法定义在 impl 块中：
//   impl 结构体名 { ... }
//
// self 的几种形式：
//   - &self：不可变借用，只读访问
//   - &mut self：可变借用，可以修改实例
//   - self：获取所有权，通常用于将实例转换为其他类型
//
// 关联函数（Associated Function）：
//   - 在 impl 块中定义，但不以 self 为参数
//   - 类似于其他语言的静态方法
//   - 使用 结构体名::函数名() 调用
//   - Self 关键字：表示 impl 块对应的类型

/// 结构体方法演示函数
fn struct_methods_demo() {
    println!("\n--- 2. 结构体方法 ---");

    // 定义一个矩形结构体
    #[derive(Debug)]
    struct Rectangle {
        width: u32,   // 宽度
        height: u32,  // 高度
    }

    // 在 impl 块中为 Rectangle 定义方法
    impl Rectangle {
        /// 计算矩形面积的方法
        /// &self 表示不可变借用，方法不会修改实例
        /// 返回 u32 类型的面积值
        fn area(&self) -> u32 {
            self.width * self.height  // 访问实例的字段
        }

        /// 判断矩形是否能容纳另一个矩形
        /// &self：当前矩形
        /// other：另一个矩形的引用
        fn can_hold(&self, other: &Rectangle) -> bool {
            self.width > other.width && self.height > other.height
        }

        /// 可变方法：修改矩形的尺寸
        /// &mut self 表示可变借用，可以修改实例
        fn scale(&mut self, factor: u32) {
            self.width *= factor;
            self.height *= factor;
        }

        /// 关联函数（类似静态方法）：创建一个正方形
        /// 不以 self 为参数，所以是关联函数而不是方法
        /// 使用 Rectangle::square(size) 调用
        /// Self 是 Rectangle 的别名
        fn square(size: u32) -> Self {
            Self {
                width: size,
                height: size,
            }
        }
    }

    // 创建矩形实例
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };

    // 调用方法（使用 . 操作符）
    println!("矩形面积: {}", rect1.area());
    // Rust 会自动引用和解引用，所以 rect1.area() 等价于 (&rect1).area()

    let rect2 = Rectangle {
        width: 10,
        height: 40,
    };
    let rect3 = Rectangle {
        width: 60,
        height: 45,
    };

    println!("rect1能容纳rect2吗？{}", rect1.can_hold(&rect2));  // true
    println!("rect1能容纳rect3吗？{}", rect1.can_hold(&rect3));  // false

    // 可变方法调用
    let mut rect4 = Rectangle { width: 10, height: 20 };
    println!("缩放前: {}x{} = {}", rect4.width, rect4.height, rect4.area());
    rect4.scale(2);  // 调用可变方法
    println!("缩放后: {}x{} = {}", rect4.width, rect4.height, rect4.area());

    // 调用关联函数（使用 :: 操作符）
    let sq = Rectangle::square(25);
    println!("正方形: {:?}, 面积: {}", sq, sq.area());

    // ---- 多个 impl 块 ----
    // Rust允许为同一个结构体定义多个 impl 块
    // 这在组织代码时很有用
    impl Rectangle {
        /// 另一个关联函数，放在另一个 impl 块中
        fn new(width: u32, height: u32) -> Rectangle {
            Rectangle { width, height }
        }
    }

    let rect5 = Rectangle::new(15, 25);
    println!("通过new创建的矩形: {:?}", rect5);
}

// ============================================================
// 3. 枚举定义与模式匹配
// ============================================================
// 枚举（Enum）允许定义一个类型，它可以是几个不同变体中的一个
// 与结构体不同，枚举的每个变体可以有不同类型的数据
//
// 枚举的强大之处在于与 match 表达式结合使用
// match 可以根据不同的变体执行不同的代码

/// 枚举定义与模式匹配演示函数
fn enum_definition_demo() {
    println!("\n--- 3. 枚举定义与模式匹配 ---");

    // ---- 基本枚举 ----
    // 定义一个 IP 地址类型的枚举
    #[derive(Debug)]
    enum IpAddrKind {
        V4,  // 变体1：IPv4
        V6,  // 变体2：IPv6
    }

    let four = IpAddrKind::V4;
    let six = IpAddrKind::V6;
    println!("IPv4: {:?}", four);
    println!("IPv6: {:?}", six);

    // ---- 带数据的枚举 ----
    // 每个变体可以携带不同类型和数量的数据
    #[derive(Debug)]
    enum IpAddr {
        // V4 变体包含四个 u8 组成的元组
        V4(u8, u8, u8, u8),
        // V6 变体包含一个 String
        V6(String),
    }

    let home = IpAddr::V4(127, 0, 0, 1);
    let loopback = IpAddr::V6(String::from("::1"));
    println!("家地址: {:?}", home);
    println!("回环地址: {:?}", loopback);

    // ---- 用 match 匹配枚举 ----
    // match 表达式可以根据枚举变体执行不同代码
    fn route(ip: IpAddr) {
        match ip {
            IpAddr::V4(a, b, c, d) => {
                println!("路由到 IPv4 地址: {}.{}.{}.{}", a, b, c, d);
            }
            IpAddr::V6(addr) => {
                println!("路由到 IPv6 地址: {}", addr);
            }
        }
    }

    route(home);
    route(loopback);

    // ---- 更复杂的枚举 ----
    // 每个变体可以有不同的结构
    #[derive(Debug)]
    enum Message {
        Quit,                       // 没有数据
        Move { x: i32, y: i32 },    // 匿名结构体
        Write(String),              // 包含一个 String
        ChangeColor(i32, i32, i32), // 包含三个 i32
    }

    // 为枚举定义方法（和结构体一样，用 impl 块）
    impl Message {
        fn call(&self) {
            match self {
                Message::Quit => println!("退出程序"),
                Message::Move { x, y } => println!("移动到 ({}, {})", x, y),
                Message::Write(text) => println!("写入: {}", text),
                Message::ChangeColor(r, g, b) => println!("改变颜色: RGB({}, {}, {})", r, g, b),
            }
        }
    }

    let m = Message::Write(String::from("hello"));
    m.call();

    let m2 = Message::Move { x: 10, y: 20 };
    m2.call();

    let m3 = Message::Quit;
    m3.call();

    // ---- if let 匹配枚举 ----
    // 当只关心一个变体时，if let 比 match 更简洁
    let some_message = Message::Write(String::from("测试"));
    if let Message::Write(text) = some_message {
        println!("if let 匹配: Write({})", text);
    } else {
        println!("不是 Write 消息");
    }

    println!("枚举特点：");
    println!("  - 每个变体可以有不同类型的数据");
    println!("  - 可以定义方法和关联函数");
    println!("  - 配合 match 使用，确保所有情况都被处理");
}

// ============================================================
// 4. Option<T> 枚举
// ============================================================
// Option<T> 是Rust标准库中定义的非常重要的枚举
// 它用于表示一个值可能存在，也可能不存在的情况
// 代替了其他语言中的 null（空指针）
//
// Option<T> 有两个变体：
//   - Some(T)：包含一个值 T
//   - None：没有值
//
// 为什么不用 null？
//   - null 是许多 bug 的来源（空指针异常）
//   - Option<T> 在类型系统层面强制我们处理空值的情况
//   - 编译器会确保我们不会忘记处理 None 的情况
//
// Option<T> 定义在标准库的 std::option 模块中，但由于它太常用了，
// Rust 自动将它引入到 prelude 中，所以不需要手动 use

/// Option<T> 枚举演示函数
fn option_demo() {
    println!("\n--- 4. Option<T> 枚举 ---");

    // ---- Option 的基本用法 ----
    // Some(T)：有值
    let some_number: Option<i32> = Some(5);  // 包含 i32 的 Option
    let some_string: Option<&str> = Some("hello");  // 包含 &str 的 Option
    println!("some_number = {:?}", some_number);
    println!("some_string = {:?}", some_string);

    // None：没有值
    // None 需要标注类型，因为编译器无法推断
    let absent_number: Option<i32> = None;
    println!("absent_number = {:?}", absent_number);

    // ---- 为什么 Option 是安全的？ ----
    // Option<T> 和 T 是不同的类型，不能直接混用
    // 这就强制我们在使用之前必须处理 None 的可能性
    let x: i32 = 5;
    let y: Option<i32> = Some(5);

    // 下面这行报错：i32 和 Option<i32> 不能直接相加
    // let sum = x + y;  // 错误：cannot add `Option<i32>` to `i32`

    // 必须先把 Option<T> 中的值取出来才能用
    // 这就确保了我们不会忘记处理空值的情况

    // ---- 处理 Option 的方法 ----
    // 方法1：match 表达式（最完整的方式）
    fn plus_one(x: Option<i32>) -> Option<i32> {
        match x {
            None => None,
            Some(i) => Some(i + 1),
        }
    }

    let five = Some(5);
    let six = plus_one(five);    // Some(6)
    let none = plus_one(None);   // None
    println!("plus_one(Some(5)) = {:?}", six);
    println!("plus_one(None) = {:?}", none);

    // 方法2：unwrap() - 如果是 Some 就返回值，如果是 None 就 panic
    // 不推荐在生产代码中使用，适合原型开发或确定不会是 None 的情况
    let val = Some(42);
    println!("unwrap(): {}", val.unwrap());  // 42

    // 下面这行会 panic
    // let none_val: Option<i32> = None;
    // none_val.unwrap();  // panic

    // 方法3：unwrap_or(default) - 如果是 None 返回默认值
    let val2: Option<i32> = None;
    println!("unwrap_or(0): {}", val2.unwrap_or(0));  // 0

    // 方法4：expect(msg) - 类似 unwrap，但可以自定义错误信息
    let val3 = Some(100);
    println!("expect(): {}", val3.expect("应该有值"));

    // 方法5：if let - 只关心 Some 的情况
    let config_max = Some(3u8);
    if let Some(max) = config_max {
        println!("最大值配置为: {}", max);
    }

    // ---- Option 的常用方法 ----
    let opt = Some(10);

    // map: 如果有值，对值应用函数，返回新的 Option
    let mapped = opt.map(|x| x * 2);
    println!("map: {:?}", mapped);  // Some(20)

    // and_then: 类似 map，但函数返回 Option（扁平化）
    let and_then_result = opt.and_then(|x| if x > 5 { Some(x * 2) } else { None });
    println!("and_then: {:?}", and_then_result);  // Some(20)

    // filter: 如果有值且满足条件，返回 Some，否则返回 None
    let filtered = opt.filter(|&x| x > 5);
    println!("filter(>5): {:?}", filtered);  // Some(10)

    // ok_or: 将 Option 转换为 Result
    let result = opt.ok_or("没有值");
    println!("ok_or: {:?}", result);  // Ok(10)

    println!("Option<T> 的意义：");
    println!("  - 替代 null，从类型层面防止空指针错误");
    println!("  - 强制开发者处理空值情况");
    println!("  - 编译时保证安全，无运行时开销");
}

// ============================================================
// 5. Result<T, E> 错误处理
// ============================================================
// Result<T, E> 是Rust中另一个重要的枚举，用于错误处理
// 它表示一个操作可能成功（返回 T），也可能失败（返回 E）
//
// Result<T, E> 有两个变体：
//   - Ok(T)：操作成功，包含值 T
//   - Err(E)：操作失败，包含错误信息 E
//
// ? 运算符：
//   - 用于传播错误
//   - 如果 Result 是 Ok，就取出里面的值继续执行
//   - 如果 Result 是 Err，就将这个 Err 作为整个函数的返回值
//   - 只能用于返回 Result 类型的函数

/// Result<T, E> 错误处理演示函数
fn result_demo() {
    println!("\n--- 5. Result<T, E> 错误处理 ---");

    // ---- Result 的基本用法 ----
    // 模拟一个可能失败的函数
    fn divide(a: f64, b: f64) -> Result<f64, String> {
        if b == 0.0 {
            // 失败：返回 Err，包含错误信息
            Err(String::from("不能除以零"))
        } else {
            // 成功：返回 Ok，包含结果
            Ok(a / b)
        }
    }

    let result1 = divide(10.0, 2.0);
    let result2 = divide(10.0, 0.0);
    println!("10/2 = {:?}", result1);  // Ok(5.0)
    println!("10/0 = {:?}", result2);  // Err("不能除以零")

    // ---- 用 match 处理 Result ----
    match divide(20.0, 4.0) {
        Ok(result) => println!("计算结果: {}", result),
        Err(error) => println!("计算失败: {}", error),
    }

    // ---- if let 处理 Result ----
    if let Ok(val) = divide(100.0, 25.0) {
        println!("if let 匹配成功: {}", val);
    }

    // ---- ? 运算符 ----
    // ? 运算符用于传播错误
    // 它必须用在返回 Result 的函数中
    fn calculate(a: f64, b: f64, c: f64) -> Result<f64, String> {
        // 先计算 a/b，如果失败，直接返回 Err
        let temp = divide(a, b)?;
        // 再计算 temp/c，如果失败，直接返回 Err
        let result = divide(temp, c)?;
        Ok(result)
    }

    // 上面的函数等价于：
    fn calculate_verbose(a: f64, b: f64, c: f64) -> Result<f64, String> {
        let temp = match divide(a, b) {
            Ok(v) => v,
            Err(e) => return Err(e),
        };
        let result = match divide(temp, c) {
            Ok(v) => v,
            Err(e) => return Err(e),
        };
        Ok(result)
    }

    let calc_result = calculate(100.0, 2.0, 5.0);
    println!("calculate(100, 2, 5) = {:?}", calc_result);  // Ok(10.0)

    let calc_error = calculate(100.0, 0.0, 5.0);
    println!("calculate(100, 0, 5) = {:?}", calc_error);  // Err("不能除以零")

    // ---- Result 的常用方法 ----
    let ok_val: Result<i32, &str> = Ok(42);
    let err_val: Result<i32, &str> = Err("出错了");

    // is_ok() / is_err()：判断是成功还是失败
    println!("ok_val.is_ok() = {}", ok_val.is_ok());    // true
    println!("err_val.is_err() = {}", err_val.is_err()); // true

    // unwrap()：如果 Ok 返回值，如果 Err 就 panic
    println!("ok_val.unwrap() = {}", ok_val.unwrap());   // 42
    // err_val.unwrap();  // 会 panic

    // unwrap_or(default)：如果 Err 返回默认值
    println!("err_val.unwrap_or(0) = {}", err_val.unwrap_or(0));  // 0

    // expect(msg)：类似 unwrap，自定义错误信息
    println!("ok_val.expect(...) = {}", ok_val.expect("应该成功"));

    // map：对 Ok 中的值应用函数
    let mapped = ok_val.map(|x| x * 2);
    println!("map: {:?}", mapped);  // Ok(84)

    // map_err：对 Err 中的值应用函数
    let mapped_err = err_val.map_err(|e| format!("错误: {}", e));
    println!("map_err: {:?}", mapped_err);

    // and_then：链式调用，函数返回 Result
    let and_then_result = ok_val.and_then(|x| Ok(x + 10));
    println!("and_then: {:?}", and_then_result);  // Ok(52)

    // ---- 解析字符串为数字 ----
    // 标准库中很多函数返回 Result
    let parsed: Result<i32, _> = "42".parse();
    match parsed {
        Ok(n) => println!("解析成功: {}", n),
        Err(e) => println!("解析失败: {}", e),
    }

    let parsed_err: Result<i32, _> = "abc".parse::<i32>();
    println!("解析'abc': {:?}", parsed_err);

    println!("Result<T, E> 的意义：");
    println!("  - 在类型层面表示可能失败的操作");
    println!("  - 强制开发者处理错误情况");
    println!("  - ? 运算符使错误传播简洁优雅");
}

// ============================================================
// 6. 结构体更新语法
// ============================================================
// 结构体更新语法：基于已有实例创建新实例，只修改部分字段
// 使用 .. 语法（结构体更新语法，struct update syntax）
// .. 后面跟已有实例，表示剩余字段使用该实例的值

/// 结构体更新语法演示函数
fn struct_update_syntax_demo() {
    println!("\n--- 6. 结构体更新语法 ---");

    #[derive(Debug, Clone)]
    struct User {
        username: String,
        email: String,
        active: bool,
        sign_in_count: u64,
    }

    let user1 = User {
        email: String::from("user1@example.com"),
        username: String::from("user1"),
        active: true,
        sign_in_count: 1,
    };
    println!("user1: {:?}", user1);

    // ---- 不使用更新语法（手动指定每个字段） ----
    let user2 = User {
        email: String::from("user2@example.com"),
        username: String::from("user2"),
        active: user1.active,            // 从 user1 复制
        sign_in_count: user1.sign_in_count,  // 从 user1 复制
    };
    println!("user2（手动复制）: {:?}", user2);

    // ---- 使用结构体更新语法 ----
    // ..user1 表示剩余的字段使用 user1 的值
    let user3 = User {
        email: String::from("user3@example.com"),
        username: String::from("user3"),
        ..user1  // 其他字段使用 user1 的值
    };
    println!("user3（更新语法）: {:?}", user3);

    // 注意：使用更新语法时，String 类型的字段会被移动（因为 String 没有 Copy trait）
    // 所以 user1 的 username 和 email 在 user3 创建后仍然有效
    // （因为我们显式指定了新的 email 和 username）
    // 但如果不指定，那些字段会被移动，原实例就不能再完整使用了

    // 使用 Clone trait 避免移动（完整复制）
    let user4 = User {
        email: String::from("user4@example.com"),
        ..user1.clone()  // clone 整个 user1，不影响 user1
    };
    println!("user4（clone+更新语法）: {:?}", user4);
    println!("user1 仍然有效: {:?}", user1);

    println!("结构体更新语法：");
    println!("  - 使用 ..实例名 指定剩余字段来源");
    println!("  - 减少重复代码");
    println!("  - 注意所有权转移问题");
}

// ============================================================
// 7. 单元结构体
// ============================================================
// 单元结构体（Unit Struct）：没有任何字段的结构体
// 类似于单元类型 ()
// 用途：
//   - 在不需要存储数据的类型上实现 trait
//   - 作为状态标记
//   - 类似于枚举的无数据变体，但它是独立的类型

/// 单元结构体演示函数
fn unit_struct_demo() {
    println!("\n--- 7. 单元结构体 ---");

    // ---- 单元结构体定义 ----
    #[derive(Debug, Clone, Copy)]
    struct Singleton;

    let s1 = Singleton;
    let s2 = Singleton;
    println!("单元结构体: {:?}", s1);
    println!("s1 和 s2 是同一类型的实例");

    // ---- 单元结构体实现 trait ----
    // 单元结构体常用于实现 trait 但不需要存储数据的场景
    trait Greeting {
        fn greet(&self) -> &str;
    }

    struct HelloGreeter;  // 单元结构体

    impl Greeting for HelloGreeter {
        fn greet(&self) -> &str {
            "Hello, World!"
        }
    }

    let greeter = HelloGreeter;
    println!("问候: {}", greeter.greet());

    // ---- 状态模式中的应用 ----
    // 单元结构体常用来表示状态
    #[derive(Debug)]
    struct Draft;    // 草稿状态
    #[derive(Debug)]
    struct PendingReview;  // 待审核状态
    #[derive(Debug)]
    struct Published;  // 已发布状态

    println!("状态表示: {:?}, {:?}, {:?}", Draft, PendingReview, Published);
    println!("（单元结构体常作为类型状态标记使用）");

    println!("\n结构体与枚举模块演示完毕！");
    println!("核心要点：");
    println!("  - 结构体：自定义数据类型，组织相关数据");
    println!("  - 方法：在 impl 块中定义，self 表示实例");
    println!("  - 枚举：一个类型的多种可能变体");
    println!("  - Option<T>：安全的空值表示");
    println!("  - Result<T,E>：优雅的错误处理");
}
