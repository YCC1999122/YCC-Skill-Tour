// ============================================================
// basics.rs - Rust基础语法模块
// ============================================================
// 本模块涵盖Rust语言的基础语法知识，包括：
//   - 变量与可变性（let/mut/const/static）
//   - 基本数据类型（标量/复合）
//   - 函数、语句与表达式
//   - 控制流（if/loop/while/for/match）
//   - 模式匹配（match/if let/while let）
//   - 字符串（String vs &str）
//   - 元组、数组、切片
//   - 注释与文档注释
// ============================================================

// 这是普通的单行注释，用于解释代码逻辑
// Rust的注释风格与C/C++类似，使用 // 表示单行注释
// 使用 /* ... */ 表示多行注释（块注释）

/// 这是文档注释（Doc Comment），使用三个斜杠 ///
/// 文档注释支持 Markdown 格式，可以用 `cargo doc` 生成HTML文档
/// 文档注释应该放在被说明的项（函数、结构体、模块等）的上方
///
/// # 示例
///
/// ```
/// let result = add(2, 3);
/// assert_eq!(result, 5);
/// ```
pub fn run_basics_demo() {
    println!("\n{}", "=".repeat(60));
    println!("基础语法演示");
    println!("{}", "=".repeat(60));

    variables_demo();
    data_types_demo();
    functions_demo();
    control_flow_demo();
    pattern_matching_demo();
    string_demo();
    tuple_array_slice_demo();
}

// ============================================================
// 1. 变量与可变性
// ============================================================
// Rust的变量默认是不可变的（immutable），这是Rust的核心设计之一
// 这种设计鼓励编写更安全、更易于并发的代码
// 要使变量可变，需要显式使用 mut 关键字

/// 变量与可变性演示函数
/// 展示 let, mut, const, static 的用法和区别
fn variables_demo() {
    println!("\n--- 1. 变量与可变性 ---");

    // ---- 不可变变量（let） ----
    // 使用 let 声明的变量默认是不可变的
    // 一旦绑定了值，就不能再修改
    let x = 5;          // 声明一个不可变变量 x，绑定值为 5
    println!("不可变变量 x = {}", x);  // 输出：不可变变量 x = 5

    // 下面这行代码会编译错误，因为 x 是不可变的
    // x = 6;  // 错误：cannot assign twice to immutable variable `x`

    // ---- 可变变量（mut） ----
    // 使用 mut 关键字声明可变变量
    // mut 是 mutable 的缩写
    let mut y = 5;      // 声明一个可变变量 y，初始值为 5
    println!("可变变量 y 的初始值 = {}", y);  // 输出：可变变量 y 的初始值 = 5
    y = 10;             // 修改 y 的值（因为 y 是可变的，所以允许）
    println!("修改后 y = {}", y);         // 输出：修改后 y = 10

    // ---- 变量遮蔽（Shadowing） ----
    // Rust允许使用相同的变量名声明新变量，新变量会"遮蔽"旧变量
    // 这与 mut 不同：shadowing 是创建了一个全新的变量（可以改变类型）
    // 而 mut 是修改同一个变量的值（类型不能变）
    let z = 5;          // 第一个 z，类型是 i32，值为 5
    let z = z + 1;      // 第二个 z，遮蔽了第一个 z，值为 6
    let z = z * 2;      // 第三个 z，遮蔽了第二个 z，值为 12
    println!("变量遮蔽后 z = {}", z);     // 输出：变量遮蔽后 z = 12

    // 遮蔽还可以改变变量的类型，这是 mut 做不到的
    let spaces = "   "; // spaces 是 &str 类型（字符串切片）
    let spaces = spaces.len();  // 新的 spaces 是 usize 类型（长度值）
    println!("spaces（遮蔽后类型改变） = {}", spaces);  // 输出：3

    // ---- 常量（const） ----
    // 常量在整个程序运行期间都是不可变的
    // 与 let 的区别：
    //   1. 不允许使用 mut，常量永远不可变
    //   2. 必须标注类型（不能省略）
    //   3. 只能赋值为常量表达式（不能是运行时计算的值）
    //   4. 常量的命名规范是全部大写，单词间用下划线分隔
    //   5. 常量可以声明在任何作用域（包括全局作用域）
    const MAX_POINTS: u32 = 100_000;  // 声明一个 u32 类型的常量
    println!("常量 MAX_POINTS = {}", MAX_POINTS);

    // 数字中可以使用下划线 _ 作为千位分隔符，提高可读性
    // 100_000 和 100000 是完全一样的
    const ONE_MILLION: i64 = 1_000_000;
    println!("常量 ONE_MILLION = {}", ONE_MILLION);

    // ---- 静态变量（static） ----
    // static 声明的是静态变量，具有 'static 生命周期
    // 与 const 的区别：
    //   1. static 变量有固定的内存地址（可以引用其地址）
    //   2. const 更像是文本替换，每次使用都是拷贝
    //   3. static 可以是可变的（使用 static mut，但不安全）
    // 访问可变静态变量需要使用 unsafe 块
    static HELLO: &str = "Hello, Rust!";  // 静态字符串引用
    println!("静态变量 HELLO = {}", HELLO);
}

// ============================================================
// 2. 基本数据类型
// ============================================================
// Rust是静态类型语言，编译时必须知道所有变量的类型
// Rust的类型系统非常强大，可以帮助我们在编译时捕获错误
//
// 数据类型分为两大类：
//   - 标量类型（Scalar）：代表单个值
//       - 整数类型：i8, i16, i32, i64, i128, isize（有符号）
//                   u8, u16, u32, u64, u128, usize（无符号）
//       - 浮点类型：f32, f64
//       - 布尔类型：bool
//       - 字符类型：char
//   - 复合类型（Compound）：将多个值组合成一个类型
//       - 元组（Tuple）
//       - 数组（Array）

/// 基本数据类型演示函数
fn data_types_demo() {
    println!("\n--- 2. 基本数据类型 ---");

    // ---- 整数类型 ----
    // 有符号整数：i8, i16, i32, i64, i128, isize
    //   - 数字表示位数，i 表示 signed（有符号）
    //   - isize 的位数取决于操作系统：64位系统是64位，32位系统是32位
    // 无符号整数：u8, u16, u32, u64, u128, usize
    //   - u 表示 unsigned（无符号），只能表示非负数
    //   - usize 常用于集合的索引和大小
    let a: i32 = 42;        // 32位有符号整数（Rust默认整数类型）
    let b: u64 = 100;       // 64位无符号整数
    let c: isize = 1000;    // 指针大小的有符号整数
    let d: usize = 2000;    // 指针大小的无符号整数
    println!("整数: i32={}, u64={}, isize={}, usize={}", a, b, c, d);

    // 整数字面量的写法：
    let decimal = 98_222;       // 十进制（可以加下划线分隔）
    let hex = 0xff;             // 十六进制（0x前缀）
    let octal = 0o77;           // 八进制（0o前缀）
    let binary = 0b1111_0000;   // 二进制（0b前缀）
    let byte = b'A';            // 字节字面量（仅u8类型，b前缀）
    println!("字面量: dec={}, hex={}, oct={}, bin={}, byte={}",
             decimal, hex, octal, binary, byte);

    // ---- 浮点类型 ----
    // f32: 单精度浮点数（32位）
    // f64: 双精度浮点数（64位），Rust默认浮点类型，精度更高
    let x: f64 = 2.0;       // 64位浮点数（默认）
    let y: f32 = 3.0;       // 32位浮点数
    println!("浮点数: f64={}, f32={}", x, y);

    // 基本数学运算
    let sum = 5 + 10;               // 加法
    let difference = 95.5 - 4.3;    // 减法
    let product = 4 * 30;           // 乘法
    let quotient = 56.7 / 32.2;     // 除法
    let truncated = -5 / 3;         // 整数除法（向零截断），结果为 -1
    let remainder = 43 % 5;         // 取余（模运算）
    println!("运算: sum={}, diff={}, prod={}, quot={}, trunc={}, rem={}",
             sum, difference, product, quotient, truncated, remainder);

    // ---- 布尔类型 ----
    // bool 类型有两个可能的值：true 和 false
    // 占用 1 个字节的内存
    let is_rust_fun: bool = true;   // 显式标注类型
    let is_learning = false;        // 类型推断为 bool
    println!("布尔值: is_rust_fun={}, is_learning={}", is_rust_fun, is_learning);

    // ---- 字符类型 ----
    // Rust的 char 类型代表一个 Unicode 标量值
    // 占用 4 个字节（32位），可以表示ASCII、中文、日文、emoji等
    // 注意：char 使用单引号 ''，字符串使用双引号 ""
    let c: char = 'z';              // 英文字符
    let heart_eyed_cat = '😻';     // emoji表情（Unicode字符）
    let chinese_char = '中';        // 中文字符
    println!("字符: c={}, emoji={}, 中文={}", c, heart_eyed_cat, chinese_char);
    println!("char 类型占用 {} 字节", std::mem::size_of::<char>());
}

// ============================================================
// 3. 函数、语句与表达式
// ============================================================
// Rust是一种基于表达式的语言，这一点与很多语言不同
// 语句（Statement）：执行一些操作但不返回值的指令
// 表达式（Expression）：计算并产生一个值
//
// 重要区别：
//   - 语句以分号 ; 结尾
//   - 表达式不以分号结尾（加上分号就变成了语句，会丢弃返回值）
//
// 函数体由一系列语句组成，最后可以由一个表达式结束（作为返回值）
// Rust函数使用 fn 关键字声明，格式：
//   fn 函数名(参数名: 类型, ...) -> 返回类型 { 函数体 }

/// 函数演示函数
fn functions_demo() {
    println!("\n--- 3. 函数、语句与表达式 ---");

    // 调用函数并接收返回值
    let result = add(5, 3);         // 调用 add 函数，传入参数 5 和 3
    println!("add(5, 3) = {}", result);  // 输出：8

    let squared = square(4);        // 调用 square 函数
    println!("square(4) = {}", squared);  // 输出：16

    // 语句 vs 表达式
    // let 是语句，不能赋值给另一个变量
    // 下面这行是错误的：
    // let x = (let y = 6);  // 错误：let 语句不能赋值给其他变量

    // 块（{}）是表达式，可以返回值
    let x = 5;
    let y = {
        let x = 3;          // 这里的 x 遮蔽了外面的 x
        x + 1               // 这是表达式（没有分号），作为整个块的返回值
    };
    println!("x = {}, y = {}", x, y);  // 输出：x = 5, y = 4
    // 解释：外面的 x 还是 5（因为块内的 x 是新变量），y 是块的返回值 3+1=4

    // 空返回值（unit type）
    // 没有显式返回值的函数返回 ()，称为单元类型（unit type）
    let unit_val = do_nothing();
    println!("do_nothing() 返回值: {:?}", unit_val);  // 输出：()
}

/// 两数相加函数
///
/// # 参数
/// * `a` - 第一个整数（i32类型）
/// * `b` - 第二个整数（i32类型）
///
/// # 返回值
/// 返回 a + b 的结果（i32类型）
fn add(a: i32, b: i32) -> i32 {
    // 函数参数必须声明类型，这是Rust的要求
    // 返回类型使用 -> 符号指定
    a + b   // 这是表达式（没有分号），作为函数的返回值
            // 等价于 return a + b; 但Rust中更习惯使用表达式隐式返回
}

/// 计算平方数的函数
/// 展示了表达式作为返回值的用法
fn square(x: i32) -> i32 {
    x * x   // 最后一个表达式的值就是返回值
            // 不需要写 return（当然写了也没错）
}

/// 无返回值的函数
/// 返回单元类型 ()，类似于其他语言的 void
fn do_nothing() {
    // 没有返回值的函数，隐式返回 ()
    // 也可以显式写成 -> ()，但通常省略
}

// ============================================================
// 4. 控制流
// ============================================================
// Rust提供了多种控制流结构：
//   - if 表达式：条件分支
//   - loop 循环：无限循环
//   - while 循环：条件循环
//   - for 循环：遍历集合
//   - match 模式匹配：强大的分支结构（在后面单独介绍）

/// 控制流演示函数
fn control_flow_demo() {
    println!("\n--- 4. 控制流 ---");

    // ---- if 表达式 ----
    // if 表达式根据条件执行不同的代码分支
    // 注意：Rust中 if 是表达式（有返回值），不是语句
    let number = 3;
    if number < 5 {
        println!("{} 小于5", number);  // 条件为真时执行
    } else {
        println!("{} 大于等于5", number);  // 条件为假时执行
    }

    // if 的条件必须是 bool 类型
    // Rust不会自动将非布尔值转换为布尔值
    // 下面这行会报错：
    // if number { ... }  // 错误：expected `bool`, found integer

    // 多条件分支（else if）
    let number = 6;
    if number % 4 == 0 {
        println!("{} 能被4整除", number);
    } else if number % 3 == 0 {
        println!("{} 能被3整除", number);
    } else if number % 2 == 0 {
        println!("{} 能被2整除", number);
    } else {
        println!("{} 不能被4、3、2整除", number);
    }

    // 在 let 语句中使用 if（因为 if 是表达式）
    // 这类似于其他语言的三元运算符，但功能更强大
    let condition = true;
    let number = if condition { 5 } else { 6 };
    println!("if 表达式的结果: {}", number);  // 输出：5
    // 注意：if 和 else 的分支必须返回相同类型，否则编译错误

    // ---- loop 循环 ----
    // loop 会无限循环，直到遇到 break 关键字
    // loop 也是表达式，可以通过 break 返回值
    let mut counter = 0;
    let result = loop {
        counter += 1;       // counter = counter + 1
        if counter == 10 {
            break counter * 2;  // break 可以返回值，结束循环
        }
    };
    println!("loop 循环的结果: {}", result);  // 输出：20

    // ---- while 循环 ----
    // while 循环：只要条件为真就继续循环
    let mut number = 3;
    while number != 0 {
        println!("{}!", number);
        number -= 1;       // 每次减1
    }
    println!("while 循环结束！");

    // ---- for 循环 ----
    // for 循环用于遍历集合中的每一个元素
    // for 循环是Rust中最常用的循环结构，安全且简洁
    let arr = [10, 20, 30, 40, 50];  // 数组
    for element in arr.iter() {       // iter() 方法返回迭代器
        println!("数组元素: {}", element);
    }

    // 使用 Range（范围）进行循环
    // 1..4 表示从1到3（不包含4），称为左闭右开区间
    for number in 1..4 {
        println!("Range循环: {}", number);  // 输出 1, 2, 3
    }

    // rev() 方法可以反转Range
    for number in (1..4).rev() {
        println!("反向Range: {}", number);  // 输出 3, 2, 1
    }

    // 包含结束值的Range（使用 ..=）
    for number in 1..=3 {
        println!("包含结束值的Range: {}", number);  // 输出 1, 2, 3
    }
}

// ============================================================
// 5. 模式匹配
// ============================================================
// 模式匹配是Rust最强大的特性之一
// match 表达式允许我们将一个值与一系列模式进行比较，并根据匹配的模式执行代码
// 模式可以是字面量、变量名、通配符等
//
// 特点：
//   - match 是表达式（有返回值）
//   - 匹配必须是穷尽的（exhaustive），即必须覆盖所有可能的情况
//   - 可以结合 if 守卫（guard）添加额外条件

/// 模式匹配演示函数
fn pattern_matching_demo() {
    println!("\n--- 5. 模式匹配 ---");

    // ---- match 基本用法 ----
    // match 表达式的格式：
    // match 值 {
    //     模式1 => 表达式1,
    //     模式2 => 表达式2,
    //     ...
    // }
    let number = 3;
    match number {
        1 => println!("一"),
        2 => println!("二"),
        3 => println!("三"),
        4 => println!("四"),
        5 => println!("五"),
        _ => println!("其他数字"),  // _ 是通配符，匹配所有情况
    }

    // ---- match 作为表达式（有返回值） ----
    let number = 7;
    let day = match number {
        1 => "星期一",
        2 => "星期二",
        3 => "星期三",
        4 => "星期四",
        5 => "星期五",
        6 | 7 => "周末",   // | 表示或（多个模式匹配同一个分支）
        _ => "无效日期",
    };
    println!("第{}天是: {}", number, day);  // 输出：第7天是: 周末

    // ---- match 和 Option<T> ----
    // Option<T> 是Rust中非常重要的枚举，用于表示可能为空的值
    // 有两个变体：Some(value) - 包含值，None - 空值
    // match 是处理 Option 的主要方式
    let some_value: Option<i32> = Some(42);  // 包含值42的Option
    let none_value: Option<i32> = None;      // 空的Option

    // 匹配 Some 的情况，同时将内部值绑定到变量 x
    match some_value {
        Some(x) => println!("some_value 包含值: {}", x),  // 输出：42
        None => println!("some_value 是空的"),
    }

    match none_value {
        Some(x) => println!("none_value 包含值: {}", x),
        None => println!("none_value 是空的"),   // 输出这个
    }

    // ---- if let 语法 ----
    // 当我们只关心一种匹配情况，其他情况忽略时，
    // 使用 if let 比 match 更简洁
    let some_u8_value = Some(3u8);

    // 使用 match 的写法（比较繁琐）
    match some_u8_value {
        Some(3) => println!("值是3（match写法）"),
        _ => (),  // 其他情况什么都不做
    }

    // 使用 if let 的写法（更简洁）
    // 语法：if let 模式 = 表达式 { ... } else { ... }
    if let Some(3) = some_u8_value {
        println!("值是3（if let写法）");
    }

    // if let 也可以带 else
    let x = Some(5);
    if let Some(3) = x {
        println!("x是3");
    } else {
        println!("x不是3");  // 输出这个
    }

    // ---- while let 语法 ----
    // while let 循环：只要模式匹配成功就继续循环
    let mut stack = Vec::new();    // 创建一个空的动态数组
    stack.push(1);                 // 推入元素
    stack.push(2);
    stack.push(3);

    // pop() 方法返回 Option<T>：有元素返回 Some，没有返回 None
    // while let 会不断尝试匹配 Some(top)，直到 pop() 返回 None
    while let Some(top) = stack.pop() {
        println!("弹出栈顶元素: {}", top);  // 依次输出：3, 2, 1
    }
    println!("栈已空");

    // ---- 更多模式匹配技巧 ----
    // 匹配范围（仅适用于数字和 char 类型）
    let x = 'c';
    match x {
        'a'..='j' => println!("字母在a到j之间"),  // 匹配这个
        'k'..='z' => println!("字母在k到z之间"),
        _ => println!("其他字符"),
    }

    // 解构结构体（后面结构体章节会详细讲）
    let point = (1, 2);  // 元组
    match point {
        (0, 0) => println!("原点"),
        (x, 0) => println!("在x轴上，x={}", x),
        (0, y) => println!("在y轴上，y={}", y),
        (x, y) => println!("点坐标: ({}, {})", x, y),
    }

    // 匹配守卫（match guard）
    // 在模式后面加上 if 条件，进一步筛选匹配
    let num = Some(4);
    match num {
        Some(x) if x < 5 => println!("小于5的数: {}", x),  // 匹配这个
        Some(x) => println!("大于等于5的数: {}", x),
        None => (),
    }
}

// ============================================================
// 6. 字符串
// ============================================================
// Rust中有两种主要的字符串类型：
//   - &str：字符串切片（string slice），是对字符串数据的引用
//       - 固定大小，不可变
//       - 字符串字面量就是 &'static str 类型
//       - 通常以引用形式使用（&str）
//   - String：可增长的、可变的、有所有权的字符串类型
//       - 在堆上分配内存
//       - 可以修改、追加等
//
// 关系：String 可以 Deref 到 &str，所以大多数接受 &str 的函数也可以接受 &String
// 转换：
//   String -> &str: &s[..] 或 s.as_str() 或 &*s
//   &str -> String: s.to_string() 或 String::from(s)

/// 字符串演示函数
fn string_demo() {
    println!("\n--- 6. 字符串（String vs &str）---");

    // ---- &str（字符串切片） ----
    // 字符串字面量的类型是 &'static str
    // 'static 表示这个字符串的生命周期和整个程序一样长
    let s1: &str = "Hello, Rust!";     // 字符串字面量，类型 &str
    println!("&str: {}", s1);
    println!("&str 长度: {}", s1.len());  // 字节长度（不是字符数）

    // ---- String（字符串对象） ----
    // String 是在堆上分配的可增长字符串
    // 创建方式1：String::from()
    let mut s2 = String::from("Hello");  // 创建String
    println!("String: {}", s2);

    // 创建方式2：to_string() 方法
    let s3 = "World".to_string();       // 从 &str 转换为 String
    println!("to_string(): {}", s3);

    // 修改 String（追加内容）
    s2.push_str(", World!");             // 追加字符串切片
    println!("追加后: {}", s2);          // 输出：Hello, World!

    s2.push('!');                        // 追加单个字符
    println!("追加字符后: {}", s2);      // 输出：Hello, World!!

    // 字符串拼接
    let s4 = String::from("Hello, ");
    let s5 = String::from("Rust!");
    let s6 = s4 + &s5;                   // 使用 + 运算符拼接
    // 注意：+ 运算符会取得 s4 的所有权，然后将 s5 的内容追加进去
    // 所以 s4 之后就不能再使用了（所有权已转移）
    println!("拼接结果: {}", s6);

    // 使用 format! 宏拼接（更灵活，不会取得所有权）
    let s7 = String::from("tic");
    let s8 = String::from("tac");
    let s9 = String::from("toe");
    let tic_tac_toe = format!("{}-{}-{}", s7, s8, s9);
    println!("format! 拼接: {}", tic_tac_toe);
    // format! 不会获取所有权，所以 s7, s8, s9 仍然可用
    println!("s7 仍然可用: {}", s7);

    // ---- 字符串索引 ----
    // Rust的字符串不支持通过整数索引访问单个字符
    // 因为字符串是UTF-8编码的，一个字符可能占用多个字节
    // 下面这行会报错：
    // let h = s2[0];  // 错误：`String` cannot be indexed by `{integer}`

    // 正确的遍历方式1：按字符遍历
    println!("按字符遍历 \"hello\":");
    for c in "hello".chars() {
        println!("  {}", c);
    }

    // 正确的遍历方式2：按字节遍历
    println!("按字节遍历 \"hello\":");
    for b in "hello".bytes() {
        println!("  {}", b);  // 输出ASCII码值
    }

    // 字符串切片（必须在字符边界处切片，否则会panic）
    let hello = "你好，世界";
    // 因为中文每个字占3个字节，所以前6个字节是"你好"
    let s = &hello[0..6];    // 正确：在字符边界处
    println!("字符串切片 [0..6]: {}", s);  // 输出：你好

    // 字符串长度（字节数 vs 字符数）
    let chinese = "你好";
    println!("\"你好\" 的字节长度: {}", chinese.len());         // 6 字节
    println!("\"你好\" 的字符数量: {}", chinese.chars().count()); // 2 个字符

    // ---- &str 和 String 的转换 ----
    let string_var = String::from("Hello");

    // String -> &str（自动解引用，大多数情况下自动发生）
    let str_ref: &str = &string_var;  // 取引用自动转换为 &str
    println!("String -> &str: {}", str_ref);

    // &str -> String
    let string_var2: String = "World".to_string();
    let string_var3: String = String::from("World");
    println!("&str -> String (to_string): {}", string_var2);
    println!("&str -> String (String::from): {}", string_var3);
}

// ============================================================
// 7. 元组、数组、切片
// ============================================================
// 复合类型：将多个值组合在一起
//
// 元组（Tuple）：
//   - 将多个不同类型的值组合成一个复合类型
//   - 长度固定，声明后不能改变
//   - 类型：(T1, T2, T3, ...)
//   - 访问：通过 . 后跟索引（t.0, t.1, ...）
//
// 数组（Array）：
//   - 多个相同类型的值的集合
//   - 长度固定，声明后不能改变
//   - 类型：[T; N]（T是元素类型，N是长度）
//   - 访问：通过 [索引]
//
// 切片（Slice）：
//   - 引用集合中一段连续的元素序列，没有所有权
//   - 类型：&[T]（不可变）或 &mut [T]（可变）
//   - 字符串切片：&str
//   - 切片由两部分组成：指向数据的指针 + 长度

/// 元组、数组、切片演示函数
fn tuple_array_slice_demo() {
    println!("\n--- 7. 元组、数组、切片 ---");

    // ---- 元组（Tuple） ----
    // 元组可以包含不同类型的值
    // 声明方式：(值1, 值2, 值3, ...)
    let tup: (i32, f64, char, &str) = (500, 6.4, 'A', "hello");
    println!("元组: {:?}", tup);  // {:?} 用于调试输出

    // 访问元组元素：使用 . 后跟索引
    println!("元组第一个元素: {}", tup.0);    // 500
    println!("元组第二个元素: {}", tup.1);    // 6.4
    println!("元组第三个元素: {}", tup.2);    // A
    println!("元组第四个元素: {}", tup.3);    // hello

    // 解构（destructuring）元组
    // 将元组的各个值分别绑定到不同的变量
    let (x, y, z, w) = tup;
    println!("解构后: x={}, y={}, z={}, w={}", x, y, z, w);

    // 单元元组（unit tuple）：()
    // 这是一个特殊的元组，没有元素，叫做单元类型
    // 没有返回值的函数隐式返回单元元组
    let unit = ();
    println!("单元元组: {:?}", unit);

    // ---- 数组（Array） ----
    // 数组的所有元素类型相同，且长度固定
    // 声明方式1：[值1, 值2, 值3, ...]
    let arr1 = [1, 2, 3, 4, 5];
    println!("数组: {:?}", arr1);

    // 声明方式2：[初始值; 长度]
    // 创建一个包含5个元素的数组，每个元素都是3
    let arr2 = [3; 5];
    println!("初始值相同的数组: {:?}", arr2);  // [3, 3, 3, 3, 3]

    // 显式标注类型：[类型; 长度]
    let arr3: [i32; 4] = [10, 20, 30, 40];
    println!("显式类型标注的数组: {:?}", arr3);

    // 访问数组元素：使用 [索引]
    println!("数组第一个元素: {}", arr1[0]);    // 1
    println!("数组第三个元素: {}", arr1[2]);    // 3

    // 数组长度
    println!("数组长度: {}", arr1.len());       // 5

    // 越界访问会导致运行时panic（Rust的安全保证）
    // 下面这行在运行时会崩溃：
    // println!("{}", arr1[10]);  // panic: index out of bounds

    // ---- 切片（Slice） ----
    // 切片是对数组（或其他集合）一部分的引用
    // 格式：&集合[起始索引..结束索引]（左闭右开）
    let arr = [1, 2, 3, 4, 5];

    let slice1 = &arr[1..4];    // 引用数组的第2到第4个元素（索引1,2,3）
    println!("切片 [1..4]: {:?}", slice1);   // [2, 3, 4]

    let slice2 = &arr[..3];     // 从开头到索引3（不包含）
    println!("切片 [..3]: {:?}", slice2);    // [1, 2, 3]

    let slice3 = &arr[2..];     // 从索引2到末尾
    println!("切片 [2..]: {:?}", slice3);    // [3, 4, 5]

    let slice4 = &arr[..];      // 整个数组的切片
    println!("切片 [..]: {:?}", slice4);     // [1, 2, 3, 4, 5]

    // 可变切片
    let mut arr_mut = [1, 2, 3, 4, 5];
    let slice_mut = &mut arr_mut[1..4];  // 可变切片
    slice_mut[0] = 99;                   // 修改切片中的元素
    println!("修改后的数组: {:?}", arr_mut);  // [1, 99, 3, 4, 5]
    // 解释：切片是引用，修改切片会影响原始数组

    // 字符串切片 &str
    // 字符串切片就是对字符串的一部分的引用
    let s = String::from("hello world");
    let hello = &s[0..5];    // "hello"
    let world = &s[6..11];   // "world"
    println!("字符串切片: {} {}", hello, world);

    // ---- 元组结构体（快速预览，详见structs_enums.rs） ----
    // 元组结构体是结构体和元组的结合体
    // 这里只是快速提一下，详细内容在结构体章节
    println!("\n（元组结构体将在 structs_enums.rs 中详细讲解）");
}
