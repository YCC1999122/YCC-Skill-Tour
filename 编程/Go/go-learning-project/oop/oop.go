// ============================================================
// package oop 面向对象编程模块
// Go 语言不是传统的面向对象语言（没有 class、继承、多态等关键字），
// 但它通过 struct + method + interface 的组合实现了面向对象的核心特性。
// Go 的设计哲学是"组合优于继承"，通过嵌入（embedding）实现代码复用。
// ============================================================
package oop

import (
	"fmt"
)

// ============================================================
// RunOopDemo 是面向对象模块的入口函数
// ============================================================
func RunOopDemo() {
	fmt.Println("\n========== 面向对象编程演示 ==========")

	demonstrateStruct()       // 演示结构体
	demonstrateMethods()      // 演示方法
	demonstrateInterface()    // 演示接口
	demonstrateTypeAssertion()// 演示类型断言
	demonstrateEmbedding()    // 演示结构体嵌入
	demonstrateStringer()     // 演示 Stringer 接口
	demonstrateError()        // 演示 error 接口
}

// ============================================================
// 1. 结构体（struct）
// ============================================================
// Go 中没有 class，使用 struct 定义自定义类型
// struct 是一组字段的集合，类似于 C 的 struct 或其他语言的 class（只有数据）

// Person 是一个表示人的结构体
// 字段名首字母大写表示导出（public），小写表示未导出（private）
type Person struct {
	Name string // 姓名，导出字段
	Age  int    // 年龄，导出字段
	city string // 城市，未导出字段（只能在本包内访问）
}

// demonstrateStruct 演示结构体的定义和使用
func demonstrateStruct() {
	fmt.Println("\n--- 1. 结构体 Struct ---")

	// ---------- 结构体初始化 ----------

	// 方式1：使用字段名初始化（推荐，可读性好，顺序无关）
	p1 := Person{
		Name: "张三",
		Age:  25,
		city: "北京",
	}
	fmt.Printf("字段名初始化: p1 = %+v\n", p1) // %+v 会显示字段名

	// 方式2：按顺序初始化（不推荐，依赖字段顺序，可读性差）
	p2 := Person{"李四", 30, "上海"}
	fmt.Printf("顺序初始化: p2 = %v\n", p2)

	// 方式3：零值结构体（所有字段都是零值）
	var p3 Person
	fmt.Printf("零值结构体: p3 = %v\n", p3)

	// 方式4：使用 new 创建指针
	p4 := new(Person) // 返回 *Person，指向零值结构体
	p4.Name = "王五"
	p4.Age = 28
	fmt.Printf("new创建: p4 = %v\n", *p4)

	// 方式5：取地址初始化（最常用，等效于 new 后赋值）
	p5 := &Person{
		Name: "赵六",
		Age:  35,
	}
	fmt.Printf("取地址初始化: p5 = %v, 类型: %T\n", p5, p5)

	// ---------- 访问字段 ----------
	// 使用 . 访问字段
	fmt.Printf("访问字段: p1.Name = %s, p1.Age = %d\n", p1.Name, p1.Age)

	// 结构体指针也可以用 . 访问字段（Go 会自动解引用）
	// 在其他语言中可能需要 p5->Name，但 Go 直接用 .
	fmt.Printf("指针访问字段: p5.Name = %s\n", p5.Name) // 自动解引用

	// 修改字段
	p1.Age = 26
	fmt.Printf("修改后: p1.Age = %d\n", p1.Age)

	// ---------- 结构体比较 ----------
	// 如果结构体的所有字段都是可比较的，那么结构体本身也是可比较的
	p6 := Person{"张三", 26, "北京"}
	fmt.Printf("结构体比较: p1 == p6? %t\n", p1 == p6)

	// ---------- 结构体是值类型 ----------
	// 结构体赋值和传参都是值拷贝
	p7 := p1
	p7.Name = "改名了"
	fmt.Printf("值类型: p1.Name = %s (不变), p7.Name = %s (已改)\n", p1.Name, p7.Name)

	// ---------- 匿名字段 ----------
	// 结构体可以包含匿名字段（只有类型，没有名字）
	// 匿名字段的名字默认为类型名
	// 这是结构体嵌入的基础（后面会详细讲）
	type Point struct {
		x int // 匿名字段也可以是基本类型（但不常用）
		y int
	}
	pt := Point{10, 20}
	fmt.Printf("匿名字段: pt.x = %d, pt.y = %d\n", pt.x, pt.y)
}

// ============================================================
// 2. 方法（Method）
// ============================================================
// Go 中的方法是附加到特定类型上的函数
// 方法定义：func (接收者 接收者类型) 方法名(参数) 返回值 { ... }
// 接收者（receiver）相当于其他语言的 this/self
//
// 接收者有两种类型：
// - 值接收者（value receiver）：操作的是接收者的拷贝
// - 指针接收者（pointer receiver）：操作的是接收者本身（可以修改原值）

// Circle 是一个圆形结构体
type Circle struct {
	Radius float64 // 半径
}

// Area 计算圆的面积（值接收者）
// 值接收者：方法操作的是 Circle 的拷贝，不会修改原始对象
// 适用场景：不需要修改接收者，或者接收者是小对象（拷贝成本低）
func (c Circle) Area() float64 {
	// c 是接收者的拷贝，修改 c.Radius 不会影响原始对象
	return 3.14159 * c.Radius * c.Radius
}

// Resize 修改圆的半径（指针接收者）
// 指针接收者：方法操作的是原始对象的指针，可以修改原始对象
// 适用场景：需要修改接收者，或者接收者是大对象（避免拷贝开销）
func (c *Circle) Resize(newRadius float64) {
	// c 是指针，通过 c.Radius 修改原始对象
	// Go 会自动解引用，所以可以直接写 c.Radius 而不是 (*c).Radius
	c.Radius = newRadius
}

// demonstrateMethods 演示方法的定义和使用
func demonstrateMethods() {
	fmt.Println("\n--- 2. 方法 Method ---")

	// 创建一个圆
	c := Circle{Radius: 10}
	fmt.Printf("初始: 半径 = %.2f, 面积 = %.2f\n", c.Radius, c.Area())

	// 调用值接收者方法（值和指针都可以调用）
	// Go 会自动在值和指针之间转换
	area := c.Area() // c 是值，自动取地址？不，值接收者用值调用即可
	fmt.Printf("面积: %.2f\n", area)

	// 调用指针接收者方法
	// 虽然 c 是值，但 Go 会自动取地址：(&c).Resize(20)
	c.Resize(20)
	fmt.Printf("Resize后: 半径 = %.2f, 面积 = %.2f\n", c.Radius, c.Area())

	// 使用指针调用方法
	cp := &c
	cp.Resize(30) // 指针调用指针接收者方法
	fmt.Printf("指针调用: 半径 = %.2f\n", cp.Radius)

	// 值接收者 vs 指针接收者的选择：
	// 1. 需要修改接收者 → 指针接收者
	// 2. 接收者很大（拷贝成本高）→ 指针接收者
	// 3. 接收者包含不能拷贝的字段（如 sync.Mutex）→ 指针接收者
	// 4. 不需要修改，接收者小 → 值接收者
	// 5. 一致性：如果某个方法用了指针接收者，其他方法最好也用指针接收者

	// ---------- 为任意类型添加方法 ----------
	// Go 可以为任何类型添加方法，不仅仅是结构体
	// 但不能为其他包的类型添加方法（包括内置类型）
	// 解决办法：定义自己的类型别名
	var myAge MyInt = 25
	fmt.Printf("自定义类型方法: %d 是否是偶数？%t\n", myAge, myAge.IsEven())
	fmt.Printf("自定义类型方法: %d 的平方 = %d\n", myAge, myAge.Square())
}

// MyInt 是基于 int 的自定义类型
// 这样我们就可以为 int 类型添加方法了
type MyInt int

// IsEven 判断是否是偶数
// 接收者是值类型（MyInt），因为不需要修改
func (m MyInt) IsEven() bool {
	return m%2 == 0
}

// Square 返回平方值
func (m MyInt) Square() MyInt {
	return m * m
}

// ============================================================
// 3. 接口（Interface）
// ============================================================
// Go 的接口是隐式实现的（鸭子类型），不需要 implements 关键字
// 只要一个类型实现了接口的所有方法，它就自动实现了该接口
//
// 接口的设计原则：
// - 接口要小（通常只有 1-3 个方法）
// - 接受接口，返回结构体（Accept interfaces, return structs）
// - 接口定义在使用方（而不是实现方）

// Shape 是形状接口
// 任何实现了 Area() 方法的类型都自动实现了 Shape 接口
type Shape interface {
	Area() float64 // 面积方法
}

// Perimeter 是周长接口
// 演示一个类型可以实现多个接口
type Perimeter interface {
	Perimeter() float64 // 周长方法
}

// Rectangle 是矩形结构体
type Rectangle struct {
	Width  float64 // 宽
	Height float64 // 高
}

// Area 实现 Shape 接口的 Area 方法
// 注意：这里是值接收者，所以 Rectangle 和 *Rectangle 都实现了 Shape 接口
func (r Rectangle) Area() float64 {
	return r.Width * r.Height
}

// Perimeter 实现 Perimeter 接口的 Perimeter 方法
func (r Rectangle) Perimeter() float64 {
	return 2 * (r.Width + r.Height)
}

// Circle 的 Area 方法前面已经定义过了
// 所以 Circle 也自动实现了 Shape 接口
// 这就是 Go 的隐式实现：不需要 Circle 声明 "implements Shape"

// demonstrateInterface 演示接口的使用
func demonstrateInterface() {
	fmt.Println("\n--- 3. 接口 Interface ---")

	// ---------- 接口变量 ----------
	// 接口类型的变量可以存储任何实现了该接口的类型
	var s Shape // 声明一个 Shape 接口变量

	// 可以将 Circle 赋值给 Shape（因为 Circle 实现了 Area 方法）
	s = Circle{Radius: 5}
	fmt.Printf("Circle作为Shape: 面积 = %.2f\n", s.Area())

	// 可以将 Rectangle 赋值给 Shape
	s = Rectangle{Width: 4, Height: 6}
	fmt.Printf("Rectangle作为Shape: 面积 = %.2f\n", s.Area())

	// ---------- 接口切片 ----------
	// 接口切片可以包含不同类型的实现
	shapes := []Shape{
		Circle{Radius: 10},
		Rectangle{Width: 3, Height: 4},
		Circle{Radius: 5},
		Rectangle{Width: 5, Height: 8},
	}

	// 多态：同样的接口调用，不同的实现
	fmt.Println("多态演示：")
	totalArea := 0.0
	for i, shape := range shapes {
		area := shape.Area()
		totalArea += area
		fmt.Printf("  形状%d: 面积 = %.2f\n", i+1, area)
	}
	fmt.Printf("总面积: %.2f\n", totalArea)

	// ---------- 空接口 interface{} ----------
	// 空接口没有任何方法，所以所有类型都实现了空接口
	// 空接口可以存储任意类型的值，类似于 Java 的 Object
	var anything interface{}

	anything = 42 // 存整数
	fmt.Printf("空接口存int: %v, 类型: %T\n", anything, anything)

	anything = "hello" // 存字符串
	fmt.Printf("空接口存string: %v, 类型: %T\n", anything, anything)

	anything = Circle{Radius: 10} // 存结构体
	fmt.Printf("空接口存结构体: %v, 类型: %T\n", anything, anything)

	// 空接口的使用场景：
	// 1. 泛型容器（存储任意类型）
	// 2. 函数参数可以接受任意类型（如 fmt.Print 的参数）
	// 3. 但要注意：使用空接口会失去类型安全，需要类型断言才能访问具体类型

	// 空接口切片示例（类似动态类型数组）
	mixed := []interface{}{1, "hello", 3.14, true}
	fmt.Print("空接口切片: ")
	for _, v := range mixed {
		fmt.Printf("%v(%T) ", v, v)
	}
	fmt.Println()

	// ---------- 接口值的内部结构 ----------
	// 接口值在内部由两部分组成：
	// 1. 动态类型（dynamic type）：具体的类型信息
	// 2. 动态值（dynamic value）：具体的值
	//
	// 接口的 nil 判断：
	// 只有当动态类型和动态值都为 nil 时，接口才等于 nil
	// 这是一个常见的坑！

	var p *Person // p 是 nil 指针
	var i interface{} = p
	fmt.Printf("接口nil陷阱: i == nil? %t (动态类型是*Person，不是nil)\n", i == nil)
	// 解释：虽然 p 是 nil，但 i 的动态类型是 *Person（不是 nil）
	// 所以 i != nil，这是 Go 新手常踩的坑

	// 正确的 nil 接口
	var nilIface interface{}
	fmt.Printf("真正的nil接口: nilIface == nil? %t\n", nilIface == nil)
}

// ============================================================
// 4. 类型断言与类型选择
// ============================================================
// 类型断言（Type Assertion）：从接口值中提取具体类型的值
// 语法：value.(Type) - 不安全，如果类型不对会 panic
// 语法：value, ok := value.(Type) - 安全，ok 表示是否成功
//
// 类型选择（Type Switch）：根据接口值的动态类型执行不同的操作

// demonstrateTypeAssertion 演示类型断言和类型选择
func demonstrateTypeAssertion() {
	fmt.Println("\n--- 4. 类型断言与类型选择 ---")

	// ---------- 类型断言 ----------
	var i interface{} = "hello"

	// 不安全的类型断言：如果类型不匹配会 panic
	s := i.(string) // 断言 i 是 string 类型
	fmt.Printf("类型断言: s = %s\n", s)

	// 安全的类型断言：ok-idiom
	// 返回两个值：断言后的值 + 是否成功的 bool
	s, ok := i.(string)
	if ok {
		fmt.Printf("安全断言成功: s = %s\n", s)
	} else {
		fmt.Println("安全断言失败")
	}

	// 断言失败的例子
	n, ok := i.(int) // i 是 string，断言 int 失败
	if ok {
		fmt.Printf("断言int成功: n = %d\n", n)
	} else {
		fmt.Printf("断言int失败: n = %d (零值), ok = %t\n", n, ok)
	}

	// ---------- 类型选择 Type Switch ----------
	// 使用 switch 和 type 关键字，可以根据接口的动态类型分支
	printType := func(v interface{}) {
		switch val := v.(type) { // 特殊语法：.(type)，只能在 switch 中使用
		case int:
			fmt.Printf("类型选择: 这是int，值 = %d\n", val)
		case string:
			fmt.Printf("类型选择: 这是string，值 = %s，长度 = %d\n", val, len(val))
		case float64:
			fmt.Printf("类型选择: 这是float64，值 = %f\n", val)
		case bool:
			fmt.Printf("类型选择: 这是bool，值 = %t\n", val)
		default:
			fmt.Printf("类型选择: 未知类型，值 = %v，类型 = %T\n", val, val)
		}
	}

	printType(42)
	printType("hello")
	printType(3.14)
	printType(true)
	printType([]int{1, 2, 3})

	// ---------- 接口的组合 ----------
	// Go 可以通过接口嵌入组合多个接口（类似多重继承）
	// 这是 Go 实现接口复用的方式
	//
	// 例子：io.ReadWriter 就是 io.Reader + io.Writer
	// type ReadWriter interface {
	//     Reader
	//     Writer
	// }

	// 下面演示一个组合接口
	var ss ShapeAndPerimeter = Rectangle{Width: 3, Height: 4}
	fmt.Printf("组合接口: 面积 = %.2f, 周长 = %.2f\n", ss.Area(), ss.Perimeter())
}

// ShapeAndPerimeter 是组合接口，同时包含 Shape 和 Perimeter
// 任何同时实现了 Area 和 Perimeter 方法的类型都实现了这个接口
type ShapeAndPerimeter interface {
	Shape      // 嵌入 Shape 接口
	Perimeter  // 嵌入 Perimeter 接口
}

// ============================================================
// 5. 结构体嵌入（Embedding）
// ============================================================
// Go 没有继承，但可以通过结构体嵌入实现代码复用
// 嵌入是指在一个结构体中，将另一个结构体作为匿名字段
// 嵌入后，外层结构体可以直接访问内层结构体的字段和方法
// 这类似于继承，但本质上是组合（has-a 关系，不是 is-a 关系）
//
// 设计原则：组合优于继承（Composition over inheritance）

// Address 地址结构体
type Address struct {
	Province string // 省份
	City     string // 城市
	Street   string // 街道
	ZipCode  string // 邮编
}

// FullAddress 返回完整地址字符串
// 这是 Address 的方法
func (a Address) FullAddress() string {
	return fmt.Sprintf("%s省%s市%s %s", a.Province, a.City, a.Street, a.ZipCode)
}

// Employee 员工结构体
// 嵌入了 Person 和 Address 两个结构体
// 这模拟了"多重继承"，但本质是组合
type Employee struct {
	Person        // 嵌入 Person 结构体（匿名字段，字段名就是类型名 Person）
	Address       // 嵌入 Address 结构体
	EmployeeID int  // 员工编号
	Position   string // 职位
}

// Work 员工工作方法
func (e Employee) Work() {
	fmt.Printf("员工%s（工号：%d）正在工作，职位：%s\n", e.Name, e.EmployeeID, e.Position)
}

// demonstrateEmbedding 演示结构体嵌入
func demonstrateEmbedding() {
	fmt.Println("\n--- 5. 结构体嵌入 Embedding ---")

	// 创建员工对象
	emp := Employee{
		Person: Person{
			Name: "张三",
			Age:  30,
			city: "北京",
		},
		Address: Address{
			Province: "广东",
			City:     "深圳",
			Street:   "科技园南路",
			ZipCode:  "518000",
		},
		EmployeeID: 1001,
		Position:   "高级工程师",
	}

	// ---------- 提升字段（Promoted Fields）----------
	// 嵌入结构体的字段会被"提升"到外层，可以直接访问
	// 不需要 emp.Person.Name，直接 emp.Name 即可
	fmt.Printf("提升字段: emp.Name = %s\n", emp.Name)   // 等价于 emp.Person.Name
	fmt.Printf("提升字段: emp.Age = %d\n", emp.Age)     // 等价于 emp.Person.Age
	fmt.Printf("提升字段: emp.City = %s\n", emp.City)   // 等价于 emp.Address.City
	fmt.Printf("自有字段: emp.Position = %s\n", emp.Position)

	// 也可以通过完整路径访问（当有字段名冲突时需要这样做）
	fmt.Printf("完整路径: emp.Person.Name = %s\n", emp.Person.Name)
	fmt.Printf("完整路径: emp.Address.City = %s\n", emp.Address.City)
	fmt.Printf("Person的city: emp.Person.city = %s\n", emp.Person.city)

	// ---------- 提升方法（Promoted Methods）----------
	// 嵌入结构体的方法也会被提升
	// FullAddress 是 Address 的方法，但可以直接通过 emp 调用
	fmt.Printf("提升方法: emp.FullAddress() = %s\n", emp.FullAddress())

	// 调用 Employee 自己的方法
	emp.Work()

	// ---------- 嵌入与继承的区别 ----------
	// 1. 嵌入是组合关系（has-a），不是继承关系（is-a）
	//    Employee 不是 Person，而是 has a Person
	// 2. 嵌入不能实现多态（Employee 不能赋值给 Person 类型的变量）
	//    但可以通过接口实现多态
	// 3. 嵌入更加灵活，可以嵌入多个类型（多重组合）
	// 4. Go 官方推荐使用组合而不是继承

	// ---------- 方法重写（Method Overriding）----------
	// 如果外层结构体定义了和内层同名的方法，外层方法会"遮蔽"内层方法
	// 这类似于子类重写父类方法
	dog := Dog{
		Animal: Animal{Name: "旺财"},
		Breed:  "金毛",
	}
	dog.Speak()      // 调用 Dog 的 Speak 方法（重写了 Animal 的 Speak）
	dog.Animal.Speak() // 显式调用 Animal 的 Speak 方法
	dog.Eat()        // 调用提升的 Eat 方法
}

// Animal 动物结构体
type Animal struct {
	Name string // 名字
}

// Speak 动物叫的方法
func (a Animal) Speak() {
	fmt.Printf("动物%s发出了叫声\n", a.Name)
}

// Eat 动物吃东西的方法
func (a Animal) Eat() {
	fmt.Printf("动物%s在吃东西\n", a.Name)
}

// Dog 狗结构体，嵌入 Animal
type Dog struct {
	Animal // 嵌入 Animal
	Breed  string // 品种
}

// Speak 狗叫的方法（重写了 Animal 的 Speak 方法）
func (d Dog) Speak() {
	fmt.Printf("狗%s在汪汪叫（品种：%s）\n", d.Name, d.Breed)
}

// ============================================================
// 6. Stringer 接口
// ============================================================
// fmt 包中定义的 Stringer 接口：
// type Stringer interface {
//     String() string
// }
//
// 如果一个类型实现了 Stringer 接口，
// 那么 fmt.Print/Printf(%v) 等函数会调用它的 String() 方法
// 这类似于 Java 的 toString() 或 Python 的 __str__()

// Book 书籍结构体
type Book struct {
	Title  string // 书名
	Author string // 作者
	Price  float64 // 价格
	Pages  int    // 页数
}

// String 实现 fmt.Stringer 接口
// 这样 fmt.Println(book) 会自动调用这个方法
func (b Book) String() string {
	return fmt.Sprintf("《%s》 - 作者: %s, 价格: ¥%.2f, %d页",
		b.Title, b.Author, b.Price, b.Pages)
}

// demonstrateStringer 演示 Stringer 接口
func demonstrateStringer() {
	fmt.Println("\n--- 6. Stringer 接口 ---")

	book := Book{
		Title:  "Go语言圣经",
		Author: "Alan A. A. Donovan",
		Price:  79.00,
		Pages:  500,
	}

	// 因为 Book 实现了 Stringer 接口，所以 %v 会调用 String() 方法
	fmt.Printf("Stringer: %v\n", book)

	// %+v 显示字段名（但如果实现了 String()，也会调用 String()）
	// 注意：不同版本行为可能有差异

	// 使用 %#v 显示 Go 语法格式（不调用 String()）
	fmt.Printf("Go语法格式: %#v\n", book)
}

// ============================================================
// 7. error 接口
// ============================================================
// Go 中错误是值，通过 error 接口表示
// type error interface {
//     Error() string
// }
//
// Go 的错误处理哲学：
// - 显式处理错误（不使用异常）
// - 函数通常返回 (结果, error)
// - 如果 error 为 nil 表示成功，否则表示失败
// - 错误是值，可以像普通值一样传递、存储、比较

// MyError 自定义错误类型
// 实现了 error 接口（即有 Error() string 方法）
type MyError struct {
	Code    int    // 错误码
	Message string // 错误信息
}

// Error 实现 error 接口
func (e *MyError) Error() string {
	return fmt.Sprintf("错误码: %d, 错误信息: %s", e.Code, e.Message)
}

// divide 除法函数，返回结果和错误
// 这是 Go 中典型的错误处理模式
func divide(a, b float64) (float64, error) {
	if b == 0 {
		// 返回自定义错误
		return 0, &MyError{
			Code:    1001,
			Message: "除数不能为零",
		}
	}
	return a / b, nil // 成功时 error 为 nil
}

// demonstrateError 演示 error 接口
func demonstrateError() {
	fmt.Println("\n--- 7. error 接口 ---")

	// 成功的情况
	result, err := divide(10, 2)
	if err != nil {
		fmt.Printf("除法错误: %v\n", err)
	} else {
		fmt.Printf("除法成功: 10 / 2 = %.2f\n", result)
	}

	// 失败的情况
	result, err = divide(10, 0)
	if err != nil {
		fmt.Printf("除法错误: %v\n", err)

		// 类型断言，获取具体的错误类型
		if myErr, ok := err.(*MyError); ok {
			fmt.Printf("  错误码: %d\n", myErr.Code)
			fmt.Printf("  错误信息: %s\n", myErr.Message)
		}
	} else {
		fmt.Printf("除法成功: %.2f\n", result)
	}

	// 使用 errors.New 创建简单错误
	// 标准库 errors 包提供了基本的错误创建函数
	// errors.New("错误信息") 创建一个简单的错误
	fmt.Println("提示: 标准库 errors.New() 可创建简单错误")
	fmt.Println("提示: fmt.Errorf() 可创建格式化错误")
}
