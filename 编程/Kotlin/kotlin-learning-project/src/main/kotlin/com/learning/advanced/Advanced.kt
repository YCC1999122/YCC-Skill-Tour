// ============================================================
// 包声明
// ============================================================
package com.learning.advanced

// 反射需要的导入
import kotlin.reflect.KClass
import kotlin.reflect.KProperty
import kotlin.reflect.full.*

// ============================================================
// 高级特性演示
// 包含：泛型、注解、反射、委托、类型别名、内联类、
//      解构声明、异常处理、操作符重载详解、DSL构建器
// ============================================================

// ============================================================
// 1. 泛型
// ============================================================

// --- 泛型类 ---
// T是类型参数，可以是任意标识符
class Box<T>(private val item: T) {
    // 获取内容
    fun getItem(): T = item

    // 打印内容
    fun printItem() {
        println("Box中的内容：$item")
    }
}

// --- 泛型函数 ---
fun <T> singletonList(item: T): List<T> {
    return listOf(item)
}

// 多个类型参数
fun <T, R> transformAndWrap(item: T, transform: (T) -> R): Box<R> {
    return Box(transform(item))
}

// --- 泛型约束（上界） ---
// 使用 : 类型 来指定类型参数的上界
// 只有Number及其子类可以作为类型参数
fun <T : Number> sumOfList(list: List<T>): Double {
    var sum = 0.0
    for (item in list) {
        sum += item.toDouble()
    }
    return sum
}

// 多个上界约束：使用where子句
interface Nameable {
    fun getName(): String
}

interface Identifiable {
    fun getId(): Int
}

// T必须同时实现Nameable和Identifiable
fun <T> printEntity(entity: T) where T : Nameable, T : Identifiable {
    println("实体：id=${entity.getId()}, name=${entity.getName()}")
}

// --- 协变（out）---
// out：生产者，只产出（返回）T，不消费（接收）T
// 即：方法可以返回T，但不能把T作为参数类型
interface Producer<out T> {
    fun produce(): T  // 正确：T在返回位置（协变点）
    // fun consume(item: T)  // 错误：T在参数位置（逆变点）
}

// 协变的应用：String是Any的子类，所以Producer<String>是Producer<out Any>的子类
open class Fruit(val name: String)
class Apple : Fruit("苹果")
class Orange : Fruit("橙子")

// --- 逆变（in）---
// in：消费者，只消费（接收）T，不产出（返回）T
// 即：方法可以把T作为参数类型，但不能返回T
interface Consumer<in T> {
    fun consume(item: T)  // 正确：T在参数位置（逆变点）
    // fun produce(): T  // 错误：T在返回位置（协变点）
}

// 逆变的应用：Any是String的父类，所以Consumer<Any>是Consumer<in String>的子类

// --- 星投影 ---
// 使用 * 代替类型参数，表示不知道具体类型
// - 对于out T：等价于 out Any?
// - 对于in T：等价于 in Nothing（可以认为什么都不能传入）
// - 对于不变的T：读取时是Any?，写入时是Nothing
fun printBox(box: Box<*>) {
    val item = box.getItem()  // item类型是Any?
    println("星投影Box内容：$item")
}

// ============================================================
// 2. 注解 annotation
// 注解是给代码添加元数据的方式
// ============================================================

// 定义一个注解
// 元注解：
// - @Target：指定注解可以用在什么地方（类、函数、属性等）
// - @Retention：指定注解保留到什么时候（源码、编译期、运行时）
// - @Repeatable：是否可重复使用
// - @MustBeDocumented：是否包含在生成的文档中

@Target(AnnotationTarget.CLASS, AnnotationTarget.FUNCTION, AnnotationTarget.PROPERTY)
@Retention(AnnotationRetention.RUNTIME)
@MustBeDocumented
annotation class Description(val text: String)  // 注解可以有参数

// 另一个注解
@Target(AnnotationTarget.PROPERTY)
@Retention(AnnotationRetention.RUNTIME)
annotation class NotNull

// 使用注解
@Description("这是一个用户数据类")
data class AnnotatedUser(
    @Description("用户ID")
    @NotNull
    val id: Int,

    @Description("用户姓名")
    val name: String
) {
    @Description("获取用户信息")
    fun getInfo(): String = "User($id, $name)"
}

// ============================================================
// 3. 反射
// 反射允许在运行时检查和操作类、函数、属性等
// ============================================================

// 用于反射演示的类
class ReflectionDemo(
    val name: String,
    var age: Int
) {
    // 主构造函数之外的属性
    var address: String = "未知"

    // 成员方法
    fun greet(greeting: String): String {
        return "$greeting，我是$name，今年$age岁"
    }

    // 私有方法
    private fun secret(): String {
        return "这是秘密"
    }

    // 伴生对象
    companion object {
        fun create(name: String): ReflectionDemo {
            return ReflectionDemo(name, 0)
        }
    }
}

// 反射演示函数
fun reflectionDemo() {
    println("=== 3. 反射 ===")

    // --- 获取类引用 ---
    // KClass：Kotlin的类引用
    val kClass: KClass<ReflectionDemo> = ReflectionDemo::class
    println("类名：${kClass.simpleName}")
    println("全限定名：${kClass.qualifiedName}")

    // 通过实例获取类引用
    val instance = ReflectionDemo("张三", 25)
    val instanceClass = instance::class
    println("实例的类：${instanceClass.simpleName}")

    // --- 获取属性 ---
    println("\n属性列表：")
    for (prop in kClass.memberProperties) {
        println("  属性：${prop.name}，类型：${prop.returnType}")
    }

    // 访问属性值
    val nameProp = kClass.memberProperties.find { it.name == "name" }
    if (nameProp != null) {
        val value = nameProp.get(instance)
        println("name属性值：$value")
    }

    // 修改可变属性
    val ageProp = kClass.memberProperties.find { it.name == "age" }
    if (ageProp is KProperty<*>) {
        // 需要转换为KMutableProperty才能set
        // 这里简化处理，直接使用实例
        instance.age = 30
        println("修改后age：${instance.age}")
    }

    // --- 获取函数 ---
    println("\n成员函数列表：")
    for (func in kClass.memberFunctions) {
        println("  函数：${func.name}，参数：${func.parameters.size}")
    }

    // 调用函数
    val greetFunc = kClass.memberFunctions.find { it.name == "greet" }
    if (greetFunc != null) {
        val result = greetFunc.call(instance, "你好")
        println("调用greet结果：$result")
    }

    // --- 构造函数 ---
    println("\n构造函数：")
    for (ctor in kClass.constructors) {
        println("  构造函数，参数数量：${ctor.parameters.size}")
    }

    // 通过构造函数创建实例
    val ctor = kClass.primaryConstructor
    if (ctor != null) {
        val newInstance = ctor.call("李四", 28)
        println("通过反射创建的实例：${newInstance.name}, ${newInstance.age}")
    }

    // --- 注解 ---
    println("\n类上的注解：")
    for (annotation in AnnotatedUser::class.annotations) {
        println("  注解：$annotation")
    }

    // 检查是否有特定注解
    val hasDesc = AnnotatedUser::class.hasAnnotation<Description>()
    println("AnnotatedUser有Description注解吗：$hasDesc")

    // 获取注解参数
    val descAnnotation = AnnotatedUser::class.findAnnotation<Description>()
    if (descAnnotation != null) {
        println("注解描述：${descAnnotation.text}")
    }
}

// ============================================================
// 4. 委托
// Kotlin的委托模式由语言级别的支持
// ============================================================

// --- 类委托 ---
// 使用 by 关键字将接口的实现委托给另一个对象
interface Base {
    fun print()
    fun printMessage(message: String)
}

class BaseImpl(val x: Int) : Base {
    override fun print() {
        println("BaseImpl: x = $x")
    }

    override fun printMessage(message: String) {
        println("BaseImpl收到消息：$message")
    }
}

// Derived类将Base接口的实现委托给b对象
// 编译器会自动生成所有Base接口的方法，调用b的对应方法
class Derived(b: Base) : Base by b {
    // 可以重写部分方法
    override fun print() {
        println("Derived: 重写了print方法")
    }
    // printMessage方法仍然由委托对象b实现
}

// --- 属性委托 ---
// 属性的getter/setter委托给另一个对象的getValue/setValue方法
// 标准库提供了几种有用的委托

// 自定义属性委托
class Delegate {
    private var value: String = ""

    // getValue：属性的getter委托
    operator fun getValue(thisRef: Any?, property: KProperty<*>): String {
        println("Delegate.getValue: 属性名=${property.name}, thisRef=$thisRef")
        return value
    }

    // setValue：属性的setter委托
    operator fun setValue(thisRef: Any?, property: KProperty<*>, newValue: String) {
        println("Delegate.setValue: 属性名=${property.name}, 新值=$newValue")
        value = newValue
    }
}

// 使用属性委托的类
class DelegateExample {
    // p属性的get/set委托给Delegate实例
    var p: String by Delegate()
}

// --- 标准委托：lazy ---
// lazy：延迟初始化，第一次访问时才计算，之后缓存结果
val lazyValue: String by lazy {
    println("lazy初始化（只会执行一次）")
    "Hello Lazy"
}

// --- 标准委托：observable ---
// Delegates.observable：属性变化时通知观察者
// 需要导入kotlin.properties.Delegates

class UserV2 {
    // 观察者委托：属性值改变时回调
    var name: String by kotlin.properties.Delegates.observable("初始值") { prop, old, new ->
        println("observable: ${prop.name} 从 '$old' 变为 '$new'")
    }

    // vetoable：可否决的属性变化
    // 如果回调返回false，则属性值不会改变
    var age: Int by kotlin.properties.Delegates.vetoable(0) { prop, old, new ->
        println("vetoable: ${prop.name} 尝试从 $old 变为 $new")
        new >= 0  // 只允许非负值，返回true才生效
    }
}

// ============================================================
// 5. 类型别名 typealias
// 给现有类型起一个更短或更有语义的名字
// ============================================================

// 给函数类型起别名
typealias ClickHandler = (Int, String) -> Unit

// 给集合类型起别名
typealias StringMap = Map<String, String>

// 给泛型类型起别名
typealias UserList<T> = List<T>

// 给内部类起别名
class OuterClass {
    inner class InnerClass(val name: String)
}
typealias Inner = OuterClass.InnerClass

// 使用类型别名的函数
fun handleClick(handler: ClickHandler) {
    handler(1, "点击事件")
}

// ============================================================
// 6. 内联类 / 值类（value class）
// 包装一个值，在运行时尽量不创建包装对象（编译时内联）
// 用于性能优化和类型安全
// ============================================================

// Kotlin 1.5+ 使用 value class（inline class 已弃用）
// value class 在 JVM 上会被尽可能地内联（不创建对象）
// 只能有一个属性（主构造函数中）
@JvmInline
value class Password(val value: String) {
    // 可以有方法和属性（但不能有幕后字段）
    val length: Int
        get() = value.length

    fun isStrong(): Boolean {
        return value.length >= 8 && value.any { it.isDigit() }
    }
}

@JvmInline
value class UserId(val id: Int)

// 使用value class提高类型安全
fun login(username: String, password: Password): Boolean {
    // 这里password是Password类型，不是普通String
    // 避免了参数顺序错误等问题
    println("登录用户：$username, 密码长度：${password.length}")
    return password.isStrong()
}

// ============================================================
// 7. 解构声明
// 将一个对象解构成多个变量
// ============================================================

// data class 自动提供 componentN() 函数，支持解构
data class PersonDestruct(val name: String, val age: Int, val city: String)

// 自定义类也可以通过定义componentN()函数支持解构
class PointDestruct(val x: Int, val y: Int) {
    operator fun component1() = x
    operator fun component2() = y
}

fun destructuringDemo() {
    println("\n=== 7. 解构声明 ===")

    // data class的解构
    val person = PersonDestruct("张三", 25, "北京")
    val (name, age, city) = person  // 解构为三个变量
    println("解构：name=$name, age=$age, city=$city")

    // 不需要的变量可以用 _ 跳过
    val (name2, _, city2) = person
    println("跳过age：name=$name2, city=$city2")

    // 自定义类的解构
    val point = PointDestruct(10, 20)
    val (x, y) = point
    println("Point解构：x=$x, y=$y")

    // for循环中解构（map的entry）
    println("\nMap解构遍历：")
    val map = mapOf("a" to 1, "b" to 2, "c" to 3)
    for ((key, value) in map) {
        println("  $key -> $value")
    }

    // 解构Pair
    val pair = "Hello" to "World"
    val (first, second) = pair
    println("Pair解构：first=$first, second=$second")

    // 函数返回多个值（用data class或Pair）
    fun getMinMax(numbers: List<Int>): Pair<Int, Int> {
        return numbers.minOrNull()!! to numbers.maxOrNull()!!
    }
    val (min, max) = getMinMax(listOf(3, 1, 4, 1, 5, 9, 2, 6))
    println("最小值=$min, 最大值=$max")
}

// ============================================================
// 8. 异常处理
// ============================================================

// 自定义异常类
class InvalidAgeException(message: String) : Exception(message)

class InvalidNameException(message: String) : RuntimeException(message)

fun exceptionDemo() {
    println("\n=== 8. 异常处理 ===")

    // --- try-catch ---
    println("--- try-catch ---")
    try {
        val result = 10 / 0
        println("结果：$result")
    } catch (e: ArithmeticException) {
        println("捕获到算术异常：${e.message}")
    }

    // --- try-catch-finally ---
    println("\n--- try-catch-finally ---")
    try {
        val str = "hello"
        val num = str.toInt()
        println("数字：$num")
    } catch (e: NumberFormatException) {
        println("捕获到数字格式异常：${e.message}")
    } finally {
        println("finally块总是会执行")
    }

    // --- try是表达式 ---
    println("\n--- try表达式 ---")
    val input = "123"
    val number: Int? = try {
        input.toInt()
    } catch (e: NumberFormatException) {
        null
    }
    println("try表达式结果：$number")

    // --- 多个catch块 ---
    println("\n--- 多个catch块 ---")
    fun parseNumber(str: String): Int? {
        return try {
            str.toInt()
        } catch (e: NumberFormatException) {
            println("数字格式错误")
            null
        } catch (e: NullPointerException) {
            println("空指针异常")
            null
        }
    }
    parseNumber("abc")

    // --- 自定义异常 ---
    println("\n--- 自定义异常 ---")
    fun validateUser(name: String, age: Int) {
        if (name.isBlank()) {
            throw InvalidNameException("姓名不能为空")
        }
        if (age < 0 || age > 150) {
            throw InvalidAgeException("年龄无效：$age")
        }
    }

    try {
        validateUser("", 25)
    } catch (e: InvalidNameException) {
        println("姓名验证失败：${e.message}")
    }

    try {
        validateUser("张三", 200)
    } catch (e: InvalidAgeException) {
        println("年龄验证失败：${e.message}")
    }

    // --- Kotlin 没有受检异常（checked exception）---
    // 所有异常都是非受检的，不需要在函数签名中声明throws
    println("\nKotlin没有受检异常，所有异常都是运行时异常")
}

// ============================================================
// 9. 操作符重载详解
// ============================================================

// 复数类，演示各种操作符重载
data class Complex(val real: Double, val imaginary: Double) {
    // + 加法
    operator fun plus(other: Complex): Complex =
        Complex(real + other.real, imaginary + other.imaginary)

    // - 减法
    operator fun minus(other: Complex): Complex =
        Complex(real - other.real, imaginary - other.imaginary)

    // * 乘法
    operator fun times(other: Complex): Complex =
        Complex(
            real * other.real - imaginary * other.imaginary,
            real * other.imaginary + imaginary * other.real
        )

    // 一元负号 -c
    operator fun unaryMinus(): Complex =
        Complex(-real, -imaginary)

    // ++ 自增
    operator fun inc(): Complex =
        Complex(real + 1, imaginary)

    // == 等于（data class自动生成，这里显式演示）
    // equals 不需要operator关键字（实际上Any中已经定义）

    // compareTo：比较运算符 < > <= >=
    // 按模长比较
    operator fun compareTo(other: Complex): Int {
        val thisMag = real * real + imaginary * imaginary
        val otherMag = other.real * other.real + other.imaginary * other.imaginary
        return thisMag.compareTo(otherMag)
    }

    // get：索引访问 c[0], c[1]
    operator fun get(index: Int): Double = when (index) {
        0 -> real
        1 -> imaginary
        else -> throw IndexOutOfBoundsException("复数只有实部和虚部")
    }

    // contains：in 运算符
    operator fun contains(value: Double): Boolean =
        real == value || imaginary == value

    // invoke：函数调用 c()
    operator fun invoke(): String = toString()

    override fun toString(): String = "$real + ${imaginary}i"
}

fun operatorOverloadDemo() {
    println("\n=== 9. 操作符重载详解 ===")

    val c1 = Complex(3.0, 4.0)
    val c2 = Complex(1.0, 2.0)

    println("c1 = $c1")
    println("c2 = $c2")
    println("c1 + c2 = ${c1 + c2}")
    println("c1 - c2 = ${c1 - c2}")
    println("c1 * c2 = ${c1 * c2}")
    println("-c1 = ${-c1}")

    var c3 = Complex(0.0, 0.0)
    c3++
    println("c3++ = $c3")

    println("c1 > c2：${c1 > c2}")
    println("c1 < c2：${c1 < c2}")

    println("c1[0] = ${c1[0]}（实部）")
    println("c1[1] = ${c1[1]}（虚部）")

    println("3.0 in c1：${3.0 in c1}")

    println("c1() = ${c1()}")
}

// ============================================================
// 10. DSL 构建器原理
// DSL（领域特定语言）利用带接收者的Lambda和扩展函数构建
// ============================================================

// --- 一个简单的HTML DSL示例 ---

// HTML元素基类
@DslMarker
annotation class HtmlTagMarker

@HtmlTagMarker
abstract class Tag(val name: String) {
    // 子元素列表
    protected val children = mutableListOf<Tag>()

    // 属性映射
    protected val attributes = mutableMapOf<String, String>()

    // 添加子元素
    protected fun <T : Tag> addChild(tag: T, init: T.() -> Unit): T {
        tag.init()
        children.add(tag)
        return tag
    }

    // 设置属性
    operator fun String.invoke(value: String) {
        attributes[this] = value
    }

    // 渲染为HTML字符串
    open fun render(builder: StringBuilder, indent: String = "") {
        builder.append("$indent<$name")
        // 渲染属性
        if (attributes.isNotEmpty()) {
            attributes.forEach { (key, value) ->
                builder.append(" $key=\"$value\"")
            }
        }
        if (children.isEmpty()) {
            builder.append(" />\n")
        } else {
            builder.append(">\n")
            children.forEach { it.render(builder, "$indent  ") }
            builder.append("$indent</$name>\n")
        }
    }
}

// 文本节点
class TextElement(private val text: String) : Tag("text") {
    override fun render(builder: StringBuilder, indent: String) {
        builder.append("$indent$text\n")
    }
}

// body标签
class Body : Tag("body") {
    fun h1(init: H1.() -> Unit) = addChild(H1(), init)
    fun p(init: P.() -> Unit) = addChild(P(), init)
    fun div(init: Div.() -> Unit) = addChild(Div(), init)
}

class H1 : Tag("h1") {
    // 添加文本内容
    operator fun String.unaryPlus() {
        children.add(TextElement(this))
    }
}

class P : Tag("p") {
    operator fun String.unaryPlus() {
        children.add(TextElement(this))
    }
}

class Div : Tag("div") {
    operator fun String.unaryPlus() {
        children.add(TextElement(this))
    }

    fun p(init: P.() -> Unit) = addChild(P(), init)
}

// html根标签
class Html : Tag("html") {
    fun body(init: Body.() -> Unit) = addChild(Body(), init)
}

// DSL入口函数：带接收者的Lambda
fun html(init: Html.() -> Unit): Html {
    val html = Html()
    html.init()
    return html
}

fun dslDemo() {
    println("\n=== 10. DSL 构建器原理 ===")

    // 使用DSL构建HTML
    val myHtml = html {
        "lang"("zh-CN")  // 设置属性

        body {
            h1 {
                "class"("title")
                +"欢迎来到Kotlin DSL"
            }

            p {
                +"这是一个简单的HTML DSL示例"
            }

            div {
                "id"("container")
                p {
                    +"DSL利用了带接收者的Lambda和扩展函数"
                }
                p {
                    +"使得代码结构和数据结构完全一致"
                }
            }
        }
    }

    // 渲染并打印
    val stringBuilder = StringBuilder()
    myHtml.render(stringBuilder)
    println("生成的HTML：")
    println(stringBuilder.toString())

    println("DSL原理：")
    println("  1. 带接收者的Lambda：lambda内部可以直接访问接收者的成员")
    println("  2. 扩展函数：为类添加新的方法，构建嵌套结构")
    println("  3. operator fun String.unaryPlus()：+字符串语法")
    println("  4. invoke约定：属性函数式调用（\"class\"(\"value\")）")
    println("  5. @DslMarker：限制外部接收者的隐式访问，避免混乱")
}

// ============================================================
// 主演示函数
// ============================================================
fun runAdvancedDemo() {
    println("============================================================")
    println("                  Kotlin 高级特性演示")
    println("============================================================")

    // --- 1. 泛型 ---
    println("=== 1. 泛型 ===")

    // 泛型类
    val intBox = Box(42)
    intBox.printItem()
    val strBox = Box("Hello")
    strBox.printItem()

    // 泛型函数
    val list = singletonList("单个元素")
    println("singletonList：$list")

    val transformed = transformAndWrap(5) { it * it }
    transformed.printItem()

    // 泛型约束
    val numbers = listOf(1, 2, 3, 4, 5)
    println("sumOfList：${sumOfList(numbers)}")

    // 实现多个接口的类，用于多约束演示
    data class Entity(private val id: Int, private val name: String) : Nameable, Identifiable {
        override fun getName(): String = name
        override fun getId(): Int = id
    }
    val entity = Entity(1, "测试实体")
    printEntity(entity)

    // 协变
    println("\n协变 out：")
    val appleBox: Box<Apple> = Box(Apple())
    // Box<Apple> 能否赋值给 Box<out Fruit>?
    // 因为Box是不变的（invariant），所以不能直接赋值
    // 但如果使用 Producer<out T> 接口，就可以
    val fruitProducer: Producer<Fruit> = object : Producer<Apple> {
        override fun produce(): Apple = Apple()
    }
    println("协变Producer<Apple> 可以赋值给 Producer<Fruit>")

    // 星投影
    println("\n星投影：")
    printBox(intBox)
    printBox(strBox)

    // --- 2. 注解 ---
    println("\n=== 2. 注解 ===")
    val annUser = AnnotatedUser(1, "张三")
    println("注解类：$annUser")

    // --- 3. 反射 ---
    reflectionDemo()

    // --- 4. 委托 ---
    println("\n=== 4. 委托 ===")

    // 类委托
    val base = BaseImpl(10)
    val derived = Derived(base)
    derived.print()  // 调用重写的方法
    derived.printMessage("Hello")  // 调用委托的方法

    // 属性委托
    val example = DelegateExample()
    example.p = "新值"
    println("委托属性值：${example.p}")

    // lazy委托
    println("第一次访问lazyValue：")
    println(lazyValue)
    println("第二次访问lazyValue：")
    println(lazyValue)

    // observable委托
    val userV2 = UserV2()
    userV2.name = "张三"
    userV2.name = "李四"

    userV2.age = 25
    userV2.age = -5  // 会被否决
    println("age最终值：${userV2.age}")

    // --- 5. 类型别名 ---
    println("\n=== 5. 类型别名 typealias ===")

    val handler: ClickHandler = { id, msg ->
        println("点击事件：id=$id, msg=$msg")
    }
    handleClick(handler)

    val map: StringMap = mapOf("key1" to "value1", "key2" to "value2")
    println("StringMap：$map")

    // --- 6. 内联类 / value class ---
    println("\n=== 6. 值类 value class ===")

    val pwd = Password("MyP@ssw0rd")
    println("密码是否强壮：${pwd.isStrong()}")
    login("user123", pwd)

    val userId = UserId(1001)
    println("UserId：${userId.id}")
    // login("user", "weak")  // 错误：类型不匹配，必须传Password类型

    // --- 7. 解构声明 ---
    destructuringDemo()

    // --- 8. 异常处理 ---
    exceptionDemo()

    // --- 9. 操作符重载 ---
    operatorOverloadDemo()

    // --- 10. DSL构建器 ---
    dslDemo()

    println("\n============================================================")
    println("                  高级特性演示结束")
    println("============================================================")
}
