# -*- coding: utf-8 -*-
"""
================================================================================
文件名：design_patterns.py
所属模块：patterns（设计模式）
功能说明：本文件演示常见的设计模式在 Python 中的实现，
         包括单例模式、工厂模式、观察者模式、策略模式、适配器模式。

设计模式（Design Pattern）是解决常见软件设计问题的最佳实践，
是前人经验的总结。合理使用设计模式可以提高代码的可维护性、可扩展性和可复用性。

运行方式：在项目根目录下执行 `python -m patterns.design_patterns`
================================================================================
"""

import functools  # 导入 functools 模块，提供 wraps 等工具
import abc        # 导入 abc 模块，提供抽象基类支持


# ==============================================================================
# 第一部分：单例模式（Singleton）
# ==============================================================================
# 单例模式：确保一个类只有一个实例，并提供一个全局访问点
# 应用场景：配置管理、日志记录器、数据库连接池、线程池等
#
# Python 中实现单例的方式有很多种：
#   1. 模块级单例（Python 模块本身就是单例）
#   2. 装饰器实现
#   3. 元类（metaclass）实现
#   4. __new__ 方法实现
# ==============================================================================

# ---------- 方式一：装饰器实现 ----------
def singleton_decorator(cls):
    """
    函数名：singleton_decorator
    功能：单例装饰器（用装饰器实现单例模式）
    参数：
        cls - 要变成单例的类
    返回值：包装后的类（实际是一个函数，调用时返回单例）
    设计思路：用闭包保存实例，第一次调用时创建实例，
             之后每次调用都返回同一个实例。
    """
    instances = {}  # 保存类和实例的映射（用字典是因为装饰器可以装饰多个类）

    @functools.wraps(cls)
    def get_instance(*args, **kwargs):
        """
        获取单例实例的函数
        如果实例不存在就创建，存在就直接返回
        """
        if cls not in instances:
            # 实例不存在，创建新实例
            instances[cls] = cls(*args, **kwargs)
        # 返回已存在的实例
        return instances[cls]

    return get_instance


@singleton_decorator
class AppConfig:
    """
    类名：AppConfig
    功能：应用配置类（单例，用装饰器实现）
    说明：整个应用中只有一份配置，所有地方共享同一个配置对象。
    """

    def __init__(self):
        """
        构造函数：初始化配置
        注意：因为是单例，__init__ 只会在第一次创建时调用
        """
        print("   [AppConfig] 初始化配置（只会执行一次）")
        self.app_name = "Python学习项目"
        self.version = "1.0.0"
        self.debug_mode = True
        self.database_url = "mysql://localhost:3306/mydb"

    def show_config(self):
        """显示当前配置"""
        print(f"   应用名：{self.app_name}")
        print(f"   版本：{self.version}")
        print(f"   调试模式：{'开启' if self.debug_mode else '关闭'}")
        print(f"   数据库地址：{self.database_url}")


# ---------- 方式二：元类实现 ----------
class SingletonMeta(type):
    """
    类名：SingletonMeta
    功能：单例元类（用元类实现单例模式）
    继承：type（所有类的元类）
    设计思路：元类控制类的创建过程。
             通过重写 __call__ 方法，在类被实例化时检查是否已有实例，
             有就直接返回，没有才创建。
    """

    # 保存所有单例类的实例
    _instances = {}

    def __call__(cls, *args, **kwargs):
        """
        方法名：__call__
        功能：当类被调用（即创建实例）时触发
        参数：
            cls - 类本身
            *args, **kwargs - 构造参数
        返回值：单例实例
        说明：type 的 __call__ 方法会调用 __new__ 和 __init__ 来创建实例。
              我们重写 __call__，在创建前检查是否已有实例。
        """
        if cls not in cls._instances:
            # 实例不存在，调用父类（type）的 __call__ 创建实例
            cls._instances[cls] = super().__call__(*args, **kwargs)
        # 返回已存在的实例
        return cls._instances[cls]


class Logger(metaclass=SingletonMeta):
    """
    类名：Logger
    功能：日志记录器类（单例，用元类实现）
    说明：整个应用中只有一个日志记录器，统一管理日志输出。
    """

    def __init__(self):
        """构造函数：初始化日志记录器"""
        print("   [Logger] 初始化日志记录器（只会执行一次）")
        self.log_count = 0  # 日志计数

    def log(self, message):
        """
        方法名：log
        功能：记录一条日志
        参数：
            message - 日志消息
        """
        self.log_count += 1
        print(f"   [日志 #{self.log_count}] {message}")


def demo_singleton():
    """
    函数名：demo_singleton
    功能：演示单例模式
    参数：无
    返回值：无
    """
    print("=" * 60)
    print("【一、单例模式（Singleton）】")
    print("=" * 60)

    # ---------- 装饰器实现的单例 ----------
    print("\n1. 装饰器实现（AppConfig）：")

    # 创建第一个"实例"
    print("   第一次获取 AppConfig 实例：")
    config1 = AppConfig()
    config1.show_config()

    # 创建第二个"实例"
    print("\n   第二次获取 AppConfig 实例：")
    config2 = AppConfig()

    # 验证是否是同一个对象
    print(f"\n   config1 is config2 ? {config1 is config2}")
    print(f"   id(config1) = {id(config1)}")
    print(f"   id(config2) = {id(config2)}")

    # 修改配置，两个引用都会看到变化（因为是同一个对象）
    config1.debug_mode = False
    print(f"\n   修改 config1.debug_mode = False 后：")
    print(f"   config2.debug_mode = {config2.debug_mode}（也变了，因为是同一个对象）")

    # ---------- 元类实现的单例 ----------
    print("\n2. 元类实现（Logger）：")

    print("   第一次获取 Logger 实例：")
    logger1 = Logger()
    logger1.log("应用启动")
    logger1.log("加载配置")

    print("\n   第二次获取 Logger 实例：")
    logger2 = Logger()
    logger2.log("用户登录")

    print(f"\n   logger1 is logger2 ? {logger1 is logger2}")
    print(f"   总日志数：{logger1.log_count}（两个引用共享同一个计数器）")


# ==============================================================================
# 第二部分：工厂模式（Factory）
# ==============================================================================
# 工厂模式：定义一个创建对象的接口，让子类决定实例化哪个类
# 核心思想：将对象的创建和使用分离，降低耦合
#
# 常见类型：
#   - 简单工厂（Simple Factory）：一个工厂类根据参数创建不同产品
#   - 工厂方法（Factory Method）：每个产品对应一个工厂类
#   - 抽象工厂（Abstract Factory）：创建一系列相关的产品
#
# 这里演示简单工厂和工厂方法
# ==============================================================================

# ---------- 产品类 ----------
class Shape(abc.ABC):
    """
    类名：Shape
    功能：形状抽象基类
    说明：所有形状的基类，定义了形状的共同接口 draw()。
          使用 abc.ABC 和 @abstractmethod 定义抽象类。
    """

    @abc.abstractmethod
    def draw(self):
        """
        方法名：draw
        功能：绘制形状（抽象方法，子类必须实现）
        """
        pass

    @abc.abstractmethod
    def area(self):
        """
        方法名：area
        功能：计算面积（抽象方法）
        """
        pass


class Circle(Shape):
    """
    类名：Circle
    功能：圆形类
    继承：Shape
    """

    def __init__(self, radius):
        self.radius = radius  # 半径

    def draw(self):
        print(f"   绘制圆形，半径：{self.radius}")

    def area(self):
        import math
        return math.pi * self.radius ** 2


class Rectangle(Shape):
    """
    类名：Rectangle
    功能：矩形类
    继承：Shape
    """

    def __init__(self, width, height):
        self.width = width    # 宽
        self.height = height  # 高

    def draw(self):
        print(f"   绘制矩形，宽：{self.width}，高：{self.height}")

    def area(self):
        return self.width * self.height


class Triangle(Shape):
    """
    类名：Triangle
    功能：三角形类
    继承：Shape
    """

    def __init__(self, base, height):
        self.base = base      # 底
        self.height = height  # 高

    def draw(self):
        print(f"   绘制三角形，底：{self.base}，高：{self.height}")

    def area(self):
        return 0.5 * self.base * self.height


# ---------- 简单工厂 ----------
class ShapeFactory:
    """
    类名：ShapeFactory
    功能：简单工厂类
    设计思路：根据传入的类型名称，创建对应的形状对象。
             客户端只需要知道类型名称，不需要知道具体的类名。
    缺点：新增产品时需要修改工厂类，违反开闭原则。
    """

    @staticmethod
    def create_shape(shape_type, **kwargs):
        """
        方法名：create_shape
        功能：创建形状对象
        参数：
            shape_type - 形状类型字符串（'circle', 'rectangle', 'triangle'）
            **kwargs - 形状的参数
        返回值：Shape 的子类实例
        """
        if shape_type == "circle":
            return Circle(**kwargs)
        elif shape_type == "rectangle":
            return Rectangle(**kwargs)
        elif shape_type == "triangle":
            return Triangle(**kwargs)
        else:
            raise ValueError(f"不支持的形状类型：{shape_type}")


# ---------- 工厂方法 ----------
# 每个产品对应一个工厂类，新增产品时只需新增工厂类，符合开闭原则

class ShapeFactoryBase(abc.ABC):
    """
    类名：ShapeFactoryBase
    功能：形状工厂基类（抽象工厂）
    """

    @abc.abstractmethod
    def create(self, **kwargs):
        """创建形状对象（抽象方法）"""
        pass


class CircleFactory(ShapeFactoryBase):
    """圆形工厂"""
    def create(self, **kwargs):
        return Circle(**kwargs)


class RectangleFactory(ShapeFactoryBase):
    """矩形工厂"""
    def create(self, **kwargs):
        return Rectangle(**kwargs)


class TriangleFactory(ShapeFactoryBase):
    """三角形工厂"""
    def create(self, **kwargs):
        return Triangle(**kwargs)


def demo_factory():
    """
    函数名：demo_factory
    功能：演示工厂模式
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、工厂模式（Factory）】")
    print("=" * 60)

    # ---------- 简单工厂 ----------
    print("\n1. 简单工厂：")

    print("   创建圆形：")
    circle = ShapeFactory.create_shape("circle", radius=5)
    circle.draw()
    print(f"   面积：{circle.area():.2f}")

    print("\n   创建矩形：")
    rectangle = ShapeFactory.create_shape("rectangle", width=4, height=6)
    rectangle.draw()
    print(f"   面积：{rectangle.area():.2f}")

    print("\n   创建三角形：")
    triangle = ShapeFactory.create_shape("triangle", base=3, height=4)
    triangle.draw()
    print(f"   面积：{triangle.area():.2f}")

    # ---------- 工厂方法 ----------
    print("\n2. 工厂方法：")

    # 创建工厂实例
    factories = {
        "circle": CircleFactory(),
        "rectangle": RectangleFactory(),
        "triangle": TriangleFactory(),
    }

    # 使用工厂创建产品
    print("   使用工厂方法创建各种形状：")
    for name, factory in factories.items():
        shape = factory.create(radius=5) if name == "circle" else \
                factory.create(width=4, height=6) if name == "rectangle" else \
                factory.create(base=3, height=4)
        shape.draw()


# ==============================================================================
# 第三部分：观察者模式（Observer）
# ==============================================================================
# 观察者模式：定义对象间一对多的依赖关系，当一个对象状态改变时，
# 所有依赖它的对象都会收到通知并自动更新
#
# 别名：发布-订阅模式（Publish-Subscribe）
# 组成角色：
#   - 主题（Subject）：被观察的对象，维护观察者列表，状态变化时通知观察者
#   - 观察者（Observer）：观察主题的对象，收到通知后更新自己
#
# 应用场景：事件监听、消息通知、MVC 模式中的 Model 和 View 等
# ==============================================================================

class Subject:
    """
    类名：Subject
    功能：主题（被观察者）基类
    属性：
        _observers - 观察者列表
    设计思路：维护观察者列表，提供注册、移除、通知方法。
    """

    def __init__(self):
        """构造函数：初始化观察者列表"""
        self._observers = []  # 观察者列表

    def attach(self, observer):
        """
        方法名：attach
        功能：注册（添加）观察者
        参数：
            observer - 观察者对象（必须有 update 方法）
        """
        if observer not in self._observers:
            self._observers.append(observer)
            print(f"   [主题] 添加观察者：{observer.name}")

    def detach(self, observer):
        """
        方法名：detach
        功能：移除观察者
        参数：
            observer - 要移除的观察者
        """
        if observer in self._observers:
            self._observers.remove(observer)
            print(f"   [主题] 移除观察者：{observer.name}")

    def notify(self, data=None):
        """
        方法名：notify
        功能：通知所有观察者（调用每个观察者的 update 方法）
        参数：
            data - 要传递给观察者的数据
        """
        print(f"   [主题] 通知所有观察者，数据：{data}")
        for observer in self._observers:
            observer.update(data)


class Observer:
    """
    类名：Observer
    功能：观察者基类
    属性：
        name - 观察者名称
    说明：具体观察者需要继承此类并实现 update 方法。
    """

    def __init__(self, name):
        self.name = name  # 观察者名称

    def update(self, data):
        """
        方法名：update
        功能：接收主题通知时调用，子类必须重写
        参数：
            data - 主题传递的数据
        """
        pass


class NewsPublisher(Subject):
    """
    类名：NewsPublisher
    功能：新闻发布者（具体主题）
    属性：
        _latest_news - 最新新闻
    设计思路：新闻发布者是被观察的对象，
             当有新新闻发布时，通知所有订阅者。
    """

    def __init__(self):
        super().__init__()
        self._latest_news = ""  # 最新新闻

    def publish_news(self, news):
        """
        方法名：publish_news
        功能：发布新闻（改变状态）
        参数：
            news - 新闻内容
        """
        print(f"\n   [新闻发布者] 发布新闻：{news}")
        self._latest_news = news
        self.notify(news)  # 通知所有观察者


class NewsSubscriber(Observer):
    """
    类名：NewsSubscriber
    功能：新闻订阅者（具体观察者）
    设计思路：订阅者收到新闻通知后，打印收到的新闻。
    """

    def update(self, data):
        """
        收到新闻通知时调用
        """
        print(f"   [{self.name}] 收到新闻：{data}")


class EmailNotifier(Observer):
    """
    类名：EmailNotifier
    功能：邮件通知器（另一种观察者）
    设计思路：收到通知后，模拟发送邮件。
    """

    def update(self, data):
        print(f"   [{self.name}] 发送邮件通知：最新新闻 - {data}")


def demo_observer():
    """
    函数名：demo_observer
    功能：演示观察者模式
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、观察者模式（Observer）】")
    print("=" * 60)

    # 创建主题（新闻发布者）
    publisher = NewsPublisher()

    # 创建观察者（订阅者）
    subscriber1 = NewsSubscriber("小明")
    subscriber2 = NewsSubscriber("小红")
    email_notifier = EmailNotifier("邮件通知系统")

    # 注册观察者
    print("\n注册观察者：")
    publisher.attach(subscriber1)
    publisher.attach(subscriber2)
    publisher.attach(email_notifier)

    # 发布新闻，所有订阅者都会收到通知
    publisher.publish_news("Python 3.12 发布了！")
    publisher.publish_news("AI 技术取得重大突破！")

    # 移除一个观察者
    print("\n移除一个观察者：")
    publisher.detach(subscriber2)

    # 再次发布新闻，被移除的订阅者收不到了
    publisher.publish_news("量子计算实现新突破！")


# ==============================================================================
# 第四部分：策略模式（Strategy）
# ==============================================================================
# 策略模式：定义一系列算法，将每个算法封装起来，使它们可以互相替换
# 核心思想：将算法的使用和算法的实现分离
#
# 组成角色：
#   - 策略（Strategy）：算法的抽象接口
#   - 具体策略（Concrete Strategy）：具体的算法实现
#   - 上下文（Context）：使用策略的对象，持有策略引用
#
# 优点：
#   - 算法可以自由切换
#   - 易于扩展（新增策略不需要修改原有代码）
#   - 避免使用多重条件判断
# ==============================================================================

class SortingStrategy(abc.ABC):
    """
    类名：SortingStrategy
    功能：排序策略抽象基类
    说明：所有排序策略的接口，定义了 sort 方法。
    """

    @abc.abstractmethod
    def sort(self, data):
        """
        方法名：sort
        功能：对数据进行排序
        参数：
            data - 待排序的列表
        返回值：排序后的列表
        """
        pass


class BubbleSortStrategy(SortingStrategy):
    """
    类名：BubbleSortStrategy
    功能：冒泡排序策略（具体策略）
    """

    def sort(self, data):
        """冒泡排序实现"""
        result = data[:]
        n = len(result)
        for i in range(n - 1):
            for j in range(n - 1 - i):
                if result[j] > result[j + 1]:
                    result[j], result[j + 1] = result[j + 1], result[j]
        print(f"   [冒泡排序] 完成排序")
        return result


class QuickSortStrategy(SortingStrategy):
    """
    类名：QuickSortStrategy
    功能：快速排序策略（具体策略）
    """

    def sort(self, data):
        """快速排序实现"""
        if len(data) <= 1:
            return data
        pivot = data[len(data) // 2]
        left = [x for x in data if x < pivot]
        middle = [x for x in data if x == pivot]
        right = [x for x in data if x > pivot]
        print(f"   [快速排序] 完成排序")
        return self.sort(left) + middle + self.sort(right)


class PythonSortStrategy(SortingStrategy):
    """
    类名：PythonSortStrategy
    功能：Python 内置排序策略（具体策略）
    说明：使用 Python 内置的 sorted 函数（Timsort 算法）。
    """

    def sort(self, data):
        """内置排序"""
        print(f"   [Python内置排序] 完成排序")
        return sorted(data)


class Sorter:
    """
    类名：Sorter
    功能：排序器（上下文 Context）
    属性：
        strategy - 当前使用的排序策略
    设计思路：持有一个排序策略对象，排序时委托给策略对象。
             可以在运行时动态切换排序策略。
    """

    def __init__(self, strategy=None):
        """
        构造函数
        参数：
            strategy - 初始排序策略，默认使用 Python 内置排序
        """
        # 默认使用内置排序策略
        self.strategy = strategy if strategy else PythonSortStrategy()

    def set_strategy(self, strategy):
        """
        方法名：set_strategy
        功能：设置（切换）排序策略
        参数：
            strategy - 新的排序策略
        """
        self.strategy = strategy
        print(f"   [排序器] 切换策略为：{type(strategy).__name__}")

    def sort(self, data):
        """
        方法名：sort
        功能：对数据进行排序（委托给策略对象）
        参数：
            data - 待排序数据
        返回值：排序后的列表
        """
        print(f"   [排序器] 开始排序，数据量：{len(data)}")
        return self.strategy.sort(data)


def demo_strategy():
    """
    函数名：demo_strategy
    功能：演示策略模式
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【四、策略模式（Strategy）】")
    print("=" * 60)

    import random
    random.seed(123)
    data = random.sample(range(1, 100), 10)  # 随机生成 10 个数
    print(f"\n原始数据：{data}")

    # 创建排序器（默认使用内置排序）
    sorter = Sorter()

    # 使用默认策略排序
    print("\n1. 默认策略（Python 内置排序）：")
    result = sorter.sort(data)
    print(f"   结果：{result}")

    # 切换到冒泡排序
    print("\n2. 切换到冒泡排序：")
    sorter.set_strategy(BubbleSortStrategy())
    result = sorter.sort(data)
    print(f"   结果：{result}")

    # 切换到快速排序
    print("\n3. 切换到快速排序：")
    sorter.set_strategy(QuickSortStrategy())
    result = sorter.sort(data)
    print(f"   结果：{result}")

    print("\n   （策略模式的好处：可以在运行时动态切换算法，新增算法也不需要修改排序器代码）")


# ==============================================================================
# 第五部分：适配器模式（Adapter）
# ==============================================================================
# 适配器模式：将一个类的接口转换成客户希望的另一个接口
# 使原本接口不兼容的类可以一起工作
#
# 别名：包装器（Wrapper）
# 应用场景：
#   - 使用现有的类，但其接口不符合需求
#   - 整合第三方库，不想修改原有代码
#   - 统一多个不同接口的类的使用方式
#
# 实现方式：
#   - 对象适配器：适配器持有被适配者的引用（组合方式，更灵活）
#   - 类适配器：适配器继承被适配者（继承方式，Python 中用得多继承）
# ==============================================================================

# ---------- 被适配者（Adaptee）：旧版的温度传感器 ----------
class OldTemperatureSensor:
    """
    类名：OldTemperatureSensor
    功能：旧式温度传感器（被适配者）
    说明：假设这是一个旧的第三方库，接口是 get_temperature_f()，
          返回华氏温度。但我们的系统使用摄氏温度和 get_celsius() 接口。
    """

    def __init__(self):
        self._temperature_f = 72.0  # 华氏温度

    def get_temperature_f(self):
        """
        方法名：get_temperature_f
        功能：获取华氏温度
        返回值：华氏温度值
        """
        # 模拟温度波动
        self._temperature_f += (0.5 - __import__('random').random())
        return self._temperature_f

    def set_temperature_f(self, value):
        """设置华氏温度"""
        self._temperature_f = value


# ---------- 目标接口（Target）：我们期望的接口 ----------
class CelsiusSensor(abc.ABC):
    """
    类名：CelsiusSensor
    功能：摄氏温度传感器接口（目标接口）
    说明：我们的系统期望使用摄氏温度的接口。
    """

    @abc.abstractmethod
    def get_celsius(self):
        """获取摄氏温度"""
        pass

    @abc.abstractmethod
    def set_celsius(self, value):
        """设置摄氏温度"""
        pass


# ---------- 适配器（Adapter）----------
class TemperatureAdapter(CelsiusSensor):
    """
    类名：TemperatureAdapter
    功能：温度传感器适配器
    继承：CelsiusSensor（目标接口）
    属性：
        old_sensor - 旧式温度传感器（被适配者）
    设计思路：对象适配器，持有旧传感器的引用，
             将旧接口（华氏温度）转换为新接口（摄氏温度）。
             转换公式：C = (F - 32) * 5/9
                      F = C * 9/5 + 32
    """

    def __init__(self, old_sensor):
        """
        构造函数
        参数：
            old_sensor - 旧式温度传感器对象
        """
        self.old_sensor = old_sensor  # 持有被适配者的引用

    def get_celsius(self):
        """
        方法名：get_celsius
        功能：获取摄氏温度（目标接口）
        原理：调用旧传感器的 get_temperature_f() 获取华氏温度，
              然后转换为摄氏温度返回。
        """
        fahrenheit = self.old_sensor.get_temperature_f()  # 调用旧接口
        celsius = (fahrenheit - 32) * 5 / 9  # 华氏转摄氏
        return round(celsius, 2)  # 保留两位小数

    def set_celsius(self, value):
        """
        方法名：set_celsius
        功能：设置摄氏温度（目标接口）
        """
        fahrenheit = value * 9 / 5 + 32  # 摄氏转华氏
        self.old_sensor.set_temperature_f(fahrenheit)  # 调用旧接口


def demo_adapter():
    """
    函数名：demo_adapter
    功能：演示适配器模式
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【五、适配器模式（Adapter）】")
    print("=" * 60)

    # 创建旧式温度传感器（只有华氏度接口）
    old_sensor = OldTemperatureSensor()
    print(f"旧式传感器（华氏度）：{old_sensor.get_temperature_f():.2f}°F")

    # 直接使用旧接口不方便（我们系统用的是摄氏度）
    # 使用适配器包装一下
    adapter = TemperatureAdapter(old_sensor)
    print(f"\n通过适配器（摄氏度）：")
    print(f"   温度：{adapter.get_celsius()}°C")

    # 设置温度（摄氏）
    adapter.set_celsius(25)
    print(f"\n设置 25°C 后：")
    print(f"   摄氏：{adapter.get_celsius()}°C")
    print(f"   华氏（旧传感器）：{old_sensor.get_temperature_f():.2f}°F")

    # 模拟读取几次温度
    print(f"\n连续读取温度（模拟波动）：")
    for i in range(5):
        temp = adapter.get_celsius()
        print(f"   第 {i+1} 次：{temp}°C")

    print("\n   （适配器模式的好处：不修改旧代码，通过适配器让接口兼容）")


# ==============================================================================
# 主函数：运行所有演示
# ==============================================================================

def run_demo():
    """
    函数名：run_demo
    功能：运行本模块的所有演示函数
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("  Python 设计模式演示")
    print("=" * 60)

    demo_singleton()   # 单例模式
    demo_factory()     # 工厂模式
    demo_observer()    # 观察者模式
    demo_strategy()    # 策略模式
    demo_adapter()     # 适配器模式

    print("\n" + "=" * 60)
    print("  设计模式演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
