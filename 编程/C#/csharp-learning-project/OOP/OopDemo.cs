// ============================================================
// OopDemo.cs - 面向对象编程演示入口
// 演示：类与对象、封装、继承、多态、抽象类、接口、
//       属性、索引器、事件、委托、静态类、扩展方法、
//       结构体、分部类、内部类
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 面向对象编程演示类
/// 这是 OOP 模块的入口，依次演示各个 OOP 概念
/// </summary>
public class OopDemo
{
    /// <summary>
    /// 运行所有 OOP 演示
    /// </summary>
    public static void RunAllDemos()
    {
        Console.WriteLine("========== 面向对象编程演示 ==========");
        Console.WriteLine();

        DemoClassesAndObjects();    // 演示类与对象
        Console.WriteLine();

        DemoEncapsulation();        // 演示封装
        Console.WriteLine();

        DemoInheritance();          // 演示继承
        Console.WriteLine();

        DemoPolymorphism();         // 演示多态
        Console.WriteLine();

        DemoAbstractClass();        // 演示抽象类
        Console.WriteLine();

        DemoInterface();            // 演示接口
        Console.WriteLine();

        DemoProperties();           // 演示属性
        Console.WriteLine();

        DemoIndexer();              // 演示索引器
        Console.WriteLine();

        DemoDelegates();            // 演示委托
        Console.WriteLine();

        DemoEvents();               // 演示事件
        Console.WriteLine();

        DemoStaticClass();          // 演示静态类
        Console.WriteLine();

        DemoExtensionMethods();     // 演示扩展方法
        Console.WriteLine();

        DemoStructs();              // 演示结构体
        Console.WriteLine();

        DemoPartialClass();         // 演示分部类
        Console.WriteLine();

        DemoNestedClass();          // 演示内部类
        Console.WriteLine();

        Console.WriteLine("========== 面向对象编程演示结束 ==========");
    }

    // ============================================================
    // 1. 类与对象
    // ============================================================
    
    /// <summary>
    /// 演示类与对象
    /// 类是模板，对象是类的实例
    /// 使用 new 关键字创建对象
    /// </summary>
    public static void DemoClassesAndObjects()
    {
        Console.WriteLine("--- 类与对象演示 ---");

        // 使用 new 关键字创建 Person 对象
        // 调用无参构造函数
        Person person1 = new Person();
        person1.Name = "张三";
        person1.Age = 25;

        // 调用方法
        person1.Introduce();
        person1.Eat();
        person1.Sleep();

        // 使用带参数的构造函数创建对象
        Person person2 = new Person("李四", 30);
        person2.Introduce();

        Console.WriteLine($"当前Person对象数量：{Person.PersonCount}");
    }

    // ============================================================
    // 2. 封装
    // ============================================================
    
    /// <summary>
    /// 演示封装
    /// 封装：隐藏内部实现细节，只暴露必要的接口
    /// 通过访问修饰符（public/private/protected/internal）实现
    /// </summary>
    public static void DemoEncapsulation()
    {
        Console.WriteLine("--- 封装演示 ---");

        Person person = new Person("王五", 28);

        // 通过属性访问私有字段（外部不能直接访问 _name 和 _age）
        Console.WriteLine($"通过属性访问：Name={person.Name}, Age={person.Age}");

        // 属性的 set 中包含验证逻辑
        person.Age = -5;  // 不合法，会被拒绝
        Console.WriteLine($"尝试设置年龄为-5后：Age={person.Age}（未变化）");

        person.Age = 35;  // 合法，成功修改
        Console.WriteLine($"设置年龄为35后：Age={person.Age}");

        // 公共方法对外暴露功能
        person.Eat();
        person.Sleep();

        // 无法访问私有字段（编译错误）
        // person._name = "测试";  // 错误：不可访问，因为受保护级别限制

        Console.WriteLine("封装的好处：数据安全、代码解耦、易于维护");
    }

    // ============================================================
    // 3. 继承
    // ============================================================
    
    /// <summary>
    /// 演示继承
    /// 继承：子类（派生类）继承父类（基类）的成员，实现代码复用
    /// 使用冒号 : 表示继承关系
    /// C# 中类只能单继承，但可以实现多个接口
    /// </summary>
    public static void DemoInheritance()
    {
        Console.WriteLine("--- 继承演示 ---");

        // 创建学生对象（Student 继承自 Person）
        Student student = new Student("小明", 18, "清华大学", "2024001");

        // 可以使用基类的属性和方法
        student.Name = "小明同学";     // 继承自 Person
        student.Age = 19;              // 继承自 Person
        student.Eat();                 // 继承自 Person 的方法
        student.Sleep();               // 继承自 Person 的方法

        // 也可以使用自己的属性和方法
        student.Study();               // Student 自己的方法
        Console.WriteLine($"学校：{student.School}，学号：{student.StudentId}");

        // 演示 sealed 类（密封类不能被继承）
        GraduateStudent grad = new GraduateStudent("小红", 25, "北大", "G2024001", "李教授");
        grad.DoResearch();
        Console.WriteLine($"研究生信息：{grad.ToString()}");
    }

    // ============================================================
    // 4. 多态
    // ============================================================
    
    /// <summary>
    /// 演示多态
    /// 多态：同一个方法调用，不同的对象有不同的行为
    /// 通过 virtual（基类）和 override（派生类）实现
    /// 多态的前提：继承 + 方法重写 + 父类引用指向子类对象
    /// </summary>
    public static void DemoPolymorphism()
    {
        Console.WriteLine("--- 多态演示 ---");

        // 父类引用指向子类对象（这是多态的关键）
        Person person = new Student("小刚", 20, "复旦大学", "2024002");

        // 调用 Introduce 方法
        // 虽然变量类型是 Person，但实际运行时会调用 Student 的 Introduce
        // 这就是多态：同一方法名，不同对象有不同实现
        Console.WriteLine("父类引用指向子类对象，调用 Introduce：");
        person.Introduce();

        // 再用基类引用指向基类对象对比
        Person normalPerson = new Person("普通人", 30);
        Console.WriteLine("\n基类对象调用 Introduce：");
        normalPerson.Introduce();

        // 多态数组：数组元素类型是基类，可以存储各种子类对象
        Console.WriteLine("\n多态数组演示：");
        Person[] people = new Person[]
        {
            new Person("张三", 25),
            new Student("李四", 20, "交大", "2024003"),
            new GraduateStudent("王五", 28, "浙大", "G2024001", "王教授")
        };

        // 遍历数组，每个对象调用自己的 Introduce
        foreach (Person p in people)
        {
            p.Introduce();
            Console.WriteLine("---");
        }
    }

    // ============================================================
    // 5. 抽象类
    // ============================================================
    
    /// <summary>
    /// 演示抽象类
    /// 抽象类不能被实例化，只能被继承
    /// 抽象类可以包含抽象成员（只有声明，没有实现）
    /// 派生类必须实现所有抽象成员
    /// </summary>
    public static void DemoAbstractClass()
    {
        Console.WriteLine("--- 抽象类演示 ---");

        // 不能直接实例化抽象类（编译错误）
        // Shape shape = new Shape();  // 错误：无法创建抽象类的实例

        // 可以创建派生类的对象，用基类引用指向它
        Shape circle = new Circle(5);
        circle.Color = "红色";
        circle.Draw();
        circle.ShowInfo();

        Shape rectangle = new Rectangle(4, 6);
        rectangle.Color = "蓝色";
        rectangle.Draw();
        rectangle.ShowInfo();

        Shape triangle = new Triangle(3, 4);
        triangle.Color = "绿色";
        triangle.Draw();
        triangle.ShowInfo();

        // 多态：用 Shape 数组存储各种形状
        Console.WriteLine("\n形状数组（多态）：");
        Shape[] shapes = new Shape[]
        {
            new Circle(3),
            new Rectangle(2, 5),
            new Triangle(4, 3)
        };

        double totalArea = 0;
        foreach (Shape shape in shapes)
        {
            shape.Draw();
            totalArea += shape.Area;
        }
        Console.WriteLine($"所有形状的总面积：{totalArea:F2}");
    }

    // ============================================================
    // 6. 接口
    // ============================================================
    
    /// <summary>
    /// 演示接口
    /// 接口定义行为契约，类可以实现一个或多个接口
    /// 接口是实现"多继承"的方式
    /// </summary>
    public static void DemoInterface()
    {
        Console.WriteLine("--- 接口演示 ---");

        // 接口引用指向实现类的对象
        IAnimal dog = new Dog { Name = "旺财" };
        dog.MakeSound();
        dog.Move();

        IAnimal bird = new Bird { Name = "小鸟" };
        bird.MakeSound();
        bird.Move();

        // 鸟实现了两个接口，可以分别使用
        IFlyable flyingBird = new Bird { Name = "老鹰" };
        Console.WriteLine($"\n老鹰的最大飞行高度：{flyingBird.MaxHeight}米");
        flyingBird.Fly();

        // 显式接口实现演示
        Console.WriteLine("\n显式接口实现演示：");
        Robot robot = new Robot();
        robot.Work();  // 调用类自己的 Work 方法

        // 通过接口引用调用显式接口实现
        IRobot iRobot = robot;
        iRobot.Work();  // 调用 IRobot.Work

        IWorker iWorker = robot;
        iWorker.Work();  // 调用 IWorker.Work

        // 多态：用接口数组存储不同实现
        Console.WriteLine("\n接口数组（多态）：");
        IAnimal[] animals = new IAnimal[]
        {
            new Dog { Name = "大黄" },
            new Bird { Name = "麻雀" }
        };
        foreach (IAnimal animal in animals)
        {
            Console.Write($"{animal.Name}：");
            animal.MakeSound();
        }
    }

    // ============================================================
    // 7. 属性
    // ============================================================
    
    /// <summary>
    /// 演示各种属性类型
    /// </summary>
    public static void DemoProperties()
    {
        Console.WriteLine("--- 属性演示 ---");

        Person person = new Person("测试", 20);

        // 完整属性
        person.Name = "属性测试";
        Console.WriteLine($"完整属性 Name：{person.Name}");

        // 自动属性（Gender 是自动属性，且set为private）
        Console.WriteLine($"自动属性 Gender：{person.Gender}");
        // 外部不能设置 Gender（因为set是private的）
        // person.Gender = "男";  // 编译错误

        // 计算属性
        Console.WriteLine($"计算属性 Info：{person.Info}");

        // init-only 属性（只能在初始化时设置）
        Person person2 = new Person("新人类", 25)
        {
            BirthDate = new DateTime(2000, 1, 1)  // 初始化时可以设置
        };
        Console.WriteLine($"init-only属性 BirthDate：{person2.BirthDate:yyyy-MM-dd}");
        // 之后不能修改
        // person2.BirthDate = DateTime.Now;  // 编译错误
    }

    // ============================================================
    // 8. 索引器
    // ============================================================
    
    /// <summary>
    /// 演示索引器
    /// 索引器允许像数组一样用 [] 访问对象的元素
    /// </summary>
    public static void DemoIndexer()
    {
        Console.WriteLine("--- 索引器演示 ---");

        Person person = new Person("联系人", 30);

        // 使用索引器设置联系人（像数组一样）
        person[0] = "张三";
        person[1] = "李四";
        person[2] = "王五";

        // 使用索引器读取联系人
        Console.WriteLine($"第1个联系人：{person[0]}");
        Console.WriteLine($"第2个联系人：{person[1]}");
        Console.WriteLine($"第3个联系人：{person[2]}");

        // 越界访问（会有边界检查）
        Console.WriteLine($"第10个联系人：{person[10]}（越界返回空）");
    }

    // ============================================================
    // 9. 委托
    // ============================================================
    
    /// <summary>
    /// 演示委托
    /// 委托是一种存储方法引用的类型
    /// 可以把委托理解为"方法的变量"
    /// </summary>
    public static void DemoDelegates()
    {
        Console.WriteLine("--- 委托演示 ---");

        // 声明委托变量，并指向一个方法
        MathOperation op = new MathOperation(Calculator.Add);
        Console.WriteLine($"委托调用Add：{op(10, 5)}");

        // 可以让委托指向另一个方法
        op = Calculator.Subtract;
        Console.WriteLine($"委托调用Subtract：{op(10, 5)}");

        // 使用委托作为参数（策略模式）
        Console.WriteLine("\n委托作为参数（策略模式）：");
        int result1 = Calculator.ExecuteOperation(10, 3, Calculator.Add);
        int result2 = Calculator.ExecuteOperation(10, 3, Calculator.Subtract);
        int result3 = Calculator.ExecuteOperation(10, 3, Calculator.Multiply);
        Console.WriteLine($"  10 + 3 = {result1}");
        Console.WriteLine($"  10 - 3 = {result2}");
        Console.WriteLine($"  10 * 3 = {result3}");

        // 多播委托：一个委托可以指向多个方法
        Console.WriteLine("\n多播委托：");
        MathOperation multiOp = Calculator.Add;
        multiOp += Calculator.Subtract;  // 用 += 添加方法
        multiOp += Calculator.Multiply;  // 再添加一个

        // 调用多播委托会依次调用所有方法
        // 返回值是最后一个方法的返回值
        int multiResult = multiOp(8, 2);
        Console.WriteLine($"  多播委托调用结果（最后一个方法的返回值）：{multiResult}");
        // 注意：多播委托会依次执行Add、Subtract、Multiply，返回值是最后一个 Multiply 的结果
    }

    // ============================================================
    // 10. 事件
    // ============================================================
    
    /// <summary>
    /// 演示事件
    /// 事件是委托的一种特殊用法，用于实现观察者模式
    /// 发布者发布事件，订阅者订阅事件
    /// </summary>
    public static void DemoEvents()
    {
        Console.WriteLine("--- 事件演示 ---");

        // 创建产品（事件发布者）
        Product phone = new Product { Name = "智能手机", Price = 5999m };

        // 创建通知器（事件订阅者）
        EmailNotifier emailNotifier = new EmailNotifier();
        SmsNotifier smsNotifier = new SmsNotifier();

        // 订阅事件
        emailNotifier.Subscribe(phone);
        smsNotifier.Subscribe(phone);

        // 修改价格，触发事件（所有订阅者都会收到通知）
        Console.WriteLine("第一次价格变化：");
        phone.Price = 5499m;

        Console.WriteLine("\n第二次价格变化：");
        phone.Price = 4999m;

        // 取消邮件订阅
        emailNotifier.Unsubscribe(phone);

        Console.WriteLine("\n取消邮件订阅后，第三次价格变化：");
        phone.Price = 4599m;  // 只有短信通知会收到
    }

    // ============================================================
    // 11. 静态类
    // ============================================================
    
    /// <summary>
    /// 演示静态类和静态成员
    /// 静态类不能实例化，直接通过类名访问成员
    /// </summary>
    public static void DemoStaticClass()
    {
        Console.WriteLine("--- 静态类演示 ---");

        // 直接通过类名调用静态方法（不需要 new）
        Console.WriteLine($"MathUtility.Max(10, 20) = {MathUtility.Max(10, 20)}");
        Console.WriteLine($"MathUtility.Min(10, 20) = {MathUtility.Min(10, 20)}");
        Console.WriteLine($"MathUtility.IsPrime(17) = {MathUtility.IsPrime(17)}");
        Console.WriteLine($"MathUtility.IsPrime(15) = {MathUtility.IsPrime(15)}");
        Console.WriteLine($"MathUtility.Factorial(5) = {MathUtility.Factorial(5)}");

        // 访问静态字段
        Console.WriteLine($"静态字段 RandomSeed = {MathUtility.RandomSeed}");
    }

    // ============================================================
    // 12. 扩展方法
    // ============================================================
    
    /// <summary>
    /// 演示扩展方法
    /// 扩展方法可以在不修改原有类的情况下，为类型添加新方法
    /// 调用方式和实例方法一样
    /// </summary>
    public static void DemoExtensionMethods()
    {
        Console.WriteLine("--- 扩展方法演示 ---");

        // string 的扩展方法
        string email1 = "test@example.com";
        string email2 = "invalid-email";
        Console.WriteLine($"\"{email1}\" 是有效邮箱吗？{email1.IsValidEmail()}");
        Console.WriteLine($"\"{email2}\" 是有效邮箱吗？{email2.IsValidEmail()}");

        string text = "Hello World";
        Console.WriteLine($"\"{text}\" 反转后：\"{text.Reverse()}\"");

        string str = "Abc";
        Console.WriteLine($"\"{str}\" 重复3次：\"{str.Repeat(3)}\"");

        string longStr = "这是一段很长很长的文字";
        Console.WriteLine($"截断到5个字符：\"{longStr.Truncate(5)}\"");

        // int 的扩展方法
        int num1 = 10;
        int num2 = 7;
        Console.WriteLine($"{num1} 是偶数吗？{num1.IsEven()}");
        Console.WriteLine($"{num2} 是奇数吗？{num2.IsOdd()}");
        Console.WriteLine($"{num2} 的中文表示：{num2.ToChineseNumber()}");
    }

    // ============================================================
    // 13. 结构体
    // ============================================================
    
    /// <summary>
    /// 演示结构体
    /// 结构体是值类型，存储在栈上
    /// 适合轻量级数据结构
    /// </summary>
    public static void DemoStructs()
    {
        Console.WriteLine("--- 结构体演示 ---");

        // 创建 Point 结构体
        Point p1 = new Point(10, 20);
        Point p2 = new Point(30, 40);

        Console.WriteLine($"点1：{p1}");
        Console.WriteLine($"点2：{p2}");
        Console.WriteLine($"两点之间的距离：{p1.DistanceTo(p2):F2}");

        // 值类型的赋值：复制整个值
        Point p3 = p1;  // p3 是 p1 的副本
        p3.Move(5, 5);  // 修改 p3 不会影响 p1
        Console.WriteLine($"\n值类型赋值后：");
        Console.WriteLine($"  p1: {p1}（未改变）");
        Console.WriteLine($"  p3: {p3}（改变了）");

        // 颜色结构体
        Console.WriteLine("\n颜色结构体：");
        ColorRGB red = ColorRGB.Red;
        ColorRGB green = ColorRGB.Green;
        Console.WriteLine($"  红色：{red.ToHex()}");
        Console.WriteLine($"  绿色：{green.ToHex()}");

        ColorRGB customColor = new ColorRGB(128, 64, 255);
        Console.WriteLine($"  自定义颜色：{customColor.ToHex()}");
    }

    // ============================================================
    // 14. 分部类
    // ============================================================
    
    /// <summary>
    /// 演示分部类
    /// 同一个类的代码可以分散在多个地方
    /// 编译后会合并成一个完整的类
    /// </summary>
    public static void DemoPartialClass()
    {
        Console.WriteLine("--- 分部类演示 ---");

        // Employee 类分为两部分定义，但使用时和普通类一样
        Employee emp = new Employee
        {
            Id = 1001,
            Name = "张三",
            Department = "研发部",
            Salary = 15000m,
            Bonus = 5000m
        };

        // 第一部分的方法
        Console.WriteLine("基本信息：");
        emp.PrintBasicInfo();

        // 第二部分的方法
        Console.WriteLine("\n完整信息：");
        emp.PrintFullInfo();

        Console.WriteLine("\n分部类的好处：自动生成代码与手写代码分离、多人协作、功能分组");
    }

    // ============================================================
    // 15. 内部类（嵌套类）
    // ============================================================
    
    /// <summary>
    /// 演示内部类（嵌套类）
    /// 在类的内部定义另一个类
    /// </summary>
    public static void DemoNestedClass()
    {
        Console.WriteLine("--- 内部类演示 ---");

        // 创建订单
        Order order = new Order { OrderId = "ORD20240928001" };

        // 添加订单项（使用内部类 OrderItem）
        order.AddItem("笔记本电脑", 1, 5999m);
        order.AddItem("无线鼠标", 2, 99m);
        order.AddItem("机械键盘", 1, 399m);

        // 打印订单
        order.PrintOrder();

        // 也可以直接创建内部类的实例（需要外部类名前缀）
        Order.OrderItem item = new Order.OrderItem
        {
            ProductName = "显示器",
            Quantity = 1,
            UnitPrice = 1999m
        };
        Console.WriteLine($"\n单独创建的订单项：{item.ProductName}，小计：{item.Subtotal:C}");
    }
}
