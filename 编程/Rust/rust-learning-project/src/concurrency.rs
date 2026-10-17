// ============================================================
// concurrency.rs - 并发编程模块
// ============================================================
// Rust的并发编程以"无畏并发"（Fearless Concurrency）著称
// 编译器通过所有权系统和类型系统在编译时保证并发安全
//
// Rust并发的核心概念：
//   - 线程（Thread）：操作系统级别的并发执行单元
//   - 消息传递（Message Passing）：通过通道（channel）通信
//   - 共享状态（Shared State）：通过互斥锁（Mutex）和原子引用计数（Arc）
//   - Send trait：允许在线程间转移所有权
//   - Sync trait：允许在线程间共享引用
//
// "无畏并发"的含义：
//   - 很多并发错误在编译时就能被发现，而不是运行时
//   - 数据竞争（Data Race）在Rust中是不可能的（编译时保证）
// ============================================================

use std::sync::mpsc;      // 多生产者单消费者通道（multi-producer, single-consumer）
use std::sync::{Arc, Mutex};  // Arc：原子引用计数；Mutex：互斥锁
use std::thread;           // 线程模块
use std::time::Duration;   // 时间间隔

/// 并发编程模块的入口演示函数
pub fn run_concurrency_demo() {
    println!("\n{}", "=".repeat(60));
    println!("并发编程演示");
    println!("{}", "=".repeat(60));

    thread_demo();
    message_passing_demo();
    shared_state_demo();
    send_sync_demo();
    arc_demo();
    fearless_concurrency_demo();
}

// ============================================================
// 1. 线程（spawn / join）
// ============================================================
// 使用 std::thread::spawn 创建新线程
// spawn 接受一个闭包作为参数，在新线程中执行
//
// join() 方法：等待子线程结束，返回线程的返回值
// 如果不调用 join，主线程结束时子线程可能还没执行完就被终止
//
// move 闭包：将环境中变量的所有权转移到闭包中
// 常用于线程，确保线程拥有数据的所有权（生命周期安全）

/// 线程演示函数
fn thread_demo() {
    println!("\n--- 1. 线程（spawn / join）---");

    // ---- 创建线程 ----
    // thread::spawn 创建一个新线程，返回 JoinHandle
    // JoinHandle 是一个所有权值，可以调用 join() 等待线程结束
    let handle = thread::spawn(|| {
        // 这是在新线程中执行的代码
        for i in 1..=5 {
            println!("子线程: 第{}次循环", i);
            // 让线程休眠一段时间，模拟耗时操作
            thread::sleep(Duration::from_millis(100));
        }
    });

    // 主线程继续执行
    for i in 1..=3 {
        println!("主线程: 第{}次循环", i);
        thread::sleep(Duration::from_millis(150));
    }

    // ---- 等待线程结束 ----
    // join() 会阻塞当前线程，直到子线程执行完毕
    // 返回 Result<T>：线程正常结束返回 Ok(返回值)，panic 返回 Err
    handle.join().unwrap();
    println!("子线程已结束");

    // ---- 使用 move 闭包转移所有权 ----
    // 线程闭包通常需要 move 关键字来获取环境中变量的所有权
    // 这样可以确保线程使用的数据在线程执行期间一直有效
    let v = vec![1, 2, 3];

    let handle = thread::spawn(move || {
        // move 闭包将 v 的所有权转移到线程中
        // 这样就不用担心 v 在线程结束前被释放
        println!("子线程拥有向量: {:?}", v);
    });

    // 主线程不能再使用 v 了（所有权已转移）
    // println!("{:?}", v);  // 错误：value borrowed here after move

    handle.join().unwrap();
    println!("move 闭包演示完成");

    // ---- 线程返回值 ----
    // join() 返回的 Result 中包含线程的返回值
    let handle = thread::spawn(|| {
        let mut sum = 0;
        for i in 1..=10 {
            sum += i;
        }
        sum  // 线程返回值
    });

    let result = handle.join().unwrap();
    println!("线程计算结果: 1+2+...+10 = {}", result);  // 55

    println!("线程基础：");
    println!("  - thread::spawn 创建线程");
    println!("  - join() 等待线程结束");
    println!("  - move 闭包转移所有权，保证安全");
}

// ============================================================
// 2. 消息传递（channel / mpsc）
// ============================================================
// 消息传递是Rust并发的重要模式之一
// "不要通过共享内存来通信；而是通过通信来共享内存"
//
// 通道（Channel）：
//   - 通道有两个端点：发送端（transmitter）和接收端（receiver）
//   - 发送端：发送消息（调用 send 方法）
//   - 接收端：接收消息（调用 recv 方法或迭代）
//
// mpsc：multi-producer, single-consumer
//   - 多个发送端，一个接收端
//   - 可以通过 clone() 发送端来实现多生产者
//
// 通道是异步的：send 不会阻塞，recv 会阻塞直到有消息或通道关闭

/// 消息传递演示函数
fn message_passing_demo() {
    println!("\n--- 2. 消息传递（channel / mpsc）---");

    // ---- 创建通道 ----
    // mpsc::channel() 返回 (发送端, 接收端) 元组
    // tx：transmitter（发送端）
    // rx：receiver（接收端）
    let (tx, rx) = mpsc::channel();

    // ---- 在另一个线程中发送消息 ----
    thread::spawn(move || {
        let val = String::from("hello");
        // send 方法发送消息
        // send 返回 Result<T>，如果接收端已关闭则返回 Err
        tx.send(val).unwrap();

        // send 会取得值的所有权，发送后 val 不能再使用
        // println!("val = {}", val);  // 错误：value borrowed here after move
    });

    // ---- 接收消息 ----
    // recv 方法会阻塞当前线程，直到收到消息或通道关闭
    // 返回 Result<T>：收到消息返回 Ok(value)，通道关闭返回 Err
    let received = rx.recv().unwrap();
    println!("收到消息: {}", received);  // "hello"

    // ---- 发送多个消息 ----
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let values = vec![
            String::from("你好"),
            String::from("来自"),
            String::from("子线程"),
        ];

        for val in values {
            tx.send(val).unwrap();
            thread::sleep(Duration::from_millis(200));
        }
    });

    // 接收端可以当作迭代器使用
    println!("接收多个消息:");
    for received in rx {
        println!("  收到: {}", received);
    }
    // 当发送端被丢弃（线程结束）时，迭代器自动结束

    // ---- 多生产者（clone 发送端）----
    let (tx, rx) = mpsc::channel();

    // 第一个发送者
    let tx1 = tx.clone();
    thread::spawn(move || {
        tx1.send(String::from("来自线程1的消息")).unwrap();
        thread::sleep(Duration::from_millis(100));
        tx1.send(String::from("线程1第二条消息")).unwrap();
    });

    // 第二个发送者
    let tx2 = tx.clone();
    thread::spawn(move || {
        tx2.send(String::from("来自线程2的消息")).unwrap();
        thread::sleep(Duration::from_millis(100));
        tx2.send(String::from("线程2第二条消息")).unwrap();
    });

    // 原始的 tx 也需要丢弃（否则通道不会关闭，recv 会一直等待）
    drop(tx);

    println!("多生产者消息:");
    for received in rx {
        println!("  收到: {}", received);
    }

    println!("消息传递：");
    println!("  - mpsc 通道：多生产者，单消费者");
    println!("  - tx.send() 发送，rx.recv() 接收");
    println!("  - 消息所有权从发送端转移到接收端");
    println!("  - 通过通信来共享内存，而非共享内存来通信");
}

// ============================================================
// 3. 共享状态（Mutex / Arc）
// ============================================================
// 有时需要在多个线程之间共享状态（共享内存）
// Mutex（互斥锁，mutual exclusion）：
//   - 保证同一时间只有一个线程能访问数据
//   - 线程必须先获取锁，才能访问数据
//   - 使用完毕后释放锁
//
// Mutex<T> 的使用：
//   - Mutex::new(data) 创建互斥锁
//   - lock() 获取锁，返回 LockResult<MutexGuard<T>>
//   - MutexGuard 是智能指针，离开作用域时自动释放锁
//
// Arc<T>（原子引用计数，Atomically Reference Counted）：
//   - 线程安全的 Rc<T>
//   - 可以在多个线程之间共享所有权
//   - 使用 clone() 增加引用计数
//
// 注意：Mutex 提供内部可变性（类似 RefCell），
// 结合 Arc 可以实现多线程共享可变数据

/// 共享状态演示函数
fn shared_state_demo() {
    println!("\n--- 3. 共享状态（Mutex / Arc）---");

    // ---- Mutex 的基本用法 ----
    // Mutex<T> 是一个智能指针，包裹着数据
    let m = Mutex::new(5);

    {
        // lock() 获取锁，返回 MutexGuard
        // MutexGuard 实现了 Deref，可以像普通引用一样使用
        // MutexGuard 实现了 Drop，离开作用域时自动释放锁
        let mut num = m.lock().unwrap();
        *num = 6;  // 修改互斥锁中的数据
        println!("Mutex 中的值: {}", num);
    }  // 在这里，MutexGuard 离开作用域，自动释放锁

    println!("释放锁后，Mutex 中的值: {}", m.lock().unwrap());

    // ---- Arc<Mutex<T>>：多线程共享可变数据 ----
    // Arc 提供线程安全的共享所有权
    // Mutex 提供线程安全的内部可变性
    // 两者结合：Arc<Mutex<T>> 是多线程共享可变数据的经典模式

    // 创建一个计数器，用 Arc<Mutex<i32>> 包装
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];  // 存储所有线程的 JoinHandle

    // 创建 10 个线程，每个线程将计数器加 1
    for _ in 0..10 {
        // clone Arc，增加引用计数
        // 每个线程都有一个指向同一个 Mutex 的 Arc 指针
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            // 获取锁并修改数据
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    // 等待所有线程结束
    for handle in handles {
        handle.join().unwrap();
    }

    // 查看最终结果
    println!("10个线程各加1后的结果: {}", *counter.lock().unwrap());
    // 结果应该是 10（如果有数据竞争，结果可能小于10）
    // Rust保证：只要正确使用 Mutex，就不会有数据竞争

    // ---- 死锁（Deadlock）----
    // 死锁是指两个或多个线程互相等待对方释放锁
    // Rust不能完全防止死锁（这是算法问题，不是类型系统问题）
    // 但 Mutex 的设计使死锁更容易被发现和避免
    println!("注意：Mutex 不能防止死锁，需要注意锁的顺序");

    println!("共享状态：");
    println!("  - Mutex<T>：互斥锁，保证同一时间只有一个线程访问");
    println!("  - Arc<T>：原子引用计数，线程安全的共享所有权");
    println!("  - Arc<Mutex<T>>：多线程共享可变数据的经典模式");
    println!("  - lock() 获取锁，drop时自动释放");
}

// ============================================================
// 4. Send 与 Sync trait
// ============================================================
// Send 和 Sync 是 Rust 并发安全的基石
// 它们是标记 trait（marker trait），没有方法，只是标记类型的性质
//
// Send trait：
//   - 实现了 Send 的类型，其所有权可以在线程间转移
//   - 大部分 Rust 类型都是 Send 的
//   - 例外：Rc<T>（非原子引用计数，线程不安全），裸指针等
//
// Sync trait：
//   - 实现了 Sync 的类型，可以安全地在多个线程中共享引用（即 &T 是 Send）
//   - 换句话说：多个线程可以同时拥有该类型的不可变引用，而不会产生数据竞争
//   - 基本类型、Mutex<T>、RwLock<T> 等是 Sync 的
//   - 例外：RefCell<T>（运行时借用检查，线程不安全），Rc<T> 等
//
// 关系：
//   - 如果 T: Sync，那么 &T: Send
//   - 组合类型如果所有成员都是 Send/Sync，那么整体也是 Send/Sync
//
// 注意：手动实现 Send 和 Sync 是不安全的（unsafe）
// 绝大多数情况下，编译器会自动推导，不需要手动实现

/// Send 与 Sync trait 演示函数
fn send_sync_demo() {
    println!("\n--- 4. Send 与 Sync trait ---");

    // ---- Send trait ----
    // Send：类型的所有权可以安全地在线程间转移
    // i32 实现了 Send，所以可以在线程间转移
    let x = 5;
    let handle = thread::spawn(move || {
        println!("在线程中使用 Send 类型: {}", x);
    });
    handle.join().unwrap();

    // Rc<T> 没有实现 Send，不能在线程间转移
    // 下面的代码会编译错误：
    // use std::rc::Rc;
    // let rc = Rc::new(5);
    // thread::spawn(move || {
    //     println!("{}", rc);
    // });
    // 错误：`Rc<i32>` cannot be sent between threads safely

    // ---- Sync trait ----
    // Sync：类型可以安全地被多个线程共享引用（&T 是 Send）
    // i32 实现了 Sync，所以多个线程可以同时引用同一个 i32
    let x = 5;
    let ref1 = &x;
    let ref2 = &x;
    println!("Sync 类型可以被多个引用共享: ref1={}, ref2={}", ref1, ref2);

    // Mutex<T> 实现了 Sync（即使 T 不是 Sync，只要 T: Send）
    // 这就是为什么 Arc<Mutex<T>> 可以在多线程中共享
    let mutex = Mutex::new(5);
    let ref1 = mutex.lock().unwrap();
    drop(ref1);  // 释放锁
    let ref2 = mutex.lock().unwrap();
    drop(ref2);
    println!("Mutex<T> 是 Sync 的，可以安全地跨线程共享");

    // ---- Send 和 Sync 的关系 ----
    // Send：所有权可以跨线程转移
    // Sync：引用可以跨线程共享
    // 如果 T: Sync，则 &T: Send
    // 换句话说：可以安全地将 T 的引用发送到另一个线程

    println!("Send 与 Sync：");
    println!("  - Send：所有权可以在线程间转移");
    println!("  - Sync：引用可以在线程间共享");
    println!("  - 大多数类型都是 Send + Sync 的");
    println!("  - Rc/RefCell 不是，Arc/Mutex 是");
    println!("  - 编译器自动推导，通常不需要手动实现");
}

// ============================================================
// 5. 线程安全的 Rc（Arc）
// ============================================================
// Rc<T>（引用计数）：单线程内的共享所有权
//   - 非线程安全，引用计数操作不是原子的
//   - 没有实现 Send trait
//
// Arc<T>（原子引用计数）：多线程间的共享所有权
//   - 线程安全，引用计数操作是原子的
//   - 实现了 Send 和 Sync trait
//   - 有轻微的性能开销（原子操作比普通操作慢一点）
//
// 选择原则：
//   - 单线程内共享：用 Rc<T>（性能更好）
//   - 多线程间共享：用 Arc<T>（线程安全）
//
// Arc 只能共享不可变数据
// 如果需要共享可变数据，使用 Arc<Mutex<T>> 或 Arc<RwLock<T>>

/// Arc 演示函数
fn arc_demo() {
    println!("\n--- 5. 线程安全的 Rc（Arc）---");

    // ---- Arc 的基本用法 ----
    // Arc::new() 创建一个新的 Arc
    let a = Arc::new(5);
    println!("初始 Arc: {}", a);
    println!("引用计数: {}", Arc::strong_count(&a));  // 1

    // clone() 增加引用计数
    let b = Arc::clone(&a);
    println!("clone 后引用计数: {}", Arc::strong_count(&a));  // 2
    println!("b = {}", b);

    {
        let c = Arc::clone(&a);
        println!("再 clone 后引用计数: {}", Arc::strong_count(&a));  // 3
        // c 离开作用域，引用计数减 1
    }
    println!("c 离开作用域后引用计数: {}", Arc::strong_count(&a));  // 2

    // ---- Arc 在多线程中的使用 ----
    // 多个线程共享同一个只读数据
    let shared_data = Arc::new(vec![1, 2, 3, 4, 5]);
    let mut handles = vec![];

    for i in 0..3 {
        // 每个线程 clone 一个 Arc，增加引用计数
        let data = Arc::clone(&shared_data);
        let handle = thread::spawn(move || {
            println!("线程{}: 共享数据 = {:?}", i, data);
            println!("线程{}: 数据和 = {}", i, data.iter().sum::<i32>());
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("所有线程结束后引用计数: {}", Arc::strong_count(&shared_data));

    // ---- Arc<Mutex<T>> 模式 ----
    // 共享可变数据的标准模式
    println!("\nArc<Mutex<T>> 模式：");
    println!("  - Arc 提供共享所有权");
    println!("  - Mutex 提供内部可变性和线程安全");
    println!("  - 两者结合实现多线程共享可变数据");

    // ---- Arc vs Rc 对比 ----
    println!("\nArc vs Rc：");
    println!("  - Rc：单线程，非原子操作，性能好");
    println!("  - Arc：多线程，原子操作，轻微开销");
    println!("  - 根据场景选择，不要盲目用 Arc");
}

// ============================================================
// 6. 无畏并发概念
// ============================================================
// "无畏并发"（Fearless Concurrency）是Rust的核心卖点之一
// 含义：Rust在编译时就能防止很多并发bug
//
// Rust如何保证并发安全：
//   1. 所有权系统：防止数据竞争（Data Race）
//   2. Send/Sync trait：标记哪些类型是线程安全的
//   3. 借用检查器：防止悬垂引用和不正确的共享
//
// 数据竞争（Data Race）的三个条件：
//   1. 两个或以上的指针同时访问同一内存
//   2. 至少有一个指针在写入
//   3. 没有使用同步机制
//
// Rust的保证：只要代码是 safe Rust（不用 unsafe），就不会有数据竞争
// 其他并发问题（死锁、活锁、饥饿等）Rust不保证，但数据竞争是最常见的bug

/// 无畏并发概念演示函数
fn fearless_concurrency_demo() {
    println!("\n--- 6. 无畏并发概念 ---");

    println!("什么是无畏并发？");
    println!("  - 编译时防止数据竞争等并发bug");
    println!("  - 不用等到运行时才发现问题");
    println!("  - 编译器是你的并发安全助手");

    println!("\nRust并发安全的三大支柱：");
    println!("  1. 所有权系统");
    println!("     - 同一时间只能有一个可变引用或多个不可变引用");
    println!("     - 从根本上防止数据竞争");
    println!("  2. Send trait");
    println!("     - 标记哪些类型的所有权可以跨线程转移");
    println!("     - 编译器检查，确保不在线程间传递不安全的类型");
    println!("  3. Sync trait");
    println!("     - 标记哪些类型可以跨线程共享引用");
    println!("     - 防止共享不安全的类型");

    println!("\n数据竞争（Data Race）的三个条件：");
    println!("  1. 两个或以上指针同时访问同一数据");
    println!("  2. 至少有一个在写入");
    println!("  3. 没有同步机制");
    println!("  Rust保证：safe Rust 中不可能出现数据竞争！");

    println!("\n并发编程的两种风格：");
    println!("  1. 消息传递（Message Passing）");
    println!("     - 线程通过通道通信");
    println!("     - 不共享内存，通过通信来共享内存");
    println!("     - Go语言的CSP模型类似");
    println!("  2. 共享状态（Shared State）");
    println!("     - 线程通过锁访问共享数据");
    println!("     - 传统多线程编程模式");
    println!("     - Rust通过类型系统保证安全");

    println!("\nRust并发最佳实践：");
    println!("  - 优先使用消息传递（channel）");
    println!("  - 需要共享状态时，使用 Arc<Mutex<T>>");
    println!("  - 尽量减少锁的持有时间");
    println!("  - 注意锁的顺序，避免死锁");
    println!("  - 相信编译器，它会帮你发现问题");

    // ---- 一个完整的并发例子 ----
    println!("\n--- 综合示例：并行计算 ---");

    let numbers: Vec<i32> = (1..=100).collect();
    let chunk_size = 25;
    let data = Arc::new(numbers);
    let mut handles = vec![];

    for i in 0..4 {
        let data = Arc::clone(&data);
        let start = i * chunk_size;
        let end = start + chunk_size;
        let handle = thread::spawn(move || {
            let slice = &data[start..end];
            let sum: i32 = slice.iter().sum();
            println!("线程{}: 计算[{}..{})的和 = {}", i, start, end, sum);
            sum
        });
        handles.push(handle);
    }

    let total: i32 = handles
        .into_iter()
        .map(|h| h.join().unwrap())
        .sum();

    println!("并行计算总和: 1+2+...+100 = {}", total);
    // 验证：1+2+...+100 = 5050
    assert_eq!(total, 5050);

    println!("\n并发编程模块演示完毕！");
    println!("核心要点：");
    println!("  - thread::spawn 创建线程，join 等待");
    println!("  - 消息传递：mpsc 通道");
    println!("  - 共享状态：Arc<Mutex<T>>");
    println!("  - Send/Sync 标记线程安全");
    println!("  - 编译时保证无数据竞争：无畏并发");
}
