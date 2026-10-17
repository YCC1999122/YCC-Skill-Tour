# -*- coding: utf-8 -*-
"""
================================================================================
文件名：01_basics.py
所属模块：basics（基础语法）
功能说明：本文件演示 Python 基础语法，包括数据类型、运算符、控制流、函数定义、
         推导式等核心概念。每个知识点都配有详细的中文注释，便于初学者理解。

运行方式：在项目根目录下执行 `python -m basics.01_basics` 或直接运行本文件。
================================================================================
"""


# ==============================================================================
# 第一部分：基本数据类型
# ==============================================================================
# Python 是动态类型语言，变量不需要声明类型，解释器会自动推断。
# Python 常见的内置数据类型包括：
#   - 数值类型：int（整数）、float（浮点数）、complex（复数）
#   - 字符串类型：str
#   - 布尔类型：bool
#   - 序列类型：list（列表）、tuple（元组）、range
#   - 映射类型：dict（字典）
#   - 集合类型：set（集合）、frozenset
#   - 空类型：NoneType（值为 None）
# ==============================================================================

def demo_basic_types():
    """
    函数名：demo_basic_types
    功能：演示 Python 基本数据类型的定义和使用
    参数：无
    返回值：无
    设计思路：通过具体示例展示每种数据类型的特点和基本操作，
             使用 print 函数输出结果，方便学习者观察。
    """
    print("=" * 60)
    print("【一、基本数据类型演示】")
    print("=" * 60)

    # ---------- 1. 整数类型（int）----------
    # Python 中整数可以是任意大小，不受位数限制（与 C/Java 不同）
    # 直接赋值一个整数值，变量类型自动为 int
    age = 25  # 定义一个整数变量 age，表示年龄，值为 25
    print(f"\n1. 整数类型 (int):")
    print(f"   age = {age}, 类型: {type(age)}")  # type() 函数返回变量的类型

    # 整数可以进行各种进制表示
    binary_num = 0b1010  # 0b 前缀表示二进制，0b1010 = 十进制的 10
    octal_num = 0o12     # 0o 前缀表示八进制，0o12 = 十进制的 10
    hex_num = 0xA        # 0x 前缀表示十六进制，0xA = 十进制的 10
    print(f"   二进制 0b1010 = {binary_num}")
    print(f"   八进制 0o12 = {octal_num}")
    print(f"   十六进制 0xA = {hex_num}")

    # ---------- 2. 浮点数类型（float）----------
    # 浮点数即小数，Python 使用双精度浮点数（64位），精度有限
    price = 99.99  # 定义一个浮点数变量 price，表示价格
    print(f"\n2. 浮点数类型 (float):")
    print(f"   price = {price}, 类型: {type(price)}")

    # 科学计数法表示浮点数
    scientific_num = 1.5e3  # e3 表示 10 的 3 次方，即 1.5 * 1000 = 1500.0
    print(f"   科学计数法 1.5e3 = {scientific_num}")

    # 注意：浮点数运算存在精度问题
    # 这是因为计算机使用二进制存储浮点数，某些十进制小数无法精确表示
    float_error = 0.1 + 0.2  # 理论上应为 0.3，但实际计算结果有微小误差
    print(f"   浮点数精度问题：0.1 + 0.2 = {float_error}")  # 输出 0.30000000000000004

    # ---------- 3. 字符串类型（str）----------
    # 字符串是字符序列，用单引号、双引号或三引号包裹
    # 单引号和双引号效果相同，三引号用于多行字符串
    name = "张三"          # 使用双引号定义字符串
    greeting = 'Hello'     # 使用单引号定义字符串，效果与双引号相同
    print(f"\n3. 字符串类型 (str):")
    print(f"   name = {name}, 类型: {type(name)}")
    print(f"   greeting = {greeting}")

    # 三引号可以定义多行字符串，保留换行格式
    multi_line = """第一行
第二行
第三行"""
    print(f"   多行字符串:\n   {multi_line.replace(chr(10), chr(10) + '   ')}")

    # 字符串是不可变的（immutable），不能修改其中的某个字符
    # name[0] = "李"  # 这行代码会报错：TypeError: 'str' object does not support item assignment

    # ---------- 4. 布尔类型（bool）----------
    # 布尔类型只有两个值：True（真）和 False（假）
    # 注意首字母大写，这与其他语言（如 JavaScript 的 true/false）不同
    is_student = True   # 定义布尔变量，表示是否为学生
    has_graduated = False  # 定义布尔变量，表示是否已毕业
    print(f"\n4. 布尔类型 (bool):")
    print(f"   is_student = {is_student}, 类型: {type(is_student)}")
    print(f"   has_graduated = {has_graduated}")

    # 布尔值本质上是整数的子类：True == 1，False == 0
    print(f"   True + 1 = {True + 1}")   # 输出 2，True 被当作 1 参与运算
    print(f"   False + 1 = {False + 1}")  # 输出 1，False 被当作 0 参与运算

    # ---------- 5. 列表类型（list）----------
    # 列表是有序、可变的元素集合，可以包含不同类型的元素
    # 列表用方括号 [] 定义，元素之间用逗号分隔
    fruits = ["苹果", "香蕉", "橙子", "葡萄"]  # 定义一个字符串列表
    print(f"\n5. 列表类型 (list):")
    print(f"   fruits = {fruits}, 类型: {type(fruits)}")

    # 列表支持索引访问，索引从 0 开始
    print(f"   第一个元素：fruits[0] = {fruits[0]}")  # 苹果
    print(f"   最后一个元素：fruits[-1] = {fruits[-1]}")  # 葡萄，负索引表示从末尾开始数

    # 列表是可变的，可以修改、添加、删除元素
    fruits[1] = "芒果"  # 修改第二个元素（索引为1）为"芒果"
    print(f"   修改后：{fruits}")

    fruits.append("西瓜")  # append() 方法在列表末尾添加一个元素
    print(f"   添加元素后：{fruits}")

    # 列表可以包含不同类型的元素（这是 Python 动态类型的特点）
    mixed_list = [1, "hello", 3.14, True, [1, 2, 3]]
    print(f"   混合类型列表：{mixed_list}")

    # ---------- 6. 元组类型（tuple）----------
    # 元组是有序、不可变的元素集合，类似于列表但不能修改
    # 元组用圆括号 () 定义，元素之间用逗号分隔
    # 设计思路：元组用于存储不应修改的数据，提供数据安全性，且性能略优于列表
    coordinates = (10.5, 20.3, 30.1)  # 定义一个三维坐标元组
    print(f"\n6. 元组类型 (tuple):")
    print(f"   coordinates = {coordinates}, 类型: {type(coordinates)}")

    # 元组同样支持索引访问
    print(f"   x 坐标：coordinates[0] = {coordinates[0]}")

    # 元组是不可变的，不能修改元素
    # coordinates[0] = 15.0  # 这行代码会报错：TypeError: 'tuple' object does not support item assignment

    # 特殊情况：只有一个元素的元组需要加逗号，否则会被当作普通括号
    single_tuple = (5,)  # 注意逗号，这是一个包含单个元素的元组
    not_tuple = (5)      # 没有逗号，这只是一个整数 5
    print(f"   单元素元组：{single_tuple}, 类型: {type(single_tuple)}")
    print(f"   非元组（只是括号）：{not_tuple}, 类型: {type(not_tuple)}")

    # ---------- 7. 字典类型（dict）----------
    # 字典是键值对（key-value）的无序集合，通过键来访问值
    # 字典用花括号 {} 定义，键和值之间用冒号 : 分隔，键值对之间用逗号分隔
    # 设计思路：字典提供 O(1) 时间复杂度的查找操作，适合存储需要快速检索的数据
    person = {
        "name": "李四",      # 键 "name" 对应值 "李四"
        "age": 30,           # 键 "age" 对应值 30
        "city": "北京",      # 键 "city" 对应值 "北京"
        "is_student": False  # 键 "is_student" 对应值 False
    }
    print(f"\n7. 字典类型 (dict):")
    print(f"   person = {person}, 类型: {type(person)}")

    # 通过键访问值
    print(f"   姓名：person['name'] = {person['name']}")

    # 字典是可变的，可以修改值、添加新的键值对
    person["age"] = 31  # 修改已有键的值
    person["job"] = "工程师"  # 添加新的键值对
    print(f"   修改后：{person}")

    # 字典的键必须是不可变类型（如字符串、数字、元组），列表不能作为键
    # 这是因为字典使用哈希表实现，键需要是可哈希的（hashable）

    # ---------- 8. 集合类型（set）----------
    # 集合是无序、不重复的元素集合
    # 集合同样用花括号 {} 定义，但只有元素没有键值对
    # 设计思路：集合用于去重和集合运算（交集、并集、差集等）
    numbers_set = {1, 2, 3, 4, 5, 3, 2}  # 重复的元素会被自动去除
    print(f"\n8. 集合类型 (set):")
    print(f"   numbers_set = {numbers_set}, 类型: {type(numbers_set)}")
    # 输出 {1, 2, 3, 4, 5}，重复的 2 和 3 被自动去除

    # 集合的基本操作
    numbers_set.add(6)   # 添加元素 6
    numbers_set.remove(1)  # 移除元素 1
    print(f"   添加6、移除1后：{numbers_set}")

    # 集合运算
    set_a = {1, 2, 3, 4}
    set_b = {3, 4, 5, 6}
    print(f"   set_a = {set_a}, set_b = {set_b}")
    print(f"   并集（|）：{set_a | set_b}")   # 两个集合的所有元素，{1, 2, 3, 4, 5, 6}
    print(f"   交集（&）：{set_a & set_b}")   # 两个集合共有的元素，{3, 4}
    print(f"   差集（-）：{set_a - set_b}")   # 在 a 中但不在 b 中的元素，{1, 2}

    # 注意：空集合不能用 {} 创建（{} 是空字典），必须用 set()
    empty_set = set()   # 正确的空集合创建方式
    empty_dict = {}     # 这是空字典，不是空集合
    print(f"   空集合：{empty_set}, 类型: {type(empty_set)}")
    print(f"   空字典：{empty_dict}, 类型: {type(empty_dict)}")

    # ---------- 9. 空值（None）----------
    # None 是 Python 中的空值，表示"什么都没有"
    # None 有自己的类型 NoneType，常用于函数没有返回值的情况
    result = None  # 定义一个值为 None 的变量
    print(f"\n9. 空值 (None):")
    print(f"   result = {result}, 类型: {type(result)}")


# ==============================================================================
# 第二部分：运算符
# ==============================================================================
# Python 支持多种运算符，用于对变量和值进行操作
# 主要包括：算术运算符、比较运算符、逻辑运算符、赋值运算符、位运算符等
# ==============================================================================

def demo_operators():
    """
    函数名：demo_operators
    功能：演示 Python 各种运算符的使用
    参数：无
    返回值：无
    设计思路：分类展示不同类型的运算符及其运算效果，帮助理解运算符优先级和规则。
    """
    print("\n" + "=" * 60)
    print("【二、运算符演示】")
    print("=" * 60)

    # ---------- 1. 算术运算符 ----------
    # 用于基本的数学运算
    a = 10  # 定义变量 a，值为 10
    b = 3   # 定义变量 b，值为 3
    print(f"\n1. 算术运算符（a={a}, b={b}）:")
    print(f"   加法：a + b = {a + b}")        # 13，普通加法
    print(f"   减法：a - b = {a - b}")        # 7，普通减法
    print(f"   乘法：a * b = {a * b}")        # 30，普通乘法
    print(f"   除法：a / b = {a / b}")        # 3.333...，注意结果是浮点数
    print(f"   整除：a // b = {a // b}")      # 3，向下取整（地板除）
    print(f"   取余：a % b = {a % b}")        # 1，取余数（模运算）
    print(f"   幂运算：a ** b = {a ** b}")    # 1000，a 的 b 次方

    # ---------- 2. 比较运算符 ----------
    # 用于比较两个值，结果为布尔值 True 或 False
    x = 5   # 定义变量 x
    y = 10  # 定义变量 y
    print(f"\n2. 比较运算符（x={x}, y={y}）:")
    print(f"   等于：x == y = {x == y}")        # False，判断是否相等
    print(f"   不等于：x != y = {x != y}")      # True，判断是否不相等
    print(f"   大于：x > y = {x > y}")          # False，判断是否大于
    print(f"   小于：x < y = {x < y}")          # True，判断是否小于
    print(f"   大于等于：x >= y = {x >= y}")    # False，判断是否大于等于
    print(f"   小于等于：x <= y = {x <= y}")    # True，判断是否小于等于

    # ---------- 3. 逻辑运算符 ----------
    # 用于组合多个布尔表达式
    p = True   # 定义布尔变量 p
    q = False  # 定义布尔变量 q
    print(f"\n3. 逻辑运算符（p={p}, q={q}）:")
    print(f"   与（and）：p and q = {p and q}")  # False，两边都为 True 才为 True
    print(f"   或（or）：p or q = {p or q}")     # True，有一边为 True 就为 True
    print(f"   非（not）：not p = {not p}")      # False，取反

    # 短路求值特性：
    # - and：如果左边为 False，直接返回左边的值，不再计算右边
    # - or：如果左边为 True，直接返回左边的值，不再计算右边
    # 这在某些情况下可以提高性能，也可能导致右侧代码不执行
    print(f"   短路与示例：0 and 100 = {0 and 100}")  # 返回 0（被视为 False），不计算 100
    print(f"   短路或示例：'hello' or 'world' = {'hello' or 'world'}")  # 返回 'hello'（被视为 True）

    # ---------- 4. 赋值运算符 ----------
    # 用于给变量赋值
    print(f"\n4. 赋值运算符:")
    n = 10  # 基本赋值，将 10 赋给变量 n
    print(f"   n = {n}")
    n += 5  # 等价于 n = n + 5，加法赋值
    print(f"   n += 5 后：n = {n}")
    n -= 3  # 等价于 n = n - 3，减法赋值
    print(f"   n -= 3 后：n = {n}")
    n *= 2  # 等价于 n = n * 2，乘法赋值
    print(f"   n *= 2 后：n = {n}")
    n /= 4  # 等价于 n = n / 4，除法赋值（结果为浮点数）
    print(f"   n /= 4 后：n = {n}")
    n //= 3  # 等价于 n = n // 3，整除赋值
    print(f"   n //= 3 后：n = {n}")
    n %= 2  # 等价于 n = n % 2，取余赋值
    print(f"   n %= 2 后：n = {n}")
    n **= 3  # 等价于 n = n ** 3，幂运算赋值
    print(f"   n **= 3 后：n = {n}")

    # ---------- 5. 成员运算符 ----------
    # 用于判断元素是否在序列中
    fruits = ["苹果", "香蕉", "橙子"]
    print(f"\n5. 成员运算符（fruits={fruits}）:")
    print(f"   '苹果' in fruits = {'苹果' in fruits}")   # True，判断元素是否在列表中
    print(f"   '葡萄' in fruits = {'葡萄' in fruits}")   # False
    print(f"   '葡萄' not in fruits = {'葡萄' not in fruits}")  # True，判断是否不在

    # ---------- 6. 身份运算符 ----------
    # 用于判断两个变量是否引用同一个对象（即内存地址是否相同）
    # 注意：is 比较的是内存地址（身份），== 比较的是值（内容）
    print(f"\n6. 身份运算符:")
    list1 = [1, 2, 3]   # 创建一个列表对象，list1 指向它
    list2 = [1, 2, 3]   # 创建另一个列表对象，虽然值相同，但是不同的对象
    list3 = list1       # list3 指向 list1 同一个对象
    print(f"   list1 = {list1}, list2 = {list2}, list3 = list1")
    print(f"   list1 == list2：{list1 == list2}")     # True，值相等
    print(f"   list1 is list2：{list1 is list2}")     # False，不是同一个对象
    print(f"   list1 is list3：{list1 is list3}")     # True，是同一个对象
    print(f"   list1 is not list2：{list1 is not list2}")  # True
    # 可以用 id() 函数查看对象的内存地址
    print(f"   id(list1) = {id(list1)}, id(list2) = {id(list2)}, id(list3) = {id(list3)}")


# ==============================================================================
# 第三部分：控制流
# ==============================================================================
# 控制流用于控制程序的执行顺序，包括条件判断和循环
# 主要结构：
#   - 条件判断：if / elif / else
#   - 循环：for / while
#   - 循环控制：break / continue / pass
# ==============================================================================

def demo_control_flow():
    """
    函数名：demo_control_flow
    功能：演示 Python 控制流语句（条件判断、循环、break/continue）
    参数：无
    返回值：无
    设计思路：通过具体示例展示各种控制流结构的执行流程，
             帮助理解程序是如何根据条件选择不同执行路径的。
    """
    print("\n" + "=" * 60)
    print("【三、控制流演示】")
    print("=" * 60)

    # ---------- 1. 条件判断（if / elif / else）----------
    # if 语句根据条件是否成立，选择执行不同的代码块
    # Python 使用缩进（通常是4个空格）来表示代码块，而不是花括号
    print("\n1. 条件判断（if/elif/else）:")

    score = 85  # 定义分数变量
    print(f"   分数：{score}")

    # if 是必须的，elif 可以有多个，else 是可选的
    # 判断顺序是从上到下，第一个满足条件的分支会被执行，之后的分支不再判断
    if score >= 90:          # 判断分数是否 >= 90
        print("   等级：优秀（A）")  # 如果条件成立，执行这行
    elif score >= 80:        # 如果上一个条件不成立，判断是否 >= 80
        print("   等级：良好（B）")  # 85 >= 80，所以执行这行
    elif score >= 60:        # 如果上一个条件不成立，判断是否 >= 60
        print("   等级：及格（C）")
    else:                    # 以上条件都不成立时执行
        print("   等级：不及格（D）")

    # 注意：Python 中没有 switch/case 语句（Python 3.10+ 有 match/case，但这里不做介绍）
    # 通常用 if/elif/else 或字典来实现类似功能

    # ---------- 2. for 循环 ----------
    # for 循环用于遍历序列（列表、元组、字符串、字典等）或其他可迭代对象
    # 语法：for 变量 in 可迭代对象:
    #           循环体
    print("\n2. for 循环:")

    # 遍历列表
    fruits = ["苹果", "香蕉", "橙子"]
    print("   遍历水果列表：")
    for fruit in fruits:  # 每次循环，fruit 依次取列表中的每个元素
        print(f"     - {fruit}")  # 缩进表示属于循环体的代码

    # 使用 range() 函数生成数字序列进行循环
    # range(n) 生成 0 到 n-1 的整数序列
    print("   range(5) 生成的数字：")
    for i in range(5):  # i 依次取 0, 1, 2, 3, 4
        print(f"     i = {i}")

    # range(start, end) 生成 start 到 end-1 的整数序列
    print("   range(2, 7) 生成的数字：", end="")
    for i in range(2, 7):  # 2, 3, 4, 5, 6
        print(i, end=" ")  # end=" " 表示不换行，用空格分隔
    print()  # 换行

    # range(start, end, step) 可以指定步长
    print("   range(0, 10, 2) 生成的数字：", end="")
    for i in range(0, 10, 2):  # 0, 2, 4, 6, 8
        print(i, end=" ")
    print()

    # 遍历字典
    person = {"name": "王五", "age": 25, "city": "上海"}
    print("   遍历字典的键：")
    for key in person:  # 直接遍历字典，默认遍历的是键
        print(f"     {key}: {person[key]}")

    print("   使用 items() 遍历键值对：")
    for key, value in person.items():  # items() 返回键值对元组，可以用两个变量分别接收
        print(f"     {key} -> {value}")

    # enumerate() 函数：同时获取索引和元素
    print("   使用 enumerate 获取索引和元素：")
    for index, fruit in enumerate(fruits):  # enumerate 返回 (索引, 元素) 元组
        print(f"     索引 {index}: {fruit}")

    # ---------- 3. while 循环 ----------
    # while 循环在条件为 True 时重复执行循环体
    # 语法：while 条件:
    #           循环体
    print("\n3. while 循环:")

    count = 1  # 初始化计数器
    print("   count 从 1 加到 5：")
    while count <= 5:  # 当 count <= 5 时，继续循环
        print(f"     count = {count}")
        count += 1  # 计数器加 1，这一步很重要，否则会导致死循环
    # 循环结束后 count 的值为 6
    print(f"   循环结束后，count = {count}")

    # ---------- 4. break 和 continue ----------
    # break：立即终止整个循环，跳出循环体
    # continue：跳过当前循环的剩余语句，直接进入下一次循环
    print("\n4. break 和 continue:")

    # break 示例：找到第一个偶数后停止
    print("   找到第一个偶数就停止（break）：")
    for num in [1, 3, 5, 7, 8, 9, 10]:
        if num % 2 == 0:  # 判断是否为偶数
            print(f"     找到偶数：{num}，停止循环")
            break  # 遇到 break，立即跳出整个 for 循环
        print(f"     {num} 是奇数，继续...")

    # continue 示例：只打印偶数，跳过奇数
    print("   只打印偶数，跳过奇数（continue）：", end="")
    for num in range(1, 11):  # 1 到 10
        if num % 2 != 0:  # 如果是奇数
            continue  # 跳过本次循环剩余的代码，直接开始下一次循环
        print(num, end=" ")
    print()

    # ---------- 5. pass 语句 ----------
    # pass 是空语句，不做任何操作，只是为了保持代码结构的完整性
    # 常用于占位，表示"这里以后会写代码，但现在先空着"
    print("\n5. pass 语句（空占位）:")
    for i in range(3):
        if i == 1:
            pass  # 什么都不做，只是占位，程序会继续执行
            print(f"   i = {i}（pass 后面的代码仍然执行）")
        else:
            print(f"   i = {i}")

    # ---------- 6. 循环中的 else 子句 ----------
    # Python 的 for/while 循环可以带 else 子句
    # 当循环正常结束（不是被 break 终止）时，执行 else 中的代码
    print("\n6. 循环的 else 子句:")

    # 正常结束的循环，else 会执行
    print("   正常结束的循环（else 会执行）：")
    for i in range(3):
        print(f"     i = {i}")
    else:  # 循环正常结束，执行 else
        print("     循环正常结束（执行了 else）")

    # 被 break 终止的循环，else 不执行
    print("   被 break 终止的循环（else 不执行）：")
    for i in range(5):
        if i == 2:
            print("     遇到 break，终止循环")
            break
        print(f"     i = {i}")
    else:  # 循环被 break 终止，不执行 else
        print("     这行不会被执行")


# ==============================================================================
# 第四部分：函数定义与参数
# ==============================================================================
# 函数是可复用的代码块，用于执行特定任务
# Python 中函数定义使用 def 关键字
# 参数类型：位置参数、关键字参数、默认参数、可变参数（*args）、关键字可变参数（**kwargs）
# ==============================================================================

def demo_functions():
    """
    函数名：demo_functions
    功能：演示 Python 函数的定义和各种参数类型的使用
    参数：无
    返回值：无
    设计思路：从简单到复杂，逐步介绍不同的函数定义方式和参数类型，
             帮助理解函数作为"一等公民"在 Python 中的重要地位。
    """
    print("\n" + "=" * 60)
    print("【四、函数定义与参数演示】")
    print("=" * 60)

    # ---------- 1. 基本函数定义 ----------
    # 使用 def 关键字定义函数，格式：def 函数名(参数列表):
    # 函数名应该具有描述性，遵循 snake_case 命名规范（小写字母，下划线分隔）
    print("\n1. 基本函数定义:")

    def greet(name):
        """
        函数名：greet
        功能：向指定的人打招呼
        参数：
            name - 要打招呼的人的名字（字符串）
        返回值：无
        说明：这是一个简单的函数，接收一个参数，打印问候语。
              三引号中的内容是文档字符串（docstring），用于说明函数的用途。
        """
        # 函数体：使用 f-string 格式化输出问候语
        print(f"   你好，{name}！欢迎学习 Python。")

    # 调用函数：函数名(参数)
    greet("小明")  # 调用 greet 函数，传入参数 "小明"
    greet("小红")  # 再次调用，传入不同参数

    # ---------- 2. 带返回值的函数 ----------
    # 使用 return 语句返回函数的计算结果
    # return 会立即结束函数执行，并将结果返回给调用者
    print("\n2. 带返回值的函数:")

    def add(a, b):
        """
        函数名：add
        功能：计算两个数的和并返回
        参数：
            a - 第一个加数（数字）
            b - 第二个加数（数字）
        返回值：a + b 的结果
        """
        result = a + b  # 计算两数之和
        return result   # 使用 return 返回计算结果

    # 调用函数并接收返回值
    sum_result = add(3, 5)  # 调用 add 函数，结果赋给 sum_result
    print(f"   add(3, 5) = {sum_result}")

    # 函数可以返回多个值（本质上是返回一个元组）
    def divide(a, b):
        """
        函数名：divide
        功能：计算两数相除的商和余数
        参数：
            a - 被除数
            b - 除数
        返回值：(商, 余数) 组成的元组
        """
        quotient = a // b  # 计算商（整除）
        remainder = a % b  # 计算余数
        return quotient, remainder  # 返回多个值，自动封装成元组

    q, r = divide(17, 5)  # 接收多个返回值，可以用多个变量分别接收（解包）
    print(f"   divide(17, 5)：商 = {q}, 余数 = {r}")

    # ---------- 3. 默认参数 ----------
    # 函数参数可以设置默认值，调用时如果不提供该参数，就使用默认值
    # 注意：默认参数必须放在非默认参数的后面
    print("\n3. 默认参数:")

    def power(base, exponent=2):
        """
        函数名：power
        功能：计算 base 的 exponent 次方
        参数：
            base - 底数
            exponent - 指数，默认值为 2（即默认计算平方）
        返回值：计算结果
        设计思路：将最常用的指数 2 设为默认值，简化大多数情况下的调用。
        """
        return base ** exponent  # 返回 base 的 exponent 次方

    # 只传一个参数，使用默认的 exponent=2
    print(f"   power(3) = {power(3)}")       # 3^2 = 9
    # 传两个参数，使用传入的 exponent
    print(f"   power(3, 3) = {power(3, 3)}")  # 3^3 = 27

    # 重要：默认参数必须是不可变对象（如 None、数字、字符串、元组）
    # 不要使用可变对象（如列表）作为默认参数，否则会有意外行为
    # 错误示例：def func(items=[]): items.append(1); return items
    # 正确做法：使用 None 作为默认值，在函数内部初始化
    def add_item(item, lst=None):
        """
        函数名：add_item
        功能：向列表中添加一个元素
        参数：
            item - 要添加的元素
            lst - 目标列表，默认为 None，表示创建新列表
        返回值：添加元素后的列表
        设计思路：使用 None 作为默认值，避免可变默认参数的陷阱。
        """
        if lst is None:  # 如果没有传入列表（使用默认值 None）
            lst = []     # 创建一个新的空列表
        lst.append(item)  # 向列表中添加元素
        return lst        # 返回列表

    print(f"   add_item(1) = {add_item(1)}")  # [1]
    print(f"   add_item(2) = {add_item(2)}")  # [2]，每次调用都创建新列表，不会累积

    # ---------- 4. 位置参数和关键字参数 ----------
    # 位置参数：按参数定义的顺序传入参数
    # 关键字参数：通过参数名指定传入哪个参数，顺序可以任意
    print("\n4. 位置参数和关键字参数:")

    def introduce(name, age, city):
        """
        函数名：introduce
        功能：打印个人介绍信息
        参数：
            name - 姓名
            age - 年龄
            city - 所在城市
        返回值：无
        """
        print(f"   大家好，我叫{name}，今年{age}岁，来自{city}。")

    # 位置参数调用：按定义顺序传入
    print("   位置参数调用：")
    introduce("张三", 25, "北京")

    # 关键字参数调用：通过参数名指定，顺序可以任意
    print("   关键字参数调用（顺序可变）：")
    introduce(age=30, city="上海", name="李四")

    # 混合使用：位置参数必须在关键字参数前面
    print("   混合调用（位置+关键字）：")
    introduce("王五", city="广州", age=28)

    # ---------- 5. 可变参数（*args）----------
    # *args 可以接收任意数量的位置参数，这些参数会被封装成一个元组
    # *args 的名字不是固定的，可以是其他名字，但约定俗成用 args
    print("\n5. 可变位置参数（*args）:")

    def sum_all(*args):
        """
        函数名：sum_all
        功能：计算任意数量数字的和
        参数：
            *args - 任意数量的数字参数，内部表现为元组
        返回值：所有数字的和
        设计思路：使用 *args 接收不确定数量的参数，让函数更灵活。
        """
        print(f"     args 的类型：{type(args)}, 值：{args}")
        total = 0  # 初始化总和为 0
        for num in args:  # 遍历 args 元组中的每个元素
            total += num  # 累加到 total
        return total  # 返回总和

    print("   sum_all(1, 2, 3)：")
    result = sum_all(1, 2, 3)  # 传入 3 个参数
    print(f"     结果：{result}")

    print("   sum_all(1, 2, 3, 4, 5)：")
    result = sum_all(1, 2, 3, 4, 5)  # 传入 5 个参数
    print(f"     结果：{result}")

    # 可以用 * 来"解包"列表或元组，将其中的元素作为位置参数传入
    numbers = [10, 20, 30]
    print(f"   解包列表 {numbers} 作为参数：")
    result = sum_all(*numbers)  # 等价于 sum_all(10, 20, 30)
    print(f"     结果：{result}")

    # ---------- 6. 关键字可变参数（**kwargs）----------
    # **kwargs 可以接收任意数量的关键字参数，这些参数会被封装成一个字典
    # **kwargs 的名字不是固定的，但约定俗成用 kwargs（keyword arguments）
    print("\n6. 关键字可变参数（**kwargs）:")

    def print_info(**kwargs):
        """
        函数名：print_info
        功能：打印任意数量的关键字参数信息
        参数：
            **kwargs - 任意数量的关键字参数，内部表现为字典
        返回值：无
        设计思路：使用 **kwargs 接收不确定数量的关键字参数，
                 常用于配置选项、传递额外参数等场景。
        """
        print(f"     kwargs 的类型：{type(kwargs)}, 值：{kwargs}")
        for key, value in kwargs.items():  # 遍历字典的键值对
            print(f"     {key}: {value}")

    print("   print_info(name='小明', age=20)：")
    print_info(name="小明", age=20)

    print("   print_info(name='小红', age=22, major='计算机')：")
    print_info(name="小红", age=22, major="计算机")

    # 可以用 ** 来"解包"字典，将其中的键值对作为关键字参数传入
    info_dict = {"name": "小刚", "age": 25, "city": "深圳"}
    print(f"   解包字典 {info_dict} 作为参数：")
    print_info(**info_dict)  # 等价于 print_info(name="小刚", age=25, city="深圳")

    # ---------- 7. 参数组合 ----------
    # 参数定义的顺序必须是：位置参数 -> 默认参数 -> *args -> 命名关键字参数 -> **kwargs
    print("\n7. 参数组合（综合示例）:")

    def complex_func(a, b, c=0, *args, **kwargs):
        """
        函数名：complex_func
        功能：演示多种参数类型的组合使用
        参数：
            a, b - 位置参数（必填）
            c - 默认参数（可选，默认为 0）
            *args - 可变位置参数
            **kwargs - 可变关键字参数
        返回值：无
        说明：参数定义顺序很重要，必须遵循：位置 -> 默认 -> *args -> **kwargs
        """
        print(f"     a = {a}, b = {b}, c = {c}")
        print(f"     args = {args}")
        print(f"     kwargs = {kwargs}")

    print("   调用 complex_func(1, 2, 3, 4, 5, x=6, y=7)：")
    complex_func(1, 2, 3, 4, 5, x=6, y=7)
    # a=1, b=2, c=3, args=(4, 5), kwargs={'x': 6, 'y': 7}


# ==============================================================================
# 第五部分：推导式
# ==============================================================================
# 推导式（Comprehension）是 Python 的一种简洁语法，用于从已有序列创建新序列
# 包括：列表推导式、字典推导式、集合推导式、生成器表达式
# 推导式的优点：代码更简洁、执行效率更高（底层优化）
# ==============================================================================

def demo_comprehensions():
    """
    函数名：demo_comprehensions
    功能：演示 Python 推导式（列表、字典、集合）和生成器表达式
    参数：无
    返回值：无
    设计思路：先展示传统写法，再展示推导式写法，对比突出推导式的简洁性。
             每种推导式都配有多个示例，由浅入深。
    """
    print("\n" + "=" * 60)
    print("【五、推导式演示】")
    print("=" * 60)

    # ---------- 1. 列表推导式 ----------
    # 语法：[表达式 for 变量 in 可迭代对象 if 条件]
    # 作用：快速生成一个新列表
    print("\n1. 列表推导式:")

    # 示例1：生成 1~10 的平方列表
    # 传统写法（使用 for 循环）：
    squares_loop = []  # 创建空列表
    for x in range(1, 11):  # 遍历 1 到 10
        squares_loop.append(x ** 2)  # 将平方值添加到列表
    print(f"   传统写法：{squares_loop}")

    # 列表推导式写法（一行代码搞定）：
    # 语法解读：对 range(1, 11) 中的每个 x，计算 x**2，将结果收集到新列表中
    squares_comp = [x ** 2 for x in range(1, 11)]
    print(f"   列表推导式：{squares_comp}")

    # 示例2：带条件的列表推导式（只保留偶数的平方）
    # if 子句用于过滤，只有满足条件的元素才会被包含
    even_squares = [x ** 2 for x in range(1, 11) if x % 2 == 0]
    print(f"   偶数的平方：{even_squares}")

    # 示例3：多重循环的列表推导式
    # 生成全排列：两个列表中元素的所有组合
    list_a = [1, 2, 3]
    list_b = ['a', 'b']
    # 等价于两层 for 循环
    combinations = [(x, y) for x in list_a for y in list_b]
    print(f"   全排列组合：{combinations}")

    # 等价的传统写法：
    # combinations = []
    # for x in list_a:
    #     for y in list_b:
    #         combinations.append((x, y))

    # 示例4：字符串处理
    words = ["hello", "world", "python"]
    # 将每个单词转为大写，并取长度
    word_info = [(word.upper(), len(word)) for word in words]
    print(f"   单词信息（大写, 长度）：{word_info}")

    # ---------- 2. 字典推导式 ----------
    # 语法：{键表达式: 值表达式 for 变量 in 可迭代对象 if 条件}
    # 作用：快速生成一个新字典
    print("\n2. 字典推导式:")

    # 示例1：将两个列表组合成字典
    keys = ["name", "age", "city"]
    values = ["张三", 25, "北京"]
    # 使用 zip() 将两个列表打包成键值对元组
    person_dict = {k: v for k, v in zip(keys, values)}
    print(f"   从两个列表生成字典：{person_dict}")

    # 示例2：字典的键值互换
    original = {"a": 1, "b": 2, "c": 3}
    reversed_dict = {v: k for k, v in original.items()}
    print(f"   键值互换：{reversed_dict}")

    # 示例3：带条件的字典推导式
    # 筛选出值大于 1 的键值对
    filtered = {k: v for k, v in original.items() if v > 1}
    print(f"   筛选值大于1的：{filtered}")

    # 示例4：使用 enumerate 生成索引字典
    fruits = ["苹果", "香蕉", "橙子"]
    index_dict = {index: fruit for index, fruit in enumerate(fruits)}
    print(f"   索引字典：{index_dict}")

    # ---------- 3. 集合推导式 ----------
    # 语法：{表达式 for 变量 in 可迭代对象 if 条件}
    # 与列表推导式类似，但使用花括号，结果是集合（自动去重、无序）
    print("\n3. 集合推导式:")

    # 示例1：生成一组数字的平方集合（自动去重）
    numbers = [1, 2, 3, 2, 4, 3, 5, 1]
    # 列表推导式的结果是列表，保留重复元素
    squares_list = [x ** 2 for x in numbers]
    print(f"   列表推导式：{squares_list}")

    # 集合推导式的结果是集合，自动去重
    squares_set = {x ** 2 for x in numbers}
    print(f"   集合推导式：{squares_set}")

    # 示例2：提取字符串中的唯一字符
    text = "hello world"
    unique_chars = {char for char in text if char != ' '}  # 排除空格
    print(f"   字符串中的唯一字符：{unique_chars}")

    # ---------- 4. 生成器表达式 ----------
    # 语法：(表达式 for 变量 in 可迭代对象 if 条件)
    # 与列表推导式类似，但使用圆括号，结果是一个生成器对象
    # 生成器不会一次性生成所有元素，而是按需生成，节省内存
    # 适合处理大量数据或无限序列
    print("\n4. 生成器表达式:")

    # 示例1：生成 1~10 的平方的生成器
    # 注意：使用圆括号，不是方括号
    gen = (x ** 2 for x in range(1, 11))
    print(f"   生成器对象：{gen}")
    print(f"   生成器类型：{type(gen)}")

    # 生成器是可迭代对象，可以用 for 循环遍历
    # 遍历时才会逐个计算值，而不是一次性全部生成
    print("   遍历生成器（逐个生成值）：")
    for value in gen:
        print(f"     {value}", end=" ")
    print()

    # 重要：生成器只能遍历一次，遍历完就"消耗"完了
    # 再次遍历不会有任何输出，因为生成器已经到达末尾
    print("   再次遍历生成器（已经没有元素了）：", end="")
    for value in gen:
        print(value, end=" ")  # 什么都不会打印
    print("（空）")

    # 示例2：生成器表达式的内存优势
    # 计算 1~1000000 的平方和
    # 使用列表推导式：需要创建一个包含100万元素的列表，占用大量内存
    # 使用生成器表达式：逐个生成值，只占用很少内存
    # sum() 函数可以直接接收生成器表达式作为参数
    import sys  # 导入 sys 模块用于查看内存占用

    # 列表方式
    squares_list_big = [x ** 2 for x in range(1000000)]
    list_memory = sys.getsizeof(squares_list_big)
    print(f"   列表推导式（100万元素）占用内存：{list_memory / 1024 / 1024:.2f} MB")

    # 生成器方式
    squares_gen_big = (x ** 2 for x in range(1000000))
    gen_memory = sys.getsizeof(squares_gen_big)
    print(f"   生成器表达式占用内存：{gen_memory} 字节")

    # 计算总和（生成器逐个产生值，不占用额外内存）
    total = sum(x ** 2 for x in range(1000000))
    print(f"   1~999999 的平方和：{total}")

    # 删除大列表，释放内存（只是演示，Python 有自动垃圾回收）
    del squares_list_big
    del squares_gen_big


# ==============================================================================
# 主函数：运行所有演示
# ==============================================================================

def run_demo():
    """
    函数名：run_demo
    功能：运行本模块的所有演示函数
    参数：无
    返回值：无
    说明：作为模块的演示入口，被 main.py 调用。
    """
    # 打印欢迎信息
    print("\n" + "=" * 60)
    print("  Python 基础语法演示")
    print("=" * 60)

    # 依次调用各个演示函数
    demo_basic_types()        # 基本数据类型
    demo_operators()          # 运算符
    demo_control_flow()       # 控制流
    demo_functions()          # 函数定义与参数
    demo_comprehensions()     # 推导式

    # 打印结束信息
    print("\n" + "=" * 60)
    print("  基础语法演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================
# 当直接运行本文件时，__name__ 的值为 "__main__"
# 当本文件被其他文件 import 时，__name__ 的值为模块名 "basics.01_basics"
# 这样设计的目的是：既可以单独运行本文件进行测试，也可以作为模块被导入使用
# ==============================================================================

if __name__ == "__main__":
    # 如果直接运行本文件，则执行演示函数
    run_demo()
