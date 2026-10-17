/**
 * ============================================================
 * 包声明：package语句必须是Java源文件的第一行有效代码（注释不算）
 * 作用：将类组织到特定的命名空间中，避免类名冲突
 * 命名规范：通常是公司域名倒写 + 项目名/模块名
 * 这里的 com.learning.basics 表示 com组织 -> learning项目 -> basics模块
 * ============================================================
 */
package com.learning.basics;

/**
 * ============================================================
 * 导入语句：import用于引入其他包中的类，这样就不用写完整的包名了
 * java.util.Arrays 是Java提供的数组工具类，包含各种数组操作的静态方法
 * java.util.* 表示导入java.util包下的所有类（但不包括子包）
 * ============================================================
 */
import java.util.Arrays;

/**
 * ============================================================
 * BasicsDemo 类：Java基础语法演示类
 *
 * 【类的定义格式】
 * public class 类名 { 类体 }
 *
 * 【public访问修饰符】
 * - public：公开的，任何地方都可以访问
 * - 一个Java源文件中最多只能有一个public类
 * - public类的类名必须和文件名完全一致（包括大小写）
 *
 * 【类名命名规范】
 * - 大驼峰命名法（UpperCamelCase）：每个单词首字母大写
 * - 例如：BasicsDemo、StringBuilder、HelloWorld
 * ============================================================
 */
public class BasicsDemo {

    /**
     * ============================================================
     * main方法：Java程序的入口方法，JVM会从这里开始执行程序
     *
     * 【方法签名详解】
     * public：公开的，JVM可以从外部调用
     * static：静态的，属于类本身，不需要创建对象就能调用
     * void：返回值类型，表示该方法不返回任何值
     * main：方法名，固定写法，JVM只认这个名字
     * String[] args：参数列表，表示命令行参数
     *   - args是字符串数组，用来接收运行时传入的参数
     *   - 例如：java BasicsDemo hello world → args = ["hello", "world"]
     * ============================================================
     */
    public static void main(String[] args) {
        // 打印欢迎信息
        System.out.println("========== Java基础语法演示 ==========");

        // 调用各个演示方法，每个方法演示一个知识点
        demonstratePrimitiveTypes();   // 演示基本数据类型
        demonstrateWrapperClass();     // 演示包装类与自动装箱拆箱
        demonstrateOperators();        // 演示运算符
        demonstrateControlFlow();      // 演示控制流
        demonstrateMethods();          // 演示方法定义与重载
        demonstrateArrays();           // 演示数组
        demonstrateString();           // 演示字符串

        System.out.println("========== 基础语法演示结束 ==========\n");
    }

    /**
     * ============================================================
     * 演示Java的8种基本数据类型
     *
     * 【基本数据类型 vs 引用数据类型】
     * 基本数据类型：变量存的是值本身，存储在栈内存中
     *   - 8种：byte, short, int, long, float, double, char, boolean
     * 引用数据类型：变量存的是对象的地址（引用），对象本身存储在堆内存中
     *   - 例如：String, 数组, 自定义类等
     * ============================================================
     */
    public static void demonstratePrimitiveTypes() {
        System.out.println("\n--- 1. 8种基本数据类型 ---");

        // ==========================================
        // 整数类型（4种）：都是有符号整数，最高位表示符号位（0正1负）
        // ==========================================

        /**
         * byte：字节型，1个字节（8位）
         * 取值范围：-2^7 ~ 2^7-1 = -128 ~ 127
         * 使用场景：节省内存，处理二进制数据（如文件流、网络流）
         */
        byte byteVar = 100;        // 声明一个byte变量，赋值为100
        System.out.println("byte类型: " + byteVar + "，取值范围: " + Byte.MIN_VALUE + " ~ " + Byte.MAX_VALUE);

        /**
         * short：短整型，2个字节（16位）
         * 取值范围：-2^15 ~ 2^15-1 = -32768 ~ 32767
         * 使用场景：需要比byte大但比int小的整数，实际使用较少
         */
        short shortVar = 10000;    // 声明一个short变量，赋值为10000
        System.out.println("short类型: " + shortVar + "，取值范围: " + Short.MIN_VALUE + " ~ " + Short.MAX_VALUE);

        /**
         * int：整型，4个字节（32位）
         * 取值范围：-2^31 ~ 2^31-1 = -2147483648 ~ 2147483647
         * 使用场景：Java中最常用的整数类型，整数字面量默认就是int类型
         * 注意：如果数字太大超过int范围，编译会报错
         */
        int intVar = 100000;       // 声明一个int变量，赋值为100000
        System.out.println("int类型: " + intVar + "，取值范围: " + Integer.MIN_VALUE + " ~ " + Integer.MAX_VALUE);

        /**
         * long：长整型，8个字节（64位）
         * 取值范围：-2^63 ~ 2^63-1 = -9223372036854775808 ~ 9223372036854775807
         * 使用场景：表示很大的整数，如时间戳、文件大小等
         * 注意：long类型的字面量后面必须加L或l（建议大写L，小写l容易和数字1混淆）
         */
        long longVar = 10000000000L; // 声明一个long变量，注意结尾的L
        System.out.println("long类型: " + longVar + "，取值范围: " + Long.MIN_VALUE + " ~ " + Long.MAX_VALUE);

        // ==========================================
        // 浮点类型（2种）：用于表示带小数的数字
        // ==========================================

        /**
         * float：单精度浮点型，4个字节（32位）
         * 精度：大约6-7位有效数字
         * 使用场景：对精度要求不高、需要节省内存的场景
         * 注意：float类型的字面量后面必须加F或f
         * 注意：浮点数不能精确表示所有小数（如0.1），会有精度损失
         */
        float floatVar = 3.14F;    // 声明一个float变量，注意结尾的F
        System.out.println("float类型: " + floatVar + "，取值范围: " + Float.MIN_VALUE + " ~ " + Float.MAX_VALUE);

        /**
         * double：双精度浮点型，8个字节（64位）
         * 精度：大约15-16位有效数字
         * 使用场景：Java中最常用的浮点类型，浮点数字面量默认就是double类型
         * 注意：精确计算（如金额）应该用BigDecimal，不要用浮点数
         */
        double doubleVar = 3.1415926; // 声明一个double变量
        System.out.println("double类型: " + doubleVar + "，取值范围: " + Double.MIN_VALUE + " ~ " + Double.MAX_VALUE);

        // ==========================================
        // 字符类型（1种）
        // ==========================================

        /**
         * char：字符型，2个字节（16位）
         * 取值范围：0 ~ 65535（即 '\u0000' ~ '\uFFFF'）
         * 使用场景：表示单个字符（Unicode字符）
         * 注意：char是无符号的，所以没有负数
         * 表示方式：
         *   - 单引号包裹：'A'、'中'
         *   - 转义字符：'\n'（换行）、'\t'（制表符）、'\\'（反斜杠）、'\''（单引号）
         *   - Unicode编码：'\u4e2d'（表示中文"中"）
         *   - 整数形式：65（表示字符'A'）
         */
        char charVar = 'A';        // 声明一个char变量，赋值为字符'A'
        System.out.println("char类型: " + charVar + "，对应的ASCII码: " + (int) charVar);
        char chineseChar = '中';   // char可以表示中文，因为Java使用Unicode编码
        System.out.println("char类型（中文）: " + chineseChar + "，Unicode编码: \\u" + Integer.toHexString(chineseChar));

        // ==========================================
        // 布尔类型（1种）
        // ==========================================

        /**
         * boolean：布尔型，只有两个值：true 和 false
         * 大小：JVM规范没有明确规定，通常占用1个字节或1个比特
         * 使用场景：条件判断、逻辑运算
         * 注意：Java中boolean和整数不能互相转换（和C语言不同）
         */
        boolean boolVar = true;    // 声明一个boolean变量，赋值为true
        System.out.println("boolean类型: " + boolVar);
        boolean boolVar2 = false;  // 布尔值只有true和false两个
        System.out.println("boolean类型2: " + boolVar2);

        // ==========================================
        // 类型转换
        // ==========================================
        System.out.println("\n--- 类型转换 ---");

        /**
         * 自动类型转换（隐式转换）：小类型 → 大类型
         * 转换规则（按容量从小到大）：
         * byte → short → int → long → float → double
         *                  ↑
         *                char
         * 注意：char和byte/short之间不能自动转换，需要强制转换
         * 注意：整数可以自动转浮点，但可能损失精度
         */
        int num1 = 100;            // 定义一个int变量
        long num2 = num1;          // int自动转long（小转大，自动完成）
        System.out.println("int自动转long: " + num2);

        /**
         * 强制类型转换（显式转换）：大类型 → 小类型
         * 语法：(目标类型) 值
         * 风险：可能导致精度损失或数据溢出
         * 使用场景：确认值在目标类型范围内时使用
         */
        double num3 = 3.99;        // 定义一个double变量
        int num4 = (int) num3;     // double强制转int，小数部分会被截断（不是四舍五入）
        System.out.println("double强制转int: " + num4 + "（小数部分被截断）");
    }

    /**
     * ============================================================
     * 演示包装类与自动装箱拆箱
     *
     * 【包装类是什么】
     * Java是面向对象的语言，但8种基本数据类型不是对象，这在某些场景下不方便
     * （比如集合只能存对象，泛型不支持基本类型）
     * 所以Java为每种基本类型都提供了对应的包装类，把基本类型"包装"成对象
     *
     * 【基本类型 → 包装类 对应关系】
     * byte → Byte
     * short → Short
     * int → Integer
     * long → Long
     * float → Float
     * double → Double
     * char → Character
     * boolean → Boolean
     *
     * 【自动装箱与拆箱（Java 5+）】
     * 自动装箱（Autoboxing）：基本类型自动转换为包装类对象
     * 自动拆箱（Unboxing）：包装类对象自动转换为基本类型
     * 本质：编译器帮我们自动调用了 valueOf() 和 xxxValue() 方法
     * ============================================================
     */
    public static void demonstrateWrapperClass() {
        System.out.println("\n--- 2. 包装类与自动装箱拆箱 ---");

        // --- 手动装箱与拆箱（Java 5之前的写法，现在不推荐） ---
        System.out.println("【手动装箱拆箱】");
        Integer intObj1 = Integer.valueOf(100); // 手动装箱：int → Integer对象
        int intVal1 = intObj1.intValue();       // 手动拆箱：Integer对象 → int
        System.out.println("手动装箱: Integer.valueOf(100) = " + intObj1);
        System.out.println("手动拆箱: intObj.intValue() = " + intVal1);

        // --- 自动装箱与拆箱（Java 5及以后，编译器自动完成） ---
        System.out.println("\n【自动装箱拆箱】");
        Integer intObj2 = 200;       // 自动装箱：编译器自动改为 Integer.valueOf(200)
        int intVal2 = intObj2;       // 自动拆箱：编译器自动改为 intObj2.intValue()
        System.out.println("自动装箱: Integer intObj = 200 → " + intObj2);
        System.out.println("自动拆箱: int val = intObj → " + intVal2);

        // --- 包装类的常用方法 ---
        System.out.println("\n【包装类常用方法】");
        // parseInt：字符串转int（静态方法）
        int parsedInt = Integer.parseInt("123");
        System.out.println("Integer.parseInt(\"123\") = " + parsedInt);

        // valueOf：基本类型/字符串转包装类（静态方法，有缓存）
        Integer intObj3 = Integer.valueOf("456");
        System.out.println("Integer.valueOf(\"456\") = " + intObj3);

        // toString：转字符串
        String intStr = Integer.toString(789);
        System.out.println("Integer.toString(789) = " + intStr);

        // compare：比较两个值（静态方法）
        int compareResult = Integer.compare(10, 20);
        System.out.println("Integer.compare(10, 20) = " + compareResult + "（负数表示小于）");

        // --- Integer缓存机制（重要面试题） ---
        System.out.println("\n【Integer缓存机制（-128~127）】");
        /**
         * Integer的valueOf方法会缓存-128到127之间的Integer对象
         * 这是因为小整数使用频率高，缓存可以节省内存、提高性能
         * 所以：
         * - 在-128~127范围内，valueOf返回的是缓存中的同一个对象
         * - 超出范围，valueOf返回的是新创建的对象
         *
         * 注意：==比较的是对象的引用（内存地址），equals比较的是值
         * 比较包装类的值应该用equals方法，不要用==
         */
        Integer a = 127;   // 缓存范围内，复用缓存对象
        Integer b = 127;   // 同一个缓存对象
        Integer c = 128;   // 超出缓存范围，创建新对象
        Integer d = 128;   // 另一个新对象

        System.out.println("Integer a = 127, b = 127");
        System.out.println("a == b: " + (a == b) + "（同一缓存对象，地址相同）");
        System.out.println("a.equals(b): " + a.equals(b) + "（值相同）");

        System.out.println("Integer c = 128, d = 128");
        System.out.println("c == d: " + (c == d) + "（不同对象，地址不同）");
        System.out.println("c.equals(d): " + c.equals(d) + "（值相同）");
    }

    /**
     * ============================================================
     * 演示Java运算符
     *
     * 【运算符分类】
     * 1. 算术运算符：+、-、*、/、%、++、--
     * 2. 赋值运算符：=、+=、-=、*=、/=、%=
     * 3. 比较运算符：==、!=、>、<、>=、<=
     * 4. 逻辑运算符：&&（短路与）、||（短路或）、!（非）
     * 5. 位运算符：&（按位与）、|（按位或）、^（按位异或）、~（按位取反）、<<、>>、>>>
     * 6. 三元运算符：条件 ? 值1 : 值2
     * ============================================================
     */
    public static void demonstrateOperators() {
        System.out.println("\n--- 3. 运算符 ---");

        // --- 算术运算符 ---
        System.out.println("【算术运算符】");
        int a = 10;
        int b = 3;
        System.out.println("a = " + a + ", b = " + b);
        System.out.println("a + b = " + (a + b));   // 加法
        System.out.println("a - b = " + (a - b));   // 减法
        System.out.println("a * b = " + (a * b));   // 乘法
        System.out.println("a / b = " + (a / b));   // 除法（整数除法，结果也是整数，舍去小数部分）
        System.out.println("a % b = " + (a % b));   // 取模（求余数），10%3=1

        /**
         * 自增运算符 ++ 和 自减运算符 --
         * 前缀式（++a）：先自增，再使用值
         * 后缀式（a++）：先使用值，再自增
         */
        int x = 5;
        System.out.println("\nx = " + x);
        System.out.println("x++ = " + (x++) + "，执行后x = " + x); // 先返回值再加1
        System.out.println("++x = " + (++x) + "，执行后x = " + x); // 先加1再返回值
        System.out.println("x-- = " + (x--) + "，执行后x = " + x); // 先返回值再减1
        System.out.println("--x = " + (--x) + "，执行后x = " + x); // 先减1再返回值

        // --- 赋值运算符 ---
        System.out.println("\n【赋值运算符】");
        int c = 10;                 // 简单赋值：把右边的值赋给左边的变量
        c += 5;                     // 等价于 c = c + 5; （复合赋值运算符会自动强制类型转换）
        System.out.println("c += 5 → c = " + c);
        c -= 3;                     // 等价于 c = c - 3;
        System.out.println("c -= 3 → c = " + c);
        c *= 2;                     // 等价于 c = c * 2;
        System.out.println("c *= 2 → c = " + c);
        c /= 4;                     // 等价于 c = c / 4;
        System.out.println("c /= 4 → c = " + c);
        c %= 5;                     // 等价于 c = c % 5;
        System.out.println("c %= 5 → c = " + c);

        // --- 比较运算符 ---
        System.out.println("\n【比较运算符】");
        int m = 10, n = 20;
        System.out.println("m = " + m + ", n = " + n);
        System.out.println("m == n: " + (m == n));   // 等于
        System.out.println("m != n: " + (m != n));   // 不等于
        System.out.println("m > n: " + (m > n));     // 大于
        System.out.println("m < n: " + (m < n));     // 小于
        System.out.println("m >= n: " + (m >= n));   // 大于等于
        System.out.println("m <= n: " + (m <= n));   // 小于等于

        // --- 逻辑运算符 ---
        System.out.println("\n【逻辑运算符】");
        boolean p = true;
        boolean q = false;
        System.out.println("p = " + p + ", q = " + q);
        System.out.println("p && q: " + (p && q));   // 短路与：两边都为true结果才为true，左边为false则右边不执行
        System.out.println("p || q: " + (p || q));   // 短路或：有一边为true结果就为true，左边为true则右边不执行
        System.out.println("!p: " + (!p));           // 逻辑非：取反

        /**
         * 短路特性演示
         * &&：左边为false时，右边不会执行（因为结果已经确定是false）
         * ||：左边为true时，右边不会执行（因为结果已经确定是true）
         * 好处：提高效率，避免不必要的计算
         * 注意：&和|也可以做逻辑运算，但它们不短路（两边都会执行），一般用于位运算
         */
        System.out.println("\n【短路特性演示】");
        int i = 5;
        boolean result1 = (i > 10) && (++i > 5);  // 左边为false，右边++i不执行
        System.out.println("(i>10) && (++i>5) → result=" + result1 + ", i=" + i + "（i没有增加，因为短路了）");
        i = 5;
        boolean result2 = (i < 10) || (++i > 5);  // 左边为true，右边++i不执行
        System.out.println("(i<10) || (++i>5) → result=" + result2 + ", i=" + i + "（i没有增加，因为短路了）");

        // --- 位运算符 ---
        System.out.println("\n【位运算符】");
        int bit1 = 6;  // 二进制: 0110
        int bit2 = 3;  // 二进制: 0011
        System.out.println("bit1 = " + bit1 + " (二进制: 0110)");
        System.out.println("bit2 = " + bit2 + " (二进制: 0011)");
        System.out.println("bit1 & bit2 = " + (bit1 & bit2) + " (按位与: 0010=2)");  // 两位都是1结果才是1
        System.out.println("bit1 | bit2 = " + (bit1 | bit2) + " (按位或: 0111=7)");  // 有一位是1结果就是1
        System.out.println("bit1 ^ bit2 = " + (bit1 ^ bit2) + " (按位异或: 0101=5)"); // 两位不同结果才是1
        System.out.println("~bit1 = " + (~bit1) + " (按位取反)");                    // 所有位取反
        System.out.println("bit1 << 1 = " + (bit1 << 1) + " (左移1位，相当于*2)");   // 左移，右边补0
        System.out.println("bit1 >> 1 = " + (bit1 >> 1) + " (右移1位，相当于/2)");   // 右移，左边补符号位
        System.out.println("bit1 >>> 1 = " + (bit1 >>> 1) + " (无符号右移1位)");     // 无符号右移，左边补0

        // --- 三元运算符 ---
        System.out.println("\n【三元运算符】");
        /**
         * 三元运算符（三目运算符）：简化的if-else
         * 语法：条件表达式 ? 值1 : 值2
         * 执行逻辑：如果条件为true，返回值1；否则返回值2
         * 好处：比if-else更简洁，特别是赋值场景
         */
        int score = 85;
        String grade = score >= 60 ? "及格" : "不及格";
        System.out.println("score = " + score + " → " + grade);

        int max = (a > b) ? a : b;  // 求两个数的最大值
        System.out.println("a = " + a + ", b = " + b + ", max = " + max);
    }

    /**
     * ============================================================
     * 演示控制流语句
     *
     * 【控制流分类】
     * 1. 顺序结构：代码从上到下依次执行（默认）
     * 2. 分支结构：if-else、switch-case
     * 3. 循环结构：for、while、do-while、增强for
     * 4. 跳转语句：break、continue、return
     * ============================================================
     */
    public static void demonstrateControlFlow() {
        System.out.println("\n--- 4. 控制流 ---");

        // --- if-else 分支语句 ---
        System.out.println("【if-else 分支】");
        int score = 85;
        /**
         * if语句：条件判断，根据条件执行不同的代码块
         * 格式1：if (条件) { 代码 }
         * 格式2：if (条件) { 代码1 } else { 代码2 }
         * 格式3：if (条件1) { 代码1 } else if (条件2) { 代码2 } else { 代码3 }
         */
        if (score >= 90) {
            System.out.println("score = " + score + " → 优秀");
        } else if (score >= 80) {
            System.out.println("score = " + score + " → 良好");
        } else if (score >= 60) {
            System.out.println("score = " + score + " → 及格");
        } else {
            System.out.println("score = " + score + " → 不及格");
        }

        // --- switch-case 分支语句 ---
        System.out.println("\n【switch-case 分支】");
        /**
         * switch语句：多分支选择，适用于一个变量有多个固定取值的情况
         * 支持的类型：byte, short, int, char, 枚举, String（Java 7+）
         * 注意：不支持long, float, double, boolean
         *
         * case穿透：如果case后面没有break，会继续执行下一个case的代码
         * 这是一个容易出错的地方，通常每个case都要加break
         * 但有时候也可以利用穿透特性（多个case执行相同代码）
         */
        int day = 3;
        switch (day) {
            case 1:                    // 如果day等于1
                System.out.println("星期一");
                break;                 // break跳出switch，避免case穿透
            case 2:
                System.out.println("星期二");
                break;
            case 3:
                System.out.println("星期三");
                break;
            case 4:
                System.out.println("星期四");
                break;
            case 5:
                System.out.println("星期五");
                break;
            case 6:
            case 7:                    // 利用case穿透：6和7都执行下面的代码
                System.out.println("周末");
                break;
            default:                   // 所有case都不匹配时执行（可选，位置可以任意）
                System.out.println("无效的天数");
                break;
        }

        // --- for 循环 ---
        System.out.println("\n【for 循环】");
        /**
         * for循环：最常用的循环，适用于循环次数已知的情况
         * 格式：for (初始化语句; 循环条件; 迭代语句) { 循环体 }
         * 执行流程：
         *   1. 执行初始化语句（只执行一次）
         *   2. 判断循环条件
         *   3. 如果为true，执行循环体
         *   4. 执行迭代语句
         *   5. 回到第2步，直到条件为false
         */
        System.out.print("1到10的和: ");
        int sum = 0;
        for (int i = 1; i <= 10; i++) {  // i从1循环到10
            sum += i;                     // 累加
        }
        System.out.println(sum);

        // --- while 循环 ---
        System.out.println("\n【while 循环】");
        /**
         * while循环：适用于循环次数不确定的情况
         * 格式：while (循环条件) { 循环体 }
         * 特点：先判断条件，再执行循环体（可能一次都不执行）
         */
        System.out.print("10以内的偶数: ");
        int num = 0;
        while (num <= 10) {               // 只要num<=10就继续循环
            if (num % 2 == 0) {
                System.out.print(num + " ");
            }
            num++;                         // 别忘了更新条件变量，否则会死循环
        }
        System.out.println();

        // --- do-while 循环 ---
        System.out.println("\n【do-while 循环】");
        /**
         * do-while循环：和while类似，但先执行再判断
         * 格式：do { 循环体 } while (循环条件);
         * 特点：循环体至少执行一次（因为先执行后判断）
         * 注意：末尾有分号
         */
        int count = 0;
        do {
            System.out.println("第 " + (count + 1) + " 次执行");
            count++;
        } while (count < 3);  // 注意分号

        // --- 增强for循环（for-each） ---
        System.out.println("\n【增强for循环（for-each）】");
        /**
         * 增强for循环：Java 5引入，用于遍历数组或集合
         * 格式：for (元素类型 变量名 : 数组/集合) { 循环体 }
         * 优点：代码简洁，不会出现下标越界
         * 缺点：无法获取下标，不能在遍历中删除元素
         */
        int[] numbers = {10, 20, 30, 40, 50};  // 定义一个数组
        System.out.print("遍历数组: ");
        for (int n : numbers) {                 // 依次取出数组中的每个元素赋值给n
            System.out.print(n + " ");
        }
        System.out.println();

        // --- break 和 continue ---
        System.out.println("\n【break 和 continue】");
        /**
         * break：跳出当前循环（或switch），结束整个循环
         * continue：跳过本次循环的剩余语句，继续下一次循环
         */
        System.out.print("输出1-10，遇到5跳过: ");
        for (int i = 1; i <= 10; i++) {
            if (i == 5) {
                continue;  // 跳过本次循环，继续下一次（5不输出）
            }
            System.out.print(i + " ");
        }
        System.out.println();

        System.out.print("输出1-10，遇到7停止: ");
        for (int i = 1; i <= 10; i++) {
            if (i == 7) {
                break;     // 跳出整个循环（7及以后都不输出）
            }
            System.out.print(i + " ");
        }
        System.out.println();

        // --- 嵌套循环与带标签的break ---
        System.out.println("\n【嵌套循环（打印乘法表部分）】");
        /**
         * 嵌套循环：循环里面还有循环
         * 外层循环执行一次，内层循环执行完整的一轮
         * 带标签的break：可以跳出指定的外层循环（类似goto，但只能跳出循环）
         */
        outer:  // 定义一个标签，命名为outer
        for (int i = 1; i <= 5; i++) {           // 外层循环控制行
            for (int j = 1; j <= 5; j++) {       // 内层循环控制列
                if (i * j > 10) {
                    break outer;  // 跳出outer标签标识的外层循环
                }
                System.out.print(j + "×" + i + "=" + (i * j) + "\t");
            }
            System.out.println();  // 换行
        }
        System.out.println("\n（当乘积超过10时，用带标签的break跳出了外层循环）");
    }

    /**
     * ============================================================
     * 演示方法定义、方法重载、可变参数
     *
     * 【方法是什么】
     * 方法是一段完成特定功能的代码块，可以重复调用
     * 好处：代码复用、模块化、提高可读性和可维护性
     *
     * 【方法定义格式】
     * 修饰符 返回值类型 方法名(参数列表) {
     *     方法体
     *     return 返回值;
     * }
     *
     * 【方法重载（Overload）】
     * 在同一个类中，方法名相同，但参数列表不同（参数个数、类型、顺序不同）
     * 注意：重载和返回值类型无关，只看方法名和参数列表
     * 好处：功能相似的方法用同一个名字，调用方便，提高可读性
     *
     * 【可变参数（Varargs）】
     * Java 5引入，可以接收任意个数的同类型参数
     * 语法：类型... 参数名
     * 本质：就是一个数组，在方法内部可以当作数组来使用
     * 注意：一个方法最多只能有一个可变参数，且必须是最后一个参数
     * ============================================================
     */
    public static void demonstrateMethods() {
        System.out.println("\n--- 5. 方法定义、重载、可变参数 ---");

        // --- 调用无参无返回值方法 ---
        System.out.println("【无参无返回值方法】");
        printHello();  // 调用printHello方法，输出问候语

        // --- 调用有参有返回值方法 ---
        System.out.println("\n【有参有返回值方法】");
        int result = add(10, 20);  // 调用add方法，传入两个参数，接收返回值
        System.out.println("add(10, 20) = " + result);

        // --- 方法重载演示 ---
        System.out.println("\n【方法重载】");
        // 同样是add方法，但参数不同，编译器会根据参数自动匹配对应的方法
        System.out.println("add(10, 20) = " + add(10, 20));          // 调用int版本
        System.out.println("add(1.5, 2.5) = " + add(1.5, 2.5));      // 调用double版本
        System.out.println("add(1, 2, 3) = " + add(1, 2, 3));        // 调用三个int版本

        // --- 可变参数演示 ---
        System.out.println("\n【可变参数】");
        // 可变参数可以传任意个数的参数（包括0个）
        System.out.println("sum(1) = " + sum(1));
        System.out.println("sum(1, 2, 3) = " + sum(1, 2, 3));
        System.out.println("sum(1, 2, 3, 4, 5) = " + sum(1, 2, 3, 4, 5));
        System.out.println("sum() = " + sum() + "（不传参数也可以）");

        // 也可以直接传数组
        int[] arr = {10, 20, 30};
        System.out.println("sum(new int[]{10,20,30}) = " + sum(arr));
    }

    /**
     * 无参无返回值方法：打印问候语
     *
     * public：访问修饰符，公开的
     * static：静态方法，属于类本身，可以直接通过类名调用
     * void：返回值类型，void表示没有返回值
     * printHello：方法名，小驼峰命名法
     * ()：参数列表为空，表示不需要参数
     */
    public static void printHello() {
        // 方法体：具体执行的代码
        System.out.println("Hello, 这是一个无参无返回值的方法！");
    }

    /**
     * 有参有返回值方法：求两个整数的和
     *
     * @param a 第一个加数
     * @param b 第二个加数
     * @return 两个数的和
     */
    public static int add(int a, int b) {
        // return语句：结束方法，并将结果返回给调用者
        return a + b;
    }

    /**
     * 方法重载：求两个小数的和
     * 方法名相同，但参数类型不同（double而不是int）
     * 这就是方法重载：方法名相同，参数列表不同
     */
    public static double add(double a, double b) {
        return a + b;
    }

    /**
     * 方法重载：求三个整数的和
     * 方法名相同，但参数个数不同（3个而不是2个）
     */
    public static int add(int a, int b, int c) {
        return a + b + c;
    }

    /**
     * 可变参数方法：求任意个整数的和
     *
     * int... nums：表示可以接收任意个int类型的参数
     * nums在方法内部本质上是一个int数组
     *
     * @param nums 可变参数，接收任意个整数
     * @return 所有数的和
     */
    public static int sum(int... nums) {
        int total = 0;                // 初始化总和为0
        for (int num : nums) {        // 遍历所有参数（nums就是一个数组）
            total += num;             // 累加
        }
        return total;                 // 返回总和
    }

    /**
     * ============================================================
     * 演示数组（一维数组、二维数组、Arrays工具类）
     *
     * 【数组是什么】
     * 数组是存储多个相同类型数据的容器，长度一旦确定就不能改变
     * 数组本身是引用类型（对象），存在堆内存中
     * 数组的元素在内存中是连续存储的，通过下标（索引）访问，下标从0开始
     *
     * 【数组的特点】
     * - 长度固定，一旦创建不能改变
     * - 元素类型必须相同
     * - 通过下标访问元素，访问速度快（随机访问O(1)）
     * - 增删元素慢（需要移动元素）
     * ============================================================
     */
    public static void demonstrateArrays() {
        System.out.println("\n--- 6. 数组 ---");

        // --- 一维数组的声明与创建 ---
        System.out.println("【一维数组的声明与创建】");

        // 方式1：声明并分配空间，元素默认初始化为0（int的默认值）
        int[] arr1 = new int[5];           // 创建一个长度为5的int数组
        arr1[0] = 10;                      // 给第1个元素赋值（下标从0开始）
        arr1[1] = 20;                      // 给第2个元素赋值
        arr1[2] = 30;
        System.out.println("arr1的长度: " + arr1.length);   // length属性获取数组长度
        System.out.println("arr1的第1个元素: " + arr1[0]);  // 通过下标访问元素

        // 方式2：声明并初始化（静态初始化）
        int[] arr2 = {1, 2, 3, 4, 5};      // 直接指定元素值，长度由元素个数决定
        System.out.println("arr2: " + Arrays.toString(arr2));  // Arrays.toString打印数组内容

        // 方式3：声明并初始化（动态初始化的完整写法）
        int[] arr3 = new int[]{10, 20, 30}; // 不常用，主要用于匿名数组
        System.out.println("arr3: " + Arrays.toString(arr3));

        // --- 数组的遍历 ---
        System.out.println("\n【数组遍历】");
        int[] nums = {10, 20, 30, 40, 50};

        // 方式1：普通for循环（可以获取下标）
        System.out.print("普通for循环: ");
        for (int i = 0; i < nums.length; i++) {
            System.out.print(nums[i] + " ");
        }
        System.out.println();

        // 方式2：增强for循环（更简洁，但无法获取下标）
        System.out.print("增强for循环: ");
        for (int num : nums) {
            System.out.print(num + " ");
        }
        System.out.println();

        // --- 二维数组 ---
        System.out.println("\n【二维数组】");
        /**
         * 二维数组：数组的数组，即每个元素又是一个一维数组
         * 可以理解为表格（行和列）
         * 二维数组的length是行数，每一行的length是列数
         * 二维数组的每一行长度可以不同（不规则数组）
         */
        // 方式1：规则二维数组（每行长度相同）
        int[][] matrix1 = new int[3][4];   // 3行4列的二维数组

        // 方式2：静态初始化
        int[][] matrix2 = {
                {1, 2, 3},     // 第0行
                {4, 5, 6},     // 第1行
                {7, 8, 9}      // 第2行
        };

        System.out.println("二维数组（3×3）:");
        // 遍历二维数组：外层循环控制行，内层循环控制列
        for (int i = 0; i < matrix2.length; i++) {           // 遍历行
            for (int j = 0; j < matrix2[i].length; j++) {    // 遍历每行的列
                System.out.print(matrix2[i][j] + "\t");      // 打印元素
            }
            System.out.println();  // 换行
        }

        // 不规则二维数组（每行长度不同）
        int[][] irregular = new int[3][];  // 只指定行数，列数后面再指定
        irregular[0] = new int[2];         // 第0行有2个元素
        irregular[1] = new int[3];         // 第1行有3个元素
        irregular[2] = new int[1];         // 第2行有1个元素
        System.out.println("不规则数组的行长度: " + irregular[0].length + ", " + irregular[1].length + ", " + irregular[2].length);

        // --- Arrays工具类 ---
        System.out.println("\n【Arrays工具类常用方法】");
        int[] testArr = {5, 2, 8, 1, 9, 3};

        // toString：将数组转换为字符串（方便打印）
        System.out.println("原数组: " + Arrays.toString(testArr));

        // sort：排序（升序），使用的是优化的快速排序算法
        int[] sortedArr = Arrays.copyOf(testArr, testArr.length);  // 先复制一份
        Arrays.sort(sortedArr);
        System.out.println("排序后: " + Arrays.toString(sortedArr));

        // binarySearch：二分查找（要求数组必须是有序的）
        // 返回值：找到返回下标，没找到返回 -(插入点+1)
        int index = Arrays.binarySearch(sortedArr, 5);
        System.out.println("二分查找5的位置: " + index);

        // copyOf：复制数组，指定新长度（底层用的是System.arraycopy）
        int[] copiedArr = Arrays.copyOf(testArr, 10);  // 复制并扩容到10
        System.out.println("复制并扩容: " + Arrays.toString(copiedArr));

        // fill：填充数组（把数组所有元素设为指定值）
        int[] filledArr = new int[5];
        Arrays.fill(filledArr, 100);  // 所有元素填充为100
        System.out.println("填充数组: " + Arrays.toString(filledArr));

        // equals：比较两个数组的内容是否相同
        int[] arrA = {1, 2, 3};
        int[] arrB = {1, 2, 3};
        System.out.println("arrA.equals(arrB): " + arrA.equals(arrB) + "（==比较的是地址）");
        System.out.println("Arrays.equals(arrA, arrB): " + Arrays.equals(arrA, arrB) + "（比较的是内容）");
    }

    /**
     * ============================================================
     * 演示String、StringBuilder、StringBuffer的区别
     *
     * 【String（不可变字符序列）】
     * - String对象一旦创建，内容就不能改变
     * - 每次"修改"实际上是创建新的String对象
     * - 底层用final修饰的char[]（Java 9+改用byte[]）存储
     * - 字符串常量池：相同字面量的字符串会被复用，节省内存
     * - 适用场景：字符串内容不经常改变的情况
     *
     * 【StringBuilder（可变字符序列，非线程安全）】
     * - 内容可以修改，不会创建新对象
     * - 底层用可变的char[]存储，不够时自动扩容
     * - 非线程安全（没有synchronized），效率高
     * - 适用场景：单线程下频繁拼接字符串
     *
     * 【StringBuffer（可变字符序列，线程安全）】
     * - 和StringBuilder基本一样，但是线程安全的
     * - 大部分方法都加了synchronized同步
     * - 线程安全但效率较低
     * - 适用场景：多线程下频繁拼接字符串
     *
     * 【性能对比】
     * StringBuilder > StringBuffer > String（拼接操作）
     * ============================================================
     */
    public static void demonstrateString() {
        System.out.println("\n--- 7. String / StringBuilder / StringBuffer ---");

        // --- String 常用方法 ---
        System.out.println("【String 常用方法】");
        String str = "Hello World";
        System.out.println("原字符串: " + str);
        System.out.println("长度: " + str.length());                    // 获取字符串长度
        System.out.println("是否为空: " + str.isEmpty());               // 判断是否为空
        System.out.println("第5个字符: " + str.charAt(4));              // 获取指定位置的字符（下标从0开始）
        System.out.println("转大写: " + str.toUpperCase());             // 转大写（返回新字符串，原字符串不变）
        System.out.println("转小写: " + str.toLowerCase());             // 转小写
        System.out.println("是否以Hello开头: " + str.startsWith("Hello"));  // 判断前缀
        System.out.println("是否以ld结尾: " + str.endsWith("ld"));      // 判断后缀
        System.out.println("World的位置: " + str.indexOf("World"));     // 查找子串位置，找不到返回-1
        System.out.println("截取[0,5): " + str.substring(0, 5));        // 截取子串（含头不含尾）
        System.out.println("替换: " + str.replace("World", "Java"));    // 替换（替换所有匹配的）
        System.out.println("去掉首尾空格: '" + "  abc  ".trim() + "'"); // 去掉首尾空白字符

        // 字符串拼接
        String s1 = "Hello";
        String s2 = " " + "World";       // 字符串拼接，+运算符
        System.out.println("字符串拼接: " + (s1 + s2));

        // --- 字符串常量池与 == 比较 ---
        System.out.println("\n【字符串常量池】");
        /**
         * 字符串常量池（String Constant Pool）：
         * Java为了节省内存，相同字面量的字符串会复用同一个对象
         * 用双引号直接写的字符串字面量会放在常量池中
         *
         * == 比较的是对象的内存地址（引用）
         * equals 比较的是字符串的内容
         * 比较字符串内容一定要用equals方法！
         */
        String str1 = "abc";             // 字符串字面量，存在常量池中
        String str2 = "abc";             // 同一个字面量，复用常量池中的对象
        String str3 = new String("abc"); // new出来的，一定是新对象，在堆中
        String str4 = new String("abc"); // 另一个新对象

        System.out.println("str1 = \"abc\", str2 = \"abc\"");
        System.out.println("str1 == str2: " + (str1 == str2) + "（都在常量池，同一个对象）");
        System.out.println("str1.equals(str2): " + str1.equals(str2));

        System.out.println("str3 = new String(\"abc\"), str4 = new String(\"abc\")");
        System.out.println("str3 == str4: " + (str3 == str4) + "（new出来的是不同对象）");
        System.out.println("str3.equals(str4): " + str3.equals(str4));

        // intern方法：将字符串加入常量池（如果池中没有的话）
        String str5 = str3.intern();
        System.out.println("str3.intern() == str1: " + (str5 == str1) + "（intern返回常量池中的对象）");

        // --- StringBuilder 演示 ---
        System.out.println("\n【StringBuilder（可变，非线程安全）】");
        /**
         * StringBuilder：可变的字符串序列，用于高效拼接字符串
         * 底层是char数组，初始容量16，不够时自动扩容（约2倍+2）
         * 常用方法：append, insert, delete, replace, reverse
         */
        StringBuilder sb = new StringBuilder();  // 创建一个空的StringBuilder，初始容量16
        sb.append("Hello");                      // 追加字符串
        sb.append(" ");                          // 追加空格
        sb.append("World");                      // 继续追加
        sb.append("!");
        System.out.println("append拼接: " + sb.toString());  // 转换为String输出

        sb.insert(5, ",");                       // 在指定位置插入
        System.out.println("insert插入: " + sb.toString());

        sb.delete(5, 6);                         // 删除指定范围的字符（含头不含尾）
        System.out.println("delete删除: " + sb.toString());

        sb.replace(0, 5, "Hi");                  // 替换指定范围
        System.out.println("replace替换: " + sb.toString());

        sb.reverse();                            // 反转字符串
        System.out.println("reverse反转: " + sb.toString());

        // --- StringBuffer 演示 ---
        System.out.println("\n【StringBuffer（可变，线程安全）】");
        /**
         * StringBuffer和StringBuilder API几乎一样
         * 区别：StringBuffer的方法都加了synchronized，是线程安全的
         * 因为有同步开销，所以性能比StringBuilder略低
         * 单线程环境推荐用StringBuilder
         */
        StringBuffer sbf = new StringBuffer();
        sbf.append("StringBuffer");
        sbf.append("是线程安全的");
        System.out.println(sbf.toString());
        System.out.println("StringBuffer的方法都加了synchronized，适合多线程环境");

        // --- 性能对比概念说明 ---
        System.out.println("\n【性能对比说明】");
        System.out.println("频繁字符串拼接时：");
        System.out.println("  StringBuilder > StringBuffer > String");
        System.out.println("  原因：String每次拼接都会创建新对象，浪费内存和时间");
        System.out.println("  单线程用StringBuilder，多线程用StringBuffer");
    }
}
