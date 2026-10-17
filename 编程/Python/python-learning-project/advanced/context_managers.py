# -*- coding: utf-8 -*-
"""
================================================================================
文件名：context_managers.py
所属模块：advanced（高级特性）
功能说明：本文件深入讲解 Python 上下文管理器，包括 with 语句原理、
         基于类的上下文管理器、contextlib.contextmanager 装饰器，
         以及常见的应用场景。

运行方式：在项目根目录下执行 `python -m advanced.context_managers`
================================================================================
"""

import os           # 导入 os 模块，用于文件路径操作
import time         # 导入 time 模块，用于计时
from contextlib import contextmanager  # 导入 contextmanager 装饰器


# ==============================================================================
# 第一部分：with 语句原理
# ==============================================================================
# with 语句是 Python 中用于资源管理的语法糖
# 常见应用：文件操作、数据库连接、锁管理、临时目录等
#
# with 语句的执行流程：
#   1. 计算 with 后面的表达式，得到一个上下文管理器对象
#   2. 调用上下文管理器的 __enter__() 方法
#   3. 如果 as 子句存在，将 __enter__() 的返回值赋值给 as 后的变量
#   4. 执行 with 块中的代码
#   5. 无论代码是否发生异常，都调用 __exit__() 方法
#
# 上下文管理器协议：
#   - __enter__(self)：进入上下文时调用，返回值赋值给 as 变量
#   - __exit__(self, exc_type, exc_val, exc_tb)：退出上下文时调用
#       exc_type - 异常类型（如果没有异常则为 None）
#       exc_val - 异常对象（如果没有异常则为 None）
#       exc_tb - 异常追踪栈（如果没有异常则为 None）
#       返回值：如果返回 True，表示已处理异常，不再向外抛出；
#              如果返回 False 或 None，表示不处理异常，继续向外抛出
# ==============================================================================

# ==============================================================================
# 第二部分：基于类的上下文管理器
# ==============================================================================
# 通过定义一个类，实现 __enter__ 和 __exit__ 方法，就可以作为上下文管理器使用
# 这是最直观、最灵活的实现方式
# ==============================================================================

class FileManager:
    """
    类名：FileManager
    功能：文件管理器上下文管理器（基于类实现）
    属性：
        filename - 文件名
        mode - 打开模式
        encoding - 编码方式
        file_obj - 文件对象
    设计思路：模拟 Python 内置的文件上下文管理器，
             在 __enter__ 中打开文件，在 __exit__ 中关闭文件。
             这样即使 with 块中发生异常，文件也能被正确关闭。
    """

    def __init__(self, filename, mode="r", encoding="utf-8"):
        """
        构造函数：初始化文件管理器参数
        参数：
            filename - 文件名/路径
            mode - 打开模式，默认 'r'（只读）
            encoding - 编码方式，默认 'utf-8'
        """
        self.filename = filename    # 保存文件名
        self.mode = mode            # 保存打开模式
        self.encoding = encoding    # 保存编码方式
        self.file_obj = None        # 文件对象，初始为 None

    def __enter__(self):
        """
        方法名：__enter__
        功能：进入上下文时调用，打开文件
        返回值：文件对象（赋值给 as 后的变量）
        说明：with 语句开始时自动调用此方法。
        """
        print(f"   [上下文管理器] 打开文件：{self.filename}，模式：{self.mode}")
        # 打开文件，保存文件对象
        self.file_obj = open(self.filename, self.mode, encoding=self.encoding)
        return self.file_obj  # 返回文件对象，供 with 块中使用

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        方法名：__exit__
        功能：退出上下文时调用，关闭文件
        参数：
            exc_type - 异常类型（如果没有异常则为 None）
            exc_val - 异常对象（如果没有异常则为 None）
            exc_tb - 异常追踪栈（如果没有异常则为 None）
        返回值：布尔值，是否处理了异常
        说明：with 块结束时自动调用此方法，无论是否发生异常。
              在这里进行资源清理工作（关闭文件）。
        """
        print(f"   [上下文管理器] 关闭文件：{self.filename}")

        # 关闭文件（如果文件对象存在）
        if self.file_obj:
            self.file_obj.close()

        # 检查是否发生了异常
        if exc_type is not None:
            print(f"   [上下文管理器] 检测到异常：{exc_type.__name__}: {exc_val}")
            # 返回 False 表示不处理异常，继续向外抛出
            # 返回 True 表示已处理异常，不再向外抛出
            # 这里返回 False，让异常继续传播
            return False

        # 没有异常，返回 None（等价于 False）
        return None


class Timer:
    """
    类名：Timer
    功能：计时上下文管理器
    属性：
        start_time - 开始时间
        elapsed - 耗时（秒）
    设计思路：进入上下文时记录开始时间，退出时计算耗时并打印。
             方便地统计一段代码的执行时间。
    """

    def __init__(self, description="代码块"):
        """
        构造函数：
        参数：
            description - 描述信息，用于标识计时的代码块
        """
        self.description = description  # 保存描述
        self.start_time = None          # 开始时间
        self.elapsed = None             # 耗时

    def __enter__(self):
        """
        进入上下文：记录开始时间
        返回值：self（可以通过 as 获取计时器对象，访问 elapsed 属性）
        """
        print(f"   [计时] 开始执行：{self.description}")
        self.start_time = time.time()  # 记录开始时间
        return self  # 返回自身，方便在 with 块中访问

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        退出上下文：计算并打印耗时
        """
        self.elapsed = time.time() - self.start_time  # 计算耗时
        print(f"   [计时] {self.description} 执行完毕，耗时：{self.elapsed:.6f} 秒")
        return False  # 不处理异常


def demo_class_context_manager():
    """
    函数名：demo_class_context_manager
    功能：演示基于类的上下文管理器
    参数：无
    返回值：无
    """
    print("=" * 60)
    print("【一、基于类的上下文管理器】")
    print("=" * 60)

    # ---------- 1. FileManager ----------
    print("\n1. FileManager（文件管理器）：")

    # 获取测试文件路径
    test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "temp_files")
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    test_file = os.path.join(test_dir, "context_test.txt")

    # 写入文件
    print("   写入文件：")
    with FileManager(test_file, "w") as f:
        f.write("第一行：Hello World\n")
        f.write("第二行：上下文管理器测试\n")
        f.write("第三行：文件会自动关闭\n")

    # 读取文件
    print("\n   读取文件：")
    with FileManager(test_file, "r") as f:
        content = f.read()
        print(f"   文件内容：\n   {content.replace(chr(10), chr(10) + '   ')}")

    # 测试异常情况下文件是否会被关闭
    print("\n   测试异常情况下文件是否关闭：")
    try:
        with FileManager(test_file, "r") as f:
            content = f.read()
            # 故意引发一个异常
            raise ValueError("故意抛出的测试异常")
    except ValueError as e:
        print(f"   捕获到异常：{e}")
        print(f"   文件已关闭吗？{f.closed}")  # f 仍然存在（但已经关闭了）

    # ---------- 2. Timer ----------
    print("\n2. Timer（计时器）：")

    # 使用 Timer 统计代码执行时间
    with Timer("循环计算") as timer:
        # 模拟一些耗时操作
        total = 0
        for i in range(1000000):
            total += i
        print(f"   计算结果：{total}")

    print(f"   通过 timer.elapsed 访问耗时：{timer.elapsed:.6f} 秒")

    # 嵌套使用
    print("\n   嵌套使用 Timer：")
    with Timer("外层"):
        time.sleep(0.1)
        with Timer("内层"):
            time.sleep(0.2)


# ==============================================================================
# 第三部分：contextlib.contextmanager
# ==============================================================================
# 使用 contextlib.contextmanager 装饰器，可以用生成器函数来创建上下文管理器
# 这是一种更简洁的方式，不需要定义类
#
# 用法：
#   @contextmanager
#   def my_context():
#       # __enter__ 部分（yield 之前的代码）
#       yield value  # yield 的值作为 __enter__ 的返回值
#       # __exit__ 部分（yield 之后的代码）
#
# yield 将函数分成两部分：
#   - yield 之前：对应 __enter__，在 with 块执行前运行
#   - yield 之后：对应 __exit__，在 with 块执行后运行
#   - yield 的值：赋值给 as 后的变量
#
# 注意：使用装饰器方式时，异常需要自己在 yield 周围用 try/finally 处理
# ==============================================================================

@contextmanager
def file_manager(filename, mode="r", encoding="utf-8"):
    """
    函数名：file_manager
    功能：文件管理器上下文管理器（使用 @contextmanager 实现）
    参数：
        filename - 文件名
        mode - 打开模式
        encoding - 编码方式
    设计思路：使用 contextmanager 装饰器 + 生成器函数，
             用更简洁的代码实现与 FileManager 类相同的功能。
    """
    # __enter__ 部分：yield 之前的代码
    print(f"   [contextmanager] 打开文件：{filename}")
    f = open(filename, mode, encoding=encoding)  # 打开文件

    try:
        # yield 将文件对象返回给 with 语句（赋值给 as 变量）
        # 函数在这里暂停，等待 with 块执行完毕
        yield f
    finally:
        # __exit__ 部分：yield 之后的代码（在 finally 中确保一定执行）
        print(f"   [contextmanager] 关闭文件：{filename}")
        f.close()  # 关闭文件


@contextmanager
def timer_context(description="代码块"):
    """
    函数名：timer_context
    功能：计时上下文管理器（使用 @contextmanager 实现）
    参数：
        description - 描述信息
    设计思路：使用生成器函数实现计时功能，比类实现更简洁。
    """
    # __enter__：记录开始时间
    print(f"   [计时] 开始执行：{description}")
    start_time = time.time()

    # 用字典保存结果，因为 yield 不能直接返回值后再修改
    # （yield 的值在进入时就确定了）
    result = {"elapsed": None}

    try:
        yield result  # 返回结果字典，with 块中可以读取 elapsed
    finally:
        # __exit__：计算耗时
        result["elapsed"] = time.time() - start_time
        print(f"   [计时] {description} 执行完毕，耗时：{result['elapsed']:.6f} 秒")


@contextmanager
def suppress_exception(*exception_types):
    """
    函数名：suppress_exception
    功能：抑制指定类型的异常（忽略这些异常）
    参数：
        *exception_types - 要抑制的异常类型
    设计思路：如果 with 块中抛出了指定类型的异常，就忽略它，
             让程序继续执行。类似于 contextlib.suppress。
    """
    try:
        yield  # 执行 with 块中的代码
    except exception_types:
        # 捕获到指定类型的异常，什么都不做（抑制异常）
        print(f"   [suppress] 捕获并抑制了指定类型的异常")
        # 这里没有重新抛出异常，相当于 __exit__ 返回 True


@contextmanager
def change_directory(path):
    """
    函数名：change_directory
    功能：临时切换工作目录，退出时恢复原目录
    参数：
        path - 要切换到的目录路径
    设计思路：这是一个典型的上下文管理器应用场景——
             进入时改变状态，退出时恢复状态。
    """
    # __enter__：保存当前目录，切换到新目录
    old_dir = os.getcwd()  # 保存当前工作目录
    print(f"   [cd] 当前目录：{old_dir}")
    os.chdir(path)  # 切换到新目录
    print(f"   [cd] 切换到：{os.getcwd()}")

    try:
        yield  # 执行 with 块
    finally:
        # __exit__：恢复原目录
        os.chdir(old_dir)  # 切回原目录
        print(f"   [cd] 恢复到：{os.getcwd()}")


def demo_contextlib():
    """
    函数名：demo_contextlib
    功能：演示 contextlib.contextmanager 装饰器的使用
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【二、contextlib.contextmanager 装饰器】")
    print("=" * 60)

    # ---------- 1. file_manager ----------
    print("\n1. file_manager（装饰器版文件管理器）：")

    test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "temp_files")
    test_file = os.path.join(test_dir, "contextlib_test.txt")

    with file_manager(test_file, "w") as f:
        f.write("使用 @contextmanager 创建的上下文管理器\n")
        f.write("代码更简洁，功能一样强大\n")

    with file_manager(test_file, "r") as f:
        content = f.read()
        print(f"   文件内容：\n   {content.replace(chr(10), chr(10) + '   ')}")

    # ---------- 2. timer_context ----------
    print("\n2. timer_context（装饰器版计时器）：")

    with timer_context("列表推导式") as result:
        lst = [x ** 2 for x in range(100000)]

    print(f"   耗时：{result['elapsed']:.6f} 秒")

    # ---------- 3. suppress_exception ----------
    print("\n3. suppress_exception（抑制异常）：")

    print("   正常情况下除以零会报错：")
    with suppress_exception(ZeroDivisionError):
        result = 10 / 0  # 这个异常会被抑制
        print(f"     结果：{result}")  # 这行不会执行
    print("   程序继续运行，异常被抑制了")

    print("\n   非指定类型的异常不会被抑制：")
    try:
        with suppress_exception(ZeroDivisionError):
            raise ValueError("值错误")  # 不是 ZeroDivisionError，不会被抑制
    except ValueError as e:
        print(f"   ValueError 没有被抑制：{e}")

    # ---------- 4. change_directory ----------
    print("\n4. change_directory（临时切换目录）：")

    # 获取上级目录用于测试
    parent_dir = os.path.dirname(os.path.abspath(__file__))
    with change_directory(parent_dir):
        print(f"   with 块中的当前目录：{os.getcwd()}")
        # 在这里做一些需要在特定目录下的操作

    print(f"   with 块结束后的当前目录：{os.getcwd()}")


# ==============================================================================
# 第四部分：常见用法
# ==============================================================================
# 上下文管理器的常见应用场景：
#   1. 文件操作（内置的 open()）
#   2. 锁管理（threading.Lock）
#   3. 数据库连接
#   4. 临时文件/目录
#   5. 资源池获取与释放
#   6. 事务管理
# ==============================================================================

# ---------- 模拟数据库连接 ----------
class DatabaseConnection:
    """
    类名：DatabaseConnection
    功能：模拟数据库连接的上下文管理器
    属性：
        db_name - 数据库名
        connected - 是否连接
    设计思路：模拟真实的数据库连接管理，
             进入时建立连接，退出时关闭连接。
    """

    def __init__(self, db_name):
        self.db_name = db_name
        self.connected = False

    def __enter__(self):
        """进入上下文：建立数据库连接"""
        print(f"   [数据库] 连接到数据库：{self.db_name}")
        self.connected = True
        return self  # 返回连接对象本身

    def __exit__(self, exc_type, exc_val, exc_tb):
        """退出上下文：关闭数据库连接"""
        print(f"   [数据库] 关闭数据库连接：{self.db_name}")
        self.connected = False
        # 如果发生异常，也确保关闭连接
        if exc_type is not None:
            print(f"   [数据库] 异常情况下仍然关闭了连接")
        return False  # 不处理异常

    def query(self, sql):
        """模拟执行 SQL 查询"""
        if not self.connected:
            raise RuntimeError("数据库未连接")
        print(f"   [数据库] 执行查询：{sql}")
        return [{"id": 1, "name": "张三"}, {"id": 2, "name": "李四"}]


# ---------- 模拟线程锁 ----------
class FakeLock:
    """
    类名：FakeLock
    功能：模拟线程锁的上下文管理器（简化版，不涉及真实多线程）
    属性：
        locked - 是否锁定
    设计思路：模拟 threading.Lock 的使用方式，
             进入时加锁，退出时解锁。
    """

    def __init__(self):
        self.locked = False

    def __enter__(self):
        """进入上下文：获取锁"""
        print(f"   [锁] 获取锁...")
        self.locked = True
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """退出上下文：释放锁"""
        print(f"   [锁] 释放锁...")
        self.locked = False
        return False


def demo_common_usage():
    """
    函数名：demo_common_usage
    功能：演示上下文管理器的常见应用场景
    参数：无
    返回值：无
    """
    print("\n" + "=" * 60)
    print("【三、上下文管理器常见用法】")
    print("=" * 60)

    # ---------- 1. 内置文件操作（最常用）----------
    print("\n1. 内置文件操作（open 本身就是上下文管理器）：")

    test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "temp_files")
    test_file = os.path.join(test_dir, "builtin_file.txt")

    # Python 内置的 open() 返回的文件对象本身就是上下文管理器
    with open(test_file, "w", encoding="utf-8") as f:
        f.write("Python 内置的文件对象就是上下文管理器\n")
        f.write("使用 with open(...) 是最推荐的文件操作方式\n")

    with open(test_file, "r", encoding="utf-8") as f:
        content = f.read()
        print(f"   文件内容：{content.strip()}")

    # ---------- 2. 数据库连接 ----------
    print("\n2. 数据库连接：")

    with DatabaseConnection("my_database") as db:
        print(f"   连接状态：{'已连接' if db.connected else '未连接'}")
        results = db.query("SELECT * FROM users")
        print(f"   查询结果：{results}")

    print(f"   with 块结束后，连接状态：{'已连接' if db.connected else '未连接'}")

    # 异常情况下也能正确关闭连接
    print("\n   测试异常情况下的数据库连接：")
    try:
        with DatabaseConnection("test_db") as db:
            db.query("SELECT * FROM nonexistent_table")
            raise RuntimeError("数据库查询出错！")
    except RuntimeError as e:
        print(f"   捕获到异常：{e}")
        print(f"   连接是否关闭？{'已关闭' if not db.connected else '未关闭'}")

    # ---------- 3. 锁管理 ----------
    print("\n3. 锁管理（模拟）：")

    lock = FakeLock()
    print(f"   初始状态：{'锁定' if lock.locked else '未锁定'}")

    with lock:
        print(f"   with 块中：{'锁定' if lock.locked else '未锁定'}")
        print("   执行受保护的代码...")

    print(f"   with 块结束后：{'锁定' if lock.locked else '未锁定'}")

    # ---------- 4. 多个上下文管理器 ----------
    print("\n4. 同时使用多个上下文管理器：")

    test_file2 = os.path.join(test_dir, "file2.txt")

    # Python 3.10+ 可以用括号包裹多个上下文管理器
    # 这里用逗号分隔的方式（所有版本都支持）
    with open(test_file, "r", encoding="utf-8") as f1, \
         open(test_file2, "w", encoding="utf-8") as f2:
        content = f1.read()
        f2.write("从文件1复制来的内容：\n")
        f2.write(content)
        print("   从 file1 复制内容到 file2")

    # 验证
    with open(test_file2, "r", encoding="utf-8") as f:
        print(f"   file2 内容：\n   {f.read().strip().replace(chr(10), chr(10) + '   ')}")


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
    print("  Python 上下文管理器演示")
    print("=" * 60)

    demo_class_context_manager()  # 基于类的上下文管理器
    demo_contextlib()             # contextlib 装饰器
    demo_common_usage()           # 常见用法

    print("\n" + "=" * 60)
    print("  上下文管理器演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
