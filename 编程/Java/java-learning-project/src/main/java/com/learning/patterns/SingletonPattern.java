/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

/**
 * ============================================================
 * 单例模式（Singleton Pattern）
 *
 * 【定义】
 * 保证一个类只有一个实例，并提供一个全局访问点
 *
 * 【使用场景】
 * - 配置类：整个应用只需要一个配置对象
 * - 连接池/线程池：池化资源只需要一个实例
 * - 工具类：无状态的工具类
 * - 日志对象：统一的日志输出
 *
 * 【实现方式对比】
 * 1. 饿汉式：类加载时就创建实例，线程安全，但可能浪费内存
 * 2. 懒汉式：第一次使用时才创建，线程不安全（需要同步）
 * 3. 双重检查锁定（DCL）：懒汉式的优化，volatile + 两次判空
 * 4. 静态内部类：利用类加载机制，懒加载且线程安全（推荐）
 * 5. 枚举：最简单，线程安全，防止反射/序列化破坏（《Effective Java》推荐）
 *
 * 【单例被破坏的情况及防护】
 * - 反射：可以通过反射调用私有构造方法（枚举可以防止）
 * - 序列化：反序列化会创建新对象（需要readResolve方法，枚举可以防止）
 * - 克隆：clone()方法会创建新对象（不实现Cloneable接口）
 * ============================================================
 */
public class SingletonPattern {

    /**
     * 方式1：饿汉式（Eager Initialization）
     *
     * 【原理】
     * - 在类加载时就创建实例
     * - 基于类加载机制，线程安全（类初始化时的<clinit>方法是同步的）
     *
     * 【优点】
     * - 简单，线程安全
     * - 获取实例速度快
     *
     * 【缺点】
     * - 类加载时就创建，可能浪费内存（如果一直不用）
     * - 不能延迟加载
     */
    public static class EagerSingleton {
        // 类加载时就创建实例
        private static final EagerSingleton INSTANCE = new EagerSingleton();

        // 私有构造方法：防止外部new
        private EagerSingleton() {
        }

        // 提供全局访问点
        public static EagerSingleton getInstance() {
            return INSTANCE;
        }

        public void showMessage() {
            System.out.println("我是饿汉式单例");
        }
    }

    /**
     * 方式2：懒汉式（Lazy Initialization）- 线程不安全版本
     *
     * 【原理】
     * - 第一次使用时才创建实例（延迟加载）
     *
     * 【优点】
     * - 延迟加载，节省内存
     *
     * 【缺点】
     * - 线程不安全，多线程环境下可能创建多个实例
     * - 不推荐在生产环境使用
     */
    public static class LazySingletonUnsafe {
        private static LazySingletonUnsafe instance;

        private LazySingletonUnsafe() {
        }

        // 多线程下，如果两个线程同时通过if判断，会创建多个实例
        public static LazySingletonUnsafe getInstance() {
            if (instance == null) {          // 第一次判断：没有同步，可能多个线程同时进入
                instance = new LazySingletonUnsafe();  // 创建实例
            }
            return instance;
        }

        public void showMessage() {
            System.out.println("我是懒汉式单例（线程不安全）");
        }
    }

    /**
     * 方式2改进：懒汉式 - 线程安全版本（synchronized方法）
     *
     * 【缺点】
     * - 每次调用getInstance都要同步，效率低
     * - 实际上只有第一次创建时才需要同步
     */
    public static class LazySingletonSafe {
        private static LazySingletonSafe instance;

        private LazySingletonSafe() {
        }

        // 方法上加synchronized，保证线程安全
        // 但每次调用都要加锁，性能差
        public static synchronized LazySingletonSafe getInstance() {
            if (instance == null) {
                instance = new LazySingletonSafe();
            }
            return instance;
        }

        public void showMessage() {
            System.out.println("我是懒汉式单例（synchronized方法，线程安全但效率低）");
        }
    }

    /**
     * 方式3：双重检查锁定（Double-Checked Locking, DCL）
     *
     * 【原理】
     * - 两次判空 + 同步块
     * - 第一次判空：不需要同步，避免每次都加锁
     * - 同步块：保证只有一个线程创建实例
     * - 第二次判空：防止多个线程通过第一次判空后重复创建
     * - volatile：禁止指令重排序，防止拿到半初始化的对象
     *
     * 【为什么需要volatile】
     * new对象的过程可以分解为三步：
     * 1. 分配内存空间
     * 2. 初始化对象
     * 3. 将引用指向分配的内存
     *
     * JVM可能会对这三步进行指令重排，变成1→3→2
     * 如果重排了，另一个线程可能拿到一个还没初始化完成的对象
     * volatile可以禁止指令重排，保证对象完全初始化后才可见
     *
     * 【优点】
     * - 延迟加载
     * - 线程安全
     * - 效率较高（只有第一次创建时才同步）
     */
    public static class DCLSingleton {
        // volatile关键字：保证可见性，禁止指令重排
        private static volatile DCLSingleton instance;

        private DCLSingleton() {
        }

        public static DCLSingleton getInstance() {
            // 第一次判空：不需要加锁，快速返回（如果已实例化）
            if (instance == null) {
                // 同步块：保证只有一个线程创建实例
                synchronized (DCLSingleton.class) {
                    // 第二次判空：防止多个线程通过第一次判空后重复创建
                    if (instance == null) {
                        instance = new DCLSingleton();
                    }
                }
            }
            return instance;
        }

        public void showMessage() {
            System.out.println("我是双重检查锁定单例（DCL）");
        }
    }

    /**
     * 方式4：静态内部类（Static Inner Class / Initialization-on-demand holder）
     *
     * 【原理】
     * - 利用JVM的类加载机制保证线程安全
     * - 外部类加载时，静态内部类不会立即加载
     * - 第一次调用getInstance时，才会加载内部类
     * - 静态变量在类初始化时赋值，由JVM保证线程安全
     *
     * 【优点】
     * - 延迟加载（和懒汉一样）
     * - 线程安全（和饿汉一样，由JVM保证）
     * - 效率高（没有同步开销）
     * - 实现相对简单
     *
     * 【缺点】
     * - 需要一个内部类
     * - 可以被反射破坏
     */
    public static class StaticInnerSingleton {

        // 私有构造方法
        private StaticInnerSingleton() {
        }

        // 静态内部类：外部类加载时不会加载这个内部类
        private static class SingletonHolder {
            // 静态变量：类初始化时创建实例
            // JVM保证类初始化的<clinit>方法是线程安全的
            private static final StaticInnerSingleton INSTANCE = new StaticInnerSingleton();
        }

        // 第一次调用这个方法时，才会加载SingletonHolder类
        public static StaticInnerSingleton getInstance() {
            return SingletonHolder.INSTANCE;
        }

        public void showMessage() {
            System.out.println("我是静态内部类单例（推荐）");
        }
    }

    /**
     * 方式5：枚举单例（Enum Singleton）
     *
     * 【原理】
     * - 利用枚举类型的特性：每个枚举常量只有一个实例
     * - JVM保证枚举的线程安全和单例性
     *
     * 【优点】
     * - 最简单，代码最少
     * - 线程安全（JVM保证）
     * - 防止反射破坏（反射不能创建枚举实例）
     * - 防止序列化破坏（枚举的序列化有特殊处理）
     * - 《Effective Java》作者Josh Bloch推荐的方式
     *
     * 【缺点】
     * - 枚举类不能继承其他类（但可以实现接口）
     * - 某些场景下不够灵活
     */
    public enum EnumSingleton {
        // 枚举常量，就是单例对象
        INSTANCE;

        // 可以有成员变量和方法
        public void showMessage() {
            System.out.println("我是枚举单例（最推荐）");
        }
    }

    /**
     * 演示各种单例模式
     */
    public static void demonstrate() {
        System.out.println("【1. 饿汉式单例】");
        EagerSingleton s1 = EagerSingleton.getInstance();
        EagerSingleton s2 = EagerSingleton.getInstance();
        s1.showMessage();
        System.out.println("是否同一个实例: " + (s1 == s2));

        System.out.println("\n【2. 懒汉式单例（线程不安全）】");
        LazySingletonUnsafe s3 = LazySingletonUnsafe.getInstance();
        s3.showMessage();
        System.out.println("注意：多线程环境下可能有问题");

        System.out.println("\n【3. 双重检查锁定（DCL）】");
        DCLSingleton s4 = DCLSingleton.getInstance();
        DCLSingleton s5 = DCLSingleton.getInstance();
        s4.showMessage();
        System.out.println("是否同一个实例: " + (s4 == s5));

        System.out.println("\n【4. 静态内部类单例】");
        StaticInnerSingleton s6 = StaticInnerSingleton.getInstance();
        StaticInnerSingleton s7 = StaticInnerSingleton.getInstance();
        s6.showMessage();
        System.out.println("是否同一个实例: " + (s6 == s7));

        System.out.println("\n【5. 枚举单例】");
        EnumSingleton s8 = EnumSingleton.INSTANCE;
        EnumSingleton s9 = EnumSingleton.INSTANCE;
        s8.showMessage();
        System.out.println("是否同一个实例: " + (s8 == s9));
    }
}
