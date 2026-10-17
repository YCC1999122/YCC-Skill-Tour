// ============================================================
// Person.cs - 人类（基类）
// 演示：类与对象、构造函数、析构函数、终结器、封装、属性、索引器
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 人类 - 作为基类，演示面向对象的基本概念
/// 封装：将数据和方法包装在类中，通过访问修饰符控制访问权限
/// </summary>
public class Person
{
    // ============================================================
    // 字段（Fields）- 私有字段体现封装性
    // 私有字段只能在类内部访问，外部通过属性来间接访问
    // ============================================================
    
    /// <summary>
    /// 私有字段：姓名
    /// private 访问修饰符：只有当前类内部可以访问
    /// 这是封装的核心 - 数据隐藏
    /// </summary>
    private string _name;

    /// <summary>
    /// 私有字段：年龄
    /// 外部不能直接修改，通过属性控制年龄的合法性
    /// </summary>
    private int _age;

    /// <summary>
    /// 受保护字段：身份标识
    /// protected 访问修饰符：当前类和派生类可以访问
    /// </summary>
    protected string _idCard = string.Empty;

    /// <summary>
    /// 内部字段：创建计数
    /// internal 访问修饰符：同一程序集内可以访问
    /// </summary>
    internal static int PersonCount = 0;

    // ============================================================
    // 构造函数（Constructors）
    // 构造函数在创建对象时自动调用，用于初始化对象
    // 构造函数名称必须与类名相同，没有返回值
    // ============================================================

    /// <summary>
    /// 无参数构造函数
    /// 如果没有定义任何构造函数，编译器会自动提供一个默认的无参构造函数
    /// </summary>
    public Person()
    {
        _name = "未知";
        _age = 0;
        PersonCount++;  // 每创建一个对象，计数加1
        Console.WriteLine($"Person() 无参构造函数被调用，当前人数：{PersonCount}");
    }

    /// <summary>
    /// 带参数的构造函数（构造函数重载）
    /// 使用 this 关键字引用当前对象的成员
    /// </summary>
    /// <param name="name">姓名</param>
    /// <param name="age">年龄</param>
    public Person(string name, int age)
    {
        // this.name 指当前对象的字段 _name
        // 这里使用 this 区分字段和参数（当参数名与字段名相同时）
        this._name = name;
        this._age = age;
        PersonCount++;
        Console.WriteLine($"Person(name, age) 构造函数被调用，创建了 {name}");
    }

    /// <summary>
    /// 构造函数链：使用 this 调用本类的另一个构造函数
    /// 避免代码重复
    /// </summary>
    /// <param name="name">姓名</param>
    public Person(string name) : this(name, 0)  // 调用上面的两个参数构造函数
    {
        // 这里的代码在调用的构造函数执行完之后执行
        Console.WriteLine($"Person(name) 构造函数被调用（链式调用）");
    }

    // ============================================================
    // 析构函数 / 终结器（Finalizer）
    // 使用 ~ 符号声明，用于在对象被垃圾回收时执行清理工作
    // 注意：尽量少用，会影响GC性能，优先使用IDisposable接口
    // ============================================================

    /// <summary>
    /// 析构函数（终结器）
    /// 在对象被垃圾回收时自动调用
    /// 方法名是 ~ 加类名，没有参数，没有访问修饰符
    /// </summary>
    ~Person()
    {
        PersonCount--;  // 对象被销毁时，计数减1
        Console.WriteLine($"~Person() 析构函数被调用，{_name} 被销毁，剩余人数：{PersonCount}");
    }

    // ============================================================
    // 属性（Properties）
    // 属性是字段的扩展，提供了灵活的方式来读取、写入或计算私有字段的值
    // ============================================================

    /// <summary>
    /// 完整属性：Name（姓名）
    /// get 访问器：用于读取属性值
    /// set 访问器：用于设置属性值（value 是隐式参数，代表传入的值）
    /// 这是传统的完整属性写法
    /// </summary>
    public string Name
    {
        get
        {
            // get 访问器返回字段的值
            return _name;
        }
        set
        {
            // set 访问器设置字段的值
            // 可以在这里添加验证逻辑
            if (string.IsNullOrEmpty(value))
            {
                Console.WriteLine("警告：姓名不能为空！");
                return;
            }
            _name = value;
        }
    }

    /// <summary>
    /// 属性：Age（年龄）
    /// 在set中添加了数据验证，保证年龄的合法性
    /// 这就是封装的好处：可以控制数据的访问和修改
    /// </summary>
    public int Age
    {
        get { return _age; }
        set
        {
            // 验证：年龄必须在合理范围内
            if (value < 0 || value > 150)
            {
                Console.WriteLine("警告：年龄不合法！");
                return;
            }
            _age = value;
        }
    }

    /// <summary>
    /// 自动属性（Auto-implemented Property）
    /// 当不需要额外逻辑时，可以使用自动属性简化代码
    /// 编译器会自动生成一个匿名的私有字段
    /// 这里 Gender 属性只有 public 的 get，set 是 private 的
    /// 即：外部只能读取，类内部可以修改
    /// </summary>
    public string Gender { get; private set; } = "未知";

    /// <summary>
    /// 计算属性（Calculated Property）
    /// 只有 get，没有 set，值是通过计算得到的
    /// 表达式体成员（C# 6+）：使用 => 简化只有一行的方法/属性
    /// </summary>
    public string Info => $"姓名：{Name}，年龄：{Age}，性别：{Gender}";

    /// <summary>
    /// Init-only 属性（C# 9+）
    /// init 访问器：只能在对象初始化时赋值，之后不可修改
    /// 介于 get-only 和 set 之间的一种状态
    /// </summary>
    public DateTime BirthDate { get; init; } = DateTime.MinValue;

    // ============================================================
    // 索引器（Indexers）
    // 索引器允许像数组一样通过索引来访问对象的元素
    // 使用 this 关键字定义
    // ============================================================

    // 存储联系人信息的数组
    private string[] _contacts = new string[5];

    /// <summary>
    /// 索引器：通过索引访问联系人
    /// this[int index] 表示这是一个整数索引的索引器
    /// </summary>
    /// <param name="index">索引位置</param>
    /// <returns>对应位置的联系人</returns>
    public string this[int index]
    {
        get
        {
            // 边界检查
            if (index < 0 || index >= _contacts.Length)
            {
                Console.WriteLine("索引越界！");
                return string.Empty;
            }
            return _contacts[index];
        }
        set
        {
            if (index >= 0 && index < _contacts.Length)
            {
                _contacts[index] = value;
            }
        }
    }

    // ============================================================
    // 方法（Methods）
    // ============================================================

    /// <summary>
    /// 虚方法：介绍自己
    /// virtual 关键字表示这个方法可以在派生类中被重写（override）
    /// 这是实现多态的基础
    /// </summary>
    public virtual void Introduce()
    {
        Console.WriteLine($"大家好，我叫{_name}，今年{_age}岁。");
    }

    /// <summary>
    /// 普通方法：吃饭
    /// </summary>
    public void Eat()
    {
        Console.WriteLine($"{_name} 正在吃饭。");
    }

    /// <summary>
    /// 普通方法：睡觉
    /// </summary>
    public void Sleep()
    {
        Console.WriteLine($"{_name} 正在睡觉。");
    }

    /// <summary>
    /// 受保护方法：只有当前类和派生类可以调用
    /// </summary>
    protected void PrivateMethod()
    {
        Console.WriteLine("这是一个受保护的方法，只有Person和它的子类能调用。");
    }
}
