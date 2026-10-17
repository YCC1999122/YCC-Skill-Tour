// ============================================================
// Interfaces.cs - 接口定义与实现
// 演示：接口 interface、接口实现、显式接口实现
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 动物接口
/// 接口定义了一组行为规范（契约），但不提供实现
/// 接口名通常以 I 开头（命名约定）
/// 接口只能包含方法、属性、事件、索引器的声明，不能有字段
/// 接口成员默认为 public，不能指定访问修饰符
/// </summary>
public interface IAnimal
{
    /// <summary>
    /// 发出叫声（接口方法声明，没有实现）
    /// </summary>
    void MakeSound();

    /// <summary>
    /// 动物名称（接口属性声明）
    /// </summary>
    string Name { get; set; }

    /// <summary>
    /// 移动方式
    /// </summary>
    void Move();
}

/// <summary>
/// 飞翔接口
/// 一个类可以实现多个接口（多继承的一种方式）
/// </summary>
public interface IFlyable
{
    /// <summary>
    /// 飞翔
    /// </summary>
    void Fly();

    /// <summary>
    /// 最大飞行高度
    /// </summary>
    double MaxHeight { get; }
}

/// <summary>
/// 狗类 - 实现 IAnimal 接口
/// 类可以继承一个基类，同时实现多个接口
/// </summary>
public class Dog : IAnimal
{
    /// <summary>
    /// 实现接口的 Name 属性
    /// </summary>
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 实现接口的 MakeSound 方法
    /// 必须与接口定义签名完全一致
    /// </summary>
    public void MakeSound()
    {
        Console.WriteLine($"{Name}：汪汪汪！");
    }

    /// <summary>
    /// 实现接口的 Move 方法
    /// </summary>
    public void Move()
    {
        Console.WriteLine($"{Name} 用四条腿跑步。");
    }

    /// <summary>
    /// 狗特有的方法：看家
    /// 这不是接口要求的，是狗类自己的方法
    /// </summary>
    public void GuardHouse()
    {
        Console.WriteLine($"{Name} 正在看家。");
    }
}

/// <summary>
/// 鸟类 - 实现 IAnimal 和 IFlyable 两个接口
/// 演示一个类实现多个接口
/// </summary>
public class Bird : IAnimal, IFlyable
{
    /// <summary>
    /// 鸟的名字
    /// </summary>
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 实现 IAnimal 接口的 MakeSound
    /// </summary>
    public void MakeSound()
    {
        Console.WriteLine($"{Name}：叽叽喳喳！");
    }

    /// <summary>
    /// 实现 IAnimal 接口的 Move
    /// </summary>
    public void Move()
    {
        Fly();  // 鸟通过飞移动
    }

    /// <summary>
    /// 实现 IFlyable 接口的 Fly
    /// </summary>
    public void Fly()
    {
        Console.WriteLine($"{Name} 正在天空中飞翔。");
    }

    /// <summary>
    /// 实现 IFlyable 接口的 MaxHeight 属性
    /// </summary>
    public double MaxHeight
    {
        get { return 1000; }  // 鸟最高飞1000米
    }
}

/// <summary>
/// 机器人接口
/// 用于演示显式接口实现
/// </summary>
public interface IRobot
{
    /// <summary>
    /// 机器人工作
    /// </summary>
    void Work();
}

/// <summary>
/// 工人接口
/// 用于演示显式接口实现
/// </summary>
public interface IWorker
{
    /// <summary>
    /// 工人工作
    /// </summary>
    void Work();
}

/// <summary>
/// 机器人类 - 实现两个都有 Work 方法的接口
/// 当两个接口有同名方法时，需要使用"显式接口实现"来区分
/// </summary>
public class Robot : IRobot, IWorker
{
    /// <summary>
    /// 机器人自己的 Work 方法
    /// </summary>
    public void Work()
    {
        Console.WriteLine("机器人在工作（类自己的方法）。");
    }

    /// <summary>
    /// 显式接口实现 IRobot.Work
    /// 语法：接口名.方法名
    /// 没有访问修饰符，只能通过接口引用调用
    /// </summary>
    void IRobot.Work()
    {
        Console.WriteLine("机器人以 IRobot 的方式工作（显式接口实现）。");
    }

    /// <summary>
    /// 显式接口实现 IWorker.Work
    /// </summary>
    void IWorker.Work()
    {
        Console.WriteLine("机器人以 IWorker 的方式工作（显式接口实现）。");
    }
}
