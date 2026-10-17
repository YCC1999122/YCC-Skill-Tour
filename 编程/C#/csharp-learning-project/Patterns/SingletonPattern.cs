// ============================================================
// SingletonPattern.cs - 单例模式
// 确保一个类只有一个实例，并提供全局访问点
// 四种实现方式：饿汉式、懒汉式、双重检查锁定、Lazy<T>
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 单例模式 - 饿汉式（Eager Initialization）
/// 特点：程序启动时就创建实例，线程安全（由CLR保证静态字段初始化的线程安全）
/// 优点：实现简单，线程安全
/// 缺点：无论是否使用，实例都会被创建，可能浪费资源
/// 适用：实例不大、创建开销小、确定会使用的情况
/// </summary>
public class EagerSingleton
{
    /// <summary>
    /// 私有静态字段：在类加载时就创建唯一实例
    /// readonly 保证只能在静态构造函数或声明时赋值
    /// </summary>
    private static readonly EagerSingleton _instance = new EagerSingleton();

    /// <summary>
    /// 私有构造函数：外部不能 new 这个类
    /// 这是单例模式的关键 - 禁止外部创建实例
    /// </summary>
    private EagerSingleton()
    {
        Console.WriteLine("  [饿汉式] 单例实例被创建");
    }

    /// <summary>
    /// 公共静态属性：提供全局访问点
    /// </summary>
    public static EagerSingleton Instance
    {
        get { return _instance; }
    }

    /// <summary>
    /// 示例方法
    /// </summary>
    public void DoSomething()
    {
        Console.WriteLine("  [饿汉式] 单例正在执行操作");
    }
}

/// <summary>
/// 单例模式 - 简单懒汉式（Lazy Initialization - Not Thread Safe）
/// 特点：第一次使用时才创建实例（延迟加载）
/// 缺点：线程不安全，多线程环境下可能创建多个实例
/// 注意：仅适用于单线程环境
/// </summary>
public class SimpleLazySingleton
{
    /// <summary>
    /// 私有静态字段：初始为 null，不创建实例
    /// </summary>
    private static SimpleLazySingleton? _instance;

    /// <summary>
    /// 私有构造函数
    /// </summary>
    private SimpleLazySingleton()
    {
        Console.WriteLine("  [简单懒汉式] 单例实例被创建");
    }

    /// <summary>
    /// 公共静态属性：第一次访问时才创建实例
    /// </summary>
    public static SimpleLazySingleton Instance
    {
        get
        {
            // 检查：如果实例不存在则创建
            // 多线程下可能同时通过 null 检查，创建多个实例
            if (_instance == null)
            {
                _instance = new SimpleLazySingleton();
            }
            return _instance;
        }
    }

    public void DoSomething()
    {
        Console.WriteLine("  [简单懒汉式] 单例正在执行操作");
    }
}

/// <summary>
/// 单例模式 - 双重检查锁定（Double-Checked Locking）
/// 特点：线程安全的懒汉式实现，只在第一次创建时加锁
/// 优点：线程安全，延迟加载，性能较好（只有第一次需要锁）
/// 这是经典的多线程环境下单例实现方式
/// </summary>
public class DoubleCheckSingleton
{
    /// <summary>
    /// 私有静态字段
    /// volatile 关键字确保多线程环境下字段的可见性
    /// 防止编译器优化导致的指令重排问题
    /// </summary>
    private static volatile DoubleCheckSingleton? _instance;

    /// <summary>
    /// 锁对象，用于同步
    /// 专门创建一个对象作为锁，避免锁其他公共资源
    /// </summary>
    private static readonly object _lock = new object();

    /// <summary>
    /// 私有构造函数
    /// </summary>
    private DoubleCheckSingleton()
    {
        Console.WriteLine("  [双重检查锁定] 单例实例被创建");
    }

    /// <summary>
    /// 公共静态属性
    /// 双重检查：先检查一次（无锁），再加锁，加锁后再检查一次
    /// 目的：只有第一次创建时需要加锁，后续访问不需要锁，提高性能
    /// </summary>
    public static DoubleCheckSingleton Instance
    {
        get
        {
            // 第一次检查：如果实例已存在，直接返回（不需要锁，提高性能）
            if (_instance == null)
            {
                // 加锁：确保只有一个线程能进入临界区
                lock (_lock)
                {
                    // 第二次检查：在锁内再次检查，防止多个线程同时通过第一次检查
                    // （比如两个线程同时通过了第一次检查，第一个创建了实例，
                    //  第二个进入锁后如果不再次检查，会再创建一个实例）
                    if (_instance == null)
                    {
                        _instance = new DoubleCheckSingleton();
                    }
                }
            }
            return _instance;
        }
    }

    public void DoSomething()
    {
        Console.WriteLine("  [双重检查锁定] 单例正在执行操作");
    }
}

/// <summary>
/// 单例模式 - Lazy<T> 实现（推荐！）
/// 特点：使用 .NET 内置的 Lazy<T> 类实现延迟初始化
/// 优点：
///   - 线程安全（Lazy<T> 默认就是线程安全的）
///   - 延迟加载
///   - 代码简洁，不需要自己处理锁和双重检查
///   - 性能优化好（.NET 框架内部优化）
/// LazyThreadSafetyMode 有三种模式：
///   - ExecutionAndPublication: 默认，最安全，使用锁
///   - PublicationOnly: 多个线程可以同时创建，但只保留一个
///   - None: 非线程安全
/// </summary>
public class LazySingleton
{
    /// <summary>
    /// 使用 Lazy<T> 延迟创建实例
    /// Lazy<T> 会自动处理线程安全和延迟初始化
    /// </summary>
    private static readonly Lazy<LazySingleton> _lazyInstance =
        new Lazy<LazySingleton>(() => new LazySingleton());

    /// <summary>
    /// 私有构造函数
    /// </summary>
    private LazySingleton()
    {
        Console.WriteLine("  [Lazy<T>] 单例实例被创建");
    }

    /// <summary>
    /// 公共静态属性
    /// 通过 _lazyInstance.Value 获取实例
    /// </summary>
    public static LazySingleton Instance
    {
        get { return _lazyInstance.Value; }
    }

    /// <summary>
    /// 是否已创建实例
    /// </summary>
    public static bool IsInstanceCreated
    {
        get { return _lazyInstance.IsValueCreated; }
    }

    public void DoSomething()
    {
        Console.WriteLine("  [Lazy<T>] 单例正在执行操作");
    }
}
