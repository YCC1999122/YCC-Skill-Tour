# -*- coding: utf-8 -*-
"""
================================================================================
文件名：classes.py
所属模块：oop（面向对象编程）
功能说明：本文件演示 Python 面向对象编程的核心概念，包括类定义、封装、继承、
         多态、魔术方法、数据类等。每个概念都配有详细的中文注释和示例。

运行方式：在项目根目录下执行 `python -m oop.classes`
================================================================================
"""

from dataclasses import dataclass  # 导入 dataclass 装饰器，用于创建数据类


# ==============================================================================
# 第一部分：类的基本定义
# ==============================================================================
# 类（Class）是创建对象的蓝图/模板，定义了对象的属性（数据）和方法（行为）
# 对象（Object/Instance）是类的具体实例
#
# Python 中使用 class 关键字定义类
# 类名遵循 PascalCase 命名规范（每个单词首字母大写）
# ==============================================================================

# ---------- 1. 最简单的类 ----------
class SimpleClass:
    """
    类名：SimpleClass
    功能：一个最简单的类，演示类的基本结构
    说明：这是一个空类，只有文档字符串，没有自定义属性和方法。
         但即使是空类，也可以创建实例，并且可以动态添加属性。
    """
    pass  # pass 表示空语句，占位用


# ---------- 2. 带有属性和方法的类 ----------
class Person:
    """
    类名：Person
    功能：人类，演示类的属性和方法
    属性（数据成员）：
        name - 姓名
        age - 年龄
    方法（成员函数）：
        __init__ - 构造函数，创建对象时自动调用
        greet - 打招呼方法
        introduce - 自我介绍方法
    """

    # 类属性：属于整个类，所有实例共享同一份
    # 在类定义中直接定义的变量就是类属性
    species = "智人"  # 物种，所有人类都是智人
    total_count = 0   # 总人数，记录创建了多少个 Person 实例

    def __init__(self, name, age):
        """
        方法名：__init__
        功能：构造函数（初始化方法），创建对象时自动调用
        参数：
            self - 指向新创建的实例本身（必须是第一个参数）
            name - 姓名参数
            age - 年龄参数
        返回值：无
        原理：当使用 Person("张三", 25) 创建对象时，Python 会：
              1. 创建一个新的空对象
              2. 自动调用 __init__ 方法，将新对象作为 self 传入
              3. 在 __init__ 中给对象添加属性并赋值
        注意：self 不是关键字，只是约定俗成的名字，可以换成其他名字，
              但强烈建议使用 self 以保持代码可读性。
        """
        # 实例属性：每个实例都有自己独立的一份
        # self.name 表示给当前实例添加一个 name 属性
        self.name = name  # 将参数 name 赋值给实例的 name 属性
        self.age = age    # 将参数 age 赋值给实例的 age 属性

        # 每次创建实例时，类属性 total_count 加 1
        # 类属性通过 类名.属性名 访问
        Person.total_count += 1

    def greet(self):
        """
        方法名：greet
        功能：实例方法，打印问候语
        参数：
            self - 指向调用该方法的实例对象
        返回值：无
        说明：实例方法的第一个参数必须是 self，代表调用方法的对象本身。
              通过 self 可以访问实例的属性和其他方法。
        """
        # self.name 访问实例自己的 name 属性
        print(f"你好，我叫{self.name}！")

    def introduce(self):
        """
        方法名：introduce
        功能：实例方法，打印完整的自我介绍
        参数：self
        返回值：无
        说明：在实例方法中可以调用其他实例方法（通过 self）。
        """
        self.greet()  # 调用同一个类中的另一个方法
        print(f"我今年{self.age}岁。")

    def __del__(self):
        """
        方法名：__del__
        功能：析构函数，对象被销毁时自动调用
        参数：self
        返回值：无
        说明：析构函数在对象被垃圾回收时调用，用于清理资源。
              注意：Python 有自动垃圾回收机制，不保证 __del__ 何时被调用，
              实际开发中很少使用 __del__，更推荐使用上下文管理器（with语句）。
        """
        # 销毁实例时，类属性 total_count 减 1
        Person.total_count -= 1
        # print(f"{self.name} 对象被销毁了")  # 取消注释可观察析构过程


def demo_basic_class():
    """
    函数名：demo_basic_class
    功能：演示类的基本定义和使用
    参数：无
    返回值：无
    """
    print("=" * 60)
    print("【一、类的基本定义和使用】")
    print("=" * 60)

    # ---------- 创建实例（对象）----------
    # 使用 类名(参数) 创建实例，会自动调用 __init__ 方法
    print("\n1. 创建 Person 实例：")

    p1 = Person("张三", 25)  # 创建第一个 Person 实例
    print(f"   p1 = {p1}")  # 打印对象，默认显示类名和内存地址
    print(f"   p1.name = {p1.name}")  # 访问实例属性
    print(f"   p1.age = {p1.age}")

    p2 = Person("李四", 30)  # 创建第二个 Person 实例
    print(f"   p2.name = {p2.name}")
    print(f"   p2.age = {p2.age}")

    # ---------- 调用实例方法 ----------
    print("\n2. 调用实例方法：")

    print("   p1.greet():")
    p1.greet()  # 调用 p1 的 greet 方法，self 自动指向 p1
    print("   p2.introduce():")
    p2.introduce()  # 调用 p2 的 introduce 方法

    # ---------- 类属性 ----------
    print("\n3. 类属性：")

    # 类属性属于整个类，所有实例共享
    print(f"   Person.species = {Person.species}")  # 通过类名访问
    print(f"   p1.species = {p1.species}")  # 也可以通过实例访问（但不推荐修改）
    print(f"   p2.species = {p2.species}")
    print(f"   Person.total_count = {Person.total_count}（共创建了 {Person.total_count} 个实例）")

    # 修改类属性会影响所有实例
    Person.species = "现代智人"
    print(f"   修改类属性后：")
    print(f"   p1.species = {p1.species}")
    print(f"   p2.species = {p2.species}")

    # ---------- 动态添加属性 ----------
    print("\n4. 动态添加属性（Python 特性）：")
    # Python 是动态语言，可以给实例动态添加属性
    p1.city = "北京"  # 给 p1 动态添加一个 city 属性，p2 没有这个属性
    print(f"   p1.city = {p1.city}")
    # print(f"   p2.city = {p2.city}")  # 这行会报错，p2 没有 city 属性
    print("   （p2 没有 city 属性，因为只给 p1 动态添加了）")

    # 注意：不推荐频繁使用动态属性，会降低代码可读性和可维护性


# ==============================================================================
# 第二部分：封装与 property 装饰器
# ==============================================================================
# 封装（Encapsulation）是面向对象的三大特性之一
# 封装的目的：隐藏内部实现细节，只暴露必要的接口，保证数据的安全性和完整性
#
# Python 没有真正的私有属性/方法，而是通过命名约定来实现：
#   - 单下划线 _xxx：约定为"内部使用"，但外部仍然可以直接访问
#   - 双下划线 __xxx：名称修饰（name mangling），外部不能直接访问
#     （实际上是被改名为 _类名__xxx，仍可以通过这个名字访问，但不推荐）
#
# property 装饰器可以将方法"装饰"成属性，提供 getter/setter 功能
# ==============================================================================

class BankAccount:
    """
    类名：BankAccount
    功能：银行账户类，演示封装和 property 装饰器
    属性：
        _balance - 账户余额（单下划线，约定为私有）
        _account_number - 账号（双下划线，名称修饰）
    设计思路：将余额和账号设为私有，不允许外部直接修改，
             只能通过公开的方法（存款、取款）来操作，保证数据安全。
    """

    def __init__(self, account_number, initial_balance=0):
        """
        构造函数：初始化银行账户
        参数：
            account_number - 账号
            initial_balance - 初始余额，默认为 0
        """
        # 双下划线开头的属性会被名称修饰（name mangling）
        # 实际存储名变为 _BankAccount__account_number
        # 这样外部无法直接通过 account_number 访问
        self.__account_number = account_number

        # 单下划线开头的属性是一种约定，表示"内部使用，不要直接访问"
        # 但实际上外部仍然可以直接访问，只是一种君子约定
        self._balance = initial_balance

    # ---------- property 装饰器 ----------
    # @property 装饰器将方法变成属性的 getter（读取器）
    # 方法名就是属性名，外部可以像访问属性一样访问这个方法
    @property
    def balance(self):
        """
        方法名：balance（被 @property 装饰后成为属性）
        功能：获取账户余额（只读属性的 getter）
        设计思路：余额可以读取，但不能直接修改，
                 只能通过 deposit() 和 withdraw() 方法来改变。
        """
        # 可以在 getter 中添加日志、验证等逻辑
        return self._balance  # 返回私有属性的值

    @property
    def account_number(self):
        """
        方法名：account_number
        功能：获取账号（只读属性）
        """
        # 返回时可以隐藏部分信息，保护隐私
        return "****" + self.__account_number[-4:]

    # 对于可写属性，需要定义 setter
    # 使用 @属性名.setter 装饰器
    # balance.setter 对应上面的 @property balance
    # 这里我们故意不提供 balance 的 setter，让它成为只读属性

    def deposit(self, amount):
        """
        方法名：deposit
        功能：存款方法
        参数：
            amount - 存款金额
        返回值：无
        设计思路：存款必须是正数，通过方法控制可以保证数据有效性。
        """
        if amount <= 0:
            raise ValueError("存款金额必须大于 0")
        self._balance += amount  # 修改私有属性
        print(f"存款成功：{amount} 元，当前余额：{self._balance} 元")

    def withdraw(self, amount):
        """
        方法名：withdraw
        功能：取款方法
        参数：
            amount - 取款金额
        返回值：无
        设计思路：取款金额必须大于0且不超过余额，通过方法控制保证数据有效性。
        """
        if amount <= 0:
            raise ValueError("取款金额必须大于 0")
        if amount > self._balance:
            raise ValueError(f"余额不足！当前余额：{self._balance} 元")
        self._balance -= amount  # 修改私有属性
        print(f"取款成功：{amount} 元，当前余额：{self._balance} 元")


def demo_encapsulation():
    """
    函数名：demo_encapsulation
    功能：演示封装与 property 装饰器的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、封装与 property 装饰器】")
    print("=" * 60)

    print("\n1. 创建银行账户：")
    account = BankAccount("6222021234567890", 1000)

    # 使用 @property 访问，像访问属性一样
    print(f"   账号：{account.account_number}")
    print(f"   余额：{account.balance} 元")

    # 尝试直接修改 balance（会失败，因为没有 setter）
    print("\n2. 尝试直接修改 balance（会报错）：")
    try:
        account.balance = 99999  # 这行代码会抛出 AttributeError
    except AttributeError as e:
        print(f"   错误：{e}")
        print("   （balance 是只读属性，不能直接赋值）")

    # 通过方法来操作余额
    print("\n3. 通过方法操作余额：")
    account.deposit(500)     # 存款 500
    account.withdraw(200)    # 取款 200
    print(f"   最终余额：{account.balance} 元")

    # 尝试非法操作
    print("\n4. 尝试非法操作：")
    try:
        account.deposit(-100)  # 存款负数
    except ValueError as e:
        print(f"   存款负数：{e}")

    try:
        account.withdraw(10000)  # 余额不足
    except ValueError as e:
        print(f"   余额不足：{e}")

    # 关于双下划线和单下划线的说明
    print("\n5. 私有属性的访问：")
    print(f"   _balance（单下划线）可以直接访问（但不推荐）：account._balance = {account._balance}")
    # 双下划线的属性被名称修饰了，不能直接访问
    # print(account.__account_number)  # 这行报错
    # 但实际上可以通过 _类名__属性名 访问（不推荐，破坏封装）
    print(f"   __account_number 被改名为 _BankAccount__account_number")
    print(f"   （不推荐这样访问，但确实可以做到）")


# ==============================================================================
# 第三部分：类方法和静态方法
# ==============================================================================
# 类中的方法分为三种：
#   1. 实例方法（最常用）：第一个参数是 self，只能通过实例调用，可以访问实例属性和类属性
#   2. 类方法：用 @classmethod 装饰，第一个参数是 cls（代表类本身），
#      可以通过类或实例调用，只能访问类属性，不能访问实例属性
#   3. 静态方法：用 @staticmethod 装饰，没有 self 或 cls 参数，
#      可以通过类或实例调用，既不能访问类属性也不能访问实例属性
#      本质上就是一个放在类里的普通函数
# ==============================================================================

class MathUtils:
    """
    类名：MathUtils
    功能：数学工具类，演示类方法和静态方法
    属性：
        PI - 圆周率（类属性）
    设计思路：工具类通常不需要实例化，所有功能通过类方法或静态方法提供。
    """

    # 类属性
    PI = 3.141592653589793

    @classmethod
    def set_pi(cls, value):
        """
        方法名：set_pi
        功能：类方法，修改圆周率的值
        参数：
            cls - 代表类本身（类似实例方法的 self）
            value - 新的圆周率值
        说明：类方法可以访问和修改类属性，但不能访问实例属性
              （因为调用时可能根本没有实例）。
        """
        cls.PI = value  # 通过 cls 修改类属性
        print(f"圆周率已更新为 {cls.PI}")

    @classmethod
    def circle_area(cls, radius):
        """
        方法名：circle_area
        功能：类方法，计算圆的面积
        参数：
            cls - 类本身
            radius - 半径
        返回值：圆的面积
        """
        return cls.PI * radius ** 2  # 使用类属性 PI

    @staticmethod
    def add(a, b):
        """
        方法名：add
        功能：静态方法，两数相加
        参数：
            a - 第一个数
            b - 第二个数
        返回值：a + b
        说明：静态方法不需要 self 或 cls，就是一个普通函数，
              只是因为逻辑上属于这个类，所以放在类里面。
              静态方法不能访问类属性和实例属性。
        """
        return a + b

    @staticmethod
    def is_prime(n):
        """
        方法名：is_prime
        功能：静态方法，判断一个数是否为质数
        参数：
            n - 要判断的整数
        返回值：布尔值，是否为质数
        """
        if n < 2:
            return False
        for i in range(2, int(n ** 0.5) + 1):
            if n % i == 0:
                return False
        return True


def demo_classmethod_staticmethod():
    """
    函数名：demo_classmethod_staticmethod
    功能：演示类方法和静态方法的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、类方法和静态方法】")
    print("=" * 60)

    print("\n1. 类方法：")
    print(f"   MathUtils.PI = {MathUtils.PI}")

    # 通过类调用类方法
    area = MathUtils.circle_area(5)
    print(f"   半径为5的圆面积：{area:.4f}")

    # 类方法可以修改类属性
    MathUtils.set_pi(3.14)
    print(f"   修改后 PI = {MathUtils.PI}")

    # 恢复原始值
    MathUtils.set_pi(3.141592653589793)

    print("\n2. 静态方法：")
    # 静态方法通过类调用
    result = MathUtils.add(10, 20)
    print(f"   MathUtils.add(10, 20) = {result}")

    # 判断质数
    for n in [2, 4, 7, 10, 13]:
        print(f"   {n} 是质数吗？{MathUtils.is_prime(n)}")

    # 类方法和静态方法也可以通过实例调用（但不推荐，容易混淆）
    print("\n3. 通过实例调用（不推荐）：")
    utils = MathUtils()  # 创建实例（其实没必要）
    print(f"   utils.circle_area(3) = {utils.circle_area(3)}")
    print(f"   utils.add(1, 2) = {utils.add(1, 2)}")


# ==============================================================================
# 第四部分：继承
# ==============================================================================
# 继承（Inheritance）是面向对象的三大特性之一
# 子类可以继承父类的属性和方法，并可以添加自己的属性和方法，或者重写父类的方法
# 继承的好处：代码复用、层次化设计、多态的基础
#
# Python 支持单继承和多继承
# 语法：class 子类名(父类名):
#       class 子类名(父类1, 父类2, ...):  # 多继承
#
# super() 函数用于调用父类的方法
# MRO（Method Resolution Order）：方法解析顺序，决定多继承时方法的查找顺序
# ==============================================================================

class Animal:
    """
    类名：Animal
    功能：动物基类（父类）
    属性：
        name - 动物名字
        age - 年龄
    方法：
        eat - 吃东西
        sleep - 睡觉
        make_sound - 发出声音（子类会重写）
    设计思路：Animal 是所有动物的基类，定义了动物共有的属性和行为。
    """

    def __init__(self, name, age):
        """
        构造函数：初始化动物的基本属性
        """
        self.name = name  # 名字
        self.age = age    # 年龄

    def eat(self, food):
        """
        方法名：eat
        功能：吃东西
        """
        print(f"{self.name} 在吃 {food}")

    def sleep(self):
        """
        方法名：sleep
        功能：睡觉
        """
        print(f"{self.name} 正在睡觉...")

    def make_sound(self):
        """
        方法名：make_sound
        功能：发出声音
        说明：基类中定义默认行为，子类可以重写（override）这个方法。
        """
        print(f"{self.name} 发出了声音")


class Dog(Animal):
    """
    类名：Dog
    功能：狗类，继承自 Animal
    继承：Animal
    新增属性：
        breed - 品种
    重写方法：
        make_sound - 狗的叫声是"汪汪汪"
    新增方法：
        fetch - 接飞盘
    设计思路：Dog 继承了 Animal 的所有属性和方法，
             只需要添加狗特有的属性和方法，或者重写不同的行为。
    """

    def __init__(self, name, age, breed):
        """
        构造函数：初始化狗的属性
        参数：
            name - 名字（继承自父类）
            age - 年龄（继承自父类）
            breed - 品种（子类新增）
        """
        # super() 调用父类的 __init__ 方法，初始化继承来的属性
        # super() 返回父类的代理对象，不需要传 self
        super().__init__(name, age)
        self.breed = breed  # 子类特有的属性

    def make_sound(self):
        """
        方法名：make_sound
        功能：重写父类的 make_sound 方法
        说明：狗的叫声是"汪汪汪"，与其他动物不同，所以需要重写。
              这就是多态的基础：同一个方法名，不同的实现。
        """
        print(f"{self.name}（{self.breed}）汪汪汪地叫！")

    def fetch(self):
        """
        方法名：fetch
        功能：接飞盘（狗特有的行为）
        """
        print(f"{self.name} 接住了飞盘！")


class Cat(Animal):
    """
    类名：Cat
    功能：猫类，继承自 Animal
    设计思路：与 Dog 类似，猫有自己的叫声和特有行为。
    """

    def __init__(self, name, age, color="橘色"):
        """
        构造函数：初始化猫的属性
        """
        super().__init__(name, age)
        self.color = color  # 毛色

    def make_sound(self):
        """
        重写方法：猫的叫声是"喵喵喵"
        """
        print(f"{self.name}（{self.color}猫）喵喵喵地叫！")

    def catch_mouse(self):
        """
        特有方法：抓老鼠
        """
        print(f"{self.name} 抓到了一只老鼠！")


# ---------- 多继承示例 ----------
class Flyer:
    """
    类名：Flyer
    功能：飞行能力类（作为一个"混入"类，提供飞行能力）
    说明：这是一个 mixin 类，只提供飞行相关的方法，
         不单独实例化，而是被其他类多继承。
    """

    def fly(self):
        """
        方法名：fly
        功能：飞行
        """
        print(f"{self.name} 正在天空中飞翔！")

    def land(self):
        """
        方法名：land
        功能：降落
        """
        print(f"{self.name} 降落了。")


class Swimmer:
    """
    类名：Swimmer
    功能：游泳能力类
    """

    def swim(self):
        """
        方法名：swim
        功能：游泳
        """
        print(f"{self.name} 正在游泳！")


class Duck(Animal, Flyer, Swimmer):
    """
    类名：Duck
    功能：鸭子类，多继承自 Animal、Flyer、Swimmer
    继承：Animal（动物）、Flyer（会飞）、Swimmer（会游泳）
    设计思路：鸭子既是动物，又会飞，还会游泳，
             通过多继承可以组合多个父类的功能。
             这是 Python 多继承的典型应用（Mixin 模式）。
    """

    def __init__(self, name, age):
        """
        构造函数：初始化鸭子
        """
        super().__init__(name, age)  # 调用 Animal 的构造函数
        # 对于多继承，super() 按照 MRO 顺序调用
        # 如果需要调用多个父类的初始化，可以分别调用
        # Flyer.__init__(self)  # 如果 Flyer 有 __init__
        # Swimmer.__init__(self)  # 如果 Swimmer 有 __init__

    def make_sound(self):
        """
        重写方法：鸭子的叫声是"嘎嘎嘎"
        """
        print(f"{self.name} 嘎嘎嘎地叫！")


def demo_inheritance():
    """
    函数名：demo_inheritance
    功能：演示继承、多继承、super() 和 MRO
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【四、继承与多态】")
    print("=" * 60)

    # ---------- 单继承 ----------
    print("\n1. 单继承演示：")

    print("   创建 Dog 实例：")
    dog = Dog("旺财", 3, "金毛")
    print(f"   名字：{dog.name}, 年龄：{dog.age}, 品种：{dog.breed}")

    # 调用继承自父类的方法
    print("   调用继承的方法：")
    dog.eat("骨头")
    dog.sleep()

    # 调用重写的方法
    print("   调用重写的方法：")
    dog.make_sound()

    # 调用子类特有的方法
    print("   调用子类特有方法：")
    dog.fetch()

    print("\n   创建 Cat 实例：")
    cat = Cat("咪咪", 2, "白色")
    cat.make_sound()
    cat.catch_mouse()

    # ---------- 多态 ----------
    print("\n2. 多态演示：")
    print("   同一个方法名 make_sound()，不同对象有不同的实现：")

    # 创建不同的动物对象
    animals = [
        Dog("旺财", 3, "金毛"),
        Cat("咪咪", 2, "橘色"),
        Animal("普通动物", 1),
    ]

    # 遍历所有动物，调用 make_sound 方法
    # 每个动物的叫声都不一样，但调用方式相同
    for animal in animals:
        animal.make_sound()  # 多态：同样的调用，不同的行为

    # ---------- 鸭子类型（Duck Typing）----------
    # Python 是动态类型语言，不关心对象的类型，只关心对象有没有对应的方法
    # "如果它走起来像鸭子，叫起来像鸭子，那它就是鸭子"
    print("\n3. 鸭子类型（Duck Typing）：")

    class Bird:
        """
        鸟类：和 Animal 没有继承关系，但也有 make_sound 方法
        """
        def __init__(self, name):
            self.name = name

        def make_sound(self):
            print(f"{self.name} 叽叽喳喳地叫！")

    def let_it_speak(animal):
        """
        函数名：let_it_speak
        功能：让一个动物/对象"说话"
        参数：
            animal - 任意对象，只要它有 make_sound 方法
        说明：这个函数不检查参数类型，只要对象有 make_sound 方法就能调用。
              这就是鸭子类型：不关心是什么类型，只关心有没有这个方法。
        """
        animal.make_sound()

    # Bird 不是 Animal 的子类，但它有 make_sound 方法
    # 所以可以传入 let_it_speak 函数
    bird = Bird("小麻雀")
    print("   Bird 不是 Animal 的子类，但也有 make_sound 方法：")
    let_it_speak(bird)  # 可以正常调用
    print("   （鸭子类型：只要有 make_sound 方法，就能用）")

    # ---------- 多继承 ----------
    print("\n4. 多继承演示：")

    duck = Duck("唐老鸭", 5)
    print(f"   创建 Duck 实例：{duck.name}")

    # 来自 Animal 的方法
    print("   来自 Animal：")
    duck.eat("面包")

    # 来自 Flyer 的方法
    print("   来自 Flyer：")
    duck.fly()
    duck.land()

    # 来自 Swimmer 的方法
    print("   来自 Swimmer：")
    duck.swim()

    # 重写的方法
    print("   重写的方法：")
    duck.make_sound()

    # ---------- MRO（方法解析顺序）----------
    print("\n5. MRO（方法解析顺序）：")
    print("   Duck 类的 MRO 顺序：")
    # __mro__ 属性保存了方法解析顺序
    for i, cls in enumerate(Duck.__mro__, 1):
        print(f"     {i}. {cls.__name__}")
    print("   （查找方法时按照这个顺序依次查找）")

    # 也可以使用 mro() 方法
    print(f"   Duck.mro() = {Duck.mro()}")

    # ---------- isinstance 和 issubclass ----------
    print("\n6. isinstance 和 issubclass：")
    print(f"   isinstance(duck, Animal) = {isinstance(duck, Animal)}（duck 是 Animal 的实例吗？）")
    print(f"   isinstance(duck, Flyer) = {isinstance(duck, Flyer)}（duck 是 Flyer 的实例吗？）")
    print(f"   issubclass(Duck, Animal) = {issubclass(Duck, Animal)}（Duck 是 Animal 的子类吗？）")
    print(f"   issubclass(Duck, Flyer) = {issubclass(Duck, Flyer)}（Duck 是 Flyer 的子类吗？）")


# ==============================================================================
# 第五部分：魔术方法
# ==============================================================================
# 魔术方法（Magic Methods / Dunder Methods）是 Python 中以双下划线开头和结尾的特殊方法
# 它们不需要手动调用，而是在特定情况下自动被 Python 调用
# 通过重写魔术方法，可以自定义类的行为，让对象支持运算符、内置函数等
#
# 常见魔术方法：
#   __init__      - 构造函数，创建对象时调用
#   __del__       - 析构函数，对象被销毁时调用
#   __str__       - 转为字符串（给人看的），对应 str() 和 print()
#   __repr__      - 转为字符串（给程序员看的），对应 repr()
#   __len__       - 长度，对应 len()
#   __add__       - 加法，对应 +
#   __eq__        - 相等比较，对应 ==
#   __lt__        - 小于比较，对应 <
#   __getitem__   - 索引访问，对应 obj[key]
#   __setitem__   - 索引赋值，对应 obj[key] = value
#   __call__      - 让对象可以像函数一样调用，对应 obj()
# ==============================================================================

class MyVector:
    """
    类名：MyVector
    功能：自定义向量类，演示各种魔术方法
    属性：
        data - 存储向量数据的列表
    设计思路：通过重写各种魔术方法，让自定义类支持内置函数和运算符，
             使自定义类型的行为更接近 Python 内置类型。
    """

    def __init__(self, *args):
        """
        构造函数：初始化向量
        参数：
            *args - 可变参数，向量的各个分量
        示例：
            v = MyVector(1, 2, 3)  # 创建三维向量 (1, 2, 3)
        """
        # 将传入的参数保存为列表
        self.data = list(args)

    def __str__(self):
        """
        方法名：__str__
        功能：返回对象的"友好"字符串表示
        返回值：字符串
        调用时机：str(obj)、print(obj) 时自动调用
        设计思路：给用户看的表示，简洁易读。
        """
        return f"向量({', '.join(str(x) for x in self.data)})"

    def __repr__(self):
        """
        方法名：__repr__
        功能：返回对象的"官方"字符串表示
        返回值：字符串
        调用时机：repr(obj)、交互式环境中直接输入对象时调用
        设计思路：给开发者看的表示，应该尽可能包含重建对象所需的信息。
                  理想情况下，eval(repr(obj)) == obj
        """
        return f"MyVector({', '.join(str(x) for x in self.data)})"

    def __len__(self):
        """
        方法名：__len__
        功能：返回向量的维度（长度）
        返回值：整数
        调用时机：len(obj) 时自动调用
        """
        return len(self.data)

    def __add__(self, other):
        """
        方法名：__add__
        功能：实现向量加法（+ 运算符）
        参数：
            other - 另一个向量
        返回值：新的向量（两向量相加的结果）
        调用时机：a + b 时，自动调用 a.__add__(b)
        """
        # 检查另一个操作数是否也是 MyVector 类型
        if not isinstance(other, MyVector):
            raise TypeError("只能和另一个向量相加")
        # 检查维度是否相同
        if len(self) != len(other):
            raise ValueError("两个向量维度不同，不能相加")
        # 对应分量相加，生成新向量
        result_data = [a + b for a, b in zip(self.data, other.data)]
        return MyVector(*result_data)  # 解包列表，创建新向量

    def __eq__(self, other):
        """
        方法名：__eq__
        功能：判断两个向量是否相等（== 运算符）
        参数：
            other - 另一个对象
        返回值：布尔值
        调用时机：a == b 时，自动调用 a.__eq__(b)
        """
        # 类型不同，直接不相等
        if not isinstance(other, MyVector):
            return False
        # 维度不同，直接不相等
        if len(self) != len(other):
            return False
        # 每个分量都相等才相等
        return self.data == other.data

    def __getitem__(self, index):
        """
        方法名：__getitem__
        功能：通过索引访问向量元素
        参数：
            index - 索引
        返回值：对应位置的元素
        调用时机：obj[index] 时自动调用
        """
        return self.data[index]

    def __setitem__(self, index, value):
        """
        方法名：__setitem__
        功能：通过索引修改向量元素
        参数：
            index - 索引
            value - 新值
        返回值：无
        调用时机：obj[index] = value 时自动调用
        """
        self.data[index] = value

    def __call__(self, scalar):
        """
        方法名：__call__
        功能：让对象可以像函数一样调用
        参数：
            scalar - 标量值
        返回值：标量乘法后的新向量
        调用时机：obj(scalar) 时自动调用
        设计思路：这里实现标量乘法，即向量的每个元素乘以一个数。
        """
        result_data = [x * scalar for x in self.data]
        return MyVector(*result_data)


def demo_magic_methods():
    """
    函数名：demo_magic_methods
    功能：演示魔术方法的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【五、魔术方法】")
    print("=" * 60)

    print("\n1. 创建向量：")
    v1 = MyVector(1, 2, 3)
    v2 = MyVector(4, 5, 6)

    # __str__ 和 __repr__
    print(f"   print(v1) 调用 __str__：{v1}")
    print(f"   repr(v1) 调用 __repr__：{repr(v1)}")

    # __len__
    print(f"\n2. len(v1) 调用 __len__：{len(v1)}（向量维度）")

    # __add__
    print(f"\n3. 向量加法（+）调用 __add__：")
    v3 = v1 + v2
    print(f"   {v1} + {v2} = {v3}")

    # __eq__
    print(f"\n4. 相等比较（==）调用 __eq__：")
    v4 = MyVector(1, 2, 3)
    print(f"   v1 == v4 ? {v1 == v4}")  # True，分量相同
    print(f"   v1 == v2 ? {v1 == v2}")  # False，分量不同

    # __getitem__ 和 __setitem__
    print(f"\n5. 索引访问和赋值：")
    print(f"   v1[0] = {v1[0]}（__getitem__）")
    print(f"   v1[1] = {v1[1]}")
    v1[0] = 10  # __setitem__
    print(f"   v1[0] = 10 后，v1 = {v1}")

    # __call__
    print(f"\n6. 对象调用（__call__）：")
    v5 = v2(2)  # 把对象当函数调用，相当于 v2.__call__(2)
    print(f"   v2(2) = {v5}（标量乘法，每个元素乘2）")


# ==============================================================================
# 第六部分：数据类（@dataclass）
# ==============================================================================
# Python 3.7 引入的 dataclass 装饰器，可以自动生成常用的特殊方法
# 如 __init__、__repr__、__eq__ 等，减少样板代码
# 特别适合主要用于存储数据的类（数据类）
#
# 用法：在类定义前加上 @dataclass
# 类属性会自动变成实例属性，并自动生成 __init__、__repr__、__eq__ 等方法
# ==============================================================================

@dataclass
class Student:
    """
    类名：Student
    功能：学生数据类，演示 @dataclass 装饰器
    属性：
        name - 姓名（str 类型）
        age - 年龄（int 类型）
        grade - 年级（str 类型）
        score - 分数（float 类型，默认值 0.0）
    说明：使用 @dataclass 装饰器后，Python 会自动生成：
          - __init__：接收所有类属性作为参数
          - __repr__：返回清晰的字符串表示
          - __eq__：比较所有属性是否相等
          大大减少了样板代码。
    """
    # 直接在类中定义带类型注解的属性
    # dataclass 会自动将这些变成实例属性，并生成 __init__ 等方法
    name: str           # 姓名，字符串类型
    age: int            # 年龄，整数类型
    grade: str          # 年级，字符串类型
    score: float = 0.0  # 分数，浮点数类型，默认值 0.0

    # 可以在数据类中添加自己的方法
    def get_grade_level(self):
        """
        方法名：get_grade_level
        功能：根据分数返回等级
        返回值：等级字符串
        """
        if self.score >= 90:
            return "A"
        elif self.score >= 80:
            return "B"
        elif self.score >= 60:
            return "C"
        else:
            return "D"


@dataclass
class Point:
    """
    类名：Point
    功能：点坐标数据类
    属性：
        x - x 坐标
        y - y 坐标
    """
    x: float
    y: float

    def distance_to(self, other):
        """
        方法名：distance_to
        功能：计算到另一个点的距离
        参数：
            other - 另一个 Point 对象
        返回值：距离（浮点数）
        """
        import math
        return math.sqrt((self.x - other.x) ** 2 + (self.y - other.y) ** 2)


def demo_dataclass():
    """
    函数名：demo_dataclass
    功能：演示 @dataclass 装饰器的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【六、数据类（@dataclass）】")
    print("=" * 60)

    print("\n1. Student 数据类：")

    # dataclass 自动生成的 __init__
    s1 = Student("小明", 18, "高三", 95.5)
    s2 = Student("小红", 17, "高二", 88.0)
    s3 = Student("小刚", 18, "高三")  # 使用默认的 score=0.0

    # dataclass 自动生成的 __repr__
    print(f"   s1 = {s1}")
    print(f"   s2 = {s2}")
    print(f"   s3 = {s3}")

    # dataclass 自动生成的 __eq__
    s4 = Student("小明", 18, "高三", 95.5)
    print(f"\n   s1 == s4 ? {s1 == s4}（所有属性都相等）")

    # 调用自定义方法
    print(f"\n   {s1.name} 的等级：{s1.get_grade_level()}")
    print(f"   {s2.name} 的等级：{s2.get_grade_level()}")
    print(f"   {s3.name} 的等级：{s3.get_grade_level()}")

    # Point 数据类示例
    print("\n2. Point 数据类：")
    p1 = Point(0, 0)
    p2 = Point(3, 4)
    print(f"   p1 = {p1}")
    print(f"   p2 = {p2}")
    distance = p1.distance_to(p2)
    print(f"   p1 到 p2 的距离：{distance}")


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
    print("  Python 面向对象编程演示")
    print("=" * 60)

    demo_basic_class()               # 类的基本定义
    demo_encapsulation()             # 封装与 property
    demo_classmethod_staticmethod()  # 类方法和静态方法
    demo_inheritance()               # 继承与多态
    demo_magic_methods()             # 魔术方法
    demo_dataclass()                 # 数据类

    print("\n" + "=" * 60)
    print("  面向对象编程演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
