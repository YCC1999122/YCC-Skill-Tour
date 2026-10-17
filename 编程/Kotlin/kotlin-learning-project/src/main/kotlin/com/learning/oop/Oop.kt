// ============================================================
// 包声明
// ============================================================
package com.learning.oop

// ============================================================
// 面向对象编程演示
// 包含：类、构造函数、属性、继承、抽象类、接口、数据类、枚举、
//      密封类、单例、伴生对象、扩展函数、运算符重载、嵌套类等
// ============================================================

// ============================================================
// 1. 类与构造函数
// ============================================================

// --- 主构造函数 ---
// 类名后面的括号就是主构造函数
// 参数前加val/var会自动成为类的属性
class Person(
    val name: String,           // 只读属性，自动生成getter
    var age: Int,               // 可变属性，自动生成getter和setter
    private var address: String = "未知"  // 带默认值的私有属性
) {
    // init初始化块：在主构造函数执行时运行
    // 可以有多个init块，按声明顺序执行
    init {
        // 可以在这里进行初始化逻辑、参数校验等
        println("Person第一个init块执行：name=$name, age=$age")
        require(age >= 0) { "年龄不能为负数" }  // 要求年龄非负
    }

    // 第二个init块
    init {
        println("Person第二个init块执行")
    }

    // --- 次构造函数 ---
    // 使用constructor关键字，必须委托给主构造函数（直接或间接）
    constructor(name: String) : this(name, 0) {
        println("次构造函数执行：只传入姓名，年龄默认0")
    }

    // 无参次构造函数
    constructor() : this("匿名", 0) {
        println("无参次构造函数执行")
    }

    // 成员函数
    fun introduce() {
        println("大家好，我叫$name，今年$age岁，住在$address")
    }

    // 可以修改私有属性的函数
    fun updateAddress(newAddress: String) {
        address = newAddress
    }
}

// ============================================================
// 2. 属性：幕后字段、自定义getter/setter
// ============================================================
class Rectangle(val width: Double, val height: Double) {
    // 自定义getter：计算属性（没有幕后字段）
    // 每次访问都会重新计算
    val area: Double
        get() = width * height  // 自定义getter

    // 带幕后字段的属性，自定义setter
    var color: String = "白色"
        set(value) {
            // field是幕后字段（backing field），只能在getter/setter中使用
            if (value.isNotBlank()) {
                field = value  // field代表实际存储值
            } else {
                println("颜色不能为空")
            }
        }

    // 自定义getter和setter的属性
    var description: String = ""
        get() = "矩形：宽=$width, 高=$height, 颜色=$color"  // 自定义getter
        set(value) {
            field = value
            println("描述被设置为：$value")
        }

    // 延迟初始化属性：lateinit
    // 用于不能在构造函数中初始化，但确定使用前会赋值的var属性
    lateinit var label: String

    // 检查lateinit是否已初始化
    fun isLabelInitialized(): Boolean = ::label.isInitialized
}

// ============================================================
// 3. 可见性修饰符
// public    ：默认，任何地方可见
// private   ：仅在当前类/文件内可见
// protected ：仅在当前类和子类中可见
// internal  ：同一模块内可见
// ============================================================
internal open class VisibilityDemo {
    // public属性（默认）
    val publicProperty = "公开的"

    // private属性：只有类内部可以访问
    private val privateProperty = "私有的"

    // protected属性：类内部和子类可以访问
    protected val protectedProperty = "受保护的"

    // internal属性：同一模块内可见
    internal val internalProperty = "模块内可见的"

    fun test() {
        // 类内部可以访问所有属性
        println(publicProperty)
        println(privateProperty)
        println(protectedProperty)
        println(internalProperty)
    }
}

// 子类可以访问protected和public和internal
class VisibilitySub : VisibilityDemo() {
    fun subTest() {
        println(publicProperty)       // 可以访问
        // println(privateProperty)   // 不能访问
        println(protectedProperty)    // 可以访问
        println(internalProperty)     // 可以访问
    }
}

// ============================================================
// 4. 继承：open/override/final
// Kotlin中类和方法默认是final的，不能被继承/重写
// 需要加open关键字才能被继承/重写
// ============================================================

// 父类：使用open关键字表示可以被继承
open class Animal(open val name: String) {
    // open方法：可以被重写
    open fun makeSound() {
        println("$name 发出了声音")
    }

    // final方法：不能被重写（默认就是final，这里显式声明）
    final fun eat() {
        println("$name 在吃东西")
    }
}

// 子类：使用 : 父类(构造参数) 表示继承
class Dog(override val name: String) : Animal(name) {
    // 重写方法：使用override关键字
    override fun makeSound() {
        println("$name 汪汪叫！")
    }

    // 子类自己的方法
    fun fetch() {
        println("$name 在捡飞盘")
    }
}

class Cat(override val name: String) : Animal(name) {
    override fun makeSound() {
        println("$name 喵喵叫！")
    }
}

// ============================================================
// 5. 抽象类
// 抽象类不能被实例化，只能被继承
// 抽象方法没有实现，必须由子类实现
// ============================================================
abstract class Shape(val name: String) {
    // 抽象属性：子类必须实现
    abstract val area: Double

    // 抽象方法：子类必须实现
    abstract fun draw()

    // 普通方法：可以有实现
    fun printInfo() {
        println("$name 的面积是 $area")
    }
}

class Circle(name: String, val radius: Double) : Shape(name) {
    // 实现抽象属性
    override val area: Double
        get() = Math.PI * radius * radius

    // 实现抽象方法
    override fun draw() {
        println("绘制一个半径为$radius 的圆：$name")
    }
}

class RectangleShape(name: String, val width: Double, val height: Double) : Shape(name) {
    override val area: Double
        get() = width * height

    override fun draw() {
        println("绘制一个宽$width 高$height 的矩形：$name")
    }
}

// ============================================================
// 6. 接口
// 接口可以包含抽象方法和有实现的方法
// 一个类可以实现多个接口
// ============================================================
interface Drivable {
    // 抽象方法（默认就是abstract的）
    fun drive()

    // 有默认实现的方法
    fun stop() {
        println("车辆停止了")
    }
}

interface Flyable {
    fun fly()

    fun stop() {
        println("飞行停止了")
    }
}

// 实现多个接口
class FlyingCar : Drivable, Flyable {
    override fun drive() {
        println("飞行汽车在路上行驶")
    }

    override fun fly() {
        println("飞行汽车在空中飞行")
    }

    // 当多个接口有同名方法时，必须重写并指定调用哪个接口的实现
    override fun stop() {
        // 使用 super<接口名> 指定调用哪个接口的默认实现
        super<Drivable>.stop()
        super<Flyable>.stop()
        println("飞行汽车完全停止了")
    }
}

// ============================================================
// 7. 数据类 data class
// 专门用于存储数据的类，自动生成以下方法：
// - equals() / hashCode()
// - toString()
// - componentN() 函数（对应主构造函数的每个属性）
// - copy() 函数
// 要求：主构造函数至少有一个参数，且都是val/var
// ============================================================
data class User(
    val id: Int,
    val name: String,
    val email: String,
    var age: Int = 18
)

// ============================================================
// 8. 枚举类 enum class
// 枚举类的每个实例都是一个对象（单例）
// ============================================================
enum class Color(val rgb: Int, val chineseName: String) {
    // 枚举常量：必须在最前面列出
    RED(0xFF0000, "红色"),
    GREEN(0x00FF00, "绿色"),
    BLUE(0x0000FF, "蓝色"),
    YELLOW(0xFFFF00, "黄色"),
    BLACK(0x000000, "黑色");  // 注意：最后一个枚举常量后要加分号

    // 枚举类可以有方法
    fun isWarmColor(): Boolean {
        return this == RED || this == YELLOW
    }

    // 伴生对象
    companion object {
        fun fromRgb(rgb: Int): Color? {
            return values().find { it.rgb == rgb }
        }
    }
}

// ============================================================
// 9. 密封类 sealed class
// 密封类表示受限的类继承结构，子类数量有限
// 常用于表示受限的状态（类似枚举，但每个子类可以有不同的属性）
// 密封类是抽象的，不能直接实例化
// ============================================================
sealed class Result<out T> {
    // 成功状态：携带数据
    data class Success<T>(val data: T) : Result<T>()

    // 错误状态：携带异常信息
    data class Error(val message: String, val exception: Throwable? = null) : Result<Nothing>()

    // 加载中状态：没有数据
    object Loading : Result<Nothing>()
}

// ============================================================
// 10. 单例 object 关键字
// 使用object声明的类只有一个实例（单例）
// 不能有构造函数，在首次访问时初始化
// ============================================================
object Singleton {
    // 单例的属性
    var count = 0

    // 单例的方法
    fun increment() {
        count++
        println("单例计数器：$count")
    }

    fun getInfo(): String {
        return "Singleton实例，当前计数：$count"
    }
}

// ============================================================
// 11. 伴生对象 companion object
// 类内部的object，类似Java的静态成员
// 伴生对象的成员可以通过类名直接访问
// ============================================================
class Calculator {
    // 实例方法
    fun add(a: Int, b: Int): Int = a + b

    // 伴生对象
    companion object {
        // 类似Java的静态常量
        const val PI = 3.1415926

        // 类似Java的静态方法
        fun multiply(a: Int, b: Int): Int = a * b

        fun divide(a: Int, b: Int): Double {
            require(b != 0) { "除数不能为0" }
            return a.toDouble() / b
        }
    }
}

// ============================================================
// 12. 扩展函数/扩展属性
// 可以在不修改类定义的情况下，为类添加新的函数和属性
// 扩展是静态解析的，不是真正修改了类
// ============================================================

// 为String类添加扩展函数
fun String.reverse(): String {
    // this代表调用这个函数的字符串对象
    return this.reversed()
}

// 为Int类添加扩展函数
fun Int.isEven(): Boolean {
    return this % 2 == 0
}

// 扩展属性（只能是计算属性，因为不能有幕后字段）
val String.lastChar: Char
    get() = this[this.length - 1]

// 可空类型的扩展函数
fun String?.printSafely() {
    // 这里this可能为null
    if (this != null) {
        println("安全打印：$this")
    } else {
        println("安全打印：字符串为null")
    }
}

// ============================================================
// 13. 运算符重载
// Kotlin允许为类重载预定义的运算符
// 使用operator关键字
// ============================================================
data class Point(val x: Int, val y: Int) {
    // 重载 + 运算符：对应plus函数
    operator fun plus(other: Point): Point {
        return Point(x + other.x, y + other.y)
    }

    // 重载 - 运算符：对应minus函数
    operator fun minus(other: Point): Point {
        return Point(x - other.x, y - other.y)
    }

    // 重载 * 运算符：乘以一个数
    operator fun times(scalar: Int): Point {
        return Point(x * scalar, y * scalar)
    }

    // 重载 == 运算符：对应equals（data class自动生成）
    // 重载 ++ 运算符：对应inc
    operator fun inc(): Point {
        return Point(x + 1, y + 1)
    }

    // 重载 get 运算符：支持 p[0], p[1] 访问
    operator fun get(index: Int): Int {
        return when (index) {
            0 -> x
            1 -> y
            else -> throw IndexOutOfBoundsException("无效索引：$index")
        }
    }

    // 重载 in 运算符：contains
    operator fun contains(value: Int): Boolean {
        return x == value || y == value
    }
}

// ============================================================
// 14. 嵌套类/内部类 inner
// ============================================================

// 外部类
class Outer(val name: String) {
    private val outerProperty = "外部类属性"

    // --- 嵌套类（默认） ---
    // 类似Java的静态内部类，不能访问外部类的成员
    class Nested {
        fun nestedFunction() {
            println("这是嵌套类的方法")
            // println(outerProperty)  // 错误：不能访问外部类成员
        }
    }

    // --- 内部类（inner关键字） ---
    // 类似Java的非静态内部类，可以访问外部类的成员
    // 内部类持有外部类的引用
    inner class Inner {
        fun innerFunction() {
            println("这是内部类的方法，可以访问外部类：$name, $outerProperty")
            // 使用 this@Outer 引用外部类实例
            println("外部类引用：${this@Outer.name}")
        }
    }
}

// ============================================================
// 主演示函数
// ============================================================
fun runOopDemo() {
    println("============================================================")
    println("                   Kotlin 面向对象演示")
    println("============================================================")

    // --- 1. 类与构造函数 ---
    println("\n=== 1. 类与构造函数 ===")
    val person1 = Person("张三", 25, "北京")  // 主构造函数
    person1.introduce()

    val person2 = Person("李四")  // 次构造函数（一个参数）
    person2.introduce()

    val person3 = Person()  // 无参次构造函数
    person3.introduce()
    person3.updateAddress("上海")
    person3.introduce()

    // --- 2. 属性与自定义getter/setter ---
    println("\n=== 2. 属性与自定义getter/setter ===")
    val rect = Rectangle(5.0, 3.0)
    println("矩形面积：${rect.area}")  // 调用自定义getter
    rect.color = "红色"
    println("矩形颜色：${rect.color}")
    rect.color = ""  // 空字符串会被setter拒绝
    println("矩形描述：${rect.description}")

    // lateinit演示
    rect.label = "这是一个矩形"
    println("label已初始化：${rect.isLabelInitialized()}")
    println("label值：${rect.label}")

    // --- 3. 可见性修饰符 ---
    println("\n=== 3. 可见性修饰符 ===")
    val visibility = VisibilityDemo()
    visibility.test()
    // println(visibility.privateProperty)  // 不能访问private
    // println(visibility.protectedProperty)  // 不能访问protected
    println("可以访问public：${visibility.publicProperty}")
    println("可以访问internal：${visibility.internalProperty}")

    // --- 4. 继承 ---
    println("\n=== 4. 继承 ===")
    val dog: Animal = Dog("旺财")
    dog.makeSound()  // 多态：调用的是Dog的方法
    dog.eat()        // final方法，不能被重写

    val cat: Animal = Cat("咪咪")
    cat.makeSound()

    // --- 5. 抽象类 ---
    println("\n=== 5. 抽象类 ===")
    val circle: Shape = Circle("大圆", 5.0)
    circle.draw()
    circle.printInfo()

    val rectShape: Shape = RectangleShape("矩形", 4.0, 6.0)
    rectShape.draw()
    rectShape.printInfo()

    // --- 6. 接口 ---
    println("\n=== 6. 接口 ===")
    val flyingCar = FlyingCar()
    flyingCar.drive()
    flyingCar.fly()
    flyingCar.stop()

    // --- 7. 数据类 ---
    println("\n=== 7. 数据类 ===")
    val user1 = User(1, "张三", "zhangsan@example.com", 25)
    val user2 = User(1, "张三", "zhangsan@example.com", 25)
    val user3 = User(2, "李四", "lisi@example.com")

    // 自动生成的toString()
    println("user1: $user1")
    println("user3: $user3")

    // 自动生成的equals()
    println("user1 == user2：${user1 == user2}")  // true，因为所有属性都相等
    println("user1 == user3：${user1 == user3}")  // false

    // copy()函数：复制对象并修改部分属性
    val user4 = user1.copy(name = "张三丰", age = 30)
    println("copy后的user4：$user4")

    // 解构声明：componentN()函数
    val (id, name, email, age) = user1
    println("解构：id=$id, name=$name, email=$email, age=$age")

    // --- 8. 枚举类 ---
    println("\n=== 8. 枚举类 ===")
    val red = Color.RED
    println("颜色：${red.chineseName}，RGB值：${red.rgb.toString(16)}")
    println("红色是暖色吗：${red.isWarmColor()}")
    println("蓝色是暖色吗：${Color.BLUE.isWarmColor()}")

    // 遍历所有枚举值
    println("所有颜色：")
    for (color in Color.values()) {
        println("  ${color.ordinal}: ${color.name} - ${color.chineseName}")
    }

    // --- 9. 密封类 ---
    println("\n=== 9. 密封类 ===")

    fun handleResult(result: Result<String>) {
        when (result) {
            is Result.Success -> println("成功！数据：${result.data}")
            is Result.Error -> println("错误！消息：${result.message}")
            is Result.Loading -> println("加载中...")
            // 不需要else，因为密封类的子类是有限的，编译器可以检查是否覆盖所有情况
        }
    }

    handleResult(Result.Loading)
    handleResult(Result.Success("Hello Sealed Class"))
    handleResult(Result.Error("网络连接失败"))

    // --- 10. 单例 ---
    println("\n=== 10. 单例 object ===")
    Singleton.increment()
    Singleton.increment()
    println(Singleton.getInfo())

    // 再次调用，还是同一个实例
    Singleton.increment()
    println(Singleton.getInfo())

    // --- 11. 伴生对象 ---
    println("\n=== 11. 伴生对象 ===")
    // 通过类名直接访问伴生对象成员（类似Java静态成员）
    println("PI常量：${Calculator.PI}")
    println("乘法：3 * 4 = ${Calculator.multiply(3, 4)}")
    println("除法：10 / 3 = ${Calculator.divide(10, 3)}")

    // --- 12. 扩展函数/扩展属性 ---
    println("\n=== 12. 扩展函数/扩展属性 ===")
    val str = "Hello Kotlin"
    println("反转字符串：${str.reverse()}")  // 使用自定义的扩展函数

    val num = 6
    println("$num 是偶数吗：${num.isEven()}")
    println("7 是偶数吗：${7.isEven()}")

    println("字符串最后一个字符：${str.lastChar}")  // 扩展属性

    val nullStr: String? = null
    nullStr.printSafely()  // 可空类型的扩展函数
    "非空字符串".printSafely()

    // --- 13. 运算符重载 ---
    println("\n=== 13. 运算符重载 ===")
    val p1 = Point(3, 5)
    val p2 = Point(2, 1)

    println("p1 = $p1")
    println("p2 = $p2")
    println("p1 + p2 = ${p1 + p2}")  // 调用plus
    println("p1 - p2 = ${p1 - p2}")  // 调用minus
    println("p1 * 3 = ${p1 * 3}")    // 调用times

    var p3 = Point(0, 0)
    p3++  // 调用inc
    println("p3++ 后：$p3")

    println("p1[0] = ${p1[0]}")  // 调用get
    println("p1[1] = ${p1[1]}")

    println("3 在 p1 中吗：${3 in p1}")  // 调用contains
    println("10 在 p1 中吗：${10 in p1}")

    // --- 14. 嵌套类/内部类 ---
    println("\n=== 14. 嵌套类/内部类 ===")
    // 嵌套类：通过外部类名直接访问
    val nested = Outer.Nested()
    nested.nestedFunction()

    // 内部类：需要外部类实例才能创建
    val outer = Outer("外部对象")
    val inner = outer.Inner()
    inner.innerFunction()

    println("\n============================================================")
    println("                  面向对象演示结束")
    println("============================================================")
}
