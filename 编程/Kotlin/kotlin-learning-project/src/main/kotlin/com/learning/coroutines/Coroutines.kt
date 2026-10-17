// ============================================================
// 包声明
// ============================================================
package com.learning.coroutines

// 协程核心库导入
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.sync.Mutex
import kotlinx.coroutines.sync.withLock
import kotlin.concurrent.thread

// ============================================================
// 协程演示
// 包含：协程基础、协程作用域、调度器、挂起函数、取消与超时、
//      协程上下文与Job、结构化并发、通道、流、并发安全
// ============================================================

// ============================================================
// 1. 协程基础（launch/runBlocking/async/await）
// ============================================================
suspend fun coroutineBasicsDemo() {
    println("=== 1. 协程基础 ===")

    // --- runBlocking ---
    // runBlocking会阻塞当前线程，等待内部所有协程完成
    // 通常用于main函数或测试中，桥接阻塞世界和协程世界
    println("runBlocking开始...")
    runBlocking {
        // launch：启动一个新协程，不返回结果（类似"放火就忘"）
        // 返回Job对象，可以用于控制协程
        val job = launch {
            println("  launch协程开始执行")
            delay(100)  // 挂起函数：非阻塞地等待100毫秒
            println("  launch协程执行完成")
        }

        println("  launch已启动，继续执行...")

        // async：启动一个新协程，返回Deferred（带结果的Job）
        // 使用await()获取结果
        val deferred = async {
            println("  async协程开始执行")
            delay(200)
            println("  async协程执行完成")
            42  // 返回结果
        }

        println("  async已启动，等待结果...")
        val result = deferred.await()  // 挂起等待结果
        println("  async结果：$result")

        job.join()  // 等待launch完成
    }
    println("runBlocking结束")

    // --- 协程 vs 线程的轻量性对比 ---
    println("\n协程的轻量性：")
    runBlocking {
        // 启动10万个协程，每个延迟1秒，内存占用很小
        val jobs = List(1000) { i ->  // 这里演示用1000个，避免耗时太长
            launch {
                delay(1000)
                // 每个协程做一点点工作
                if (i % 200 == 0) print(".")
            }
        }
        println("\n启动了${jobs.size}个协程，等待完成...")
        jobs.forEach { it.join() }
        println("\n所有协程完成！")
    }
}

// ============================================================
// 2. 协程作用域 CoroutineScope
// 协程必须在作用域中启动，作用域管理协程的生命周期
// ============================================================
suspend fun coroutineScopeDemo() {
    println("\n=== 2. 协程作用域 CoroutineScope ===")

    // --- GlobalScope ---
    // 全局作用域：生命周期与应用程序一样长，不推荐日常使用
    // 因为没有结构化并发，取消时不会自动取消子协程
    println("GlobalScope启动的协程（不受作用域管理）：")
    val globalJob = GlobalScope.launch {
        delay(500)
        println("  GlobalScope协程完成")
    }
    globalJob.join()

    // --- coroutineScope ---
    // 创建一个新的作用域，等待所有子协程完成后才完成
    // 结构化并发：一个子协程失败，所有兄弟协程都会被取消
    println("\ncoroutineScope结构化并发：")
    coroutineScope {
        launch {
            delay(100)
            println("  子协程1完成")
        }
        launch {
            delay(200)
            println("  子协程2完成")
        }
        println("  coroutineScope内启动了两个子协程")
    }
    println("  coroutineScope的所有子协程都完成了")

    // --- 自定义作用域 ---
    println("\n自定义CoroutineScope：")
    val scope = CoroutineScope(Dispatchers.Default)  // 使用默认调度器
    val customJob = scope.launch {
        println("  自定义作用域中的协程")
        delay(100)
        println("  自定义作用域协程完成")
    }
    customJob.join()
    // scope.cancel()  // 可以取消整个作用域的所有协程
}

// ============================================================
// 3. 调度器 Dispatchers
// 调度器决定协程在哪个线程或线程池上执行
// ============================================================
suspend fun dispatchersDemo() {
    println("\n=== 3. 调度器 Dispatchers ===")

    runBlocking {
        // --- Dispatchers.Default ---
        // 默认调度器：使用共享的线程池，线程数等于CPU核心数
        // 适合CPU密集型任务（计算、排序等）
        launch(Dispatchers.Default) {
            println("  Default调度器：线程=${Thread.currentThread().name}")
            // 模拟CPU密集型计算
            var sum = 0
            for (i in 1..10000) sum += i
        }

        // --- Dispatchers.IO ---
        // IO调度器：用于IO密集型任务（网络、文件、数据库等）
        // 按需创建线程，有更多的线程
        launch(Dispatchers.IO) {
            println("  IO调度器：线程=${Thread.currentThread().name}")
            // 模拟IO操作
            delay(100)
        }

        // --- Dispatchers.Unconfined ---
        // 不受限调度器：在调用者线程开始执行，恢复时在哪个线程不确定
        // 不推荐在生产代码中使用，特殊情况才用
        launch(Dispatchers.Unconfined) {
            println("  Unconfined调度器（挂起前）：线程=${Thread.currentThread().name}")
            delay(10)
            println("  Unconfined调度器（挂起后）：线程=${Thread.currentThread().name}")
        }

        // --- 继承调度器 ---
        // launch不传参数时，继承父协程的调度器和上下文
        launch {
            println("  继承父协程调度器：线程=${Thread.currentThread().name}")
        }

        // --- withContext ---
        // 切换协程的上下文（调度器），挂起等待结果
        val result = withContext(Dispatchers.Default) {
            // 在Default调度器中执行
            println("  withContext切换到Default：线程=${Thread.currentThread().name}")
            "计算结果"
        }
        println("  withContext返回：$result，当前线程=${Thread.currentThread().name}")

        delay(300)  // 等待所有协程完成
    }
}

// ============================================================
// 4. suspend 挂起函数
// 挂起函数只能在协程或其他挂起函数中调用
// 挂起不会阻塞线程，只是暂停协程的执行
// ============================================================

// 定义一个挂起函数：模拟网络请求
suspend fun fetchUserData(userId: Int): String {
    // delay是挂起函数，模拟网络延迟
    delay(200)
    return "用户$userId 的数据"
}

// 另一个挂起函数：模拟数据库查询
suspend fun queryUserFromDB(userId: Int): String {
    delay(150)
    return "DB中用户$userId 的记录"
}

// 挂起函数可以组合使用
suspend fun getUserCompleteInfo(userId: Int): String {
    // 串行执行：先调用一个，再调用另一个
    val networkData = fetchUserData(userId)
    val dbData = queryUserFromDB(userId)
    return "$networkData | $dbData"
}

suspend fun suspendFunctionDemo() {
    println("\n=== 4. 挂起函数 suspend ===")

    runBlocking {
        println("开始获取用户信息（串行）...")
        val startTime = System.currentTimeMillis()
        val info = getUserCompleteInfo(123)
        val endTime = System.currentTimeMillis()
        println("用户信息：$info")
        println("耗时：${endTime - startTime} 毫秒（串行）")

        // 使用async并发执行
        println("\n开始获取用户信息（并行）...")
        val startTime2 = System.currentTimeMillis()
        val networkDeferred = async { fetchUserData(456) }
        val dbDeferred = async { queryUserFromDB(456) }
        val info2 = "${networkDeferred.await()} | ${dbDeferred.await()}"
        val endTime2 = System.currentTimeMillis()
        println("用户信息：$info2")
        println("耗时：${endTime2 - startTime2} 毫秒（并行）")
    }
}

// ============================================================
// 5. 协程取消与超时
// ============================================================
suspend fun cancellationDemo() {
    println("\n=== 5. 协程取消与超时 ===")

    runBlocking {
        // --- 取消协程 ---
        println("--- 取消协程 ---")
        val job = launch {
            repeat(10) { i ->
                println("  协程执行中：第 $i 次")
                delay(100)  // delay会检查取消状态
            }
        }

        delay(250)  // 等待250毫秒
        println("取消协程...")
        job.cancel()  // 取消协程
        job.join()    // 等待协程完全取消
        println("协程已取消")

        // --- 不能取消的计算 ---
        println("\n--- 计算密集型协程的取消（需要检查isActive） ---")
        val cpuJob = launch(Dispatchers.Default) {
            var nextPrintTime = System.currentTimeMillis()
            var i = 0
            // 对于CPU密集型任务，需要主动检查isActive
            while (isActive) {  // isActive是协程的状态属性
                if (System.currentTimeMillis() >= nextPrintTime) {
                    println("  计算中：i = ${i++}")
                    nextPrintTime += 200
                }
            }
            println("  计算协程被取消了")
        }

        delay(700)
        println("取消计算协程...")
        cpuJob.cancel()
        cpuJob.join()

        // --- 超时 ---
        println("\n--- withTimeout 超时 ---")
        try {
            withTimeout(300) {  // 300毫秒超时
                repeat(10) { i ->
                    println("  超时测试：第 $i 次")
                    delay(100)
                }
            }
        } catch (e: TimeoutCancellationException) {
            println("  捕获到超时异常：${e.message}")
        }

        // withTimeoutOrNull：超时时返回null而不是抛异常
        println("\n--- withTimeoutOrNull ---")
        val result = withTimeoutOrNull(300) {
            delay(500)  // 需要500毫秒，但超时是300毫秒
            "完成了"
        }
        println("  withTimeoutOrNull结果：$result")  // null
    }
}

// ============================================================
// 6. 协程上下文与Job
// ============================================================
suspend fun contextAndJobDemo() {
    println("\n=== 6. 协程上下文与Job ===")

    runBlocking {
        // --- Job ---
        // Job代表协程的生命周期，可以取消、等待、查询状态
        val job = launch {
            println("  协程启动")
            delay(200)
            println("  协程完成")
        }

        println("  Job是否活跃：${job.isActive}")
        println("  Job是否完成：${job.isCompleted}")
        println("  Job是否取消：${job.isCancelled}")

        job.join()
        println("  join后，Job是否完成：${job.isCompleted}")

        // --- 父子协程关系 ---
        println("\n--- 父子协程 ---")
        val parentJob = launch {
            println("  父协程启动")

            // 子协程继承父协程的上下文
            val childJob = launch {
                println("    子协程启动")
                delay(200)
                println("    子协程完成")
            }

            println("  子协程Job：$childJob")
            childJob.join()
            println("  父协程完成")
        }
        parentJob.join()

        // --- 取消父协程会取消所有子协程 ---
        println("\n--- 取消父协程 ---")
        val parentJob2 = launch {
            launch {
                repeat(10) {
                    println("    子协程1：第 $it 次")
                    delay(100)
                }
            }
            launch {
                repeat(10) {
                    println("    子协程2：第 $it 次")
                    delay(100)
                }
            }
        }

        delay(250)
        println("  取消父协程...")
        parentJob2.cancel()
        parentJob2.join()
        println("  父协程已取消，子协程也都取消了")
    }
}

// ============================================================
// 7. 结构化并发
// ============================================================
suspend fun structuredConcurrencyDemo() {
    println("\n=== 7. 结构化并发 ===")

    runBlocking {
        // 结构化并发的核心原则：
        // 1. 子协程在父协程的作用域内启动
        // 2. 父协程等待所有子协程完成
        // 3. 父协程取消，所有子协程也取消
        // 4. 子协程失败，父协程也失败（并取消其他子协程）

        println("结构化并发示例：")
        println("coroutineScope会等待所有子协程完成")

        val result = coroutineScope {
            val deferred1 = async {
                delay(100)
                println("  异步任务1完成")
                10
            }
            val deferred2 = async {
                delay(200)
                println("  异步任务2完成")
                20
            }
            deferred1.await() + deferred2.await()
        }
        println("所有子协程完成，结果：$result")

        // 异常传播：一个子协程失败，其他子协程会被取消
        println("\n异常传播演示：")
        try {
            coroutineScope {
                launch {
                    try {
                        delay(200)
                        println("  协程1：我应该不会完成（因为兄弟协程失败了）")
                    } catch (e: CancellationException) {
                        println("  协程1：被取消了")
                    }
                }
                launch {
                    delay(100)
                    println("  协程2：我要抛出异常了！")
                    throw RuntimeException("协程2出错了")
                }
            }
        } catch (e: RuntimeException) {
            println("捕获到异常：${e.message}")
        }
    }
}

// ============================================================
// 8. 通道 Channel
// 通道用于协程之间的通信（类似BlockingQueue，但非阻塞）
// ============================================================
suspend fun channelDemo() {
    println("\n=== 8. 通道 Channel ===")

    runBlocking {
        // --- 基础通道 ---
        println("--- 基础通道 ---")
        val channel = Channel<Int>()

        // 生产者协程：发送数据
        launch {
            for (i in 1..5) {
                println("  发送：$i")
                channel.send(i)  // send是挂起函数，通道满了会挂起
                delay(50)
            }
            channel.close()  // 关闭通道，表示发送完毕
        }

        // 消费者协程：接收数据
        launch {
            // 遍历通道，直到通道关闭
            for (value in channel) {
                println("  接收：$value")
            }
            println("  通道已关闭，接收完毕")
        }.join()

        // --- 带缓冲的通道 ---
        println("\n--- 带缓冲的通道 ---")
        val bufferedChannel = Channel<Int>(capacity = 3)  // 容量为3

        launch {
            for (i in 1..5) {
                println("  发送到缓冲通道：$i")
                bufferedChannel.send(i)
            }
            bufferedChannel.close()
        }

        delay(200)  // 等一下让发送者先填充缓冲区
        launch {
            for (value in bufferedChannel) {
                println("  从缓冲通道接收：$value")
                delay(100)
            }
        }.join()

        // --- produce构造器 ---
        println("\n--- produce 生产者 ---")
        // produce：方便地创建一个生产数据的协程，返回ReceiveChannel
        val numbersChannel = produce {
            for (i in 1..5) {
                send(i * i)  // 发送平方数
                delay(30)
            }
        }

        numbersChannel.consumeEach {
            println("  消费平方数：$it")
        }
    }
}

// ============================================================
// 9. 流 Flow
// Flow是冷流：只有被收集时才开始发射数据
// 类似序列，但是异步的（挂起函数）
// ============================================================
suspend fun flowDemo() {
    println("\n=== 9. 流 Flow ===")

    runBlocking {
        // --- 创建流 ---
        println("--- 创建简单流 ---")

        // flow { ... } 构造器
        val simpleFlow = flow {
            println("  流开始发射数据")
            for (i in 1..3) {
                delay(100)  // 异步操作
                emit(i)     // 发射一个值
            }
            println("  流发射完毕")
        }

        // 收集流（终端操作，触发流执行）
        println("  开始收集流...")
        simpleFlow.collect { value ->
            println("  收集到：$value")
        }

        // 流是冷的：每次收集都会重新执行
        println("\n  再次收集流...")
        simpleFlow.collect { value ->
            println("  第二次收集到：$value")
        }

        // --- 流操作符 ---
        println("\n--- 流操作符 ---")

        val numbersFlow = (1..10).asFlow()  // 将集合转为流

        // map：转换
        numbersFlow
            .map { it * it }  // 平方
            .filter { it > 10 }  // 过滤大于10的
            .collect { println("  map+filter结果：$it") }

        // transform：更灵活的转换
        (1..5).asFlow()
            .transform { value ->
                emit("变换前：$value")
                emit("变换后：${value * 2}")
            }
            .collect { println("  transform：$it") }

        // take：取前n个
        (1..100).asFlow()
            .take(3)
            .collect { println("  take(3)：$it") }

        // reduce/fold：聚合
        val sum = (1..5).asFlow()
            .reduce { accumulator, value -> accumulator + value }
        println("  reduce求和：$sum")

        // --- flowOn：改变流的上下文（调度器） ---
        println("\n--- flowOn 切换线程 ---")
        flow {
            println("  流发射线程：${Thread.currentThread().name}")
            emit(1)
            emit(2)
        }
            .flowOn(Dispatchers.Default)  // 上游在Default调度器
            .collect { value ->
                println("  收集线程：${Thread.currentThread().name}，值：$value")
            }

        // --- StateFlow 和 SharedFlow（简单介绍） ---
        println("\n--- StateFlow ---")
        // StateFlow是状态流，有一个当前值，collector会收到最新值和后续更新
        // 类似LiveData，但更强大
    }
}

// ============================================================
// 10. 并发安全（Mutex/actor）
// 协程在多线程调度时也会有并发问题，需要同步机制
// ============================================================
suspend fun concurrencySafetyDemo() {
    println("\n=== 10. 并发安全 Mutex ===")

    runBlocking {
        // --- 没有同步的情况：竞态条件 ---
        println("--- 竞态条件（无同步） ---")
        var counter = 0
        val jobs = List(100) {
            launch(Dispatchers.Default) {
                repeat(1000) {
                    counter++  // 非原子操作，会有竞态条件
                }
            }
        }
        jobs.forEach { it.join() }
        println("  期望结果：100000，实际结果：$counter")

        // --- 使用Mutex互斥锁 ---
        println("\n--- Mutex 互斥锁 ---")
        val mutex = Mutex()
        var safeCounter = 0

        val safeJobs = List(100) {
            launch(Dispatchers.Default) {
                repeat(1000) {
                    // withLock：自动加锁和解锁（挂起函数，不会阻塞线程）
                    mutex.withLock {
                        safeCounter++
                    }
                }
            }
        }
        safeJobs.forEach { it.join() }
        println("  期望结果：100000，实际结果：$safeCounter")

        // --- actor模式 ---
        // actor是一个协程+通道的组合，通过消息传递来管理共享状态
        // 避免了显式锁，是协程中推荐的并发安全方式
        println("\n--- Actor 模式 ---")

        // 定义消息类型
        sealed class CounterMessage
        object Increment : CounterMessage()
        object Decrement : CounterMessage()
        class GetValue(val response: CompletableDeferred<Int>) : CounterMessage()

        // 创建actor
        val counterActor = actor<CounterMessage> {
            var count = 0  // actor内部状态，只有actor自己能访问
            for (msg in channel) {
                when (msg) {
                    is Increment -> count++
                    is Decrement -> count--
                    is GetValue -> msg.response.complete(count)
                }
            }
        }

        // 使用actor
        val actorJobs = List(100) {
            launch(Dispatchers.Default) {
                repeat(1000) {
                    counterActor.send(Increment)
                }
            }
        }
        actorJobs.forEach { it.join() }

        // 获取最终值
        val deferred = CompletableDeferred<Int>()
        counterActor.send(GetValue(deferred))
        val finalCount = deferred.await()
        println("  期望结果：100000，实际结果：$finalCount")

        counterActor.close()
    }
}

// ============================================================
// 主演示函数
// ============================================================
fun runCoroutinesDemo() {
    println("============================================================")
    println("                  Kotlin 协程演示")
    println("============================================================")

    // 使用runBlocking作为协程的入口点
    runBlocking {
        coroutineBasicsDemo()         // 协程基础
        coroutineScopeDemo()          // 协程作用域
        dispatchersDemo()             // 调度器
        suspendFunctionDemo()         // 挂起函数
        cancellationDemo()            // 取消与超时
        contextAndJobDemo()           // 上下文与Job
        structuredConcurrencyDemo()   // 结构化并发
        channelDemo()                 // 通道
        flowDemo()                    // 流
        concurrencySafetyDemo()       // 并发安全
    }

    println("\n============================================================")
    println("                  协程演示结束")
    println("============================================================")
}
