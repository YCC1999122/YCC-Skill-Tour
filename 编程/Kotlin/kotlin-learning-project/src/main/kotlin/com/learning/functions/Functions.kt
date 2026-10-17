// ============================================================
// 包声明
// ============================================================
package com.learning.functions

// ============================================================
// 函数式编程演示
// 包含：函数定义、默认参数、命名参数、单表达式函数、局部函数、
//      高阶函数、Lambda、内联函数、匿名函数、闭包、标准库函数、尾递归
// ============================================================

// ============================================================
// 1. 函数定义、参数默认值、命名参数
// ============================================================

// 基本函数定义：fun 函数名(参数: 类型): 返回类型 { ... }
fun add(a: Int, b: Int): Int {
    return a + b
}

// 带默认值的参数：调用时可以省略有默认值的参数
fun greet(name: String, greeting: String = "你好", punctuation: String = "！"): String {
    return "$greeting，$name$punctuation"
}

// 多个默认参数
fun createUser(
    name: String,
    age: Int = 18,
    email: String = "",
    isActive: Boolean = true
): String {
    return "用户：$name, 年龄：$age, 邮箱：$email, 活跃：$isActive"
}

// ============================================================
// 2. 单表达式函数
// 当函数体只有一个表达式时，可以用 = 号代替花括号和return
// 返回类型可以自动推断
// ============================================================
fun double(x: Int): Int = x * 2

fun triple(x: Int) = x * 3  // 返回类型自动推断为Int

fun isPositive(x: Int) = x > 0  // 返回Boolean

fun getMax(a: Int, b: Int) = if (a > b) a else b

// ============================================================
// 3. 局部函数（嵌套函数）
// 函数内部可以定义函数，局部函数可以访问外部函数的变量
// ============================================================
fun calculateTotal(numbers: List<Int>): Int {
    // 局部函数：只能在calculateTotal内部使用
    fun validateAndAdd(sum: Int, num: Int): Int {
        // 可以访问外部函数的变量（这里是numbers，虽然没用到）
        require(num >= 0) { "不允许负数：$num" }
        return sum + num
    }

    var total = 0
    for (num in numbers) {
        total = validateAndAdd(total, num)
    }
    return total
}

// 局部函数的另一个例子：递归时使用
fun factorial(n: Int): Int {
    // 局部辅助函数
    fun fact(current: Int, accumulator: Int): Int {
        return if (current <= 1) {
            accumulator
        } else {
            fact(current - 1, current * accumulator)
        }
    }
    return fact(n, 1)
}

// ============================================================
// 4. 高阶函数：函数作为参数/返回值
// 高阶函数是接受函数类型作为参数，或返回函数类型的函数
// 函数类型：(参数类型列表) -> 返回类型
// ============================================================

// --- 函数作为参数 ---
// operation是一个函数类型参数：接受两个Int，返回Int
fun calculate(a: Int, b: Int, operation: (Int, Int) -> Int): Int {
    // 调用传入的函数
    return operation(a, b)
}

// --- 函数作为返回值 ---
// 根据操作类型返回对应的函数
fun getOperation(op: String): (Int, Int) -> Int {
    return when (op) {
        "add" -> { a, b -> a + b }
        "subtract" -> { a, b -> a - b }
        "multiply" -> { a, b -> a * b }
        else -> { _, _ -> 0 }
    }
}

// ============================================================
// 5. Lambda表达式
// Lambda是一种匿名函数，可以作为值传递
// 语法：{ 参数列表 -> 函数体 }
// ============================================================

// 定义Lambda变量
val sumLambda: (Int, Int) -> Int = { a: Int, b: Int -> a + b }

// 简化的Lambda（类型推断）
val multiplyLambda = { a: Int, b: Int -> a * b }

// 单参数Lambda可以用it代替参数名
val square: (Int) -> Int = { it * it }

// Lambda可以有多个语句，最后一个表达式是返回值
val printAndDouble: (Int) -> Int = {
    println("处理数字：$it")
    it * 2  // 最后一行是返回值
}

// ============================================================
// 6. 内联函数 inline/noinline/crossinline
// inline：将函数体和Lambda参数内联到调用处，避免函数调用开销和对象创建
// noinline：标记某个函数参数不参与内联
// crossinline：标记Lambda参数不能有非局部返回
// ============================================================

// 内联函数：函数体会被内联到调用处
inline fun <T> timeIt(block: () -> T): T {
    val start = System.nanoTime()
    val result = block()  // 执行传入的Lambda
    val end = System.nanoTime()
    println("执行耗时：${(end - start) / 1000} 微秒")
    return result
}

// 带有多个函数参数的内联函数，使用noinline排除某个参数
inline fun twoOperations(
    first: () -> Unit,
    noinline second: () -> Unit  // second不参与内联
) {
    first()
    second()
}

// crossinline：Lambda中不能有return（非局部返回）
inline fun runWithCrossinline(crossinline block: () -> Unit) {
    // 在内联函数中，如果Lambda在另一个执行上下文中被调用（如对象中），需要crossinline
    val runnable = Runnable {
        block()  // 这里block在Runnable中执行，所以需要crossinline
    }
    runnable.run()
}

// ============================================================
// 7. 匿名函数
// 类似Lambda，但使用fun关键字，更像普通函数
// 主要区别：匿名函数可以指定返回类型，且return是从匿名函数返回（不是外层函数）
// ============================================================

// 匿名函数作为变量
val anonymousSum = fun(a: Int, b: Int): Int {
    return a + b
}

// 匿名函数作为参数传递
val numbersList = listOf(1, 2, 3, 4, 5)
val doubled = numbersList.map(fun(x): Int {
    return x * 2
})

// ============================================================
// 8. 闭包
// Lambda/匿名函数可以访问并修改其外部作用域的变量
// 这些变量会被"捕获"并与函数一起存在
// ============================================================
fun makeCounter(): () -> Int {
    var count = 0  // 这个变量会被闭包捕获

    // 返回的Lambda捕获了count变量
    return {
        count++  // 可以修改外部变量
        count    // 返回当前计数
    }
}

// 另一个闭包例子：累加器
fun makeAdder(initial: Int): (Int) -> Int {
    var sum = initial
    return { value ->
        sum += value
        sum
    }
}

// ============================================================
// 9. 标准库函数：let/also/run/apply/with/repeat
// ============================================================
class ScopeFunctionsDemo {
    data class Person(var name: String, var age: Int, var city: String = "")

    fun demo() {
        val person = Person("张三", 25)

        // let：it引用对象，返回lambda结果
        // 常用于非空检查和转换
        val nameLength = person.name.let {
            println("let：名字是$it")
            it.length  // 返回值
        }
        println("let返回：$nameLength")

        // also：it引用对象，返回对象本身
        // 常用于附加操作（日志、副作用）
        person.also {
            println("also：修改前 - ${it.name}, ${it.age}")
        }.age = 30
        println("also后年龄：${person.age}")

        // run：this引用对象，返回lambda结果
        // 常用于配置对象并计算结果
        val info = person.run {
            city = "北京"
            "姓名：$name，年龄：$age，城市：$city"
        }
        println("run返回：$info")

        // apply：this引用对象，返回对象本身
        // 常用于对象初始化（建造者模式的替代）
        val newPerson = Person("", 0).apply {
            name = "李四"
            age = 28
            city = "上海"
        }
        println("apply创建：$newPerson")

        // with：不是扩展函数，对象作为参数传入
        // 常用于对同一对象进行多次操作
        with(person) {
            println("with：$name, $age, $city")
        }

        // repeat：重复执行指定次数
        print("repeat 3次：")
        repeat(3) { i ->
            print("${i + 1} ")
        }
        println()
    }
}

// ============================================================
// 10. 尾递归 tailrec
// 尾递归函数：递归调用是函数的最后一个操作
// 编译器会将尾递归优化为循环，避免栈溢出
// 使用tailrec关键字标记
// ============================================================

// 尾递归版本的阶乘
tailrec fun factorialTail(n: Int, accumulator: Int = 1): Int {
    // 递归调用必须是最后一个操作（尾调用）
    return if (n <= 1) {
        accumulator  // 基准情况：返回累积值
    } else {
        factorialTail(n - 1, n * accumulator)  // 递归调用在最后
    }
}

// 尾递归版本的斐波那契数列
tailrec fun fibonacci(n: Int, a: Int = 0, b: Int = 1): Int {
    return if (n == 0) {
        a
    } else {
        fibonacci(n - 1, b, a + b)
    }
}

// 尾递归查找元素
tailrec fun <T> findElement(list: List<T>, predicate: (T) -> Boolean, index: Int = 0): T? {
    if (index >= list.size) return null
    val element = list[index]
    return if (predicate(element)) {
        element
    } else {
        findElement(list, predicate, index + 1)  // 尾递归调用
    }
}

// ============================================================
// 主演示函数
// ============================================================
fun runFunctionsDemo() {
    println("============================================================")
    println("                  Kotlin 函数式特性演示")
    println("============================================================")

    // --- 1. 函数定义、默认参数、命名参数 ---
    println("\n=== 1. 函数定义、默认参数、命名参数 ===")
    println("add(3, 5) = ${add(3, 5)}")

    // 使用默认参数
    println(greet("张三"))                        // 使用所有默认值
    println(greet("李四", "早上好"))              // 指定第二个参数
    println(greet("王五", punctuation = "。"))    // 命名参数：只指定第三个

    // 命名参数的优势：可以任意顺序，可以只传部分参数
    println(createUser("赵六"))
    println(createUser("钱七", age = 30))
    println(createUser("孙八", email = "sun@example.com", age = 25))

    // --- 2. 单表达式函数 ---
    println("\n=== 2. 单表达式函数 ===")
    println("double(5) = ${double(5)}")
    println("triple(5) = ${triple(5)}")
    println("isPositive(-3) = ${isPositive(-3)}")
    println("getMax(10, 20) = ${getMax(10, 20)}")

    // --- 3. 局部函数 ---
    println("\n=== 3. 局部函数 ===")
    val nums = listOf(1, 2, 3, 4, 5)
    println("计算总和：${calculateTotal(nums)}")
    println("5的阶乘：${factorial(5)}")

    // --- 4. 高阶函数 ---
    println("\n=== 4. 高阶函数 ===")

    // 函数作为参数
    val sumResult = calculate(10, 5) { a, b -> a + b }
    println("加法：$sumResult")

    val subResult = calculate(10, 5, { a, b -> a - b })
    println("减法：$subResult")

    // 函数作为返回值
    val addOp = getOperation("add")
    val mulOp = getOperation("multiply")
    println("addOp(3, 4) = ${addOp(3, 4)}")
    println("mulOp(3, 4) = ${mulOp(3, 4)}")

    // --- 5. Lambda表达式 ---
    println("\n=== 5. Lambda表达式 ===")
    println("sumLambda(3, 4) = ${sumLambda(3, 4)}")
    println("multiplyLambda(3, 4) = ${multiplyLambda(3, 4)}")
    println("square(5) = ${square(5)}")
    println("printAndDouble(10) = ${printAndDouble(10)}")

    // 集合操作中的Lambda（尾随lambda语法）
    val numbers = listOf(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

    // filter：过滤
    val evens = numbers.filter { it % 2 == 0 }
    println("偶数：$evens")

    // map：转换
    val squares = numbers.map { it * it }
    println("平方：$squares")

    // 链式调用
    val result = numbers
        .filter { it > 5 }
        .map { it * 2 }
        .sum()
    println("大于5的数翻倍后求和：$result")

    // 解构Lambda参数
    val pairs = listOf(1 to "一", 2 to "二", 3 to "三")
    val pairResult = pairs.map { (num, str) -> "$num:$str" }
    println("解构：$pairResult")

    // --- 6. 内联函数 ---
    println("\n=== 6. 内联函数 ===")
    val timeResult = timeIt {
        // 这里的代码会被内联
        var sum = 0
        for (i in 1..10000) {
            sum += i
        }
        sum
    }
    println("计算结果：$timeResult")

    // crossinline演示
    println("crossinline演示：")
    runWithCrossinline {
        println("在Runnable中执行的代码")
    }

    // --- 7. 匿名函数 ---
    println("\n=== 7. 匿名函数 ===")
    println("anonymousSum(3, 4) = ${anonymousSum(3, 4)}")
    println("doubled: $doubled")

    // 匿名函数与Lambda的区别：return的行为不同
    val list = listOf(1, 2, 3, 4, 5)
    list.forEach(fun(x) {
        // 这里的return只从匿名函数返回，不会跳出forEach
        if (x == 3) return
        print("$x ")
    })
    println()

    // --- 8. 闭包 ---
    println("\n=== 8. 闭包 ===")
    val counter1 = makeCounter()
    val counter2 = makeCounter()

    println("counter1第一次：${counter1()}")  // 1
    println("counter1第二次：${counter1()}")  // 2
    println("counter1第三次：${counter1()}")  // 3

    println("counter2第一次：${counter2()}")  // 1（独立的计数器）
    println("counter1第四次：${counter1()}")  // 4

    val adder = makeAdder(10)
    println("adder(5) = ${adder(5)}")    // 15
    println("adder(3) = ${adder(3)}")    // 18

    // --- 9. 标准库作用域函数 ---
    println("\n=== 9. 标准库作用域函数 ===")
    ScopeFunctionsDemo().demo()

    // --- 10. 尾递归 ---
    println("\n=== 10. 尾递归 tailrec ===")
    println("5的阶乘（尾递归）：${factorialTail(5)}")
    println("10的阶乘（尾递归）：${factorialTail(10)}")
    println("斐波那契第10项：${fibonacci(10)}")

    val findList = listOf("apple", "banana", "cherry", "date")
    val found = findElement(findList) { it.startsWith("c") }
    println("找到以c开头的元素：$found")

    println("\n============================================================")
    println("                  函数式特性演示结束")
    println("============================================================")
}
