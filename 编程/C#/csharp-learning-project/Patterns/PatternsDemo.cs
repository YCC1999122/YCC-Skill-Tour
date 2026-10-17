// ============================================================
// PatternsDemo.cs - 设计模式演示入口
// 演示：单例模式、工厂模式、观察者模式、策略模式、
//       装饰器模式、建造者模式
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 设计模式演示类
/// </summary>
public class PatternsDemo
{
    /// <summary>
    /// 运行所有设计模式演示
    /// </summary>
    public static void RunAllDemos()
    {
        Console.WriteLine("========== 设计模式演示 ==========");
        Console.WriteLine();

        DemoSingleton();        // 单例模式
        Console.WriteLine();

        DemoFactory();          // 工厂模式
        Console.WriteLine();

        DemoObserver();         // 观察者模式
        Console.WriteLine();

        DemoStrategy();         // 策略模式
        Console.WriteLine();

        DemoDecorator();        // 装饰器模式
        Console.WriteLine();

        DemoBuilder();          // 建造者模式
        Console.WriteLine();

        Console.WriteLine("========== 设计模式演示结束 ==========");
    }

    // ============================================================
    // 1. 单例模式
    // ============================================================
    
    /// <summary>
    /// 演示单例模式
    /// 确保一个类只有一个实例
    /// </summary>
    public static void DemoSingleton()
    {
        Console.WriteLine("--- 单例模式演示 ---");

        // ---- 饿汉式 ----
        Console.WriteLine("1. 饿汉式单例：");
        Console.WriteLine("  （类加载时就创建实例）");
        EagerSingleton s1 = EagerSingleton.Instance;
        EagerSingleton s2 = EagerSingleton.Instance;
        Console.WriteLine($"  两个实例是否相同：{object.ReferenceEquals(s1, s2)}");
        s1.DoSomething();

        // ---- 懒汉式（双重检查锁定）----
        Console.WriteLine("\n2. 双重检查锁定单例：");
        Console.WriteLine("  （线程安全，延迟加载）");
        DoubleCheckSingleton d1 = DoubleCheckSingleton.Instance;
        DoubleCheckSingleton d2 = DoubleCheckSingleton.Instance;
        Console.WriteLine($"  两个实例是否相同：{object.ReferenceEquals(d1, d2)}");
        d1.DoSomething();

        // ---- Lazy<T> 实现 ----
        Console.WriteLine("\n3. Lazy<T> 单例（推荐）：");
        Console.WriteLine($"  访问前，实例是否已创建：{LazySingleton.IsInstanceCreated}");
        LazySingleton l1 = LazySingleton.Instance;  // 第一次访问，创建实例
        Console.WriteLine($"  访问后，实例是否已创建：{LazySingleton.IsInstanceCreated}");
        LazySingleton l2 = LazySingleton.Instance;
        Console.WriteLine($"  两个实例是否相同：{object.ReferenceEquals(l1, l2)}");
        l1.DoSomething();

        Console.WriteLine("\n单例模式应用场景：");
        Console.WriteLine("  - 配置管理器（全局只有一份配置）");
        Console.WriteLine("  - 日志记录器（全局只有一个日志实例）");
        Console.WriteLine("  - 数据库连接池（统一管理连接）");
        Console.WriteLine("  - 线程池（全局线程池管理）");
    }

    // ============================================================
    // 2. 工厂模式
    // ============================================================
    
    /// <summary>
    /// 演示工厂模式
    /// </summary>
    public static void DemoFactory()
    {
        Console.WriteLine("--- 工厂模式演示 ---");

        // ---- 简单工厂 ----
        Console.WriteLine("1. 简单工厂模式：");
        ILogger consoleLogger = SimpleLoggerFactory.CreateLogger("console");
        consoleLogger.WriteLog("这是一条控制台日志");

        ILogger fileLogger = SimpleLoggerFactory.CreateLogger("file");
        fileLogger.WriteLog("这是一条文件日志");

        // ---- 工厂方法 ----
        Console.WriteLine("\n2. 工厂方法模式：");
        Console.WriteLine("  定义创建对象的接口，让子类决定实例化哪个类");

        // 不同的工厂创建不同的产品
        ILoggerFactory consoleFactory = new ConsoleLoggerFactory();
        ILogger logger1 = consoleFactory.CreateLogger();
        logger1.WriteLog("通过控制台工厂创建的日志");

        ILoggerFactory dbFactory = new DatabaseLoggerFactory();
        ILogger logger2 = dbFactory.CreateLogger();
        logger2.WriteLog("通过数据库工厂创建的日志");

        Console.WriteLine("\n  工厂方法的好处：符合开闭原则，新增产品只需新增工厂类");

        // ---- 抽象工厂 ----
        Console.WriteLine("\n3. 抽象工厂模式：");
        Console.WriteLine("  创建一系列相关的产品（产品族）");

        Console.WriteLine("\n  XML环境工厂：");
        IAppFactory xmlFactory = new XmlEnvironmentFactory();
        ILogger xmlLogger = xmlFactory.CreateLogger();
        IConfigReader xmlReader = xmlFactory.CreateConfigReader();
        xmlLogger.WriteLog("XML环境下的日志");
        xmlReader.ReadConfig("connectionString");

        Console.WriteLine("\n  JSON环境工厂：");
        IAppFactory jsonFactory = new JsonEnvironmentFactory();
        ILogger jsonLogger = jsonFactory.CreateLogger();
        IConfigReader jsonReader = jsonFactory.CreateConfigReader();
        jsonLogger.WriteLog("JSON环境下的日志");
        jsonReader.ReadConfig("connectionString");
    }

    // ============================================================
    // 3. 观察者模式
    // ============================================================
    
    /// <summary>
    /// 演示观察者模式
    /// </summary>
    public static void DemoObserver()
    {
        Console.WriteLine("--- 观察者模式演示 ---");

        // ---- 经典观察者模式实现 ----
        Console.WriteLine("1. 经典观察者模式（接口实现）：");

        // 创建主题（股票行情）
        StockTicker stock = new StockTicker { Symbol = "AAPL", Price = 150.00m };

        // 创建观察者（投资者）
        Investor investor1 = new Investor { Name = "张三", BuyPrice = 140.00m };
        Investor investor2 = new Investor { Name = "李四", BuyPrice = 160.00m };
        StockAnalyst analyst = new StockAnalyst { Name = "王分析师" };

        // 注册观察者
        stock.Attach(investor1);
        stock.Attach(investor2);
        stock.Attach(analyst);

        // 价格变化（触发通知）
        Console.WriteLine("  股价从150涨到155：");
        stock.Price = 155.00m;

        // 移除一个观察者
        stock.Detach(investor1);
        Console.WriteLine("\n  张三取消订阅后，股价跌到145：");
        stock.Price = 145.00m;

        // ---- C# event/delegate 实现 ----
        Console.WriteLine("\n2. C# event/delegate 实现（推荐）：");

        // 创建发布者（气象站）
        WeatherStation station = new WeatherStation();

        // 创建订阅者
        PhoneApp phoneApp = new PhoneApp { AppName = "天气通APP" };
        DisplayBoard displayBoard = new DisplayBoard { Location = "广场" };

        // 订阅
        phoneApp.Subscribe(station);
        displayBoard.Subscribe(station);

        // 天气变化（触发事件）
        Console.WriteLine("  天气更新为晴天：");
        station.UpdateWeather(25.0f, 60.0f, "晴");

        Console.WriteLine("\n  天气更新为雨天：");
        station.UpdateWeather(18.0f, 85.0f, "雨");

        // 取消订阅
        phoneApp.Unsubscribe(station);
        Console.WriteLine("\n  手机APP取消订阅后，天气更新：");
        station.UpdateWeather(22.0f, 70.0f, "多云");
    }

    // ============================================================
    // 4. 策略模式
    // ============================================================
    
    /// <summary>
    /// 演示策略模式
    /// </summary>
    public static void DemoStrategy()
    {
        Console.WriteLine("--- 策略模式演示 ---");

        // ---- 排序策略示例 ----
        Console.WriteLine("1. 排序策略：");

        int[] numbers = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 10 };
        Console.WriteLine($"  原始数组：{string.Join(", ", numbers)}");

        // 创建排序器，初始使用冒泡排序
        Sorter sorter = new Sorter(new BubbleSortStrategy());
        int[] arr1 = (int[])numbers.Clone();
        sorter.SortArray(arr1);
        Console.WriteLine($"  排序结果：{string.Join(", ", arr1)}");

        // 切换为快速排序
        Console.WriteLine("\n  切换为快速排序：");
        sorter.SetStrategy(new QuickSortStrategy());
        int[] arr2 = (int[])numbers.Clone();
        sorter.SortArray(arr2);
        Console.WriteLine($"  排序结果：{string.Join(", ", arr2)}");

        // 切换为选择排序
        Console.WriteLine("\n  切换为选择排序：");
        sorter.SetStrategy(new SelectionSortStrategy());
        int[] arr3 = (int[])numbers.Clone();
        sorter.SortArray(arr3);
        Console.WriteLine($"  排序结果：{string.Join(", ", arr3)}");

        // ---- 支付策略示例 ----
        Console.WriteLine("\n2. 支付策略：");

        ShoppingCart cart = new ShoppingCart { TotalAmount = 599.00m };

        // 选择支付宝支付
        Console.WriteLine("  选择支付宝支付：");
        cart.SetPaymentStrategy(new AlipayPayment());
        cart.Checkout();

        // 切换为微信支付
        Console.WriteLine("\n  切换为微信支付：");
        cart.SetPaymentStrategy(new WechatPayment());
        cart.Checkout();

        // 切换为银行卡支付
        Console.WriteLine("\n  切换为银行卡支付：");
        cart.SetPaymentStrategy(new BankCardPayment());
        cart.Checkout();

        Console.WriteLine("\n策略模式的优点：");
        Console.WriteLine("  - 算法可以自由切换");
        Console.WriteLine("  - 避免使用多重条件判断");
        Console.WriteLine("  - 扩展性好（新增算法只需新增策略类）");
    }

    // ============================================================
    // 5. 装饰器模式
    // ============================================================
    
    /// <summary>
    /// 演示装饰器模式
    /// 动态给对象添加功能，比继承更灵活
    /// </summary>
    public static void DemoDecorator()
    {
        Console.WriteLine("--- 装饰器模式演示 ---");

        Console.WriteLine("咖啡定制系统：");
        Console.WriteLine();

        // 基础咖啡
        ICoffee simpleCoffee = new SimpleCoffee();
        Console.WriteLine($"  1. {simpleCoffee.Description} - {simpleCoffee.Price:C}");

        // 加奶的咖啡
        ICoffee milkCoffee = new MilkDecorator(new SimpleCoffee());
        Console.WriteLine($"  2. {milkCoffee.Description} - {milkCoffee.Price:C}");

        // 加奶加糖的咖啡（多层装饰）
        ICoffee milkSugarCoffee = new SugarDecorator(
            new MilkDecorator(new SimpleCoffee()));
        Console.WriteLine($"  3. {milkSugarCoffee.Description} - {milkSugarCoffee.Price:C}");

        // 更复杂的装饰（美式+奶+糖+奶泡+巧克力）
        ICoffee fancyCoffee = new ChocolateDecorator(
            new WhippedCreamDecorator(
                new SugarDecorator(
                    new MilkDecorator(new Americano()))));
        Console.WriteLine($"  4. {fancyCoffee.Description} - {fancyCoffee.Price:C}");

        // 只用糖的美式
        ICoffee americanoWithSugar = new SugarDecorator(new Americano());
        Console.WriteLine($"  5. {americanoWithSugar.Description} - {americanoWithSugar.Price:C}");

        Console.WriteLine("\n装饰器模式的优点：");
        Console.WriteLine("  - 比继承更灵活，可以动态组合功能");
        Console.WriteLine("  - 可以有多种排列组合，子类爆炸问题");
        Console.WriteLine("  - 符合开闭原则，新增装饰器不影响现有代码");
    }

    // ============================================================
    // 6. 建造者模式
    // ============================================================
    
    /// <summary>
    /// 演示建造者模式
    /// </summary>
    public static void DemoBuilder()
    {
        Console.WriteLine("--- 建造者模式演示 ---");

        // ---- 经典建造者模式（Director + Builder）----
        Console.WriteLine("1. 经典建造者模式（电脑定制）：");

        ComputerDirector director = new ComputerDirector();

        // 游戏本
        Console.WriteLine("  【游戏本】");
        IComputerBuilder gamingBuilder = new GamingComputerBuilder();
        Computer gamingPC = director.Construct(gamingBuilder);
        gamingPC.ShowConfig();

        Console.WriteLine();

        // 办公本
        Console.WriteLine("  【办公本】");
        IComputerBuilder officeBuilder = new OfficeComputerBuilder();
        Computer officePC = director.Construct(officeBuilder);
        officePC.ShowConfig();

        Console.WriteLine();

        // 只有主机的游戏本
        Console.WriteLine("  【游戏本（仅主机）】");
        IComputerBuilder gamingHostBuilder = new GamingComputerBuilder();
        Computer gamingHost = director.ConstructHostOnly(gamingHostBuilder);
        gamingHost.ShowConfig();

        // ---- Fluent Builder 风格 ----
        Console.WriteLine("\n2. Fluent Builder（邮件构建）：");

        Console.WriteLine("  【简单邮件】");
        Email email1 = new EmailBuilder()
            .From("sender@example.com")
            .To("user@example.com")
            .Subject("测试邮件")
            .Body("这是一封测试邮件")
            .Build();
        email1.ShowInfo();

        Console.WriteLine();

        Console.WriteLine("  【带附件的HTML邮件】");
        Email email2 = new EmailBuilder()
            .From("system@company.com")
            .To("admin@company.com")
            .Subject("系统告警")
            .Body("<h1>CPU使用率过高！</h1>")
            .IsHtml(true)
            .AddAttachment("report.pdf")
            .AddAttachment("log.txt")
            .Build();
        email2.ShowInfo();

        Console.WriteLine("\n建造者模式适用场景：");
        Console.WriteLine("  - 对象的创建比较复杂（很多参数）");
        Console.WriteLine("  - 需要创建不同表示的同类对象");
        Console.WriteLine("  - 需要构造过程与表示分离");
    }
}
