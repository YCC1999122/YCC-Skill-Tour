// ============================================================
// 包声明：每个Go文件都必须以包声明开头
// package basics 表示当前文件属于 basics 包
// 包名通常是小写的，与目录名一致
// 同一个目录下的所有文件必须属于同一个包
// ============================================================
package basics

// ============================================================
// 导入语句：import 用于导入其他包
// 导入的包可以是标准库包，也可以是第三方包或本地包
// fmt 是标准库中的格式化输入输出包
// ============================================================
import (
	"fmt"  // fmt 包提供了格式化输入输出函数，类似于 C 的 printf/scanf
	"math" // math 包提供了数学计算相关的函数和常量
)

// ============================================================
// RunBasicsDemo 是基础语法模块的入口函数
// 首字母大写表示该函数是导出的（public），可以被其他包调用
// 首字母小写表示该函数是未导出的（private），只能在当前包内使用
// 这是 Go 语言中访问控制的唯一机制，没有 public/private 关键字
// ============================================================
func RunBasicsDemo() {
	// 打印分隔线，用于区分不同的演示部分
	fmt.Println("\n========== 基础语法演示 ==========")

	// 依次调用各个演示函数
	demonstrateDataTypes()   // 演示基本数据类型
	demonstrateVariables()   // 演示变量声明
	demonstrateOperators()   // 演示运算符
	demonstrateControlFlow() // 演示控制流
	demonstrateFunctions()   // 演示函数
	demonstrateArrays()      // 演示数组
	demonstrateSlices()      // 演示切片
	demonstrateMaps()        // 演示 map
	demonstrateRange()       // 演示 range 遍历
	demonstratePointers()    // 演示指针
}

// ============================================================
// demonstrateDataTypes 演示 Go 的基本数据类型
// Go 是静态类型语言，变量在编译时就确定了类型
// ============================================================
func demonstrateDataTypes() {
	fmt.Println("\n--- 1. 基本数据类型 ---")

	// ---------- 整数类型 ----------
	// int 是有符号整数，其大小取决于操作系统（32位系统上是32位，64位系统上是64位）
	var age int = 25
	fmt.Printf("int 类型: age = %d, 类型: %T\n", age, age)

	// int8: 8位有符号整数，范围: -128 ~ 127
	var smallInt int8 = 127
	fmt.Printf("int8 类型: smallInt = %d\n", smallInt)

	// int16: 16位有符号整数，范围: -32768 ~ 32767
	var mediumInt int16 = 32767
	fmt.Printf("int16 类型: mediumInt = %d\n", mediumInt)

	// int32: 32位有符号整数，范围: -2147483648 ~ 2147483647
	var bigInt int32 = 2147483647
	fmt.Printf("int32 类型: bigInt = %d\n", bigInt)

	// int64: 64位有符号整数，范围非常大
	var hugeInt int64 = 9223372036854775807
	fmt.Printf("int64 类型: hugeInt = %d\n", hugeInt)

	// uint 是无符号整数，同样取决于系统位数
	var positiveNum uint = 100
	fmt.Printf("uint 类型: positiveNum = %d\n", positiveNum)

	// uint8/uint16/uint32/uint64: 各种位数的无符号整数
	var byteVal byte = 255 // byte 是 uint8 的别名，常用于表示字节数据
	fmt.Printf("byte 类型 (uint8别名): byteVal = %d\n", byteVal)

	// ---------- 浮点数类型 ----------
	// float32: 32位浮点数，精度约6-7位小数
	var pi32 float32 = 3.1415926
	fmt.Printf("float32 类型: pi32 = %f\n", pi32)

	// float64: 64位浮点数，精度约15-16位小数（推荐使用）
	var pi64 float64 = math.Pi
	fmt.Printf("float64 类型: pi64 = %.10f\n", pi64)

	// ---------- 布尔类型 ----------
	// bool 类型只有两个值：true 和 false
	// Go 中布尔类型不能与整数类型相互转换（这是与 C 语言的重要区别）
	var isGoFun bool = true
	var isHard bool = false
	fmt.Printf("bool 类型: isGoFun = %t, isHard = %t\n", isGoFun, isHard)

	// ---------- 字符串类型 ----------
	// string 是不可变的字节序列，内部使用 UTF-8 编码
	// 字符串一旦创建就不能修改，修改字符串会创建新的字符串
	var greeting string = "Hello, Go!"
	fmt.Printf("string 类型: greeting = %s\n", greeting)

	// 反引号定义的原始字符串，支持多行，不解析转义字符
	var rawString string = `这是原始字符串
可以包含换行符
\t不会被解析为制表符`
	fmt.Printf("原始字符串:\n%s\n", rawString)

	// ---------- rune 类型 ----------
	// rune 是 int32 的别名，用于表示一个 Unicode 码点（字符）
	// 在 Go 中，字符串是字节数组，而不是字符数组
	// 处理中文等多字节字符时，需要使用 rune
	var ch rune = 'A'       // 单引号表示字符常量，类型是 rune
	var chineseChar rune = '中'
	fmt.Printf("rune 类型: ch = %c, Unicode码点: %U\n", ch, ch)
	fmt.Printf("中文字符: chineseChar = %c, Unicode码点: %U\n", chineseChar, chineseChar)

	// ---------- 复数类型 ----------
	// complex64: 实部和虚部都是 float32
	// complex128: 实部和虚部都是 float64
	var c complex128 = complex(3, 4) // 使用 complex 函数创建复数
	fmt.Printf("complex 类型: c = %v, 实部: %f, 虚部: %f\n", c, real(c), imag(c))
}

// ============================================================
// demonstrateVariables 演示变量声明的各种方式
// Go 提供了多种变量声明方式，适用于不同场景
// ============================================================
func demonstrateVariables() {
	fmt.Println("\n--- 2. 变量声明 ---")

	// ---------- var 声明 ----------
	// var 变量名 类型 = 初始值
	// 这是最完整的声明方式，适用于包级变量和需要明确类型的场景
	var a int = 10
	fmt.Printf("var 声明: a = %d\n", a)

	// 类型推导：如果提供了初始值，Go 可以自动推导出变量类型
	var b = 20 // 自动推导为 int 类型
	fmt.Printf("类型推导: b = %d, 类型: %T\n", b, b)

	// 只声明不初始化：变量会被赋予"零值"
	// 数值类型零值是 0，布尔类型是 false，字符串是空串 ""，引用类型是 nil
	var c int
	var d string
	var e bool
	fmt.Printf("零值: c(int)=%d, d(string)=%q, e(bool)=%t\n", c, d, e)

	// 同时声明多个变量
	var f, g, h int = 1, 2, 3
	fmt.Printf("多变量声明: f=%d, g=%d, h=%d\n", f, g, h)

	// var 块声明：用于声明多个变量，更整洁
	var (
		name    string = "张三"
		age     int    = 18
		isAdult bool   = true
	)
	fmt.Printf("var块: name=%s, age=%d, isAdult=%t\n", name, age, isAdult)

	// ---------- 短变量声明 := ----------
	// 短变量声明操作符 := 只能在函数内部使用
	// 它会自动推导类型，并且要求左边至少有一个新变量
	// 这是 Go 中最常用的变量声明方式
	i := 100
	fmt.Printf("短变量声明: i = %d, 类型: %T\n", i, i)

	// 短变量声明可以同时声明多个变量
	j, k := "hello", 3.14
	fmt.Printf("多变量短声明: j=%s, k=%f\n", j, k)

	// 短变量声明中可以有已经声明过的变量，但至少要有一个新变量
	// 这在函数返回多值时特别有用（比如 err 变量可以复用）
	j, l := "world", 200 // j 已经存在，会被重新赋值；l 是新变量
	fmt.Printf("重新赋值: j=%s, l=%d\n", j, l)

	// ---------- const 常量 ----------
	// const 声明常量，常量的值在编译时就确定了，运行时不能修改
	const Pi = 3.1415926
	const World = "世界"
	fmt.Printf("常量: Pi=%f, World=%s\n", Pi, World)

	// 常量也可以用块声明
	const (
		Monday    = 1
		Tuesday   = 2
		Wednesday = 3
	)
	fmt.Printf("常量块: Monday=%d, Tuesday=%d\n", Monday, Tuesday)

	// ---------- iota 枚举 ----------
	// iota 是一个特殊的常量生成器，用于生成自增的枚举值
	// iota 在每个 const 块中从 0 开始，每行递增 1
	// iota 常用于定义枚举类型
	const (
		Sunday    = iota // 0: iota 初始值为 0
		Monday2          // 1: 省略赋值，自动继承上一行的表达式（即 iota）
		Tuesday2         // 2
		Wednesday2       // 3
		Thursday2        // 4
		Friday2          // 5
		Saturday2        // 6
	)
	fmt.Printf("iota枚举: Sunday=%d, Monday=%d, Friday=%d\n", Sunday, Monday2, Friday2)

	// iota 的高级用法：可以参与表达式运算
	const (
		_   = iota             // 跳过 0（使用空白标识符 _）
		KB  = 1 << (10 * iota) // 1 << 10 = 1024
		MB                     // 1 << 20 = 1048576
		GB                     // 1 << 30
		TB                     // 1 << 40
	)
	fmt.Printf("存储单位: KB=%d, MB=%d, GB=%d\n", KB, MB, GB)
}

// ============================================================
// demonstrateOperators 演示 Go 的运算符
// ============================================================
func demonstrateOperators() {
	fmt.Println("\n--- 3. 运算符 ---")

	// ---------- 算术运算符 ----------
	a, b := 10, 3
	fmt.Printf("算术运算: %d+%d=%d, %d-%d=%d, %d*%d=%d\n", a, b, a+b, a, b, a-b, a, b, a*b)
	fmt.Printf("算术运算: %d/%d=%d (整数除法), %d%%%d=%d (取余)\n", a, b, a/b, a, b, a%b)

	// 自增自减运算符
	// 注意：Go 中只有 i++ 和 i--，没有 ++i 和 --i
	// 而且 ++/-- 是语句，不是表达式，不能用于赋值或其他表达式中
	c := 5
	c++ // 等价于 c = c + 1
	fmt.Printf("自增: c = %d\n", c)
	c-- // 等价于 c = c - 1
	fmt.Printf("自减: c = %d\n", c)

	// ---------- 比较运算符 ----------
	// 比较运算符的结果是 bool 类型
	x, y := 5, 10
	fmt.Printf("比较: %d==%d=%t, %d!=%d=%t\n", x, y, x == y, x, y, x != y)
	fmt.Printf("比较: %d<%d=%t, %d>=%d=%t\n", x, y, x < y, x, y, x >= y)

	// ---------- 逻辑运算符 ----------
	// &&: 逻辑与（短路与：左边为 false 时不计算右边）
	// ||: 逻辑或（短路或：左边为 true 时不计算右边）
	// !:  逻辑非
	p, q := true, false
	fmt.Printf("逻辑: p&&q=%t, p||q=%t, !p=%t\n", p && q, p || q, !p)

	// ---------- 位运算符 ----------
	m, n := 6, 3 // 6 = 0110, 3 = 0011
	fmt.Printf("位运算: %d&%d=%d (按位与), %d|%d=%d (按位或)\n", m, n, m&n, m, n, m|n)
	fmt.Printf("位运算: %d^%d=%d (按位异或), %d<<1=%d (左移), %d>>1=%d (右移)\n", m, n, m^n, m, m<<1, m, m>>1)

	// ---------- 赋值运算符 ----------
	d := 10
	d += 5 // 等价于 d = d + 5
	fmt.Printf("赋值: d += 5 后, d = %d\n", d)
	d *= 2 // 等价于 d = d * 2
	fmt.Printf("赋值: d *= 2 后, d = %d\n", d)
	// 还有 -=, /=, %=, &=, |=, ^=, <<=, >>= 等
}

// ============================================================
// demonstrateControlFlow 演示控制流语句
// 包括 if、switch、for、select 等
// ============================================================
func demonstrateControlFlow() {
	fmt.Println("\n--- 4. 控制流 ---")

	// ---------- if 语句 ----------
	// Go 的 if 语句不需要括号（加了也可以，但不推荐）
	// 但大括号是必须的，即使只有一行语句
	age := 20
	if age >= 18 {
		fmt.Println("if: 你是成年人")
	}

	// if-else 语句
	if age >= 60 {
		fmt.Println("if: 老年人")
	} else if age >= 18 {
		fmt.Println("if: 成年人")
	} else {
		fmt.Println("if: 未成年人")
	}

	// if 的特殊语法：可以在条件判断前加一个简短语句
	// 这个语句中声明的变量只在 if-else 块中有效
	// 常用于错误处理：if err := someFunc(); err != nil { ... }
	if num := 15; num%2 == 0 {
		fmt.Println("if-简短语句: num是偶数")
	} else {
		fmt.Println("if-简短语句: num是奇数")
	}
	// 注意：num 在这里不可访问，因为它的作用域只在 if 块内

	// ---------- switch 语句 ----------
	// Go 的 switch 比其他语言更灵活：
	// 1. case 后面不需要 break（默认就有 break 行为）
	// 2. case 可以是任意类型，不一定是常量或整数
	// 3. 多个 case 值可以用逗号分隔
	// 4. switch 后面可以不带表达式（相当于 switch true）
	day := "星期一"
	switch day {
	case "星期一", "星期二", "星期三", "星期四", "星期五":
		fmt.Println("switch: 工作日")
	case "星期六", "星期日":
		fmt.Println("switch: 周末")
	default:
		fmt.Println("switch: 无效日期")
	}

	// switch 带简短语句（类似于 if）
	switch num := 7; { // 省略表达式，相当于 switch true
	case num < 0:
		fmt.Println("switch: 负数")
	case num == 0:
		fmt.Println("switch: 零")
	case num > 0 && num < 10:
		fmt.Println("switch: 个位数正数")
	default:
		fmt.Println("switch: 多位数正数")
	}

	// fallthrough 关键字：强制执行下一个 case（不判断条件）
	// 这是为了兼容 C 语言的 switch 行为，Go 中不常用
	switch num := 2; num {
	case 1:
		fmt.Println("fallthrough: 1")
		fallthrough // 继续执行下一个 case
	case 2:
		fmt.Println("fallthrough: 2")
		fallthrough
	case 3:
		fmt.Println("fallthrough: 3 (通过 fallthrough 到达)")
	default:
		fmt.Println("fallthrough: other")
	}

	// ---------- for 循环 ----------
	// Go 只有 for 循环一种循环结构，但有多种用法

	// 用法1：标准 for 循环（初始化; 条件; 后置）
	fmt.Print("for标准循环: ")
	for i := 0; i < 5; i++ {
		fmt.Printf("%d ", i)
	}
	fmt.Println()

	// 用法2：只有条件的 for 循环（相当于 while 循环）
	fmt.Print("for当while: ")
	j := 0
	for j < 3 {
		fmt.Printf("%d ", j)
		j++
	}
	fmt.Println()

	// 用法3：无限循环（省略初始化、条件、后置）
	// 需要配合 break 使用
	fmt.Print("for无限循环+break: ")
	k := 0
	for {
		if k >= 4 {
			break // break 跳出当前循环
		}
		fmt.Printf("%d ", k)
		k++
	}
	fmt.Println()

	// 用法4：for-range 遍历（在后面 demonstrateRange 中详细演示）

	// continue 关键字：跳过当前迭代，继续下一次
	fmt.Print("continue示例(只打印偶数): ")
	for i := 0; i < 10; i++ {
		if i%2 != 0 {
			continue // 跳过奇数
		}
		fmt.Printf("%d ", i)
	}
	fmt.Println()

	// 标签和 goto（了解即可，实际开发中不推荐使用 goto）
	// 标签可以用于 break 和 continue，跳出多层循环
	fmt.Print("多层循环break标签: ")
outerLoop: // 定义一个标签
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			if i == 1 && j == 1 {
				break outerLoop // 跳出到 outerLoop 标签处（跳出外层循环）
			}
			fmt.Printf("(%d,%d) ", i, j)
		}
	}
	fmt.Println()

	// ---------- select 语句 ----------
	// select 用于在多个 channel 操作中进行选择
	// 类似于 switch，但每个 case 都是 channel 操作
	// 这里只做简单介绍，详细内容在 concurrency 包中演示
	fmt.Println("select: 用于多路 channel 复用，详见并发模块")
}

// ============================================================
// demonstrateFunctions 演示函数的各种用法
// Go 的函数是一等公民，可以作为参数、返回值，也可以赋值给变量
// ============================================================
func demonstrateFunctions() {
	fmt.Println("\n--- 5. 函数 ---")

	// ---------- 基本函数调用 ----------
	result := add(3, 5)
	fmt.Printf("基本函数: add(3,5) = %d\n", result)

	// ---------- 多返回值 ----------
	// Go 函数可以返回多个值，这是 Go 的一个重要特性
	// 常用于返回 (结果, 错误) 对
	sum, diff := calculate(10, 3)
	fmt.Printf("多返回值: sum=%d, diff=%d\n", sum, diff)

	// 使用空白标识符 _ 忽略不需要的返回值
	sum2, _ := calculate(20, 5) // 忽略第二个返回值
	fmt.Printf("忽略返回值: sum2=%d\n", sum2)

	// ---------- 命名返回值 ----------
	// 函数可以给返回值命名，相当于在函数开头声明了返回变量
	// return 语句可以不带参数，直接返回当前返回变量的值（裸返回）
	// 注意：裸返回在短函数中很方便，但长函数中会影响可读性
	area, perimeter := rectProps(5, 3)
	fmt.Printf("命名返回值: area=%d, perimeter=%d\n", area, perimeter)

	// ---------- 函数作为变量 ----------
	// 函数是一等公民，可以赋值给变量
	addFunc := add // 函数变量，类型为 func(int, int) int
	fmt.Printf("函数变量: addFunc(4,6) = %d\n", addFunc(4, 6))

	// ---------- 函数作为参数 ----------
	// 可以将函数作为参数传递给另一个函数
	// 这是函数式编程的基础
	nums := []int{1, 2, 3, 4, 5}
	sumResult := compute(nums, add) // 将 add 函数传入
	fmt.Printf("函数作为参数: compute求和 = %d\n", sumResult)

	// ---------- 函数作为返回值 ----------
	// 函数可以返回另一个函数
	// 常用于创建闭包、工厂函数等
	adder := makeAdder(10)      // 返回一个函数，这个函数会给参数加 10
	fmt.Printf("函数作为返回值: adder(5) = %d\n", adder(5))     // 10 + 5 = 15
	fmt.Printf("函数作为返回值: adder(20) = %d\n", adder(20))   // 10 + 20 = 30

	// ---------- 匿名函数 ----------
	// 没有名字的函数，可以直接定义并调用
	greet := func(name string) string {
		return "你好, " + name
	}
	fmt.Printf("匿名函数: %s\n", greet("小明"))

	// 立即执行的匿名函数（IIFE）
	func(msg string) {
		fmt.Printf("立即执行函数: %s\n", msg)
	}("Hello Go!") // 直接在定义后调用

	// ---------- 可变参数 ----------
	// 函数可以接受可变数量的参数（使用 ... 语法）
	// 可变参数在函数内部表现为一个切片
	total := sumAll(1, 2, 3, 4, 5)
	fmt.Printf("可变参数: sumAll(1,2,3,4,5) = %d\n", total)

	// 可以将切片展开作为可变参数传入
	numbers := []int{10, 20, 30}
	total2 := sumAll(numbers...) // 使用 ... 将切片展开
	fmt.Printf("切片展开: sumAll(numbers...) = %d\n", total2)

	// ---------- 闭包 ----------
	// 闭包是引用了自由变量的函数，自由变量和函数一同存在
	// 即使离开了自由变量的声明环境，闭包仍然可以访问和修改这些变量
	counter := newCounter() // 返回一个闭包，内部维护一个计数器
	fmt.Printf("闭包计数器: %d\n", counter()) // 1
	fmt.Printf("闭包计数器: %d\n", counter()) // 2
	fmt.Printf("闭包计数器: %d\n", counter()) // 3
	// 每个闭包都有自己独立的环境变量
	counter2 := newCounter()
	fmt.Printf("新闭包计数器: %d\n", counter2()) // 1（独立的计数器）
}

// ---------- 函数定义示例 ----------

// add 是一个简单的加法函数
// 参数类型写在参数名后面，这是 Go 的语法特点
// 返回值类型写在参数列表后面
func add(a int, b int) int {
	return a + b
}

// calculate 返回两个数的和与差
// 多返回值：返回值类型列表用括号括起来
func calculate(a int, b int) (int, int) {
	sum := a + b
	diff := a - b
	return sum, diff
}

// rectProps 计算矩形的面积和周长（命名返回值）
// 返回值有名字（area 和 perimeter），相当于在函数内部声明了这些变量
// 裸返回（return 不带参数）会自动返回这些变量的当前值
func rectProps(length, width int) (area, perimeter int) {
	// 直接赋值给返回变量
	area = length * width
	perimeter = 2 * (length + width)
	// 裸返回：等价于 return area, perimeter
	return
}

// compute 接受一个整数切片和一个函数，对切片中的元素应用该函数
// fn 的类型是 func(int, int) int，表示接受两个 int 参数，返回一个 int
func compute(nums []int, fn func(int, int) int) int {
	result := 0
	for _, num := range nums {
		result = fn(result, num)
	}
	return result
}

// makeAdder 返回一个加法器函数
// 返回的函数会"记住"创建它时的 base 参数（闭包特性）
func makeAdder(base int) func(int) int {
	return func(x int) int {
		return base + x
	}
}

// sumAll 计算所有传入参数的和
// ...int 表示可变参数，可以传入任意数量的 int
func sumAll(nums ...int) int {
	total := 0
	for _, num := range nums {
		total += num
	}
	return total
}

// newCounter 创建一个计数器闭包
// 返回的函数每次调用都会使计数器加 1 并返回当前值
func newCounter() func() int {
	count := 0 // 这个变量会被闭包捕获，生命周期延长
	return func() int {
		count++
		return count
	}
}

// ============================================================
// demonstrateArrays 演示数组
// 数组是固定长度的同类型元素序列
// 数组的长度是数组类型的一部分（例如 [3]int 和 [5]int 是不同的类型）
// ============================================================
func demonstrateArrays() {
	fmt.Println("\n--- 6. 数组 ---")

	// ---------- 数组声明 ----------
	// var 数组名 [长度]类型
	var arr1 [5]int // 长度为 5 的 int 数组，元素默认零值
	fmt.Printf("数组声明: arr1 = %v, 长度: %d\n", arr1, len(arr1))

	// 使用数组字面量初始化
	arr2 := [3]int{10, 20, 30}
	fmt.Printf("数组初始化: arr2 = %v\n", arr2)

	// 使用 ... 让编译器自动计算数组长度
	arr3 := [...]string{"Go", "Java", "Python", "C++"}
	fmt.Printf("自动长度: arr3 = %v, 长度: %d\n", arr3, len(arr3))

	// 指定索引位置初始化（未指定的位置为零值）
	arr4 := [5]int{0: 100, 2: 200, 4: 300}
	fmt.Printf("指定索引: arr4 = %v\n", arr4)

	// ---------- 数组访问 ----------
	// 使用下标访问数组元素，下标从 0 开始
	fmt.Printf("数组访问: arr2[0] = %d, arr2[2] = %d\n", arr2[0], arr2[2])

	// 修改数组元素
	arr2[1] = 200
	fmt.Printf("修改后: arr2 = %v\n", arr2)

	// ---------- 数组遍历 ----------
	fmt.Print("for遍历数组: ")
	for i := 0; i < len(arr2); i++ {
		fmt.Printf("%d ", arr2[i])
	}
	fmt.Println()

	// 使用 for-range 遍历数组
	fmt.Print("range遍历数组: ")
	for index, value := range arr3 {
		fmt.Printf("[%d]=%s ", index, value)
	}
	fmt.Println()

	// ---------- 数组是值类型 ----------
	// Go 中数组是值类型，赋值和传参都会复制整个数组
	// 这与 C 语言中数组退化为指针不同
	arr5 := [3]int{1, 2, 3}
	arr6 := arr5 // 完整复制数组
	arr6[0] = 100
	fmt.Printf("值类型: arr5 = %v (不变), arr6 = %v (已修改)\n", arr5, arr6)

	// 数组长度是类型的一部分，[3]int 和 [4]int 是不同类型
	// 所以不能相互赋值
}

// ============================================================
// demonstrateSlices 演示切片
// 切片是 Go 中非常重要的数据结构，可以看作是"动态数组"
// 切片底层是数组，但切片本身是引用类型（包含指针、长度、容量）
// 切片的零值是 nil
// ============================================================
func demonstrateSlices() {
	fmt.Println("\n--- 7. 切片 ---")

	// ---------- 切片的底层原理 ----------
	// 切片在内部由三部分组成：
	// 1. 指针（pointer）：指向底层数组的起始位置
	// 2. 长度（length）：切片中元素的数量
	// 3. 容量（capacity）：从切片起始位置到底层数组末尾的元素数量
	//
	// 切片结构示意：
	//   slice := make([]int, 3, 5)
	//   ┌─────────────┐
	//   │  pointer    │───→ 底层数组: [0, 0, 0, _, _]
	//   │  length = 3 │           ↑___________↑
	//   │  capacity = 5│               容量=5
	//   └─────────────┘
	//
	// 长度是当前有多少元素，容量是最多能容纳多少元素
	// 当 append 超过容量时，会分配新的更大的底层数组（通常扩容为原来的2倍）

	// ---------- 切片声明 ----------
	// 方式1：声明一个 nil 切片（零值）
	var s1 []int
	fmt.Printf("nil切片: s1 = %v, len=%d, cap=%d, isNil=%t\n", s1, len(s1), cap(s1), s1 == nil)

	// 方式2：使用 make 创建切片
	// make([]类型, 长度, 容量)
	// 如果省略容量，则容量等于长度
	s2 := make([]int, 3)    // 长度为3，容量为3
	fmt.Printf("make切片: s2 = %v, len=%d, cap=%d\n", s2, len(s2), cap(s2))

	s3 := make([]int, 3, 5) // 长度为3，容量为5
	fmt.Printf("make切片带容量: s3 = %v, len=%d, cap=%d\n", s3, len(s3), cap(s3))

	// 方式3：使用切片字面量
	s4 := []int{1, 2, 3, 4, 5}
	fmt.Printf("字面量切片: s4 = %v, len=%d, cap=%d\n", s4, len(s4), cap(s4))

	// ---------- 切片操作（基于数组或切片创建新切片） ----------
	// 切片语法：s[low:high]，包含 s[low]，不包含 s[high]
	// 新切片的长度 = high - low
	// 新切片的容量 = 原数组/切片容量 - low
	arr := [10]int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	s5 := arr[2:5] // 从索引2到4（不包含5）
	fmt.Printf("数组切片: s5 = %v, len=%d, cap=%d\n", s5, len(s5), cap(s5))

	// 省略 low 表示从 0 开始
	s6 := arr[:5] // 等价于 arr[0:5]
	fmt.Printf("省略low: s6 = %v\n", s6)

	// 省略 high 表示到末尾
	s7 := arr[5:] // 等价于 arr[5:len(arr)]
	fmt.Printf("省略high: s7 = %v\n", s7)

	// 都省略表示整个数组（将数组转为切片）
	s8 := arr[:]
	fmt.Printf("全部省略: s8 = %v\n", s8)

	// 完整切片表达式：s[low:high:max]
	// max 限制新切片的容量（容量 = max - low）
	// 这可以防止新切片的 append 操作意外修改原数组的后续元素
	s9 := arr[2:5:7] // 长度=3, 容量=5
	fmt.Printf("完整切片: s9 = %v, len=%d, cap=%d\n", s9, len(s9), cap(s9))

	// ---------- append 函数 ----------
	// append 用于向切片末尾添加元素
	// 如果容量足够，直接在底层数组上添加，返回原切片（长度增加）
	// 如果容量不足，会分配新的底层数组，返回新切片
	// 重要：必须接收 append 的返回值！通常写成 s = append(s, elem)
	s10 := make([]int, 0, 3)
	fmt.Printf("append前: s10 = %v, len=%d, cap=%d\n", s10, len(s10), cap(s10))

	s10 = append(s10, 1)
	s10 = append(s10, 2, 3) // 一次添加多个元素
	fmt.Printf("append后: s10 = %v, len=%d, cap=%d\n", s10, len(s10), cap(s10))

	// 容量不足时自动扩容
	s10 = append(s10, 4) // 超过容量，触发扩容
	fmt.Printf("扩容后: s10 = %v, len=%d, cap=%d (容量翻倍)\n", s10, len(s10), cap(s10))

	// 追加另一个切片（使用 ... 展开）
	s11 := []int{5, 6, 7}
	s10 = append(s10, s11...)
	fmt.Printf("追加切片: s10 = %v\n", s10)

	// ---------- copy 函数 ----------
	// copy(dst, src) 将 src 中的元素复制到 dst
	// 返回复制的元素个数（等于两个切片中较小的长度）
	src := []int{10, 20, 30, 40, 50}
	dst := make([]int, 3)
	n := copy(dst, src)
	fmt.Printf("copy: dst = %v, 复制了 %d 个元素\n", dst, n)

	// 切片间复制，源和目标可以重叠（类似 memmove）
	s12 := []int{1, 2, 3, 4, 5}
	copy(s12[2:], s12[:3]) // 从位置2开始，复制前3个元素
	fmt.Printf("重叠copy: s12 = %v\n", s12)

	// ---------- 切片的引用特性 ----------
	// 切片是引用类型，多个切片可以共享同一个底层数组
	// 修改其中一个切片的元素，会影响到共享底层数组的其他切片
	original := []int{1, 2, 3, 4, 5}
	sliceRef := original[1:4] // 共享底层数组
	sliceRef[0] = 99          // 修改切片的第一个元素，即 original[1]
	fmt.Printf("引用特性: original = %v (被修改了!)\n", original)

	// ---------- 常用操作 ----------
	// 在切片中间插入元素
	s13 := []int{1, 2, 4, 5}
	// 在索引2的位置插入3
	// 方法：先 append 一个零值扩展空间，然后 copy 后移元素，最后赋值
	s13 = append(s13, 0)       // 扩展一个位置
	copy(s13[3:], s13[2:])     // 从索引2开始的元素后移一位
	s13[2] = 3                 // 插入新元素
	fmt.Printf("插入元素: s13 = %v\n", s13)

	// 删除切片中的元素
	s14 := []int{1, 2, 3, 4, 5}
	// 删除索引2的元素（即3）
	s14 = append(s14[:2], s14[3:]...)
	fmt.Printf("删除元素: s14 = %v\n", s14)
}

// ============================================================
// demonstrateMaps 演示 map（映射/字典）
// map 是键值对的无序集合，类似于其他语言的 hash map / dictionary
// map 的零值是 nil，不能直接使用，需要用 make 创建
// ============================================================
func demonstrateMaps() {
	fmt.Println("\n--- 8. Map ---")

	// ---------- map 声明 ----------
	// map[键类型]值类型
	// 键类型必须是可比较的（可以用 == 比较），比如 int、string 等
	// 切片、map、函数等不能作为 map 的键

	// 方式1：声明 nil map（零值，不能直接使用）
	var m1 map[string]int
	fmt.Printf("nil map: m1 = %v, isNil=%t\n", m1, m1 == nil)

	// 方式2：使用 make 创建空 map
	m2 := make(map[string]int)
	fmt.Printf("make map: m2 = %v, len=%d\n", m2, len(m2))

	// 方式3：使用 map 字面量初始化
	m3 := map[string]int{
		"苹果": 5,
		"香蕉": 3,
		"橙子": 8,
	}
	fmt.Printf("字面量map: m3 = %v, len=%d\n", m3, len(m3))

	// ---------- 增删改查 ----------
	fruits := make(map[string]int)

	// 添加：直接赋值
	fruits["苹果"] = 10
	fruits["香蕉"] = 6
	fruits["葡萄"] = 15
	fmt.Printf("添加后: fruits = %v\n", fruits)

	// 修改：直接赋值（键已存在则修改）
	fruits["苹果"] = 12
	fmt.Printf("修改后: fruits = %v\n", fruits)

	// 查询：使用 map[key] 语法
	appleCount := fruits["苹果"]
	fmt.Printf("查询: 苹果 = %d\n", appleCount)

	// 查询不存在的键：返回值类型的零值（不会报错）
	orangeCount := fruits["橙子"] // 橙子不存在，返回 0
	fmt.Printf("查询不存在的键: 橙子 = %d (返回零值)\n", orangeCount)

	// ---------- ok-idiom（逗号ok模式） ----------
	// 如何区分"键不存在"和"键存在但值为零值"？
	// 使用双返回值语法：value, ok := map[key]
	// ok 是 bool 类型，表示键是否存在
	count, ok := fruits["橙子"]
	if ok {
		fmt.Printf("ok-idiom: 橙子存在，数量 = %d\n", count)
	} else {
		fmt.Printf("ok-idiom: 橙子不存在 (count=%d 是零值)\n", count)
	}

	// 也可以写成一行
	if count, ok := fruits["苹果"]; ok {
		fmt.Printf("ok-idiom简写: 苹果存在，数量 = %d\n", count)
	}

	// ---------- 删除 ----------
	// 使用 delete 函数删除键值对
	delete(fruits, "香蕉")
	fmt.Printf("删除香蕉后: fruits = %v\n", fruits)

	// 删除不存在的键不会报错（什么也不做）
	delete(fruits, "西瓜") // 不会 panic

	// ---------- 遍历 map ----------
	// 使用 for-range 遍历 map
	// 注意：map 的遍历顺序是不确定的（Go 故意这样设计，防止依赖顺序）
	fmt.Print("遍历map: ")
	for key, value := range fruits {
		fmt.Printf("%s=%d ", key, value)
	}
	fmt.Println()

	// 只遍历键
	fmt.Print("只遍历键: ")
	for key := range fruits {
		fmt.Printf("%s ", key)
	}
	fmt.Println()

	// 只遍历值（使用空白标识符忽略键）
	fmt.Print("只遍历值: ")
	for _, value := range fruits {
		fmt.Printf("%d ", value)
	}
	fmt.Println()

	// ---------- map 是引用类型 ----------
	// map 是引用类型，赋值和传参不会复制整个 map
	// 多个变量可以引用同一个底层 map 数据
	m4 := map[string]int{"a": 1, "b": 2}
	m5 := m4       // m4 和 m5 指向同一个底层 map
	m5["a"] = 100  // 修改 m5 会影响 m4
	fmt.Printf("引用类型: m4 = %v (被修改了!)\n", m4)

	// ---------- map 的注意事项 ----------
	// 1. map 不是并发安全的，并发读写需要加锁（详见并发模块）
	// 2. 不能获取 map 元素的地址（&map[key] 是非法的），因为 map 扩容时元素会移动
	// 3. map 的长度用 len() 获取，没有 cap()
}

// ============================================================
// demonstrateRange 演示 range 遍历
// range 可以遍历数组、切片、map、字符串、channel 等
// ============================================================
func demonstrateRange() {
	fmt.Println("\n--- 9. Range 遍历 ---")

	// ---------- 遍历数组/切片 ----------
	nums := []int{10, 20, 30, 40, 50}
	fmt.Print("range遍历切片: ")
	for index, value := range nums {
		fmt.Printf("[%d]=%d ", index, value)
	}
	fmt.Println()

	// 只需要值，不需要索引（使用空白标识符 _）
	fmt.Print("只遍历值: ")
	for _, value := range nums {
		fmt.Printf("%d ", value)
	}
	fmt.Println()

	// 只需要索引，不需要值（省略第二个返回值）
	fmt.Print("只遍历索引: ")
	for index := range nums {
		fmt.Printf("%d ", index)
	}
	fmt.Println()

	// ---------- 遍历字符串 ----------
	// range 遍历字符串时，按 Unicode 字符（rune）遍历
	// 对于 ASCII 字符，索引每次加 1
	// 对于中文等多字节字符，索引会跳过多字节
	str := "Go语言"
	fmt.Print("range遍历字符串: ")
	for index, char := range str {
		fmt.Printf("[%d]=%c ", index, char)
	}
	fmt.Println()
	// 注意：中文字符占 3 个字节（UTF-8），所以索引不是连续的

	// ---------- 遍历 map ----------
	// map 的遍历顺序是随机的
	scores := map[string]int{"语文": 90, "数学": 85, "英语": 95}
	fmt.Print("range遍历map: ")
	for key, value := range scores {
		fmt.Printf("%s=%d ", key, value)
	}
	fmt.Println()

	// ---------- range 中的值是拷贝 ----------
	// 重要：range 返回的 value 是元素的拷贝，不是引用
	// 所以修改 value 不会影响原始切片/数组
	people := []struct {
		name string
		age  int
	}{
		{"张三", 20},
		{"李四", 25},
	}

	for _, p := range people {
		p.age += 10 // 修改的是拷贝，不会影响原切片
	}
	fmt.Printf("range值拷贝: people[0].age = %d (未改变)\n", people[0].age)

	// 如果要修改原元素，应该使用索引访问
	for i := range people {
		people[i].age += 10 // 直接修改原元素
	}
	fmt.Printf("索引修改: people[0].age = %d (已改变)\n", people[0].age)
}

// ============================================================
// demonstratePointers 演示指针
// Go 保留了指针，但比 C 语言的指针简单得多
// 主要区别：
// 1. Go 指针不能进行算术运算（不能 p++ 或 p + 1）
// 2. Go 有自动内存管理（垃圾回收），不需要手动释放内存
// 3. Go 中不需要担心悬垂指针（栈逃逸机制会将变量分配到堆上）
// ============================================================
func demonstratePointers() {
	fmt.Println("\n--- 10. 指针 ---")

	// ---------- 基本概念 ----------
	// & 操作符：取地址
	// * 操作符：解引用（通过指针访问变量的值）
	// *类型：指针类型（指向该类型的指针）

	x := 42
	// &x 获取变量 x 的内存地址，类型是 *int（int 类型的指针）
	p := &x
	fmt.Printf("指针基础: x = %d, x的地址 = %p, p = %p\n", x, &x, p)

	// *p 解引用，访问指针指向的值
	fmt.Printf("指针解引用: *p = %d\n", *p)

	// 通过指针修改变量的值
	*p = 100
	fmt.Printf("通过指针修改: x = %d\n", x)

	// ---------- 指针的零值 ----------
	// 指针的零值是 nil（空指针）
	var p2 *int
	fmt.Printf("nil指针: p2 = %v, isNil=%t\n", p2, p2 == nil)

	// 解引用 nil 指针会导致 panic（运行时错误）
	// fmt.Println(*p2) // 这行会 panic

	// ---------- new 函数 ----------
	// new(类型) 分配内存，返回指向该类型零值的指针
	// 相当于创建了一个指向零值的指针
	p3 := new(int) // 分配一个 int 的内存，初始值为 0，返回 *int
	fmt.Printf("new函数: p3 = %p, *p3 = %d\n", p3, *p3)

	*p3 = 50
	fmt.Printf("修改new的值: *p3 = %d\n", *p3)

	// ---------- 指针与函数 ----------
	// 函数参数传递是值传递（拷贝）
	// 如果想在函数内部修改外部变量，需要传递指针
	num := 10
	fmt.Printf("函数传值前: num = %d\n", num)
	incrementByValue(num) // 传值，函数内部修改不影响外部
	fmt.Printf("函数传值后: num = %d (未改变)\n", num)

	incrementByPointer(&num) // 传指针，函数内部可以修改外部变量
	fmt.Printf("函数传指针后: num = %d (已改变)\n", num)

	// ---------- 指针与切片/map ----------
	// 切片和 map 本身就是引用类型，传递时不需要再用指针
	// （它们内部已经包含了指向底层数据的指针）

	// ---------- Go 指针与 C 指针的区别 ----------
	// 1. Go 指针不能进行算术运算（p++ 是非法的）
	// 2. Go 有垃圾回收，不需要 free
	// 3. Go 中返回局部变量的地址是安全的（栈逃逸）
	//    编译器会自动将变量分配到堆上，而不是栈上
	// 4. Go 中有 unsafe.Pointer 可以进行低级操作，但不推荐使用

	// 演示：返回局部变量的地址是安全的
	localPtr := returnLocalPointer()
	fmt.Printf("返回局部变量地址: *localPtr = %d (安全，Go会自动堆分配)\n", *localPtr)
}

// incrementByValue 传值方式，参数是 int 的拷贝
// 函数内部修改的是拷贝，不影响外部变量
func incrementByValue(n int) {
	n++ // 修改的是局部拷贝
}

// incrementByPointer 传指针方式，参数是 *int
// 函数内部通过解引用修改外部变量
func incrementByPointer(n *int) {
	*n++ // 解引用后修改，会影响外部变量
}

// returnLocalPointer 返回局部变量的地址
// 在 C 语言中这是危险的（局部变量在栈上，函数返回后会被销毁）
// 在 Go 中这是安全的，编译器会进行"栈逃逸分析"，将变量分配到堆上
func returnLocalPointer() *int {
	x := 42 // 这个变量会被分配到堆上（因为被返回了地址）
	return &x
}
