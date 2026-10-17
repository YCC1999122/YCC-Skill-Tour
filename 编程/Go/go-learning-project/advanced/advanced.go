// ============================================================
// package advanced 高级特性模块
// 包含 Go 语言的一些高级特性：错误处理、反射、闭包、
// 函数选项模式、泛型、embed 等
// ============================================================
package advanced

import (
	"errors"
	"fmt"
	"reflect"
	"strings"
)

// ============================================================
// RunAdvancedDemo 是高级特性模块的入口函数
// ============================================================
func RunAdvancedDemo() {
	fmt.Println("\n========== 高级特性演示 ==========")

	demonstrateDeferPanicRecover()  // 演示 defer/panic/recover
	demonstrateCustomError()        // 演示自定义错误
	demonstrateReflection()         // 演示反射
	demonstrateClosure()            // 演示闭包
	demonstrateDeferOrder()         // 演示 defer 执行顺序
	demonstrateFunctionalOptions()  // 演示函数选项模式
	demonstrateGenerics()           // 演示泛型
	demonstrateEmbed()              // 演示 embed
}

// ============================================================
// 1. defer / panic / recover 错误处理
// ============================================================
// Go 没有 try-catch-finally 异常机制
// 取而代之的是：
// - error：普通错误，函数返回值传递
// - panic：运行时恐慌（严重错误，程序无法继续运行）
// - recover：从 panic 中恢复（只能在 defer 中使用）
//
// defer：延迟执行，在函数返回前执行
// 常用于资源释放、解锁、关闭文件等

// demonstrateDeferPanicRecover 演示 defer/panic/recover
func demonstrateDeferPanicRecover() {
	fmt.Println("\n--- 1. defer / panic / recover ---")

	// ---------- defer 基本用法 ----------
	fmt.Println("  defer 基本用法：")
	deferDemo()

	// ---------- panic 基本用法 ----------
	fmt.Println("\n  panic 基本用法：")
	panicDemo()

	// ---------- recover 恢复 panic ----------
	fmt.Println("\n  recover 恢复 panic：")
	recoverDemo()

	// ---------- defer + recover 模拟 try-catch ----------
	fmt.Println("\n  模拟 try-catch：")
	safeDivide := func(a, b int) (result int, err error) {
		// 使用 defer + recover 捕获 panic
		defer func() {
			if r := recover(); r != nil {
				// 从 panic 中恢复，将 panic 转为 error
				err = fmt.Errorf("发生恐慌: %v", r)
			}
		}()

		if b == 0 {
			panic("除数为零") // 触发 panic
		}
		result = a / b
		return // 正常返回
	}

	result, err := safeDivide(10, 2)
	if err != nil {
		fmt.Printf("    10/2 错误: %v\n", err)
	} else {
		fmt.Printf("    10/2 = %d\n", result)
	}

	result, err = safeDivide(10, 0)
	if err != nil {
		fmt.Printf("    10/0 错误: %v\n", err)
	} else {
		fmt.Printf("    10/0 = %d\n", result)
	}

	// panic 和 recover 的使用原则：
	// 1. 不要滥用 panic，普通错误应该使用 error 返回
	// 2. panic 只用于真正不可恢复的错误（程序无法继续运行）
	// 3. recover 只能在 defer 函数中使用
	// 4. recover 只能捕获同一个 goroutine 中的 panic
	// 5. recover 捕获后，程序继续执行（但当前函数会返回）
}

// deferDemo 演示 defer 的基本用法
func deferDemo() {
	// defer 语句会将函数延迟到当前函数返回前执行
	// defer 后面必须跟函数调用（不能是语句）
	fmt.Println("    开始执行 deferDemo")

	defer fmt.Println("    第1个 defer: 最后执行（LIFO）")
	defer fmt.Println("    第2个 defer: 中间执行")
	defer fmt.Println("    第3个 defer: 最先执行")

	fmt.Println("    deferDemo 函数体执行完毕")

	// 输出顺序：
	// 开始执行 deferDemo
	// deferDemo 函数体执行完毕
	// 第3个 defer: 最先执行
	// 第2个 defer: 中间执行
	// 第1个 defer: 最后执行（LIFO）
	//
	// 多个 defer 的执行顺序是 LIFO（后进先出）
	// 类似于栈，后声明的先执行
}

// panicDemo 演示 panic 的基本用法
func panicDemo() {
	// 使用 defer + recover 防止 panic 终止整个程序
	defer func() {
		if r := recover(); r != nil {
			fmt.Printf("    捕获到 panic: %v\n", r)
		}
	}()

	fmt.Println("    即将触发 panic...")

	// panic 会立即停止当前函数的执行
	// 然后开始向上逐层执行 defer
	// 如果一直没有 recover，程序会崩溃
	panic("这是一个测试 panic")

	// panic 后面的代码不会执行
	fmt.Println("    这行不会执行")
}

// recoverDemo 演示 recover 的使用
func recoverDemo() {
	// recover 只能在 defer 函数中调用
	// recover 只有在 panic 发生时才会返回非 nil 值
	fmt.Println("    正常情况下，recover 返回 nil")

	defer func() {
		if r := recover(); r != nil {
			fmt.Printf("    recover 捕获: %v\n", r)
		} else {
			fmt.Println("    recover 返回 nil（没有 panic）")
		}
	}()

	fmt.Println("    函数正常执行，没有 panic")
}

// ============================================================
// 2. error 接口与自定义错误
// ============================================================
// Go 的 error 是一个内置接口：
// type error interface {
//     Error() string
// }
//
// 创建错误的方式：
// 1. errors.New("错误信息") - 创建简单错误
// 2. fmt.Errorf("格式化错误: %v", err) - 创建格式化错误
// 3. 自定义错误类型 - 实现 Error() 方法

// ErrNotFound 是一个预定义的错误变量（哨兵错误）
// 通常用于可以直接比较的错误
var ErrNotFound = errors.New("未找到")

// ValidationError 自定义验证错误
// 包含更多错误信息的结构体错误
type ValidationError struct {
	Field   string // 字段名
	Message string // 错误信息
}

// Error 实现 error 接口
func (e *ValidationError) Error() string {
	return fmt.Sprintf("字段 %s 验证失败: %s", e.Field, e.Message)
}

// demonstrateCustomError 演示自定义错误
func demonstrateCustomError() {
	fmt.Println("\n--- 2. 自定义错误 ---")

	// ---------- errors.New ----------
	fmt.Println("  errors.New：")
	err1 := errors.New("这是一个错误")
	fmt.Printf("    错误信息: %v\n", err1)

	// ---------- fmt.Errorf ----------
	fmt.Println("\n  fmt.Errorf：")
	name := "张三"
	err2 := fmt.Errorf("用户 %s 不存在", name)
	fmt.Printf("    错误信息: %v\n", err2)

	// ---------- 错误包装（Error Wrapping）----------
	// Go 1.13 引入了错误包装特性
	// 使用 %w 格式动词可以包装错误
	// 使用 errors.Is 和 errors.As 可以检查被包装的错误
	fmt.Println("\n  错误包装：")

	// 底层错误
	baseErr := ErrNotFound
	// 包装错误
	wrappedErr := fmt.Errorf("查询失败: %w", baseErr)
	fmt.Printf("    包装后的错误: %v\n", wrappedErr)

	// errors.Is: 判断错误链中是否包含某个特定错误
	if errors.Is(wrappedErr, ErrNotFound) {
		fmt.Println("    errors.Is: 错误链中包含 ErrNotFound")
	}

	// ---------- 自定义错误类型 ----------
	fmt.Println("\n  自定义错误类型：")

	validateInput := func(username string) error {
		if len(username) < 3 {
			return &ValidationError{
				Field:   "username",
				Message: "用户名长度不能少于3个字符",
			}
		}
		return nil
	}

	err := validateInput("ab")
	if err != nil {
		fmt.Printf("    验证错误: %v\n", err)

		// errors.As: 检查错误是否是某个类型
		var vErr *ValidationError
		if errors.As(err, &vErr) {
			fmt.Printf("    错误字段: %s\n", vErr.Field)
			fmt.Printf("    错误详情: %s\n", vErr.Message)
		}
	}

	// ---------- 哨兵错误 vs 自定义错误类型 ----------
	// 哨兵错误（如 ErrNotFound）：
	//   - 适合简单的、可枚举的错误
	//   - 使用 errors.Is 判断
	//   - 缺点：暴露了实现细节，难以扩展
	//
	// 自定义错误类型（如 ValidationError）：
	//   - 适合需要携带额外信息的错误
	//   - 使用 errors.As 判断
	//   - 优点：可扩展、信息丰富
}

// ============================================================
// 3. 反射（reflect 包）
// ============================================================
// 反射是指程序在运行时检查自身结构和行为的能力
// Go 的反射通过 reflect 包实现
//
// 反射的核心概念：
// - reflect.Type: 类型信息
// - reflect.Value: 值信息
// - Kind: 底层类型分类（如 int、struct、slice 等）
//
// 反射的三大定律：
// 1. 反射可以从接口值得到反射对象
// 2. 反射可以从反射对象得到接口值
// 3. 要修改反射对象，值必须是可设置的（settable）
//
// 注意：反射功能强大但性能较低，且代码可读性差，应谨慎使用

// demonstrateReflection 演示反射的基本使用
func demonstrateReflection() {
	fmt.Println("\n--- 3. 反射 reflect ---")

	// ---------- 获取类型信息 ----------
	fmt.Println("  获取类型信息：")

	x := 42
	t := reflect.TypeOf(x) // 获取 reflect.Type
	fmt.Printf("    TypeOf(42): %v, Kind: %v\n", t, t.Kind())

	type Person struct {
		Name string `json:"name" xml:"name_attr"`
		Age  int    `json:"age"`
	}

	p := Person{Name: "张三", Age: 25}
	t = reflect.TypeOf(p)
	fmt.Printf("    TypeOf(Person): %v, Kind: %v\n", t, t.Kind())
	fmt.Printf("    字段数量: %d\n", t.NumField())

	// 遍历结构体字段
	for i := 0; i < t.NumField(); i++ {
		field := t.Field(i)
		fmt.Printf("    字段%d: %s, 类型: %v, tag: %v\n",
			i, field.Name, field.Type, field.Tag)
		// 获取特定 tag
		jsonTag := field.Tag.Get("json")
		xmlTag := field.Tag.Get("xml")
		fmt.Printf("      json tag: %s, xml tag: %s\n", jsonTag, xmlTag)
	}

	// ---------- 获取值信息 ----------
	fmt.Println("\n  获取值信息：")

	v := reflect.ValueOf(p)
	fmt.Printf("    ValueOf(Person): %v\n", v)
	fmt.Printf("    字段数: %d\n", v.NumField())
	fmt.Printf("    Name 字段值: %v\n", v.Field(0))
	fmt.Printf("    Age 字段值: %v\n", v.FieldByName("Age"))

	// ---------- 修改值（需要指针且是可设置的）----------
	fmt.Println("\n  修改反射值：")

	// 要修改值，必须传递指针（否则 Value 是不可设置的）
	vp := reflect.ValueOf(&p).Elem() // Elem() 解引用指针
	fmt.Printf("    可设置? %t\n", vp.CanSet())

	if vp.CanSet() {
		// 修改 Name 字段
		nameField := vp.FieldByName("Name")
		if nameField.CanSet() {
			nameField.SetString("李四")
			fmt.Printf("    修改后 Name: %s\n", p.Name)
		}

		// 修改 Age 字段
		ageField := vp.FieldByName("Age")
		if ageField.CanSet() {
			ageField.SetInt(30)
			fmt.Printf("    修改后 Age: %d\n", p.Age)
		}
	}

	// ---------- 调用方法 ----------
	fmt.Println("\n  反射调用方法：")

	type Calculator struct{}
	calc := Calculator{}

	// 获取方法
	method := reflect.ValueOf(calc).MethodByName("Add")
	if method.IsValid() {
		// 构造参数（必须是 []reflect.Value）
		args := []reflect.Value{
			reflect.ValueOf(10),
			reflect.ValueOf(20),
		}
		// 调用方法
		results := method.Call(args)
		if len(results) > 0 {
			fmt.Printf("    调用 Add(10, 20) = %v\n", results[0].Int())
		}
	}

	// ---------- 反射的使用场景 ----------
	// 1. 序列化/反序列化（如 encoding/json）
	// 2. ORM 框架（数据库映射）
	// 3. 依赖注入框架
	// 4. 通用函数/工具库
	//
	// 注意：反射有性能开销，普通业务代码应尽量避免使用
}

// Add 计算器的加法方法（供反射调用演示）
func (c Calculator) Add(a, b int) int {
	return a + b
}

// Calculator 计算器结构体（供反射演示）
type Calculator struct{}

// ============================================================
// 4. 闭包（Closure）
// ============================================================
// 闭包是引用了自由变量的函数
// 自由变量会和函数绑定在一起，即使离开了声明环境仍然可以访问
//
// 闭包的本质：函数 + 引用环境
//
// 闭包的用途：
// 1. 封装状态（类似于对象的私有变量）
// 2. 实现工厂函数
// 3. 实现装饰器
// 4. 延迟执行

// demonstrateClosure 演示闭包
func demonstrateClosure() {
	fmt.Println("\n--- 4. 闭包 Closure ---")

	// ---------- 基本闭包 ----------
	fmt.Println("  基本闭包：")

	// makeAdder 返回一个加法器闭包
	// 这个闭包"记住"了创建它时的 base 参数
	makeAdder := func(base int) func(int) int {
		// base 是自由变量，被返回的函数捕获
		return func(x int) int {
			return base + x // 引用了外部的 base 变量
		}
	}

	add5 := makeAdder(5)
	add10 := makeAdder(10)

	fmt.Printf("    add5(3) = %d\n", add5(3))   // 5 + 3 = 8
	fmt.Printf("    add5(7) = %d\n", add5(7))   // 5 + 7 = 12
	fmt.Printf("    add10(3) = %d\n", add10(3)) // 10 + 3 = 13

	// ---------- 闭包封装状态 ----------
	fmt.Println("\n  闭包封装状态：")

	// makeCounter 创建一个计数器闭包
	// 计数器的状态（count）是私有的，只能通过返回的函数访问
	makeCounter := func() func() int {
		count := 0 // 私有状态，外部无法直接访问
		return func() int {
			count++
			return count
		}
	}

	counter1 := makeCounter()
	fmt.Printf("    counter1: %d\n", counter1()) // 1
	fmt.Printf("    counter1: %d\n", counter1()) // 2
	fmt.Printf("    counter1: %d\n", counter1()) // 3

	counter2 := makeCounter()
	fmt.Printf("    counter2: %d\n", counter2()) // 1（独立的计数器）

	// ---------- 闭包的常见陷阱：循环变量捕获 ----------
	fmt.Println("\n  闭包陷阱：循环变量捕获")

	// 错误示例：所有闭包都捕获同一个变量 i
	var funcs []func()
	for i := 0; i < 3; i++ {
		funcs = append(funcs, func() {
			// 这里捕获的是变量 i 的引用，不是值
			// 循环结束后 i = 3，所以所有函数打印的都是 3
			fmt.Printf("    错误捕获: i = %d\n", i)
		})
	}

	// 执行函数（预期打印 0, 1, 2，实际都打印 3）
	fmt.Println("    错误捕获结果（循环变量引用）：")
	for _, f := range funcs {
		f()
	}

	// 正确示例：每次循环创建一个新变量
	var funcs2 []func()
	for i := 0; i < 3; i++ {
		i := i // 在循环体内重新声明变量（每次迭代创建一个新变量）
		funcs2 = append(funcs2, func() {
			fmt.Printf("    正确捕获: i = %d\n", i)
		})
	}

	fmt.Println("    正确捕获结果（每次迭代新变量）：")
	for _, f := range funcs2 {
		f()
	}
}

// ============================================================
// 5. defer 执行顺序详解
// ============================================================
// defer 的执行规则：
// 1. 多个 defer 按 LIFO（后进先出）顺序执行
// 2. defer 语句中的参数在声明时就已经求值（不是在执行时）
// 3. defer 可以修改函数的命名返回值

// demonstrateDeferOrder 演示 defer 的执行顺序
func demonstrateDeferOrder() {
	fmt.Println("\n--- 5. defer 执行顺序 ---")

	// ---------- LIFO 顺序 ----------
	fmt.Println("  LIFO 顺序（后进先出）：")
	fmt.Println("    请查看代码中的 defer 顺序")

	// 执行顺序：3, 2, 1
	defer fmt.Println("    defer 1")
	defer fmt.Println("    defer 2")
	defer fmt.Println("    defer 3")

	// ---------- defer 参数立即求值 ----------
	fmt.Println("\n  defer 参数立即求值：")

	i := 0
	defer fmt.Printf("    defer 中的 i = %d (声明时求值)\n", i) // i = 0
	i = 100
	fmt.Printf("    函数体中 i = %d\n", i) // i = 100

	// ---------- defer 修改命名返回值 ----------
	fmt.Println("\n  defer 修改命名返回值：")

	result := modifyReturn()
	fmt.Printf("    modifyReturn() 返回: %d (被 defer 修改了)\n", result)
}

// modifyReturn 演示 defer 修改命名返回值
// 返回值有名字（result），defer 可以在函数返回前修改它
func modifyReturn() (result int) {
	result = 100 // 设置返回值为 100

	// defer 在函数返回前执行，可以修改命名返回值
	defer func() {
		result += 50 // 修改返回值，变为 150
	}()

	return result // 返回前先执行 defer，所以实际返回 150
}

// ============================================================
// 6. 函数选项模式（Functional Options）
// ============================================================
// 函数选项模式是 Go 中一种优雅的配置对象方式
// 通过一系列返回 Option 函数的函数来配置对象
//
// 优点：
// 1. 支持默认值（不需要传所有参数）
// 2. 可扩展（新增选项不破坏现有代码）
// 3. 可读性好（配置项有名字）
// 4. 支持顺序无关

// ServerConfig 服务器配置
type ServerConfig struct {
	Host    string // 主机地址
	Port    int    // 端口号
	Timeout int    // 超时时间（秒）
	MaxConn int    // 最大连接数
	Debug   bool   // 调试模式
}

// ServerOption 是一个函数类型，用于修改 ServerConfig
// 这是函数选项的核心：选项就是一个修改配置的函数
type ServerOption func(*ServerConfig)

// WithHost 设置主机地址
// 返回一个 ServerOption 函数
func WithHost(host string) ServerOption {
	return func(c *ServerConfig) {
		c.Host = host
	}
}

// WithPort 设置端口号
func WithPort(port int) ServerOption {
	return func(c *ServerConfig) {
		c.Port = port
	}
}

// WithTimeout 设置超时时间
func WithTimeout(timeout int) ServerOption {
	return func(c *ServerConfig) {
		c.Timeout = timeout
	}
}

// WithMaxConn 设置最大连接数
func WithMaxConn(maxConn int) ServerOption {
	return func(c *ServerConfig) {
		c.MaxConn = maxConn
	}
}

// WithDebug 设置调试模式
func WithDebug(debug bool) ServerOption {
	return func(c *ServerConfig) {
		c.Debug = debug
	}
}

// NewServer 创建服务器（函数选项模式）
// 参数是可变的 ServerOption，调用者可以按需传入配置
func NewServer(opts ...ServerOption) *ServerConfig {
	// 默认配置
	config := &ServerConfig{
		Host:    "localhost",
		Port:    8080,
		Timeout: 30,
		MaxConn: 100,
		Debug:   false,
	}

	// 应用所有选项
	for _, opt := range opts {
		opt(config)
	}

	return config
}

// demonstrateFunctionalOptions 演示函数选项模式
func demonstrateFunctionalOptions() {
	fmt.Println("\n--- 6. 函数选项模式 ---")

	// 使用默认配置创建服务器
	server1 := NewServer()
	fmt.Printf("  默认配置: %+v\n", *server1)

	// 使用部分选项创建服务器
	server2 := NewServer(
		WithHost("0.0.0.0"),
		WithPort(9090),
	)
	fmt.Printf("  自定义端口: %+v\n", *server2)

	// 使用所有选项创建服务器
	server3 := NewServer(
		WithHost("127.0.0.1"),
		WithPort(8888),
		WithTimeout(60),
		WithMaxConn(200),
		WithDebug(true),
	)
	fmt.Printf("  完整配置: %+v\n", *server3)

	// 函数选项模式的优点：
	// 1. 支持默认值（不传的选项用默认值）
	// 2. 可扩展（新增选项只需加一个 WithXxx 函数）
	// 3. 可读性好（选项名清晰表达含义）
	// 4. 顺序无关（选项可以按任意顺序传入）
	// 5. 向后兼容（新增选项不影响现有调用）
}

// ============================================================
// 7. 泛型基础（Go 1.18+）
// ============================================================
// Go 1.18 引入了泛型（Generics）
// 使用 type parameters（类型参数）实现泛型
//
// 语法：
//   func 函数名[T 约束](参数 T) 返回值 { ... }
//
// 约束（Constraint）：
// - any: 任意类型（等价于 interface{}）
// - comparable: 可比较的类型（支持 == 和 !=）
// - 自定义约束：使用接口定义

// ---------- 泛型函数 ----------

// PrintSlice 打印任意类型的切片
// T 是类型参数，any 表示 T 可以是任意类型
func PrintSlice[T any](s []T) {
	fmt.Print("[")
	for i, v := range s {
		if i > 0 {
			fmt.Print(", ")
		}
		fmt.Printf("%v", v)
	}
	fmt.Println("]")
}

// Map 对切片中的每个元素应用转换函数，返回新切片
// 两个类型参数：T（输入类型）和 E（输出类型）
func Map[T any, E any](s []T, f func(T) E) []E {
	result := make([]E, len(s))
	for i, v := range s {
		result[i] = f(v)
	}
	return result
}

// Filter 过滤切片，返回满足条件的元素
func Filter[T any](s []T, f func(T) bool) []T {
	var result []T
	for _, v := range s {
		if f(v) {
			result = append(result, v)
		}
	}
	return result
}

// Reduce 归约切片
func Reduce[T any, E any](s []T, init E, f func(E, T) E) E {
	result := init
	for _, v := range s {
		result = f(result, v)
	}
	return result
}

// ---------- 自定义约束 ----------

// Number 是一个自定义约束，表示所有数值类型
// 使用接口定义约束，列出所有允许的类型
type Number interface {
	int | int8 | int16 | int32 | int64 |
		uint | uint8 | uint16 | uint32 | uint64 |
		float32 | float64
}

// Sum 计算切片中所有元素的和
// T 必须满足 Number 约束（即必须是数值类型）
func Sum[T Number](s []T) T {
	var sum T
	for _, v := range s {
		sum += v
	}
	return sum
}

// ---------- 泛型类型 ----------

// Stack 是一个泛型栈
// T 是栈元素的类型
type Stack[T any] struct {
	items []T // 底层使用切片存储
}

// Push 入栈
func (s *Stack[T]) Push(item T) {
	s.items = append(s.items, item)
}

// Pop 出栈
func (s *Stack[T]) Pop() (T, bool) {
	if len(s.items) == 0 {
		var zero T // 零值
		return zero, false
	}
	last := len(s.items) - 1
	item := s.items[last]
	s.items = s.items[:last]
	return item, true
}

// Len 获取栈大小
func (s *Stack[T]) Len() int {
	return len(s.items)
}

// demonstrateGenerics 演示泛型
func demonstrateGenerics() {
	fmt.Println("\n--- 7. 泛型 Generics ---")

	// ---------- 泛型函数 ----------
	fmt.Println("  泛型函数：")

	// 打印不同类型的切片
	fmt.Print("    int 切片: ")
	PrintSlice([]int{1, 2, 3, 4, 5})

	fmt.Print("    string 切片: ")
	PrintSlice([]string{"Go", "Java", "Python"})

	// Map 函数：转换切片元素
	nums := []int{1, 2, 3, 4, 5}
	squares := Map(nums, func(n int) int {
		return n * n
	})
	fmt.Print("    Map 平方: ")
	PrintSlice(squares)

	// Filter 函数：过滤
	evens := Filter(nums, func(n int) bool {
		return n%2 == 0
	})
	fmt.Print("    Filter 偶数: ")
	PrintSlice(evens)

	// Reduce 函数：归约
	sum := Reduce(nums, 0, func(acc, n int) int {
		return acc + n
	})
	fmt.Printf("    Reduce 求和: %d\n", sum)

	// ---------- 自定义约束 ----------
	fmt.Println("\n  自定义约束：")

	intSum := Sum([]int{1, 2, 3, 4, 5})
	fmt.Printf("    int 求和: %d\n", intSum)

	floatSum := Sum([]float64{1.5, 2.5, 3.0})
	fmt.Printf("    float64 求和: %.1f\n", floatSum)

	// ---------- 泛型类型 ----------
	fmt.Println("\n  泛型类型：")

	// int 类型的栈
	intStack := &Stack[int]{}
	intStack.Push(10)
	intStack.Push(20)
	intStack.Push(30)
	fmt.Printf("    int 栈大小: %d\n", intStack.Len())

	if val, ok := intStack.Pop(); ok {
		fmt.Printf("    弹出: %d\n", val)
	}

	// string 类型的栈
	strStack := &Stack[string]{}
	strStack.Push("hello")
	strStack.Push("world")
	fmt.Printf("    string 栈大小: %d\n", strStack.Len())

	if val, ok := strStack.Pop(); ok {
		fmt.Printf("    弹出: %s\n", val)
	}

	// 泛型的使用场景：
	// 1. 通用数据结构（栈、队列、链表、树等）
	// 2. 通用工具函数（map、filter、reduce 等）
	// 3. 算法实现（排序、搜索等）
	//
	// 注意：不要滥用泛型
	// 如果用接口就能解决，优先用接口
	// 泛型适合真正需要类型参数化的场景
}

// ============================================================
// 8. embed 嵌入静态文件
// ============================================================
// //go:embed 是 Go 1.16 引入的指令
// 可以在编译时将静态文件嵌入到二进制程序中
//
// 支持三种类型：
// 1. string: 嵌入单个文件为字符串
// 2. []byte: 嵌入单个文件为字节切片
// 3. embed.FS: 嵌入多个文件/目录为文件系统
//
// 注意：
// - //go:embed 指令必须紧挨着变量声明
// - 变量必须是包级变量（不能是函数内局部变量）
// - 路径是相对于源文件的
// - 不能嵌入以 . 或 _ 开头的文件（除非使用 *）

// demonstrateEmbed 演示 embed 的使用
// 注意：由于这是学习项目，我们使用字符串模拟 embed 的效果
// 实际项目中需要导入 "embed" 包并使用 //go:embed 指令
func demonstrateEmbed() {
	fmt.Println("\n--- 8. embed 嵌入静态文件 ---")

	// embed 的基本用法（概念演示）：
	//
	// import "embed"
	//
	// // 嵌入单个文件为 string
	// //go:embed version.txt
	// var version string
	//
	// // 嵌入单个文件为 []byte
	// //go:embed config.json
	// var config []byte
	//
	// // 嵌入多个文件/目录为 embed.FS
	// //go:embed templates/*
	// var templates embed.FS

	fmt.Println("  embed 概念说明：")
	fmt.Println("    //go:embed 是 Go 1.16 引入的编译指令")
	fmt.Println("    可以将静态文件嵌入到二进制程序中")
	fmt.Println("    支持 string、[]byte、embed.FS 三种类型")

	fmt.Println("\n  模拟嵌入效果：")

	// 模拟嵌入的版本文件
	version := "1.0.0"
	fmt.Printf("    嵌入的版本号: %s\n", version)

	// 模拟嵌入的配置
	config := `{
  "host": "localhost",
  "port": 8080,
  "debug": true
}`
	fmt.Printf("    嵌入的配置:\n%s\n", indent(config, "      "))

	// embed 的使用场景：
	// 1. Web 应用：嵌入 HTML 模板、静态资源（CSS/JS/图片）
	// 2. 配置文件：嵌入默认配置
	// 3. 数据库迁移脚本
	// 4. 证书文件
	//
	// 优点：
	// - 单二进制部署（不需要额外携带资源文件）
	// - 启动速度快（不需要读文件）
	// - 安全性（资源被编译进二进制，不易被篡改）
}

// indent 给字符串的每一行添加缩进
func indent(s, prefix string) string {
	lines := strings.Split(s, "\n")
	for i, line := range lines {
		lines[i] = prefix + line
	}
	return strings.Join(lines, "\n")
}
