// ============================================================
// Program.cs - 主程序入口
// 使用顶级语句（Top-level statements）实现交互菜单
// 用户可以选择运行各个模块的演示
// ============================================================

// 导入各模块的命名空间
using CSharpLearning.Basics;
using CSharpLearning.OOP;
using CSharpLearning.Collections;
using CSharpLearning.Advanced;
using CSharpLearning.Patterns;

// ============================================================
// 主程序循环：显示菜单，等待用户选择
// ============================================================

// 显示欢迎信息
Console.WriteLine("========================================");
Console.WriteLine("      C# 学习项目 - 交互演示");
Console.WriteLine("========================================");
Console.WriteLine();

// 主循环：持续显示菜单直到用户选择退出
while (true)
{
    // 显示主菜单
    ShowMainMenu();

    // 读取用户输入
    Console.Write("\n请输入选项编号（0退出）：");
    string? input = Console.ReadLine();

    // 处理用户选择
    switch (input?.Trim())
    {
        case "0":
            // 退出程序
            Console.WriteLine("\n感谢使用 C# 学习项目！再见！");
            return;  // 顶级语句中直接 return 退出程序

        case "1":
            // 基础语法
            RunModule("基础语法", () => BasicsDemo.RunAllDemos());
            break;

        case "2":
            // 面向对象编程
            RunModule("面向对象编程", () => OopDemo.RunAllDemos());
            break;

        case "3":
            // 集合与泛型
            RunModule("集合与泛型", () => CollectionsDemo.RunAllDemos());
            break;

        case "4":
            // 高级特性
            RunModule("高级特性", () => AdvancedDemo.RunAllDemos());
            break;

        case "5":
            // 设计模式
            RunModule("设计模式", () => PatternsDemo.RunAllDemos());
            break;

        case "6":
            // 全部运行
            Console.WriteLine("\n即将运行所有模块演示...");
            Console.WriteLine("========================================");
            BasicsDemo.RunAllDemos();
            Console.WriteLine();
            OopDemo.RunAllDemos();
            Console.WriteLine();
            CollectionsDemo.RunAllDemos();
            Console.WriteLine();
            AdvancedDemo.RunAllDemos();
            Console.WriteLine();
            PatternsDemo.RunAllDemos();
            Console.WriteLine("\n所有模块演示完成！");
            break;

        default:
            // 无效输入
            Console.WriteLine("\n无效选项，请重新输入！");
            break;
    }

    // 等待用户按任意键继续
    Console.WriteLine("\n按任意键返回菜单...");
    Console.ReadKey(true);
    Console.Clear();  // 清屏
}

// ============================================================
// 辅助方法
// ============================================================

/// <summary>
/// 显示主菜单
/// </summary>
static void ShowMainMenu()
{
    Console.WriteLine("************** 主菜单 **************");
    Console.WriteLine("  1. 基础语法（Basics）");
    Console.WriteLine("     - 数据类型、变量、枚举、运算符");
    Console.WriteLine("     - 控制流、方法、数组、字符串、可空类型");
    Console.WriteLine();
    Console.WriteLine("  2. 面向对象编程（OOP）");
    Console.WriteLine("     - 类与对象、封装、继承、多态");
    Console.WriteLine("     - 抽象类、接口、属性、索引器");
    Console.WriteLine("     - 委托、事件、静态类、扩展方法");
    Console.WriteLine("     - 结构体、分部类、内部类");
    Console.WriteLine();
    Console.WriteLine("  3. 集合与泛型（Collections）");
    Console.WriteLine("     - 泛型基础、List、Dictionary、HashSet");
    Console.WriteLine("     - Queue、Stack、LinkedList");
    Console.WriteLine("     - 数组vsList性能、Lambda表达式");
    Console.WriteLine("     - LINQ查询语法、方法语法、进阶用法");
    Console.WriteLine();
    Console.WriteLine("  4. 高级特性（Advanced）");
    Console.WriteLine("     - 异常处理、特性、反射");
    Console.WriteLine("     - 异步编程、线程与任务");
    Console.WriteLine("     - 指针与不安全代码、模式匹配");
    Console.WriteLine("     - 记录类型 record");
    Console.WriteLine();
    Console.WriteLine("  5. 设计模式（Patterns）");
    Console.WriteLine("     - 单例模式、工厂模式");
    Console.WriteLine("     - 观察者模式、策略模式");
    Console.WriteLine("     - 装饰器模式、建造者模式");
    Console.WriteLine();
    Console.WriteLine("  6. 运行全部演示");
    Console.WriteLine("  0. 退出程序");
    Console.WriteLine("************************************");
}

/// <summary>
/// 运行指定模块的演示，并处理异常
/// </summary>
/// <param name="moduleName">模块名称</param>
/// <param name="action">要执行的操作</param>
static void RunModule(string moduleName, Action action)
{
    try
    {
        Console.WriteLine($"\n正在运行【{moduleName}】模块...");
        Console.WriteLine("========================================");
        action();
    }
    catch (Exception ex)
    {
        // 捕获并显示异常信息
        Console.WriteLine($"\n运行出错：{ex.Message}");
        Console.WriteLine($"异常类型：{ex.GetType().Name}");
        Console.WriteLine($"堆栈跟踪：{ex.StackTrace}");
    }
}
