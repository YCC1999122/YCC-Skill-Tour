// ============================================================
// advanced.rs - 高级特性模块
// ============================================================
// 本模块涵盖Rust的高级特性，包括：
//   - 闭包（Fn/FnMut/FnOnce）
//   - 迭代器与闭包
//   - 智能指针（Box<T>/Rc<T>/RefCell<T>）
//   - 内部可变性
//   - 宏（macro_rules! 基础）
//   - 模块系统（mod/use/crate/super）
//   - 属性与派生宏（derive）
//   - 泛型函数与泛型结构体
// ============================================================

use std::cell::RefCell;  // 内部可变性
use std::rc::Rc;         // 引用计数智能指针

/// 高级特性模块的入口演示函数
pub fn run_advanced_demo() {
    println!("\n{}", "=".repeat(60));
    println!("高级特性演示");
    println!("{}", "=".repeat(60));

    closures_demo();
    iterators_and_closures_demo();
    smart_pointers_demo();
    interior_mutability_demo();
    macros_demo();
    module_system_demo();
    attributes_derive_demo();
    generic_functions_structs_demo();
}

// ============================================================
// 1. 闭包（Fn/FnMut/FnOnce）
// ============================================================
// 闭包（Closure）：可以捕获环境变量的匿名函数
// 闭包可以保存为变量，或作为参数传递给其他函数
//
// 闭包的语法：
//   |参数| 表达式              （单表达式）
//   |参数| { 表达式1; 表达式2 } （多表达式）
//
// 闭包的三个 trait（表示捕获环境的方式）：
//   - FnOnce：获取所有权，只能调用一次
//   - FnMut：可变借用，可以修改环境变量，可以调用多次
//   - Fn：不可变借用，只读访问环境变量，可以调用多次
//
// 三者的关系：
//   - Fn 是 FnMut 的超集（所有 Fn 都实现了 FnMut）
//   - FnMut 是 FnOnce 的超集（所有 FnMut 都实现了 FnOnce）
//   - 换句话说：Fn 最严格，FnOnce 最宽松

/// 闭包演示函数
fn closures_demo() {
    println!("\n--- 1. 闭包（Fn/FnMut/FnOnce）---");

    // ---- 闭包的基本语法 ----
    // 闭包可以保存为变量
    let add_one = |x: i32| -> i32 { x + 1 };
    println!("add_one(5) = {}", add_one(5));  // 6

    // 类型推断：闭包的参数和返回值类型通常可以自动推断
    let add_two = |x| x + 2;  // 编译器根据使用推断类型
    println!("add_two(5) = {}", add_two(5));  // 7

    // 多表达式闭包（用花括号）
    let calculate = |x, y| {
        let sum = x + y;
        let product = x * y;
        sum + product
    };
    println!("calculate(3, 4) = {}", calculate(3, 4));  // 3+4+12 = 19

    // ---- 闭包捕获环境变量 ----
    // 闭包可以捕获定义它的作用域中的变量
    let x = 4;

    // 闭包 equal_to_x 捕获了外部变量 x
    let equal_to_x = |z| z == x;
    let y = 4;
    println!("equal_to_x({}) = {}", y, equal_to_x(y));  // true

    // 闭包 vs 函数：
    // 函数不能捕获环境变量，闭包可以
    // 闭包可以捕获其所在作用域中的变量

    // ---- Fn/FnMut/FnOnce ----
    // 闭包根据捕获方式自动实现相应的 trait

    // Fn：不可变借用环境变量
    let s = String::from("hello");
    let print_s = || println!("Fn 闭包: {}", s);
    print_s();  // 第一次调用
    print_s();  // 可以多次调用
    println!("s 仍然可用: {}", s);  // s 仍然可用（只是被借用）

    // FnMut：可变借用环境变量（闭包本身必须是 mut）
    let mut count = 0;
    let mut inc = || {
        count += 1;  // 修改外部变量
        println!("FnMut 闭包: count = {}", count);
    };
    inc();  // count = 1
    inc();  // count = 2
    drop(inc);  // 释放闭包，归还 count 的可变借用
    println!("count = {}", count);  // 2

    // FnOnce：获取环境变量的所有权，只能调用一次
    let s = String::from("hello");
    let consume = || {
        let s2 = s;  // 取得 s 的所有权
        println!("FnOnce 闭包: 消耗了 '{}'", s2);
    };
    consume();  // 第一次调用（也是唯一一次）
    // consume();  // 错误：不能再次调用，因为 s 已经被移走了
    // println!("{}", s);  // 错误：s 的所有权已转移

    // ---- move 闭包 ----
    // 使用 move 关键字强制闭包获取环境变量的所有权
    let s = String::from("hello");
    let move_closure = move || {
        println!("move 闭包: {}", s);
    };
    // println!("{}", s);  // 错误：s 的所有权已转移到闭包中
    move_closure();

    println!("\n闭包的三个 trait：");
    println!("  - FnOnce: 获取所有权，只能调用一次");
    println!("  - FnMut: 可变借用，可调用多次，可修改环境");
    println!("  - Fn: 不可变借用，可调用多次，只读访问");
    println!("  - 所有闭包都实现了 FnOnce");
    println!("  - 不移动值的闭包实现了 FnMut");
    println!("  - 不修改值的闭包实现了 Fn");
}

// ============================================================
// 2. 迭代器与闭包
// ============================================================
// 迭代器和闭包是Rust函数式编程的核心
// 闭包常用作迭代器适配器的参数
//
// 这种组合的优点：
//   - 代码简洁、表达力强
//   - 零成本抽象（编译后和手写循环一样高效）
//   - 符合函数式编程风格

/// 迭代器与闭包演示函数
fn iterators_and_closures_demo() {
    println!("\n--- 2. 迭代器与闭包 ---");

    let numbers = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

    // ---- map + 闭包 ----
    let doubled: Vec<i32> = numbers.iter()
        .map(|x| x * 2)  // 闭包：每个元素乘2
        .collect();
    println!("map 乘2: {:?}", doubled);

    // ---- filter + 闭包 ----
    let evens: Vec<&i32> = numbers.iter()
        .filter(|x| *x % 2 == 0)  // 闭包：过滤偶数
        .collect();
    println!("filter 偶数: {:?}", evens);

    // ---- fold + 闭包 ----
    let sum = numbers.iter()
        .fold(0, |acc, x| acc + x);  // 闭包：累加
    println!("fold 求和: {}", sum);

    // ---- 复杂链式调用 ----
    // 找出所有大于3的偶数的平方和
    let result: i32 = numbers.iter()
        .filter(|&&x| x > 3)        // 大于3
        .filter(|&&x| x % 2 == 0)   // 偶数
        .map(|&x| x * x)            // 平方
        .sum();                     // 求和
    println!("大于3的偶数平方和: {}", result);  // 16 + 36 + 64 + 100 = 216

    // ---- 使用闭包作为函数参数 ----
    // 自定义一个接受闭包的函数
    fn apply<F>(x: i32, f: F) -> i32
    where
        F: Fn(i32) -> i32,  // F 必须是实现了 Fn(i32)->i32 的闭包（或函数）
    {
        f(x)  // 调用闭包
    }

    let result = apply(5, |x| x * x);  // 传入求平方的闭包
    println!("apply(5, 平方) = {}", result);  // 25

    let result = apply(10, |x| x + 1);  // 传入加1的闭包
    println!("apply(10, 加1) = {}", result);  // 11

    println!("迭代器 + 闭包：");
    println!("  - 函数式编程风格");
    println!("  - 代码简洁，表达力强");
    println!("  - 零成本抽象，性能和手写循环一样");
}

// ============================================================
// 3. 智能指针（Box<T>/Rc<T>/RefCell<T>）
// ============================================================
// 智能指针（Smart Pointer）：像指针一样行为，但有额外的元数据和功能
// Rust中，智能指针通常实现了 Deref 和 Drop trait
//
// 常见的智能指针：
//   - Box<T>：堆上分配，单一所有权
//   - Rc<T>：引用计数，共享所有权（单线程）
//   - Arc<T>：原子引用计数，共享所有权（多线程）
//   - RefCell<T>：内部可变性，运行时借用检查（单线程）
//   - Mutex<T>：内部可变性，运行时借用检查（多线程）
//   - Cow<'a, B>：写时复制（Clone on Write）

/// 智能指针演示函数
fn smart_pointers_demo() {
    println!("\n--- 3. 智能指针（Box<T>/Rc<T>/RefCell<T>）---");

    // ---- Box<T> ----
    // Box 是最简单的智能指针，将值分配到堆上
    // 栈上保存指针，堆上保存数据
    // 用途：
    //   1. 在堆上存储数据（而非栈上）
    //   2. 实现递归类型（如链表、树）
    //   3. 实现 trait 对象（dyn Trait）

    let b = Box::new(5);  // 在堆上分配一个 i32
    println!("Box 指向的值: {}", b);  // 自动解引用
    // b 离开作用域时，自动释放堆内存（Drop trait）

    // Box 的解引用
    let x = 5;
    let y = Box::new(x);
    assert_eq!(5, x);
    assert_eq!(5, *y);  // 使用 * 解引用
    println!("*y = {}", *y);

    // 递归类型的例子：Cons list（类似链表）
    #[derive(Debug)]
    enum List {
        Cons(i32, Box<List>),  // 必须用 Box，否则递归类型大小无法计算
        Nil,
    }

    use List::{Cons, Nil};
    let list = Cons(1, Box::new(Cons(2, Box::new(Cons(3, Box::new(Nil))))));
    println!("递归列表: {:?}", list);

    // ---- Rc<T>（引用计数）----
    // Rc（Reference Counted）：通过引用计数实现共享所有权
    // 适用于单线程内，需要多个所有者共享数据的场景
    // 不是线程安全的（不能跨线程传递）

    #[derive(Debug)]
    struct Node {
        value: i32,
    }

    let a = Rc::new(Node { value: 5 });
    println!("创建 a，引用计数 = {}", Rc::strong_count(&a));  // 1

    // clone() 增加引用计数（不是深拷贝，只是增加计数）
    let b = Rc::clone(&a);
    println!("clone b 后，引用计数 = {}", Rc::strong_count(&a));  // 2

    {
        let c = Rc::clone(&a);
        println!("clone c 后，引用计数 = {}", Rc::strong_count(&a));  // 3
        // c 离开作用域，引用计数减 1
    }
    println!("c 离开后，引用计数 = {}", Rc::strong_count(&a));  // 2

    drop(b);  // 手动 drop，引用计数减 1
    println!("drop b 后，引用计数 = {}", Rc::strong_count(&a));  // 1

    println!("Rc 中的值: {}", a.value);

    // ---- RefCell<T> ----
    // RefCell 提供内部可变性（在后面详细讲）
    // 这里只简单提一下
    println!("\nBox / Rc / RefCell 对比：");
    println!("  - Box<T>: 单一所有权，堆分配");
    println!("  - Rc<T>: 共享所有权（单线程），引用计数");
    println!("  - RefCell<T>: 内部可变性（单线程），运行时借用检查");
    println!("  - Arc<T>: 共享所有权（多线程），原子引用计数");
    println!("  - Mutex<T>: 内部可变性（多线程），互斥锁");
}

// ============================================================
// 4. 内部可变性
// ============================================================
// 内部可变性（Interior Mutability）：
//   即使只有不可变引用，也能修改内部数据
//
// Rust的借用规则：
//   - 同一时间，只能有一个可变引用，或任意数量的不可变引用
//   - 引用必须总是有效的
//
// 但有时我们需要在"外部看起来不可变"的情况下修改内部数据
// 这就是内部可变性的用武之地
//
// RefCell<T>：
//   - 提供内部可变性
//   - 在运行时检查借用规则（而不是编译时）
//   - 违反规则会 panic（而不是编译错误）
//   - 只能用于单线程
//
// 常用方法：
//   - borrow()：获取不可变引用（返回 Ref<T>）
//   - borrow_mut()：获取可变引用（返回 RefMut<T>）
//   - 违反借用规则时，borrow/borrow_mut 会 panic

/// 内部可变性演示函数
fn interior_mutability_demo() {
    println!("\n--- 4. 内部可变性（RefCell<T>）---");

    // ---- RefCell 的基本用法 ----
    // 即使 RefCell 本身是不可变的，也能修改内部数据
    let value = RefCell::new(5);
    println!("初始值: {}", value.borrow());  // 5

    {
        // 获取可变引用并修改
        let mut mut_ref = value.borrow_mut();
        *mut_ref += 10;
        println!("修改后: {}", mut_ref);  // 15
    }  // mutable reference 离开作用域，归还借用

    println!("最终值: {}", value.borrow());  // 15

    // ---- 借用规则在运行时检查 ----
    // RefCell 在运行时检查借用规则
    // 同一时间只能有一个可变引用，或多个不可变引用
    // 违反时会 panic（而不是编译错误）

    // 正确：多个不可变引用
    let ref1 = value.borrow();
    let ref2 = value.borrow();
    println!("两个不可变引用: ref1={}, ref2={}", ref1, ref2);
    drop(ref1);
    drop(ref2);

    // 错误：不可变和可变引用同时存在
    // 下面的代码会 panic
    // let ref1 = value.borrow();       // 不可变引用
    // let ref_mut = value.borrow_mut(); // 可变引用 - panic!

    // ---- Rc<RefCell<T>> 模式 ----
    // Rc 提供共享所有权，RefCell 提供内部可变性
    // 两者结合：多个所有者共享可变数据（单线程内）

    #[derive(Debug)]
    struct Person {
        name: String,
        age: u32,
    }

    let person = Rc::new(RefCell::new(Person {
        name: String::from("张三"),
        age: 25,
    }));

    // clone 一个 Rc（增加引用计数）
    let person2 = Rc::clone(&person);

    // 通过 person2 修改内部数据
    person2.borrow_mut().age = 26;

    // 通过 person 查看数据（修改生效了）
    println!("修改后: {} - {}岁",
             person.borrow().name,
             person.borrow().age);  // 张三 - 26岁

    println!("\n内部可变性：");
    println!("  - RefCell<T>：运行时检查借用规则");
    println!("  - 外部不可变，内部可变");
    println!("  - Rc<RefCell<T>>：单线程共享可变数据");
    println!("  - Arc<Mutex<T>>：多线程共享可变数据");
    println!("  - 代价：运行时检查，违反规则会 panic");
}

// ============================================================
// 5. 宏（macro_rules! 基础）
// ============================================================
// 宏（Macro）：元编程的一种，在编译时生成代码
// Rust的宏有两种：
//   1. 声明式宏（Declarative Macro）：macro_rules!
//   2. 过程宏（Procedural Macro）：派生宏、属性宏、函数宏
//
// 宏 vs 函数：
//   - 宏在编译时展开，函数在运行时调用
//   - 宏可以有可变数量的参数
//   - 宏可以操作语法树
//   - 宏需要先定义才能使用（不像函数可以后定义）
//
// macro_rules! 语法：
//   macro_rules! 宏名 {
//       (模式1) => { 代码1 };
//       (模式2) => { 代码2 };
//       ...
//   }

/// 宏演示函数
fn macros_demo() {
    println!("\n--- 5. 宏（macro_rules! 基础）---");

    // ---- 常用标准库宏 ----
    // println!：格式化输出
    println!("println! 宏：这是一条输出");

    // vec!：创建向量
    let v = vec![1, 2, 3];
    println!("vec! 宏：{:?}", v);

    // format!：格式化字符串
    let s = format!("名字: {}, 年龄: {}", "小明", 18);
    println!("format! 宏：{}", s);

    // assert_eq!：断言相等
    assert_eq!(2 + 2, 4);
    println!("assert_eq! 宏：断言通过");

    // dbg!：调试输出（同时返回值）
    let x = 5;
    let y = dbg!(x * 2);  // 会打印调试信息
    println!("dbg! 结果: y = {}", y);

    // ---- 自定义宏 ----
    // 定义一个简单的宏：hello!
    macro_rules! hello {
        // 模式：没有参数
        () => {
            println!("你好，世界！");
        };
        // 模式：一个参数
        ($name:expr) => {
            println!("你好，{}！", $name);
        };
    }

    hello!();           // 调用无参数版本
    hello!("Rust");     // 调用带参数版本

    // 定义一个可变参数的宏：my_vec!
    macro_rules! my_vec {
        // 模式：$(...),* 表示零个或多个表达式，用逗号分隔
        ($($x:expr),*) => {
            {
                let mut temp_vec = Vec::new();
                // $( ... )* 表示对每个匹配的表达式重复展开
                $(
                    temp_vec.push($x);
                )*
                temp_vec
            }
        };
    }

    let v = my_vec![1, 2, 3, 4, 5];
    println!("自定义 my_vec! 宏：{:?}", v);

    // 定义一个 map 宏（类似 vec! 但是生成 HashMap）
    macro_rules! hash_map {
        ($($key:expr => $value:expr),*) => {
            {
                let mut map = std::collections::HashMap::new();
                $(
                    map.insert($key, $value);
                )*
                map
            }
        };
    }

    let scores = hash_map! {
        "数学" => 95,
        "语文" => 88,
        "英语" => 92
    };
    println!("自定义 hash_map! 宏：{:?}", scores);

    println!("\n宏的特点：");
    println!("  - 编译时展开，生成代码");
    println!("  - 可以有可变参数");
    println!("  - 可以操作语法树");
    println!("  - 声明宏 macro_rules! 比较容易上手");
    println!("  - 过程宏更强大，但也更复杂");
}

// ============================================================
// 6. 模块系统
// ============================================================
// Rust的模块系统用于组织代码，控制可见性和作用域
//
// 模块系统的组成：
//   - crate：编译单元（库或二进制程序）
//   - mod：模块，用于组织代码
//   - use：将路径引入作用域
//   - pub：控制可见性
//
// 路径（Path）：
//   - 绝对路径：从 crate 根开始，crate::...
//   - 相对路径：从当前模块开始，self:: 或 super::
//
// 可见性规则：
//   - 默认是私有的（private）
//   - 使用 pub 关键字标记为公开（public）
//   - 父模块不能访问子模块的私有项
//   - 子模块可以访问父模块的所有项（包括私有）

/// 模块系统演示函数
fn module_system_demo() {
    println!("\n--- 6. 模块系统（mod/use/crate/super）---");

    // ---- 定义模块 ----
    // 使用 mod 关键字定义模块
    // 模块可以嵌套
    mod front_of_house {
        // 子模块
        pub mod hosting {
            // pub 函数：可以从外部访问
            pub fn add_to_waitlist() {
                println!("  添加到等待列表");
            }

            // 私有函数：只能在模块内部访问
            fn _seat_at_table() {
                println!("  安排座位");
            }
        }

        pub mod serving {
            pub fn take_order() {
                println!("  接单");
            }

            pub fn serve_order() {
                println!("  上菜");
            }
        }
    }

    // ---- 使用绝对路径调用 ----
    // crate:: 表示从 crate 根开始
    // 但在函数内部定义的模块，路径需要从外层函数开始
    println!("模块调用演示：");
    front_of_house::hosting::add_to_waitlist();
    front_of_house::serving::take_order();
    front_of_house::serving::serve_order();

    // ---- use 关键字 ----
    // use 可以将路径引入当前作用域，简化调用
    use front_of_house::hosting;

    hosting::add_to_waitlist();  // 现在可以直接用 hosting

    // ---- as 关键字 ----
    // 可以使用 as 给引入的项起别名
    use front_of_house::serving as service;
    service::serve_order();

    // ---- pub use（重新导出）----
    // pub use 可以将引入的项重新导出为自己的公开项
    // 外部用户可以通过你的模块访问它
    mod back_of_house {
        pub mod kitchen {
            pub fn cook() {
                println!("  烹饪中...");
            }
        }

        // 重新导出 kitchen，外部可以通过 back_of_house::cook 访问
        pub use kitchen::cook;
    }

    back_of_house::cook();  // 通过重新导出的方式调用

    // ---- super 关键字 ----
    // super 表示父模块（类似文件系统的 ..）
    mod parent {
        pub fn parent_func() {
            println!("  父模块函数");
        }

        pub mod child {
            pub fn child_func() {
                println!("  子模块函数");
                // 使用 super 访问父模块
                super::parent_func();
            }
        }
    }

    parent::child::child_func();

    // ---- 嵌套路径和通配符 ----
    // 嵌套路径：use a::b::{c, d}
    // 通配符：use a::b::* （引入所有公开项）

    println!("\n模块系统：");
    println!("  - mod 定义模块，组织代码");
    println!("  - pub 控制可见性");
    println!("  - use 引入路径，简化调用");
    println!("  - crate/super/self 路径关键字");
    println!("  - pub use 重新导出");
}

// ============================================================
// 7. 属性与派生宏（derive）
// ============================================================
// 属性（Attribute）：给代码添加元数据或指令
// 语法：#[属性名] 或 #![属性名]
//   - #[...] 作用于紧随其后的项
//   - #![...] 作用于整个模块/crate
//
// 常见属性：
//   - #[derive(...)]：自动派生 trait 实现
//   - #[allow(...)]：允许特定的编译器警告
//   - #[cfg(...)]：条件编译
//   - #[test]：标记测试函数
//   - #[derive(Debug)]：自动实现 Debug trait
//
// 派生宏（Derive Macro）：
//   - 过程宏的一种
//   - 通过 #[derive(TraitName)] 自动为类型生成 trait 实现
//   - 标准库支持的 derive trait：Debug, Clone, Copy, PartialEq, Eq,
//     PartialOrd, Ord, Hash, Default 等

/// 属性与派生宏演示函数
fn attributes_derive_demo() {
    println!("\n--- 7. 属性与派生宏（derive）---");

    // ---- derive 属性 ----
    // 自动实现多个 trait
    #[derive(Debug, Clone, PartialEq, Eq, Hash)]
    struct Person {
        name: String,
        age: u32,
    }

    let p1 = Person {
        name: String::from("小明"),
        age: 20,
    };

    // Debug trait：可以用 {:?} 打印
    println!("Debug: {:?}", p1);

    // Clone trait：可以 clone()
    let p2 = p1.clone();
    println!("Clone: p2 = {:?}", p2);

    // PartialEq trait：可以用 == 比较
    println!("p1 == p2 ? {}", p1 == p2);  // true

    // ---- Default trait ----
    // 可以为类型实现 Default trait，提供默认值
    #[derive(Debug, Default)]
    struct Configuration {
        host: String,
        port: u16,
        debug: bool,
    }

    // 使用 default() 创建默认值
    let config = Configuration::default();
    println!("Default: {:?}", config);

    // 也可以用结构体更新语法，基于默认值修改
    let custom_config = Configuration {
        host: String::from("localhost"),
        port: 8080,
        ..Default::default()
    };
    println!("自定义配置: {:?}", custom_config);

    // ---- 其他常用属性 ----
    // #[allow(dead_code)]：禁止"未使用代码"警告
    #[allow(dead_code)]
    fn unused_function() {
        println!("这个函数没被使用，但不会警告");
    }

    // #[cfg(test)]：只在测试模式下编译
    // 常用于单元测试模块
    println!("\n常用属性：");
    println!("  - #[derive(...)]：自动派生trait实现");
    println!("  - #[allow(...)]：允许特定警告");
    println!("  - #[cfg(...)]：条件编译");
    println!("  - #[test]：标记测试函数");
    println!("  - #[derive(Debug/Clone/Copy/PartialEq/Default)]");
}

// ============================================================
// 8. 泛型函数与泛型结构体
// ============================================================
// 泛型（Generics）：编写可以处理多种类型的代码
// 是Rust零成本抽象的重要组成部分
//
// 编译时单态化（Monomorphization）：
//   - 编译器为每个具体类型生成对应的代码
//   - 运行时无性能损失
//
// 泛型函数：
//   fn largest<T: PartialOrd>(list: &[T]) -> &T { ... }
//
// 泛型结构体：
//   struct Point<T> { x: T, y: T }
//
// 泛型枚举：
//   enum Option<T> { Some(T), None }
//   enum Result<T, E> { Ok(T), Err(E) }

/// 泛型函数与泛型结构体演示函数
fn generic_functions_structs_demo() {
    println!("\n--- 8. 泛型函数与泛型结构体 ---");

    // ---- 泛型函数 ----
    // 找出切片中的最大值
    fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
        let mut largest = list[0];
        for &item in list.iter() {
            if item > largest {
                largest = item;
            }
        }
        largest
    }

    let numbers = vec![34, 50, 25, 100, 65];
    println!("最大数字: {}", largest(&numbers));

    let chars = vec!['y', 'm', 'a', 'q'];
    println!("最大字符: {}", largest(&chars));

    // ---- 泛型结构体 ----
    #[derive(Debug)]
    struct Point<T> {
        x: T,
        y: T,
    }

    let int_point = Point { x: 5, y: 10 };
    let float_point = Point { x: 1.5, y: 4.2 };
    println!("整数点: {:?}", int_point);
    println!("浮点点点: {:?}", float_point);

    // 多类型参数
    #[derive(Debug)]
    struct Pair<T, U> {
        first: T,
        second: U,
    }

    let mixed = Pair { first: 5, second: "hello" };
    println!("混合对: {:?}", mixed);

    // ---- 泛型方法 ----
    impl<T> Point<T> {
        fn x(&self) -> &T {
            &self.x
        }
    }

    // 为特定类型实现方法
    impl Point<f64> {
        fn distance_from_origin(&self) -> f64 {
            (self.x.powi(2) + self.y.powi(2)).sqrt()
        }
    }

    let p = Point { x: 3.0, y: 4.0 };
    println!("距离原点: {}", p.distance_from_origin());

    // ---- 泛型枚举 ----
    // Option<T> 和 Result<T, E> 就是泛型枚举的例子
    let opt: Option<i32> = Some(42);
    let res: Result<i32, String> = Ok(100);
    println!("Option: {:?}", opt);
    println!("Result: {:?}", res);

    // ---- 单态化（Monomorphization）----
    // 泛型在编译时会被单态化
    // 例如：largest(&numbers) 和 largest(&chars)
    // 编译器会生成两个具体版本的函数
    // 一个用于 i32，一个用于 char
    // 运行时零开销

    println!("\n泛型的特点：");
    println!("  - 编写一次，适用于多种类型");
    println!("  - 编译时单态化，运行时零开销");
    println!("  - 配合 trait bound 约束类型行为");
    println!("  - 零成本抽象的核心之一");

    println!("\n高级特性模块演示完毕！");
    println!("核心要点：");
    println!("  - 闭包：捕获环境的匿名函数，Fn/FnMut/FnOnce");
    println!("  - 智能指针：Box/Rc/RefCell，管理内存");
    println!("  - 内部可变性：RefCell，运行时借用检查");
    println!("  - 宏：编译时代码生成，元编程");
    println!("  - 模块系统：组织代码，控制可见性");
    println!("  - 泛型：零成本抽象，单态化");
}
