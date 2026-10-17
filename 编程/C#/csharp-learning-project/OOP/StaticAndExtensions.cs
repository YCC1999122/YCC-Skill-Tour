// ============================================================
// StaticAndExtensions.cs - 静态类与扩展方法
// 演示：静态类 static、静态成员、扩展方法 this
// ============================================================

namespace CSharpLearning.OOP;

/// <summary>
/// 静态类
/// 使用 static 关键字修饰
/// 静态类不能被实例化（不能 new），也不能被继承
/// 静态类只能包含静态成员
/// 常用于工具类（工具方法不需要实例状态）
/// </summary>
public static class MathUtility
{
    // 静态构造函数
    // 静态构造函数在第一次使用静态类之前自动调用，只执行一次
    // 不能有访问修饰符，不能有参数
    static MathUtility()
    {
        Console.WriteLine("MathUtility 静态构造函数被调用（只调用一次）");
        RandomSeed = new Random().Next();
    }

    /// <summary>
    /// 静态字段
    /// 静态字段属于类本身，不属于某个实例
    /// 所有实例共享同一个静态字段
    /// </summary>
    public static int RandomSeed;

    /// <summary>
    /// 静态方法：求最大值
    /// 静态方法只能访问静态成员（除非传入实例）
    /// </summary>
    public static int Max(int a, int b)
    {
        return a > b ? a : b;
    }

    /// <summary>
    /// 静态方法：求最小值
    /// </summary>
    public static int Min(int a, int b)
    {
        return a < b ? a : b;
    }

    /// <summary>
    /// 静态方法：判断是否为素数
    /// </summary>
    public static bool IsPrime(int n)
    {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0) return false;
        }
        return true;
    }

    /// <summary>
    /// 静态方法：阶乘
    /// </summary>
    public static long Factorial(int n)
    {
        if (n < 0) throw new ArgumentException("不能计算负数的阶乘");
        long result = 1;
        for (int i = 2; i <= n; i++)
        {
            result *= i;
        }
        return result;
    }
}

/// <summary>
/// 扩展方法类
/// 扩展方法允许在不修改原有类的情况下，为类型添加新方法
/// 扩展方法必须定义在静态类中
/// 第一个参数用 this 关键字修饰，表示要扩展的类型
/// </summary>
public static class StringExtensions
{
    /// <summary>
    /// 扩展方法：判断字符串是否为有效的邮箱格式
    /// this string 表示这个方法扩展了 string 类型
    /// 调用方式：str.IsValidEmail() （就像 string 本身有这个方法一样）
    /// </summary>
    /// <param name="str">要检查的字符串（this参数，调用时不需要显式传递）</param>
    /// <returns>是否为有效邮箱</returns>
    public static bool IsValidEmail(this string str)
    {
        // 简单的邮箱格式验证（实际项目中建议用正则表达式）
        if (string.IsNullOrWhiteSpace(str))
            return false;

        int atIndex = str.IndexOf('@');
        if (atIndex < 1 || atIndex == str.Length - 1)
            return false;

        int dotIndex = str.LastIndexOf('.');
        if (dotIndex <= atIndex || dotIndex == str.Length - 1)
            return false;

        return true;
    }

    /// <summary>
    /// 扩展方法：字符串反转
    /// </summary>
    public static string Reverse(this string str)
    {
        if (string.IsNullOrEmpty(str))
            return str;

        char[] chars = str.ToCharArray();
        Array.Reverse(chars);
        return new string(chars);
    }

    /// <summary>
    /// 扩展方法：将字符串重复 n 次
    /// </summary>
    public static string Repeat(this string str, int count)
    {
        if (string.IsNullOrEmpty(str) || count <= 0)
            return string.Empty;

        return string.Concat(Enumerable.Repeat(str, count));
    }

    /// <summary>
    /// 扩展方法：截断字符串到指定长度，超出部分用省略号代替
    /// </summary>
    public static string Truncate(this string str, int maxLength)
    {
        if (string.IsNullOrEmpty(str) || str.Length <= maxLength)
            return str;

        return str.Substring(0, maxLength) + "...";
    }
}

/// <summary>
/// int 类型的扩展方法
/// 演示对值类型也可以使用扩展方法
/// </summary>
public static class IntExtensions
{
    /// <summary>
    /// 扩展方法：判断整数是否为偶数
    /// </summary>
    public static bool IsEven(this int n)
    {
        return n % 2 == 0;
    }

    /// <summary>
    /// 扩展方法：判断整数是否为奇数
    /// </summary>
    public static bool IsOdd(this int n)
    {
        return n % 2 != 0;
    }

    /// <summary>
    /// 扩展方法：将整数转换为中文数字
    /// </summary>
    public static string ToChineseNumber(this int n)
    {
        string[] digits = { "零", "一", "二", "三", "四", "五", "六", "七", "八", "九" };
        if (n >= 0 && n <= 9)
            return digits[n];

        // 简化版：只处理个位数，返回原始字符串
        return n.ToString();
    }
}
