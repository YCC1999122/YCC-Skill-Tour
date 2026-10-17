# -*- coding: utf-8 -*-
"""
================================================================================
文件名：generators.py
所属模块：advanced（高级特性）
功能说明：本文件深入讲解 Python 生成器、迭代器协议和协程基础。
         包括生成器函数（yield）、生成器表达式、迭代器协议、yield from 等。

运行方式：在项目根目录下执行 `python -m advanced.generators`
================================================================================
"""

import sys  # 导入 sys 模块，用于查看内存占用


# ==============================================================================
# 第一部分：迭代器协议
# ==============================================================================
# 迭代器（Iterator）是 Python 中一个重要的概念
# 可迭代对象（Iterable）：可以被迭代的对象，如列表、元组、字符串、字典等
# 迭代器（Iterator）：具有 __next__ 方法的对象，可以逐个返回元素
#
# 迭代器协议：
#   - 实现了 __iter__() 方法的对象是可迭代的（Iterable）
#   - 实现了 __next__() 方法的对象是迭代器（Iterator）
#   - __iter__() 返回迭代器对象（通常是 self）
#   - __next__() 返回下一个元素，如果没有元素了，抛出 StopIteration 异常
#
# for 循环的本质：
#   1. 调用 iter(obj) 获取迭代器
#   2. 不断调用 next(iterator) 获取下一个元素
#   3. 遇到 StopIteration 异常时结束循环
# ==============================================================================

class MyRange:
    """
    类名：MyRange
    功能：自定义的 range 类，实现迭代器协议
    属性：
        start - 起始值
        end - 结束值（不包含）
        step - 步长
        current - 当前值（迭代状态）
    设计思路：通过实现 __iter__ 和 __next__ 方法，让自定义类可以被 for 循环迭代。
             这就是迭代器协议的核心。
    """

    def __init__(self, start, end, step=1):
        """
        构造函数：初始化范围参数
        参数：
            start - 起始值
            end - 结束值（不包含）
            step - 步长，默认为 1
        """
        self.start = start    # 起始值
        self.end = end        # 结束值
        self.step = step      # 步长
        self.current = start  # 当前值，初始为 start

    def __iter__(self):
        """
        方法名：__iter__
        功能：返回迭代器对象
        返回值：self（因为自身就是迭代器）
        说明：实现了 __iter__ 方法，对象就是可迭代的（Iterable）。
              通常迭代器的 __iter__ 直接返回 self。
        """
        # 每次迭代开始时重置 current，让迭代器可以重复使用
        self.current = self.start
        return self

    def __next__(self):
        """
        方法名：__next__
        功能：返回下一个元素
        返回值：当前值
        异常：如果超出范围，抛出 StopIteration
        说明：实现了 __next__ 方法，对象就是迭代器（Iterator）。
              每次调用返回下一个元素，没有元素时抛出 StopIteration。
        """
        # 判断是否已经到达/超过结束值
        if self.current >= self.end:
            # 没有更多元素了，抛出 StopIteration 异常
            # for 循环捕获到这个异常就会结束循环
            raise StopIteration

        # 保存当前值
        value = self.current
        # 移动到下一个值
        self.current += self.step
        # 返回当前值
        return value


def demo_iterator_protocol():
    """
    函数名：demo_iterator_protocol
    功能：演示迭代器协议
    参数：无
    返回值：无
    """
    print("=" * 60)
    print("【一、迭代器协议】")
    print("=" * 60)

    # ---------- 1. 自定义迭代器 ----------
    print("\n1. 自定义迭代器 MyRange：")

    my_range = MyRange(0, 10, 2)  # 创建自定义范围：0, 2, 4, 6, 8
    print(f"   MyRange(0, 10, 2) 迭代结果：", end="")
    for num in my_range:
        print(num, end=" ")
    print()

    # ---------- 2. 手动使用迭代器 ----------
    print("\n2. 手动使用迭代器（模拟 for 循环）：")

    my_list = [10, 20, 30]
    print(f"   列表：{my_list}")

    # 1. 调用 iter() 获取迭代器（等价于调用 my_list.__iter__()）
    iterator = iter(my_list)
    print(f"   迭代器对象：{iterator}")
    print(f"   迭代器类型：{type(iterator).__name__}")

    # 2. 不断调用 next() 获取元素（等价于调用 iterator.__next__()）
    print("   手动调用 next()：")
    print(f"     第1次：{next(iterator)}")
    print(f"     第2次：{next(iterator)}")
    print(f"     第3次：{next(iterator)}")

    # 3. 第4次调用时，没有元素了，抛出 StopIteration
    print("     第4次：", end="")
    try:
        next(iterator)
    except StopIteration:
        print("抛出 StopIteration 异常（迭代结束）")

    # ---------- 3. 可迭代对象 vs 迭代器 ----------
    print("\n3. 可迭代对象 vs 迭代器：")

    from collections.abc import Iterable, Iterator

    lst = [1, 2, 3]
    it = iter(lst)

    print(f"   列表是可迭代的吗？{isinstance(lst, Iterable)}")
    print(f"   列表是迭代器吗？{isinstance(lst, Iterator)}")
    print(f"   列表的迭代器是可迭代的吗？{isinstance(it, Iterable)}")
    print(f"   列表的迭代器是迭代器吗？{isinstance(it, Iterator)}")
    print("   （迭代器一定是可迭代的，但可迭代的不一定是迭代器）")


# ==============================================================================
# 第二部分：生成器函数
# ==============================================================================
# 生成器（Generator）是一种特殊的迭代器，使用 yield 关键字创建
# 生成器函数：包含 yield 关键字的函数，调用时返回一个生成器对象
#
# yield 和 return 的区别：
#   - return：结束函数，返回一个值
#   - yield：暂停函数，返回一个值，下次调用 next() 时从暂停处继续执行
#
# 生成器的优点：
#   - 惰性求值：只在需要时才生成值，节省内存
#   - 状态保持：每次 yield 后暂停，保留当前状态
#   - 代码简洁：比手动实现迭代器简单得多
# ==============================================================================

def simple_generator():
    """
    函数名：simple_generator
    功能：一个最简单的生成器函数
    说明：包含 yield 关键字的函数就是生成器函数。
          调用生成器函数不会执行函数体，而是返回一个生成器对象。
    """
    print("   生成器：开始执行")
    yield 1  # 第一次 next() 执行到这里，暂停，返回 1
    print("   生成器：继续执行")
    yield 2  # 第二次 next() 从这里继续，执行到这里暂停，返回 2
    print("   生成器：继续执行")
    yield 3  # 第三次 next() 从这里继续，执行到这里暂停，返回 3
    print("   生成器：执行结束")
    # 函数结束，下次 next() 会抛出 StopIteration


def fibonacci_generator(n):
    """
    函数名：fibonacci_generator
    功能：斐波那契数列生成器
    参数：
        n - 生成前 n 个斐波那契数
    返回值：生成器对象
    设计思路：使用生成器实现斐波那契数列，不需要保存所有结果，
             每次只生成下一个值，内存效率极高。
    """
    a, b = 0, 1  # 初始化前两个数
    for _ in range(n):
        yield a  # 返回当前的 a，暂停
        a, b = b, a + b  # 计算下一对数


def infinite_counter():
    """
    函数名：infinite_counter
    功能：无限计数器生成器
    返回值：生成器对象，每次返回递增的整数
    设计思路：生成器可以表示无限序列，因为它是惰性求值的，
             不会一次性生成所有元素（那样会内存溢出）。
             使用者只取自己需要的数量即可。
    """
    count = 0
    while True:  # 无限循环
        yield count  # 每次返回当前计数
        count += 1   # 计数加 1


def demo_generator_functions():
    """
    函数名：demo_generator_functions
    功能：演示生成器函数的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、生成器函数（yield）】")
    print("=" * 60)

    # ---------- 1. 简单生成器 ----------
    print("\n1. 简单生成器：")

    # 调用生成器函数，返回生成器对象（函数体并未执行）
    gen = simple_generator()
    print(f"   生成器对象：{gen}")
    print(f"   生成器类型：{type(gen).__name__}")

    # 第一次调用 next()，从函数开头执行到第一个 yield
    print("\n   第一次 next(gen)：")
    value = next(gen)
    print(f"   返回值：{value}")

    # 第二次调用 next()，从第一个 yield 后继续，到第二个 yield
    print("\n   第二次 next(gen)：")
    value = next(gen)
    print(f"   返回值：{value}")

    # 第三次调用 next()
    print("\n   第三次 next(gen)：")
    value = next(gen)
    print(f"   返回值：{value}")

    # 第四次调用，函数执行完毕，抛出 StopIteration
    print("\n   第四次 next(gen)：", end="")
    try:
        next(gen)
    except StopIteration:
        print("抛出 StopIteration（生成器结束）")

    # ---------- 2. 用 for 循环迭代生成器 ----------
    print("\n2. 用 for 循环迭代生成器：")

    print("   斐波那契数列前 10 项：", end="")
    for num in fibonacci_generator(10):
        print(num, end=" ")
    print()

    # ---------- 3. 无限生成器 ----------
    print("\n3. 无限生成器（惰性求值，只取需要的部分）：")

    counter = infinite_counter()  # 创建无限计数器生成器
    print("   取前 5 个值：", end="")
    for _ in range(5):
        print(next(counter), end=" ")
    print()

    print("   再取 5 个值（接着之前的继续）：", end="")
    for _ in range(5):
        print(next(counter), end=" ")
    print()

    # ---------- 4. 内存对比 ----------
    print("\n4. 内存效率对比：")

    # 普通列表：一次性生成所有元素，占用大量内存
    big_list = [x ** 2 for x in range(1000000)]
    list_memory = sys.getsizeof(big_list)
    print(f"   列表推导式（100万元素）：{list_memory / 1024 / 1024:.2f} MB")

    # 生成器：只保存状态，不保存所有元素
    big_gen = (x ** 2 for x in range(1000000))
    gen_memory = sys.getsizeof(big_gen)
    print(f"   生成器表达式：{gen_memory} 字节")
    print(f"   （生成器内存占用极小，因为不保存所有元素）")

    # 清理
    del big_list
    del big_gen


# ==============================================================================
# 第三部分：生成器表达式
# ==============================================================================
# 生成器表达式是创建生成器的一种简洁方式，语法与列表推导式类似，
# 但使用圆括号而不是方括号
#
# 语法：(表达式 for 变量 in 可迭代对象 if 条件)
#
# 什么时候用生成器表达式：
#   - 数据量大，不需要一次性全部生成
#   - 只需要迭代一次
#   - 需要节省内存
# 什么时候用列表推导式：
#   - 需要多次迭代
#   - 需要索引访问
#   - 需要列表的各种方法（append, sort 等）
# ==============================================================================

def demo_generator_expressions():
    """
    函数名：demo_generator_expressions
    功能：演示生成器表达式的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、生成器表达式】")
    print("=" * 60)

    # ---------- 1. 基本用法 ----------
    print("\n1. 基本用法：")

    # 列表推导式：方括号，返回列表
    list_comp = [x ** 2 for x in range(10)]
    print(f"   列表推导式：{list_comp}")
    print(f"   类型：{type(list_comp).__name__}")

    # 生成器表达式：圆括号，返回生成器
    gen_exp = (x ** 2 for x in range(10))
    print(f"   生成器表达式：{gen_exp}")
    print(f"   类型：{type(gen_exp).__name__}")

    # 遍历生成器
    print("   遍历生成器：", end="")
    for value in gen_exp:
        print(value, end=" ")
    print()

    # ---------- 2. 带条件的生成器表达式 ----------
    print("\n2. 带条件的生成器表达式：")

    # 只生成偶数的平方
    even_squares = (x ** 2 for x in range(20) if x % 2 == 0)
    print("   偶数的平方（0~19）：", end="")
    for value in even_squares:
        print(value, end=" ")
    print()

    # ---------- 3. 函数参数中的生成器表达式 ----------
    print("\n3. 作为函数参数：")

    # 当生成器表达式是函数的唯一参数时，可以省略外层圆括号
    total = sum(x ** 2 for x in range(100))  # 计算 0~99 的平方和
    print(f"   sum(x**2 for x in range(100)) = {total}")

    max_val = max(x for x in range(100) if x % 7 == 0)  # 100以内最大的7的倍数
    print(f"   100以内最大的7的倍数：{max_val}")

    any_divisible = any(x % 17 == 0 for x in range(100, 200))
    print(f"   100~199 中有没有17的倍数？{any_divisible}")

    # ---------- 4. 一次性使用 ----------
    print("\n4. 生成器只能遍历一次：")

    gen = (x for x in range(5))
    print("   第一次遍历：", end="")
    for value in gen:
        print(value, end=" ")
    print()

    print("   第二次遍历：", end="")
    for value in gen:
        print(value, end=" ")  # 什么都不会输出
    print("（空，生成器已经耗尽）")


# ==============================================================================
# 第四部分：生成器的高级用法
# ==============================================================================
# 生成器不仅可以产出值（yield value），还可以：
#   - 接收值：通过 generator.send(value) 向生成器发送值
#   - 抛出异常：通过 generator.throw(exception) 向生成器抛出异常
#   - 关闭生成器：通过 generator.close() 关闭生成器
#
# 这些特性是协程（Coroutine）的基础
# ==============================================================================

def echo_generator():
    """
    函数名：echo_generator
    功能：回声生成器，演示 send() 方法
    说明：生成器可以通过 yield 表达式接收外部发送的值。
          value = yield  表示：
          - 执行到 yield 时暂停，返回 yield 后面的值（如果有的话）
          - 恢复执行时，将 send() 传入的值赋给 value
    """
    print("   [回声生成器] 启动")
    while True:
        # 等号左边的 value 接收 send() 传入的值
        # 等号右边的 yield 后可以跟返回值（这里返回接收到的值）
        value = yield  # 暂停，等待外部 send 值进来
        print(f"   [回声生成器] 收到：{value}，回声：{value}")


def running_average():
    """
    函数名：running_average
    功能：移动平均值生成器
    返回值：生成器对象
    设计思路：每次接收一个数值，返回当前所有数值的平均值。
             这是协程的典型应用：持续接收数据，维护内部状态，返回结果。
    """
    total = 0      # 总和
    count = 0      # 数量
    average = None  # 平均值
    while True:
        # 接收 send 过来的值，同时返回当前的平均值
        value = yield average  # yield average 返回平均值，然后等待接收新值
        total += value
        count += 1
        average = total / count


def demo_generator_advanced():
    """
    函数名：demo_generator_advanced
    功能：演示生成器的高级用法（send, throw, close）
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【四、生成器高级用法（协程基础）】")
    print("=" * 60)

    # ---------- 1. send() 发送值 ----------
    print("\n1. send() 向生成器发送值：")

    echo = echo_generator()
    print("   启动生成器（第一次 next 或 send(None)）：")
    next(echo)  # 第一次必须用 next() 或 send(None) 启动，让代码运行到第一个 yield

    print("   发送 'Hello'：")
    echo.send("Hello")

    print("   发送 'Python'：")
    echo.send("Python")

    print("   发送 12345：")
    echo.send(12345)

    # 关闭生成器
    echo.close()
    print("   生成器已关闭")

    # ---------- 2. 移动平均值 ----------
    print("\n2. 移动平均值协程：")

    avg = running_average()
    next(avg)  # 预激（prime）协程，让它运行到第一个 yield

    print("   依次输入 10, 20, 30, 40, 50：")
    for num in [10, 20, 30, 40, 50]:
        result = avg.send(num)
        print(f"     输入 {num}，当前平均值：{result:.2f}")

    avg.close()

    # ---------- 3. throw() 抛出异常 ----------
    print("\n3. throw() 向生成器抛出异常：")

    def gen_with_try():
        """带有异常处理的生成器"""
        try:
            yield "正常状态"
            yield "还是正常"
        except ValueError as e:
            print(f"   [生成器内部] 捕获到 ValueError：{e}")
            yield "异常处理后恢复"
        yield "继续运行"

    gen = gen_with_try()
    print(f"   第一次 next：{next(gen)}")
    print(f"   throw ValueError('测试异常')：{gen.throw(ValueError('测试异常'))}")
    print(f"   再 next：{next(gen)}")


# ==============================================================================
# 第五部分：yield from
# ==============================================================================
# yield from 是 Python 3.3 引入的语法，用于在生成器中委托给另一个生成器
# 也叫"生成器委托"（Generator Delegation）
#
# yield from sub_generator 的作用：
#   - 将 sub_generator 的所有值逐个 yield 出去
#   - 自动处理 send/throw/close 的转发
#   - 获取子生成器的返回值（即子生成器 return 的值）
#
# yield from 是协程和 asyncio 的重要基础
# ==============================================================================

def sub_generator():
    """
    函数名：sub_generator
    功能：子生成器
    """
    yield 1
    yield 2
    yield 3
    return "子生成器的返回值"  # 生成器的 return 值只能通过 yield from 获取


def main_generator():
    """
    函数名：main_generator
    功能：主生成器，使用 yield from 委托给子生成器
    """
    print("   [主生成器] 开始")

    # yield from 委托给子生成器
    # 等价于：
    #   for value in sub_generator():
    #       yield value
    # 但 yield from 功能更强：还能转发 send/throw/close，获取 return 值
    result = yield from sub_generator()  # 接收子生成器的 return 值

    print(f"   [主生成器] 子生成器返回：{result}")
    yield "主生成器继续"
    print("   [主生成器] 结束")


def flatten(nested_list):
    """
    函数名：flatten
    功能：使用 yield from 展平嵌套列表
    参数：
        nested_list - 可能嵌套的列表
    返回值：生成器，逐个产出所有元素
    设计思路：递归 + yield from，简洁地实现列表展平。
    """
    for item in nested_list:
        if isinstance(item, list):
            # 如果元素是列表，递归展平，用 yield from 委托出去
            yield from flatten(item)
        else:
            # 如果不是列表，直接产出
            yield item


def demo_yield_from():
    """
    函数名：demo_yield_from
    功能：演示 yield from 的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【五、yield from（生成器委托）】")
    print("=" * 60)

    # ---------- 1. 基本用法 ----------
    print("\n1. yield from 基本用法：")

    print("   遍历主生成器：")
    for value in main_generator():
        print(f"     产出：{value}")

    # ---------- 2. 展平嵌套列表 ----------
    print("\n2. yield from 展平嵌套列表：")

    nested = [1, [2, 3, [4, 5]], 6, [7, [8, [9, 10]]]]
    print(f"   原始嵌套列表：{nested}")
    print("   展平后：", end="")
    for item in flatten(nested):
        print(item, end=" ")
    print()


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
    print("  Python 生成器与迭代器演示")
    print("=" * 60)

    demo_iterator_protocol()      # 迭代器协议
    demo_generator_functions()    # 生成器函数
    demo_generator_expressions()  # 生成器表达式
    demo_generator_advanced()     # 生成器高级用法
    demo_yield_from()             # yield from

    print("\n" + "=" * 60)
    print("  生成器与迭代器演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
