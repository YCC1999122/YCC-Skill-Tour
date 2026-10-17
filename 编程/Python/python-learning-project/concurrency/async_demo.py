# -*- coding: utf-8 -*-
"""
================================================================================
文件名：async_demo.py
所属模块：concurrency（并发编程）
功能说明：本文件演示 Python 三种并发编程方式：多线程、多进程、协程，
         并对比它们的特点和适用场景。

并发（Concurrency）：多个任务在宏观上同时进行（微观上可能交替执行）
并行（Parallelism）：多个任务在微观上真正同时执行（需要多核 CPU）

Python 并发编程的三种方式：
  1. 多线程（threading）：轻量，受 GIL 限制，适合 IO 密集型任务
  2. 多进程（multiprocessing）：真正并行，适合 CPU 密集型任务，开销大
  3. 协程（asyncio）：单线程内的并发，开销极小，适合高并发 IO 任务

GIL（全局解释器锁）：CPython 中的一把互斥锁，确保同一时刻只有一个线程
执行 Python 字节码。这使得多线程在 CPU 密集型任务上无法真正并行。

运行方式：在项目根目录下执行 `python -m concurrency.async_demo`
================================================================================
"""

import time           # 导入 time 模块，用于计时和休眠
import threading      # 导入 threading 模块，多线程编程
import multiprocessing  # 导入 multiprocessing 模块，多进程编程
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor  # 线程池/进程池
import asyncio        # 导入 asyncio 模块，异步 IO/协程


# ==============================================================================
# 第一部分：多线程（threading）
# ==============================================================================
# 线程是操作系统调度的最小单位，一个进程可以包含多个线程
# 同一进程的多个线程共享内存空间
#
# Python 中的多线程：
#   - 优点：轻量，创建开销小，共享内存方便通信
#   - 缺点：受 GIL 限制，CPU 密集型任务无法真正并行
#   - 适用场景：IO 密集型任务（网络请求、文件读写、数据库查询等）
# ==============================================================================

def io_task(name, duration):
    """
    函数名：io_task
    功能：模拟一个 IO 密集型任务（用 sleep 模拟等待）
    参数：
        name - 任务名称
        duration - 任务持续时间（秒）
    返回值：任务结果字符串
    说明：IO 密集型任务大部分时间在等待（如等待网络响应、等待磁盘IO），
          这时候 CPU 是空闲的，多线程可以利用等待时间执行其他任务。
    """
    print(f"   [线程 {threading.current_thread().name}] 任务 {name} 开始，预计耗时 {duration} 秒")
    time.sleep(duration)  # 用 sleep 模拟 IO 等待（此时线程会释放 GIL）
    print(f"   [线程 {threading.current_thread().name}] 任务 {name} 完成")
    return f"{name} 的结果"


def cpu_task(name, n):
    """
    函数名：cpu_task
    功能：模拟一个 CPU 密集型任务（大量计算）
    参数：
        name - 任务名称
        n - 计算规模（计算 n 以内的质数个数）
    返回值：质数个数
    说明：CPU 密集型任务持续占用 CPU，由于 GIL 的存在，
          Python 多线程在这类任务上无法真正并行。
    """
    print(f"   [线程 {threading.current_thread().name}] 任务 {name} 开始计算...")
    count = 0
    for num in range(2, n):
        is_prime = True
        for i in range(2, int(num ** 0.5) + 1):
            if num % i == 0:
                is_prime = False
                break
        if is_prime:
            count += 1
    print(f"   [线程 {threading.current_thread().name}] 任务 {name} 完成，质数个数：{count}")
    return count


def demo_threading():
    """
    函数名：demo_threading
    功能：演示多线程编程
    参数：无
    返回值：无
    """
    print("=" * 60)
    print("【一、多线程（threading）】")
    print("=" * 60)

    # ---------- 1. 基本用法：创建和启动线程 ----------
    print("\n1. 基本用法：创建和启动线程")

    # 记录开始时间
    start = time.time()

    # 创建线程对象
    # target: 线程要执行的函数
    # args: 传递给函数的参数（元组形式）
    # name: 线程名称
    t1 = threading.Thread(target=io_task, args=("任务A", 2), name="Thread-1")
    t2 = threading.Thread(target=io_task, args=("任务B", 3), name="Thread-2")

    # 启动线程（调用 start() 后线程才开始运行）
    print("   启动两个线程：")
    t1.start()
    t2.start()

    # 等待线程结束（join() 会阻塞当前线程，直到目标线程结束）
    t1.join()
    t2.join()

    elapsed = time.time() - start
    print(f"   总耗时：{elapsed:.2f} 秒（串行需要 2+3=5 秒，并发后约 3 秒）")

    # ---------- 2. 线程池（ThreadPoolExecutor）----------
    print("\n2. 线程池（ThreadPoolExecutor）：")
    print("   使用线程池执行 5 个 IO 任务：")

    start = time.time()

    # 使用 with 语句，确保线程池被正确关闭
    # max_workers: 最大线程数
    with ThreadPoolExecutor(max_workers=3) as executor:
        # submit 提交任务，返回 Future 对象
        futures = []
        for i in range(5):
            future = executor.submit(io_task, f"任务{i+1}", 1)
            futures.append(future)

        # 获取结果
        for i, future in enumerate(futures):
            result = future.result()  # 阻塞等待结果
            print(f"   收到 {result}")

    elapsed = time.time() - start
    print(f"   总耗时：{elapsed:.2f} 秒（5个任务，每个1秒，3个线程，约2秒）")

    # ---------- 3. IO 密集 vs CPU 密集 ----------
    print("\n3. IO 密集型 vs CPU 密集型（GIL 影响）：")

    print("   IO 密集型任务（多线程有效）：")
    start = time.time()
    with ThreadPoolExecutor(max_workers=2) as executor:
        executor.submit(io_task, "IO1", 1)
        executor.submit(io_task, "IO2", 1)
    elapsed_io = time.time() - start
    print(f"   耗时：{elapsed_io:.2f} 秒")

    print("\n   CPU 密集型任务（GIL 导致多线程效果差）：")
    start = time.time()
    with ThreadPoolExecutor(max_workers=2) as executor:
        executor.submit(cpu_task, "CPU1", 20000)
        executor.submit(cpu_task, "CPU2", 20000)
    elapsed_cpu = time.time() - start
    print(f"   耗时：{elapsed_cpu:.2f} 秒（几乎和串行一样，因为 GIL）")


# ==============================================================================
# 第二部分：多进程（multiprocessing）
# ==============================================================================
# 进程是操作系统资源分配的最小单位
# 每个进程有独立的内存空间，互不干扰
#
# Python 中的多进程：
#   - 优点：不受 GIL 限制，可以真正并行，适合 CPU 密集型任务
#   - 缺点：创建开销大，进程间通信复杂
#   - 适用场景：CPU 密集型任务（科学计算、图像处理、数据处理等）
# ==============================================================================

# 注意：多进程中执行的函数必须定义在模块顶层（可以被 pickle 序列化）
# 所以 cpu_task 函数在前面已经定义了，可以在多进程中使用

def cpu_task_process(name, n):
    """
    函数名：cpu_task_process
    功能：CPU 密集型任务（用于多进程测试）
    说明：单独定义一个版本，因为多进程在 Windows 下需要函数在模块顶层，
          且需要可以被序列化。
    """
    import time as _time
    process_name = multiprocessing.current_process().name
    print(f"   [进程 {process_name}] 任务 {name} 开始计算...")
    count = 0
    for num in range(2, n):
        is_prime = True
        for i in range(2, int(num ** 0.5) + 1):
            if num % i == 0:
                is_prime = False
                break
        if is_prime:
            count += 1
    print(f"   [进程 {process_name}] 任务 {name} 完成，质数个数：{count}")
    return count


def io_task_process(name, duration):
    """
    函数名：io_task_process
    功能：IO 密集型任务（用于多进程测试）
    """
    import time as _time
    process_name = multiprocessing.current_process().name
    print(f"   [进程 {process_name}] 任务 {name} 开始，预计耗时 {duration} 秒")
    _time.sleep(duration)
    print(f"   [进程 {process_name}] 任务 {name} 完成")
    return f"{name} 的结果"


def demo_multiprocessing():
    """
    函数名：demo_multiprocessing
    功能：演示多进程编程
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、多进程（multiprocessing）】")
    print("=" * 60)

    # ---------- 1. 进程池（ProcessPoolExecutor）----------
    print("\n1. 进程池（ProcessPoolExecutor）：")

    print("   CPU 密集型任务（多进程可以真正并行）：")
    start = time.time()

    # 使用进程池执行 CPU 密集型任务
    with ProcessPoolExecutor(max_workers=2) as executor:
        futures = [
            executor.submit(cpu_task_process, "CPU1", 30000),
            executor.submit(cpu_task_process, "CPU2", 30000),
        ]
        for future in futures:
            future.result()

    elapsed = time.time() - start
    print(f"   总耗时：{elapsed:.2f} 秒（多核下应该接近串行的一半）")

    # ---------- 2. 串行对比 ----------
    print("\n2. 串行执行对比：")
    start = time.time()
    cpu_task("串行1", 30000)
    cpu_task("串行2", 30000)
    elapsed_serial = time.time() - start
    print(f"   串行耗时：{elapsed_serial:.2f} 秒")

    print(f"\n   加速比：{elapsed_serial / elapsed:.2f} 倍")


# ==============================================================================
# 第三部分：协程（asyncio）
# ==============================================================================
# 协程（Coroutine）是一种用户态的轻量级并发单元
# 协程在单个线程内运行，由程序自己控制切换（协作式多任务）
#
# asyncio 是 Python 3.4+ 引入的异步 IO 库
# 核心概念：
#   - async def：定义协程函数
#   - await：等待协程或异步操作（挂起当前协程，切换执行其他协程）
#   - event loop：事件循环，调度和管理协程的执行
#   - task：任务，是协程的包装，可被事件循环调度
#
# 优点：
#   - 开销极小（比线程轻量得多）
#   - 单线程内并发，没有线程安全问题
#   - 适合高并发 IO 场景（如 Web 服务器、爬虫）
# 缺点：
#   - 所有 IO 操作都必须使用异步版本
#   - 编程复杂度较高
# ==============================================================================

async def async_io_task(name, duration):
    """
    函数名：async_io_task
    功能：异步 IO 任务（协程函数）
    参数：
        name - 任务名称
        duration - 任务持续时间
    返回值：任务结果字符串
    说明：使用 async def 定义协程函数。
          调用协程函数不会立即执行，而是返回一个协程对象。
          需要通过 await 或事件循环来执行。
    """
    print(f"   [协程] 任务 {name} 开始，预计耗时 {duration} 秒")
    # asyncio.sleep 是异步版本的 sleep，不会阻塞整个线程
    # await 会挂起当前协程，让事件循环去执行其他协程
    await asyncio.sleep(duration)
    print(f"   [协程] 任务 {name} 完成")
    return f"{name} 的结果"


async def async_cpu_work(name, n):
    """
    函数名：async_cpu_work
    功能：模拟协程中的计算工作
    说明：注意：协程中如果有纯 CPU 计算，会阻塞整个事件循环，
          因为协程是单线程的。CPU 密集型任务不适合用协程。
    """
    print(f"   [协程] 任务 {name} 开始计算...")
    count = 0
    for num in range(2, n):
        is_prime = True
        for i in range(2, int(num ** 0.5) + 1):
            if num % i == 0:
                is_prime = False
                break
        if is_prime:
            count += 1
        # 每隔一定数量主动让出控制权（模拟协作式调度）
        # 实际开发中不建议这样做，CPU 密集任务应该用多进程
        if num % 5000 == 0:
            await asyncio.sleep(0)  # sleep(0) 主动让出控制权
    print(f"   [协程] 任务 {name} 完成，质数个数：{count}")
    return count


def demo_asyncio():
    """
    函数名：demo_asyncio
    功能：演示 asyncio 协程编程
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、协程（asyncio）】")
    print("=" * 60)

    # ---------- 1. 基本用法：async/await ----------
    print("\n1. 基本用法：async/await")

    async def main_basic():
        """主协程：基本用法演示"""
        start = time.time()

        # 注意：如果直接顺序 await，还是串行执行
        print("   顺序 await（串行）：")
        await async_io_task("A", 1)
        await async_io_task("B", 1)

        elapsed = time.time() - start
        print(f"   顺序执行耗时：{elapsed:.2f} 秒")

    # 运行协程（Python 3.7+ 推荐用 asyncio.run）
    asyncio.run(main_basic())

    # ---------- 2. 并发执行：gather ----------
    print("\n2. 并发执行：asyncio.gather()")

    async def main_gather():
        """主协程：并发执行演示"""
        start = time.time()

        # 使用 asyncio.gather 并发执行多个协程
        print("   使用 gather 并发执行 3 个任务：")
        results = await asyncio.gather(
            async_io_task("任务1", 2),
            async_io_task("任务2", 1),
            async_io_task("任务3", 2),
        )

        elapsed = time.time() - start
        print(f"   并发执行结果：{results}")
        print(f"   总耗时：{elapsed:.2f} 秒（3个任务，最长2秒，所以约2秒）")

    asyncio.run(main_gather())

    # ---------- 3. Task 任务 ----------
    print("\n3. Task 任务：")

    async def main_task():
        """主协程：Task 演示"""
        start = time.time()

        # 使用 asyncio.create_task 创建任务（立即被调度执行）
        print("   创建 3 个 Task：")
        task1 = asyncio.create_task(async_io_task("任务A", 2))
        task2 = asyncio.create_task(async_io_task("任务B", 1))
        task3 = asyncio.create_task(async_io_task("任务C", 2))

        # 等待任务完成
        result1 = await task1
        result2 = await task2
        result3 = await task3

        elapsed = time.time() - start
        print(f"   总耗时：{elapsed:.2f} 秒")

    asyncio.run(main_task())

    # ---------- 4. 协程处理大量并发 ----------
    print("\n4. 协程处理大量并发任务：")

    async def main_many():
        """主协程：大量并发任务演示"""
        start = time.time()

        # 创建 20 个并发任务
        print("   并发执行 20 个 IO 任务（每个 1 秒）：")
        tasks = [async_io_task(f"任务{i}", 1) for i in range(20)]
        results = await asyncio.gather(*tasks)

        elapsed = time.time() - start
        print(f"   20 个任务总耗时：{elapsed:.2f} 秒（协程开销极小）")
        print(f"   完成任务数：{len(results)}")

    asyncio.run(main_many())


# ==============================================================================
# 第四部分：三种并发方式对比
# ==============================================================================

def demo_comparison():
    """
    函数名：demo_comparison
    功能：对比三种并发方式的特点和适用场景
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【四、三种并发方式对比】")
    print("=" * 60)

    print("""
   +----------------+----------------+----------------+----------------+
   | 特性            | 多线程         | 多进程         | 协程           |
   +----------------+----------------+----------------+----------------+
   | 资源开销        | 中             | 大             | 极小           |
   | 切换开销        | 小（操作系统） | 大（操作系统） | 极小（用户态） |
   | GIL 影响        | 受限制         | 不受限         | 不受限（单线程）|
   | 并行能力        | 伪并行（IO）   | 真并行         | 并发（非并行） |
   | 数据共享        | 容易（同内存） | 困难（不同内存）| 简单（同线程） |
   | 线程安全        | 需要锁等机制   | 不需要         | 不需要         |
   | 适用场景        | IO 密集型      | CPU 密集型     | 高并发 IO 型   |
   | 编程复杂度      | 中等           | 较难           | 较难           |
   +----------------+----------------+----------------+----------------+
""")

    print("   选择建议：")
    print("   - 如果是 IO 密集型任务（网络爬虫、Web 请求、文件读写）：")
    print("     * 并发量不大：多线程（简单易用）")
    print("     * 并发量很大（几千上万）：协程（性能更好）")
    print("   - 如果是 CPU 密集型任务（科学计算、数据处理）：")
    print("     * 多进程（可以利用多核 CPU）")
    print("   - 实际项目中往往是混合使用：")
    print("     * 多进程 + 协程：每个进程跑一个事件循环，充分利用多核")
    print()

    # 用 IO 任务对比三种方式的耗时
    print("   IO 密集型任务（10 个任务，每个 1 秒）对比：")

    # 多线程
    print("   多线程（3个线程）：", end=" ")
    start = time.time()
    with ThreadPoolExecutor(max_workers=3) as executor:
        futures = [executor.submit(io_task, f"T{i}", 1) for i in range(10)]
        for f in futures:
            f.result()
    print(f"耗时 {time.time() - start:.2f} 秒")

    # 协程
    print("   协程：", end=" ")
    start = time.time()

    async def run_async():
        tasks = [async_io_task(f"C{i}", 1) for i in range(10)]
        await asyncio.gather(*tasks)

    asyncio.run(run_async())
    print(f"   耗时 {time.time() - start:.2f} 秒")

    # 串行（对比基准）
    print("   串行（对比）：", end=" ")
    start = time.time()
    for i in range(10):
        time.sleep(1)
    print(f"耗时 {time.time() - start:.2f} 秒")


# ==============================================================================
# 主函数：运行所有演示
# ==============================================================================

def run_demo():
    """
    函数名：run_demo
    功能：运行本模块的所有演示函数
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("  Python 并发编程演示")
    print("=" * 60)

    demo_threading()         # 多线程
    demo_multiprocessing()   # 多进程
    demo_asyncio()           # 协程
    demo_comparison()        # 三种方式对比

    print("\n" + "=" * 60)
    print("  并发编程演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
