// ============================================================
// package patterns 设计模式模块
// 使用 Go 语言实现常见的设计模式
// 包括：单例模式、工厂模式、观察者模式、策略模式、装饰器模式、选项模式
// ============================================================
package patterns

import (
	"fmt"
	"sync"
	"time"
)

// ============================================================
// RunPatternsDemo 是设计模式模块的入口函数
// ============================================================
func RunPatternsDemo() {
	fmt.Println("\n========== 设计模式演示 ==========")

	demonstrateSingleton()  // 演示单例模式
	demonstrateFactory()    // 演示工厂模式
	demonstrateObserver()   // 演示观察者模式
	demonstrateStrategy()   // 演示策略模式
	demonstrateDecorator()  // 演示装饰器模式
	demonstrateOptions()    // 演示选项模式
}

// ============================================================
// 1. 单例模式（Singleton）
// ============================================================
// 单例模式：确保一个类只有一个实例，并提供全局访问点
//
// Go 中实现单例模式的方式：
// 1. 包级变量（最简单，利用包的初始化机制）
// 2. sync.Once（最推荐，线程安全且只执行一次）
// 3. sync.Mutex + 双重检查（不推荐，Go 中有更好的方式）
//
// sync.Once 是 Go 标准库提供的，保证函数只执行一次
// 内部使用原子操作 + 互斥锁实现，性能好且线程安全

// ConfigManager 配置管理器（单例）
type ConfigManager struct {
	configs map[string]string // 配置项
	mu      sync.RWMutex      // 读写锁，保护并发访问
}

// 包级私有变量，存储单例实例
var configInstance *ConfigManager

// once 用于确保初始化只执行一次
var configOnce sync.Once

// GetConfigManager 获取配置管理器单例
// 使用 sync.Once 保证线程安全的懒汉式单例
func GetConfigManager() *ConfigManager {
	// Do 方法只在第一次调用时执行函数
	// 后续调用直接跳过（原子操作判断，性能很高）
	configOnce.Do(func() {
		fmt.Println("    单例模式：创建 ConfigManager 实例（只创建一次）")
		configInstance = &ConfigManager{
			configs: make(map[string]string),
		}
	})
	return configInstance
}

// Set 设置配置项
func (c *ConfigManager) Set(key, value string) {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.configs[key] = value
}

// Get 获取配置项
func (c *ConfigManager) Get(key string) (string, bool) {
	c.mu.RLock()
	defer c.mu.RUnlock()
	val, ok := c.configs[key]
	return val, ok
}

// demonstrateSingleton 演示单例模式
func demonstrateSingleton() {
	fmt.Println("\n--- 1. 单例模式 Singleton ---")

	var wg sync.WaitGroup

	// 多个 goroutine 同时获取单例，验证只创建一次
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			instance := GetConfigManager()
			instance.Set(fmt.Sprintf("key%d", id), fmt.Sprintf("value%d", id))
			fmt.Printf("    Goroutine %d 获取实例: %p\n", id, instance)
		}(i)
	}

	wg.Wait()

	// 验证所有 goroutine 获取的是同一个实例
	instance1 := GetConfigManager()
	instance2 := GetConfigManager()
	fmt.Printf("  验证单例: instance1 == instance2 ? %v\n", instance1 == instance2)

	// 验证配置数据共享
	if val, ok := instance1.Get("key0"); ok {
		fmt.Printf("  配置共享: key0 = %s\n", val)
	}

	// 单例模式的适用场景：
	// 1. 配置管理器
	// 2. 数据库连接池
	// 3. 日志记录器
	// 4. 线程池
}

// ============================================================
// 2. 工厂模式（Factory）
// ============================================================
// 工厂模式：定义一个创建对象的接口，让子类决定实例化哪个类
// 工厂方法使一个类的实例化延迟到其子类
//
// Go 中没有类和继承，所以使用接口和函数实现工厂模式
// 常见变体：
// - 简单工厂（Simple Factory）：一个函数根据参数创建不同对象
// - 工厂方法（Factory Method）：定义工厂接口，具体工厂实现创建逻辑
// - 抽象工厂（Abstract Factory）：创建一系列相关的对象

// ---------- 产品接口 ----------

// Shape 形状接口（产品接口）
type Shape interface {
	Draw() string // 绘制形状
	GetName() string // 获取形状名称
}

// ---------- 具体产品 ----------

// Circle 圆形（具体产品）
type ShapeCircle struct {
	Radius float64
}

func (c *ShapeCircle) Draw() string {
	return fmt.Sprintf("绘制圆形，半径: %.2f", c.Radius)
}

func (c *ShapeCircle) GetName() string {
	return "圆形"
}

// Rectangle 矩形（具体产品）
type ShapeRectangle struct {
	Width  float64
	Height float64
}

func (r *ShapeRectangle) Draw() string {
	return fmt.Sprintf("绘制矩形，宽: %.2f, 高: %.2f", r.Width, r.Height)
}

func (r *ShapeRectangle) GetName() string {
	return "矩形"
}

// Triangle 三角形（具体产品）
type ShapeTriangle struct {
	Base   float64
	Height float64
}

func (t *ShapeTriangle) Draw() string {
	return fmt.Sprintf("绘制三角形，底: %.2f, 高: %.2f", t.Base, t.Height)
}

func (t *ShapeTriangle) GetName() string {
	return "三角形"
}

// ---------- 简单工厂 ----------

// ShapeType 形状类型枚举
type ShapeType int

const (
	CircleType    ShapeType = iota // 圆形
	RectangleType                  // 矩形
	TriangleType                   // 三角形
)

// ShapeFactory 形状工厂（简单工厂）
// 根据类型创建不同的形状对象
func ShapeFactory(shapeType ShapeType) (Shape, error) {
	switch shapeType {
	case CircleType:
		return &ShapeCircle{Radius: 10}, nil
	case RectangleType:
		return &ShapeRectangle{Width: 20, Height: 10}, nil
	case TriangleType:
		return &ShapeTriangle{Base: 15, Height: 12}, nil
	default:
		return nil, fmt.Errorf("未知的形状类型: %d", shapeType)
	}
}

// ---------- 工厂方法 ----------

// ShapeFactoryInterface 形状工厂接口
// 定义创建形状的方法，具体工厂实现这个接口
type ShapeFactoryInterface interface {
	CreateShape() Shape // 工厂方法：创建形状
}

// CircleFactory 圆形工厂（具体工厂）
type CircleFactory struct {
	Radius float64
}

func (f *CircleFactory) CreateShape() Shape {
	return &ShapeCircle{Radius: f.Radius}
}

// RectangleFactory 矩形工厂（具体工厂）
type RectangleFactory struct {
	Width  float64
	Height float64
}

func (f *RectangleFactory) CreateShape() Shape {
	return &ShapeRectangle{Width: f.Width, Height: f.Height}
}

// demonstrateFactory 演示工厂模式
func demonstrateFactory() {
	fmt.Println("\n--- 2. 工厂模式 Factory ---")

	// ---------- 简单工厂 ----------
	fmt.Println("  简单工厂：")

	// 使用工厂创建不同的形状
	shape1, _ := ShapeFactory(CircleType)
	shape2, _ := ShapeFactory(RectangleType)
	shape3, _ := ShapeFactory(TriangleType)

	fmt.Printf("    %s: %s\n", shape1.GetName(), shape1.Draw())
	fmt.Printf("    %s: %s\n", shape2.GetName(), shape2.Draw())
	fmt.Printf("    %s: %s\n", shape3.GetName(), shape3.Draw())

	// ---------- 工厂方法 ----------
	fmt.Println("\n  工厂方法：")

	// 使用不同的工厂创建不同的产品
	factories := []ShapeFactoryInterface{
		&CircleFactory{Radius: 5},
		&RectangleFactory{Width: 10, Height: 8},
	}

	for _, factory := range factories {
		shape := factory.CreateShape()
		fmt.Printf("    %s: %s\n", shape.GetName(), shape.Draw())
	}

	// 工厂模式的适用场景：
	// 1. 创建对象需要复杂的逻辑
	// 2. 需要灵活替换产品实现
	// 3. 解耦对象的创建和使用
}

// ============================================================
// 3. 观察者模式（Observer）
// ============================================================
// 观察者模式：定义对象间的一对多依赖关系，当一个对象状态改变时，
// 所有依赖它的对象都会收到通知并自动更新
//
// 也叫发布-订阅模式（Publish-Subscribe）
//
// Go 中实现观察者模式的方式：
// 1. 传统方式：注册/注销 + 通知方法
// 2. Channel 方式：利用 Go 的 channel 实现通知（更符合 Go 风格）

// ---------- Channel 实现的观察者模式 ----------

// Event 事件类型
type Event struct {
	Type    string      // 事件类型
	Data    interface{} // 事件数据
	Time    time.Time   // 事件时间
}

// EventBus 事件总线（被观察者/主题）
// 使用 channel 实现观察者模式
type EventBus struct {
	subscribers map[string][]chan Event // 按事件类型存储订阅者的 channel
	mu          sync.RWMutex            // 保护 subscribers 的读写锁
}

// NewEventBus 创建新的事件总线
func NewEventBus() *EventBus {
	return &EventBus{
		subscribers: make(map[string][]chan Event),
	}
}

// Subscribe 订阅事件
// eventType: 要订阅的事件类型
// 返回一个 channel，用于接收事件
func (eb *EventBus) Subscribe(eventType string) <-chan Event {
	eb.mu.Lock()
	defer eb.mu.Unlock()

	// 创建一个带缓冲的 channel 用于接收事件
	ch := make(chan Event, 10)
	// 添加到订阅者列表
	eb.subscribers[eventType] = append(eb.subscribers[eventType], ch)
	return ch
}

// Unsubscribe 取消订阅
func (eb *EventBus) Unsubscribe(eventType string, ch <-chan Event) {
	eb.mu.Lock()
	defer eb.mu.Unlock()

	subs := eb.subscribers[eventType]
	for i, sub := range subs {
		if sub == ch {
			// 移除订阅者
			eb.subscribers[eventType] = append(subs[:i], subs[i+1:]...)
			close(sub) // 关闭 channel
			break
		}
	}
}

// Publish 发布事件
// 向所有订阅了该事件类型的观察者发送事件
func (eb *EventBus) Publish(eventType string, data interface{}) {
	eb.mu.RLock()
	defer eb.mu.RUnlock()

	event := Event{
		Type: eventType,
		Data: data,
		Time: time.Now(),
	}

	// 向所有订阅者发送事件
	// 使用 goroutine 异步发送，避免阻塞发布者
	for _, ch := range eb.subscribers[eventType] {
		go func(c chan Event) {
			// 使用 select 防止 channel 满时阻塞
			select {
			case c <- event:
				// 发送成功
			default:
				// channel 满了，丢弃事件（或记录日志）
				fmt.Println("    观察者模式：channel 已满，丢弃事件")
			}
		}(ch)
	}
}

// demonstrateObserver 演示观察者模式
func demonstrateObserver() {
	fmt.Println("\n--- 3. 观察者模式 Observer ---")

	bus := NewEventBus()

	// 订阅"消息"事件
	sub1 := bus.Subscribe("message")
	sub2 := bus.Subscribe("message")

	// 订阅"通知"事件
	sub3 := bus.Subscribe("notification")

	// 启动观察者 goroutine 处理事件
	var wg sync.WaitGroup

	wg.Add(1)
	go func() {
		defer wg.Done()
		for event := range sub1 {
			fmt.Printf("    观察者1收到[%s]: %v\n", event.Type, event.Data)
		}
	}()

	wg.Add(1)
	go func() {
		defer wg.Done()
		for event := range sub2 {
			fmt.Printf("    观察者2收到[%s]: %v\n", event.Type, event.Data)
		}
	}()

	wg.Add(1)
	go func() {
		defer wg.Done()
		for event := range sub3 {
			fmt.Printf("    观察者3收到[%s]: %v\n", event.Type, event.Data)
		}
	}()

	// 发布事件
	fmt.Println("  发布 'message' 事件 'Hello World'")
	bus.Publish("message", "Hello World")

	time.Sleep(50 * time.Millisecond) // 等待事件传播

	fmt.Println("  发布 'notification' 事件 '系统更新通知'")
	bus.Publish("notification", "系统更新通知")

	time.Sleep(50 * time.Millisecond) // 等待事件传播

	// 取消订阅
	fmt.Println("  观察者2取消订阅 'message'")
	bus.Unsubscribe("message", sub2)

	time.Sleep(50 * time.Millisecond)

	fmt.Println("  再次发布 'message' 事件 '第二条消息'")
	bus.Publish("message", "第二条消息")

	time.Sleep(50 * time.Millisecond)

	// 关闭所有订阅（清理）
	bus.Unsubscribe("message", sub1)
	bus.Unsubscribe("notification", sub3)

	wg.Wait()

	// 观察者模式的适用场景：
	// 1. 事件驱动系统
	// 2. 消息通知系统
	// 3. 数据变化监听
	// 4. MVC 模式中的 Model 和 View 解耦
}

// ============================================================
// 4. 策略模式（Strategy）
// ============================================================
// 策略模式：定义一系列算法，把它们一个个封装起来，并且使它们可以互相替换
// 策略模式让算法的变化独立于使用算法的客户端
//
// 核心思想：将可变化的部分抽象为接口，不同的实现就是不同的策略

// ---------- 策略接口 ----------

// PaymentStrategy 支付策略接口
type PaymentStrategy interface {
	Pay(amount float64) string // 支付方法，返回支付结果描述
	GetName() string           // 获取策略名称
}

// ---------- 具体策略 ----------

// AlipayStrategy 支付宝支付（具体策略）
type AlipayStrategy struct {
	Account string // 支付宝账号
}

func (a *AlipayStrategy) Pay(amount float64) string {
	return fmt.Sprintf("使用支付宝(%s)支付 ¥%.2f", a.Account, amount)
}

func (a *AlipayStrategy) GetName() string {
	return "支付宝"
}

// WeChatPayStrategy 微信支付（具体策略）
type WeChatPayStrategy struct {
	OpenID string // 微信 OpenID
}

func (w *WeChatPayStrategy) Pay(amount float64) string {
	return fmt.Sprintf("使用微信(%s)支付 ¥%.2f", w.OpenID, amount)
}

func (w *WeChatPayStrategy) GetName() string {
	return "微信支付"
}

// CreditCardStrategy 信用卡支付（具体策略）
type CreditCardStrategy struct {
	CardNumber string // 卡号
	Bank       string // 银行
}

func (c *CreditCardStrategy) Pay(amount float64) string {
	return fmt.Sprintf("使用%s信用卡(%s)支付 ¥%.2f", c.Bank, maskCard(c.CardNumber), amount)
}

func (c *CreditCardStrategy) GetName() string {
	return "信用卡"
}

// maskCard 脱敏卡号（只显示后四位）
func maskCard(card string) string {
	if len(card) <= 4 {
		return card
	}
	return "****" + card[len(card)-4:]
}

// ---------- 上下文（使用策略的类）----------

// ShoppingCart 购物车（上下文）
// 购物车使用支付策略来完成支付
type ShoppingCart struct {
	items   []string       // 商品列表
	total   float64        // 总金额
	strategy PaymentStrategy // 当前使用的支付策略
}

// NewShoppingCart 创建购物车
func NewShoppingCart() *ShoppingCart {
	return &ShoppingCart{
		items: make([]string, 0),
	}
}

// AddItem 添加商品
func (sc *ShoppingCart) AddItem(item string, price float64) {
	sc.items = append(sc.items, item)
	sc.total += price
}

// SetPaymentStrategy 设置支付策略
// 可以在运行时动态切换策略
func (sc *ShoppingCart) SetPaymentStrategy(strategy PaymentStrategy) {
	sc.strategy = strategy
}

// Checkout 结账
func (sc *ShoppingCart) Checkout() string {
	if sc.strategy == nil {
		return "请先选择支付方式"
	}
	return sc.strategy.Pay(sc.total)
}

// demonstrateStrategy 演示策略模式
func demonstrateStrategy() {
	fmt.Println("\n--- 4. 策略模式 Strategy ---")

	cart := NewShoppingCart()

	// 添加商品
	cart.AddItem("Go语言圣经", 79.0)
	cart.AddItem("机械键盘", 399.0)
	cart.AddItem("鼠标", 129.0)
	fmt.Printf("  购物车总金额: ¥%.2f\n", cart.total)

	// 使用支付宝支付
	alipay := &AlipayStrategy{Account: "user@example.com"}
	cart.SetPaymentStrategy(alipay)
	fmt.Printf("  %s结账: %s\n", alipay.GetName(), cart.Checkout())

	// 切换到微信支付（策略可以在运行时动态切换）
	wechat := &WeChatPayStrategy{OpenID: "wx_openid_12345"}
	cart.SetPaymentStrategy(wechat)
	fmt.Printf("  %s结账: %s\n", wechat.GetName(), cart.Checkout())

	// 切换到信用卡支付
	creditCard := &CreditCardStrategy{
		CardNumber: "6222021234567890123",
		Bank:       "工商银行",
	}
	cart.SetPaymentStrategy(creditCard)
	fmt.Printf("  %s结账: %s\n", creditCard.GetName(), cart.Checkout())

	// 策略模式的适用场景：
	// 1. 需要在多种算法中动态切换
	// 2. 算法有多种变体，需要灵活扩展
	// 3. 希望将算法的实现与使用分离
	// 4. 避免使用多重条件判断（if-else / switch）
}

// ============================================================
// 5. 装饰器模式（Decorator）
// ============================================================
// 装饰器模式：动态地给一个对象添加一些额外的职责
// 比继承更灵活，可以在运行时动态添加功能
//
// 核心思想：装饰器和被装饰对象实现相同的接口，
// 装饰器持有被装饰对象的引用，在调用前后添加额外功能
//
// Go 中装饰器模式常以函数装饰器的形式出现（函数式编程风格）

// ---------- 接口装饰器（经典装饰器模式）----------

// Coffee 咖啡接口
type Coffee interface {
	GetCost() float64 // 获取价格
	GetDesc() string  // 获取描述
}

// SimpleCoffee 简单咖啡（被装饰的基础对象）
type SimpleCoffee struct{}

func (c *SimpleCoffee) GetCost() float64 {
	return 10.0 // 基础咖啡 10 元
}

func (c *SimpleCoffee) GetDesc() string {
	return "简单咖啡"
}

// MilkDecorator 牛奶装饰器
// 持有 Coffee 接口引用，在基础咖啡上添加牛奶
type MilkDecorator struct {
	coffee Coffee // 被装饰的咖啡
}

func (m *MilkDecorator) GetCost() float64 {
	return m.coffee.GetCost() + 3.0 // 加牛奶 +3 元
}

func (m *MilkDecorator) GetDesc() string {
	return m.coffee.GetDesc() + ", 加牛奶"
}

// SugarDecorator 糖装饰器
type SugarDecorator struct {
	coffee Coffee
}

func (s *SugarDecorator) GetCost() float64 {
	return s.coffee.GetCost() + 1.0 // 加糖 +1 元
}

func (s *SugarDecorator) GetDesc() string {
	return s.coffee.GetDesc() + ", 加糖"
}

// WhipDecorator 奶泡装饰器
type WhipDecorator struct {
	coffee Coffee
}

func (w *WhipDecorator) GetCost() float64 {
	return w.coffee.GetCost() + 2.5 // 加奶泡 +2.5 元
}

func (w *WhipDecorator) GetDesc() string {
	return w.coffee.GetDesc() + ", 加奶泡"
}

// ---------- 函数装饰器（Go 风格）----------

// HandlerFunc 处理函数类型
type HandlerFunc func(string) string

// LoggingDecorator 日志装饰器
// 给处理函数添加日志功能
func LoggingDecorator(handler HandlerFunc) HandlerFunc {
	return func(input string) string {
		fmt.Printf("    [日志] 开始处理，输入: %s\n", input)
		result := handler(input) // 调用原函数
		fmt.Printf("    [日志] 处理完成，输出: %s\n", result)
		return result
	}
}

// TimingDecorator 计时装饰器
// 给处理函数添加计时功能
func TimingDecorator(handler HandlerFunc) HandlerFunc {
	return func(input string) string {
		start := time.Now()
		result := handler(input) // 调用原函数
		elapsed := time.Since(start)
		fmt.Printf("    [计时] 处理耗时: %v\n", elapsed)
		return result
	}
}

// UpperHandler 转大写处理函数
func UpperHandler(input string) string {
	time.Sleep(10 * time.Millisecond) // 模拟处理耗时
	return toUpper(input)
}

// toUpper 简单的转大写函数（不依赖 strings 包，保持示例简洁）
func toUpper(s string) string {
	result := make([]byte, len(s))
	for i := 0; i < len(s); i++ {
		if s[i] >= 'a' && s[i] <= 'z' {
			result[i] = s[i] - 32
		} else {
			result[i] = s[i]
		}
	}
	return string(result)
}

// demonstrateDecorator 演示装饰器模式
func demonstrateDecorator() {
	fmt.Println("\n--- 5. 装饰器模式 Decorator ---")

	// ---------- 接口装饰器 ----------
	fmt.Println("  接口装饰器（咖啡示例）：")

	// 基础咖啡
	coffee := &SimpleCoffee{}
	fmt.Printf("    %s: ¥%.2f\n", coffee.GetDesc(), coffee.GetCost())

	// 加牛奶
	coffeeWithMilk := &MilkDecorator{coffee: coffee}
	fmt.Printf("    %s: ¥%.2f\n", coffeeWithMilk.GetDesc(), coffeeWithMilk.GetCost())

	// 再加糖（装饰器可以嵌套使用）
	coffeeWithMilkAndSugar := &SugarDecorator{coffee: coffeeWithMilk}
	fmt.Printf("    %s: ¥%.2f\n", coffeeWithMilkAndSugar.GetDesc(), coffeeWithMilkAndSugar.GetCost())

	// 再加奶泡
	latte := &WhipDecorator{coffee: coffeeWithMilkAndSugar}
	fmt.Printf("    %s: ¥%.2f\n", latte.GetDesc(), latte.GetCost())

	// ---------- 函数装饰器 ----------
	fmt.Println("\n  函数装饰器（Go 风格）：")

	// 原始处理函数
	fmt.Println("    原始函数：")
	result := UpperHandler("hello world")
	fmt.Printf("    结果: %s\n", result)

	// 应用日志装饰器
	fmt.Println("\n    应用日志装饰器：")
	loggedHandler := LoggingDecorator(UpperHandler)
	result = loggedHandler("hello go")
	fmt.Printf("    结果: %s\n", result)

	// 应用多个装饰器（装饰器可以叠加）
	fmt.Println("\n    应用日志+计时装饰器：")
	decoratedHandler := TimingDecorator(LoggingDecorator(UpperHandler))
	result = decoratedHandler("decorator pattern")
	fmt.Printf("    结果: %s\n", result)

	// 装饰器模式的适用场景：
	// 1. 动态地给对象添加功能
	// 2. 功能可以自由组合
	// 3. 不想通过继承扩展功能（避免类爆炸）
	// 4. Web 中间件（最典型的应用）
}

// ============================================================
// 6. 选项模式（Functional Options）
// ============================================================
// 函数选项模式：使用函数来配置对象
// 通过一系列返回选项函数的函数来配置对象
//
// 这是 Go 中非常流行的一种设计模式
// 优点：
// 1. 支持默认值
// 2. 可扩展（新增选项不破坏现有代码）
// 3. 可读性好
// 4. 顺序无关
//
// 注意：此模式在 advanced 包中也有实现，这里以更完整的例子演示

// Server 服务器配置（选项模式的目标对象）
type Server struct {
	host     string // 主机地址
	port     int    // 端口
	timeout  int    // 超时时间（秒）
	maxConns int    // 最大连接数
	tls      bool   // 是否启用 TLS
	certFile string // 证书文件
	keyFile  string // 密钥文件
	debug    bool   // 调试模式
}

// ServerOption 服务器选项函数类型
// 接收一个 *Server 参数，修改其配置
type ServerOption func(*Server)

// WithHost 设置主机地址
func WithHost(host string) ServerOption {
	return func(s *Server) {
		s.host = host
	}
}

// WithPort 设置端口
func WithPort(port int) ServerOption {
	return func(s *Server) {
		s.port = port
	}
}

// WithTimeout 设置超时
func WithTimeout(timeout int) ServerOption {
	return func(s *Server) {
		s.timeout = timeout
	}
}

// WithMaxConns 设置最大连接数
func WithMaxConns(maxConns int) ServerOption {
	return func(s *Server) {
		s.maxConns = maxConns
	}
}

// WithTLS 启用 TLS
// 选项可以有参数，也可以没有参数
func WithTLS(certFile, keyFile string) ServerOption {
	return func(s *Server) {
		s.tls = true
		s.certFile = certFile
		s.keyFile = keyFile
	}
}

// WithDebug 设置调试模式
func WithDebug(debug bool) ServerOption {
	return func(s *Server) {
		s.debug = debug
	}
}

// NewServer 创建服务器（使用选项模式）
// opts 是可变参数，可以传入任意数量的选项
func NewServer(opts ...ServerOption) *Server {
	// 默认配置
	s := &Server{
		host:     "localhost",
		port:     8080,
		timeout:  30,
		maxConns: 100,
		tls:      false,
		debug:    false,
	}

	// 应用所有选项
	for _, opt := range opts {
		opt(s)
	}

	return s
}

// String 实现 Stringer 接口，方便打印
func (s *Server) String() string {
	tlsStatus := "关闭"
	if s.tls {
		tlsStatus = fmt.Sprintf("开启(cert=%s, key=%s)", s.certFile, s.keyFile)
	}
	return fmt.Sprintf("Server{host:%s, port:%d, timeout:%ds, maxConns:%d, TLS:%s, debug:%t}",
		s.host, s.port, s.timeout, s.maxConns, tlsStatus, s.debug)
}

// demonstrateOptions 演示选项模式
func demonstrateOptions() {
	fmt.Println("\n--- 6. 选项模式 Functional Options ---")

	// 使用默认配置
	server1 := NewServer()
	fmt.Printf("  默认配置: %s\n", server1)

	// 只修改端口
	server2 := NewServer(WithPort(9090))
	fmt.Printf("  自定义端口: %s\n", server2)

	// 修改多个配置（顺序无关）
	server3 := NewServer(
		WithHost("0.0.0.0"),
		WithPort(8443),
		WithTLS("server.crt", "server.key"),
		WithMaxConns(500),
	)
	fmt.Printf("  启用TLS: %s\n", server3)

	// 完整配置
	server4 := NewServer(
		WithHost("192.168.1.100"),
		WithPort(8080),
		WithTimeout(60),
		WithMaxConns(1000),
		WithDebug(true),
	)
	fmt.Printf("  完整配置: %s\n", server4)

	// 选项模式的优点：
	// 1. 支持默认值（不需要传所有参数）
	// 2. 可扩展（新增选项只需加一个 WithXxx 函数）
	// 3. 可读性好（选项名清晰表达含义）
	// 4. 顺序无关（选项可以按任意顺序传入）
	// 5. 向后兼容（新增选项不影响现有调用）
	//
	// 选项模式的适用场景：
	// 1. 构造函数参数太多
	// 2. 参数有默认值
	// 3. 配置项可能会增加
	// 4. 希望 API 保持向后兼容
}
