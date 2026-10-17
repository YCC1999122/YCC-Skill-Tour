// ============================================================
// DecoratorPattern.cs - 装饰器模式
// 动态地给一个对象添加一些额外的职责
// 就增加功能来说，装饰器模式比生成子类更为灵活
// 核心：装饰器和被装饰对象实现相同的接口，装饰器持有被装饰对象的引用
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 抽象组件：咖啡接口
/// 定义所有咖啡和装饰器的公共接口
/// </summary>
public interface ICoffee
{
    /// <summary>
    /// 获取咖啡描述
    /// </summary>
    string Description { get; }

    /// <summary>
    /// 获取咖啡价格
    /// </summary>
    decimal Price { get; }
}

/// <summary>
/// 具体组件：简单咖啡（被装饰的基础对象）
/// </summary>
public class SimpleCoffee : ICoffee
{
    public string Description => "简单咖啡";
    public decimal Price => 10.0m;
}

/// <summary>
/// 具体组件：美式咖啡
/// </summary>
public class Americano : ICoffee
{
    public string Description => "美式咖啡";
    public decimal Price => 15.0m;
}

/// <summary>
/// 抽象装饰器：咖啡装饰器基类
/// 实现与被装饰对象相同的接口（ICoffee）
/// 持有一个被装饰对象的引用
/// 所有具体装饰器都继承自这个类
/// </summary>
public abstract class CoffeeDecorator : ICoffee
{
    /// <summary>
    /// 被装饰的咖啡对象
    /// </summary>
    protected ICoffee _coffee;

    /// <summary>
    /// 构造函数：传入被装饰的咖啡
    /// </summary>
    protected CoffeeDecorator(ICoffee coffee)
    {
        _coffee = coffee;
    }

    /// <summary>
    /// 描述：默认委托给被装饰对象
    /// 子类可以重写以添加新的描述
    /// </summary>
    public virtual string Description
    {
        get { return _coffee.Description; }
    }

    /// <summary>
    /// 价格：默认委托给被装饰对象
    /// 子类可以重写以添加新的价格
    /// </summary>
    public virtual decimal Price
    {
        get { return _coffee.Price; }
    }
}

/// <summary>
/// 具体装饰器：加奶
/// 给咖啡添加牛奶，同时增加价格
/// </summary>
public class MilkDecorator : CoffeeDecorator
{
    /// <summary>
    /// 牛奶价格
    /// </summary>
    private decimal _milkPrice = 3.0m;

    public MilkDecorator(ICoffee coffee) : base(coffee)
    {
    }

    /// <summary>
    /// 重写描述：在原有描述基础上添加"加奶"
    /// </summary>
    public override string Description
    {
        get { return $"{_coffee.Description} + 奶"; }
    }

    /// <summary>
    /// 重写价格：在原有价格基础上加上牛奶价格
    /// </summary>
    public override decimal Price
    {
        get { return _coffee.Price + _milkPrice; }
    }
}

/// <summary>
/// 具体装饰器：加糖
/// </summary>
public class SugarDecorator : CoffeeDecorator
{
    private decimal _sugarPrice = 1.0m;

    public SugarDecorator(ICoffee coffee) : base(coffee)
    {
    }

    public override string Description
    {
        get { return $"{_coffee.Description} + 糖"; }
    }

    public override decimal Price
    {
        get { return _coffee.Price + _sugarPrice; }
    }
}

/// <summary>
/// 具体装饰器：加奶泡（摩卡）
/// </summary>
public class WhippedCreamDecorator : CoffeeDecorator
{
    private decimal _creamPrice = 5.0m;

    public WhippedCreamDecorator(ICoffee coffee) : base(coffee)
    {
    }

    public override string Description
    {
        get { return $"{_coffee.Description} + 奶泡"; }
    }

    public override decimal Price
    {
        get { return _coffee.Price + _creamPrice; }
    }
}

/// <summary>
/// 具体装饰器：加巧克力
/// </summary>
public class ChocolateDecorator : CoffeeDecorator
{
    private decimal _chocolatePrice = 4.0m;

    public ChocolateDecorator(ICoffee coffee) : base(coffee)
    {
    }

    public override string Description
    {
        get { return $"{_coffee.Description} + 巧克力"; }
    }

    public override decimal Price
    {
        get { return _coffee.Price + _chocolatePrice; }
    }
}
