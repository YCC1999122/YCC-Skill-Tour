/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.concurrency;

/**
 * ============================================================
 * 导入需要的并发工具类
 * java.util.concurrent 是Java并发包，包含各种并发工具
 * ============================================================
 */
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * ============================================================
 * ConcurrencyDemo 类：Java并发编程演示
 *
 * 【并发编程是什么】
 * - 在同一时间内执行多个任务（线程）
 * - 目的：提高CPU利用率、提高程序响应速度
 * - 进程：操作系统分配资源的最小单位，有独立的内存空间
 * - 线程：CPU调度的最小单位，共享进程的内存空间
 * - 一个进程可以有多个线程，线程之间共享堆内存，但有独立的栈内存
 *
 * 【并发编程的三大问题】
 * 1. 原子性：一个操作要么全部执行，要么都不执行（中间不能被打断）
 * 2. 可见性：一个线程修改了共享变量，其他线程能立即看到修改后的值
 * 3. 有序性：程序执行的顺序和代码的顺序一致（编译器/CPU可能会重排序）
 * ============================================================
 */
public class ConcurrencyDemo {

    /**
     * main方法：程序入口
     */
    public static void main(String[] args) {
        System.out.println("========== Java并发编程演示 ==========");

        demonstrateThreadCreation();   // 演示线程创建的三种方式
        demonstrateThreadState();      // 演示线程生命周期与状态
        demonstrateSynchronized();     // 演示synchronized同步
        demonstrateLock();             // 演示Lock接口
        demonstrateVolatile();         // 演示volatile关键字
        demonstrateAtomic();           // 演示原子类
        demonstrateThreadPool();       // 演示线程池
        demonstrateSyncUtils();        // 演示同步工具类
        demonstrateDeadlock();         // 演示死锁

        System.out.println("========== 并发编程演示结束 ==========\n");
    }

    /**
     * ============================================================
     * 演示线程创建的三种方式
     *
     * 【创建线程的三种方式】
     * 1. 继承Thread类，重写run()方法
     *    - 优点：简单，直接使用this就代表当前线程
     *    - 缺点：单继承限制，不能再继承其他类
     * 2. 实现Runnable接口，重写run()方法
     *    - 优点：可以实现多个接口，可以被多个线程共享
     *    - 缺点：run()方法没有返回值，不能抛出受检异常
     * 3. 实现Callable接口，重写call()方法（Java 5+）
     *    - 优点：有返回值，可以抛出异常
     *    - 缺点：需要配合FutureTask使用，稍复杂
     *
     * 【注意】
     * - 启动线程必须调用start()方法，而不是run()方法
     * - start()方法会启动一个新线程，然后由新线程执行run()方法
     * - 直接调用run()方法只是普通的方法调用，不会启动新线程
     * ============================================================
     */
    public static void demonstrateThreadCreation() {
        System.out.println("\n--- 1. 线程创建的三种方式 ---");

        // --- 方式1：继承Thread类 ---
        System.out.println("【方式1：继承Thread类】");
        // 创建线程对象
        Thread t1 = new MyThread("线程A");
        // 启动线程（调用start()，不是run()！）
        t1.start();

        // --- 方式2：实现Runnable接口 ---
        System.out.println("\n【方式2：实现Runnable接口】");
        // 创建Runnable实现类对象
        Runnable myRunnable = new MyRunnable();
        // 将Runnable对象传给Thread
        Thread t2 = new Thread(myRunnable, "线程B");
        t2.start();

        // 匿名内部类方式（更常用）
        Thread t3 = new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println(Thread.currentThread().getName() + "：匿名内部类方式创建");
            }
        }, "线程C");
        t3.start();

        // Lambda表达式方式（Java 8+，最简洁）
        Thread t4 = new Thread(() -> {
            System.out.println(Thread.currentThread().getName() + "：Lambda表达式方式创建");
        }, "线程D");
        t4.start();

        // --- 方式3：实现Callable接口（有返回值） ---
        System.out.println("\n【方式3：实现Callable接口（有返回值）】");
        // Callable需要配合FutureTask使用
        Callable<Integer> myCallable = new MyCallable();
        FutureTask<Integer> futureTask = new FutureTask<>(myCallable);
        Thread t5 = new Thread(futureTask, "线程E");
        t5.start();

        try {
            // get()方法会阻塞，直到线程执行完毕返回结果
            Integer result = futureTask.get();
            System.out.println("Callable线程的返回结果: " + result);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }

        // 等待上面的线程执行完（避免输出混乱）
        try {
            Thread.sleep(50);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * 自定义线程类：继承Thread
     */
    static class MyThread extends Thread {
        public MyThread(String name) {
            super(name);  // 调用父类构造方法，设置线程名
        }

        @Override
        public void run() {
            // run()方法中是线程要执行的任务
            System.out.println(getName() + "：继承Thread方式创建，正在执行");
        }
    }

    /**
     * 自定义Runnable类：实现Runnable接口
     */
    static class MyRunnable implements Runnable {
        @Override
        public void run() {
            // Thread.currentThread()：获取当前正在执行的线程对象
            System.out.println(Thread.currentThread().getName() + "：实现Runnable方式创建，正在执行");
        }
    }

    /**
     * 自定义Callable类：实现Callable接口
     * Callable的泛型参数表示返回值类型
     */
    static class MyCallable implements Callable<Integer> {
        @Override
        public Integer call() throws Exception {
            System.out.println(Thread.currentThread().getName() + "：实现Callable方式创建，正在计算");
            // 模拟计算
            int sum = 0;
            for (int i = 1; i <= 100; i++) {
                sum += i;
            }
            return sum;  // 有返回值
        }
    }

    /**
     * ============================================================
     * 演示线程的生命周期与状态
     *
     * 【线程的6种状态（Thread.State枚举）】
     * 1. NEW（新建）：线程对象刚创建，还没调用start()
     * 2. RUNNABLE（可运行）：调用了start()，在JVM中正在执行或等待CPU调度
     *    - 包括Ready（就绪，等待CPU）和Running（运行中，正在执行）
     * 3. BLOCKED（阻塞）：等待锁（synchronized），被阻塞在同步块外
     * 4. WAITING（等待）：调用wait()、join()、LockSupport.park()进入无限等待
     *    - 需要被其他线程唤醒（notify()、notifyAll()）
     * 5. TIMED_WAITING（计时等待）：有超时时间的等待
     *    - 调用sleep(time)、wait(time)、join(time)等
     *    - 超时后自动唤醒，或被提前唤醒
     * 6. TERMINATED（终止）：线程执行完毕（run方法结束）
     *
     * 【状态转换】
     * NEW → start() → RUNNABLE
     * RUNNABLE → 等待锁 → BLOCKED → 获得锁 → RUNNABLE
     * RUNNABLE → wait() → WAITING → notify() → RUNNABLE
     * RUNNABLE → sleep(time) → TIMED_WAITING → 超时 → RUNNABLE
     * RUNNABLE → 执行完毕 → TERMINATED
     * ============================================================
     */
    public static void demonstrateThreadState() {
        System.out.println("\n--- 2. 线程生命周期与状态 ---");

        System.out.println("【线程的6种状态】");
        System.out.println("NEW：新建状态，线程刚创建，还没start()");
        System.out.println("RUNNABLE：可运行状态，正在运行或等待CPU");
        System.out.println("BLOCKED：阻塞状态，等待锁");
        System.out.println("WAITING：等待状态，等待其他线程唤醒");
        System.out.println("TIMED_WAITING：计时等待状态，超时自动唤醒");
        System.out.println("TERMINATED：终止状态，执行完毕");

        // 演示NEW和TERMINATED状态
        Thread t = new Thread(() -> {
            System.out.println("线程执行中，状态: " + Thread.currentThread().getState());
        });
        System.out.println("\n创建线程后（未start）: " + t.getState());  // NEW
        t.start();
        try {
            t.join();  // 等待线程执行完
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("线程执行完后: " + t.getState());  // TERMINATED

        // 演示TIMED_WAITING状态
        Thread sleepThread = new Thread(() -> {
            try {
                Thread.sleep(200);  // 睡眠200毫秒，进入TIMED_WAITING
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
        sleepThread.start();
        try {
            Thread.sleep(50);  // 主线程等一会儿，确保子线程进入sleep
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("线程sleep中: " + sleepThread.getState());  // TIMED_WAITING

        // 常见方法说明
        System.out.println("\n【线程常用方法】");
        System.out.println("start()：启动线程，进入RUNNABLE状态");
        System.out.println("run()：线程的任务逻辑，不要直接调用");
        System.out.println("sleep(long)：睡眠指定时间，进入TIMED_WAITING");
        System.out.println("yield()：让出CPU，从Running→Ready，还是RUNNABLE状态");
        System.out.println("join()：等待其他线程执行完，进入WAITING");
        System.out.println("interrupt()：中断线程（设置中断标记）");
        System.out.println("isInterrupted()：判断是否被中断");
        System.out.println("interrupted()：判断是否被中断，并清除中断标记（静态方法）");
        System.out.println("setPriority(int)：设置线程优先级（1-10，默认5）");
        System.out.println("setDaemon(boolean)：设置为守护线程");
    }

    /**
     * ============================================================
     * 演示synchronized同步
     *
     * 【为什么需要同步】
     * - 多个线程同时访问共享资源时，可能会出现数据不一致的问题
     * - 这就是"线程安全问题"
     * - 典型例子：多个线程同时对一个变量做++操作
     *
     * 【synchronized是什么】
     * - Java内置的锁机制，保证同一时刻只有一个线程执行被同步的代码
     * - 保证原子性、可见性、有序性
     * - 是一种互斥锁（独占锁）
     *
     * 【synchronized的三种使用方式】
     * 1. 同步方法：修饰普通方法，锁的是this（当前对象）
     * 2. 静态同步方法：修饰静态方法，锁的是类的Class对象
     * 3. 同步代码块：synchronized(锁对象) { ... }，锁的是指定对象
     *
     * 【synchronized的底层原理】
     * - 每个对象都有一个monitor（监视器锁）
     * - 进入同步块前要先获取monitor，退出时释放monitor
     * - Java 6之后synchronized做了很多优化：
     *   偏向锁 → 轻量级锁 → 重量级锁（锁膨胀）
     * ============================================================
     */
    public static void demonstrateSynchronized() {
        System.out.println("\n--- 3. synchronized 同步 ---");

        System.out.println("【线程安全问题演示：无同步的计数器】");
        Counter counter = new Counter();  // 共享的计数器对象
        int threadCount = 10;             // 10个线程
        int perThread = 1000;             // 每个线程加1000次

        Thread[] threads = new Thread[threadCount];
        for (int i = 0; i < threadCount; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < perThread; j++) {
                    counter.incrementUnsafe();  // 无同步的递增
                }
            });
            threads[i].start();
        }

        // 等待所有线程执行完
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // 预期结果应该是10*1000=10000，但实际可能小于10000
        System.out.println("预期结果: " + (threadCount * perThread));
        System.out.println("实际结果: " + counter.getCount() + "（可能小于预期，因为线程不安全）");
        System.out.println("原因：count++不是原子操作，包含读取-修改-写入三步");

        // --- 使用synchronized同步方法 ---
        System.out.println("\n【使用synchronized同步方法的计数器】");
        Counter syncCounter = new Counter();
        Thread[] syncThreads = new Thread[threadCount];
        for (int i = 0; i < threadCount; i++) {
            syncThreads[i] = new Thread(() -> {
                for (int j = 0; j < perThread; j++) {
                    syncCounter.incrementSync();  // 同步方法，线程安全
                }
            });
            syncThreads[i].start();
        }

        for (Thread t : syncThreads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("预期结果: " + (threadCount * perThread));
        System.out.println("实际结果: " + syncCounter.getCount() + "（和预期一致，线程安全）");

        // --- 同步代码块演示 ---
        System.out.println("\n【同步代码块演示】");
        Object lock = new Object();  // 锁对象
        final int[] num = {0};
        Thread t1 = new Thread(() -> {
            synchronized (lock) {    // 同步代码块，锁是lock对象
                for (int i = 0; i < 5; i++) {
                    num[0]++;
                    System.out.println("线程1: num = " + num[0]);
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        Thread t2 = new Thread(() -> {
            synchronized (lock) {    // 同一个锁对象，所以两个线程互斥
                for (int i = 0; i < 5; i++) {
                    num[0]++;
                    System.out.println("线程2: num = " + num[0]);
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        t1.start();
        t2.start();
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("同步代码块保证了两个线程顺序执行，不会交替");
    }

    /**
     * 计数器类：用于演示线程安全问题
     */
    static class Counter {
        private int count = 0;

        /**
         * 非线程安全的递增方法
         * count++不是原子操作，包含三步：
         * 1. 读取count的值
         * 2. 将值加1
         * 3. 将新值写回count
         * 多线程下可能出现丢失更新
         */
        public void incrementUnsafe() {
            count++;
        }

        /**
         * 线程安全的递增方法（同步方法）
         * synchronized修饰普通方法，锁的是this对象
         * 同一时刻只能有一个线程执行这个对象的同步方法
         */
        public synchronized void incrementSync() {
            count++;
        }

        public int getCount() {
            return count;
        }
    }

    /**
     * ============================================================
     * 演示Lock接口与ReentrantLock
     *
     * 【Lock接口是什么】
     * - Java 5引入的显式锁，比synchronized更灵活
     * - 需要手动加锁和释放锁（lock()和unlock()）
     * - 实现类：ReentrantLock、ReentrantReadWriteLock等
     *
     * 【ReentrantLock vs synchronized】
     * 相同点：
     * - 都是可重入锁（同一个线程可以多次获取同一把锁）
     * - 都是互斥锁
     *
     * 不同点：
     * - synchronized是关键字，JVM层面实现；Lock是接口，API层面实现
     * - Lock可以尝试获取锁（tryLock），可以设置超时时间
     * - Lock可以中断等待（lockInterruptibly）
     * - Lock可以实现公平锁（先到先得）
     * - Lock需要手动释放锁（必须在finally中unlock，防止死锁）
     * - synchronized的锁释放是自动的（退出同步块自动释放）
     *
     * 【可重入锁是什么】
     * - 同一个线程可以多次获取同一把锁
     * - 锁内部有一个计数器，获取一次+1，释放一次-1
     * - 计数器为0时才真正释放锁
     * - 好处：避免死锁，支持递归调用
     * ============================================================
     */
    public static void demonstrateLock() {
        System.out.println("\n--- 4. Lock 接口与 ReentrantLock ---");

        System.out.println("【ReentrantLock 演示】");
        LockCounter lockCounter = new LockCounter();
        int threadCount = 10;
        int perThread = 1000;

        Thread[] threads = new Thread[threadCount];
        for (int i = 0; i < threadCount; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < perThread; j++) {
                    lockCounter.increment();
                }
            });
            threads[i].start();
        }

        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("预期结果: " + (threadCount * perThread));
        System.out.println("实际结果: " + lockCounter.getCount());

        // --- tryLock 尝试获取锁 ---
        System.out.println("\n【tryLock 尝试获取锁】");
        Lock tryLock = new ReentrantLock();

        Thread t1 = new Thread(() -> {
            boolean locked = tryLock.tryLock();  // 尝试获取锁，立即返回
            if (locked) {
                try {
                    System.out.println("线程1：获取锁成功，执行任务");
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    tryLock.unlock();
                    System.out.println("线程1：释放锁");
                }
            } else {
                System.out.println("线程1：获取锁失败，做其他事情");
            }
        });

        Thread t2 = new Thread(() -> {
            boolean locked = false;
            try {
                // 尝试获取锁，最多等待50毫秒
                locked = tryLock.tryLock(50, TimeUnit.MILLISECONDS);
                if (locked) {
                    System.out.println("线程2：获取锁成功");
                } else {
                    System.out.println("线程2：等待50毫秒后仍未获取锁，放弃");
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                if (locked) {
                    tryLock.unlock();
                }
            }
        });

        t1.start();
        try {
            Thread.sleep(10);  // 确保t1先拿到锁
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        t2.start();

        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("\n【Lock vs synchronized 总结】");
        System.out.println("Lock更灵活：可尝试获取锁、可中断、可公平锁");
        System.out.println("synchronized更简单：自动释放锁，不会忘记解锁");
        System.out.println("可重入性：两者都是可重入锁");
    }

    /**
     * 使用ReentrantLock的计数器
     */
    static class LockCounter {
        private int count = 0;
        // 创建一个ReentrantLock实例
        private final Lock lock = new ReentrantLock();

        public void increment() {
            lock.lock();  // 加锁
            try {
                count++;  // 临界区代码
            } finally {
                lock.unlock();  // 释放锁，必须在finally中，确保一定执行
            }
        }

        public int getCount() {
            return count;
        }
    }

    /**
     * ============================================================
     * 演示volatile关键字
     *
     * 【volatile是什么】
     * - 轻量级的同步机制，只能修饰成员变量
     * - 保证可见性和有序性，不保证原子性
     *
     * 【保证可见性】
     * - Java内存模型（JMM）：每个线程有自己的工作内存，共享主内存
     * - 普通变量：线程修改后写回主内存的时机不确定，其他线程可能看不到
     * - volatile变量：修改后立即写回主内存，读取时直接从主内存读
     * - 实现：通过内存屏障（Memory Barrier）实现
     *
     * 【保证有序性】
     * - 禁止指令重排序（编译器和CPU可能会对指令进行重排序优化）
     * - volatile变量的读写操作会插入内存屏障，阻止重排序
     *
     * 【不保证原子性】
     * - volatile不能保证复合操作的原子性（如count++）
     * - 所以volatile不能完全替代synchronized
     *
     * 【volatile的典型使用场景】
     * 1. 状态标记：boolean flag，一个线程设置，其他线程读取
     * 2. 双重检查锁定（DCL）的单例模式
     * ============================================================
     */
    public static void demonstrateVolatile() {
        System.out.println("\n--- 5. volatile 关键字 ---");

        System.out.println("【volatile的两大特性】");
        System.out.println("1. 保证可见性：修改对所有线程立即可见");
        System.out.println("2. 保证有序性：禁止指令重排序");
        System.out.println("3. 不保证原子性：如count++这样的复合操作不保证原子");

        // --- 可见性演示 ---
        System.out.println("\n【可见性演示：volatile状态标记】");
        VolatileTask task = new VolatileTask();
        Thread t = new Thread(task);
        t.start();

        try {
            Thread.sleep(100);  // 主线程睡100毫秒
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        task.stopFlag = true;  // 设置停止标记
        System.out.println("主线程设置了stopFlag = true");

        try {
            t.join(1000);  // 最多等1秒
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("如果flag是volatile的，子线程会立即看到并停止");
        System.out.println("如果不是volatile的，子线程可能一直看不到（取决于JVM优化）");

        // --- volatile不保证原子性 ---
        System.out.println("\n【volatile不保证原子性】");
        VolatileCounter vc = new VolatileCounter();
        for (int i = 0; i < 10; i++) {
            new Thread(() -> {
                for (int j = 0; j < 1000; j++) {
                    vc.increment();  // volatile变量的++操作不是原子的
                }
            }).start();
        }

        // 等所有线程结束（简化处理，睡一下）
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("预期结果: 10000");
        System.out.println("实际结果: " + vc.count + "（可能小于10000，因为volatile不保证原子性）");
    }

    /**
     * volatile演示任务
     */
    static class VolatileTask implements Runnable {
        // volatile修饰的变量，保证可见性
        public volatile boolean stopFlag = false;

        @Override
        public void run() {
            int i = 0;
            while (!stopFlag) {  // 循环检查停止标记
                i++;
                // 注意：如果这里有println或sleep，也会刷新内存，可能导致即使不加volatile也可见
            }
            System.out.println("子线程检测到stopFlag为true，停止运行，循环了" + i + "次");
        }
    }

    /**
     * volatile计数器：演示volatile不保证原子性
     */
    static class VolatileCounter {
        // volatile只能保证可见性，不能保证原子性
        public volatile int count = 0;

        public void increment() {
            count++;  // 这个操作不是原子的：读→改→写
        }
    }

    /**
     * ============================================================
     * 演示原子类
     *
     * 【原子类是什么】
     * - java.util.concurrent.atomic包下的类
     * - 利用CAS（Compare-And-Swap）操作保证原子性
     * - 是一种无锁（Lock-Free）的实现方式，性能比synchronized好
     *
     * 【CAS是什么】
     * - Compare-And-Swap：比较并交换，是一种CPU指令级的原子操作
     * - 包含三个操作数：内存位置V、预期值A、新值B
     * - 操作逻辑：如果V的值等于A，就把V更新为B，返回成功；否则返回失败
     * - CAS是乐观锁的实现：假设没有冲突，直接操作，失败了再重试
     *
     * 【常见原子类】
     * - 基本类型：AtomicInteger、AtomicLong、AtomicBoolean
     * - 引用类型：AtomicReference、AtomicStampedReference
     * - 数组类型：AtomicIntegerArray、AtomicLongArray
     * - 字段更新器：AtomicIntegerFieldUpdater等
     * - 累加器：LongAdder、DoubleAdder（高并发下比AtomicLong性能好）
     *
     * 【ABA问题】
     * - CAS的一个问题：如果值原来是A，变成了B，又变回了A
     * - CAS会认为它没变，但实际上变过了
     * - 解决方法：使用版本号或时间戳（AtomicStampedReference）
     * ============================================================
     */
    public static void demonstrateAtomic() {
        System.out.println("\n--- 6. 原子类（Atomic）---");

        System.out.println("【AtomicInteger 演示】");
        AtomicInteger atomicInt = new AtomicInteger(0);

        // 常用方法
        System.out.println("初始值: " + atomicInt.get());
        atomicInt.incrementAndGet();  // 原子递增，返回新值（++i）
        System.out.println("incrementAndGet: " + atomicInt.get());
        atomicInt.getAndIncrement();  // 原子递增，返回旧值（i++）
        System.out.println("getAndIncrement: " + atomicInt.get());

        atomicInt.addAndGet(10);      // 原子加指定值，返回新值
        System.out.println("addAndGet(10): " + atomicInt.get());

        atomicInt.compareAndSet(12, 100);  // CAS：如果当前值是12，就设为100
        System.out.println("compareAndSet(12, 100): " + atomicInt.get());

        // --- 线程安全的计数器 ---
        System.out.println("\n【AtomicInteger实现线程安全计数器】");
        AtomicInteger counter = new AtomicInteger(0);
        int threadCount = 10;
        int perThread = 1000;

        Thread[] threads = new Thread[threadCount];
        for (int i = 0; i < threadCount; i++) {
            threads[i] = new Thread(() -> {
                for (int j = 0; j < perThread; j++) {
                    counter.incrementAndGet();  // 原子递增
                }
            });
            threads[i].start();
        }

        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("预期结果: " + (threadCount * perThread));
        System.out.println("实际结果: " + counter.get() + "（一定等于预期，原子类保证）");

        System.out.println("\n【CAS原理】");
        System.out.println("CAS = Compare-And-Swap（比较并交换）");
        System.out.println("操作：V（内存值）== A（预期值）？ V = B（新值）: 失败重试");
        System.out.println("是CPU级别的原子指令，不需要加锁，性能更高");
        System.out.println("缺点：ABA问题、自旋消耗CPU、只能保证一个变量的原子性");
    }

    /**
     * ============================================================
     * 演示线程池
     *
     * 【为什么需要线程池】
     * - 线程的创建和销毁开销很大（需要OS参与）
     * - 频繁创建销毁线程会浪费资源，降低性能
     * - 线程池：预先创建一些线程，用完放回池中复用
     * - 好处：降低资源消耗、提高响应速度、便于管理控制
     *
     * 【线程池的核心参数（ThreadPoolExecutor）】
     * 1. corePoolSize：核心线程数，常驻线程
     * 2. maximumPoolSize：最大线程数，池中最多的线程数
     * 3. keepAliveTime：非核心线程的空闲存活时间
     * 4. unit：时间单位
     * 5. workQueue：任务队列，等待执行的任务
     * 6. threadFactory：线程工厂，创建线程的方式
     * 7. handler：拒绝策略，队列满了且线程数达到最大时的处理方式
     *
     * 【任务提交流程】
     * 提交任务 → 核心线程数未满？→ 创建核心线程执行
     *                  ↓ 已满
     *             队列未满？→ 加入队列等待
     *                  ↓ 已满
     *             最大线程数未满？→ 创建非核心线程执行
     *                  ↓ 已满
     *             执行拒绝策略
     *
     * 【四种拒绝策略】
     * 1. AbortPolicy：直接抛出异常（默认）
     * 2. CallerRunsPolicy：由提交任务的线程自己执行
     * 3. DiscardPolicy：直接丢弃任务
     * 4. DiscardOldestPolicy：丢弃队列中最老的任务，尝试重新提交
     *
     * 【Executors提供的四种常用线程池（不推荐生产环境直接用）】
     * 1. newFixedThreadPool：固定线程数的线程池
     * 2. newCachedThreadPool：可缓存的线程池（自动回收）
     * 3. newSingleThreadExecutor：单线程的线程池
     * 4. newScheduledThreadPool：定时任务线程池
     *
     * 注意：《阿里巴巴Java开发手册》不推荐用Executors创建线程池，
     * 因为FixedThreadPool和SingleThreadExecutor的队列是无界的，
     * CachedThreadPool的最大线程数是Integer.MAX_VALUE，都可能OOM。
     * 推荐用ThreadPoolExecutor自定义线程池。
     * ============================================================
     */
    public static void demonstrateThreadPool() {
        System.out.println("\n--- 7. 线程池（ThreadPoolExecutor）---");

        System.out.println("【线程池执行任务演示】");

        // 创建一个自定义线程池（推荐方式）
        ThreadPoolExecutor executor = new ThreadPoolExecutor(
                2,                 // corePoolSize 核心线程数
                5,                 // maximumPoolSize 最大线程数
                60L,               // keepAliveTime 空闲线程存活时间
                TimeUnit.SECONDS,  // unit 时间单位
                new ArrayBlockingQueue<>(10),  // workQueue 任务队列
                Executors.defaultThreadFactory(), // threadFactory 线程工厂
                new ThreadPoolExecutor.AbortPolicy() // handler 拒绝策略
        );

        // 提交10个任务
        for (int i = 1; i <= 10; i++) {
            final int taskId = i;
            executor.execute(() -> {
                System.out.println("任务" + taskId + " 正在执行，线程: "
                        + Thread.currentThread().getName());
                try {
                    Thread.sleep(50);  // 模拟任务执行
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            });
        }

        // 关闭线程池（不再接受新任务，已提交的任务会执行完）
        executor.shutdown();

        try {
            // 等待所有任务执行完，最多等5秒
            executor.awaitTermination(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("所有任务执行完毕");

        // --- Executors提供的线程池（了解即可） ---
        System.out.println("\n【Executors的四种线程池（了解）】");
        System.out.println("1. newFixedThreadPool：固定线程数，无界队列（可能OOM）");
        System.out.println("2. newCachedThreadPool：弹性线程数，最大Integer.MAX_VALUE（可能OOM）");
        System.out.println("3. newSingleThreadExecutor：单线程，保证顺序执行");
        System.out.println("4. newScheduledThreadPool：定时任务线程池");

        // 演示ScheduledExecutorService
        System.out.println("\n【ScheduledExecutorService 定时任务演示】");
        ScheduledExecutorService scheduledExecutor = Executors.newScheduledThreadPool(1);

        // 延迟100毫秒执行一次
        scheduledExecutor.schedule(() -> {
            System.out.println("延迟任务：100毫秒后执行");
        }, 100, TimeUnit.MILLISECONDS);

        // 周期性执行：初始延迟50ms，之后每200ms执行一次
        // 注意：这里只是演示，实际会一直执行，需要关闭
        ScheduledFuture<?> scheduledFuture = scheduledExecutor.scheduleAtFixedRate(() -> {
            System.out.println("周期任务：每隔200毫秒执行一次");
        }, 50, 200, TimeUnit.MILLISECONDS);

        // 执行一会儿后关闭
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        scheduledFuture.cancel(true);  // 取消周期任务
        scheduledExecutor.shutdown();  // 关闭线程池

        System.out.println("定时任务演示结束");

        System.out.println("\n【线程池参数设置建议】");
        System.out.println("CPU密集型任务：核心线程数 = CPU核心数 + 1");
        System.out.println("IO密集型任务：核心线程数 = CPU核心数 * 2 或更多");
        System.out.println("具体数值需要根据实际压测调整");
    }

    /**
     * ============================================================
     * 演示同步工具类
     *
     * 【常见的同步工具类】
     * 1. CountDownLatch：倒计时门闩，一个或多个线程等待其他线程完成
     * 2. CyclicBarrier：循环栅栏，一组线程互相等待，到齐后一起执行
     * 3. Semaphore：信号量，控制同时访问资源的线程数量
     * 4. Exchanger：交换器，两个线程在同步点交换数据
     * 5. Phaser：阶段器，更灵活的CyclicBarrier（Java 7+）
     *
     * 【CountDownLatch vs CyclicBarrier】
     * - CountDownLatch：一个线程等多个线程，计数减到0就结束，不能复用
     * - CyclicBarrier：多个线程互相等待，到齐了一起走，可以重置复用
     * ============================================================
     */
    public static void demonstrateSyncUtils() {
        System.out.println("\n--- 8. 同步工具类 ---");

        // --- CountDownLatch 演示 ---
        System.out.println("【CountDownLatch 倒计时门闩】");
        System.out.println("场景：主线程等待3个子线程都完成后再继续");

        CountDownLatch latch = new CountDownLatch(3);  // 计数器初始值为3

        for (int i = 1; i <= 3; i++) {
            final int id = i;
            new Thread(() -> {
                try {
                    System.out.println("线程" + id + "：开始执行任务");
                    Thread.sleep(id * 100);  // 模拟任务执行
                    System.out.println("线程" + id + "：任务完成");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    latch.countDown();  // 计数器减1
                }
            }).start();
        }

        try {
            System.out.println("主线程：等待所有子线程完成...");
            latch.await();  // 等待计数器变为0
            System.out.println("主线程：所有子线程都完成了，继续执行");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // --- CyclicBarrier 演示 ---
        System.out.println("\n【CyclicBarrier 循环栅栏】");
        System.out.println("场景：3个运动员到齐后一起起跑");

        CyclicBarrier barrier = new CyclicBarrier(3, () -> {
            System.out.println("所有人都到齐了，开始比赛！");
        });

        for (int i = 1; i <= 3; i++) {
            final int id = i;
            new Thread(() -> {
                try {
                    System.out.println("运动员" + id + "：到达起跑线，等待其他人...");
                    Thread.sleep(id * 100);  // 模拟到达时间不同
                    barrier.await();  // 等待，直到所有线程都到齐
                    System.out.println("运动员" + id + "：出发！");
                } catch (InterruptedException | BrokenBarrierException e) {
                    e.printStackTrace();
                }
            }).start();
        }

        // 等待演示完成
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // --- Semaphore 演示 ---
        System.out.println("\n【Semaphore 信号量】");
        System.out.println("场景：5辆车，只有2个停车位");

        Semaphore semaphore = new Semaphore(2);  // 2个许可（停车位）

        for (int i = 1; i <= 5; i++) {
            final int carId = i;
            new Thread(() -> {
                try {
                    System.out.println("车" + carId + "：想要停车");
                    semaphore.acquire();  // 获取许可（停车位）
                    System.out.println("车" + carId + "：成功停车");
                    Thread.sleep(200);   // 模拟停车时间
                    System.out.println("车" + carId + "：离开停车场");
                    semaphore.release();  // 释放许可
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }).start();
        }

        // 等待演示完成
        try {
            Thread.sleep(1500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * ============================================================
     * 演示死锁
     *
     * 【什么是死锁】
     * - 两个或多个线程互相等待对方释放锁，导致都无法继续执行
     * - 死锁一旦发生，程序就卡住了
     *
     * 【死锁产生的四个必要条件】
     * 1. 互斥条件：资源只能被一个线程占用
     * 2. 持有并等待：线程持有至少一个资源，又请求其他被占有的资源
     * 3. 不可剥夺：资源不能被强行剥夺，只能由持有者主动释放
     * 4. 循环等待：线程之间形成头尾相接的循环等待关系
     *
     * 【如何避免死锁】
     * - 破坏循环等待：按固定顺序获取锁
     * - 破坏持有并等待：一次性申请所有需要的资源
     * - 破坏不可剥夺：尝试获取锁失败就释放已持有的锁（tryLock）
     * - 避免使用嵌套锁
     * ============================================================
     */
    public static void demonstrateDeadlock() {
        System.out.println("\n--- 9. 死锁（Deadlock）---");

        System.out.println("【死锁的四个必要条件】");
        System.out.println("1. 互斥条件：资源只能被一个线程使用");
        System.out.println("2. 持有并等待：持有一个资源，等待另一个");
        System.out.println("3. 不可剥夺：资源不能被强行夺走");
        System.out.println("4. 循环等待：线程形成循环等待链");

        System.out.println("\n【死锁演示（注意：会卡住，请谨慎运行）】");
        System.out.println("线程A：先获取锁1，再获取锁2");
        System.out.println("线程B：先获取锁2，再获取锁1");
        System.out.println("结果：A持有锁1等锁2，B持有锁2等锁1 → 死锁");
        System.out.println("（为了不影响后续演示，这里只说明不实际运行）");

        // 下面是死锁代码示例，注释掉不运行，因为会卡住
        /*
        Object lock1 = new Object();
        Object lock2 = new Object();

        new Thread(() -> {
            synchronized (lock1) {
                System.out.println("线程A：获取了锁1");
                try { Thread.sleep(100); } catch (InterruptedException e) {}
                System.out.println("线程A：等待锁2...");
                synchronized (lock2) {
                    System.out.println("线程A：获取了锁2");
                }
            }
        }).start();

        new Thread(() -> {
            synchronized (lock2) {
                System.out.println("线程B：获取了锁2");
                try { Thread.sleep(100); } catch (InterruptedException e) {}
                System.out.println("线程B：等待锁1...");
                synchronized (lock1) {
                    System.out.println("线程B：获取了锁1");
                }
            }
        }).start();
        */

        System.out.println("\n【避免死锁的方法】");
        System.out.println("1. 按固定顺序获取锁（破坏循环等待）");
        System.out.println("2. 一次性申请所有资源（破坏持有并等待）");
        System.out.println("3. 使用tryLock超时机制（破坏不可剥夺）");
        System.out.println("4. 尽量减少嵌套锁的使用");
    }
}
