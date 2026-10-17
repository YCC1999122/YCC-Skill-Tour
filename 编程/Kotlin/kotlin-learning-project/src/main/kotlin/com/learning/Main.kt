// ============================================================
// 包声明：主程序入口所在的包
// ============================================================
package com.learning

// 导入各个模块的演示函数
import com.learning.basics.runBasicsDemo
import com.learning.oop.runOopDemo
import com.learning.functions.runFunctionsDemo
import com.learning.collections.runCollectionsDemo
import com.learning.coroutines.runCoroutinesDemo
import com.learning.advanced.runAdvancedDemo
import com.learning.patterns.runPatternsDemo

// ============================================================
// 主程序入口
// Kotlin程序的入口点是顶层的main函数
// 这是一个交互菜单程序，用户可以选择运行不同模块的演示
// ============================================================
fun main() {
    // 打印欢迎信息
    printWelcomeBanner()

    // 主循环：显示菜单并处理用户输入
    while (true) {
        // 显示菜单
        printMainMenu()

        // 读取用户输入
        val input = readLine()?.trim()

        // 处理用户选择
        when (input) {
            "1" -> {
                printSectionTitle("基础语法")
                runBasicsDemo()
                waitForEnter()
            }
            "2" -> {
                printSectionTitle("面向对象")
                runOopDemo()
                waitForEnter()
            }
            "3" -> {
                printSectionTitle("函数式特性")
                runFunctionsDemo()
                waitForEnter()
            }
            "4" -> {
                printSectionTitle("集合与序列")
                runCollectionsDemo()
                waitForEnter()
            }
            "5" -> {
                printSectionTitle("协程")
                runCoroutinesDemo()
                waitForEnter()
            }
            "6" -> {
                printSectionTitle("高级特性")
                runAdvancedDemo()
                waitForEnter()
            }
            "7" -> {
                printSectionTitle("设计模式")
                runPatternsDemo()
                waitForEnter()
            }
            "8" -> {
                // 运行所有演示
                printSectionTitle("运行所有演示")
                println("将依次运行所有模块的演示...")
                println()

                runBasicsDemo()
                println()
                runOopDemo()
                println()
                runFunctionsDemo()
                println()
                runCollectionsDemo()
                println()
                runCoroutinesDemo()
                println()
                runAdvancedDemo()
                println()
                runPatternsDemo()

                waitForEnter()
            }
            "0", "q", "Q", "exit", "quit" -> {
                // 退出程序
                println()
                println("感谢使用Kotlin学习项目！")
                println("祝学习愉快，Bye!")
                println()
                return
            }
            else -> {
                // 无效输入
                println()
                println("⚠️  无效的选择：$input")
                println("请输入 0-8 之间的数字，或输入 q 退出")
                waitForEnter()
            }
        }
    }
}

// ============================================================
// 打印欢迎横幅
// ============================================================
fun printWelcomeBanner() {
    println()
    println("╔══════════════════════════════════════════════════════════════╗")
    println("║                                                              ║")
    println("║               🌟  Kotlin 学习项目  🌟                       ║")
    println("║                                                              ║")
    println("║           一个全面的Kotlin语言学习演示项目                   ║")
    println("║                                                              ║")
    println("╚══════════════════════════════════════════════════════════════╝")
    println()
    println("  项目包含以下学习模块：")
    println("    1. 基础语法 - 变量、数据类型、控制流、空安全等")
    println("    2. 面向对象 - 类、继承、接口、数据类、密封类等")
    println("    3. 函数式特性 - 高阶函数、Lambda、闭包、尾递归等")
    println("    4. 集合与序列 - List/Map/Set、序列操作、性能对比等")
    println("    5. 协程 - 异步编程、通道、流、结构化并发等")
    println("    6. 高级特性 - 泛型、反射、委托、DSL等")
    println("    7. 设计模式 - 单例、工厂、观察者、策略等常用模式")
    println()
    println("  每个模块都包含详细的中文注释和可运行的示例代码。")
    println()
}

// ============================================================
// 打印主菜单
// ============================================================
fun printMainMenu() {
    println()
    println("┌──────────────────────────────────────────────────────────────┐")
    println("│                      📋 主菜单                               │")
    println("├──────────────────────────────────────────────────────────────┤")
    println("│                                                              │")
    println("│  1️⃣  基础语法 (Basics)                                      │")
    println("│  2️⃣  面向对象 (OOP)                                         │")
    println("│  3️⃣  函数式特性 (Functions)                                 │")
    println("│  4️⃣  集合与序列 (Collections)                               │")
    println("│  5️⃣  协程 (Coroutines)                                      │")
    println("│  6️⃣  高级特性 (Advanced)                                    │")
    println("│  7️⃣  设计模式 (Patterns)                                    │")
    println("│                                                              │")
    println("│  8️⃣  运行全部演示                                           │")
    println("│  0️⃣  退出程序                                               │")
    println("│                                                              │")
    println("└──────────────────────────────────────────────────────────────┘")
    println()
    print("请输入你的选择（0-8）： ")
}

// ============================================================
// 打印章节标题
// ============================================================
fun printSectionTitle(title: String) {
    println()
    println("──────────────────────────────────────────────────────────────")
    println("  📚 $title")
    println("──────────────────────────────────────────────────────────────")
    println()
}

// ============================================================
// 等待用户按回车键继续
// ============================================================
fun waitForEnter() {
    println()
    print("按回车键继续...")
    readLine()  // 等待用户输入一行（按回车）
}
