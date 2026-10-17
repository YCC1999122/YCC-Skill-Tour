// ============================================================
// 包声明：指定当前文件所属的包
// 包名通常与目录结构一致，用点分隔
// ============================================================
package com.learning.basics

// ============================================================
// 顶层常量：使用const关键字定义编译时常量
// const只能修饰顶层属性或object的属性，且必须是基本类型或String
// ============================================================
const val APP_NAME = "Kotlin学习项目"  // 编译时常量：在编译时就确定值
const val APP_VERSION = "1.0.0"        // 版本号常量

// ============================================================
// 基础语法演示类
// 这个类包含Kotlin基础语法的所有知识点
// ============================================================
class Basics {

    // ============================================================
    // 变量声明：val vs var
    // val (value)：只读变量，赋值后不可修改（类似Java的final）
    // var (variable)：可变变量，可以重新赋值
    // ============================================================
    fun variableDemo() {
        println("=== 变量演示 ===")

        // val：只读变量（不可重新赋值）
        val immutableName: String = "张三"   // 显式类型声明
        // immutableName = "李四"  // 错误：val不能重新赋值

        // var：可变变量（可以重新赋值）
        var mutableAge: Int = 25             // 显式类型声明
        mutableAge = 26                      // 正确：var可以重新赋值
        println("姓名：$immutableName，年龄：$mutableAge")

        // 类型推断：Kotlin可以自动推断变量类型
        val inferredString = "自动推断为String"  // 编译器推断为String类型
        var inferredInt = 100                   // 编译器推断为Int类型
        println("类型推断：$inferredString, $inferredInt")

        // 延迟初始化：lateinit用于延迟初始化var变量
        // 只能用于var，且不能是基本类型，不能有自定义getter/setter
        // lateinit var lateVar: Int  // 错误：lateinit不能用于基本类型
    }

    // ============================================================
    // 基本数据类型
    // Kotlin中一切皆对象，没有像Java那样的原始类型
    // 但编译器会在底层优化为原始类型以提高性能
    // ============================================================
    fun dataTypeDemo() {
        println("\n=== 基本数据类型演示 ===")

        // --- 整数类型 ---
        val byteVal: Byte = 127               // 字节：8位，范围 -128~127
        val shortVal: Short = 32767           // 短整型：16位，范围 -32768~32767
        val intVal: Int = 2147483647          // 整型：32位（默认整数类型）
        val longVal: Long = 9223372036854775807L  // 长整型：64位，后缀L
        println("整数类型：Byte=$byteVal, Short=$shortVal, Int=$intVal, Long=$longVal")

        // --- 浮点类型 ---
        val floatVal: Float = 3.14F           // 单精度浮点数：32位，后缀F/f
        val doubleVal: Double = 3.1415926535  // 双精度浮点数：64位（默认浮点类型）
        println("浮点类型：Float=$floatVal, Double=$doubleVal")

        // --- 布尔类型 ---
        val isTrue: Boolean = true            // 布尔值：true/false
        val isFalse: Boolean = false
        println("布尔类型：isTrue=$isTrue, isFalse=$isFalse")

        // --- 字符类型 ---
        val charVal: Char = 'A'               // 字符：16位，用单引号
        val unicodeChar: Char = '\u4E2D'      // Unicode字符：'中'
        println("字符类型：charVal=$charVal, unicodeChar=$unicodeChar")

        // --- 字符串类型 ---
        val strVal: String = "Hello Kotlin"   // 字符串：用双引号
        println("字符串类型：$strVal")

        // --- 数字类型转换 ---
        val intNum = 100
        val longNum: Long = intNum.toLong()   // 显式转换为Long
        val doubleNum: Double = intNum.toDouble()  // 显式转换为Double
        println("类型转换：Int($intNum) -> Long($longNum) -> Double($doubleNum)")

        // 注意：Kotlin不支持隐式的小范围到大范围转换（如Int自动转Long）
        // val longNum2: Long = intNum  // 错误：类型不匹配
    }

    // ============================================================
    // 字符串模板和原始字符串
    // ============================================================
    fun stringDemo() {
        println("\n=== 字符串演示 ===")

        // 字符串模板：使用$符号引用变量
        val name = "小明"
        val age = 20
        val greeting = "大家好，我叫$name，今年${age}岁"  // ${}中可以放表达式
        println("字符串模板：$greeting")

        // 模板中的表达式
        val a = 10
        val b = 20
        println("a + b = ${a + b}")  // 表达式计算结果会被插入字符串

        // 原始字符串：使用三个双引号，可以包含换行和任意字符
        val rawString = """
            这是原始字符串
            可以包含换行符
            不需要转义
            格式完全保留
        """.trimIndent()  // trimIndent()用于去除公共前导空白
        println("原始字符串：")
        println(rawString)

        // 字符串的常见操作
        val str = "Hello World"
        println("字符串长度：${str.length}")           // 长度
        println("转大写：${str.uppercase()}")          // 转大写
        println("转小写：${str.lowercase()}")          // 转小写
        println("是否以Hello开头：${str.startsWith("Hello")}")  // 开头判断
        println("截取子串：${str.substring(0, 5)}")    // 截取子串
        println("替换：${str.replace("World", "Kotlin")}")  // 替换
    }

    // ============================================================
    // 数组
    // Kotlin数组用Array类表示，也有专门的基本类型数组
    // ============================================================
    fun arrayDemo() {
        println("\n=== 数组演示 ===")

        // 使用arrayOf创建数组（元素类型可以任意）
        val mixedArray = arrayOf(1, "two", 3.0, true)  // 混合类型数组，类型推断为Array<Any>
        println("混合数组：${mixedArray.contentToString()}")

        // 指定类型的数组
        val stringArray: Array<String> = arrayOf("苹果", "香蕉", "橙子")
        println("字符串数组：${stringArray.contentToString()}")

        // 基本类型数组：避免装箱开销，性能更好
        val intArray = intArrayOf(1, 2, 3, 4, 5)       // IntArray
        val longArray = longArrayOf(1L, 2L, 3L)        // LongArray
        val doubleArray = doubleArrayOf(1.1, 2.2, 3.3) // DoubleArray
        val charArray = charArrayOf('a', 'b', 'c')     // CharArray
        println("Int数组：${intArray.contentToString()}")
        println("Long数组：${longArray.contentToString()}")

        // 使用Array构造函数创建数组
        // 第一个参数是大小，第二个参数是初始化函数（根据索引生成元素）
        val squaredArray = Array(5) { index -> index * index }  // 索引从0开始
        println("构造函数创建的数组（平方数）：${squaredArray.contentToString()}")

        // 数组访问和修改
        val numbers = intArrayOf(10, 20, 30)
        numbers[0] = 100                    // 使用[]修改元素
        println("修改后的数组：numbers[0]=${numbers[0]}")  // 使用[]访问元素
        println("数组大小：${numbers.size}")
    }

    // ============================================================
    // 运算符
    // ============================================================
    fun operatorDemo() {
        println("\n=== 运算符演示 ===")

        val a = 10
        val b = 3

        // 算术运算符
        println("算术运算：")
        println("a + b = ${a + b}")    // 加法
        println("a - b = ${a - b}")    // 减法
        println("a * b = ${a * b}")    // 乘法
        println("a / b = ${a / b}")    // 除法（整数除法）
        println("a % b = ${a % b}")    // 取余
        println("a++ = ${a.inc()}")    // 自增（使用inc()函数，对应++运算符）

        // 比较运算符
        println("\n比较运算：")
        println("a > b = ${a > b}")    // 大于
        println("a < b = ${a < b}")    // 小于
        println("a >= b = ${a >= b}")  // 大于等于
        println("a <= b = ${a <= b}")  // 小于等于
        println("a == b = ${a == b}")  // 等于（比较内容，类似Java的equals）
        println("a != b = ${a != b}")  // 不等于
        // === 比较引用地址（是否是同一个对象）

        // 逻辑运算符
        val x = true
        val y = false
        println("\n逻辑运算：")
        println("x && y = ${x && y}")  // 逻辑与（短路）
        println("x || y = ${x || y}")  // 逻辑或（短路）
        println("!x = ${!x}")          // 逻辑非

        // 赋值运算符
        var c = 10
        c += 5  // 等价于 c = c + 5
        println("\n赋值运算：c += 5 后 c = $c")
    }

    // ============================================================
    // 控制流：if表达式
    // Kotlin中if是表达式，有返回值，可以赋值给变量
    // ============================================================
    fun ifExpressionDemo() {
        println("\n=== if表达式演示 ===")

        // 传统if语句
        val score = 85
        if (score >= 60) {
            println("及格了")
        } else {
            println("没及格")
        }

        // if作为表达式（有返回值）
        val result = if (score >= 60) "及格" else "不及格"
        println("if表达式结果：$result")

        // 多分支if表达式
        val grade = if (score >= 90) {
            "优秀"
        } else if (score >= 80) {
            "良好"
        } else if (score >= 60) {
            "及格"
        } else {
            "不及格"
        }
        println("成绩等级：$grade")
    }

    // ============================================================
    // 控制流：when表达式
    // when是Kotlin中的"增强版switch"，支持模式匹配
    // ============================================================
    fun whenExpressionDemo() {
        println("\n=== when表达式演示 ===")

        val x = 3

        // 基本when表达式（类似switch）
        when (x) {
            1 -> println("x是1")        // 匹配常量值
            2 -> println("x是2")
            3, 4 -> println("x是3或4")  // 多个值匹配同一个分支
            else -> println("x是其他值") // 默认分支（类似default）
        }

        // when作为表达式（有返回值）
        val result = when (x) {
            1 -> "一"
            2 -> "二"
            3 -> "三"
            else -> "其他"
        }
        println("when表达式结果：$result")

        // 类型判断（智能类型转换）
        val anyValue: Any = "Hello"
        val typeInfo = when (anyValue) {
            is String -> "这是字符串，长度为${anyValue.length}"  // is判断类型，智能转换为String
            is Int -> "这是整数，值为$anyValue"                   // 智能转换为Int
            is Double -> "这是双精度浮点数"
            else -> "未知类型"
        }
        println("类型判断：$typeInfo")

        // 范围判断
        val score = 75
        val level = when (score) {
            in 90..100 -> "优秀"   // in判断是否在范围内
            in 80 until 90 -> "良好"  // until不包含右端点
            in 60 until 80 -> "及格"
            !in 0..100 -> "无效分数" // !in判断是否不在范围内
            else -> "不及格"
        }
        println("分数等级：$level")

        // 不带参数的when（相当于if-else链）
        val temp = 28
        when {
            temp > 30 -> println("天气很热")
            temp > 20 -> println("天气暖和")
            temp > 10 -> println("天气凉爽")
            else -> println("天气寒冷")
        }
    }

    // ============================================================
    // 控制流：for循环
    // ============================================================
    fun forLoopDemo() {
        println("\n=== for循环演示 ===")

        // 遍历数组
        val fruits = arrayOf("苹果", "香蕉", "橙子", "葡萄")
        print("遍历数组：")
        for (fruit in fruits) {
            print("$fruit ")
        }
        println()

        // 遍历范围（range）
        print("遍历1到5：")
        for (i in 1..5) {      // 1..5 表示从1到5（包含5）
            print("$i ")
        }
        println()

        // 倒序遍历
        print("倒序遍历5到1：")
        for (i in 5 downTo 1) {  // downTo：从大到小
            print("$i ")
        }
        println()

        // 指定步长
        print("步长为2遍历1到10：")
        for (i in 1..10 step 2) {  // step：步长
            print("$i ")
        }
        println()

        // until：不包含右端点
        print("until遍历1到5（不包含5）：")
        for (i in 1 until 5) {
            print("$i ")
        }
        println()

        // 带索引遍历（使用withIndex）
        print("带索引遍历：")
        for ((index, value) in fruits.withIndex()) {
            print("[$index:$value] ")
        }
        println()

        // 遍历字符串
        print("遍历字符串：")
        for (char in "Kotlin") {
            print("$char ")
        }
        println()
    }

    // ============================================================
    // 控制流：while和do-while循环
    // ============================================================
    fun whileLoopDemo() {
        println("\n=== while循环演示 ===")

        // while循环：先判断条件，再执行循环体
        println("while循环：")
        var i = 1
        while (i <= 5) {
            print("$i ")
            i++
        }
        println()

        // do-while循环：先执行一次循环体，再判断条件
        // 循环体至少执行一次
        println("do-while循环：")
        var j = 1
        do {
            print("$j ")
            j++
        } while (j <= 5)
        println()
    }

    // ============================================================
    // 范围与数列
    // ============================================================
    fun rangeDemo() {
        println("\n=== 范围与数列演示 ===")

        // 闭区间：[1, 10]，包含两端
        val range1 = 1..10
        println("范围1..10：${range1.toList()}")

        // 半开区间：[1, 10)，不包含右端点
        val range2 = 1 until 10
        println("范围1 until 10：${range2.toList()}")

        // 倒序范围
        val range3 = 10 downTo 1
        println("倒序10 downTo 1：${range3.toList()}")

        // 带步长的范围
        val range4 = 1..10 step 2
        println("步长2：${range4.toList()}")

        // 字符范围
        val charRange = 'a'..'e'
        println("字符范围a..e：${charRange.toList()}")

        // in操作符：判断是否在范围内
        val num = 5
        println("5是否在1..10中：${num in 1..10}")
        println("15是否在1..10中：${15 in 1..10}")
    }

    // ============================================================
    // 空安全（Kotlin的核心特性之一）
    // Kotlin通过类型系统区分可空类型和非空类型，从根源上避免空指针异常
    // ============================================================
    fun nullSafetyDemo() {
        println("\n=== 空安全演示 ===")

        // 非空类型：不能赋值为null（编译时检查）
        var nonNullStr: String = "Hello"
        // nonNullStr = null  // 错误：非空类型不能为null
        println("非空字符串：$nonNullStr，长度：${nonNullStr.length}")

        // 可空类型：在类型后面加?，表示可以为null
        var nullableStr: String? = "World"
        nullableStr = null  // 正确：可空类型可以为null
        println("可空字符串：$nullableStr")

        // 安全调用运算符 ?.
        // 如果对象不为null则调用方法/属性，否则返回null
        val len1 = nullableStr?.length  // nullableStr为null，返回null
        println("安全调用结果：$len1")

        nullableStr = "Hello"
        val len2 = nullableStr?.length  // nullableStr不为null，返回长度
        println("安全调用结果（非空时）：$len2")

        // Elvis运算符 ?:
        // 如果左边表达式不为null则返回左边值，否则返回右边值
        // 类似于Java中的三目运算符：x != null ? x : default
        val name: String? = null
        val displayName = name ?: "匿名用户"  // name为null，使用默认值
        println("Elvis运算符结果：$displayName")

        // 非空断言运算符 !!
        // 告诉编译器"我确定这个值不为null"，如果为null则抛出NPE
        // 慎用！除非你确定值不为null
        val definitelyNotNull: String? = "一定不为空"
        val len3 = definitelyNotNull!!.length  // 强制断言非空
        println("非空断言结果：$len3")

        // let函数：配合?.使用，只在非空时执行代码块
        val maybeNull: String? = "测试let"
        maybeNull?.let {
            // it就是非空的字符串
            println("let函数执行：字符串'$it'的长度是${it.length}")
        }

        // let也可以用在可空对象上，非空才执行
        val nullValue: String? = null
        nullValue?.let {
            println("这行不会执行，因为nullValue是null")
        }

        // also函数：与let类似，但返回对象本身（而不是lambda结果）
        val result = maybeNull?.also {
            println("also函数执行：$it")
        }
        println("also返回值：$result")  // 返回的是maybeNull本身
    }

    // ============================================================
    // 智能类型转换（Smart Cast）
    // Kotlin编译器会自动进行类型转换，无需显式强制转换
    // ============================================================
    fun smartCastDemo() {
        println("\n=== 智能类型转换演示 ===")

        // 定义一个Any类型的变量（Any类似Java的Object）
        val value: Any = "Hello Kotlin"

        // 使用is运算符判断类型
        if (value is String) {
            // 编译器自动将value转换为String类型（智能转换）
            // 不需要像Java那样强制转换 (String)value
            println("这是字符串，长度为：${value.length}")
            println("转大写：${value.uppercase()}")
        }

        // 使用!is判断不是某类型
        val num: Any = 123
        if (num !is String) {
            println("这不是字符串")
        }

        // 在when表达式中的智能转换
        fun getTypeInfo(obj: Any): String {
            return when (obj) {
                is Int -> "整数，值为$obj"        // 智能转换为Int
                is String -> "字符串，长度${obj.length}"  // 智能转换为String
                is Double -> "双精度浮点数"        // 智能转换为Double
                is IntArray -> "整型数组，大小${obj.size}"  // 智能转换为IntArray
                else -> "未知类型"
            }
        }
        println(getTypeInfo(42))
        println(getTypeInfo("Kotlin"))
        println(getTypeInfo(3.14))
        println(getTypeInfo(intArrayOf(1, 2, 3)))

        // 注意：智能转换只适用于不可变的val变量
        // 因为var变量可能在判断后被其他线程修改
        var mutableValue: Any = "可变字符串"
        if (mutableValue is String) {
            // 对于var，不能直接使用智能转换（因为可能在判断后被修改）
            // 需要显式转换或使用本地变量
            // println(mutableValue.length)  // 可能无法智能转换
            val localStr = mutableValue  // 赋值给本地val变量
            println("本地变量智能转换：${localStr.length}")
        }
    }

    // ============================================================
    // 标准库函数：let/also/run/apply/with
    // 这些是Kotlin中非常常用的作用域函数
    // ============================================================
    fun scopeFunctionDemo() {
        println("\n=== 作用域函数演示 ===")

        data class Person(var name: String, var age: Int)

        // --- let函数 ---
        // 特点：it引用对象，返回lambda结果
        // 用途：非空执行代码块、转换结果
        val name: String? = "张三"
        val greeting = name?.let {
            "你好，${it}！"  // it是非空的name
        }
        println("let结果：$greeting")

        // --- also函数 ---
        // 特点：it引用对象，返回对象本身
        // 用途：附加操作（日志、副作用）
        val numbers = mutableListOf(1, 2, 3)
        numbers.also {
            println("also：添加元素前列表是$it")
        }.add(4)
        println("also：添加后列表是$numbers")

        // --- run函数 ---
        // 特点：this引用对象，返回lambda结果
        // 用途：对象配置并计算结果
        val person1 = Person("李四", 30)
        val info = person1.run {
            age += 1  // this可以省略
            "姓名：$name，年龄：$age"
        }
        println("run结果：$info")

        // --- apply函数 ---
        // 特点：this引用对象，返回对象本身
        // 用途：对象初始化/配置（建造者模式的替代）
        val person2 = Person("", 0).apply {
            name = "王五"
            age = 25
        }
        println("apply结果：$person2")

        // --- with函数 ---
        // 特点：不是扩展函数，将对象作为参数传入，this引用对象，返回lambda结果
        // 用途：对同一个对象进行多次操作
        val person3 = Person("赵六", 28)
        val description = with(person3) {
            "姓名：$name，年龄：$age岁，${if (age >= 18) "成年" else "未成年"}"
        }
        println("with结果：$description")

        // --- repeat函数 ---
        // 重复执行指定次数
        print("repeat执行3次：")
        repeat(3) { index ->
            print("第${index + 1}次 ")
        }
        println()
    }
}

// ============================================================
// 顶层函数：不属于任何类的函数
// 这个函数用于运行所有基础语法演示
// ============================================================
fun runBasicsDemo() {
    println("============================================================")
    println("                    Kotlin 基础语法演示")
    println("============================================================")

    // 创建Basics类的实例
    val basics = Basics()

    // 依次调用各个演示函数
    basics.variableDemo()        // 变量演示
    basics.dataTypeDemo()        // 数据类型演示
    basics.stringDemo()          // 字符串演示
    basics.arrayDemo()           // 数组演示
    basics.operatorDemo()        // 运算符演示
    basics.ifExpressionDemo()    // if表达式演示
    basics.whenExpressionDemo()  // when表达式演示
    basics.forLoopDemo()         // for循环演示
    basics.whileLoopDemo()       // while循环演示
    basics.rangeDemo()           // 范围与数列演示
    basics.nullSafetyDemo()      // 空安全演示
    basics.smartCastDemo()       // 智能类型转换演示
    basics.scopeFunctionDemo()   // 作用域函数演示

    println("\n============================================================")
    println("                  基础语法演示结束")
    println("============================================================")
}
