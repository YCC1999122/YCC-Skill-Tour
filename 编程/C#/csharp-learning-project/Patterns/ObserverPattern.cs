// ============================================================
// ObserverPattern.cs - 观察者模式
// 定义对象间的一种一对多的依赖关系
// 当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新
// 别名：发布-订阅模式（Publish/Subscribe）
// 在 C# 中可以直接使用 event/delegate 实现
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 抽象主题（Subject）接口
/// 定义了添加、删除、通知观察者的接口
/// </summary>
public interface ISubject
{
    /// <summary>
    /// 注册观察者（订阅）
    /// </summary>
    void Attach(IObserver observer);

    /// <summary>
    /// 移除观察者（取消订阅）
    /// </summary>
    void Detach(IObserver observer);

    /// <summary>
    /// 通知所有观察者
    /// </summary>
    void Notify();
}

/// <summary>
/// 抽象观察者（Observer）接口
/// 定义了收到通知时的更新接口
/// </summary>
public interface IObserver
{
    /// <summary>
    /// 更新方法
    /// </summary>
    /// <param name="subject">主题对象（用于获取状态）</param>
    void Update(ISubject subject);
}

/// <summary>
/// 具体主题：股票行情
/// 当股价变化时，通知所有观察者
/// </summary>
public class StockTicker : ISubject
{
    private List<IObserver> _observers = new List<IObserver>();

    /// <summary>
    /// 股票代码
    /// </summary>
    public string Symbol { get; set; } = string.Empty;

    /// <summary>
    /// 股票价格
    /// </summary>
    private decimal _price;

    /// <summary>
    /// 股票价格属性
    /// 当价格变化时，自动通知所有观察者
    /// </summary>
    public decimal Price
    {
        get { return _price; }
        set
        {
            if (_price != value)
            {
                _price = value;
                Notify();  // 价格变化，通知观察者
            }
        }
    }

    /// <summary>
    /// 注册观察者
    /// </summary>
    public void Attach(IObserver observer)
    {
        _observers.Add(observer);
    }

    /// <summary>
    /// 移除观察者
    /// </summary>
    public void Detach(IObserver observer)
    {
        _observers.Remove(observer);
    }

    /// <summary>
    /// 通知所有观察者
    /// </summary>
    public void Notify()
    {
        foreach (var observer in _observers)
        {
            observer.Update(this);
        }
    }
}

/// <summary>
/// 具体观察者：投资者
/// 当股票价格变化时，投资者收到通知
/// </summary>
public class Investor : IObserver
{
    /// <summary>
    /// 投资者姓名
    /// </summary>
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 买入价格（用于判断盈亏）
    /// </summary>
    public decimal BuyPrice { get; set; }

    /// <summary>
    /// 收到通知时的处理逻辑
    /// </summary>
    public void Update(ISubject subject)
    {
        if (subject is StockTicker stock)
        {
            decimal change = stock.Price - BuyPrice;
            string status = change >= 0 ? "盈利" : "亏损";
            Console.WriteLine($"    投资者{Name}收到通知：{stock.Symbol}现价{stock.Price:C}，{status}{Math.Abs(change):C}");
        }
    }
}

/// <summary>
/// 具体观察者：股票分析师
/// 另一个观察者，对价格变化有不同的反应
/// </summary>
public class StockAnalyst : IObserver
{
    public string Name { get; set; } = string.Empty;

    public void Update(ISubject subject)
    {
        if (subject is StockTicker stock)
        {
            Console.WriteLine($"    分析师{Name}分析：{stock.Symbol}当前价格{stock.Price:C}，建议关注");
        }
    }
}

// ============================================================
// 使用 C# event/delegate 实现观察者模式
// 这是 C# 中更常用、更简洁的方式
// ============================================================

/// <summary>
/// 事件参数：天气变化事件参数
/// </summary>
public class WeatherChangedEventArgs : EventArgs
{
    public float Temperature { get; set; }
    public float Humidity { get; set; }
    public string WeatherType { get; set; } = string.Empty;
}

/// <summary>
/// 发布者：天气预报站
/// 使用 C# event 实现观察者模式
/// </summary>
public class WeatherStation
{
    /// <summary>
    /// 天气变化事件
    /// 观察者通过订阅这个事件来接收通知
    /// </summary>
    public event EventHandler<WeatherChangedEventArgs>? WeatherChanged;

    private float _temperature;
    private float _humidity;
    private string _weatherType = "晴";

    /// <summary>
    /// 更新天气数据
    /// </summary>
    public void UpdateWeather(float temperature, float humidity, string weatherType)
    {
        _temperature = temperature;
        _humidity = humidity;
        _weatherType = weatherType;

        // 触发事件，通知所有订阅者
        OnWeatherChanged(new WeatherChangedEventArgs
        {
            Temperature = temperature,
            Humidity = humidity,
            WeatherType = weatherType
        });
    }

    /// <summary>
    /// 触发天气变化事件
    /// </summary>
    protected virtual void OnWeatherChanged(WeatherChangedEventArgs e)
    {
        WeatherChanged?.Invoke(this, e);
    }
}

/// <summary>
/// 订阅者：手机APP
/// 订阅天气变化事件
/// </summary>
public class PhoneApp
{
    public string AppName { get; set; } = string.Empty;

    /// <summary>
    /// 订阅天气站
    /// </summary>
    public void Subscribe(WeatherStation station)
    {
        station.WeatherChanged += OnWeatherChanged;
    }

    /// <summary>
    /// 取消订阅
    /// </summary>
    public void Unsubscribe(WeatherStation station)
    {
        station.WeatherChanged -= OnWeatherChanged;
    }

    /// <summary>
    /// 事件处理方法
    /// </summary>
    private void OnWeatherChanged(object? sender, WeatherChangedEventArgs e)
    {
        Console.WriteLine($"    [{AppName}] 天气更新：{e.WeatherType}，温度{e.Temperature}°C，湿度{e.Humidity}%");
    }
}

/// <summary>
/// 订阅者：电子显示屏
/// 另一个订阅者
/// </summary>
public class DisplayBoard
{
    public string Location { get; set; } = string.Empty;

    public void Subscribe(WeatherStation station)
    {
        station.WeatherChanged += (sender, e) =>
        {
            Console.WriteLine($"    [{Location}显示屏] 当前天气：{e.WeatherType}，{e.Temperature}°C");
        };
    }
}
