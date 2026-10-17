// ============================================================
// package concurrency 并发编程模块
// Go 的并发模型基于 CSP（Communicating Sequential Processes）理论
// 核心是 goroutine（轻量级线程）+ channel（通信管道）
// Go 并发哲学："不要通过共享内存来通信，而要通过通信来共享内存"
// ============================================================
package concurrency

import (
	"context"
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// ============================================================
// RunConcurrencyDemo 是并发编程模块的入口函数
// ============================================================
func RunConcurrencyDemo() {
	fmt.Println("\n========== 并发编程演示 ==========")

	demonstrateGoroutine()      // 演示 goroutine
	demonstrateChannel()        // 演示 channel
	demonstrateSelect()         // 演示 select
	demonstrateSync()           // 演示 sync 包
	demonstrateProducerConsumer() // 演示生产者消费者模式
	demonstrateContext()        // 演示 context
	demonstrateConcurrencyPatterns() // 演示并发模式
	demonstrateRaceCondition()  // 演示数据竞争
}

// ============================================================
// 1. Goroutine 基本使用
// ============================================================
// goroutine 是 Go 中并发执行的最小单元
// 使用 go 关键字启动一个 goroutine
// goroutine 非常轻量：
// - 初始栈大小只有 2KB（可按需增长/缩小）
// - 由 Go 运行时调度（不是操作系统线程）
// - 一个程序可以有上万个 goroutine 同时运行

// sayHello 是一个简单的函数，用于在 goroutine 中执行
func sayHello(name string) {
	fmt.Printf("你好，%s！（来自 goroutine）\n", name)
}

// demonstrateGoroutine 演示 goroutine 的基本使用
func demonstrateGoroutine() {
	fmt.Println("\n--- 1. Goroutine 基本使用 ---")

	// 启动一个 goroutine
	// go 关键字后面跟函数调用，就会在新的 goroutine 中执行
	go sayHello("张三")

	// 启动多个 goroutine
	for i := 1; i <= 3; i++ {
		// 注意：循环变量捕获问题！
		// 直接使用 i 可能会导致所有 goroutine 都使用同一个 i（闭包陷阱）
		// 解决方法：将 i 作为参数传入
		go func(n int) {
			fmt.Printf("goroutine %d 正在执行\n", n)
		}(i) // 立即传入当前的 i 值
	}

	// 等待一下，让 goroutine 有机会执行
	// 实际开发中应该使用 sync.WaitGroup（后面会讲）
	time.Sleep(100 * time.Millisecond)
	fmt.Println("主 goroutine 继续执行...")

	// goroutine 的特点：
	// 1. 非阻塞：go 语句立即返回，不等待函数执行完成
	// 2. 调度不确定：goroutine 的执行顺序由调度器决定
	// 3. 轻量级：栈按需增长，初始只有 2KB
	// 4. 没有 ID：goroutine 没有可获取的 ID（Go 故意这样设计，防止依赖 goroutine ID）
}

// ============================================================
// 2. Channel 通道
// ============================================================
// channel 是 goroutine 之间通信的管道
// 类似于 Unix 的管道，用于在 goroutine 之间传递数据
//
// channel 的特性：
// - 发送和接收都是阻塞的（无缓冲 channel）
// - 是类型安全的（每个 channel 只能传输一种类型的数据）
// - 可以是单向的（只能发送或只能接收）
// - 关闭后不可再发送，但可以继续接收剩余数据
//
// 语法：
//   声明: var ch chan int
//   创建: ch = make(chan int)         // 无缓冲
//         ch = make(chan int, 10)     // 有缓冲，容量10
//   发送: ch <- value
//   接收: value := <-ch
//   关闭: close(ch)

// demonstrateChannel 演示 channel 的使用
func demonstrateChannel() {
	fmt.Println("\n--- 2. Channel 通道 ---")

	// ---------- 无缓冲 channel ----------
	// 无缓冲 channel 的发送和接收都是阻塞的
	// 发送方会阻塞，直到有接收方接收
	// 接收方会阻塞，直到有发送方发送
	// 无缓冲 channel 可以用于 goroutine 之间的同步
	fmt.Println("  无缓冲 channel：")

	ch := make(chan int) // 创建无缓冲的 int channel

	// 启动一个 goroutine 发送数据
	go func() {
		fmt.Println("    发送者: 准备发送数据...")
		ch <- 42 // 发送数据到 channel（会阻塞，直到有人接收）
		fmt.Println("    发送者: 数据发送完成")
	}()

	time.Sleep(50 * time.Millisecond) // 让发送者先运行
	fmt.Println("    接收者: 准备接收数据...")
	value := <-ch // 从 channel 接收数据（会阻塞，直到有数据）
	fmt.Printf("    接收者: 接收到数据: %d\n", value)

	// ---------- 有缓冲 channel ----------
	// 有缓冲 channel 有一个缓冲区
	// 缓冲区未满时，发送不会阻塞
	// 缓冲区非空时，接收不会阻塞
	fmt.Println("\n  有缓冲 channel：")

	bufferedCh := make(chan string, 3) // 创建容量为 3 的有缓冲 channel

	// 向有缓冲 channel 发送数据（不会阻塞，因为缓冲区未满）
	bufferedCh <- "消息1"
	bufferedCh <- "消息2"
	bufferedCh <- "消息3"
	fmt.Printf("    发送了3条消息，len=%d, cap=%d\n",
		len(bufferedCh), cap(bufferedCh))

	// 从有缓冲 channel 接收数据
	fmt.Printf("    接收: %s\n", <-bufferedCh)
	fmt.Printf("    接收: %s\n", <-bufferedCh)
	fmt.Printf("    接收后 len=%d\n", len(bufferedCh))

	// ---------- 单向 channel ----------
	// 可以将 channel 限制为只能发送或只能接收
	// 通常用于函数参数，确保函数不会误用 channel
	fmt.Println("\n  单向 channel：")

	msgCh := make(chan string, 2)
	// sendOnly 只能发送，recvOnly 只能接收
	sendMessage(msgCh, "单向channel消息")
	receiveMessage(msgCh)

	// ---------- 关闭 channel ----------
	// 使用 close() 关闭 channel
	// 关闭后：
	// - 不能再发送（会 panic）
	// - 可以继续接收剩余数据
	// - 数据接收完后，再接收会得到零值且不阻塞
	fmt.Println("\n  关闭 channel：")

	closeCh := make(chan int, 3)
	closeCh <- 1
	closeCh <- 2
	closeCh <- 3
	close(closeCh) // 关闭 channel

	// 接收已关闭的 channel（会先接收完所有数据）
	for i := 0; i < 5; i++ {
		val, ok := <-closeCh // ok 为 false 表示 channel 已关闭且无数据
		fmt.Printf("    接收: val=%d, ok=%t\n", val, ok)
		if !ok {
			break
		}
	}

	// ---------- for-range 遍历 channel ----------
	// for-range 会一直从 channel 接收数据，直到 channel 被关闭
	fmt.Println("\n  for-range 遍历 channel：")

	rangeCh := make(chan int)
	go func() {
		for i := 1; i <= 5; i++ {
			rangeCh <- i * 10
		}
		close(rangeCh) // 发送完后关闭，否则 range 会一直阻塞
	}()

	fmt.Print("    遍历结果: ")
	for val := range rangeCh {
		fmt.Printf("%d ", val)
	}
	fmt.Println()
}

// sendMessage 只发送的 channel
// 参数类型 chan<- string 表示只能向 channel 发送，不能接收
func sendMessage(ch chan<- string, msg string) {
	ch <- msg // 只能发送
	fmt.Printf("    发送函数: 已发送 \"%s\"\n", msg)
}

// receiveMessage 只接收的 channel
// 参数类型 <-chan string 表示只能从 channel 接收，不能发送
func receiveMessage(ch <-chan string) {
	msg := <-ch // 只能接收
	fmt.Printf("    接收函数: 收到 \"%s\"\n", msg)
}

// ============================================================
// 3. select 语句
// ============================================================
// select 用于在多个 channel 操作中进行选择
// 类似于 switch，但每个 case 都是 channel 操作
// 规则：
// 1. 所有 case 中的 channel 操作都会被评估
// 2. 如果有多个 case 可以执行，随机选择一个
// 3. 如果没有 case 可以执行：
//    - 有 default：执行 default（非阻塞）
//    - 没有 default：阻塞等待
// 4. nil channel 永远不会被选中

// demonstrateSelect 演示 select 语句
func demonstrateSelect() {
	fmt.Println("\n--- 3. Select 语句 ---")

	// ---------- 基本用法 ----------
	fmt.Println("  基本用法：")

	ch1 := make(chan string)
	ch2 := make(chan string)

	// 启动两个 goroutine，分别向两个 channel 发送数据
	go func() {
		time.Sleep(100 * time.Millisecond)
		ch1 <- "来自 channel 1 的数据"
	}()

	go func() {
		time.Sleep(150 * time.Millisecond)
		ch2 <- "来自 channel 2 的数据"
	}()

	// 使用 select 等待多个 channel
	// 哪个 channel 先有数据，就执行哪个 case
	select {
	case msg := <-ch1:
		fmt.Printf("    收到: %s\n", msg)
	case msg := <-ch2:
		fmt.Printf("    收到: %s\n", msg)
	}

	// ---------- timeout 超时模式 ----------
	fmt.Println("\n  超时模式：")

	timeoutCh := make(chan string)
	go func() {
		time.Sleep(200 * time.Millisecond)
		timeoutCh <- "处理完成"
	}()

	// 使用 time.After 实现超时
	// time.After 返回一个 channel，指定时间后会发送一个值
	select {
	case result := <-timeoutCh:
		fmt.Printf("    正常完成: %s\n", result)
	case <-time.After(100 * time.Millisecond):
		fmt.Println("    超时！（100ms 内没有收到数据）")
	}

	// ---------- default 非阻塞 ----------
	fmt.Println("\n  default 非阻塞：")

	nonBlockCh := make(chan int)
	// 没有数据时，select 会执行 default（不会阻塞）
	select {
	case val := <-nonBlockCh:
		fmt.Printf("    收到数据: %d\n", val)
	default:
		fmt.Println("    没有数据，执行 default（非阻塞）")
	}

	// ---------- select + for 循环 ----------
	fmt.Println("\n  select + for 循环：")

	tick := time.Tick(50 * time.Millisecond) // 定时触发器
	boom := time.After(160 * time.Millisecond) // 爆炸定时器
	count := 0

loop: // 标签，用于跳出外层 for
	for {
		select {
		case <-tick:
			count++
			fmt.Printf("    tick %d\n", count)
		case <-boom:
			fmt.Println("    Boom! 时间到")
			break loop // 跳出 for 循环（使用标签）
		}
	}

	// ---------- nil channel 的作用 ----------
	// nil channel 的发送和接收都会永远阻塞
	// 在 select 中，nil channel 的 case 永远不会被选中
	// 可以用来"禁用"某个 case
	fmt.Println("\n  nil channel 禁用 case：")

	var nilCh chan int // nil channel
	activeCh := make(chan string, 1)
	activeCh <- "有效数据"

	select {
	case v := <-nilCh: // 永远不会被选中
		fmt.Printf("    nilCh: %d\n", v)
	case v := <-activeCh: // 会被选中
		fmt.Printf("    activeCh: %s\n", v)
	}
}

// ============================================================
// 4. sync 包
// ============================================================
// sync 包提供了多种同步原语：
// - WaitGroup: 等待一组 goroutine 完成
// - Mutex: 互斥锁（排他锁）
// - RWMutex: 读写锁（读共享，写排他）
// - Once: 确保某个函数只执行一次
// - Cond: 条件变量（较少使用，推荐用 channel）

// demonstrateSync 演示 sync 包的使用
func demonstrateSync() {
	fmt.Println("\n--- 4. sync 包 ---")

	// ---------- WaitGroup ----------
	// WaitGroup 用于等待一组 goroutine 完成
	// 三个方法：
	// - Add(n): 增加/减少计数（n 为正增加，为负减少）
	// - Done(): 减少计数（等价于 Add(-1)）
	// - Wait(): 阻塞直到计数为 0
	fmt.Println("  WaitGroup：")

	var wg sync.WaitGroup

	for i := 1; i <= 3; i++ {
		wg.Add(1) // 增加计数（在 goroutine 外调用更安全）
		go func(n int) {
			defer wg.Done() // goroutine 结束时减少计数（用 defer 确保一定会执行）
			fmt.Printf("    Worker %d 开始工作...\n", n)
			time.Sleep(50 * time.Millisecond)
			fmt.Printf("    Worker %d 完成工作\n", n)
		}(i)
	}

	fmt.Println("    主 goroutine 等待所有 worker 完成...")
	wg.Wait() // 阻塞，直到计数为 0
	fmt.Println("    所有 worker 都完成了！")

	// ---------- Mutex 互斥锁 ----------
	// Mutex 是互斥锁，用于保护共享资源
	// 两个方法：Lock() 和 Unlock()
	// Lock() 会阻塞直到获得锁
	// Unlock() 释放锁
	// 同一个 goroutine 不能重复 Lock（会死锁）
	fmt.Println("\n  Mutex 互斥锁：")

	var mu sync.Mutex
	counter := 0

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			mu.Lock()   // 加锁（获得锁之前会阻塞）
			counter++   // 临界区：访问共享资源
			mu.Unlock() // 解锁（必须在退出临界区后调用）
		}()
	}
	wg.Wait()
	fmt.Printf("    计数器结果: %d (正确应为 1000)\n", counter)

	// ---------- RWMutex 读写锁 ----------
	// RWMutex 是读写分离的锁
	// - 多个读操作可以同时持有读锁（共享）
	// - 写操作需要排他的写锁
	// 适用场景：读多写少
	// 方法：RLock()/RUnlock() 读锁，Lock()/Unlock() 写锁
	fmt.Println("\n  RWMutex 读写锁：")

	var rwMu sync.RWMutex
	data := make(map[string]int)
	data["key1"] = 100
	data["key2"] = 200

	// 读操作：使用 RLock/RUnlock
	readData := func(key string) int {
		rwMu.RLock()         // 加读锁（多个 goroutine 可以同时加读锁）
		defer rwMu.RUnlock() // 释放读锁
		return data[key]
	}

	// 写操作：使用 Lock/Unlock
	writeData := func(key string, val int) {
		rwMu.Lock()         // 加写锁（排他，需要等待所有读锁释放）
		defer rwMu.Unlock() // 释放写锁
		data[key] = val
	}

	// 启动多个 reader
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(n int) {
			defer wg.Done()
			val := readData("key1")
			fmt.Printf("    Reader %d: key1 = %d\n", n, val)
		}(i)
	}

	// 启动一个 writer
	wg.Add(1)
	go func() {
		defer wg.Done()
		writeData("key1", 999)
		fmt.Println("    Writer: 更新了 key1 = 999")
	}()

	wg.Wait()

	// ---------- Once ----------
	// sync.Once 确保某个函数只执行一次
	// 常用于单例模式、初始化操作等
	// 方法：Do(func())
	fmt.Println("\n  sync.Once：")

	var once sync.Once
	onceBody := func() {
		fmt.Println("    这个函数只会执行一次！")
	}

	// 启动多个 goroutine 同时调用 once.Do
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(n int) {
			defer wg.Done()
			once.Do(onceBody) // 只有第一个调用会执行函数
			fmt.Printf("    Goroutine %d 通过了 once.Do\n", n)
		}(i)
	}
	wg.Wait()
}

// ============================================================
// 5. 生产者消费者模式
// ============================================================
// 生产者消费者模式是经典的并发模式
// 生产者：生成数据放入 channel
// 消费者：从 channel 取出数据处理
// channel 作为缓冲区，解耦生产者和消费者

// demonstrateProducerConsumer 演示生产者消费者模式
func demonstrateProducerConsumer() {
	fmt.Println("\n--- 5. 生产者消费者模式 ---")

	// 创建一个有缓冲的 channel 作为队列
	queue := make(chan int, 5)
	var wg sync.WaitGroup

	// ---------- 生产者 ----------
	// 生产者向 channel 中发送数据
	producer := func(id int, count int) {
		defer wg.Done()
		for i := 1; i <= count; i++ {
			item := id*100 + i
			fmt.Printf("    生产者%d 生产: %d\n", id, item)
			queue <- item
			time.Sleep(30 * time.Millisecond)
		}
		fmt.Printf("    生产者%d 生产完毕\n", id)
	}

	// ---------- 消费者 ----------
	// 消费者从 channel 中取出数据处理
	consumer := func(id int) {
		defer wg.Done()
		for item := range queue { // 从 channel 接收，直到 channel 关闭
			fmt.Printf("    消费者%d 消费: %d\n", id, item)
			time.Sleep(50 * time.Millisecond)
		}
		fmt.Printf("    消费者%d 退出\n", id)
	}

	// 启动 2 个生产者
	wg.Add(2)
	go producer(1, 3)
	go producer(2, 3)

	// 等待所有生产者完成，然后关闭 channel
	// 注意：不能在生产者 goroutine 内部关闭，因为有多个生产者
	go func() {
		wg.Wait() // 等待所有生产者完成
		close(queue) // 关闭 channel，通知消费者没有更多数据了
		fmt.Println("    所有生产者完成，channel 已关闭")
	}()

	// 启动 3 个消费者
	wg.Add(3)
	go consumer(1)
	go consumer(2)
	go consumer(3)

	wg.Wait() // 等待所有消费者完成
	fmt.Println("    所有消费者完成")
}

// ============================================================
// 6. context 上下文控制
// ============================================================
// context 包用于在 goroutine 之间传递取消信号、超时、截止时间和请求范围的值
// context 是并发安全的，可以在多个 goroutine 中共享
//
// 主要类型：
// - context.Context: 上下文接口
// - context.Background(): 根上下文（空上下文，不能取消）
// - context.TODO(): 当不确定用什么 context 时使用
//
// 主要函数：
// - context.WithCancel(): 创建可取消的上下文
// - context.WithTimeout(): 创建带超时的上下文
// - context.WithDeadline(): 创建带截止时间的上下文
// - context.WithValue(): 创建带值的上下文

// demonstrateContext 演示 context 的使用
func demonstrateContext() {
	fmt.Println("\n--- 6. Context 上下文控制 ---")

	// ---------- WithCancel 取消 ----------
	fmt.Println("  WithCancel 取消：")

	ctx, cancel := context.WithCancel(context.Background())
	var wg sync.WaitGroup

	wg.Add(1)
	go func(ctx context.Context) {
		defer wg.Done()
		for {
			select {
			case <-ctx.Done(): // 收到取消信号
				fmt.Println("    Worker 收到取消信号，退出")
				return
			default:
				fmt.Println("    Worker 正在工作...")
				time.Sleep(50 * time.Millisecond)
			}
		}
	}(ctx)

	time.Sleep(150 * time.Millisecond)
	fmt.Println("    主 goroutine 发送取消信号")
	cancel() // 发送取消信号
	wg.Wait()

	// ---------- WithTimeout 超时 ----------
	fmt.Println("\n  WithTimeout 超时：")

	// 创建一个 100ms 后自动超时的 context
	ctxTimeout, cancelTimeout := context.WithTimeout(context.Background(), 100*time.Millisecond)
	defer cancelTimeout() // 释放资源

	wg.Add(1)
	go func(ctx context.Context) {
		defer wg.Done()
		select {
		case <-time.After(200 * time.Millisecond):
			fmt.Println("    任务正常完成（不会发生，因为超时了）")
		case <-ctx.Done():
			fmt.Printf("    任务超时退出: %v\n", ctx.Err())
		}
	}(ctxTimeout)

	wg.Wait()

	// ---------- WithValue 传值 ----------
	fmt.Println("\n  WithValue 传值：")

	// 使用 WithValue 在 context 中存储请求范围的值
	// 注意：不要用 WithValue 传递可选参数，只用于传递请求范围的元数据
	type ctxKey string // 自定义 key 类型，避免冲突
	key := ctxKey("userID")

	ctxValue := context.WithValue(context.Background(), key, "user_12345")

	wg.Add(1)
	go func(ctx context.Context) {
		defer wg.Done()
		// 从 context 中取值
		if userID, ok := ctx.Value(key).(string); ok {
			fmt.Printf("    从 context 中获取 userID: %s\n", userID)
		}
	}(ctxValue)

	wg.Wait()
}

// ============================================================
// 7. 并发模式
// ============================================================
// 常见的并发模式：扇入、扇出、工作池、pipeline 等

// demonstrateConcurrencyPatterns 演示常见并发模式
func demonstrateConcurrencyPatterns() {
	fmt.Println("\n--- 7. 并发模式 ---")

	// ---------- 扇出（Fan-out）----------
	// 一个输入 channel，多个 worker goroutine 并行处理
	fmt.Println("  扇出模式（Fan-out）：")

	input := make(chan int, 10)
	var wg sync.WaitGroup

	// 启动 3 个 worker 并行处理（扇出）
	for i := 1; i <= 3; i++ {
		wg.Add(1)
		go func(workerID int) {
			defer wg.Done()
			for num := range input {
				result := num * num // 计算平方
				fmt.Printf("    Worker %d 处理: %d -> %d\n", workerID, num, result)
			}
		}(i)
	}

	// 发送任务
	for i := 1; i <= 6; i++ {
		input <- i
	}
	close(input) // 关闭输入 channel
	wg.Wait()    // 等待所有 worker 完成

	// ---------- 扇入（Fan-in）----------
	// 多个输入 channel，合并到一个输出 channel
	fmt.Println("\n  扇入模式（Fan-in）：")

	ch1 := make(chan string)
	ch2 := make(chan string)

	// 启动两个生产者
	go func() {
		for i := 1; i <= 3; i++ {
			ch1 <- fmt.Sprintf("来源A-%d", i)
		}
		close(ch1)
	}()

	go func() {
		for i := 1; i <= 3; i++ {
			ch2 <- fmt.Sprintf("来源B-%d", i)
		}
		close(ch2)
	}()

	// 扇入函数：将多个 channel 合并为一个
	fanIn := func(c1, c2 <-chan string) <-chan string {
		out := make(chan string)
		var wg sync.WaitGroup
		wg.Add(2)

		// 从 c1 读取
		go func() {
			defer wg.Done()
			for v := range c1 {
				out <- v
			}
		}()

		// 从 c2 读取
		go func() {
			defer wg.Done()
			for v := range c2 {
				out <- v
			}
		}()

		// 两个输入都完成后关闭输出
		go func() {
			wg.Wait()
			close(out)
		}()

		return out
	}

	merged := fanIn(ch1, ch2)
	fmt.Print("    合并结果: ")
	for v := range merged {
		fmt.Printf("%s ", v)
	}
	fmt.Println()

	// ---------- 工作池（Worker Pool）----------
	fmt.Println("\n  工作池模式（Worker Pool）：")

	const numWorkers = 3
	const numJobs = 10

	jobs := make(chan int, numJobs)
	results := make(chan int, numJobs)

	// 启动 worker
	for w := 1; w <= numWorkers; w++ {
		wg.Add(1)
		go func(workerID int) {
			defer wg.Done()
			for job := range jobs {
				fmt.Printf("    Worker %d 开始任务 %d\n", workerID, job)
				time.Sleep(30 * time.Millisecond) // 模拟工作
				results <- job * 2                // 结果
				fmt.Printf("    Worker %d 完成任务 %d\n", workerID, job)
			}
		}(w)
	}

	// 发送任务
	for j := 1; j <= numJobs; j++ {
		jobs <- j
	}
	close(jobs) // 关闭任务 channel

	// 等待所有 worker 完成，然后关闭结果 channel
	go func() {
		wg.Wait()
		close(results)
	}()

	// 收集结果
	fmt.Print("    所有结果: ")
	for r := range results {
		fmt.Printf("%d ", r)
	}
	fmt.Println()

	// ---------- Pipeline 流水线 ----------
	fmt.Println("\n  Pipeline 流水线模式：")

	// 生成数字
	generate := func(nums ...int) <-chan int {
		out := make(chan int)
		go func() {
			for _, n := range nums {
				out <- n
			}
			close(out)
		}()
		return out
	}

	// 计算平方
	square := func(in <-chan int) <-chan int {
		out := make(chan int)
		go func() {
			for n := range in {
				out <- n * n
			}
			close(out)
		}()
		return out
	}

	// 打印结果
	gen := generate(1, 2, 3, 4, 5)
	sq := square(gen)

	fmt.Print("    流水线结果: ")
	for v := range sq {
		fmt.Printf("%d ", v)
	}
	fmt.Println()
}

// ============================================================
// 8. Race Condition 数据竞争
// ============================================================
// 数据竞争：多个 goroutine 并发访问同一块内存，
// 并且至少有一个是写操作，且没有同步机制。
//
// 数据竞争会导致未定义行为，是并发编程中最常见的 bug。
//
// 检测方法：
//   go run -race <file.go>
//
// 解决方法：
// 1. 互斥锁（sync.Mutex）
// 2. 原子操作（sync/atomic）
// 3. Channel（通过通信共享内存）

// demonstrateRaceCondition 演示数据竞争及解决方案
func demonstrateRaceCondition() {
	fmt.Println("\n--- 8. 数据竞争 ---")

	// ---------- 有数据竞争的版本 ----------
	fmt.Println("  数据竞争示例（无同步保护）：")

	var counter int64 = 0
	var wg sync.WaitGroup

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			// 这行代码有数据竞争：
			// counter++ 不是原子操作，它包含读-修改-写三步
			// 多个 goroutine 同时执行会导致结果不正确
			counter++
		}()
	}
	wg.Wait()
	fmt.Printf("    计数器结果: %d (期望 1000，实际可能不正确)\n", counter)
	// 注意：由于数据竞争，每次运行结果可能不同
	// 使用 go run -race 可以检测到数据竞争

	// ---------- 解决方案1：互斥锁 ----------
	fmt.Println("\n  解决方案1：互斥锁 sync.Mutex")

	var mu sync.Mutex
	var counter2 int64 = 0

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			mu.Lock()   // 加锁，确保只有一个 goroutine 能访问
			counter2++  // 临界区
			mu.Unlock() // 解锁
		}()
	}
	wg.Wait()
	fmt.Printf("    互斥锁结果: %d (正确应为 1000)\n", counter2)

	// ---------- 解决方案2：原子操作 ----------
	fmt.Println("\n  解决方案2：原子操作 sync/atomic")

	var counter3 int64 = 0

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			// atomic.AddInt64 是原子操作，CPU 指令级别保证原子性
			atomic.AddInt64(&counter3, 1)
		}()
	}
	wg.Wait()
	fmt.Printf("    原子操作结果: %d (正确应为 1000)\n", atomic.LoadInt64(&counter3))

	// ---------- 解决方案3：Channel ----------
	fmt.Println("\n  解决方案3：Channel（通过通信共享内存）")

	incrementCh := make(chan int, 1000)
	done := make(chan int)

	// 一个 goroutine 专门负责计数（串行化访问）
	go func() {
		count := 0
		for range incrementCh {
			count++
		}
		done <- count
	}()

	// 多个 goroutine 发送"加1"请求
	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			incrementCh <- 1 // 发送一个增量信号
		}()
	}
	wg.Wait()
	close(incrementCh) // 关闭 channel，通知计数器 goroutine

	result := <-done // 等待结果
	fmt.Printf("    Channel结果: %d (正确应为 1000)\n", result)

	// 三种方案的选择：
	// 1. 互斥锁：通用、简单，适合复杂临界区
	// 2. 原子操作：性能最高，但只支持简单操作（加减、交换等）
	// 3. Channel：最符合 Go 哲学，适合 goroutine 间协作
}
