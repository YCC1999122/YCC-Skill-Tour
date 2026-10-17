// ============================================================
// EventsDemo.cs - 委托与事件
// 演示：委托 delegate、事件 event、事件发布与订阅
// ============================================================

namespace CSharpLearning.OOP;

// ============================================================
// 委托（Delegate）
// 委托是一种类型，它存储对方法的引用
// 可以把委托理解为"方法的指针"或"方法的容器"
// 委托常用于回调方法和事件处理
// ============================================================

/// <summary>
/// 委托定义：表示一个接收两个int参数、返回int的方法
/// 使用 delegate 关键字定义委托类型
/// 委托的签名必须与它引用的方法签名一致
/// </summary>
/// <param name="a">第一个整数</param>
/// <param name="b">第二个整数</param>
/// <returns>计算结果</returns>
public delegate int MathOperation(int a, int b);

/// <summary>
/// 自定义事件参数类
/// 继承自 EventArgs，用于在事件触发时传递数据
/// 命名约定：以 EventArgs 结尾
/// </summary>
public class PriceChangedEventArgs : EventArgs
{
    /// <summary>
    /// 旧价格
    /// </summary>
    public decimal OldPrice { get; }

    /// <summary>
    /// 新价格
    /// </summary>
    public decimal NewPrice { get; }

    /// <summary>
    /// 构造函数
    /// </summary>
    public PriceChangedEventArgs(decimal oldPrice, decimal newPrice)
    {
        OldPrice = oldPrice;
        NewPrice = newPrice;
    }
}

/// <summary>
/// 产品类 - 事件发布者（Publisher）
/// 当价格变化时，触发 PriceChanged 事件
/// 发布者不知道谁会订阅事件，只负责发布通知
/// </summary>
public class Product
{
    // 私有字段：产品价格
    private decimal _price;

    /// <summary>
    /// 产品名称
    /// </summary>
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 事件定义：价格变化事件
    /// 使用 event 关键字声明事件
    /// EventHandler<T> 是 .NET 提供的泛型事件委托
    /// 事件是委托的一种特殊用法，提供了更严格的封装
    /// 外部只能用 += 和 -= 来订阅/取消订阅，不能直接调用
    /// </summary>
    public event EventHandler<PriceChangedEventArgs>? PriceChanged;

    /// <summary>
    /// 产品价格
    /// 在 set 中检查价格是否真的变化了，如果变化则触发事件
    /// </summary>
    public decimal Price
    {
        get { return _price; }
        set
        {
            if (_price != value)
            {
                decimal oldPrice = _price;  // 保存旧价格
                _price = value;             // 更新价格

                // 触发事件（通知所有订阅者）
                OnPriceChanged(oldPrice, value);
            }
        }
    }

    /// <summary>
    /// 受保护的虚方法：触发价格变化事件
    /// 约定：事件触发方法命名为 On + 事件名
    /// 设为 protected virtual 方便子类重写
    /// </summary>
    protected virtual void OnPriceChanged(decimal oldPrice, decimal newPrice)
    {
        // 如果有订阅者（事件不为null），则触发事件
        // ?.Invoke 是 C# 6+ 的写法：如果不为null则调用
        PriceChanged?.Invoke(this, new PriceChangedEventArgs(oldPrice, newPrice));
    }
}

/// <summary>
/// 邮件通知类 - 事件订阅者（Subscriber）
/// 订阅产品价格变化事件，当价格变化时发送邮件
/// </summary>
public class EmailNotifier
{
    /// <summary>
    /// 订阅产品的价格变化事件
    /// </summary>
    public void Subscribe(Product product)
    {
        // 使用 += 操作符订阅事件
        // 方法签名必须与事件委托匹配
        product.PriceChanged += OnProductPriceChanged;
    }

    /// <summary>
    /// 取消订阅
    /// </summary>
    public void Unsubscribe(Product product)
    {
        // 使用 -= 操作符取消订阅
        product.PriceChanged -= OnProductPriceChanged;
    }

    /// <summary>
    /// 事件处理方法：当产品价格变化时被调用
    /// 标准事件处理程序签名：object sender, EventArgs e
    /// sender 是触发事件的对象，e 是事件参数
    /// </summary>
    private void OnProductPriceChanged(object? sender, PriceChangedEventArgs e)
    {
        // sender 可以转为 Product 获取更多信息
        if (sender is Product product)
        {
            Console.WriteLine($"[邮件通知] 产品\"{product.Name}\"价格变化：{e.OldPrice:C} -> {e.NewPrice:C}");
        }
    }
}

/// <summary>
/// 短信通知类 - 另一个事件订阅者
/// 演示多个订阅者同时订阅同一个事件
/// </summary>
public class SmsNotifier
{
    /// <summary>
    /// 订阅事件
    /// </summary>
    public void Subscribe(Product product)
    {
        product.PriceChanged += (sender, e) =>
        {
            // 也可以用 Lambda 表达式作为事件处理程序
            if (sender is Product p)
            {
                Console.WriteLine($"[短信通知] {p.Name}价格从{e.OldPrice:C}变为{e.NewPrice:C}");
            }
        };
    }
}

/// <summary>
/// 计算器类 - 委托的实际使用演示
/// </summary>
public class Calculator
{
    /// <summary>
    /// 加法方法，与 MathOperation 委托签名匹配
    /// </summary>
    public static int Add(int a, int b)
    {
        return a + b;
    }

    /// <summary>
    /// 减法方法，与 MathOperation 委托签名匹配
    /// </summary>
    public static int Subtract(int a, int b)
    {
        return a - b;
    }

    /// <summary>
    /// 乘法方法
    /// </summary>
    public static int Multiply(int a, int b)
    {
        return a * b;
    }

    /// <summary>
    /// 使用委托作为参数的方法
    /// 这是"策略模式"的基础：把算法（方法）作为参数传递
    /// </summary>
    /// <param name="a">第一个数</param>
    /// <param name="b">第二个数</param>
    /// <param name="operation">运算委托（决定做什么运算）</param>
    /// <returns>运算结果</returns>
    public static int ExecuteOperation(int a, int b, MathOperation operation)
    {
        // 通过委托调用方法
        return operation(a, b);
    }
}
