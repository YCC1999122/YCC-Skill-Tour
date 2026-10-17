# -*- coding: utf-8 -*-
"""
================================================================================
文件名：structures.py
所属模块：data_structures（数据结构）
功能说明：本文件使用 Python 实现常见的数据结构和排序算法，
         包括链表、栈、队列、二叉树，以及冒泡、选择、插入、快速排序。

运行方式：在项目根目录下执行 `python -m data_structures.structures`
================================================================================
"""

import random  # 导入 random 模块，用于生成随机测试数据


# ==============================================================================
# 第一部分：链表（Linked List）
# ==============================================================================
# 链表是一种线性数据结构，由一系列节点（Node）组成
# 每个节点包含数据和指向下一个节点的指针
# 链表的优点：插入删除效率高（O(1)，只需要修改指针）
# 链表的缺点：随机访问效率低（O(n)，需要从头遍历）
#
# 常见链表类型：
#   - 单链表（Singly Linked List）：每个节点只有 next 指针
#   - 双链表（Doubly Linked List）：每个节点有 prev 和 next 两个指针
#   - 循环链表（Circular Linked List）：尾节点指向头节点
# ==============================================================================

class ListNode:
    """
    类名：ListNode
    功能：链表节点类
    属性：
        data - 节点存储的数据
        next - 指向下一个节点的引用（指针）
    说明：链表的基本单元，每个节点保存数据和下一个节点的地址。
    """

    def __init__(self, data):
        """
        构造函数：创建一个链表节点
        参数：
            data - 节点要存储的数据
        """
        self.data = data  # 存储数据
        self.next = None  # 指向下一个节点，初始为 None（没有后继）


class SinglyLinkedList:
    """
    类名：SinglyLinkedList
    功能：单链表类
    属性：
        head - 头节点引用
        size - 链表大小（节点数量）
    设计思路：使用头节点（head）来表示链表的起点，
             通过遍历 next 指针来访问各个节点。
    """

    def __init__(self):
        """
        构造函数：创建一个空链表
        """
        self.head = None  # 头节点，初始为 None（空链表）
        self.size = 0     # 链表大小，初始为 0

    def is_empty(self):
        """
        方法名：is_empty
        功能：判断链表是否为空
        返回值：布尔值
        """
        return self.head is None  # 头节点为 None 表示空链表

    def get_size(self):
        """
        方法名：get_size
        功能：获取链表大小（节点数量）
        返回值：整数
        """
        return self.size

    def append(self, data):
        """
        方法名：append
        功能：在链表末尾添加一个节点
        参数：
            data - 要添加的数据
        时间复杂度：O(n)，需要遍历到链表末尾
        """
        new_node = ListNode(data)  # 创建新节点

        if self.is_empty():
            # 链表为空，新节点作为头节点
            self.head = new_node
        else:
            # 链表不为空，找到最后一个节点
            current = self.head  # 从头节点开始
            while current.next is not None:  # 遍历到最后一个节点
                current = current.next
            current.next = new_node  # 将最后一个节点的 next 指向新节点

        self.size += 1  # 大小加 1

    def prepend(self, data):
        """
        方法名：prepend
        功能：在链表开头添加一个节点
        参数：
            data - 要添加的数据
        时间复杂度：O(1)，直接在头部插入
        """
        new_node = ListNode(data)  # 创建新节点
        new_node.next = self.head  # 新节点的 next 指向原来的头节点
        self.head = new_node       # 头节点更新为新节点
        self.size += 1             # 大小加 1

    def insert(self, index, data):
        """
        方法名：insert
        功能：在指定位置插入节点
        参数：
            index - 插入位置（从 0 开始）
            data - 要插入的数据
        时间复杂度：O(n)，需要遍历到指定位置
        """
        # 检查索引是否有效
        if index < 0 or index > self.size:
            raise IndexError("索引超出范围")

        if index == 0:
            # 插入到头部
            self.prepend(data)
            return

        if index == self.size:
            # 插入到尾部
            self.append(data)
            return

        # 插入到中间位置
        new_node = ListNode(data)  # 创建新节点
        current = self.head        # 从头开始
        # 移动到插入位置的前一个节点
        for _ in range(index - 1):
            current = current.next
        # 新节点的 next 指向当前节点的 next
        new_node.next = current.next
        # 当前节点的 next 指向新节点
        current.next = new_node
        self.size += 1  # 大小加 1

    def delete(self, index):
        """
        方法名：delete
        功能：删除指定位置的节点
        参数：
            index - 要删除的位置
        返回值：被删除节点的数据
        时间复杂度：O(n)
        """
        if self.is_empty():
            raise IndexError("链表为空")
        if index < 0 or index >= self.size:
            raise IndexError("索引超出范围")

        if index == 0:
            # 删除头节点
            data = self.head.data      # 保存头节点数据
            self.head = self.head.next  # 头节点指向下一个节点
            self.size -= 1              # 大小减 1
            return data

        # 删除中间或尾部节点
        current = self.head  # 从头开始
        # 移动到删除位置的前一个节点
        for _ in range(index - 1):
            current = current.next
        # 保存被删除节点的数据
        data = current.next.data
        # 将前一个节点的 next 指向被删除节点的 next（跳过被删除节点）
        current.next = current.next.next
        self.size -= 1  # 大小减 1
        return data

    def get(self, index):
        """
        方法名：get
        功能：获取指定位置的节点数据
        参数：
            index - 位置索引
        返回值：节点数据
        时间复杂度：O(n)
        """
        if index < 0 or index >= self.size:
            raise IndexError("索引超出范围")

        current = self.head  # 从头节点开始
        for _ in range(index):  # 移动到指定位置
            current = current.next
        return current.data

    def search(self, data):
        """
        方法名：search
        功能：查找指定数据是否存在
        参数：
            data - 要查找的数据
        返回值：找到返回索引，没找到返回 -1
        时间复杂度：O(n)
        """
        current = self.head  # 从头节点开始
        index = 0
        while current is not None:
            if current.data == data:
                return index  # 找到，返回索引
            current = current.next
            index += 1
        return -1  # 没找到，返回 -1

    def to_list(self):
        """
        方法名：to_list
        功能：将链表转换为 Python 列表（方便查看）
        返回值：列表
        """
        result = []
        current = self.head  # 从头节点开始
        while current is not None:
            result.append(current.data)  # 将数据加入结果列表
            current = current.next       # 移动到下一个节点
        return result

    def __str__(self):
        """
        方法名：__str__
        功能：返回链表的字符串表示
        """
        return f"LinkedList({self.to_list()})"


# ==============================================================================
# 第二部分：栈（Stack）
# ==============================================================================
# 栈是一种"后进先出"（LIFO, Last In First Out）的线性数据结构
# 只能在栈顶进行插入（push）和删除（pop）操作
#
# 应用场景：
#   - 函数调用栈（程序运行时的函数调用关系）
#   - 表达式求值（逆波兰表达式）
#   - 括号匹配
#   - 深度优先搜索（DFS）
#
# 这里用列表实现栈（Python 的 list 本身就可以当栈用）
# 也可以用链表实现
# ==============================================================================

class Stack:
    """
    类名：Stack
    功能：栈数据结构（基于列表实现）
    属性：
        items - 存储栈元素的列表
    设计思路：使用 Python 列表作为底层存储，
             列表末尾作为栈顶（append/pop 都是 O(1) 操作）。
    """

    def __init__(self):
        """
        构造函数：创建一个空栈
        """
        self.items = []  # 用列表存储栈元素，列表末尾是栈顶

    def is_empty(self):
        """
        方法名：is_empty
        功能：判断栈是否为空
        返回值：布尔值
        """
        return len(self.items) == 0

    def push(self, item):
        """
        方法名：push
        功能：将元素压入栈顶
        参数：
            item - 要压入的元素
        时间复杂度：O(1)
        """
        self.items.append(item)  # 在列表末尾添加元素（栈顶）

    def pop(self):
        """
        方法名：pop
        功能：弹出栈顶元素并返回
        返回值：栈顶元素
        时间复杂度：O(1)
        异常：栈为空时抛出 IndexError
        """
        if self.is_empty():
            raise IndexError("栈为空，不能弹出元素")
        return self.items.pop()  # 弹出列表最后一个元素（栈顶）

    def peek(self):
        """
        方法名：peek
        功能：查看栈顶元素但不弹出
        返回值：栈顶元素
        时间复杂度：O(1)
        """
        if self.is_empty():
            raise IndexError("栈为空，不能查看栈顶")
        return self.items[-1]  # 返回列表最后一个元素（栈顶）

    def size(self):
        """
        方法名：size
        功能：获取栈的大小
        返回值：整数
        """
        return len(self.items)

    def __str__(self):
        """返回字符串表示"""
        return f"Stack({self.items})"


# ---------- 栈的应用：括号匹配 ----------
def is_valid_parentheses(s):
    """
    函数名：is_valid_parentheses
    功能：判断括号字符串是否有效匹配
    参数：
        s - 只包含括号字符的字符串
    返回值：布尔值，是否有效
    算法思路：
        1. 遇到左括号，压入栈
        2. 遇到右括号，弹出栈顶，检查是否匹配
        3. 最后栈为空则有效
    """
    # 括号匹配关系字典
    matching = {')': '(', ']': '[', '}': '{'}
    stack = Stack()  # 创建栈

    for char in s:
        if char in '([{':
            # 左括号，压入栈
            stack.push(char)
        elif char in ')]}':
            # 右括号，检查匹配
            if stack.is_empty():
                return False  # 没有对应的左括号
            top = stack.pop()
            if top != matching[char]:
                return False  # 括号类型不匹配

    # 最后栈必须为空（所有左括号都有对应的右括号）
    return stack.is_empty()


# ==============================================================================
# 第三部分：队列（Queue）
# ==============================================================================
# 队列是一种"先进先出"（FIFO, First In First Out）的线性数据结构
# 只能在队尾插入（enqueue），在队头删除（dequeue）
#
# 应用场景：
#   - 任务调度
#   - 广度优先搜索（BFS）
#   - 消息队列
#
# 这里用两个栈实现队列（经典面试题）
# 也可以用 collections.deque 实现（更高效）
# ==============================================================================

class Queue:
    """
    类名：Queue
    功能：队列数据结构（用两个栈实现）
    属性：
        in_stack - 入队栈（负责添加元素）
        out_stack - 出队栈（负责删除元素）
    设计思路：使用两个栈模拟队列的先进先出特性。
             入队时往 in_stack 压，出队时从 out_stack 弹。
             如果 out_stack 空了，就把 in_stack 的元素全部倒入 out_stack。
    """

    def __init__(self):
        """
        构造函数：创建空队列
        """
        self.in_stack = Stack()   # 入队栈
        self.out_stack = Stack()  # 出队栈

    def is_empty(self):
        """
        方法名：is_empty
        功能：判断队列是否为空
        返回值：布尔值
        """
        # 两个栈都为空，队列才为空
        return self.in_stack.is_empty() and self.out_stack.is_empty()

    def enqueue(self, item):
        """
        方法名：enqueue
        功能：入队（在队尾添加元素）
        参数：
            item - 要添加的元素
        时间复杂度：O(1)
        """
        self.in_stack.push(item)  # 直接往入队栈里压

    def dequeue(self):
        """
        方法名：dequeue
        功能：出队（删除并返回队头元素）
        返回值：队头元素
        时间复杂度：均摊 O(1)（每个元素最多被搬运一次）
        """
        if self.is_empty():
            raise IndexError("队列为空，不能出队")

        # 如果出队栈为空，把入队栈的所有元素倒入出队栈
        if self.out_stack.is_empty():
            while not self.in_stack.is_empty():
                self.out_stack.push(self.in_stack.pop())

        # 从出队栈弹出（就是队头元素）
        return self.out_stack.pop()

    def peek(self):
        """
        方法名：peek
        功能：查看队头元素但不删除
        返回值：队头元素
        """
        if self.is_empty():
            raise IndexError("队列为空")

        if self.out_stack.is_empty():
            while not self.in_stack.is_empty():
                self.out_stack.push(self.in_stack.pop())

        return self.out_stack.peek()

    def size(self):
        """
        方法名：size
        功能：获取队列大小
        返回值：整数
        """
        return self.in_stack.size() + self.out_stack.size()

    def __str__(self):
        """返回字符串表示"""
        # 为了方便显示，把两个栈的内容合起来
        in_items = self.in_stack.items[:]  # 复制入队栈
        out_items = self.out_stack.items[:]  # 复制出队栈
        # 出队栈是倒序的，反转后放在前面
        all_items = list(reversed(out_items)) + in_items
        return f"Queue({all_items})"


# ==============================================================================
# 第四部分：二叉树（Binary Tree）
# ==============================================================================
# 二叉树是每个节点最多有两个子节点的树结构
# 两个子节点分别称为左子节点和右子节点
#
# 二叉树的遍历方式：
#   - 前序遍历（Pre-order）：根 -> 左 -> 右
#   - 中序遍历（In-order）：左 -> 根 -> 右
#   - 后序遍历（Post-order）：左 -> 右 -> 根
#   - 层序遍历（Level-order）：按层次从上到下、从左到右
#
# 二叉搜索树（BST, Binary Search Tree）：
#   左子树所有节点的值 < 根节点的值 < 右子树所有节点的值
# ==============================================================================

class TreeNode:
    """
    类名：TreeNode
    功能：二叉树节点类
    属性：
        value - 节点值
        left - 左子节点
        right - 右子节点
    """

    def __init__(self, value):
        """
        构造函数：创建二叉树节点
        参数：
            value - 节点存储的值
        """
        self.value = value  # 节点值
        self.left = None    # 左子节点
        self.right = None   # 右子节点


class BinarySearchTree:
    """
    类名：BinarySearchTree
    功能：二叉搜索树（BST）
    属性：
        root - 根节点
    设计思路：实现一个二叉搜索树，支持插入、查找、遍历等操作。
             BST 的性质：左子树所有节点 < 根 < 右子树所有节点。
    """

    def __init__(self):
        """
        构造函数：创建空二叉搜索树
        """
        self.root = None  # 根节点，初始为 None

    def is_empty(self):
        """判断是否为空树"""
        return self.root is None

    def insert(self, value):
        """
        方法名：insert
        功能：插入一个值到 BST 中
        参数：
            value - 要插入的值
        时间复杂度：平均 O(log n)，最坏 O(n)（退化成链表）
        """
        if self.is_empty():
            # 树为空，新节点作为根
            self.root = TreeNode(value)
        else:
            # 树不为空，递归插入
            self._insert_recursive(self.root, value)

    def _insert_recursive(self, node, value):
        """
        方法名：_insert_recursive
        功能：递归插入辅助方法（私有）
        参数：
            node - 当前节点
            value - 要插入的值
        说明：以下划线开头表示这是内部方法，外部不应该直接调用。
        """
        if value < node.value:
            # 值小于当前节点，插入到左子树
            if node.left is None:
                node.left = TreeNode(value)  # 左子节点为空，直接插入
            else:
                self._insert_recursive(node.left, value)  # 递归处理左子树
        elif value > node.value:
            # 值大于当前节点，插入到右子树
            if node.right is None:
                node.right = TreeNode(value)  # 右子节点为空，直接插入
            else:
                self._insert_recursive(node.right, value)  # 递归处理右子树
        # value == node.value 的情况：值已存在，不重复插入

    def search(self, value):
        """
        方法名：search
        功能：查找值是否存在于 BST 中
        参数：
            value - 要查找的值
        返回值：布尔值
        时间复杂度：平均 O(log n)，最坏 O(n)
        """
        return self._search_recursive(self.root, value)

    def _search_recursive(self, node, value):
        """递归查找辅助方法"""
        if node is None:
            return False  # 到达叶子节点，没找到
        if value == node.value:
            return True  # 找到
        elif value < node.value:
            return self._search_recursive(node.left, value)  # 在左子树找
        else:
            return self._search_recursive(node.right, value)  # 在右子树找

    # ---------- 遍历方法 ----------

    def preorder_traversal(self):
        """
        方法名：preorder_traversal
        功能：前序遍历（根 -> 左 -> 右）
        返回值：遍历结果列表
        """
        result = []
        self._preorder_recursive(self.root, result)
        return result

    def _preorder_recursive(self, node, result):
        """前序遍历递归辅助"""
        if node is not None:
            result.append(node.value)                     # 1. 访问根节点
            self._preorder_recursive(node.left, result)   # 2. 遍历左子树
            self._preorder_recursive(node.right, result)  # 3. 遍历右子树

    def inorder_traversal(self):
        """
        方法名：inorder_traversal
        功能：中序遍历（左 -> 根 -> 右）
        返回值：遍历结果列表
        说明：BST 的中序遍历结果是有序的（升序），这是 BST 的重要性质。
        """
        result = []
        self._inorder_recursive(self.root, result)
        return result

    def _inorder_recursive(self, node, result):
        """中序遍历递归辅助"""
        if node is not None:
            self._inorder_recursive(node.left, result)    # 1. 遍历左子树
            result.append(node.value)                     # 2. 访问根节点
            self._inorder_recursive(node.right, result)   # 3. 遍历右子树

    def postorder_traversal(self):
        """
        方法名：postorder_traversal
        功能：后序遍历（左 -> 右 -> 根）
        返回值：遍历结果列表
        """
        result = []
        self._postorder_recursive(self.root, result)
        return result

    def _postorder_recursive(self, node, result):
        """后序遍历递归辅助"""
        if node is not None:
            self._postorder_recursive(node.left, result)   # 1. 遍历左子树
            self._postorder_recursive(node.right, result)  # 2. 遍历右子树
            result.append(node.value)                      # 3. 访问根节点

    def level_order_traversal(self):
        """
        方法名：level_order_traversal
        功能：层序遍历（按层次从上到下、从左到右）
        返回值：遍历结果列表
        算法思路：使用队列实现广度优先搜索（BFS）。
        """
        if self.is_empty():
            return []

        result = []
        queue = []  # 用列表模拟队列
        queue.append(self.root)  # 根节点入队

        while queue:
            node = queue.pop(0)  # 出队（注意：list.pop(0) 是 O(n)，实际用 deque）
            result.append(node.value)  # 访问节点

            # 左子节点入队
            if node.left:
                queue.append(node.left)
            # 右子节点入队
            if node.right:
                queue.append(node.right)

        return result

    def get_height(self):
        """
        方法名：get_height
        功能：获取树的高度（最大深度）
        返回值：整数
        """
        return self._height_recursive(self.root)

    def _height_recursive(self, node):
        """高度递归辅助"""
        if node is None:
            return 0  # 空节点高度为 0
        # 左子树高度和右子树高度的较大值 + 1（当前节点）
        left_height = self._height_recursive(node.left)
        right_height = self._height_recursive(node.right)
        return max(left_height, right_height) + 1


# ==============================================================================
# 第五部分：排序算法
# ==============================================================================
# 排序是将一组数据按照特定顺序（升序/降序）重新排列的过程
# 常见排序算法：
#   - 冒泡排序（Bubble Sort）：O(n^2)，稳定
#   - 选择排序（Selection Sort）：O(n^2)，不稳定
#   - 插入排序（Insertion Sort）：O(n^2)，稳定
#   - 快速排序（Quick Sort）：平均 O(n log n)，最坏 O(n^2)，不稳定
#   - 归并排序（Merge Sort）：O(n log n)，稳定
#   - 堆排序（Heap Sort）：O(n log n)，不稳定
#
# 这里实现四种经典排序：冒泡、选择、插入、快排
# ==============================================================================

def bubble_sort(arr):
    """
    函数名：bubble_sort
    功能：冒泡排序（升序）
    参数：
        arr - 待排序的列表
    返回值：排序后的列表（不修改原列表）
    时间复杂度：O(n^2)
    空间复杂度：O(1)
    稳定性：稳定（相等元素的相对位置不变）
    算法思路：
        重复遍历列表，比较相邻元素，如果顺序不对就交换。
        每一轮会将最大的元素"冒泡"到列表末尾。
        如果某一轮没有发生任何交换，说明已经有序，可以提前结束。
    """
    # 创建副本，不修改原列表
    result = arr[:]
    n = len(result)  # 列表长度

    # 外层循环：控制排序轮数，最多 n-1 轮
    for i in range(n - 1):
        swapped = False  # 标记本轮是否发生了交换

        # 内层循环：每轮比较相邻元素，把最大的冒到后面
        # 每轮结束后，后面的 i+1 个元素已经排好序，不需要再比较
        for j in range(n - 1 - i):
            if result[j] > result[j + 1]:
                # 前一个比后一个大，交换位置
                result[j], result[j + 1] = result[j + 1], result[j]
                swapped = True  # 标记发生了交换

        # 如果本轮没有发生交换，说明已经有序，提前结束
        if not swapped:
            break

    return result


def selection_sort(arr):
    """
    函数名：selection_sort
    功能：选择排序（升序）
    参数：
        arr - 待排序的列表
    返回值：排序后的列表
    时间复杂度：O(n^2)
    空间复杂度：O(1)
    稳定性：不稳定
    算法思路：
        将列表分为已排序区和未排序区。
        每一轮从未排序区中找到最小的元素，
        与未排序区的第一个元素交换位置。
    """
    result = arr[:]
    n = len(result)

    # 外层循环：控制已排序区的边界
    for i in range(n - 1):
        min_index = i  # 假设未排序区的第一个元素是最小的

        # 内层循环：在未排序区中找最小元素的索引
        for j in range(i + 1, n):
            if result[j] < result[min_index]:
                min_index = j  # 更新最小值索引

        # 将最小元素与未排序区的第一个元素交换
        if min_index != i:
            result[i], result[min_index] = result[min_index], result[i]

    return result


def insertion_sort(arr):
    """
    函数名：insertion_sort
    功能：插入排序（升序）
    参数：
        arr - 待排序的列表
    返回值：排序后的列表
    时间复杂度：O(n^2)，但对近乎有序的数据效率很高
    空间复杂度：O(1)
    稳定性：稳定
    算法思路：
        将列表分为已排序区和未排序区。
        每次从未排序区取出第一个元素，
        插入到已排序区的合适位置（保持有序）。
        类似于打扑克牌时理牌的过程。
    """
    result = arr[:]
    n = len(result)

    # 外层循环：从第二个元素开始，逐个插入到已排序区
    # 第一个元素默认已排序
    for i in range(1, n):
        key = result[i]  # 当前要插入的元素
        j = i - 1        # 已排序区的最后一个位置

        # 将大于 key 的元素向后移动，为 key 腾出位置
        while j >= 0 and result[j] > key:
            result[j + 1] = result[j]  # 元素后移
            j -= 1  # 继续向前比较

        # 插入 key 到正确位置
        result[j + 1] = key

    return result


def quick_sort(arr):
    """
    函数名：quick_sort
    功能：快速排序（升序）
    参数：
        arr - 待排序的列表
    返回值：排序后的列表
    时间复杂度：平均 O(n log n)，最坏 O(n^2)（当数组已有序时）
    空间复杂度：O(log n)（递归调用栈）
    稳定性：不稳定
    算法思路（分治法）：
        1. 选择一个基准元素（pivot）
        2. 分区：小于基准的放左边，大于基准的放右边
        3. 递归：对左右两部分分别快速排序
    实现方式：这里使用列表推导式的简洁实现方式，
             优点是代码简洁易读，缺点是空间复杂度略高。
    """
    # 递归终止条件：空列表或只有一个元素的列表已经有序
    if len(arr) <= 1:
        return arr

    # 选择基准元素（这里选中间的元素，也可以选第一个、最后一个或随机）
    pivot_index = len(arr) // 2  # 取中间位置
    pivot = arr[pivot_index]     # 基准值

    # 分区：小于基准、等于基准、大于基准
    left = [x for x in arr if x < pivot]    # 小于基准的元素
    middle = [x for x in arr if x == pivot] # 等于基准的元素
    right = [x for x in arr if x > pivot]   # 大于基准的元素

    # 递归排序左右两部分，然后合并
    return quick_sort(left) + middle + quick_sort(right)


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
    print("  Python 数据结构与算法演示")
    print("=" * 60)

    # ---------- 1. 链表 ----------
    print("\n" + "=" * 60)
    print("【一、单链表】")
    print("=" * 60)

    linked_list = SinglyLinkedList()
    print(f"初始链表：{linked_list}")

    # 尾部添加
    linked_list.append(10)
    linked_list.append(20)
    linked_list.append(30)
    print(f"append 10, 20, 30 后：{linked_list}")

    # 头部添加
    linked_list.prepend(5)
    print(f"prepend 5 后：{linked_list}")

    # 中间插入
    linked_list.insert(2, 15)
    print(f"insert(2, 15) 后：{linked_list}")

    # 获取元素
    print(f"get(3) = {linked_list.get(3)}")

    # 查找
    print(f"search(20) = {linked_list.search(20)}（索引）")
    print(f"search(99) = {linked_list.search(99)}（-1 表示没找到）")

    # 删除
    deleted = linked_list.delete(1)
    print(f"delete(1) 删除了 {deleted}，链表变为：{linked_list}")

    # 大小
    print(f"链表大小：{linked_list.get_size()}")

    # ---------- 2. 栈 ----------
    print("\n" + "=" * 60)
    print("【二、栈】")
    print("=" * 60)

    stack = Stack()
    print(f"初始栈：{stack}")

    # 压入元素
    stack.push(10)
    stack.push(20)
    stack.push(30)
    print(f"push 10, 20, 30 后：{stack}")

    # 查看栈顶
    print(f"peek() = {stack.peek()}（栈顶）")

    # 弹出元素
    popped = stack.pop()
    print(f"pop() = {popped}，弹出后：{stack}")

    popped = stack.pop()
    print(f"再 pop() = {popped}，弹出后：{stack}")

    # 大小
    print(f"栈大小：{stack.size()}")

    # 括号匹配应用
    print("\n栈的应用：括号匹配")
    test_cases = ["()", "()[]{}", "(]", "([)]", "{[]}", "((()))"]
    for test in test_cases:
        print(f"   '{test}' -> {'有效' if is_valid_parentheses(test) else '无效'}")

    # ---------- 3. 队列 ----------
    print("\n" + "=" * 60)
    print("【三、队列（双栈实现）】")
    print("=" * 60)

    queue = Queue()
    print(f"初始队列：{queue}")

    # 入队
    queue.enqueue(10)
    queue.enqueue(20)
    queue.enqueue(30)
    print(f"enqueue 10, 20, 30 后：{queue}")

    # 查看队头
    print(f"peek() = {queue.peek()}（队头）")

    # 出队
    dequeued = queue.dequeue()
    print(f"dequeue() = {dequeued}，出队后：{queue}")

    # 继续入队
    queue.enqueue(40)
    print(f"enqueue 40 后：{queue}")

    # 继续出队
    dequeued = queue.dequeue()
    print(f"dequeue() = {dequeued}，出队后：{queue}")

    dequeued = queue.dequeue()
    print(f"dequeue() = {dequeued}，出队后：{queue}")

    print(f"队列大小：{queue.size()}")

    # ---------- 4. 二叉搜索树 ----------
    print("\n" + "=" * 60)
    print("【四、二叉搜索树（BST）】")
    print("=" * 60)

    bst = BinarySearchTree()

    # 插入数据
    values = [50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45]
    for val in values:
        bst.insert(val)
    print(f"插入值：{values}")

    # 遍历
    print(f"前序遍历：{bst.preorder_traversal()}")
    print(f"中序遍历：{bst.inorder_traversal()}（BST 中序遍历是有序的）")
    print(f"后序遍历：{bst.postorder_traversal()}")
    print(f"层序遍历：{bst.level_order_traversal()}")

    # 查找
    print(f"search(35) = {bst.search(35)}")
    print(f"search(99) = {bst.search(99)}")

    # 高度
    print(f"树的高度：{bst.get_height()}")

    # ---------- 5. 排序算法 ----------
    print("\n" + "=" * 60)
    print("【五、排序算法】")
    print("=" * 60)

    # 生成随机测试数据
    random.seed(42)  # 设置随机种子，保证结果可重现
    test_data = random.sample(range(1, 100), 10)  # 从 1~99 中随机取 10 个
    print(f"原始数据：{test_data}")

    print(f"\n冒泡排序：{bubble_sort(test_data)}")
    print(f"选择排序：{selection_sort(test_data)}")
    print(f"插入排序：{insertion_sort(test_data)}")
    print(f"快速排序：{quick_sort(test_data)}")

    # 验证结果一致性
    expected = sorted(test_data)
    print(f"\n内置 sorted()：{expected}")
    print(f"四种排序结果都正确吗？"
          f"{bubble_sort(test_data) == expected and selection_sort(test_data) == expected and insertion_sort(test_data) == expected and quick_sort(test_data) == expected}")

    print("\n" + "=" * 60)
    print("  数据结构与算法演示结束！")
    print("=" * 60)


# ==============================================================================
# 模块入口判断
# ==============================================================================

if __name__ == "__main__":
    run_demo()
