# -*- coding: utf-8 -*-
"""
================================================================================
文件名：02_strings_and_files.py
所属模块：basics（基础语法）
功能说明：本文件演示 Python 字符串操作、文件IO 和异常处理。
         包括字符串方法、格式化、文件读写、上下文管理器、异常处理等内容。

运行方式：在项目根目录下执行 `python -m basics.02_strings_and_files`
================================================================================
"""

import os  # 导入 os 模块，用于操作文件路径和文件系统


# ==============================================================================
# 第一部分：字符串操作
# ==============================================================================
# 字符串是 Python 中最常用的数据类型之一
# Python 提供了丰富的字符串方法，用于处理文本数据
# 字符串是不可变的（immutable），所有字符串方法都返回新字符串
# ==============================================================================

def demo_string_operations():
    """
    函数名：demo_string_operations
    功能：演示 Python 字符串的各种操作方法
    参数：无
    返回值：无
    设计思路：分类展示字符串的常用方法，包括索引切片、查找替换、
             大小写转换、分割连接、判断方法、去除空白等。
    """
    print("=" * 60)
    print("【一、字符串操作演示】")
    print("=" * 60)

    # 定义一个示例字符串，供后续操作使用
    text = "  Hello, Python World!  "
    print(f"\n原始字符串：'{text}'")
    print(f"字符串长度：{len(text)} 个字符（包括空格）")

    # ---------- 1. 索引和切片 ----------
    # 字符串是字符序列，可以通过索引访问单个字符，通过切片访问子串
    # 索引从 0 开始，支持负数索引（从末尾开始数）
    print("\n1. 索引和切片：")

    s = "Python"  # 定义一个简单的字符串用于演示
    print(f"   示例字符串：'{s}'")

    # 索引访问：string[index]
    print(f"   第1个字符（索引0）：s[0] = '{s[0]}'")   # 'P'
    print(f"   第3个字符（索引2）：s[2] = '{s[2]}'")   # 't'
    print(f"   最后1个字符（索引-1）：s[-1] = '{s[-1]}'")  # 'n'
    print(f"   倒数第2个字符（索引-2）：s[-2] = '{s[-2]}'")  # 'o'

    # 切片操作：string[start:end:step]
    # 包含 start，不包含 end（左闭右开）
    # start 省略表示从开头开始，end 省略表示到结尾
    print(f"   前3个字符：s[:3] = '{s[:3]}'")      # 'Pyt'，从开头到索引3（不包含3）
    print(f"   从索引3到结尾：s[3:] = '{s[3:]}'")  # 'hon'，从索引3到结尾
    print(f"   索引1到4：s[1:4] = '{s[1:4]}'")     # 'yth'，包含1不包含4
    print(f"   步长为2：s[::2] = '{s[::2]}'")      # 'Pto'，每隔一个字符取一个
    print(f"   反转字符串：s[::-1] = '{s[::-1]}'") # 'nohtyP'，步长为-1表示从后往前

    # ---------- 2. 查找与替换 ----------
    print("\n2. 查找与替换：")

    s = "Hello, Python World! Python is great!"
    print(f"   示例字符串：'{s}'")

    # find()：查找子串第一次出现的位置，返回索引；找不到返回 -1
    pos = s.find("Python")  # 查找 "Python" 第一次出现的位置
    print(f"   find('Python') = {pos}（第一次出现的索引位置）")

    # rfind()：从右边开始查找，返回最后一次出现的位置
    pos_r = s.rfind("Python")  # 查找 "Python" 最后一次出现的位置
    print(f"   rfind('Python') = {pos_r}（最后一次出现的索引位置）")

    # index()：与 find() 类似，但找不到会抛出 ValueError 异常
    pos_idx = s.index("World")
    print(f"   index('World') = {pos_idx}")

    # count()：统计子串出现的次数
    count = s.count("Python")  # 统计 "Python" 出现的次数
    print(f"   count('Python') = {count}（出现次数）")

    # replace()：替换子串，返回新字符串（原字符串不变）
    # replace(old, new, count)，count 可选，表示最多替换几次
    new_s = s.replace("Python", "Java")  # 将所有 "Python" 替换为 "Java"
    print(f"   replace('Python', 'Java') = '{new_s}'")

    # 只替换前 1 次
    new_s2 = s.replace("Python", "Java", 1)
    print(f"   只替换1次：'{new_s2}'")

    # ---------- 3. 大小写转换 ----------
    print("\n3. 大小写转换：")

    s = "Hello World"
    print(f"   原始：'{s}'")

    print(f"   全部大写：s.upper() = '{s.upper()}'")    # 转为大写
    print(f"   全部小写：s.lower() = '{s.lower()}'")    # 转为小写
    print(f"   首字母大写：s.capitalize() = '{s.capitalize()}'")  # 首字母大写，其余小写
    print(f"   每个单词首字母大写：s.title() = '{s.title()}'")    # 每个单词首字母大写
    print(f"   大小写互换：s.swapcase() = '{s.swapcase()}'")      # 大写变小写，小写变大写

    # ---------- 4. 分割与连接 ----------
    print("\n4. 分割与连接：")

    # split()：将字符串按指定分隔符分割成列表
    s = "苹果,香蕉,橙子,葡萄"
    print(f"   原始字符串：'{s}'")
    fruits = s.split(",")  # 按逗号分割，返回列表
    print(f"   split(',') 分割结果：{fruits}")

    # split() 不指定分隔符时，按空白字符（空格、换行、制表符等）分割
    sentence = "Hello   World   Python"
    words = sentence.split()  # 按空白分割，自动忽略连续的空白
    print(f"   按空白分割：{words}")

    # join()：将列表（或可迭代对象）中的字符串连接成一个字符串
    # 语法：分隔符.join(列表)
    fruits_list = ["苹果", "香蕉", "橙子"]
    result = "、".join(fruits_list)  # 用顿号连接列表中的元素
    print(f"   '、'.join(列表) = '{result}'")

    # 用换行符连接
    lines = ["第一行", "第二行", "第三行"]
    result = "\n".join(lines)
    print(f"   用换行符连接：\n   {result.replace(chr(10), chr(10) + '   ')}")

    # ---------- 5. 去除空白 ----------
    print("\n5. 去除空白字符：")

    s = "   Hello World   "
    print(f"   原始：'{s}'")
    print(f"   去除两端空白：s.strip() = '{s.strip()}'")     # 去除左右两端的空白
    print(f"   去除左边空白：s.lstrip() = '{s.lstrip()}'")   # 只去除左边的空白
    print(f"   去除右边空白：s.rstrip() = '{s.rstrip()}'")   # 只去除右边的空白

    # 也可以指定要去除的字符
    s2 = "###Hello###"
    print(f"   去除指定字符：s.strip('#') = '{s2.strip('#')}'")

    # ---------- 6. 判断方法 ----------
    # 这些方法返回布尔值，用于判断字符串的特征
    print("\n6. 字符串判断方法：")

    test_strings = ["abc", "123", "abc123", "   ", "ABC", ""]
    for s in test_strings:
        print(f"   '{s}':")
        print(f"     isalpha()={s.isalpha()}（是否全是字母）")
        print(f"     isdigit()={s.isdigit()}（是否全是数字）")
        print(f"     isalnum()={s.isalnum()}（是否全是字母或数字）")
        print(f"     isspace()={s.isspace()}（是否全是空白字符）")
        print(f"     isupper()={s.isupper()}（是否全是大写字母）")
        print(f"     islower()={s.islower()}（是否全是小写字母）")
        print()

    # ---------- 7. 开头和结尾判断 ----------
    print("\n7. 开头和结尾判断：")

    filename = "hello_world.py"
    print(f"   文件名：'{filename}'")
    print(f"   startswith('hello') = {filename.startswith('hello')}（是否以指定字符串开头）")
    print(f"   endswith('.py') = {filename.endswith('.py')}（是否以指定字符串结尾）")
    print(f"   endswith(('.py', '.txt')) = {filename.endswith(('.py', '.txt'))}（是否以元组中任意一个结尾）")


# ==============================================================================
# 第二部分：字符串格式化
# ==============================================================================
# 字符串格式化是将变量值插入到字符串中的方法
# Python 主要有三种格式化方式：
#   1. f-string（Python 3.6+ 推荐，最简洁）
#   2. str.format() 方法（Python 2.7+/3.x 支持）
#   3. % 格式化（老式，类似 C 语言的 printf）
# ==============================================================================

def demo_string_formatting():
    """
    函数名：demo_string_formatting
    功能：演示 Python 三种字符串格式化方式
    参数：无
    返回值：无
    设计思路：通过相同的例子分别用三种方式实现，便于对比学习。
             重点介绍 f-string 的各种用法，因为它是现代 Python 的推荐方式。
    """
    print("\n" + "=" * 60)
    print("【二、字符串格式化演示】")
    print("=" * 60)

    # 定义一些变量，用于格式化演示
    name = "张三"      # 姓名
    age = 25           # 年龄
    height = 1.75      # 身高（米）
    score = 95.678     # 分数

    # ---------- 1. f-string（推荐）----------
    # Python 3.6 引入，语法：f"文本{表达式}"
    # 在花括号中可以直接写变量名或表达式
    # 优点：直观、简洁、功能强大、性能好
    print("\n1. f-string 格式化（推荐）：")

    # 基本用法：直接在花括号中写变量名
    result = f"我叫{name}，今年{age}岁"
    print(f"   基本用法：{result}")

    # 花括号中可以写任意表达式
    result = f"明年我就{age + 1}岁了"
    print(f"   表达式：{result}")

    # 调用函数或方法
    result = f"名字大写：{name.upper()}"
    print(f"   调用方法：{result}")

    # 浮点数格式化：{变量:.nf} 保留 n 位小数
    result = f"身高：{height:.2f} 米"  # 保留 2 位小数
    print(f"   保留2位小数：{result}")

    # 宽度和对齐：{变量:宽度}，默认右对齐
    # < 左对齐，> 右对齐，^ 居中对齐
    result = f"|{name:<10}|{age:>10}|"
    print(f"   宽度与对齐：{result}")

    # 数字格式化
    pi = 3.1415926535
    print(f"   π 保留3位小数：{pi:.3f}")
    print(f"   百分比：{0.856:.2%}")  # 转百分比，保留2位小数
    print(f"   千分位：{1234567.89:,}")  # 添加千分位逗号

    # 填充字符
    num = 42
    print(f"   左侧补零到5位：{num:05d}")  # 00042
    print(f"   居中，用*填充：{num:*^10}")  # ****42****

    # ---------- 2. str.format() 方法 ----------
    # 使用 format() 方法进行格式化，用 {} 作为占位符
    # 优点：功能丰富，兼容性好（Python 2.7+ 都支持）
    print("\n2. str.format() 方法：")

    # 按位置顺序填充
    result = "我叫{}，今年{}岁".format(name, age)
    print(f"   按位置：{result}")

    # 按索引填充（可以重复使用、调整顺序）
    result = "{0}今年{1}岁，{0}喜欢Python".format(name, age)
    print(f"   按索引：{result}")

    # 按关键字填充
    result = "姓名：{n}，年龄：{a}".format(n=name, a=age)
    print(f"   按关键字：{result}")

    # 格式化说明符（在冒号后面）
    result = "身高：{:.2f} 米".format(height)
    print(f"   保留2位小数：{result}")

    result = "|{:<10}|{:>10}|".format(name, age)
    print(f"   宽度与对齐：{result}")

    # ---------- 3. % 格式化（老式）----------
    # 使用 % 运算符进行格式化，类似 C 语言的 printf
    # 缺点：不够灵活，功能有限，不推荐在新代码中使用
    # 但仍需了解，因为很多老代码还在使用
    print("\n3. % 格式化（老式，了解即可）：")

    # %s 表示字符串，%d 表示整数，%f 表示浮点数
    result = "我叫%s，今年%d岁" % (name, age)
    print(f"   基本用法：{result}")

    # 浮点数格式化：%.nf 保留 n 位小数
    result = "身高：%.2f 米" % height
    print(f"   保留2位小数：{result}")

    # 宽度与对齐
    result = "|%-10s|%10d|" % (name, age)  # - 表示左对齐
    print(f"   宽度与对齐：{result}")


# ==============================================================================
# 第三部分：文件 IO
# ==============================================================================
# 文件操作是编程中非常重要的技能，用于持久化存储数据
# Python 内置了 open() 函数用于打开文件
# 文件操作的基本步骤：打开文件 -> 读写操作 -> 关闭文件
#
# 打开文件模式：
#   'r'  - 只读模式（默认），文件必须存在
#   'w'  - 写入模式，如果文件存在则清空内容，不存在则创建
#   'a'  - 追加模式，如果文件存在则在末尾追加，不存在则创建
#   'r+' - 读写模式，文件必须存在
#   'w+' - 读写模式，如果文件存在则清空，不存在则创建
#   'a+' - 追加读写模式
#   'b'  - 二进制模式（与上述模式组合使用，如 'rb', 'wb'）
#   't'  - 文本模式（默认，可省略）
# ==============================================================================

def demo_file_io():
    """
    函数名：demo_file_io
    功能：演示 Python 文件读写操作
    参数：无
    返回值：无
    设计思路：从最基本的 open/close 开始，逐步介绍上下文管理器 with、
             文本文件读写、二进制文件读写、文件指针操作等。
    """
    print("\n" + "=" * 60)
    print("【三、文件 IO 演示】")
    print("=" * 60)

    # 定义测试文件的路径，放在当前目录下的 temp_files 文件夹中
    # os.path.join() 用于拼接路径，自动处理不同系统的路径分隔符
    test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "temp_files")
    # os.path.abspath(__file__) 获取当前文件的绝对路径
    # os.path.dirname() 获取路径的目录部分
    # os.path.join() 拼接路径

    # 确保测试目录存在，不存在则创建
    if not os.path.exists(test_dir):  # 判断目录是否存在
        os.makedirs(test_dir)         # 创建目录（包括中间目录）

    test_file = os.path.join(test_dir, "test.txt")  # 测试文本文件路径
    binary_file = os.path.join(test_dir, "test.bin")  # 测试二进制文件路径

    # ---------- 1. 文本文件写入（w 模式）----------
    print("\n1. 文本文件写入（write）：")

    # 传统方式：open -> write -> close
    # 注意：如果忘记 close()，可能导致数据丢失或文件损坏
    f = open(test_file, "w", encoding="utf-8")  # 打开文件，w模式（写入），指定编码为utf-8
    # encoding 参数很重要，Windows 下默认是 gbk，可能导致乱码
    f.write("第一行：Hello World!\n")  # 写入一行，\n 表示换行
    f.write("第二行：Python 文件操作\n")
    f.write("第三行：这是一个测试文件\n")
    f.close()  # 关闭文件，必须手动关闭！
    print(f"   已写入文件：{test_file}")

    # ---------- 2. 使用 with 语句（上下文管理器）----------
    # with 语句会自动管理文件的打开和关闭
    # 即使 with 块内发生异常，文件也会被正确关闭
    # 这是 Python 推荐的文件操作方式
    print("\n2. 使用 with 语句（推荐）：")

    with open(test_file, "r", encoding="utf-8") as f:
        # as f 将打开的文件对象赋值给变量 f
        # 在这个缩进块内，文件是打开的
        content = f.read()  # 读取整个文件内容
        # with 块结束后，文件自动关闭，不需要手动调用 f.close()

    print(f"   读取的内容：\n   {content.replace(chr(10), chr(10) + '   ')}")

    # ---------- 3. 文本文件读取的多种方式 ----------
    print("\n3. 文本文件读取方式：")

    # 方式一：read() - 一次性读取整个文件
    # 适合文件不大的情况，大文件可能占用过多内存
    with open(test_file, "r", encoding="utf-8") as f:
        content = f.read()  # 读取全部内容到一个字符串
    print(f"   read() 读取全部：{len(content)} 个字符")

    # 方式二：readline() - 逐行读取
    # 每次调用读取一行，包括换行符
    print("   readline() 逐行读取：")
    with open(test_file, "r", encoding="utf-8") as f:
        line1 = f.readline()  # 读取第一行
        line2 = f.readline()  # 读取第二行
        line3 = f.readline()  # 读取第三行
        line4 = f.readline()  # 读取第四行（没有了，返回空字符串）
    print(f"     第1行：{repr(line1)}")  # repr() 显示字符串的原始表示（包括转义字符）
    print(f"     第2行：{repr(line2)}")
    print(f"     第3行：{repr(line3)}")
    print(f"     第4行（末尾）：{repr(line4)}")  # 空字符串表示到达文件末尾

    # 方式三：readlines() - 读取所有行，返回列表
    # 每行是列表的一个元素，包含换行符
    with open(test_file, "r", encoding="utf-8") as f:
        lines = f.readlines()  # 返回列表
    print(f"   readlines() 返回列表：{lines}")

    # 方式四：直接迭代文件对象（最常用、最节省内存）
    # 文件对象本身是可迭代的，每次迭代返回一行
    print("   直接迭代文件对象（推荐）：")
    with open(test_file, "r", encoding="utf-8") as f:
        for i, line in enumerate(f, 1):  # enumerate 从 1 开始计数
            # strip() 去除两端空白（包括换行符）
            print(f"     行{i}：{line.strip()}")

    # ---------- 4. 追加写入（a 模式）----------
    print("\n4. 追加写入（append）：")

    with open(test_file, "a", encoding="utf-8") as f:
        # a 模式：在文件末尾追加内容，不会清空原有内容
        f.write("第四行：这是追加的内容\n")
        f.write("第五行：追加模式不会覆盖原有内容\n")

    # 读取验证
    with open(test_file, "r", encoding="utf-8") as f:
        content = f.read()
    print(f"   追加后的内容：\n   {content.replace(chr(10), chr(10) + '   ')}")

    # ---------- 5. 二进制文件读写 ----------
    # 对于图片、音频、视频等非文本文件，需要使用二进制模式（'b'）
    # 二进制模式下，读写的数据是 bytes 类型，不是 str
    print("\n5. 二进制文件读写：")

    # 写入二进制数据
    binary_data = b"\x48\x65\x6c\x6c\x6f\x20\x42\x69\x6e\x61\x72\x79"
    # b 前缀表示 bytes 类型，\x 表示十六进制
    # 上面的数据实际上是 "Hello Binary" 的 ASCII 编码
    with open(binary_file, "wb") as f:  # wb = write binary
        f.write(binary_data)  # 写入字节数据
    print(f"   已写入二进制文件：{binary_file}")

    # 读取二进制数据
    with open(binary_file, "rb") as f:  # rb = read binary
        data = f.read()  # 读取字节数据
    print(f"   读取的二进制数据：{data}")
    print(f"   类型：{type(data)}")
    print(f"   解码为字符串：{data.decode('utf-8')}")  # 将字节解码为字符串

    # ---------- 6. 文件指针操作 ----------
    # 文件对象维护一个文件指针，表示当前读写位置
    print("\n6. 文件指针操作：")

    with open(test_file, "r", encoding="utf-8") as f:
        print(f"   初始位置：f.tell() = {f.tell()}")  # tell() 返回当前位置

        content = f.read(10)  # 读取 10 个字符
        print(f"   读取10个字符：{repr(content)}")
        print(f"   读取后位置：{f.tell()}")

        f.seek(0)  # seek() 移动文件指针到指定位置（0 表示开头）
        print(f"   seek(0) 后位置：{f.tell()}")

        content = f.read(5)  # 从头读取 5 个字符
        print(f"   从头读取5个字符：{repr(content)}")

    # ---------- 7. 清理临时文件 ----------
    # 演示完毕，删除临时文件和目录（可选）
    # 这里保留文件，方便学习者查看
    print(f"\n   提示：测试文件保存在 {test_dir}，可自行查看")


# ==============================================================================
# 第四部分：异常处理
# ==============================================================================
# 异常（Exception）是程序运行时出现的错误
# Python 使用 try/except 机制来捕获和处理异常
# 妥善处理异常可以让程序更加健壮，不会因为一处错误而崩溃
#
# 常见异常类型：
#   Exception    - 所有异常的基类
#   ValueError   - 值错误（如 int('abc')）
#   TypeError    - 类型错误（如 1 + 'a'）
#   IndexError   - 索引越界
#   KeyError     - 字典键不存在
#   FileNotFoundError - 文件不存在
#   ZeroDivisionError - 除以零
#   IOError      - IO 操作失败
# ==============================================================================

def demo_exception_handling():
    """
    函数名：demo_exception_handling
    功能：演示 Python 异常处理机制
    参数：无
    返回值：无
    设计思路：从最基本的 try/except 开始，逐步介绍 else、finally、
             多个 except、自定义异常、raise 等高级用法。
    """
    print("\n" + "=" * 60)
    print("【四、异常处理演示】")
    print("=" * 60)

    # ---------- 1. 基本的 try/except ----------
    print("\n1. 基本的 try/except：")

    # 没有异常处理时，程序会直接崩溃
    # 例如除以零会引发 ZeroDivisionError
    print("   尝试除以零（有异常处理）：")
    try:
        # try 块中放置可能引发异常的代码
        result = 10 / 0  # 这行代码会引发 ZeroDivisionError 异常
        print(f"   结果：{result}")  # 上面的行出错，这行不会执行
    except ZeroDivisionError:
        # except 块中处理异常
        # 当 try 块中发生 ZeroDivisionError 时，执行这里的代码
        print("   错误：不能除以零！")

    print("   程序继续运行...")  # 异常被捕获处理后，程序继续执行

    # ---------- 2. 获取异常信息 ----------
    print("\n2. 获取异常信息：")

    try:
        num = int("abc")  # 字符串 "abc" 不能转为整数，引发 ValueError
    except ValueError as e:
        # as e 将异常对象赋值给变量 e，可以从中获取异常信息
        print(f"   发生值错误：{e}")
        print(f"   异常类型：{type(e).__name__}")

    # ---------- 3. 多个 except 子句 ----------
    # 可以有多个 except 子句，分别处理不同类型的异常
    print("\n3. 多个 except 子句：")

    test_cases = [
        ("10 / 2", lambda: 10 / 2),            # 正常
        ("10 / 0", lambda: 10 / 0),            # ZeroDivisionError
        ("int('abc')", lambda: int("abc")),     # ValueError
        ("[1,2,3][5]", lambda: [1, 2, 3][5]),  # IndexError
    ]

    for desc, func in test_cases:
        print(f"   测试 {desc}：", end=" ")
        try:
            result = func()  # 执行测试函数
            print(f"成功，结果 = {result}")
        except ZeroDivisionError:
            print("捕获到 ZeroDivisionError（除以零）")
        except ValueError:
            print("捕获到 ValueError（值错误）")
        except IndexError:
            print("捕获到 IndexError（索引越界）")
        except Exception as e:
            # Exception 是大多数异常的基类，可以捕获所有未被前面捕获的异常
            # 通常放在最后，作为"兜底"
            print(f"捕获到其他异常：{type(e).__name__}: {e}")

    # ---------- 4. else 子句 ----------
    # else 块中的代码只有在 try 块没有发生异常时才执行
    print("\n4. else 子句（无异常时执行）：")

    test_values = ["123", "abc"]
    for val in test_values:
        print(f"   转换 '{val}'：", end="")
        try:
            num = int(val)
        except ValueError:
            print("转换失败")
        else:
            # 只有 try 块正常执行（没有异常），才会执行 else 块
            print(f"转换成功，结果 = {num}")

    # ---------- 5. finally 子句 ----------
    # finally 块中的代码无论是否发生异常都会执行
    # 常用于资源清理操作，如关闭文件、释放锁等
    print("\n5. finally 子句（无论是否异常都执行）：")

    for val in test_values:
        print(f"   转换 '{val}'：")
        try:
            num = int(val)
            print(f"     转换成功：{num}")
        except ValueError:
            print(f"     转换失败")
        finally:
            # 无论成功或失败，finally 块都会执行
            print(f"     （finally 块执行了）")

    # ---------- 6. 手动抛出异常（raise）----------
    # 使用 raise 语句可以主动抛出异常
    print("\n6. 手动抛出异常（raise）：")

    def set_age(age):
        """
        函数名：set_age
        功能：设置年龄，年龄必须在合理范围内
        参数：
            age - 年龄值
        返回值：无
        异常：如果年龄不在 0~150 范围内，抛出 ValueError
        """
        if age < 0 or age > 150:
            # 手动抛出 ValueError 异常，并附带错误信息
            raise ValueError(f"年龄不合理：{age}，年龄应在 0~150 之间")
        print(f"   年龄设置成功：{age}")

    try:
        set_age(25)   # 正常
        set_age(-5)   # 会抛出异常
    except ValueError as e:
        print(f"   捕获异常：{e}")

    # ---------- 7. 自定义异常 ----------
    # 通过继承 Exception 类可以创建自定义异常类型
    # 自定义异常可以更精确地表达业务逻辑中的错误
    print("\n7. 自定义异常：")

    class InsufficientFundsError(Exception):
        """
        类名：InsufficientFundsError
        功能：自定义异常类，表示"余额不足"错误
        继承：Exception（所有异常的基类）
        设计思路：当银行账户余额不足时抛出此异常，
                 调用者可以精确捕获这种特定类型的错误。
        """
        def __init__(self, balance, amount):
            """
            构造函数：初始化异常对象
            参数：
                balance - 当前余额
                amount - 尝试取款的金额
            """
            self.balance = balance  # 保存当前余额
            self.amount = amount    # 保存取款金额
            # 调用父类的构造函数，传入错误信息
            super().__init__(f"余额不足！当前余额：{balance}，尝试取款：{amount}")

    def withdraw(balance, amount):
        """
        函数名：withdraw
        功能：模拟取款操作
        参数：
            balance - 当前余额
            amount - 取款金额
        返回值：取款后的余额
        异常：余额不足时抛出 InsufficientFundsError
        """
        if amount > balance:
            # 余额不足，抛出自定义异常
            raise InsufficientFundsError(balance, amount)
        return balance - amount

    try:
        new_balance = withdraw(1000, 500)  # 正常取款
        print(f"   取款成功，剩余余额：{new_balance}")
        new_balance = withdraw(new_balance, 800)  # 余额不足
        print(f"   取款成功，剩余余额：{new_balance}")
    except InsufficientFundsError as e:
        # 捕获自定义异常
        print(f"   捕获自定义异常：{e}")
        print(f"   （当前余额：{e.balance}，需要：{e.amount}）")


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
    print("\n" + "=" * 60)
    print("  Python 字符串与文件 IO 演示")
    print("=" * 60)

    demo_string_operations()    # 字符串操作
    demo_string_formatting()    # 字符串格式化
    demo_file_io()              # 文件 IO
    demo_exception_handling()   # 异常处理

    print("\n" + "=" * 60)
    print("  字符串与文件 IO 演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
