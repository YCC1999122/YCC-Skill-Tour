// ============================================================
// AdvancedDemo.cs - C#高级特性演示
// 演示：异常处理、特性、反射、异步编程、线程、指针、
//       模式匹配、记录类型等
// ============================================================

using System.Reflection;  // 反射所需命名空间

namespace CSharpLearning.Advanced;

/// <summary>
/// C#高级特性演示类
/// </summary>
public class AdvancedDemo
{
    /// <summary>
    /// 运行所有高级特性演示
    /// </summary>
    public static void RunAllDemos()
    {
        Console.WriteLine("========== 高级特性演示 ==========");
        Console.WriteLine();

        DemoExceptionHandling();     // 异常处理
        Console.WriteLine();

        DemoAttributes();            // 特性
        Console.WriteLine();

        DemoReflection();            // 反射
        Console.WriteLine();

        DemoAsyncAwait();            // 异步编程
        Console.WriteLine();

        DemoThreadAndTask();         // 线程与任务
        Console.WriteLine();

        DemoUnsafeCode();            // 不安全代码与指针
        Console.WriteLine();

        DemoPatternMatching();       // 模式匹配
        Console.WriteLine();

        DemoRecordTypes();           // 记录类型
        Console.WriteLine();

        Console.WriteLine("========== 高级特性演示结束 ==========");
    }

    // ============================================================
    // 1. 异常处理
    // ============================================================
    
    /// <summary>
    /// 演示异常处理
    /// try/catch/finally/throw、自定义异常
    /// 异常处理是程序健壮性的重要保障
    /// </summary>
    public static void DemoExceptionHandling()
    {
        Console.WriteLine("--- 异常处理演示 ---");

        // ---- 基本 try/catch ----
        Console.WriteLine("1. 基本 try/catch：");
        try
        {
            // try 块：包含可能抛出异常的代码
            int a = 10;
            int b = 0;
            int result = a / b;  // 除以零，会抛出 DivideByZeroException
            Console.WriteLine($"结果：{result}");
        }
        catch (DivideByZeroException ex)
        {
            // catch 块：捕获特定类型的异常
            Console.WriteLine($"  捕获到除以零异常：{ex.Message}");
        }

        // ---- 多个 catch 块 ----
        Console.WriteLine("\n2. 多个 catch 块（按类型从具体到通用）：");
        try
        {
            string? input = null;
            int length = input!.Length;  // null引用异常
        }
        catch (NullReferenceException ex)
        {
            // 先捕获具体的异常类型
            Console.WriteLine($"  捕获到空引用异常：{ex.Message}");
        }
        catch (Exception ex)
        {
            // 最后捕获通用异常（Exception是所有异常的基类）
            Console.WriteLine($"  捕获到通用异常：{ex.Message}");
        }

        // ---- try/catch/finally ----
        Console.WriteLine("\n3. try/catch/finally：");
        FileStream? fs = null;
        try
        {
            // 尝试打开一个不存在的文件
            fs = File.OpenRead("不存在的文件.txt");
            Console.WriteLine("  文件打开成功");
        }
        catch (FileNotFoundException ex)
        {
            Console.WriteLine($"  文件未找到：{ex.Message}");
        }
        finally
        {
            // finally 块：无论是否发生异常都会执行
            // 通常用于释放资源（关闭文件、关闭数据库连接等）
            if (fs != null)
            {
                fs.Dispose();
                Console.WriteLine("  finally块：文件流已释放");
            }
            else
            {
                Console.WriteLine("  finally块：资源清理完成");
            }
        }

        // ---- using 语句（简化资源释放）----
        Console.WriteLine("\n4. using 语句（自动释放资源）：");
        // using 语句会在代码块结束时自动调用 Dispose 方法
        // 相当于 try/finally 的语法糖
        using (var reader = new StringReader("Hello World"))
        {
            string content = reader.ReadToEnd();
            Console.WriteLine($"  读取内容：{content}");
        }  // 这里自动调用 reader.Dispose()
        Console.WriteLine("  using块结束，资源已自动释放");

        // ---- throw 抛出异常 ----
        Console.WriteLine("\n5. throw 抛出异常：");
        try
        {
            Withdraw(100, 200);  // 取款超过余额
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine($"  操作失败：{ex.Message}");
        }

        // ---- 自定义异常 ----
        Console.WriteLine("\n6. 自定义异常：");
        try
        {
            RegisterUser("ab", "123");  // 用户名太短
        }
        catch (RegistrationException ex)
        {
            Console.WriteLine($"  注册失败：{ex.Message}（错误码：{ex.ErrorCode}）");
        }
    }

    /// <summary>
    /// 取款方法 - 演示主动抛出异常
    /// </summary>
    /// <param name="balance">余额</param>
    /// <param name="amount">取款金额</param>
    private static void Withdraw(decimal balance, decimal amount)
    {
        if (amount > balance)
        {
            // 使用 throw 抛出异常
            throw new InvalidOperationException("余额不足，无法取款");
        }
        // ... 取款逻辑
    }

    /// <summary>
    /// 用户注册方法 - 演示自定义异常
    /// </summary>
    private static void RegisterUser(string username, string password)
    {
        if (username.Length < 3)
        {
            throw new RegistrationException("用户名长度不能少于3个字符", 1001);
        }
        if (password.Length < 6)
        {
            throw new RegistrationException("密码长度不能少于6个字符", 1002);
        }
        // ... 注册逻辑
    }

    // ============================================================
    // 2. 特性 Attribute
    // ============================================================
    
    /// <summary>
    /// 演示特性（Attribute）
    /// 特性是一种给代码添加元数据的方式
    /// 可以附加到类、方法、属性等上面，通过反射读取
    /// </summary>
    public static void DemoAttributes()
    {
        Console.WriteLine("--- 特性演示 ---");

        // ---- 内置特性示例 ----
        Console.WriteLine("1. 内置特性：");

        // [Obsolete] 特性：标记方法已过时
        // 调用时编译器会给出警告或错误
        Console.WriteLine("  [Obsolete] - 标记过时的代码");
        Console.WriteLine("  [Conditional] - 条件编译");
        Console.WriteLine("  [Serializable] - 标记可序列化");
        Console.WriteLine("  [DataMember] - 数据成员标记");

        // ---- 自定义特性 ----
        Console.WriteLine("\n2. 自定义特性：");

        // 获取类上的特性（通过反射，详细在反射部分演示）
        Type type = typeof(ProductInfo);
        object[] attrs = type.GetCustomAttributes(typeof(DescriptionAttribute), false);
        if (attrs.Length > 0)
        {
            DescriptionAttribute desc = (DescriptionAttribute)attrs[0];
            Console.WriteLine($"  类描述：{desc.Description}");
        }

        // 获取属性上的特性
        PropertyInfo? prop = type.GetProperty("Name");
        if (prop != null)
        {
            var displayAttr = prop.GetCustomAttribute<DisplayAttribute>();
            if (displayAttr != null)
            {
                Console.WriteLine($"  Name属性显示名称：{displayAttr.DisplayName}");
                Console.WriteLine($"  Name属性顺序：{displayAttr.Order}");
            }
        }

        // 获取方法上的特性
        MethodInfo? method = type.GetMethod("Save");
        if (method != null)
        {
            var authAttr = method.GetCustomAttribute<AuthorizeAttribute>();
            if (authAttr != null)
            {
                Console.WriteLine($"  Save方法需要权限：{authAttr.RequiredRole}");
            }
        }
    }

    // ============================================================
    // 3. 反射
    // ============================================================
    
    /// <summary>
    /// 演示反射（Reflection）
    /// 反射允许在运行时获取类型信息、动态创建对象、调用方法等
    /// 主要类：Assembly、Type、MethodInfo、PropertyInfo、FieldInfo
    /// </summary>
    public static void DemoReflection()
    {
        Console.WriteLine("--- 反射演示 ---");

        // ---- 获取 Type 对象 ----
        Console.WriteLine("1. 获取 Type 对象的三种方式：");

        // 方式1：通过 typeof 运算符（编译时确定）
        Type t1 = typeof(string);
        Console.WriteLine($"  typeof(string): {t1.FullName}");

        // 方式2：通过对象的 GetType() 方法（运行时确定）
        string str = "hello";
        Type t2 = str.GetType();
        Console.WriteLine($"  \"hello\".GetType(): {t2.FullName}");

        // 方式3：通过 Type.GetType（按名称查找）
        Type? t3 = Type.GetType("System.Int32");
        Console.WriteLine($"  Type.GetType(\"System.Int32\"): {t3?.FullName}");

        // ---- 检查类型信息 ----
        Console.WriteLine("\n2. 类型信息：");
        Type productType = typeof(ProductInfo);
        Console.WriteLine($"  类名：{productType.Name}");
        Console.WriteLine($"  全名：{productType.FullName}");
        Console.WriteLine($"  命名空间：{productType.Namespace}");
        Console.WriteLine($"  是类吗？{productType.IsClass}");
        Console.WriteLine($"  是抽象的吗？{productType.IsAbstract}");
        Console.WriteLine($"  是密封的吗？{productType.IsSealed}");
        Console.WriteLine($"  基类：{productType.BaseType?.Name}");

        // ---- 获取属性信息 ----
        Console.WriteLine("\n3. 属性信息：");
        PropertyInfo[] properties = productType.GetProperties();
        foreach (PropertyInfo prop in properties)
        {
            Console.WriteLine($"  属性：{prop.Name}，类型：{prop.PropertyType.Name}，可写：{prop.CanWrite}");
        }

        // ---- 获取方法信息 ----
        Console.WriteLine("\n4. 方法信息（公共实例方法）：");
        MethodInfo[] methods = productType.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly);
        foreach (MethodInfo method in methods)
        {
            // 获取参数信息
            ParameterInfo[] parameters = method.GetParameters();
            string paramStr = string.Join(", ", parameters.Select(p => $"{p.ParameterType.Name} {p.Name}"));
            Console.WriteLine($"  方法：{method.ReturnType.Name} {method.Name}({paramStr})");
        }

        // ---- 动态创建对象与调用方法 ----
        Console.WriteLine("\n5. 动态创建对象与调用方法：");

        // 动态创建对象
        object? obj = Activator.CreateInstance(productType);
        if (obj != null)
        {
            // 动态设置属性值
            PropertyInfo nameProp = productType.GetProperty("Name")!;
            nameProp.SetValue(obj, "反射创建的产品");

            PropertyInfo priceProp = productType.GetProperty("Price")!;
            priceProp.SetValue(obj, 99.99m);

            // 动态获取属性值
            string name = (string)nameProp.GetValue(obj)!;
            decimal price = (decimal)priceProp.GetValue(obj)!;
            Console.WriteLine($"  动态设置后：Name={name}, Price={price:C}");

            // 动态调用方法
            MethodInfo saveMethod = productType.GetMethod("Save")!;
            saveMethod.Invoke(obj, null);  // 无参数方法
        }

        // ---- 加载程序集 ----
        Console.WriteLine("\n6. 程序集信息：");
        Assembly assembly = Assembly.GetExecutingAssembly();  // 当前程序集
        Console.WriteLine($"  程序集名称：{assembly.GetName().Name}");
        Console.WriteLine($"  程序集版本：{assembly.GetName().Version}");
        Console.WriteLine($"  程序集路径：{assembly.Location}");

        // 统计程序集中的类型数量
        Type[] allTypes = assembly.GetTypes();
        Console.WriteLine($"  程序集中类型数量：{allTypes.Length}");
    }

    // ============================================================
    // 4. 异步编程 async/await
    // ============================================================
    
    /// <summary>
    /// 演示异步编程 async/await
    /// async/await 是 C# 5 引入的异步编程模型
    /// 可以像写同步代码一样写异步代码，不会阻塞线程
    /// </summary>
    public static void DemoAsyncAwait()
    {
        Console.WriteLine("--- 异步编程 async/await 演示 ---");

        Console.WriteLine("1. async/await 基本用法：");

        // 调用异步方法（使用 .Wait() 等待，仅演示用，实际项目中用 await）
        Task<string> task = DownloadDataAsync("https://example.com");
        Console.WriteLine("  异步方法已启动，正在等待结果...");

        // 注意：在同步方法中等待异步方法，使用 .Result 或 .Wait()
        // 实际项目中应该在 async 方法中使用 await
        string result = task.GetAwaiter().GetResult();
        Console.WriteLine($"  下载完成，数据长度：{result.Length}");

        // ---- 多个异步任务并行执行 ----
        Console.WriteLine("\n2. 多个任务并行执行：");

        var sw = System.Diagnostics.Stopwatch.StartNew();

        // 启动多个异步任务（并行执行）
        Task<int> task1 = CalculateAsync(1000, "任务1");
        Task<int> task2 = CalculateAsync(1500, "任务2");
        Task<int> task3 = CalculateAsync(500, "任务3");

        Console.WriteLine("  三个任务已启动，等待全部完成...");

        // 等待所有任务完成
        Task.WhenAll(task1, task2, task3).GetAwaiter().GetResult();

        sw.Stop();
        Console.WriteLine($"  全部完成，总耗时：{sw.ElapsedMilliseconds}ms");
        Console.WriteLine($"  任务1结果：{task1.Result}");
        Console.WriteLine($"  任务2结果：{task2.Result}");
        Console.WriteLine($"  任务3结果：{task3.Result}");
        Console.WriteLine("  （如果串行执行需要约3000ms，并行只需要约1500ms）");

        // ---- Task 与 ValueTask ----
        Console.WriteLine("\n3. Task 与 ValueTask：");
        Console.WriteLine("  Task: 引用类型，适合长时间异步操作");
        Console.WriteLine("  ValueTask: 值类型，适合快速完成的异步操作，减少GC压力");
        Console.WriteLine("  大多数场景下使用 Task 即可");
    }

    /// <summary>
    /// 异步方法：模拟下载数据
    /// async 关键字标记这是一个异步方法
    /// 返回 Task<T> 表示异步操作的结果
    /// 方法名约定以 Async 结尾
    /// </summary>
    /// <param name="url">下载地址</param>
    /// <returns>下载的内容</returns>
    private static async Task<string> DownloadDataAsync(string url)
    {
        Console.WriteLine($"  开始下载：{url}");

        // await 等待异步操作完成
        // 等待期间不会阻塞线程（线程可以去做其他事情）
        await Task.Delay(1000);  // 模拟网络延迟1秒

        Console.WriteLine($"  下载完成：{url}");
        return $"这是从 {url} 下载的数据内容（模拟）";
    }

    /// <summary>
    /// 异步计算方法
    /// </summary>
    private static async Task<int> CalculateAsync(int delayMs, string taskName)
    {
        Console.WriteLine($"  {taskName} 开始计算...");
        await Task.Delay(delayMs);  // 模拟计算耗时
        Console.WriteLine($"  {taskName} 计算完成");
        return delayMs;
    }

    // ============================================================
    // 5. 线程与任务
    // ============================================================
    
    /// <summary>
    /// 演示线程与任务
    /// Thread: 操作系统级别的线程，重量级
    /// ThreadPool: 线程池，管理一组可重用的线程
    /// Task: 基于线程池的任务抽象，轻量级，推荐使用
    /// </summary>
    public static void DemoThreadAndTask()
    {
        Console.WriteLine("--- 线程与任务演示 ---");

        // ---- Thread ----
        Console.WriteLine("1. Thread（线程）：");

        // 创建一个新线程
        Thread thread = new Thread(() =>
        {
            Console.WriteLine("  新线程开始执行");
            Thread.Sleep(500);  // 模拟工作
            Console.WriteLine("  新线程执行完毕");
        });

        Console.WriteLine("  启动线程...");
        thread.Start();
        thread.Join();  // 等待线程完成
        Console.WriteLine("  主线程继续执行");

        // ---- ThreadPool 线程池 ----
        Console.WriteLine("\n2. ThreadPool（线程池）：");

        // 将工作项排入线程池
        bool queued = ThreadPool.QueueUserWorkItem(state =>
        {
            Console.WriteLine("  线程池工作项开始执行");
            Thread.Sleep(300);
            Console.WriteLine("  线程池工作项执行完毕");
        });
        Console.WriteLine($"  工作项已排队：{queued}");

        // 等待线程池工作完成
        Thread.Sleep(500);

        // ---- Task（任务）----
        Console.WriteLine("\n3. Task（任务）- 推荐使用：");

        // Task.Run 将任务放到线程池执行
        Task<int> task = Task.Run(() =>
        {
            Console.WriteLine("  Task 开始执行");
            int sum = 0;
            for (int i = 1; i <= 100; i++)
            {
                sum += i;
            }
            Thread.Sleep(200);
            Console.WriteLine("  Task 执行完毕");
            return sum;
        });

        Console.WriteLine($"  Task已启动，等待结果...");
        Console.WriteLine($"  Task结果（1到100的和）：{task.Result}");

        // ---- 任务延续（ContinueWith）----
        Console.WriteLine("\n4. 任务延续 ContinueWith：");

        Task<string> continueTask = Task.Run(() =>
        {
            Thread.Sleep(100);
            return "Hello";
        }).ContinueWith(prevTask =>
        {
            // 前一个任务完成后执行
            return prevTask.Result + " World";
        });

        Console.WriteLine($"  延续任务结果：{continueTask.Result}");

        Console.WriteLine("\n5. 总结：");
        Console.WriteLine("  - Thread: 重量级，适合长时间运行的任务，自己管理生命周期");
        Console.WriteLine("  - ThreadPool: 轻量级，线程复用，适合短任务");
        Console.WriteLine("  - Task: 基于ThreadPool，功能强大，推荐使用");
        Console.WriteLine("  - async/await: Task的语法糖，让异步代码更易读");
    }

    // ============================================================
    // 6. 指针与不安全代码
    // ============================================================
    
    /// <summary>
    /// 演示指针与不安全代码
    /// C# 通常是"安全"的代码（托管代码），不直接操作内存
    /// 使用 unsafe 关键字可以编写不安全代码，直接操作指针
    /// 需要在项目文件中设置 AllowUnsafeBlocks = true
    /// </summary>
    public static unsafe void DemoUnsafeCode()
    {
        Console.WriteLine("--- 不安全代码与指针演示 ---");

        // ---- 基本指针操作 ----
        Console.WriteLine("1. 基本指针操作：");

        int number = 100;
        int* p = &number;  // & 取地址，p 是指向 int 的指针

        Console.WriteLine($"  变量 number 的值：{number}");
        Console.WriteLine($"  变量 number 的地址：{(long)p:X}（十六进制）");
        Console.WriteLine($"  通过指针读取值：{*p}");  // * 解引用，获取指针指向的值

        // 通过指针修改值
        *p = 200;
        Console.WriteLine($"  通过指针修改后，number 的值：{number}");

        // ---- 指针与数组 ----
        Console.WriteLine("\n2. 指针与数组：");

        int[] arr = { 10, 20, 30, 40, 50 };
        Console.WriteLine($"  原数组：{string.Join(", ", arr)}");

        // 使用 fixed 固定数组在内存中的位置（防止GC移动）
        fixed (int* pArr = arr)
        {
            // 指针遍历数组
            for (int i = 0; i < arr.Length; i++)
            {
                // 指针算术：pArr + i 指向第i个元素
                Console.WriteLine($"  arr[{i}] = {*(pArr + i)}（地址：{(long)(pArr + i):X}）");

                // 通过指针修改数组元素
                *(pArr + i) *= 2;
            }
        }
        Console.WriteLine($"  每个元素乘2后：{string.Join(", ", arr)}");

        // ---- 值类型与引用类型的内存 ----
        Console.WriteLine("\n3. 值类型 vs 引用类型（内存角度）：");
        Console.WriteLine("  值类型存储在栈上，直接包含数据");
        Console.WriteLine("  引用类型存储在堆上，变量存储的是引用（地址）");
        Console.WriteLine("  指针直接操作内存地址，可以读写任何类型的数据");

        Console.WriteLine("\n注意：不安全代码应谨慎使用！");
        Console.WriteLine("  - 绕过了类型安全检查，可能导致程序崩溃");
        Console.WriteLine("  - 主要用于性能优化、与非托管代码互操作");
        Console.WriteLine("  - 普通业务开发几乎不需要使用");
    }

    // ============================================================
    // 7. 模式匹配
    // ============================================================
    
    /// <summary>
    /// 演示模式匹配（Pattern Matching）
    /// 模式匹配是 C# 7+ 引入的强大功能
    /// 包括：is 模式、switch 表达式、属性模式、位置模式等
    /// </summary>
    public static void DemoPatternMatching()
    {
        Console.WriteLine("--- 模式匹配演示 ---");

        // ---- is 模式（类型模式）----
        Console.WriteLine("1. is 模式（类型判断+变量声明）：");

        object obj = "Hello World";

        // 传统写法：先判断类型，再转换
        if (obj is string)
        {
            string s = (string)obj;
            Console.WriteLine($"  传统方式：是字符串，长度={s.Length}");
        }

        // 模式匹配写法：判断并声明变量（一步完成）
        if (obj is string str)
        {
            // 如果 obj 是 string，str 就是转换后的结果
            Console.WriteLine($"  模式匹配：是字符串，长度={str.Length}");
        }

        // 不仅可以判断类型，还可以判断值
        if (obj is string str2 && str2.Length > 5)
        {
            Console.WriteLine($"  带条件的模式匹配：字符串长度>5，内容=\"{str2}\"");
        }

        // ---- 常量模式 ----
        Console.WriteLine("\n2. 常量模式（判断是否等于某个常量）：");
        int? x = 5;
        if (x is 5)
        {
            Console.WriteLine("  x 等于 5");
        }
        if (x is not null)
        {
            Console.WriteLine("  x 不为 null");
        }

        // ---- switch 表达式 + 各种模式 ----
        Console.WriteLine("\n3. switch 表达式（C# 8+）：");

        // 类型模式
        object[] items = { 123, "hello", 3.14, true, null };
        foreach (object? item in items)
        {
            string typeName = item switch
            {
                int => "整数",
                string => "字符串",
                double => "浮点数",
                bool => "布尔值",
                null => "null",
                _ => "其他类型"  // _ 是弃元，相当于 default
            };
            Console.WriteLine($"  {item ?? "null"} -> {typeName}");
        }

        // ---- 属性模式 ----
        Console.WriteLine("\n4. 属性模式（根据属性值匹配）：");

        var people = new[]
        {
            new { Name = "张三", Age = 25, City = "北京" },
            new { Name = "李四", Age = 17, City = "上海" },
            new { Name = "王五", Age = 30, City = "北京" },
            new { Name = "赵六", Age = 15, City = "北京" }
        };

        foreach (var p in people)
        {
            // 属性模式：同时匹配多个属性
            string description = p switch
            {
                { Age: >= 18, City: "北京" } => "北京成年人",
                { Age: < 18, City: "北京" } => "北京未成年人",
                { City: "上海" } => "上海人",
                _ => "其他"
            };
            Console.WriteLine($"  {p.Name}（{p.Age}岁，{p.City}）-> {description}");
        }

        // ---- 关系模式（C# 9+）----
        Console.WriteLine("\n5. 关系模式：");

        string Grade(int score) => score switch
        {
            < 0 or > 100 => "无效",   // 分数不在0-100范围内为无效
            >= 90 => "优秀",
            >= 80 => "良好",
            >= 60 => "及格",
            _ => "不及格"             // 剩下的就是 0-59 分
        };

        Console.WriteLine($"  分数95 -> {Grade(95)}");
        Console.WriteLine($"  分数85 -> {Grade(85)}");
        Console.WriteLine($"  分数70 -> {Grade(70)}");
        Console.WriteLine($"  分数50 -> {Grade(50)}");

        // ---- 逻辑模式（C# 9+）----
        Console.WriteLine("\n6. 逻辑模式（and/or/not）：");

        bool IsLetter(char c) => c is >= 'a' and <= 'z' or >= 'A' and <= 'Z';

        Console.WriteLine($"  'a' 是字母吗？{IsLetter('a')}");
        Console.WriteLine($"  '5' 是字母吗？{IsLetter('5')}");
        Console.WriteLine($"  'Z' 是字母吗？{IsLetter('Z')}");
    }

    // ============================================================
    // 8. 记录类型 record
    // ============================================================
    
    /// <summary>
    /// 演示记录类型 record（C# 9+）
    /// record 是引用类型，但具有值类型的相等语义
    /// 主要用于不可变的数据模型（DTO、数据传输对象）
    /// 特点：不可变性、值相等、with表达式、解构
    /// </summary>
    public static void DemoRecordTypes()
    {
        Console.WriteLine("--- 记录类型 record 演示 ---");

        // ---- 创建 record 对象 ----
        Console.WriteLine("1. 创建 record 对象：");

        // 使用位置参数创建 record（主构造函数）
        PersonRecord p1 = new PersonRecord("张三", 25, "北京");
        Console.WriteLine($"  p1: {p1}");

        // 使用对象初始化器
        PersonRecord p2 = new PersonRecord("李四", 30, "上海") { Age = 31 };
        Console.WriteLine($"  p2: {p2}");

        // ---- 值相等语义 ----
        Console.WriteLine("\n2. 值相等语义：");

        // 创建两个内容相同的 record
        PersonRecord p3 = new PersonRecord("张三", 25, "北京");
        PersonRecord p4 = new PersonRecord("张三", 25, "北京");

        Console.WriteLine($"  p1 和 p3 内容相同");
        Console.WriteLine($"  引用相等（ReferenceEquals）：{object.ReferenceEquals(p1, p3)}");
        Console.WriteLine($"  值相等（==）：{p1 == p3}");
        Console.WriteLine($"  Equals方法：{p1.Equals(p3)}");
        Console.WriteLine("  （普通类的 == 比较引用，record 的 == 比较值）");

        // ---- with 表达式（非破坏性修改）----
        Console.WriteLine("\n3. with 表达式（非破坏性修改）：");

        // 创建一个 p1 的副本，只修改 Age 属性
        PersonRecord olderP1 = p1 with { Age = p1.Age + 10 };
        Console.WriteLine($"  原始：{p1}");
        Console.WriteLine($"  with修改年龄后：{olderP1}");
        Console.WriteLine($"  是同一个对象吗？{object.ReferenceEquals(p1, olderP1)}");

        // with 可以修改多个属性
        PersonRecord movedP1 = p1 with { Age = 26, City = "深圳" };
        Console.WriteLine($"  修改年龄和城市后：{movedP1}");

        // ---- 解构（Deconstruct）----
        Console.WriteLine("\n4. 解构：");

        // record 自动生成解构方法
        var (name, age, city) = p1;
        Console.WriteLine($"  解构后：name={name}, age={age}, city={city}");

        // 也可以只解构部分（用弃元 _）
        var (name2, _, _) = p1;
        Console.WriteLine($"  只解构姓名：name2={name2}");

        // ---- record 类 vs record 结构体 ----
        Console.WriteLine("\n5. record class vs record struct（C# 10+）：");
        Console.WriteLine("  record / record class: 引用类型的记录");
        Console.WriteLine("  record struct: 值类型的记录（结构体）");

        // record struct 示例
        PointRecord point = new PointRecord(10, 20);
        Console.WriteLine($"  record struct: {point}");

        // ---- record 的继承 ----
        Console.WriteLine("\n6. record 的继承：");

        StudentRecord student = new StudentRecord("小明", 20, "北京", "清华大学", "2024001");
        Console.WriteLine($"  学生record: {student}");

        // 基类 record 变量指向派生 record
        PersonRecord personAsStudent = student;
        Console.WriteLine($"  基类引用: {personAsStudent}");

        Console.WriteLine("\nrecord 的适用场景：");
        Console.WriteLine("  - 数据传输对象（DTO）");
        Console.WriteLine("  - 不可变的数据模型");
        Console.WriteLine("  - API 响应/请求模型");
        Console.WriteLine("  - 配置选项类");
    }
}

// ============================================================
// 异常处理相关类
// ============================================================

/// <summary>
/// 自定义异常类：注册异常
/// 自定义异常通常继承自 Exception（或更具体的异常类）
/// 命名约定：以 Exception 结尾
/// </summary>
public class RegistrationException : Exception
{
    /// <summary>
    /// 错误码
    /// </summary>
    public int ErrorCode { get; }

    /// <summary>
    /// 构造函数
    /// </summary>
    public RegistrationException(string message, int errorCode)
        : base(message)  // 调用基类构造函数
    {
        ErrorCode = errorCode;
    }
}

// ============================================================
// 特性相关类
// ============================================================

/// <summary>
/// 自定义特性：描述特性
/// 自定义特性继承自 Attribute 基类
/// 命名约定：以 Attribute 结尾
/// </summary>
[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
public class DescriptionAttribute : Attribute
{
    /// <summary>
    /// 描述文本
    /// </summary>
    public string Description { get; }

    public DescriptionAttribute(string description)
    {
        Description = description;
    }
}

/// <summary>
/// 自定义特性：显示名称特性
/// </summary>
[AttributeUsage(AttributeTargets.Property)]
public class DisplayAttribute : Attribute
{
    /// <summary>
    /// 显示名称
    /// </summary>
    public string DisplayName { get; set; } = string.Empty;

    /// <summary>
    /// 显示顺序
    /// </summary>
    public int Order { get; set; }
}

/// <summary>
/// 自定义特性：授权特性
/// </summary>
[AttributeUsage(AttributeTargets.Method)]
public class AuthorizeAttribute : Attribute
{
    /// <summary>
    /// 所需角色
    /// </summary>
    public string RequiredRole { get; set; } = string.Empty;
}

/// <summary>
/// 产品信息类 - 用于特性和反射演示
/// 应用了多个自定义特性
/// </summary>
[Description("产品信息类，用于存储产品数据")]
public class ProductInfo
{
    /// <summary>
    /// 产品名称
    /// </summary>
    [Display(DisplayName = "产品名称", Order = 1)]
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 产品价格
    /// </summary>
    [Display(DisplayName = "价格", Order = 2)]
    public decimal Price { get; set; }

    /// <summary>
    /// 库存数量
    /// </summary>
    [Display(DisplayName = "库存", Order = 3)]
    public int Stock { get; set; }

    /// <summary>
    /// 保存产品
    /// 需要管理员权限
    /// </summary>
    [Authorize(RequiredRole = "Admin")]
    public void Save()
    {
        Console.WriteLine($"  产品\"{Name}\"已保存（价格：{Price:C}，库存：{Stock}）");
    }
}

// ============================================================
// 记录类型（Record）相关类
// ============================================================

/// <summary>
/// 人员记录类型
/// record 关键字定义记录类型
/// 主构造函数参数会自动生成属性和初始化
/// </summary>
/// <param name="Name">姓名</param>
/// <param name="Age">年龄</param>
/// <param name="City">城市</param>
public record PersonRecord(string Name, int Age, string City)
{
    // 可以在 record 中添加额外的属性或方法
    // 计算属性
    public string Info => $"{Name}（{Age}岁，{City}）";

    // 重写 ToString（record 已经有默认的 ToString，但可以自定义）
    // public override string ToString() => Info;  // 如果自定义就不会自动生成
}

/// <summary>
/// 学生记录类型 - 继承自人员记录
/// record 可以继承 record
/// </summary>
public record StudentRecord(string Name, int Age, string City, string School, string StudentId)
    : PersonRecord(Name, Age, City)  // 调用基类 record 的构造函数
{
    // 可以添加额外的属性
    public string Grade => Age < 18 ? "未成年" : "成年";
}

/// <summary>
/// 点记录结构体
/// record struct 是值类型的记录
/// </summary>
public readonly record struct PointRecord(int X, int Y);
