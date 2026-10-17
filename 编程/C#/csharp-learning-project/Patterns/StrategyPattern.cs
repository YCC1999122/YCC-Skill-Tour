// ============================================================
// StrategyPattern.cs - 策略模式
// 定义一系列算法，把它们一个个封装起来，并且使它们可以互相替换
// 本模式使得算法可独立于使用它的客户而变化
// 核心：把算法的使用和算法的实现分开
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 抽象策略接口：排序策略
/// 定义所有排序算法的公共接口
/// </summary>
public interface ISortStrategy
{
    /// <summary>
    /// 排序方法
    /// </summary>
    /// <param name="array">要排序的数组</param>
    void Sort(int[] array);

    /// <summary>
    /// 策略名称
    /// </summary>
    string Name { get; }
}

/// <summary>
/// 具体策略：冒泡排序
/// </summary>
public class BubbleSortStrategy : ISortStrategy
{
    public string Name => "冒泡排序";

    public void Sort(int[] array)
    {
        int n = array.Length;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (array[j] > array[j + 1])
                {
                    // 交换
                    (array[j], array[j + 1]) = (array[j + 1], array[j]);
                }
            }
        }
    }
}

/// <summary>
/// 具体策略：快速排序
/// </summary>
public class QuickSortStrategy : ISortStrategy
{
    public string Name => "快速排序";

    public void Sort(int[] array)
    {
        QuickSort(array, 0, array.Length - 1);
    }

    private void QuickSort(int[] arr, int low, int high)
    {
        if (low < high)
        {
            int pivotIndex = Partition(arr, low, high);
            QuickSort(arr, low, pivotIndex - 1);
            QuickSort(arr, pivotIndex + 1, high);
        }
    }

    private int Partition(int[] arr, int low, int high)
    {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++)
        {
            if (arr[j] <= pivot)
            {
                i++;
                (arr[i], arr[j]) = (arr[j], arr[i]);
            }
        }
        (arr[i + 1], arr[high]) = (arr[high], arr[i + 1]);
        return i + 1;
    }
}

/// <summary>
/// 具体策略：选择排序
/// </summary>
public class SelectionSortStrategy : ISortStrategy
{
    public string Name => "选择排序";

    public void Sort(int[] array)
    {
        int n = array.Length;
        for (int i = 0; i < n - 1; i++)
        {
            int minIndex = i;
            for (int j = i + 1; j < n; j++)
            {
                if (array[j] < array[minIndex])
                {
                    minIndex = j;
                }
            }
            (array[i], array[minIndex]) = (array[minIndex], array[i]);
        }
    }
}

/// <summary>
/// 上下文（Context）：排序器
/// 持有一个策略对象的引用，客户端通过它来使用不同的策略
/// 策略模式的关键：上下文不关心具体用什么算法，只知道策略接口
/// </summary>
public class Sorter
{
    /// <summary>
    /// 当前使用的排序策略
    /// </summary>
    private ISortStrategy _strategy;

    /// <summary>
    /// 构造函数：传入初始策略
    /// </summary>
    public Sorter(ISortStrategy strategy)
    {
        _strategy = strategy;
    }

    /// <summary>
    /// 动态切换策略
    /// 这是策略模式的优势之一：运行时可以切换算法
    /// </summary>
    public void SetStrategy(ISortStrategy strategy)
    {
        _strategy = strategy;
    }

    /// <summary>
    /// 执行排序
    /// 调用策略的方法，具体实现由策略对象决定
    /// </summary>
    public void SortArray(int[] array)
    {
        Console.WriteLine($"    使用{_strategy.Name}进行排序...");
        var sw = System.Diagnostics.Stopwatch.StartNew();
        _strategy.Sort(array);
        sw.Stop();
        Console.WriteLine($"    排序完成，耗时：{sw.ElapsedTicks} ticks");
    }
}

// ============================================================
// 另一个策略模式示例：支付方式
// ============================================================

/// <summary>
/// 支付策略接口
/// </summary>
public interface IPaymentStrategy
{
    /// <summary>
    /// 支付方式名称
    /// </summary>
    string PaymentName { get; }

    /// <summary>
    /// 支付
    /// </summary>
    /// <param name="amount">支付金额</param>
    /// <returns>是否支付成功</returns>
    bool Pay(decimal amount);
}

/// <summary>
/// 具体策略：支付宝支付
/// </summary>
public class AlipayPayment : IPaymentStrategy
{
    public string PaymentName => "支付宝";

    public bool Pay(decimal amount)
    {
        Console.WriteLine($"    使用支付宝支付 {amount:C}");
        // 模拟支付过程
        return true;
    }
}

/// <summary>
/// 具体策略：微信支付
/// </summary>
public class WechatPayment : IPaymentStrategy
{
    public string PaymentName => "微信支付";

    public bool Pay(decimal amount)
    {
        Console.WriteLine($"    使用微信支付 {amount:C}");
        return true;
    }
}

/// <summary>
/// 具体策略：银行卡支付
/// </summary>
public class BankCardPayment : IPaymentStrategy
{
    public string PaymentName => "银行卡";

    public bool Pay(decimal amount)
    {
        Console.WriteLine($"    使用银行卡支付 {amount:C}");
        return true;
    }
}

/// <summary>
/// 上下文：购物车
/// </summary>
public class ShoppingCart
{
    /// <summary>
    /// 当前支付策略
    /// </summary>
    private IPaymentStrategy? _paymentStrategy;

    /// <summary>
    /// 购物车商品总额
    /// </summary>
    public decimal TotalAmount { get; set; }

    /// <summary>
    /// 设置支付方式
    /// </summary>
    public void SetPaymentStrategy(IPaymentStrategy paymentStrategy)
    {
        _paymentStrategy = paymentStrategy;
    }

    /// <summary>
    /// 结账
    /// </summary>
    public void Checkout()
    {
        if (_paymentStrategy == null)
        {
            Console.WriteLine("    请先选择支付方式！");
            return;
        }

        Console.WriteLine($"    购物车总额：{TotalAmount:C}");
        bool success = _paymentStrategy.Pay(TotalAmount);
        if (success)
        {
            Console.WriteLine("    支付成功！");
        }
    }
}
