// ============================================================
// main.rs - 主程序入口（交互菜单）
// ============================================================
// 本文件是Rust学习项目的主入口
// 提供一个交互式菜单，用户可以选择运行不同模块的演示
//
// 主要功能：
//   - 声明所有模块（mod 语句）
//   - 读取用户输入（std::io）
//   - 显示菜单选项
//   - 根据用户选择调用对应模块的演示函数
// ============================================================

// ---- 模块声明 ----
// 使用 mod 关键字声明模块
// 每个 mod 对应 src/ 目录下的一个 .rs 文件
// Rust 编译器会自动查找对应的文件

// 基础语法模块
mod basics;

// 所有权与借用模块
mod ownership;

// 结构体与枚举模块
mod structs_enums;

// OOP与Trait模块
mod oop_traits;

// 集合与迭代器模块
mod collections;

// 并发编程模块
mod concurrency;

// 高级特性模块
mod advanced;

// 设计模式模块
mod patterns;

// ============================================================
// main 函数 - 程序入口点
// ============================================================
// Rust程序从 main 函数开始执行
// main 函数没有参数，也没有返回值（或者返回 Result）
fn main() {
    // 打印欢迎信息
    println!("{}", "=".repeat(60));
    println!("            Rust 语言学习项目");
    println!("            Rust Learning Project");
    println!("{}", "=".repeat(60));
    println!();
    println!("本项目包含 Rust 语言的核心知识点演示，每个模块都有");
    println!("详细的中文注释，帮助你理解 Rust 的设计思想和语法。");
    println!();

    // 主循环：不断显示菜单，直到用户选择退出
    loop {
        // 显示菜单
        print_menu();

        // 读取用户输入
        let mut choice = String::new();
        // std::io::stdin() 返回标准输入的句柄
        // read_line() 读取一行输入到字符串中
        // 返回 Result，需要处理可能的错误
        match std::io::stdin().read_line(&mut choice) {
            Ok(_) => {
                // 读取成功，处理输入
            }
            Err(e) => {
                println!("读取输入失败: {}", e);
                continue;
            }
        }

        // trim() 去除首尾空白（包括换行符）
        // parse() 将字符串解析为指定类型
        let choice: u32 = match choice.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("\n请输入有效的数字！\n");
                continue;
            }
        };

        // 根据选择执行对应操作
        match choice {
            1 => {
                println!("\n>>> 正在运行：基础语法演示 <<<");
                basics::run_basics_demo();
                pause();
            }
            2 => {
                println!("\n>>> 正在运行：所有权与借用演示 <<<");
                ownership::run_ownership_demo();
                pause();
            }
            3 => {
                println!("\n>>> 正在运行：结构体与枚举演示 <<<");
                structs_enums::run_structs_enums_demo();
                pause();
            }
            4 => {
                println!("\n>>> 正在运行：OOP与Trait演示 <<<");
                oop_traits::run_oop_traits_demo();
                pause();
            }
            5 => {
                println!("\n>>> 正在运行：集合与迭代器演示 <<<");
                collections::run_collections_demo();
                pause();
            }
            6 => {
                println!("\n>>> 正在运行：并发编程演示 <<<");
                concurrency::run_concurrency_demo();
                pause();
            }
            7 => {
                println!("\n>>> 正在运行：高级特性演示 <<<");
                advanced::run_advanced_demo();
                pause();
            }
            8 => {
                println!("\n>>> 正在运行：设计模式演示 <<<");
                patterns::run_patterns_demo();
                pause();
            }
            9 => {
                // 运行所有演示
                println!("\n>>> 正在运行：全部演示 <<<");
                println!("（这将运行所有模块的演示，可能需要一些时间）\n");

                basics::run_basics_demo();
                ownership::run_ownership_demo();
                structs_enums::run_structs_enums_demo();
                oop_traits::run_oop_traits_demo();
                collections::run_collections_demo();
                concurrency::run_concurrency_demo();
                advanced::run_advanced_demo();
                patterns::run_patterns_demo();

                println!("\n{}", "=".repeat(60));
                println!("   所有模块演示完毕！感谢学习 Rust！");
                println!("{}", "=".repeat(60));
                pause();
            }
            0 => {
                // 退出程序
                println!("\n感谢使用 Rust 学习项目！再见！\n");
                break;
            }
            _ => {
                // 无效选项
                println!("\n无效的选择，请输入 0-9 之间的数字。\n");
            }
        }
    }
}

// ============================================================
// print_menu 函数
// ============================================================
// 显示主菜单选项
fn print_menu() {
    println!("{}", "-".repeat(60));
    println!("                      主菜单");
    println!("{}", "-".repeat(60));
    println!("  1. 基础语法（变量、类型、函数、控制流、字符串等）");
    println!("  2. 所有权与借用（所有权规则、引用、生命周期等）");
    println!("  3. 结构体与枚举（struct, enum, Option, Result）");
    println!("  4. OOP与Trait（trait, 泛型, 运算符重载, Drop等）");
    println!("  5. 集合与迭代器（Vec, HashMap, Iterator等）");
    println!("  6. 并发编程（线程、通道、Mutex, Arc, Send/Sync）");
    println!("  7. 高级特性（闭包、智能指针、宏、模块系统等）");
    println!("  8. 设计模式（单例、工厂、策略、观察者、建造者等）");
    println!("  9. 运行全部演示");
    println!("  0. 退出程序");
    println!("{}", "-".repeat(60));
    print!("请输入选项 (0-9): ");

    // 使用 std::io::Write trait 的 flush() 方法确保输出立即显示
    // 因为 print! 不会自动刷新输出缓冲区
    use std::io::Write;
    std::io::stdout().flush().unwrap();
}

// ============================================================
// pause 函数
// ============================================================
// 暂停程序，等待用户按回车键继续
// 用于演示结束后，让用户有时间阅读输出内容
fn pause() {
    println!();
    println!("按回车键继续...");

    // 清空输入缓冲区，确保读取到的是新的输入
    let mut buf = String::new();
    // 读取一行，忽略结果（用户按回车后继续）
    let _ = std::io::stdin().read_line(&mut buf);
}
