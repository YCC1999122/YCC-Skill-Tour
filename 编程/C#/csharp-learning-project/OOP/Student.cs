// ============================================================
// Student.cs - 学生类（继承自Person）
// 演示：继承、base关键字、sealed、override、多态
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 学生类 - 继承自人类 Person
/// 使用冒号 : 表示继承关系，Student 是派生类（子类），Person 是基类（父类）
/// C# 中类只能单继承（只能继承一个基类），但可以实现多个接口
/// </summary>
public class Student : Person
{
    // 学生类自己的私有字段
    private string _school;      // 学校
    private string _studentId;   // 学号

    /// <summary>
    /// 学生构造函数
    /// 使用 base 关键字调用基类的构造函数
    /// 先执行基类构造函数，再执行派生类构造函数
    /// </summary>
    /// <param name="name">姓名</param>
    /// <param name="age">年龄</param>
    /// <param name="school">学校</param>
    /// <param name="studentId">学号</param>
    public Student(string name, int age, string school, string studentId)
        : base(name, age)  // 调用基类 Person 的带两个参数的构造函数
    {
        _school = school;
        _studentId = studentId;
        // 给受保护字段赋值（protected 字段可以在派生类中访问）
        _idCard = "学生身份证号";
        Console.WriteLine($"Student 构造函数被调用，学生 {name} 来自 {school}");
    }

    /// <summary>
    /// 学校属性（自动属性）
    /// </summary>
    public string School
    {
        get { return _school; }
        set { _school = value; }
    }

    /// <summary>
    /// 学号属性（只有get，只读）
    /// </summary>
    public string StudentId
    {
        get { return _studentId; }
    }

    /// <summary>
    /// 重写（override）基类的虚方法 Introduce
    /// 使用 override 关键字重写 base 中标记为 virtual 的方法
    /// 这是实现多态的关键：同一个方法名，不同对象有不同实现
    /// </summary>
    public override void Introduce()
    {
        // 调用基类的 Introduce 方法（先执行父类的行为）
        base.Introduce();
        
        // 然后添加学生特有的行为
        Console.WriteLine($"我是一名学生，在{_school}上学，学号是{_studentId}。");
    }

    /// <summary>
    /// 学生特有的方法：学习
    /// </summary>
    public void Study()
    {
        Console.WriteLine($"{Name} 正在努力学习。");
    }

    /// <summary>
    /// 学生特有的方法：考试
    /// sealed 关键字：这个方法不能再被派生类重写
    /// 注意：sealed 必须和 override 一起使用，表示"最后一次重写"
    /// </summary>
    public sealed override string ToString()
    {
        return $"学生：{Name}，学校：{_school}，学号：{_studentId}";
    }
}

/// <summary>
/// sealed 类：密封类，不能被继承
/// 使用 sealed 关键字标记的类不能作为基类
/// 适用于不需要再扩展的工具类等
/// </summary>
public sealed class GraduateStudent : Student
{
    /// <summary>
    /// 研究生导师
    /// </summary>
    public string Advisor { get; set; } = string.Empty;

    /// <summary>
    /// 构造函数：调用基类构造函数
    /// </summary>
    public GraduateStudent(string name, int age, string school, string studentId, string advisor)
        : base(name, age, school, studentId)
    {
        Advisor = advisor;
    }

    /// <summary>
    /// 重写 Introduce 方法
    /// 注意：ToString 方法在 Student 中被 sealed 了，这里不能重写
    /// </summary>
    public override void Introduce()
    {
        base.Introduce();
        Console.WriteLine($"我是研究生，我的导师是{Advisor}。");
    }

    /// <summary>
    /// 做研究
    /// </summary>
    public void DoResearch()
    {
        Console.WriteLine($"{Name} 在导师{Advisor}的指导下做研究。");
    }
}
