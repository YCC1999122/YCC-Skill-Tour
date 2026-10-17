// ============================================================
// StructsAndPartials.cs - 结构体与分部类
// 演示：结构体 struct、分部类 partial、内部类
// ============================================================

namespace CSharpLearning.OOP;

// ============================================================
// 结构体（Struct）
// 结构体是值类型，存储在栈上
// 结构体适合表示轻量级的数据结构
// 结构体不能继承其他结构体或类，也不能作为基类
// 但结构体可以实现接口
// ============================================================

/// <summary>
/// 点结构体 - 表示二维坐标系中的一个点
/// struct 关键字定义结构体
/// 结构体是值类型，赋值时会复制整个值
/// </summary>
public struct Point
{
    /// <summary>
    /// X 坐标
    /// </summary>
    public int X { get; set; }

    /// <summary>
    /// Y 坐标
    /// </summary>
    public int Y { get; set; }

    /// <summary>
    /// 构造函数
    /// 结构体可以有构造函数，但不能有无参构造函数（编译器会自动提供）
    /// 在 C# 10+ 中可以定义无参构造函数
    /// </summary>
    public Point(int x, int y)
    {
        X = x;
        Y = y;
    }

    /// <summary>
    /// 方法：计算到另一个点的距离
    /// </summary>
    public double DistanceTo(Point other)
    {
        int dx = X - other.X;
        int dy = Y - other.Y;
        return Math.Sqrt(dx * dx + dy * dy);
    }

    /// <summary>
    /// 方法：移动点的位置
    /// </summary>
    public void Move(int dx, int dy)
    {
        X += dx;
        Y += dy;
    }

    /// <summary>
    /// 重写 ToString 方法
    /// </summary>
    public override string ToString()
    {
        return $"({X}, {Y})";
    }
}

/// <summary>
/// 颜色结构体
/// 使用结构体表示颜色（典型的轻量级数据）
/// </summary>
public struct ColorRGB
{
    /// <summary>
    /// 红色分量 (0-255)
    /// </summary>
    public byte R;

    /// <summary>
    /// 绿色分量 (0-255)
    /// </summary>
    public byte G;

    /// <summary>
    /// 蓝色分量 (0-255)
    /// </summary>
    public byte B;

    /// <summary>
    /// 构造函数
    /// </summary>
    public ColorRGB(byte r, byte g, byte b)
    {
        R = r;
        G = g;
        B = b;
    }

    /// <summary>
    /// 预定义颜色：红色
    /// 静态属性
    /// </summary>
    public static ColorRGB Red => new ColorRGB(255, 0, 0);

    /// <summary>
    /// 预定义颜色：绿色
    /// </summary>
    public static ColorRGB Green => new ColorRGB(0, 255, 0);

    /// <summary>
    /// 预定义颜色：蓝色
    /// </summary>
    public static ColorRGB Blue => new ColorRGB(0, 0, 255);

    /// <summary>
    /// 转十六进制表示
    /// </summary>
    public string ToHex()
    {
        return $"#{R:X2}{G:X2}{B:X2}";
    }
}

// ============================================================
// 分部类（Partial Class）
// 使用 partial 关键字将一个类的定义分散到多个文件中
// 编译器会把所有部分合并成一个完整的类
// 常用于：
//   1. 自动生成的代码（如 WinForms、EF）与手写代码分离
//   2. 多人协作开发大型类
//   3. 将不同功能的代码分到不同文件
// ============================================================

/// <summary>
/// 员工类（第一部分）
/// partial 关键字表示这是分部类的一部分
/// 这个文件主要定义员工的基本信息
/// </summary>
public partial class Employee
{
    /// <summary>
    /// 员工ID
    /// </summary>
    public int Id { get; set; }

    /// <summary>
    /// 员工姓名
    /// </summary>
    public string Name { get; set; } = string.Empty;

    /// <summary>
    /// 部门
    /// </summary>
    public string Department { get; set; } = string.Empty;

    /// <summary>
    /// 第一部分的方法
    /// </summary>
    public void PrintBasicInfo()
    {
        Console.WriteLine($"员工ID：{Id}，姓名：{Name}，部门：{Department}");
    }
}

/// <summary>
/// 员工类（第二部分）
/// 同一个类的另一个部分，放在不同文件（或同一文件）中
/// 这个文件主要定义员工的薪资信息
/// 注意：虽然分开定义，但编译后它们是同一个类
/// </summary>
public partial class Employee
{
    /// <summary>
    /// 基本工资
    /// </summary>
    public decimal Salary { get; set; }

    /// <summary>
    /// 奖金
    /// </summary>
    public decimal Bonus { get; set; }

    /// <summary>
    /// 第二部分的方法：计算总收入
    /// </summary>
    /// <returns>总收入</returns>
    public decimal GetTotalIncome()
    {
        return Salary + Bonus;
    }

    /// <summary>
    /// 打印完整信息（可以访问两部分的成员）
    /// </summary>
    public void PrintFullInfo()
    {
        // 可以访问第一部分定义的属性（Id, Name, Department）
        Console.WriteLine($"员工 {Name}（ID:{Id}）在{Department}工作");
        // 也可以访问本部分定义的属性（Salary, Bonus）
        Console.WriteLine($"薪资：基本工资{Salary:C}，奖金{Bonus:C}，总计{GetTotalIncome():C}");
    }
}

// ============================================================
// 内部类 / 嵌套类（Nested Class）
// 在一个类的内部定义另一个类
// 内部类可以访问外部类的所有成员（包括私有成员）
// 适合定义只在外部类内部使用的辅助类
// ============================================================

/// <summary>
/// 外部类：订单类
/// </summary>
public class Order
{
    /// <summary>
    /// 订单号
    /// </summary>
    public string OrderId { get; set; } = string.Empty;

    /// <summary>
    /// 私有字段：订单状态码
    /// </summary>
    private int _statusCode;

    /// <summary>
    /// 订单总金额
    /// </summary>
    public decimal TotalAmount { get; set; }

    /// <summary>
    /// 内部类：订单项
    /// 嵌套在 Order 类内部
    /// </summary>
    public class OrderItem
    {
        /// <summary>
        /// 商品名称
        /// </summary>
        public string ProductName { get; set; } = string.Empty;

        /// <summary>
        /// 数量
        /// </summary>
        public int Quantity { get; set; }

        /// <summary>
        /// 单价
        /// </summary>
        public decimal UnitPrice { get; set; }

        /// <summary>
        /// 小计
        /// </summary>
        public decimal Subtotal => Quantity * UnitPrice;
    }

    // 订单项列表
    private List<OrderItem> _items = new List<OrderItem>();

    /// <summary>
    /// 添加订单项
    /// </summary>
    public void AddItem(string productName, int quantity, decimal unitPrice)
    {
        _items.Add(new OrderItem
        {
            ProductName = productName,
            Quantity = quantity,
            UnitPrice = unitPrice
        });

        // 重新计算总金额
        TotalAmount = _items.Sum(item => item.Subtotal);
    }

    /// <summary>
    /// 打印订单
    /// </summary>
    public void PrintOrder()
    {
        Console.WriteLine($"订单号：{OrderId}");
        Console.WriteLine($"订单项：");
        foreach (var item in _items)
        {
            Console.WriteLine($"  {item.ProductName} x {item.Quantity} = {item.Subtotal:C}");
        }
        Console.WriteLine($"总金额：{TotalAmount:C}");
    }
}
