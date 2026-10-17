# -*- coding: utf-8 -*-
"""
================================================================================
文件名：decorators.py
所属模块：advanced（高级特性）
功能说明：本文件深入讲解 Python 装饰器的原理和使用，包括函数是一等公民、
         简单装饰器、带参数的装饰器、类装饰器、functools.wraps 等。

运行方式：在项目根目录下执行 `python -m advanced.decorators`
================================================================================
"""

import time          # 导入 time 模块，用于计时
import functools     # 导入 functools 模块，提供 wraps 等工具


# ==============================================================================
# 第一部分：函数是一等公民
# ==============================================================================
# 装饰器的原理基于"函数是一等公民"（First-Class Citizen）这一特性
# 一等公民意味着函数可以：
#   1. 赋值给变量
#   2. 作为参数传递给另一个函数
#   3. 作为另一个函数的返回值
#   4. 在函数内部定义另一个函数（嵌套函数/闭包）
# ==============================================================================

def demo_first_class_functions():
    """
    函数名：demo_first_class_functions
    功能：演示 Python 中函数是一等公民的特性
    参数：无
    返回值：无
    设计思路：通过四个示例分别展示函数作为一等公民的四个核心特性，
             为理解装饰器原理打下基础。
    """
    print("=" * 60)
    print("【一、函数是一等公民（装饰器原理基础）】")
    print("=" * 60)

    # ---------- 1. 函数可以赋值给变量 ----------
    print("\n1. 函数赋值给变量：")

    def greet(name):
        """简单的问候函数"""
        return f"你好，{name}！"

    # 将函数本身赋值给变量 say_hello（不是调用函数的结果）
    # 注意：greet 后面没有括号，表示函数对象本身
    say_hello = greet

    # 通过变量调用函数
    print(f"   greet('小明') = {greet('小明')}")
    print(f"   say_hello('小红') = {say_hello('小红')}")
    print(f"   say_hello 和 greet 是同一个函数吗？{say_hello is greet}")

    # ---------- 2. 函数可以作为参数传递 ----------
    print("\n2. 函数作为参数传递：")

    def add(a, b):
        """加法函数"""
        return a + b

    def multiply(a, b):
        """乘法函数"""
        return a * b

    def calculate(func, x, y):
        """
        计算函数：接收一个函数作为参数
        参数：
            func - 一个二元运算函数
            x, y - 两个操作数
        返回值：func(x, y) 的结果
        """
        return func(x, y)

    # 将不同的函数传入 calculate
    result1 = calculate(add, 3, 5)       # 传入 add 函数
    result2 = calculate(multiply, 3, 5)  # 传入 multiply 函数
    print(f"   calculate(add, 3, 5) = {result1}")
    print(f"   calculate(multiply, 3, 5) = {result2}")

    # ---------- 3. 函数可以作为返回值 ----------
    print("\n3. 函数作为返回值：")

    def create_adder(n):
        """
        创建一个加法器函数
        参数：
            n - 要加的数
        返回值：一个函数，这个函数接收一个参数 x，返回 x + n
        """
        def adder(x):
            """内部函数：将参数 x 和外部的 n 相加"""
            return x + n
        # 将内部函数 adder 作为返回值返回
        return adder

    # 创建两个不同的加法器
    add5 = create_adder(5)    # add5 是一个函数，功能是加 5
    add10 = create_adder(10)  # add10 是一个函数，功能是加 10

    print(f"   add5(3) = {add5(3)}")      # 3 + 5 = 8
    print(f"   add5(10) = {add5(10)}")    # 10 + 5 = 15
    print(f"   add10(3) = {add10(3)}")    # 3 + 10 = 13

    # ---------- 4. 闭包（Closure）----------
    print("\n4. 闭包（Closure）：")

    # 闭包是指：内部函数可以访问外部函数的变量（即使外部函数已经执行完毕）
    # 上面的 adder 函数就是一个闭包，它记住了外部函数 create_adder 的变量 n

    def counter():
        """
        计数器函数：返回一个闭包，每次调用计数加 1
        """
        count = 0  # 外部函数的局部变量

        def increment():
            """内部函数：每次调用将 count 加 1 并返回"""
            nonlocal count  # nonlocal 声明 count 来自外部作用域（不是全局也不是局部）
            count += 1
            return count

        return increment  # 返回内部函数（闭包）

    # 创建两个独立的计数器
    c1 = counter()  # c1 是一个闭包，有自己独立的 count
    c2 = counter()  # c2 是另一个闭包，有自己独立的 count

    print("   计数器 c1：", end="")
    for _ in range(3):
        print(c1(), end=" ")
    print()

    print("   计数器 c2：", end="")
    for _ in range(5):
        print(c2(), end=" ")
    print()
    print("   （两个计数器互不干扰，各有各的 count 变量）")


# ==============================================================================
# 第二部分：简单装饰器
# ==============================================================================
# 装饰器（Decorator）本质上是一个函数，它接收另一个函数作为参数，
# 并返回一个新的函数（通常是在原函数基础上增加了额外功能）。
#
# 装饰器的作用：在不修改原函数代码的前提下，给函数增加新功能
# 常见应用：日志记录、性能计时、权限校验、缓存、参数校验等
#
# 语法糖：使用 @装饰器名 放在函数定义上面
#   @decorator
#   def func():
#       pass
# 等价于：
#   func = decorator(func)
# ==============================================================================

# ---------- 1. 手写装饰器（不用语法糖）----------
def simple_decorator(func):
    """
    函数名：simple_decorator
    功能：一个最简单的装饰器函数
    参数：
        func - 被装饰的函数
    返回值：一个新的包装函数
    设计思路：装饰器接收一个函数，返回一个新函数。
             新函数在调用原函数前后可以添加额外的功能。
    """
    def wrapper():
        """包装函数：在原函数前后添加额外功能"""
        print("   【装饰器】函数调用前的操作...")
        func()  # 调用原函数
        print("   【装饰器】函数调用后的操作...")

    return wrapper  # 返回包装函数


# ---------- 2. 使用 @ 语法糖 ----------
def log_decorator(func):
    """
    函数名：log_decorator
    功能：日志装饰器，在函数调用前后打印日志
    参数：
        func - 被装饰的函数
    返回值：包装函数
    设计思路：这是一个通用的日志装饰器，可以给任意函数添加日志功能。
    """
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        """
        包装函数：
        *args 和 **kwargs 用于接收任意参数，这样装饰器可以用于任何函数
        """
        print(f"   [日志] 开始调用函数：{func.__name__}")
        print(f"   [日志] 参数：args={args}, kwargs={kwargs}")

        result = func(*args, **kwargs)  # 调用原函数，保存返回值

        print(f"   [日志] 函数 {func.__name__} 调用结束，返回值：{result}")
        return result  # 返回原函数的返回值

    return wrapper


# 使用 @ 语法糖应用装饰器
# 等价于：add = log_decorator(add)
@log_decorator
def add(a, b):
    """加法函数（被装饰的）"""
    return a + b


@log_decorator
def greet(name):
    """问候函数（被装饰的）"""
    return f"你好，{name}！"


# ---------- 3. 计时装饰器 ----------
def timer_decorator(func):
    """
    函数名：timer_decorator
    功能：计时装饰器，统计函数的执行时间
    参数：
        func - 被装饰的函数
    返回值：包装函数
    应用场景：性能分析，统计函数运行耗时
    """
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        """包装函数：记录函数执行时间"""
        start_time = time.time()  # 记录开始时间（时间戳，单位秒）

        result = func(*args, **kwargs)  # 执行原函数

        end_time = time.time()  # 记录结束时间
        elapsed = end_time - start_time  # 计算耗时

        print(f"   [计时] 函数 {func.__name__} 执行耗时：{elapsed:.6f} 秒")
        return result

    return wrapper


@timer_decorator
def slow_function():
    """模拟一个耗时的函数"""
    time.sleep(0.5)  # 休眠 0.5 秒，模拟耗时操作
    return "操作完成"


@timer_decorator
def fast_function():
    """模拟一个快速的函数"""
    total = 0
    for i in range(10000):
        total += i
    return total


def demo_simple_decorators():
    """
    函数名：demo_simple_decorators
    功能：演示简单装饰器的原理和使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、简单装饰器】")
    print("=" * 60)

    # ---------- 1. 手动使用装饰器（不用 @ 语法糖）----------
    print("\n1. 手动使用装饰器：")

    def hello():
        """简单的 hello 函数"""
        print("   Hello, World!")

    # 手动装饰：将 hello 函数传入装饰器，得到包装后的新函数
    decorated_hello = simple_decorator(hello)
    print("   调用装饰后的函数：")
    decorated_hello()

    # ---------- 2. 使用 @ 语法糖 ----------
    print("\n2. 使用 @ 语法糖的日志装饰器：")

    print("   调用 add(3, 5)：")
    result = add(3, 5)
    print(f"   最终结果：{result}")

    print("\n   调用 greet('小明')：")
    result = greet("小明")
    print(f"   最终结果：{result}")

    # ---------- 3. functools.wraps 的作用 ----------
    print("\n3. functools.wraps 的作用：")

    # 没有使用 wraps 的装饰器，被装饰函数的元信息会丢失
    def no_wraps_decorator(func):
        """没有使用 wraps 的装饰器"""
        def wrapper():
            func()
        return wrapper

    @no_wraps_decorator
    def test_func():
        """这是一个测试函数"""
        pass

    print(f"   没有 wraps 的装饰器：")
    print(f"     函数名：{test_func.__name__}")  # 变成了 wrapper
    print(f"     文档字符串：{test_func.__doc__}")  # 变成了 None

    # 使用了 wraps 的装饰器，会保留原函数的元信息
    print(f"   有 wraps 的装饰器（log_decorator 装饰的 add）：")
    print(f"     函数名：{add.__name__}")  # 保留了 add
    print(f"     文档字符串：{add.__doc__}")  # 保留了原文档

    # ---------- 4. 计时装饰器 ----------
    print("\n4. 计时装饰器：")

    print("   调用 slow_function()：")
    result = slow_function()
    print(f"   返回值：{result}")

    print("\n   调用 fast_function()：")
    result = fast_function()
    print(f"   返回值（0+1+...+9999）：{result}")


# ==============================================================================
# 第三部分：带参数的装饰器
# ==============================================================================
# 有时候我们需要给装饰器本身传递参数，来控制装饰器的行为
# 带参数的装饰器实际上是"装饰器工厂"：调用它返回一个真正的装饰器
#
# 语法：
#   @decorator(arg1, arg2)
#   def func():
#       pass
# 等价于：
#   func = decorator(arg1, arg2)(func)
# 即：先调用 decorator(arg1, arg2) 得到真正的装饰器，再用它装饰 func
# ==============================================================================

def repeat(times):
    """
    函数名：repeat
    功能：重复执行装饰器工厂（带参数的装饰器）
    参数：
        times - 重复执行的次数
    返回值：一个真正的装饰器函数
    设计思路：这是一个"装饰器工厂"，接收参数后返回一个装饰器。
             返回的装饰器会让被装饰的函数重复执行 times 次。
    """
    def decorator(func):
        """真正的装饰器函数"""
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            """包装函数：重复调用原函数 times 次"""
            results = []  # 保存每次调用的结果
            for i in range(times):
                print(f"   第 {i+1} 次执行 {func.__name__}：")
                result = func(*args, **kwargs)
                results.append(result)
            return results  # 返回所有结果的列表

        return wrapper

    return decorator  # 返回真正的装饰器


# 使用带参数的装饰器
# 等价于：greet_twice = repeat(2)(greet_twice)
@repeat(3)  # 让函数重复执行 3 次
def greet_twice(name):
    """重复问候函数"""
    msg = f"你好，{name}！"
    print(f"     {msg}")
    return msg


# ---------- 更复杂的带参数装饰器：日志级别 ----------
def log_with_level(level="INFO"):
    """
    函数名：log_with_level
    功能：带日志级别的日志装饰器
    参数：
        level - 日志级别，默认为 "INFO"，可选 "DEBUG", "INFO", "WARNING", "ERROR"
    返回值：装饰器函数
    """
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            print(f"   [{level}] 进入函数 {func.__name__}")
            result = func(*args, **kwargs)
            print(f"   [{level}] 退出函数 {func.__name__}")
            return result
        return wrapper
    return decorator


@log_with_level("DEBUG")  # 指定日志级别为 DEBUG
def debug_func():
    """调试函数"""
    print("     执行调试逻辑...")
    return "debug result"


@log_with_level("WARNING")  # 指定日志级别为 WARNING
def warn_func():
    """警告函数"""
    print("     执行警告逻辑...")
    return "warn result"


def demo_decorator_with_args():
    """
    函数名：demo_decorator_with_args
    功能：演示带参数的装饰器
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、带参数的装饰器】")
    print("=" * 60)

    print("\n1. repeat 装饰器（重复执行）：")
    results = greet_twice("小明")
    print(f"   所有返回值：{results}")

    print("\n2. log_with_level 装饰器（带日志级别）：")
    print("   调用 debug_func()：")
    debug_func()
    print("\n   调用 warn_func()：")
    warn_func()


# ==============================================================================
# 第四部分：类装饰器
# ==============================================================================
# 装饰器不仅可以是函数，也可以是类
# 类装饰器需要实现 __call__ 方法，让类的实例可以像函数一样被调用
#
# 类装饰器的优点：
#   - 可以维护状态（通过实例属性）
#   - 代码结构更清晰（复杂逻辑可以拆分成多个方法）
# ==============================================================================

class CountCalls:
    """
    类名：CountCalls
    功能：调用次数统计装饰器（类实现）
    属性：
        func - 被装饰的函数
        count - 调用次数
    设计思路：使用类实现装饰器，可以方便地维护状态（如调用次数）。
             每次调用被装饰的函数，计数器加 1。
    """

    def __init__(self, func):
        """
        构造函数：接收被装饰的函数
        参数：
            func - 被装饰的函数
        """
        self.func = func  # 保存被装饰的函数
        self.count = 0    # 初始化调用次数为 0
        # 使用 wraps 保留原函数的元信息
        functools.update_wrapper(self, func)

    def __call__(self, *args, **kwargs):
        """
        方法名：__call__
        功能：让实例可以像函数一样被调用
        说明：当调用被装饰的函数时，实际上是调用装饰器实例的 __call__ 方法。
        """
        self.count += 1  # 调用次数加 1
        print(f"   [计数] 函数 {self.func.__name__} 已被调用 {self.count} 次")
        return self.func(*args, **kwargs)  # 调用原函数


@CountCalls
def say_hello(name):
    """打招呼函数（被类装饰器装饰）"""
    print(f"     Hello, {name}!")
    return f"你好，{name}"


# ---------- 另一个类装饰器：缓存 ----------
class CacheDecorator:
    """
    类名：CacheDecorator
    功能：缓存装饰器，缓存函数的返回值
    属性：
        func - 被装饰的函数
        cache - 缓存字典，key 是参数，value 是返回值
    设计思路：对于计算量大的函数，如果同样的参数会被多次调用，
             可以将结果缓存起来，下次直接从缓存中取，提高性能。
    """

    def __init__(self, func):
        """构造函数：初始化缓存"""
        self.func = func
        self.cache = {}  # 缓存字典
        functools.update_wrapper(self, func)

    def __call__(self, *args):
        """调用时先查缓存"""
        # 将参数作为缓存的 key（这里简化处理，只考虑位置参数）
        # 对于字典等不可哈希的参数，需要更复杂的处理
        if args in self.cache:
            print(f"   [缓存] 从缓存中获取结果，参数：{args}")
            return self.cache[args]
        else:
            print(f"   [缓存] 缓存未命中，执行计算，参数：{args}")
            result = self.func(*args)
            self.cache[args] = result  # 将结果存入缓存
            return result


@CacheDecorator
def fibonacci(n):
    """
    计算斐波那契数列的第 n 项（递归实现，效率低，适合演示缓存）
    """
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)


def demo_class_decorator():
    """
    函数名：demo_class_decorator
    功能：演示类装饰器的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【四、类装饰器】")
    print("=" * 60)

    print("\n1. CountCalls（调用次数统计）：")
    say_hello("Alice")
    say_hello("Bob")
    say_hello("Charlie")
    print(f"   总调用次数：{say_hello.count}")

    print("\n2. CacheDecorator（缓存装饰器）：")
    print("   计算 fibonacci(10)：")
    result = fibonacci(10)
    print(f"   结果：{result}")
    print(f"   缓存中的条目数：{len(fibonacci.cache)}")

    print("\n   再次计算 fibonacci(10)（应该从缓存中取）：")
    result = fibonacci(10)
    print(f"   结果：{result}")


# ==============================================================================
# 第五部分：常见内置装饰器
# ==============================================================================
# Python 提供了一些常用的内置装饰器：
#   @property      - 将方法变成属性（在 oop/classes.py 中已详细介绍）
#   @staticmethod   - 静态方法（在 oop/classes.py 中已详细介绍）
#   @classmethod    - 类方法（在 oop/classes.py 中已详细介绍）
#   @functools.lru_cache - 缓存装饰器（Least Recently Used）
# ==============================================================================

def demo_builtin_decorators():
    """
    函数名：demo_builtin_decorators
    功能：演示常见的内置装饰器
    参数：无
    返回值：无
    说明：@property, @staticmethod, @classmethod 在 oop 模块中已详细介绍，
          这里重点介绍 functools.lru_cache。
    """
    print("\n" + "=" * 60)
    print("【五、常见内置装饰器】")
    print("=" * 60)

    print("\n1. functools.lru_cache（LRU 缓存）：")
    print("   （LRU = Least Recently Used，最近最少使用）")

    # 使用 lru_cache 装饰器，自动缓存函数结果
    # maxsize 参数指定缓存的最大条目数，None 表示不限制
    @functools.lru_cache(maxsize=128)
    def fib(n):
        """
        斐波那契数列（使用 lru_cache 缓存）
        """
        if n <= 1:
            return n
        return fib(n - 1) + fib(n - 2)

    # 计算第 30 项（不缓存会非常慢）
    start = time.time()
    result = fib(30)
    elapsed = time.time() - start
    print(f"   fib(30) = {result}，耗时：{elapsed:.6f} 秒")

    # 再次计算，直接从缓存取
    start = time.time()
    result = fib(30)
    elapsed = time.time() - start
    print(f"   再次计算 fib(30) = {result}，耗时：{elapsed:.6f} 秒")

    # 查看缓存信息
    cache_info = fib.cache_info()
    print(f"   缓存信息：{cache_info}")
    print(f"     命中次数：{cache_info.hits}")
    print(f"     未命中次数：{cache_info.misses}")
    print(f"     缓存大小：{cache_info.currsize} / {cache_info.maxsize}")

    print("\n2. @property / @staticmethod / @classmethod")
    print("   这三个装饰器在 oop/classes.py 中已有详细介绍，这里不再重复。")
    print("   - @property：将方法变成属性访问器")
    print("   - @staticmethod：静态方法，不需要 self 或 cls")
    print("   - @classmethod：类方法，第一个参数是 cls（类本身）")


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
    print("  Python 装饰器演示")
    print("=" * 60)

    demo_first_class_functions()   # 函数是一等公民
    demo_simple_decorators()       # 简单装饰器
    demo_decorator_with_args()     # 带参数的装饰器
    demo_class_decorator()         # 类装饰器
    demo_builtin_decorators()      # 内置装饰器

    print("\n" + "=" * 60)
    print("  装饰器演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
