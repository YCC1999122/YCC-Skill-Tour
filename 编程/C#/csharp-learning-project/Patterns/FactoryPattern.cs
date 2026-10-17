// ============================================================
// FactoryPattern.cs - 工厂模式
// 三种工厂模式：简单工厂、工厂方法、抽象工厂
// 工厂模式的核心思想：将对象的创建和使用分离
// ============================================================

namespace CSharpLearning.Patterns;

// ============================================================
// 产品接口和具体产品
// ============================================================

/// <summary>
/// 产品接口：日志记录器
/// 所有具体产品都实现这个接口
/// </summary>
public interface ILogger
{
    /// <summary>
    /// 写日志
    /// </summary>
    void WriteLog(string message);
}

/// <summary>
/// 具体产品：控制台日志记录器
/// </summary>
public class ConsoleLogger : ILogger
{
    public void WriteLog(string message)
    {
        Console.WriteLine($"    [控制台日志] {message}");
    }
}

/// <summary>
/// 具体产品：文件日志记录器
/// </summary>
public class FileLogger : ILogger
{
    public void WriteLog(string message)
    {
        Console.WriteLine($"    [文件日志] 写入文件：{message}");
    }
}

/// <summary>
/// 具体产品：数据库日志记录器
/// </summary>
public class DatabaseLogger : ILogger
{
    public void WriteLog(string message)
    {
        Console.WriteLine($"    [数据库日志] 写入数据库：{message}");
    }
}

// ============================================================
// 1. 简单工厂模式（Simple Factory）
// 不属于 GoF 23种设计模式，但很常用
// 一个工厂类根据参数创建不同的产品
// ============================================================

/// <summary>
/// 简单工厂
/// 一个工厂类，根据传入的类型创建对应的产品
/// 缺点：新增产品需要修改工厂类（违反开闭原则）
/// </summary>
public class SimpleLoggerFactory
{
    /// <summary>
    /// 根据类型创建日志记录器
    /// </summary>
    /// <param name="loggerType">日志类型</param>
    /// <returns>日志记录器实例</returns>
    public static ILogger CreateLogger(string loggerType)
    {
        switch (loggerType.ToLower())
        {
            case "console":
                return new ConsoleLogger();
            case "file":
                return new FileLogger();
            case "database":
                return new DatabaseLogger();
            default:
                throw new ArgumentException($"不支持的日志类型：{loggerType}");
        }
    }
}

// ============================================================
// 2. 工厂方法模式（Factory Method）
// 定义一个创建对象的接口，让子类决定实例化哪个类
// 工厂方法使一个类的实例化延迟到其子类
// ============================================================

/// <summary>
/// 抽象工厂接口：日志工厂
/// 定义创建产品的接口
/// </summary>
public interface ILoggerFactory
{
    /// <summary>
    /// 工厂方法：创建日志记录器
    /// 具体由子类来实现创建哪种产品
    /// </summary>
    ILogger CreateLogger();
}

/// <summary>
/// 具体工厂：控制台日志工厂
/// 负责创建控制台日志记录器
/// </summary>
public class ConsoleLoggerFactory : ILoggerFactory
{
    public ILogger CreateLogger()
    {
        return new ConsoleLogger();
    }
}

/// <summary>
/// 具体工厂：文件日志工厂
/// </summary>
public class FileLoggerFactory : ILoggerFactory
{
    public ILogger CreateLogger()
    {
        return new FileLogger();
    }
}

/// <summary>
/// 具体工厂：数据库日志工厂
/// </summary>
public class DatabaseLoggerFactory : ILoggerFactory
{
    public ILogger CreateLogger()
    {
        return new DatabaseLogger();
    }
}

// ============================================================
// 3. 抽象工厂模式（Abstract Factory）
// 提供一个创建一系列相关或相互依赖对象的接口
// 而无需指定它们具体的类
// ============================================================

// ---- 第一类产品：日志 ----
// （上面已经定义了 ILogger 及其实现）

// ---- 第二类产品：配置读取器 ----

/// <summary>
/// 抽象产品：配置读取器接口
/// </summary>
public interface IConfigReader
{
    /// <summary>
    /// 读取配置
    /// </summary>
    string ReadConfig(string key);
}

/// <summary>
/// 具体产品：XML配置读取器
/// </summary>
public class XmlConfigReader : IConfigReader
{
    public string ReadConfig(string key)
    {
        Console.WriteLine($"    [XML配置] 读取配置项：{key}");
        return $"<xml>{key}_value</xml>";
    }
}

/// <summary>
/// 具体产品：JSON配置读取器
/// </summary>
public class JsonConfigReader : IConfigReader
{
    public string ReadConfig(string key)
    {
        Console.WriteLine($"    [JSON配置] 读取配置项：{key}");
        return $"{{\"{key}\": \"value\"}}";
    }
}

/// <summary>
/// 抽象工厂接口：应用工厂
/// 可以创建一系列相关的产品（日志 + 配置）
/// 每个具体工厂创建一个"产品族"
/// </summary>
public interface IAppFactory
{
    /// <summary>
    /// 创建日志记录器
    /// </summary>
    ILogger CreateLogger();

    /// <summary>
    /// 创建配置读取器
    /// </summary>
    IConfigReader CreateConfigReader();
}

/// <summary>
/// 具体工厂：XML环境工厂
/// 这个工厂创建的产品族是：控制台日志 + XML配置
/// </summary>
public class XmlEnvironmentFactory : IAppFactory
{
    public ILogger CreateLogger()
    {
        return new ConsoleLogger();  // XML环境用控制台日志
    }

    public IConfigReader CreateConfigReader()
    {
        return new XmlConfigReader();  // XML环境用XML配置
    }
}

/// <summary>
/// 具体工厂：JSON环境工厂
/// 这个工厂创建的产品族是：文件日志 + JSON配置
/// </summary>
public class JsonEnvironmentFactory : IAppFactory
{
    public ILogger CreateLogger()
    {
        return new FileLogger();  // JSON环境用文件日志
    }

    public IConfigReader CreateConfigReader()
    {
        return new JsonConfigReader();  // JSON环境用JSON配置
    }
}
