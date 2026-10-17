// ============================================================
// Shapes.cs - 抽象类与多态
// 演示：抽象类 abstract、抽象方法、多态
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 形状抽象类
/// abstract 关键字表示这是一个抽象类
/// 抽象类不能被实例化（不能直接 new Shape()），只能被继承
/// 抽象类可以包含抽象成员和非抽象成员
/// </summary>
public abstract class Shape
{
    /// <summary>
    /// 形状颜色
    /// </summary>
    public string Color { get; set; } = "黑色";

    /// <summary>
    /// 抽象属性：面积
    /// 只有声明，没有实现（没有 get 的具体代码）
    /// 派生类必须实现这个属性
    /// </summary>
    public abstract double Area { get; }

    /// <summary>
    /// 抽象方法：绘制图形
    /// 抽象方法只有方法签名，没有方法体（用分号结束）
    /// 派生类必须使用 override 重写这个方法
    /// 抽象方法只能存在于抽象类中
    /// </summary>
    public abstract void Draw();

    /// <summary>
    /// 普通方法：获取形状信息
    /// 抽象类中可以有普通方法（有具体实现）
    /// 这是抽象类和接口的区别之一
    /// </summary>
    public void ShowInfo()
    {
        Console.WriteLine($"这是一个{Color}的形状，面积为{Area:F2}。");
    }
}

/// <summary>
/// 圆形类 - 继承自抽象类 Shape
/// 必须实现所有的抽象成员（Area 属性和 Draw 方法）
/// </summary>
public class Circle : Shape
{
    /// <summary>
    /// 圆的半径
    /// </summary>
    public double Radius { get; set; }

    /// <summary>
    /// 构造函数
    /// </summary>
    /// <param name="radius">半径</param>
    public Circle(double radius)
    {
        Radius = radius;
    }

    /// <summary>
    /// 实现抽象属性 Area
    /// 圆的面积 = π * r²
    /// 使用 override 关键字重写抽象属性
    /// </summary>
    public override double Area
    {
        get { return Math.PI * Radius * Radius; }
    }

    /// <summary>
    /// 实现抽象方法 Draw
    /// </summary>
    public override void Draw()
    {
        Console.WriteLine($"画一个{Color}的圆，半径为{Radius}。");
    }
}

/// <summary>
/// 矩形类 - 继承自抽象类 Shape
/// </summary>
public class Rectangle : Shape
{
    /// <summary>
    /// 宽度
    /// </summary>
    public double Width { get; set; }

    /// <summary>
    /// 高度
    /// </summary>
    public double Height { get; set; }

    /// <summary>
    /// 构造函数
    /// </summary>
    public Rectangle(double width, double height)
    {
        Width = width;
        Height = height;
    }

    /// <summary>
    /// 实现抽象属性 Area
    /// 矩形面积 = 宽 * 高
    /// </summary>
    public override double Area
    {
        get { return Width * Height; }
    }

    /// <summary>
    /// 实现抽象方法 Draw
    /// </summary>
    public override void Draw()
    {
        Console.WriteLine($"画一个{Color}的矩形，宽{Width}，高{Height}。");
    }
}

/// <summary>
/// 三角形类 - 继承自抽象类 Shape
/// </summary>
public class Triangle : Shape
{
    /// <summary>
    /// 底边长
    /// </summary>
    public double Base { get; set; }

    /// <summary>
    /// 高
    /// </summary>
    public double Height { get; set; }

    /// <summary>
    /// 构造函数
    /// </summary>
    public Triangle(double @base, double height)
    {
        Base = @base;   // base 是关键字，用 @ 转义作为变量名
        Height = height;
    }

    /// <summary>
    /// 实现抽象属性 Area
    /// 三角形面积 = 底 * 高 / 2
    /// </summary>
    public override double Area
    {
        get { return Base * Height / 2; }
    }

    /// <summary>
    /// 实现抽象方法 Draw
    /// </summary>
    public override void Draw()
    {
        Console.WriteLine($"画一个{Color}的三角形，底{Base}，高{Height}。");
    }
}
