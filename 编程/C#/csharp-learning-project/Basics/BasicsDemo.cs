// ============================================================
// BasicsDemo.cs - C#基础语法演示
// 功能：演示C#的基本数据类型、变量、运算符、控制流、方法、数组、字符串、可空类型等
// ============================================================

// 文件范围命名空间（C# 10+ 新特性）：整个文件的类型都属于此命名空间
namespace CSharpLearning.Basics;

/// <summary>
/// 基础语法演示类
/// 包含C#语言中最基础的语法元素演示
/// </summary>
public class BasicsDemo
{
    // ============================================================
    // 常量定义区
    // ============================================================
    
    /// <summary>
    /// 常量示例：程序运行期间不可修改的值
    /// const 修饰的常量在编译时就确定值，属于编译时常量
    /// </summary>
    public const double PI = 3.141592653589793;  // 圆周率常量
    
    /// <summary>
    /// 只读字段示例：运行时常量，只能在声明或构造函数中赋值
    /// readonly 与 const 的区别：const是编译时常量，readonly是运行时常量
    /// </summary>
    public static readonly int AppVersion = 100;  // 应用程序版本号

    // ============================================================
    // 枚举定义
    // ============================================================
    
    /// <summary>
    /// 枚举示例：一周的星期
    /// 枚举是一组命名的整型常量，默认底层类型是 int
    /// </summary>
    public enum DayOfWeek
    {
        Monday,        // 星期一，默认值为 0
        Tuesday,       // 星期二，值为 1
        Wednesday,     // 星期三，值为 2
        Thursday,      // 星期四，值为 3
        Friday,        // 星期五，值为 4
        Saturday,      // 星期六，值为 5
        Sunday         // 星期日，值为 6
    }

    /// <summary>
    /// 带标志位的枚举（Flags特性）
    /// 可以使用按位运算组合多个枚举值
    /// </summary>
    [Flags]
    public enum Permissions
    {
        None = 0,          // 无权限（0，二进制 0000）
        Read = 1,          // 读权限（1，二进制 0001）
        Write = 2,         // 写权限（2，二进制 0010）
        Delete = 4,        // 删除权限（4，二进制 0100）
        Admin = 7          // 管理员权限（7，二进制 0111，包含读/写/删）
    }

    // ============================================================
    // 主运行方法：执行所有基础语法演示
    // ============================================================
    
    /// <summary>
    /// 运行所有基础语法演示
    /// 这是Basics模块的入口方法，依次演示各个语法点
    /// </summary>
    public static void RunAllDemos()
    {
        // 输出分隔线，区分不同的演示模块
        Console.WriteLine("========== 基础语法演示 ==========");
        Console.WriteLine();

        // 依次调用各个演示方法
        DemoValueTypes();           // 演示值类型
        Console.WriteLine();
        
        DemoReferenceTypes();       // 演示引用类型
        Console.WriteLine();
        
        DemoVariablesAndConstants();// 演示变量和常量
        Console.WriteLine();
        
        DemoEnums();                // 演示枚举
        Console.WriteLine();
        
        DemoOperators();            // 演示运算符
        Console.WriteLine();
        
        DemoControlFlow();          // 演示控制流
        Console.WriteLine();
        
        DemoMethods();              // 演示方法
        Console.WriteLine();
        
        DemoArrays();               // 演示数组
        Console.WriteLine();
        
        DemoStrings();              // 演示字符串
        Console.WriteLine();
        
        DemoNullableTypes();        // 演示可空类型
        Console.WriteLine();

        Console.WriteLine("========== 基础语法演示结束 ==========");
    }

    // ============================================================
    // 1. 基本数据类型 - 值类型
    // ============================================================
    
    /// <summary>
    /// 演示值类型
    /// 值类型的变量直接存储数据，存储在栈（Stack）上
    /// 常见值类型：整数类型、浮点类型、decimal、bool、char、枚举、结构体
    /// </summary>
    public static void DemoValueTypes()
    {
        Console.WriteLine("--- 值类型演示 ---");

        // ---- 整数类型 ----
        // sbyte: 8位有符号整数，范围 -128 ~ 127
        sbyte sb = 127;
        Console.WriteLine($"sbyte: {sb}，最小值: {sbyte.MinValue}，最大值: {sbyte.MaxValue}");

        // byte: 8位无符号整数，范围 0 ~ 255
        byte b = 255;
        Console.WriteLine($"byte: {b}，最小值: {byte.MinValue}，最大值: {byte.MaxValue}");

        // short: 16位有符号整数，范围 -32768 ~ 32767
        short s = 32767;
        Console.WriteLine($"short: {s}，最小值: {short.MinValue}，最大值: {short.MaxValue}");

        // ushort: 16位无符号整数，范围 0 ~ 65535
        ushort us = 65535;
        Console.WriteLine($"ushort: {us}，最小值: {ushort.MinValue}，最大值: {ushort.MaxValue}");

        // int: 32位有符号整数，最常用的整数类型
        int i = 123456;
        Console.WriteLine($"int: {i}，最小值: {int.MinValue}，最大值: {int.MaxValue}");

        // uint: 32位无符号整数
        uint ui = 4294967295;
        Console.WriteLine($"uint: {ui}，最小值: {uint.MinValue}，最大值: {uint.MaxValue}");

        // long: 64位有符号整数
        long l = 9223372036854775807;
        Console.WriteLine($"long: {l}，最小值: {long.MinValue}，最大值: {long.MaxValue}");

        // ulong: 64位无符号整数
        ulong ul = 18446744073709551615;
        Console.WriteLine($"ulong: {ul}，最小值: {ulong.MinValue}，最大值: {ulong.MaxValue}");

        // ---- 浮点类型 ----
        // float: 32位单精度浮点数，精度约6-9位，后缀 f 或 F
        float f = 3.14f;
        Console.WriteLine($"float: {f}，最小值: {float.MinValue}，最大值: {float.MaxValue}");

        // double: 64位双精度浮点数，精度约15-17位，是默认浮点类型
        double d = 3.1415926535;
        Console.WriteLine($"double: {d}，最小值: {double.MinValue}，最大值: {double.MaxValue}");

        // ---- 高精度十进制 ----
        // decimal: 128位高精度十进制数，适合财务和货币计算，后缀 m 或 M
        decimal money = 12345.67m;
        Console.WriteLine($"decimal: {money}，最小值: {decimal.MinValue}，最大值: {decimal.MaxValue}");

        // ---- 布尔类型 ----
        // bool: 布尔类型，只有 true 和 false 两个值
        bool isTrue = true;
        bool isFalse = false;
        Console.WriteLine($"bool: isTrue={isTrue}, isFalse={isFalse}");

        // ---- 字符类型 ----
        // char: 16位Unicode字符，使用单引号
        char c = 'A';
        Console.WriteLine($"char: {c}，Unicode编码: {(int)c}");

        // ---- 值类型的赋值演示 ----
        // 值类型赋值时，会创建值的副本，两个变量互不影响
        int a = 10;
        int b2 = a;  // 将 a 的值复制给 b2
        b2 = 20;     // 修改 b2 不会影响 a
        Console.WriteLine($"值类型赋值：a={a}, b2={b2}（修改b2不影响a）");
    }

    // ============================================================
    // 2. 基本数据类型 - 引用类型
    // ============================================================
    
    /// <summary>
    /// 演示引用类型
    /// 引用类型的变量存储的是数据的引用（内存地址），数据存储在堆（Heap）上
    /// 常见引用类型：string、object、数组、类、接口、委托、dynamic
    /// </summary>
    public static void DemoReferenceTypes()
    {
        Console.WriteLine("--- 引用类型演示 ---");

        // ---- string 字符串 ----
        // string 是引用类型，但行为上有特殊性（不可变性、字符串驻留）
        string str1 = "Hello World";
        Console.WriteLine($"string: \"{str1}\"，长度: {str1.Length}");

        // ---- object 对象 ----
        // object 是所有类型的基类（C#统一类型系统）
        // 任何类型都可以赋值给 object 类型变量（装箱操作）
        object obj1 = 123;          // 装箱：值类型转为引用类型
        object obj2 = "字符串";       // 引用类型赋值给object
        object obj3 = new int[] { 1, 2, 3 };  // 数组也是引用类型
        Console.WriteLine($"object可以存储任何类型：obj1={obj1}, obj2={obj2}");

        // ---- 引用类型的赋值演示 ----
        // 引用类型赋值时，只是复制引用（地址），两个变量指向同一个对象
        int[] arr1 = new int[] { 1, 2, 3 };
        int[] arr2 = arr1;   // arr2 和 arr1 指向同一个数组对象
        arr2[0] = 99;        // 通过 arr2 修改数组，arr1 也能看到变化
        Console.WriteLine($"引用类型赋值：arr1[0]={arr1[0]}, arr2[0]={arr2[0]}（修改arr2影响arr1）");

        // ---- 动态类型 dynamic ----
        // dynamic 类型在编译时不进行类型检查，类型在运行时确定
        dynamic dyn = 10;
        Console.WriteLine($"dynamic(初始为int): {dyn}，类型: {dyn.GetType().Name}");
        dyn = "现在是字符串";  // 可以改变类型
        Console.WriteLine($"dynamic(改为string): {dyn}，类型: {dyn.GetType().Name}");
    }

    // ============================================================
    // 3. 变量、常量
    // ============================================================
    
    /// <summary>
    /// 演示变量和常量的声明与使用
    /// </summary>
    public static void DemoVariablesAndConstants()
    {
        Console.WriteLine("--- 变量和常量演示 ---");

        // ---- 显式类型变量 ----
        // 先声明后赋值
        int age;      // 声明一个int类型变量
        age = 25;     // 给变量赋值
        Console.WriteLine($"显式类型变量：age={age}");

        // 声明同时赋值
        string name = "张三";
        Console.WriteLine($"声明同时赋值：name={name}");

        // ---- 隐式类型变量（var） ----
        // var 关键字让编译器推断变量类型
        // var 不是动态类型，编译时就确定了类型，和显式声明完全一样
        var number = 42;           // 编译器推断为 int
        var greeting = "你好";      // 编译器推断为 string
        var price = 99.99m;        // 编译器推断为 decimal
        Console.WriteLine($"var隐式类型：number={number}({number.GetType().Name}), greeting={greeting}");

        // ---- 常量 const ----
        // 编译时常量，必须在声明时赋值，不能修改
        const int MaxCount = 100;
        const string AppName = "C#学习程序";
        Console.WriteLine($"常量：MaxCount={MaxCount}, AppName={AppName}");

        // ---- 多个变量同时声明 ----
        int x = 1, y = 2, z = 3;
        Console.WriteLine($"多个变量：x={x}, y={y}, z={z}");
    }

    // ============================================================
    // 4. 枚举
    // ============================================================
    
    /// <summary>
    /// 演示枚举类型的使用
    /// </summary>
    public static void DemoEnums()
    {
        Console.WriteLine("--- 枚举演示 ---");

        // 基本枚举使用
        DayOfWeek today = DayOfWeek.Friday;
        Console.WriteLine($"今天是: {today}，整数值: {(int)today}");

        // 枚举与整数的转换
        int dayValue = 3;
        DayOfWeek day = (DayOfWeek)dayValue;  // 整数转枚举
        Console.WriteLine($"整数{dayValue}对应的枚举: {day}");

        // 遍历枚举所有值
        Console.WriteLine("所有星期：");
        foreach (DayOfWeek d in Enum.GetValues(typeof(DayOfWeek)))
        {
            Console.WriteLine($"  {d} = {(int)d}");
        }

        // ---- 标志位枚举（Flags）----
        // 使用按位或组合多个权限
        Permissions userPerm = Permissions.Read | Permissions.Write;
        Console.WriteLine($"用户权限: {userPerm}，整数值: {(int)userPerm}");

        // 使用 HasFlag 方法检查是否包含某个权限
        bool canRead = userPerm.HasFlag(Permissions.Read);
        bool canDelete = userPerm.HasFlag(Permissions.Delete);
        Console.WriteLine($"是否有读权限: {canRead}，是否有删除权限: {canDelete}");

        // 管理员权限包含所有权限
        Permissions adminPerm = Permissions.Admin;
        Console.WriteLine($"管理员权限: {adminPerm}，包含读权限: {adminPerm.HasFlag(Permissions.Read)}");
    }

    // ============================================================
    // 5. 运算符
    // ============================================================
    
    /// <summary>
    /// 演示C#中的各种运算符
    /// </summary>
    public static void DemoOperators()
    {
        Console.WriteLine("--- 运算符演示 ---");

        int a = 10;
        int b = 3;

        // ---- 算术运算符 ----
        Console.WriteLine("算术运算符：");
        Console.WriteLine($"  {a} + {b} = {a + b}");   // 加法
        Console.WriteLine($"  {a} - {b} = {a - b}");   // 减法
        Console.WriteLine($"  {a} * {b} = {a * b}");   // 乘法
        Console.WriteLine($"  {a} / {b} = {a / b}");   // 整数除法（结果取整）
        Console.WriteLine($"  {a} % {b} = {a % b}");   // 取余（模运算）

        // ---- 自增自减运算符 ----
        int c = 5;
        Console.WriteLine("\n自增自减：");
        Console.WriteLine($"  c初始值: {c}");
        Console.WriteLine($"  c++: {c++}（后自增，返回原值），现在c={c}");
        Console.WriteLine($"  ++c: {++c}（前自增，返回新值），现在c={c}");

        // ---- 关系运算符 ----
        Console.WriteLine("\n关系运算符：");
        Console.WriteLine($"  {a} == {b}: {a == b}");   // 等于
        Console.WriteLine($"  {a} != {b}: {a != b}");   // 不等于
        Console.WriteLine($"  {a} > {b}: {a > b}");     // 大于
        Console.WriteLine($"  {a} < {b}: {a < b}");     // 小于
        Console.WriteLine($"  {a} >= {b}: {a >= b}");   // 大于等于
        Console.WriteLine($"  {a} <= {b}: {a <= b}");   // 小于等于

        // ---- 逻辑运算符 ----
        bool x = true;
        bool y = false;
        Console.WriteLine("\n逻辑运算符：");
        Console.WriteLine($"  true && false = {x && y}");  // 逻辑与（短路）
        Console.WriteLine($"  true || false = {x || y}");  // 逻辑或（短路）
        Console.WriteLine($"  !true = {!x}");              // 逻辑非

        // ---- 条件运算符（三元运算符）----
        int age = 20;
        string status = age >= 18 ? "成年人" : "未成年人";
        Console.WriteLine($"\n三元运算符：年龄{age}，{status}");

        // ---- null合并运算符 ----
        string? maybeNull = null;
        string result = maybeNull ?? "默认值";  // 如果左边为null则取右边
        Console.WriteLine($"null合并运算符：result = \"{result}\"");

        // ---- 按位运算符 ----
        int m = 5;  // 二进制: 0101
        int n = 3;  // 二进制: 0011
        Console.WriteLine("\n按位运算符：");
        Console.WriteLine($"  {m} & {n} = {m & n}  （按位与: 0101 & 0011 = 0001）");
        Console.WriteLine($"  {m} | {n} = {m | n}  （按位或: 0101 | 0011 = 0111）");
        Console.WriteLine($"  {m} ^ {n} = {m ^ n}  （按位异或: 0101 ^ 0011 = 0110）");
        Console.WriteLine($"  ~{m} = {~m}  （按位取反）");
        Console.WriteLine($"  {m} << 1 = {m << 1}  （左移一位，相当于乘2）");
        Console.WriteLine($"  {m} >> 1 = {m >> 1}  （右移一位，相当于除2）");
    }

    // ============================================================
    // 6. 控制流
    // ============================================================
    
    /// <summary>
    /// 演示各种控制流语句
    /// 包括：if/else、switch、for、foreach、while、do-while、break/continue
    /// </summary>
    public static void DemoControlFlow()
    {
        Console.WriteLine("--- 控制流演示 ---");

        // ---- if / else if / else ----
        // 条件判断语句
        Console.WriteLine("if/else 语句：");
        int score = 85;
        if (score >= 90)
        {
            // 当条件为true时执行
            Console.WriteLine("  优秀");
        }
        else if (score >= 80)
        {
            // 当前面的条件都为false，且此条件为true时执行
            Console.WriteLine("  良好");
        }
        else if (score >= 60)
        {
            Console.WriteLine("  及格");
        }
        else
        {
            // 所有条件都为false时执行
            Console.WriteLine("  不及格");
        }

        // ---- switch 语句 ----
        // 多分支选择语句
        Console.WriteLine("\nswitch 语句：");
        char grade = 'B';
        switch (grade)
        {
            case 'A':
                Console.WriteLine("  分数范围：90-100");
                break;  // break 用于跳出 switch，不可省略（除非case为空）
            case 'B':
                Console.WriteLine("  分数范围：80-89");
                break;
            case 'C':
                Console.WriteLine("  分数范围：70-79");
                break;
            case 'D':
                Console.WriteLine("  分数范围：60-69");
                break;
            case 'F':
                Console.WriteLine("  不及格");
                break;
            default:  // 默认分支，所有case都不匹配时执行
                Console.WriteLine("  无效等级");
                break;
        }

        // ---- switch 表达式（C# 8+）----
        // 更简洁的switch写法，直接返回值
        Console.WriteLine("\nswitch 表达式：");
        string dayName = grade switch
        {
            'A' => "优秀",
            'B' => "良好",
            'C' => "中等",
            'D' => "及格",
            'F' => "不及格",
            _ => "无效"  // _ 相当于 default
        };
        Console.WriteLine($"  等级{grade}对应：{dayName}");

        // ---- for 循环 ----
        // 已知循环次数时使用
        Console.WriteLine("\nfor 循环（1到5）：");
        for (int i = 1; i <= 5; i++)  // 初始化; 条件; 迭代
        {
            Console.Write($"  {i}");
        }
        Console.WriteLine();

        // ---- foreach 循环 ----
        // 遍历集合或数组
        Console.WriteLine("\nforeach 循环（遍历数组）：");
        string[] fruits = { "苹果", "香蕉", "橙子" };
        foreach (string fruit in fruits)
        {
            Console.WriteLine($"  {fruit}");
        }

        // ---- while 循环 ----
        // 先判断条件，再执行循环体，可能一次都不执行
        Console.WriteLine("\nwhile 循环（倒计时）：");
        int count = 3;
        while (count > 0)
        {
            Console.WriteLine($"  {count}");
            count--;  // 递减，否则死循环
        }
        Console.WriteLine("  发射！");

        // ---- do-while 循环 ----
        // 先执行循环体，再判断条件，至少执行一次
        Console.WriteLine("\ndo-while 循环：");
        int num = 5;
        do
        {
            Console.WriteLine($"  num = {num}");
            num++;
        } while (num < 3);  // 条件为false，但循环体已经执行了一次

        // ---- break 和 continue ----
        Console.WriteLine("\nbreak 和 continue：");
        for (int i = 1; i <= 10; i++)
        {
            if (i == 5)
            {
                continue;  // 跳过本次循环的剩余部分，继续下一次循环
            }
            if (i == 8)
            {
                break;  // 立即跳出整个循环
            }
            Console.Write($"  {i}");
        }
        Console.WriteLine("（跳过了5，到8就停止了）");
    }

    // ============================================================
    // 7. 方法
    // ============================================================
    
    /// <summary>
    /// 演示方法的各种用法
    /// 包括：普通方法、ref参数、out参数、params数组、可选参数、命名参数
    /// </summary>
    public static void DemoMethods()
    {
        Console.WriteLine("--- 方法演示 ---");

        // ---- 普通方法调用 ----
        int sum = Add(3, 5);
        Console.WriteLine($"普通方法：Add(3, 5) = {sum}");

        // ---- ref 参数 ----
        // ref 参数允许方法修改传递进来的变量（引用传递）
        // 变量在传递前必须初始化
        int x = 10;
        Console.WriteLine($"\nref参数：调用前 x = {x}");
        IncrementByRef(ref x);  // 调用时也要加 ref
        Console.WriteLine($"ref参数：调用后 x = {x}（方法内修改了原值）");

        // ---- out 参数 ----
        // out 参数用于从方法返回多个值
        // 变量在传递前不需要初始化，但方法内必须赋值
        int result;
        int remainder;
        Divide(17, 5, out result, out remainder);
        Console.WriteLine($"\nout参数：17 / 5 = {result} 余 {remainder}");

        // 使用元组也可以返回多个值（C# 7+ 更推荐的方式）
        (int res, int rem) = DivideTuple(17, 5);
        Console.WriteLine($"元组返回：17 / 5 = {res} 余 {rem}");

        // ---- params 参数 ----
        // params 允许传递可变数量的参数
        Console.WriteLine("\nparams参数：");
        int total = SumAll(1, 2, 3, 4, 5);  // 可以传任意数量的int
        Console.WriteLine($"  SumAll(1, 2, 3, 4, 5) = {total}");
        int total2 = SumAll(10, 20);  // 也可以传两个
        Console.WriteLine($"  SumAll(10, 20) = {total2}");

        // ---- 可选参数 ----
        // 可选参数有默认值，调用时可以省略
        Console.WriteLine("\n可选参数：");
        Greet("张三");                     // 使用默认的问候语
        Greet("李四", "早上好");            // 指定问候语

        // ---- 命名参数 ----
        // 命名参数可以不按顺序传递参数，提高可读性
        Console.WriteLine("\n命名参数：");
        PrintInfo(name: "王五", age: 30, city: "北京");
        PrintInfo(city: "上海", name: "赵六", age: 25);  // 顺序可以任意

        // ---- 方法重载 ----
        // 同名方法但参数列表不同（参数类型或数量不同）
        Console.WriteLine("\n方法重载：");
        Console.WriteLine($"  Add(3, 5) = {Add(3, 5)}");        // int版本
        Console.WriteLine($"  Add(3.5, 2.5) = {Add(3.5, 2.5)}"); // double版本
        Console.WriteLine($"  Add(\"Hello\", \" World\") = {Add("Hello", " World")}"); // string版本
    }

    // ---- 方法定义区 ----

    /// <summary>
    /// 加法方法（int版本）
    /// </summary>
    /// <param name="a">第一个整数</param>
    /// <param name="b">第二个整数</param>
    /// <returns>两数之和</returns>
    public static int Add(int a, int b)
    {
        return a + b;  // return 返回结果并结束方法
    }

    /// <summary>
    /// 加法方法重载（double版本）
    /// 方法名相同，但参数类型不同，构成重载
    /// </summary>
    public static double Add(double a, double b)
    {
        return a + b;
    }

    /// <summary>
    /// 加法方法重载（string版本）
    /// 字符串拼接
    /// </summary>
    public static string Add(string a, string b)
    {
        return a + b;
    }

    /// <summary>
    /// 使用ref参数递增数值
    /// ref关键字使得参数按引用传递，方法内修改会影响原始变量
    /// </summary>
    /// <param name="n">要递增的整数引用</param>
    public static void IncrementByRef(ref int n)
    {
        n = n + 1;  // 直接修改原始变量的值
    }

    /// <summary>
    /// 除法运算，使用out参数返回商和余数
    /// out关键字用于从方法中传出多个值
    /// </summary>
    /// <param name="dividend">被除数</param>
    /// <param name="divisor">除数</param>
    /// <param name="result">输出参数：商</param>
    /// <param name="remainder">输出参数：余数</param>
    public static void Divide(int dividend, int divisor, out int result, out int remainder)
    {
        result = dividend / divisor;     // 必须给out参数赋值
        remainder = dividend % divisor;  // 必须给out参数赋值
    }

    /// <summary>
    /// 使用元组返回多个值（C# 7+ 推荐方式）
    /// 比out参数更优雅的多返回值方式
    /// </summary>
    public static (int result, int remainder) DivideTuple(int dividend, int divisor)
    {
        return (dividend / divisor, dividend % divisor);
    }

    /// <summary>
    /// 计算所有传入整数的和
    /// params关键字允许传递可变数量的参数（可以是任意个）
    /// params参数必须是方法的最后一个参数
    /// </summary>
    /// <param name="numbers">可变数量的整数数组</param>
    /// <returns>所有数的和</returns>
    public static int SumAll(params int[] numbers)
    {
        int sum = 0;
        foreach (int n in numbers)
        {
            sum += n;
        }
        return sum;
    }

    /// <summary>
    /// 问候方法，带有可选参数
    /// greeting参数有默认值，调用时可以不传递
    /// 可选参数必须在必选参数之后
    /// </summary>
    /// <param name="name">姓名（必选）</param>
    /// <param name="greeting">问候语（可选，默认"你好"）</param>
    public static void Greet(string name, string greeting = "你好")
    {
        Console.WriteLine($"  {greeting}，{name}！");
    }

    /// <summary>
    /// 打印个人信息
    /// 这个方法用来演示命名参数的用法
    /// </summary>
    public static void PrintInfo(string name, int age, string city)
    {
        Console.WriteLine($"  姓名：{name}，年龄：{age}，城市：{city}");
    }

    // ============================================================
    // 8. 数组
    // ============================================================
    
    /// <summary>
    /// 演示数组的使用
    /// 包括：一维数组、多维数组、交错数组、数组的常用操作
    /// </summary>
    public static void DemoArrays()
    {
        Console.WriteLine("--- 数组演示 ---");

        // ---- 一维数组 ----
        Console.WriteLine("一维数组：");
        
        // 声明并初始化数组的三种方式
        int[] arr1 = new int[5];                     // 指定长度，元素默认值为0
        int[] arr2 = new int[] { 1, 2, 3, 4, 5 };    // 初始化器指定元素
        int[] arr3 = { 10, 20, 30 };                 // 简化写法

        // 数组元素的访问（通过索引，从0开始）
        arr1[0] = 100;  // 设置第一个元素
        arr1[1] = 200;  // 设置第二个元素
        Console.WriteLine($"  arr1[0] = {arr1[0]}");
        Console.WriteLine($"  arr1[1] = {arr1[1]}");
        Console.WriteLine($"  数组长度：{arr1.Length}");

        // 遍历数组
        Console.Write("  arr2的元素：");
        for (int i = 0; i < arr2.Length; i++)
        {
            Console.Write($"{arr2[i]} ");
        }
        Console.WriteLine();

        // ---- 多维数组（矩形数组）----
        Console.WriteLine("\n二维数组（矩形数组）：");
        
        // 声明一个2行3列的二维数组
        int[,] matrix = new int[2, 3]
        {
            { 1, 2, 3 },   // 第0行
            { 4, 5, 6 }    // 第1行
        };

        // 访问二维数组元素：[行索引, 列索引]
        Console.WriteLine($"  matrix[0,0] = {matrix[0, 0]}");
        Console.WriteLine($"  matrix[1,2] = {matrix[1, 2]}");
        Console.WriteLine($"  数组维度：{matrix.Rank}");
        Console.WriteLine($"  第一维长度（行数）：{matrix.GetLength(0)}");
        Console.WriteLine($"  第二维长度（列数）：{matrix.GetLength(1)}");

        // 遍历二维数组
        Console.WriteLine("  遍历二维数组：");
        for (int row = 0; row < matrix.GetLength(0); row++)
        {
            Console.Write("    ");
            for (int col = 0; col < matrix.GetLength(1); col++)
            {
                Console.Write($"{matrix[row, col]} ");
            }
            Console.WriteLine();
        }

        // ---- 交错数组（数组的数组）----
        // 交错数组的每一行可以有不同的长度
        Console.WriteLine("\n交错数组：");
        
        int[][] jagged = new int[3][];  // 3行，但每行长度未定
        jagged[0] = new int[] { 1, 2 };          // 第0行有2个元素
        jagged[1] = new int[] { 3, 4, 5 };       // 第1行有3个元素
        jagged[2] = new int[] { 6, 7, 8, 9 };    // 第2行有4个元素

        // 访问交错数组：[行索引][列索引]
        Console.WriteLine($"  jagged[0][1] = {jagged[0][1]}");
        Console.WriteLine($"  jagged[2][3] = {jagged[2][3]}");

        // 遍历交错数组
        Console.WriteLine("  遍历交错数组：");
        for (int i = 0; i < jagged.Length; i++)
        {
            Console.Write("    ");
            for (int j = 0; j < jagged[i].Length; j++)
            {
                Console.Write($"{jagged[i][j]} ");
            }
            Console.WriteLine($"(第{i}行，{jagged[i].Length}个元素)");
        }

        // ---- Array 类的常用方法 ----
        Console.WriteLine("\nArray类常用方法：");
        int[] nums = { 5, 2, 8, 1, 9 };
        Console.Write("  原数组：");
        PrintArray(nums);

        // 排序
        Array.Sort(nums);
        Console.Write("  排序后：");
        PrintArray(nums);

        // 反转
        Array.Reverse(nums);
        Console.Write("  反转后：");
        PrintArray(nums);

        // 查找
        int index = Array.IndexOf(nums, 8);
        Console.WriteLine($"  元素8的索引：{index}");

        // 清空（设为默认值）
        int[] copyArr = new int[5];
        Array.Copy(nums, copyArr, nums.Length);  // 复制数组
        Console.Write("  复制的数组：");
        PrintArray(copyArr);
    }

    /// <summary>
    /// 辅助方法：打印int数组
    /// </summary>
    private static void PrintArray(int[] arr)
    {
        foreach (int item in arr)
        {
            Console.Write($"{item} ");
        }
        Console.WriteLine();
    }

    // ============================================================
    // 9. 字符串操作
    // ============================================================
    
    /// <summary>
    /// 演示字符串操作
    /// 包括：string 常用方法、字符串插值、StringBuilder
    /// </summary>
    public static void DemoStrings()
    {
        Console.WriteLine("--- 字符串演示 ---");

        string str = "Hello, C# World!";
        Console.WriteLine($"原始字符串：\"{str}\"");

        // ---- string 常用属性 ----
        Console.WriteLine($"\n字符串属性：");
        Console.WriteLine($"  长度：{str.Length}");
        Console.WriteLine($"  第一个字符：{str[0]}");
        Console.WriteLine($"  最后一个字符：{str[^1]}");  // ^1 表示倒数第一个（C# 8+ 索引运算符）

        // ---- string 常用方法 ----
        Console.WriteLine($"\n字符串方法：");
        Console.WriteLine($"  转大写：{str.ToUpper()}");
        Console.WriteLine($"  转小写：{str.ToLower()}");
        Console.WriteLine($"  是否包含\"C#\"：{str.Contains("C#")}");
        Console.WriteLine($"  是否以\"Hello\"开头：{str.StartsWith("Hello")}");
        Console.WriteLine($"  是否以\"!\"结尾：{str.EndsWith("!")}");
        Console.WriteLine($"  \"C#\"的位置：{str.IndexOf("C#")}");
        Console.WriteLine($"  替换：{str.Replace("World", "世界")}");
        Console.WriteLine($"  截取子串(7到9)：{str.Substring(7, 2)}");

        // ---- 字符串插值（C# 6+）----
        // 使用 $ 前缀，在字符串中直接嵌入表达式
        Console.WriteLine($"\n字符串插值：");
        string name = "小明";
        int age = 20;
        string greeting = $"你好，我叫{name}，今年{age}岁。";
        Console.WriteLine($"  {greeting}");

        // 插值字符串中可以使用表达式
        Console.WriteLine($"  明年我就{age + 1}岁了。");

        // ---- 字符串连接 ----
        string s1 = "Hello";
        string s2 = "World";
        string s3 = s1 + " " + s2;  // 使用 + 连接
        Console.WriteLine($"\n字符串连接：\"{s3}\"");

        // 使用 string.Join 连接集合
        string[] words = { "C#", "Java", "Python" };
        string joined = string.Join(", ", words);
        Console.WriteLine($"  string.Join：\"{joined}\"");

        // ---- 字符串分割 ----
        string csv = "苹果,香蕉,橙子,葡萄";
        string[] fruits = csv.Split(',');
        Console.WriteLine($"\n字符串分割：\"{csv}\"");
        Console.Write("  分割结果：");
        foreach (string fruit in fruits)
        {
            Console.Write($"{fruit} ");
        }
        Console.WriteLine();

        // ---- 字符串格式化 ----
        Console.WriteLine($"\n字符串格式化：");
        Console.WriteLine($"  货币：{1234.56:C}");   // 货币格式
        Console.WriteLine($"  百分比：{0.85:P}");    // 百分比格式
        Console.WriteLine($"  数字格式：{1234567.89:N2}");  // 数字格式（2位小数）
        Console.WriteLine($"  日期：{DateTime.Now:yyyy-MM-dd HH:mm:ss}");

        // ---- 空字符串与null ----
        string? emptyStr = "";
        string? nullStr = null;
        Console.WriteLine($"\n空字符串与null：");
        Console.WriteLine($"  string.IsNullOrEmpty(emptyStr)：{string.IsNullOrEmpty(emptyStr)}");
        Console.WriteLine($"  string.IsNullOrEmpty(nullStr)：{string.IsNullOrEmpty(nullStr)}");
        Console.WriteLine($"  string.IsNullOrWhiteSpace(\"  \")：{string.IsNullOrWhiteSpace("  ")}");

        // ---- StringBuilder ----
        // 当需要频繁修改字符串时，使用StringBuilder比直接拼接string更高效
        // 因为string是不可变的，每次修改都会创建新对象
        Console.WriteLine($"\nStringBuilder演示：");
        System.Text.StringBuilder sb = new System.Text.StringBuilder();
        
        // 使用Append方法追加内容（不会创建新字符串）
        sb.Append("这是");
        sb.Append("一个");
        sb.Append("StringBuilder");
        sb.Append("的示例。");
        
        // 也可以追加行
        sb.AppendLine();
        sb.AppendLine("第二行内容。");
        
        // 插入内容
        sb.Insert(2, "（插入的）");
        
        // 最后调用ToString()获取最终字符串
        string result = sb.ToString();
        Console.WriteLine($"  StringBuilder结果：\n  {result}");
        Console.WriteLine($"  字符串长度：{sb.Length}");
    }

    // ============================================================
    // 10. 可空类型
    // ============================================================
    
    /// <summary>
    /// 演示可空类型
    /// 值类型默认不能为null，使用Nullable&lt;T&gt;或?后缀可以让值类型也能为null
    /// </summary>
    public static void DemoNullableTypes()
    {
        Console.WriteLine("--- 可空类型演示 ---");

        // ---- 可空值类型 ----
        // Nullable<int> 等价于 int?
        Nullable<int> nullableInt1 = null;  // 使用 Nullable<T> 写法
        int? nullableInt2 = 42;             // 使用 ? 简写（更常用）
        int? nullableInt3 = null;           // 可以赋值为 null

        Console.WriteLine($"nullableInt1: {nullableInt1}（值为null）");
        Console.WriteLine($"nullableInt2: {nullableInt2}（值为42）");

        // ---- HasValue 和 Value 属性 ----
        // HasValue: 判断是否有值（不为null）
        // Value: 获取值（如果为null会抛异常）
        Console.WriteLine($"\nnullableInt2.HasValue: {nullableInt2.HasValue}");
        if (nullableInt2.HasValue)
        {
            Console.WriteLine($"nullableInt2.Value: {nullableInt2.Value}");
        }

        // ---- 安全获取值的方式 ----
        // 方式1：GetValueOrDefault() - 如果为null返回默认值
        int val1 = nullableInt3.GetValueOrDefault();      // 返回0（int的默认值）
        int val2 = nullableInt3.GetValueOrDefault(100);   // 返回指定的默认值100
        Console.WriteLine($"\nGetValueOrDefault：");
        Console.WriteLine($"  nullableInt3（null）默认值：{val1}");
        Console.WriteLine($"  nullableInt3（null）指定默认值100：{val2}");

        // 方式2：null条件运算符 ?. （C# 6+）
        // 如果左边为null，则整个表达式返回null，不会抛出NullReferenceException
        string? maybeNull = null;
        int? length = maybeNull?.Length;  // maybeNull为null，length也为null
        Console.WriteLine($"\nnull条件运算符：");
        Console.WriteLine($"  maybeNull?.Length = {length}（值为null，不抛异常）");

        // 方式3：null合并运算符 ??
        // 如果左边为null，则取右边的值
        string realValue = maybeNull ?? "默认字符串";
        Console.WriteLine($"  null合并运算符：realValue = \"{realValue}\"");

        // ---- 可空引用类型（C# 8+）----
        // 在启用了Nullable的项目中，引用类型默认不可为null
        // string 表示不可为null的字符串
        // string? 表示可为null的字符串
        Console.WriteLine($"\n可空引用类型（C# 8+）：");
        string notNullString = "一定不为null";  // 编译器保证不会为null
        string? maybeNullString = null;          // 可以为null
        Console.WriteLine($"  notNullString: \"{notNullString}\"");
        Console.WriteLine($"  maybeNullString: {maybeNullString ?? "(null)"}");

        // ---- 可空枚举 ----
        DayOfWeek? nullableDay = null;  // 枚举也可以是可空的
        Console.WriteLine($"\n可空枚举：nullableDay = {nullableDay ?? DayOfWeek.Monday}（使用了默认值）");

        // ---- null包容运算符 ! ----
        // 告诉编译器"我确定这个值不为null"，抑制编译器警告
        // 慎用！如果实际为null，运行时仍会抛异常
        string definitelyNotNull = maybeNullString!;  // 强制认为不为null
        // 注意：这里只是演示语法，实际运行时如果maybeNullString为null，访问其成员会异常
        Console.WriteLine($"  null包容运算符：definitelyNotNull = {definitelyNotNull ?? "(实际上还是null)"}");
    }
}
