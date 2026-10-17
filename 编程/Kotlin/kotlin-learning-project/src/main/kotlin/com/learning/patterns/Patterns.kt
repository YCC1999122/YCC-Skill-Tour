// ============================================================
// 包声明
// ============================================================
package com.learning.patterns

// ============================================================
// 设计模式演示
// 包含：单例模式、工厂模式、观察者模式、策略模式、
//      建造者模式（DSL风格）、装饰器模式、状态模式
// ============================================================

// ============================================================
// 1. 单例模式（object 关键字）
// Kotlin中使用object关键字直接实现单例，线程安全，懒加载
// ============================================================

// 最简单的单例：直接使用object声明
object SingletonPattern {
    // 单例的属性
    private var count = 0

    // 单例的方法
    fun getInstanceName(): String = "SingletonPattern单例"

    fun incrementAndGet(): Int {
        count++
        return count
    }

    fun getCount(): Int = count
}

// 带初始化的单例（在init块中初始化）
object ConfigManager {
    // 配置项
    val appName: String
    val version: String
    val maxConnections: Int

    // 初始化块：首次访问时执行一次
    init {
        println("ConfigManager 单例初始化中...")
        appName = "Kotlin学习项目"
        version = "1.0.0"
        maxConnections = 100
    }

    fun printConfig() {
        println("应用名：$appName")
        println("版本：$version")
        println("最大连接数：$maxConnections")
    }
}

// 单例演示函数
fun singletonPatternDemo() {
    println("=== 1. 单例模式（object 关键字）===")

    // 直接通过对象名访问（首次访问时初始化）
    println("单例名称：${SingletonPattern.getInstanceName()}")
    println("计数：${SingletonPattern.incrementAndGet()}")
    println("计数：${SingletonPattern.incrementAndGet()}")
    println("计数：${SingletonPattern.incrementAndGet()}")

    // 再次访问，还是同一个实例
    println("最终计数：${SingletonPattern.getCount()}")

    println("\n配置管理器单例：")
    ConfigManager.printConfig()

    println("\nKotlin的object单例特点：")
    println("  - 线程安全（由JVM保证）")
    println("  - 懒加载（首次访问时初始化）")
    println("  - 不能有构造函数")
    println("  - 本质是Java中的饿汉式单例（静态初始化）")
}

// ============================================================
// 2. 工厂模式（伴生对象工厂/简单工厂）
// ============================================================

// --- 产品接口 ---
interface Transport {
    fun deliver()
    fun getType(): String
}

// --- 具体产品 ---
class Truck : Transport {
    override fun deliver() {
        println("  🚚 卡车通过陆路运输货物")
    }

    override fun getType(): String = "卡车"
}

class Ship : Transport {
    override fun deliver() {
        println("  🚢 轮船通过海路运输货物")
    }

    override fun getType(): String = "轮船"
}

class Airplane : Transport {
    override fun deliver() {
        println("  ✈️ 飞机通过空运运输货物")
    }

    override fun getType(): String = "飞机"
}

// --- 简单工厂（伴生对象实现）---
// 伴生对象的成员可以通过类名直接访问，类似Java的静态方法
class TransportFactory {
    companion object {
        // 工厂方法：根据类型创建对应的产品
        fun createTransport(type: String): Transport {
            return when (type.lowercase()) {
                "truck", "卡车", "陆路" -> Truck()
                "ship", "轮船", "海路" -> Ship()
                "airplane", "飞机", "空运" -> Airplane()
                else -> throw IllegalArgumentException("未知的运输方式：$type")
            }
        }

        // 便捷工厂方法：语义更清晰
        fun createTruck(): Transport = Truck()
        fun createShip(): Transport = Ship()
        fun createAirplane(): Transport = Airplane()
    }
}

// --- 工厂方法模式 ---
// 定义一个创建对象的接口，让子类决定实例化哪个类
abstract class Logistics {
    // 工厂方法：由子类实现，创建具体的Transport
    abstract fun createTransport(): Transport

    // 业务逻辑：使用工厂方法创建的对象
    fun planDelivery() {
        val transport = createTransport()
        println("计划运输方式：${transport.getType()}")
        transport.deliver()
    }
}

class RoadLogistics : Logistics() {
    override fun createTransport(): Transport = Truck()
}

class SeaLogistics : Logistics() {
    override fun createTransport(): Transport = Ship()
}

class AirLogistics : Logistics() {
    override fun createTransport(): Transport = Airplane()
}

fun factoryPatternDemo() {
    println("\n=== 2. 工厂模式 ===")

    // --- 简单工厂 ---
    println("--- 简单工厂（伴生对象）---")
    val truck = TransportFactory.createTransport("卡车")
    truck.deliver()

    val ship = TransportFactory.createTransport("海路")
    ship.deliver()

    val airplane = TransportFactory.createAirplane()
    airplane.deliver()

    // --- 工厂方法模式 ---
    println("\n--- 工厂方法模式 ---")
    val roadLogistics = RoadLogistics()
    roadLogistics.planDelivery()

    val seaLogistics = SeaLogistics()
    seaLogistics.planDelivery()

    val airLogistics = AirLogistics()
    airLogistics.planDelivery()

    println("\n工厂模式要点：")
    println("  - 简单工厂：一个工厂类创建多种产品，适合产品不多的场景")
    println("  - 工厂方法：每个产品对应一个工厂子类，符合开闭原则")
    println("  - Kotlin中可以用伴生对象实现简单工厂")
}

// ============================================================
// 3. 观察者模式
// 定义对象间的一对多依赖关系，当一个对象状态改变时，所有依赖者都收到通知
// ============================================================

// --- 观察者接口 ---
interface Observer {
    fun update(temperature: Float, humidity: Float, pressure: Float)
}

// --- 主题（被观察者）接口 ---
interface Subject {
    fun registerObserver(observer: Observer)   // 注册观察者
    fun removeObserver(observer: Observer)     // 移除观察者
    fun notifyObservers()                      // 通知所有观察者
}

// --- 具体主题：气象站 ---
class WeatherStation : Subject {
    // 观察者列表
    private val observers = mutableListOf<Observer>()

    // 气象数据
    private var temperature: Float = 0f
    private var humidity: Float = 0f
    private var pressure: Float = 0f

    override fun registerObserver(observer: Observer) {
        observers.add(observer)
    }

    override fun removeObserver(observer: Observer) {
        observers.remove(observer)
    }

    override fun notifyObservers() {
        for (observer in observers) {
            observer.update(temperature, humidity, pressure)
        }
    }

    // 设置气象数据（模拟数据更新）
    fun setMeasurements(temp: Float, humidity: Float, pressure: Float) {
        this.temperature = temp
        this.humidity = humidity
        this.pressure = pressure
        measurementsChanged()
    }

    private fun measurementsChanged() {
        notifyObservers()
    }
}

// --- 具体观察者：天气显示板 ---
class CurrentConditionsDisplay(private val name: String) : Observer {
    private var temperature: Float = 0f
    private var humidity: Float = 0f

    override fun update(temperature: Float, humidity: Float, pressure: Float) {
        this.temperature = temperature
        this.humidity = humidity
        display()
    }

    private fun display() {
        println("  [$name] 当前温度：${temperature}°C，湿度：${humidity}%")
    }
}

// 另一个观察者：天气预报
class ForecastDisplay : Observer {
    private var lastPressure: Float = 0f
    private var currentPressure: Float = 0f

    override fun update(temperature: Float, humidity: Float, pressure: Float) {
        lastPressure = currentPressure
        currentPressure = pressure
        display()
    }

    private fun display() {
        val forecast = when {
            currentPressure > lastPressure -> "天气正在好转"
            currentPressure == lastPressure -> "天气保持稳定"
            else -> "天气即将变坏"
        }
        println("  [天气预报] $forecast（气压变化：${currentPressure - lastPressure}）")
    }
}

fun observerPatternDemo() {
    println("\n=== 3. 观察者模式 ===")

    // 创建主题（气象站）
    val weatherStation = WeatherStation()

    // 创建观察者
    val display1 = CurrentConditionsDisplay("北京观测站")
    val display2 = CurrentConditionsDisplay("上海观测站")
    val forecast = ForecastDisplay()

    // 注册观察者
    weatherStation.registerObserver(display1)
    weatherStation.registerObserver(display2)
    weatherStation.registerObserver(forecast)

    // 模拟数据更新
    println("第一次气象数据更新：")
    weatherStation.setMeasurements(25.5f, 65f, 1013.25f)

    println("\n第二次气象数据更新：")
    weatherStation.setMeasurements(28.0f, 70f, 1012.00f)

    // 移除一个观察者
    println("\n移除上海观测站...")
    weatherStation.removeObserver(display2)

    println("\n第三次气象数据更新：")
    weatherStation.setMeasurements(22.0f, 80f, 1010.50f)

    println("\n观察者模式要点：")
    println("  - 主题和观察者是一对多关系")
    println("  - 主题状态变化时，自动通知所有观察者")
    println("  - 观察者之间相互独立，松耦合")
    println("  - Kotlin中也可以用Flow/Channel实现响应式观察者模式")
}

// ============================================================
// 4. 策略模式
// 定义一系列算法，把它们封装起来，并且使它们可以相互替换
// ============================================================

// --- 策略接口 ---
interface PaymentStrategy {
    fun pay(amount: Double): Boolean  // 支付
    fun getMethodName(): String       // 获取支付方式名称
}

// --- 具体策略：支付宝支付 ---
class AlipayStrategy(private val account: String) : PaymentStrategy {
    override fun pay(amount: Double): Boolean {
        println("  使用支付宝支付 ¥$amount（账户：$account）")
        // 模拟支付过程...
        return true
    }

    override fun getMethodName(): String = "支付宝"
}

// --- 具体策略：微信支付 ---
class WechatPayStrategy(private val openId: String) : PaymentStrategy {
    override fun pay(amount: Double): Boolean {
        println("  使用微信支付 ¥$amount（微信ID：$openId）")
        return true
    }

    override fun getMethodName(): String = "微信支付"
}

// --- 具体策略：银行卡支付 ---
class BankCardStrategy(
    private val cardNumber: String,
    private val cardHolder: String
) : PaymentStrategy {
    override fun pay(amount: Double): Boolean {
        val maskedCard = cardNumber.take(4) + "****" + cardNumber.takeLast(4)
        println("  使用银行卡支付 ¥$amount（卡号：$maskedCard，持卡人：$cardHolder）")
        return true
    }

    override fun getMethodName(): String = "银行卡"
}

// --- 上下文：购物车 ---
class ShoppingCart {
    private val items = mutableListOf<Pair<String, Double>>()  // 商品和价格

    // 当前支付策略
    private var paymentStrategy: PaymentStrategy? = null

    // 添加商品
    fun addItem(name: String, price: Double) {
        items.add(name to price)
    }

    // 设置支付策略（可以在运行时切换）
    fun setPaymentStrategy(strategy: PaymentStrategy) {
        this.paymentStrategy = strategy
    }

    // 计算总价
    fun calculateTotal(): Double {
        return items.sumOf { it.second }
    }

    // 结账
    fun checkout(): Boolean {
        val strategy = paymentStrategy
        if (strategy == null) {
            println("  错误：请先选择支付方式")
            return false
        }

        val total = calculateTotal()
        println("  购物车商品：${items.joinToString(", ") { "${it.first}(¥${it.second})" }}")
        println("  应付总额：¥$total")
        println("  支付方式：${strategy.getMethodName()}")

        return strategy.pay(total)
    }
}

fun strategyPatternDemo() {
    println("\n=== 4. 策略模式 ===")

    // 创建购物车
    val cart = ShoppingCart()

    // 添加商品
    cart.addItem("Kotlin编程指南", 89.0)
    cart.addItem("无线鼠标", 129.0)
    cart.addItem("机械键盘", 399.0)

    // 使用支付宝支付
    println("--- 使用支付宝支付 ---")
    cart.setPaymentStrategy(AlipayStrategy("user@example.com"))
    cart.checkout()

    // 切换到微信支付（运行时切换策略）
    println("\n--- 切换到微信支付 ---")
    cart.setPaymentStrategy(WechatPayStrategy("wx_abc123"))
    cart.checkout()

    // 切换到银行卡支付
    println("\n--- 切换到银行卡支付 ---")
    cart.setPaymentStrategy(BankCardStrategy("6222021234567890123", "张三"))
    cart.checkout()

    println("\n策略模式要点：")
    println("  - 算法可以在运行时动态切换")
    println("  - 每种算法封装在独立的类中，职责单一")
    println("  - 新增算法不需要修改现有代码（开闭原则）")
    println("  - Kotlin中也可以用高阶函数+Lambda实现轻量级策略模式")
}

// ============================================================
// 5. 建造者模式（DSL风格）
// 将复杂对象的构建与表示分离，同样的构建过程可以创建不同的表示
// Kotlin中可以用DSL风格实现更优雅的建造者
// ============================================================

// --- 产品：用户 ---
data class UserProfile(
    val username: String,
    val email: String,
    val firstName: String,
    val lastName: String,
    val age: Int,
    val address: String,
    val phone: String,
    val hobbies: List<String>,
    val isActive: Boolean
)

// --- DSL风格的建造者 ---
// 使用带接收者的Lambda构建对象
class UserBuilder {
    // 必填字段
    var username: String = ""
    var email: String = ""

    // 可选字段（有默认值）
    var firstName: String = ""
    var lastName: String = ""
    var age: Int = 0
    var address: String = ""
    var phone: String = ""
    private val hobbies = mutableListOf<String>()
    var isActive: Boolean = true

    // 添加爱好（DSL风格的方法）
    fun hobby(hobby: String) {
        hobbies.add(hobby)
    }

    // 批量添加爱好
    fun hobbies(vararg hobbyArray: String) {
        hobbies.addAll(hobbyArray)
    }

    // 构建方法
    fun build(): UserProfile {
        // 校验必填字段
        require(username.isNotBlank()) { "用户名不能为空" }
        require(email.isNotBlank()) { "邮箱不能为空" }

        return UserProfile(
            username = username,
            email = email,
            firstName = firstName,
            lastName = lastName,
            age = age,
            address = address,
            phone = phone,
            hobbies = hobbies.toList(),
            isActive = isActive
        )
    }
}

// DSL入口函数
fun user(builder: UserBuilder.() -> Unit): UserProfile {
    val userBuilder = UserBuilder()
    userBuilder.builder()
    return userBuilder.build()
}

// --- 更复杂的嵌套DSL：建造者模式的进阶 ---
// 比如一个订单系统，有多个嵌套对象

data class Order(
    val orderId: String,
    val customer: String,
    val items: List<OrderItem>,
    val shippingAddress: Address,
    val total: Double
)

data class OrderItem(
    val productName: String,
    val quantity: Int,
    val price: Double
)

data class Address(
    val street: String,
    val city: String,
    val zipCode: String,
    val country: String
)

// 订单DSL建造者
class OrderBuilder {
    var orderId: String = ""
    var customer: String = ""
    private val items = mutableListOf<OrderItem>()
    private var address: Address = Address("", "", "", "")

    // 订单项DSL
    fun item(block: OrderItemBuilder.() -> Unit) {
        val builder = OrderItemBuilder()
        builder.block()
        items.add(builder.build())
    }

    // 地址DSL
    fun shippingAddress(block: AddressBuilder.() -> Unit) {
        val builder = AddressBuilder()
        builder.block()
        address = builder.build()
    }

    fun build(): Order {
        val total = items.sumOf { it.price * it.quantity }
        return Order(orderId, customer, items, address, total)
    }
}

class OrderItemBuilder {
    var productName: String = ""
    var quantity: Int = 1
    var price: Double = 0.0

    fun build(): OrderItem = OrderItem(productName, quantity, price)
}

class AddressBuilder {
    var street: String = ""
    var city: String = ""
    var zipCode: String = ""
    var country: String = ""

    fun build(): Address = Address(street, city, zipCode, country)
}

// DSL入口
fun order(block: OrderBuilder.() -> Unit): Order {
    val builder = OrderBuilder()
    builder.block()
    return builder.build()
}

fun builderPatternDemo() {
    println("\n=== 5. 建造者模式（DSL风格）===")

    // --- 简单用户建造者 ---
    println("--- 用户建造者 ---")
    val user1 = user {
        username = "zhangsan"
        email = "zhangsan@example.com"
        firstName = "三"
        lastName = "张"
        age = 25
        address = "北京市朝阳区"
        phone = "13800138000"
        hobby("编程")
        hobby("读书")
        hobbies("游泳", "跑步", "摄影")
        isActive = true
    }
    println("创建的用户：")
    println("  用户名：${user1.username}")
    println("  邮箱：${user1.email}")
    println("  姓名：${user1.lastName}${user1.firstName}")
    println("  年龄：${user1.age}")
    println("  爱好：${user1.hobbies.joinToString("、")}")

    // --- 嵌套DSL订单建造者 ---
    println("\n--- 订单DSL建造者 ---")
    val myOrder = order {
        orderId = "ORD-2024-001"
        customer = "张三"

        item {
            productName = "Kotlin编程实战"
            quantity = 2
            price = 79.0
        }

        item {
            productName = "机械键盘"
            quantity = 1
            price = 399.0
        }

        item {
            productName = "无线鼠标"
            quantity = 1
            price = 129.0
        }

        shippingAddress {
            street = "科技园路100号"
            city = "深圳"
            zipCode = "518000"
            country = "中国"
        }
    }

    println("创建的订单：")
    println("  订单号：${myOrder.orderId}")
    println("  客户：${myOrder.customer}")
    println("  商品：")
    myOrder.items.forEach {
        println("    - ${it.productName} x ${it.quantity} = ¥${it.price * it.quantity}")
    }
    println("  收货地址：${myOrder.shippingAddress.country}${myOrder.shippingAddress.city}${myOrder.shippingAddress.street}")
    println("  订单总额：¥${myOrder.total}")

    println("\n建造者模式要点：")
    println("  - 将复杂对象的构建过程封装起来")
    println("  - 可以分步构建，灵活配置")
    println("  - Kotlin中用DSL（带接收者的Lambda）实现更优雅")
    println("  - 比传统Builder模式代码更简洁，可读性更好")
}

// ============================================================
// 6. 装饰器模式
// 动态地给一个对象添加额外的职责，比继承更灵活
// ============================================================

// --- 组件接口：咖啡 ---
interface Coffee {
    fun getDescription(): String
    fun cost(): Double
}

// --- 具体组件：简单咖啡 ---
class SimpleCoffee : Coffee {
    override fun getDescription(): String = "简单咖啡"
    override fun cost(): Double = 10.0
}

// --- 装饰器基类 ---
// 持有一个Coffee对象，同时也实现Coffee接口
abstract class CoffeeDecorator(protected val coffee: Coffee) : Coffee {
    override fun getDescription(): String = coffee.getDescription()
    override fun cost(): Double = coffee.cost()
}

// --- 具体装饰器：加牛奶 ---
class MilkDecorator(coffee: Coffee) : CoffeeDecorator(coffee) {
    override fun getDescription(): String = "${super.getDescription()}，加牛奶"
    override fun cost(): Double = super.cost() + 3.0
}

// --- 具体装饰器：加糖 ---
class SugarDecorator(coffee: Coffee) : CoffeeDecorator(coffee) {
    override fun getDescription(): String = "${super.getDescription()}，加糖"
    override fun cost(): Double = super.cost() + 1.0
}

// --- 具体装饰器：加奶泡 ---
class WhipDecorator(coffee: Coffee) : CoffeeDecorator(coffee) {
    override fun getDescription(): String = "${super.getDescription()}，加奶泡"
    override fun cost(): Double = super.cost() + 2.5
}

// --- 具体装饰器：加浓缩 ---
class EspressoShotDecorator(coffee: Coffee) : CoffeeDecorator(coffee) {
    override fun getDescription(): String = "${super.getDescription()}，加一份浓缩"
    override fun cost(): Double = super.cost() + 5.0
}

fun decoratorPatternDemo() {
    println("\n=== 6. 装饰器模式 ===")

    // 简单咖啡
    val simpleCoffee: Coffee = SimpleCoffee()
    println("简单咖啡：")
    println("  ${simpleCoffee.getDescription()}")
    println("  价格：¥${simpleCoffee.cost()}")

    // 加牛奶
    val milkCoffee = MilkDecorator(SimpleCoffee())
    println("\n牛奶咖啡：")
    println("  ${milkCoffee.getDescription()}")
    println("  价格：¥${milkCoffee.cost()}")

    // 加牛奶+糖
    val milkSugarCoffee = SugarDecorator(MilkDecorator(SimpleCoffee()))
    println("\n牛奶加糖咖啡：")
    println("  ${milkSugarCoffee.getDescription()}")
    println("  价格：¥${milkSugarCoffee.cost()}")

    // 豪华版：全加
    val deluxeCoffee = WhipDecorator(
        EspressoShotDecorator(
            MilkDecorator(
                SugarDecorator(SimpleCoffee())
            )
        )
    )
    println("\n豪华咖啡（全加）：")
    println("  ${deluxeCoffee.getDescription()}")
    println("  价格：¥${deluxeCoffee.cost()}")

    // 使用扩展函数风格的装饰器（更Kotlin化）
    fun Coffee.withMilk() = MilkDecorator(this)
    fun Coffee.withSugar() = SugarDecorator(this)
    fun Coffee.withWhip() = WhipDecorator(this)
    fun Coffee.withEspressoShot() = EspressoShotDecorator(this)

    // 链式调用，更优雅
    val myCoffee = SimpleCoffee()
        .withMilk()
        .withSugar()
        .withWhip()

    println("\n链式调用装饰：")
    println("  ${myCoffee.getDescription()}")
    println("  价格：¥${myCoffee.cost()}")

    println("\n装饰器模式要点：")
    println("  - 动态地给对象添加功能，比继承更灵活")
    println("  - 可以任意组合装饰器，实现不同的功能搭配")
    println("  - 装饰器和被装饰对象实现相同的接口，透明使用")
    println("  - Kotlin中可以用扩展函数+链式调用简化装饰器的使用")
}

// ============================================================
// 7. 状态模式
// 允许对象在内部状态改变时改变它的行为，对象看起来好像修改了它的类
// ============================================================

// --- 状态接口 ---
interface PlayerState {
    fun play(context: AudioPlayer)   // 播放
    fun pause(context: AudioPlayer)  // 暂停
    fun stop(context: AudioPlayer)   // 停止
    fun getStateName(): String       // 获取状态名称
}

// --- 上下文：音频播放器 ---
class AudioPlayer {
    // 当前状态
    var state: PlayerState = StoppedState()
        private set

    // 当前播放的歌曲
    var currentTrack: String = ""

    // 切换状态
    fun changeState(newState: PlayerState) {
        println("  [状态切换] ${state.getStateName()} -> ${newState.getStateName()}")
        state = newState
    }

    // 操作委托给当前状态对象
    fun play() {
        println("  按下播放键...")
        state.play(this)
    }

    fun pause() {
        println("  按下暂停键...")
        state.pause(this)
    }

    fun stop() {
        println("  按下停止键...")
        state.stop(this)
    }
}

// --- 具体状态：停止状态 ---
class StoppedState : PlayerState {
    override fun play(context: AudioPlayer) {
        println("  开始播放：${context.currentTrack}")
        context.changeState(PlayingState())
    }

    override fun pause(context: AudioPlayer) {
        println("  已经停止了，无法暂停")
    }

    override fun stop(context: AudioPlayer) {
        println("  已经是停止状态")
    }

    override fun getStateName(): String = "停止"
}

// --- 具体状态：播放状态 ---
class PlayingState : PlayerState {
    override fun play(context: AudioPlayer) {
        println("  已经在播放了")
    }

    override fun pause(context: AudioPlayer) {
        println("  暂停播放")
        context.changeState(PausedState())
    }

    override fun stop(context: AudioPlayer) {
        println("  停止播放")
        context.changeState(StoppedState())
    }

    override fun getStateName(): String = "播放中"
}

// --- 具体状态：暂停状态 ---
class PausedState : PlayerState {
    override fun play(context: AudioPlayer) {
        println("  继续播放：${context.currentTrack}")
        context.changeState(PlayingState())
    }

    override fun pause(context: AudioPlayer) {
        println("  已经是暂停状态")
    }

    override fun stop(context: AudioPlayer) {
        println("  停止播放")
        context.changeState(StoppedState())
    }

    override fun getStateName(): String = "已暂停"
}

fun statePatternDemo() {
    println("\n=== 7. 状态模式 ===")

    // 创建音频播放器
    val player = AudioPlayer()
    player.currentTrack = "Kotlin学习之歌"

    println("初始状态：${player.state.getStateName()}")
    println("当前歌曲：${player.currentTrack}")

    // 测试各种操作
    println("\n--- 操作序列 1 ---")
    player.play()    // 停止 -> 播放
    player.pause()   // 播放 -> 暂停
    player.play()    // 暂停 -> 播放
    player.stop()    // 播放 -> 停止

    println("\n--- 操作序列 2 ---")
    player.pause()   // 停止状态下按暂停（无效）
    player.stop()    // 停止状态下按停止（无效）
    player.play()    // 停止 -> 播放
    player.play()    // 播放状态下按播放（无效）
    player.stop()    // 播放 -> 停止

    // 换一首歌
    println("\n--- 换一首歌 ---")
    player.currentTrack = "协程进行曲"
    println("当前歌曲：${player.currentTrack}")
    player.play()

    println("\n状态模式要点：")
    println("  - 将状态封装为独立的类，每个类处理自己的行为")
    println("  - 上下文对象将操作委托给当前状态对象")
    println("  - 状态切换时，上下文对象改变其内部状态对象")
    println("  - 避免了大量的if-else或switch-case，更符合开闭原则")
    println("  - Kotlin中也可以用sealed class + when表达式实现简化版状态模式")
}

// ============================================================
// 主演示函数
// ============================================================
fun runPatternsDemo() {
    println("============================================================")
    println("                  Kotlin 设计模式演示")
    println("============================================================")

    singletonPatternDemo()    // 单例模式
    factoryPatternDemo()      // 工厂模式
    observerPatternDemo()     // 观察者模式
    strategyPatternDemo()     // 策略模式
    builderPatternDemo()      // 建造者模式
    decoratorPatternDemo()    // 装饰器模式
    statePatternDemo()        // 状态模式

    println("\n============================================================")
    println("                  设计模式演示结束")
    println("============================================================")
}
