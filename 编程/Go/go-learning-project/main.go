// ============================================================
// main 包是程序的入口包
// 每个可执行程序都必须有一个 main 包和 main 函数
// main 函数是程序的入口点，程序从这里开始执行
// ============================================================
package main

// ============================================================
// 导入各个学习模块
// 导入路径是模块名 + 子目录名
// 模块名在 go.mod 中定义为 "go-learning-project"
// ============================================================
import (
	"bufio"      // 带缓冲的 I/O，用于读取用户输入
	"fmt"        // 格式化输入输出
	"os"         // 操作系统相关功能，如标准输入、退出等
	"strconv"    // 字符串转换，用于将用户输入转为数字
	"strings"    // 字符串处理

	// 导入各个学习模块（使用模块相对路径）
	"go-learning-project/advanced"     // 高级特性模块
	"go-learning-project/basics"       // 基础语法模块
	"go-learning-project/concurrency"  // 并发编程模块
	"go-learning-project/oop"          // 面向对象模块
	"go-learning-project/patterns"     // 设计模式模块
	"go-learning-project/structures"   // 数据结构模块
)

// ============================================================
// main 函数是程序的入口
// 程序启动时自动调用 main 函数
// main 函数没有参数和返回值
// ============================================================
func main() {
	// 打印欢迎信息
	printWelcome()

	// 创建 bufio.Scanner 用于读取用户输入
	// Scanner 按行读取输入，比 fmt.Scan 更灵活
	// 可以处理包含空格的输入
	scanner := bufio.NewScanner(os.Stdin)

	// 主循环：不断显示菜单并处理用户选择
	for {
		// 显示主菜单
		printMenu()

		// 提示用户输入
		fmt.Print("\n请选择菜单选项 (输入数字后按回车，q 退出): ")

		// 读取一行输入
		// Scan() 返回 bool，表示是否成功读取到一行
		if !scanner.Scan() {
			// 读取失败（比如遇到 EOF），退出程序
			fmt.Println("\n读取输入失败，程序退出。")
			break
		}

		// 获取用户输入的文本
		input := scanner.Text()
		// 去除首尾空白字符
		input = strings.TrimSpace(input)

		// 处理退出命令
		if input == "q" || input == "Q" || input == "quit" || input == "exit" {
			fmt.Println("\n感谢使用 Go 语言学习项目，再见！")
			break
		}

		// 处理用户选择
		handleChoice(input)

		// 等待用户按回车继续
		fmt.Print("\n按回车键返回主菜单...")
		scanner.Scan() // 等待用户输入（忽略输入内容，只需要按回车）
	}

	// 检查 scanner 是否有错误
	if err := scanner.Err(); err != nil {
		fmt.Fprintf(os.Stderr, "读取输入时发生错误: %v\n", err)
		os.Exit(1) // 非零退出码表示异常退出
	}
}

// ============================================================
// printWelcome 打印欢迎信息
// ============================================================
func printWelcome() {
	fmt.Println("============================================================")
	fmt.Println("                    Go 语言学习项目")
	fmt.Println("============================================================")
	fmt.Println("  本项目包含 Go 语言的核心知识点，每个模块都有详细的代码")
	fmt.Println("  和中文注释。通过交互菜单可以选择运行各个模块的演示。")
	fmt.Println("============================================================")
}

// ============================================================
// printMenu 打印主菜单
// ============================================================
func printMenu() {
	fmt.Println("\n======================= 主菜单 =======================")
	fmt.Println("  1. 基础语法 (Basics)")
	fmt.Println("     - 数据类型、变量、运算符、控制流、函数")
	fmt.Println("     - 数组、切片、Map、Range、指针")
	fmt.Println()
	fmt.Println("  2. 面向对象 (OOP)")
	fmt.Println("     - 结构体、方法、接口")
	fmt.Println("     - 类型断言、结构体嵌入")
	fmt.Println("     - Stringer、error 接口")
	fmt.Println()
	fmt.Println("  3. 并发编程 (Concurrency)")
	fmt.Println("     - Goroutine、Channel、Select")
	fmt.Println("     - sync 包、生产者消费者")
	fmt.Println("     - Context、并发模式、数据竞争")
	fmt.Println()
	fmt.Println("  4. 高级特性 (Advanced)")
	fmt.Println("     - defer/panic/recover、自定义错误")
	fmt.Println("     - 反射、闭包、函数选项模式")
	fmt.Println("     - 泛型、embed")
	fmt.Println()
	fmt.Println("  5. 数据结构 (Structures)")
	fmt.Println("     - 链表、栈、队列")
	fmt.Println("     - 二叉搜索树、堆/优先队列")
	fmt.Println("     - 排序算法")
	fmt.Println()
	fmt.Println("  6. 设计模式 (Patterns)")
	fmt.Println("     - 单例、工厂、观察者")
	fmt.Println("     - 策略、装饰器、选项模式")
	fmt.Println()
	fmt.Println("  7. 运行所有模块")
	fmt.Println("     - 依次运行全部演示")
	fmt.Println()
	fmt.Println("  q. 退出程序")
	fmt.Println("=======================================================")
}

// ============================================================
// handleChoice 处理用户的菜单选择
// input: 用户输入的字符串
// ============================================================
func handleChoice(input string) {
	// 使用 switch 语句处理不同的选择
	switch input {
	case "1":
		// 运行基础语法模块
		fmt.Println("\n>>> 正在运行：基础语法演示")
		basics.RunBasicsDemo()

	case "2":
		// 运行面向对象模块
		fmt.Println("\n>>> 正在运行：面向对象演示")
		oop.RunOopDemo()

	case "3":
		// 运行并发编程模块
		fmt.Println("\n>>> 正在运行：并发编程演示")
		concurrency.RunConcurrencyDemo()

	case "4":
		// 运行高级特性模块
		fmt.Println("\n>>> 正在运行：高级特性演示")
		advanced.RunAdvancedDemo()

	case "5":
		// 运行数据结构模块
		fmt.Println("\n>>> 正在运行：数据结构演示")
		structures.RunStructuresDemo()

	case "6":
		// 运行设计模式模块
		fmt.Println("\n>>> 正在运行：设计模式演示")
		patterns.RunPatternsDemo()

	case "7":
		// 运行所有模块
		fmt.Println("\n>>> 正在运行：所有模块演示")
		runAllDemos()

	default:
		// 尝试将输入解析为数字，判断是否是无效选择
		_, err := strconv.Atoi(input)
		if err != nil {
			// 不是数字，提示无效输入
			fmt.Printf("\n无效输入: %q，请输入菜单数字或 q 退出\n", input)
		} else {
			// 是数字，但不在菜单范围内
			fmt.Printf("\n无效选项: %s，请输入 1-7 或 q\n", input)
		}
	}
}

// ============================================================
// runAllDemos 依次运行所有模块的演示
// ============================================================
func runAllDemos() {
	fmt.Println("\n===== 开始运行所有模块 =====")

	fmt.Println("\n[1/6] 基础语法模块...")
	basics.RunBasicsDemo()

	fmt.Println("\n[2/6] 面向对象模块...")
	oop.RunOopDemo()

	fmt.Println("\n[3/6] 并发编程模块...")
	concurrency.RunConcurrencyDemo()

	fmt.Println("\n[4/6] 高级特性模块...")
	advanced.RunAdvancedDemo()

	fmt.Println("\n[5/6] 数据结构模块...")
	structures.RunStructuresDemo()

	fmt.Println("\n[6/6] 设计模式模块...")
	patterns.RunPatternsDemo()

	fmt.Println("\n===== 所有模块运行完毕 =====")
}
