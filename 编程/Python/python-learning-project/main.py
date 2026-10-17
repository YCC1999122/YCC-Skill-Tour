# -*- coding: utf-8 -*-
"""
================================================================================
文件名：main.py
所属项目：Python 学习项目
功能说明：主程序入口，提供交互式菜单，让用户选择运行各个模块的演示。
         集成了基础语法、面向对象、高级特性、数据结构、设计模式、并发编程
         等模块的演示功能。

使用方式：在项目根目录下执行 `python main.py`
================================================================================
"""

import sys       # 导入 sys 模块，用于退出程序和模块路径
import os        # 导入 os 模块，用于路径操作
import importlib  # 导入 importlib 模块，用于动态导入模块


# ==============================================================================
# 模块导入
# ==============================================================================
# 由于部分模块文件名以数字开头（如 01_basics.py），无法直接用 import 语句导入
# 因此使用 importlib 动态导入模块
# 使用 try/except 包裹，防止某个模块导入失败影响整个程序
# ==============================================================================

# 获取项目根目录的绝对路径（main.py 所在目录）
# sys.path 中需要包含项目根目录，才能正确导入各个包
_project_root = os.path.dirname(os.path.abspath(__file__))
if _project_root not in sys.path:
    sys.path.insert(0, _project_root)


def _import_module(module_path):
    """
    函数名：_import_module
    功能：安全地导入一个模块
    参数：
        module_path - 模块的完整路径（如 'basics.01_basics'）
    返回值：模块对象，导入失败返回 None
    说明：使用 importlib 动态导入，支持以数字开头的模块名。
          以下划线开头的函数名表示这是内部辅助函数。
    """
    try:
        # importlib.import_module 可以导入任意名称的模块
        module = importlib.import_module(module_path)
        return module
    except ImportError as e:
        print(f"警告：无法导入 {module_path} 模块：{e}")
        return None


# 导入各个模块
basics_module1 = _import_module("basics.01_basics")                # 基础语法一
basics_module2 = _import_module("basics.02_strings_and_files")      # 基础语法二
oop_module = _import_module("oop.classes")                           # 面向对象
decorators_module = _import_module("advanced.decorators")            # 装饰器
generators_module = _import_module("advanced.generators")            # 生成器
context_module = _import_module("advanced.context_managers")         # 上下文管理器
ds_module = _import_module("data_structures.structures")             # 数据结构
patterns_module = _import_module("patterns.design_patterns")         # 设计模式
concurrency_module = _import_module("concurrency.async_demo")        # 并发编程


# ==============================================================================
# 菜单显示函数
# ==============================================================================

def print_banner():
    """
    函数名：print_banner
    功能：打印程序欢迎横幅
    参数：无
    返回值：无
    """
    print()
    print("=" * 60)
    print("           Python 学习项目 - 交互式演示系统")
    print("=" * 60)
    print()


def print_menu():
    """
    函数名：print_menu
    功能：打印主菜单
    参数：无
    返回值：无
    设计思路：使用编号菜单，用户通过输入数字选择要运行的模块。
             菜单分为六个大类，涵盖 Python 学习的主要知识点。
    """
    print("请选择要运行的演示模块：")
    print()
    print("【一、基础语法】")
    print("   1. 基础语法（数据类型、运算符、控制流、函数、推导式）")
    print("   2. 字符串与文件 IO（字符串操作、格式化、文件读写、异常处理）")
    print()
    print("【二、面向对象】")
    print("   3. 面向对象编程（类、封装、继承、多态、魔术方法、数据类）")
    print()
    print("【三、高级特性】")
    print("   4. 装饰器（一等函数、简单/带参数/类装饰器、内置装饰器）")
    print("   5. 生成器与迭代器（迭代器协议、yield、生成器表达式、yield from）")
    print("   6. 上下文管理器（with语句、类实现、contextlib、常见用法）")
    print()
    print("【四、数据结构与算法】")
    print("   7. 数据结构（链表、栈、队列、二叉树、排序算法）")
    print()
    print("【五、设计模式】")
    print("   8. 设计模式（单例、工厂、观察者、策略、适配器）")
    print()
    print("【六、并发编程】")
    print("   9. 并发编程（多线程、多进程、协程、三种方式对比）")
    print()
    print("【其他】")
    print("   0. 运行全部演示")
    print("   q. 退出程序")
    print()


# ==============================================================================
# 模块运行函数
# ==============================================================================

def run_module(module, name):
    """
    函数名：run_module
    功能：安全地运行一个模块的演示
    参数：
        module - 模块对象
        name - 模块名称（用于提示）
    返回值：无
    设计思路：使用 try/except 包裹模块运行，
             防止某个模块的异常导致整个程序崩溃。
    """
    if module is None:
        print(f"\n错误：{name} 模块未正确加载，无法运行。")
        input("\n按回车键返回菜单...")
        return

    print(f"\n正在运行 {name} 演示...")
    print("-" * 60)

    try:
        # 调用模块的 run_demo 函数
        module.run_demo()
    except Exception as e:
        print(f"\n运行出错：{e}")
        import traceback
        traceback.print_exc()

    print("-" * 60)
    print(f"{name} 演示结束。")
    input("\n按回车键返回菜单...")


# ==============================================================================
# 主循环
# ==============================================================================

def main():
    """
    函数名：main
    功能：主函数，程序入口
    参数：无
    返回值：无
    设计思路：使用 while 循环维持菜单交互，
             根据用户输入选择对应的模块运行，
             直到用户输入 'q' 退出。
    """
    # 打印欢迎横幅
    print_banner()

    # 主循环：不断显示菜单，等待用户输入
    while True:
        # 显示菜单
        print_menu()

        # 获取用户输入
        try:
            choice = input("请输入选项编号（0-9 或 q）：").strip().lower()
        except (KeyboardInterrupt, EOFError):
            # 用户按 Ctrl+C 或 Ctrl+D 退出
            print("\n\n再见！")
            break

        # 处理用户选择
        if choice == "q" or choice == "quit" or choice == "exit":
            # 退出程序
            print("\n感谢使用 Python 学习项目！再见！")
            break

        elif choice == "0":
            # 运行全部演示
            print("\n将依次运行所有模块的演示...")
            print("=" * 60)

            modules_list = [
                (basics_module1, "基础语法"),
                (basics_module2, "字符串与文件IO"),
                (oop_module, "面向对象编程"),
                (decorators_module, "装饰器"),
                (generators_module, "生成器与迭代器"),
                (context_module, "上下文管理器"),
                (ds_module, "数据结构与算法"),
                (patterns_module, "设计模式"),
                (concurrency_module, "并发编程"),
            ]

            for i, (mod, name) in enumerate(modules_list, 1):
                print(f"\n{'='*60}")
                print(f"  第 {i}/{len(modules_list)} 个模块：{name}")
                print(f"{'='*60}")
                run_module(mod, name)

            print("\n" + "=" * 60)
            print("  全部演示运行完毕！")
            print("=" * 60)
            input("\n按回车键返回菜单...")

        elif choice == "1":
            run_module(basics_module1, "基础语法")

        elif choice == "2":
            run_module(basics_module2, "字符串与文件IO")

        elif choice == "3":
            run_module(oop_module, "面向对象编程")

        elif choice == "4":
            run_module(decorators_module, "装饰器")

        elif choice == "5":
            run_module(generators_module, "生成器与迭代器")

        elif choice == "6":
            run_module(context_module, "上下文管理器")

        elif choice == "7":
            run_module(ds_module, "数据结构与算法")

        elif choice == "8":
            run_module(patterns_module, "设计模式")

        elif choice == "9":
            run_module(concurrency_module, "并发编程")

        else:
            # 无效输入
            print(f"\n无效的选项：{choice}，请重新输入。")
            input("按回车键继续...")


# ==============================================================================
# 程序入口
# ==============================================================================

if __name__ == "__main__":
    # 当直接运行 main.py 时，调用主函数
    main()
