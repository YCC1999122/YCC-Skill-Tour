/**
 * ============================================================
 * 文件名：structures.js
 * 描述：JavaScript 数据结构与算法 - 链表、栈、队列、二叉搜索树、排序算法
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件使用 JavaScript 实现常见的数据结构和算法，包括：
 * 1. 链表（LinkedList）
 * 2. 栈（Stack）
 * 3. 队列（Queue）
 * 4. 二叉搜索树（BinarySearchTree）
 * 5. 常见排序算法（冒泡、选择、插入、快速、归并）
 *
 * 运行方式：node data/structures.js
 */

'use strict';

module.exports = function demoDataStructures() {
  console.log('='.repeat(60));
  console.log('一、链表（LinkedList）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 单向链表
  // ------------------------------------------------------------
  // 链表是一种线性数据结构，由节点组成，每个节点包含数据和指向下一个节点的指针
  // 与数组相比：
  // - 优点：插入删除效率高（O(1)），不需要连续内存
  // - 缺点：随机访问效率低（O(n)），需要额外空间存储指针
  console.log('\n--- 1.1 单向链表 ---');

  /**
   * 链表节点类
   * 每个节点包含数据和指向下一个节点的指针
   */
  class ListNode {
    /**
     * 构造函数
     * @param {*} value - 节点存储的数据
     */
    constructor(value) {
      this.value = value;    // 节点存储的数据
      this.next = null;      // 指向下一个节点的指针，初始为 null
    }
  }

  /**
   * 单向链表类
   */
  class LinkedList {
    constructor() {
      this.head = null;      // 头节点
      this.tail = null;      // 尾节点
      this.length = 0;       // 链表长度
    }

    /**
     * 在链表末尾添加节点
     * @param {*} value - 要添加的值
     * @returns {LinkedList} 返回链表本身，支持链式调用
     */
    append(value) {
      // 创建新节点
      const newNode = new ListNode(value);

      // 如果链表为空，头和尾都指向新节点
      if (this.length === 0) {
        this.head = newNode;
        this.tail = newNode;
      } else {
        // 否则，将当前尾节点的 next 指向新节点，然后更新尾节点
        this.tail.next = newNode;
        this.tail = newNode;
      }

      this.length++;
      return this;
    }

    /**
     * 在链表头部添加节点
     * @param {*} value - 要添加的值
     * @returns {LinkedList}
     */
    prepend(value) {
      const newNode = new ListNode(value);

      if (this.length === 0) {
        this.head = newNode;
        this.tail = newNode;
      } else {
        newNode.next = this.head;  // 新节点的 next 指向原来的头
        this.head = newNode;       // 更新头节点
      }

      this.length++;
      return this;
    }

    /**
     * 根据索引获取节点
     * @param {number} index - 索引位置
     * @returns {ListNode|null} 找到的节点，找不到返回 null
     */
    get(index) {
      // 边界检查
      if (index < 0 || index >= this.length) return null;

      let current = this.head;
      // 从头节点开始遍历到目标位置
      for (let i = 0; i < index; i++) {
        current = current.next;
      }
      return current;
    }

    /**
     * 在指定位置插入节点
     * @param {number} index - 插入位置
     * @param {*} value - 插入的值
     * @returns {boolean} 是否插入成功
     */
    insert(index, value) {
      // 边界检查
      if (index < 0 || index > this.length) return false;

      // 插入头部
      if (index === 0) {
        this.prepend(value);
        return true;
      }

      // 插入尾部
      if (index === this.length) {
        this.append(value);
        return true;
      }

      // 插入中间
      const newNode = new ListNode(value);
      const prev = this.get(index - 1);  // 获取前一个节点
      newNode.next = prev.next;          // 新节点指向原来的当前节点
      prev.next = newNode;               // 前一个节点指向新节点
      this.length++;
      return true;
    }

    /**
     * 删除指定位置的节点
     * @param {number} index - 要删除的位置
     * @returns {ListNode|null} 被删除的节点
     */
    remove(index) {
      // 边界检查
      if (index < 0 || index >= this.length) return null;

      let removedNode;

      // 删除头节点
      if (index === 0) {
        removedNode = this.head;
        this.head = this.head.next;
        // 如果只有一个节点，删除后尾节点也为 null
        if (this.length === 1) {
          this.tail = null;
        }
      } else {
        const prev = this.get(index - 1);   // 获取前一个节点
        removedNode = prev.next;            // 被删除的节点
        prev.next = removedNode.next;       // 前一个节点指向被删节点的下一个
        // 如果删除的是尾节点，更新尾指针
        if (index === this.length - 1) {
          this.tail = prev;
        }
      }

      this.length--;
      return removedNode;
    }

    /**
     * 反转链表
     * @returns {LinkedList}
     */
    reverse() {
      // 空链表或只有一个节点，不需要反转
      if (this.length <= 1) return this;

      let prev = null;           // 前一个节点
      let current = this.head;   // 当前节点
      let next = null;           // 下一个节点

      this.tail = this.head;     // 反转后原来的头变成尾

      // 遍历链表，逐个反转指针方向
      while (current !== null) {
        next = current.next;     // 保存下一个节点
        current.next = prev;     // 当前节点指向前一个节点（反转）
        prev = current;          // prev 前移
        current = next;          // current 前移
      }

      this.head = prev;          // 循环结束后，prev 是新的头节点
      return this;
    }

    /**
     * 将链表转换为数组（便于打印和调试）
     * @returns {Array}
     */
    toArray() {
      const result = [];
      let current = this.head;
      while (current !== null) {
        result.push(current.value);
        current = current.next;
      }
      return result;
    }

    /**
     * 获取链表长度
     * @returns {number}
     */
    size() {
      return this.length;
    }
  }

  // 链表演示
  const list = new LinkedList();
  console.log('  初始链表:', list.toArray());

  list.append(1).append(2).append(3);
  console.log('  append 1, 2, 3 后:', list.toArray());

  list.prepend(0);
  console.log('  prepend 0 后:', list.toArray());

  list.insert(2, 100);
  console.log('  索引 2 处插入 100:', list.toArray());

  console.log('  索引 2 处的值:', list.get(2).value);

  list.remove(2);
  console.log('  删除索引 2 后:', list.toArray());

  list.reverse();
  console.log('  反转后:', list.toArray());

  console.log('  链表长度:', list.size());

  // ============================================================
  // 第二部分：栈（Stack）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、栈（Stack）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 栈的实现
  // ------------------------------------------------------------
  // 栈是一种后进先出（LIFO - Last In First Out）的数据结构
  // 只能在栈顶进行添加和删除操作
  // 应用场景：函数调用栈、撤销操作、括号匹配、表达式求值等
  console.log('\n--- 2.1 栈的实现 ---');

  /**
   * 栈类（基于数组实现）
   */
  class Stack {
    constructor() {
      this.items = [];  // 用数组存储栈元素
    }

    /**
     * 入栈：在栈顶添加元素
     * @param {*} element - 要添加的元素
     */
    push(element) {
      this.items.push(element);
    }

    /**
     * 出栈：移除并返回栈顶元素
     * @returns {*} 栈顶元素，栈空返回 undefined
     */
    pop() {
      if (this.isEmpty()) return undefined;
      return this.items.pop();
    }

    /**
     * 查看栈顶元素（不移除）
     * @returns {*} 栈顶元素
     */
    peek() {
      if (this.isEmpty()) return undefined;
      return this.items[this.items.length - 1];
    }

    /**
     * 判断栈是否为空
     * @returns {boolean}
     */
    isEmpty() {
      return this.items.length === 0;
    }

    /**
     * 获取栈的大小
     * @returns {number}
     */
    size() {
      return this.items.length;
    }

    /**
     * 清空栈
     */
    clear() {
      this.items = [];
    }

    /**
     * 转换为数组
     * @returns {Array}
     */
    toArray() {
      return [...this.items];
    }
  }

  // 栈演示
  const stack = new Stack();
  console.log('  初始栈是否为空:', stack.isEmpty());

  stack.push('A');
  stack.push('B');
  stack.push('C');
  console.log('  push A, B, C 后:', stack.toArray());
  console.log('  栈顶元素:', stack.peek());
  console.log('  栈大小:', stack.size());

  console.log('  pop:', stack.pop());
  console.log('  pop 后:', stack.toArray());

  // ------------------------------------------------------------
  // 2.2 栈的应用：括号匹配
  // ------------------------------------------------------------
  console.log('\n--- 2.2 栈的应用：括号匹配 ---');

  /**
   * 检查括号是否匹配
   * 思路：遇到左括号入栈，遇到右括号则检查栈顶是否为对应的左括号
   * @param {string} str - 要检查的字符串
   * @returns {boolean} 是否匹配
   */
  function isValidParentheses(str) {
    const stack = new Stack();
    // 定义括号的对应关系
    const pairs = {
      ')': '(',
      ']': '[',
      '}': '{'
    };

    for (const char of str) {
      // 如果是左括号，入栈
      if (char === '(' || char === '[' || char === '{') {
        stack.push(char);
      }
      // 如果是右括号
      else if (char === ')' || char === ']' || char === '}') {
        // 栈为空或栈顶不匹配，返回 false
        if (stack.isEmpty() || stack.peek() !== pairs[char]) {
          return false;
        }
        stack.pop(); // 匹配成功，弹出左括号
      }
    }

    // 栈必须为空才是完全匹配
    return stack.isEmpty();
  }

  console.log('  "()":', isValidParentheses('()'));           // true
  console.log('  "()[]{}":', isValidParentheses('()[]{}'));   // true
  console.log('  "(]":', isValidParentheses('(]'));           // false
  console.log('  "([)]":', isValidParentheses('([)]'));       // false
  console.log('  "{[]}":', isValidParentheses('{[]}'));       // true

  // ============================================================
  // 第三部分：队列（Queue）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、队列（Queue）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 队列的实现
  // ------------------------------------------------------------
  // 队列是一种先进先出（FIFO - First In First Out）的数据结构
  // 只能在队尾添加元素，在队头删除元素
  // 应用场景：任务队列、消息队列、打印队列、广度优先搜索等
  console.log('\n--- 3.1 队列的实现 ---');

  /**
   * 队列类（基于对象实现，比纯数组实现出队效率更高）
   */
  class Queue {
    constructor() {
      this.items = {};    // 用对象存储元素
      this.headIndex = 0; // 队头索引
      this.tailIndex = 0; // 队尾索引（下一个入队位置）
    }

    /**
     * 入队：在队尾添加元素
     * @param {*} element - 要添加的元素
     */
    enqueue(element) {
      this.items[this.tailIndex] = element;
      this.tailIndex++;
    }

    /**
     * 出队：移除并返回队头元素
     * @returns {*} 队头元素
     */
    dequeue() {
      if (this.isEmpty()) return undefined;

      const item = this.items[this.headIndex];
      delete this.items[this.headIndex];
      this.headIndex++;
      return item;
    }

    /**
     * 查看队头元素
     * @returns {*}
     */
    peek() {
      if (this.isEmpty()) return undefined;
      return this.items[this.headIndex];
    }

    /**
     * 判断队列是否为空
     * @returns {boolean}
     */
    isEmpty() {
      return this.tailIndex - this.headIndex === 0;
    }

    /**
     * 获取队列大小
     * @returns {number}
     */
    size() {
      return this.tailIndex - this.headIndex;
    }

    /**
     * 清空队列
     */
    clear() {
      this.items = {};
      this.headIndex = 0;
      this.tailIndex = 0;
    }

    /**
     * 转换为数组
     * @returns {Array}
     */
    toArray() {
      const result = [];
      for (let i = this.headIndex; i < this.tailIndex; i++) {
        result.push(this.items[i]);
      }
      return result;
    }
  }

  // 队列演示
  const queue = new Queue();
  console.log('  初始队列是否为空:', queue.isEmpty());

  queue.enqueue('任务1');
  queue.enqueue('任务2');
  queue.enqueue('任务3');
  console.log('  入队后:', queue.toArray());
  console.log('  队头元素:', queue.peek());
  console.log('  队列大小:', queue.size());

  console.log('  出队:', queue.dequeue());
  console.log('  出队后:', queue.toArray());

  // ------------------------------------------------------------
  // 3.2 循环队列
  // ------------------------------------------------------------
  console.log('\n--- 3.2 循环队列（双端队列简化版） ---');

  // 循环队列是一种固定大小的队列，头尾相连，可以复用空间
  // 这里演示一个基于数组的简单实现

  /**
   * 循环队列
   */
  class CircularQueue {
    /**
     * @param {number} capacity - 队列容量
     */
    constructor(capacity) {
      this.capacity = capacity;  // 队列容量
      this.items = new Array(capacity);
      this.head = 0;             // 队头指针
      this.count = 0;            // 元素数量
    }

    // 计算队尾位置
    get tailIndex() {
      return (this.head + this.count) % this.capacity;
    }

    enqueue(element) {
      if (this.isFull()) return false;
      this.items[this.tailIndex] = element;
      this.count++;
      return true;
    }

    dequeue() {
      if (this.isEmpty()) return undefined;
      const item = this.items[this.head];
      this.items[this.head] = undefined;
      this.head = (this.head + 1) % this.capacity;
      this.count--;
      return item;
    }

    isEmpty() {
      return this.count === 0;
    }

    isFull() {
      return this.count === this.capacity;
    }

    size() {
      return this.count;
    }

    toArray() {
      const result = [];
      for (let i = 0; i < this.count; i++) {
        result.push(this.items[(this.head + i) % this.capacity]);
      }
      return result;
    }
  }

  const circularQueue = new CircularQueue(5);
  console.log('  循环队列容量: 5');
  for (let i = 1; i <= 5; i++) {
    circularQueue.enqueue(i);
  }
  console.log('  满队列:', circularQueue.toArray());
  console.log('  是否满:', circularQueue.isFull());

  circularQueue.dequeue();
  circularQueue.dequeue();
  console.log('  出队两个后:', circularQueue.toArray());

  circularQueue.enqueue(6);
  circularQueue.enqueue(7);
  console.log('  再入队两个:', circularQueue.toArray());

  // ============================================================
  // 第四部分：二叉搜索树（Binary Search Tree）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、二叉搜索树（BST）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 二叉搜索树的实现
  // ------------------------------------------------------------
  // 二叉搜索树是一种特殊的二叉树，满足：
  // - 左子树所有节点的值都小于根节点的值
  // - 右子树所有节点的值都大于根节点的值
  // - 左右子树也都是二叉搜索树
  // 平均查找、插入、删除的时间复杂度都是 O(log n)
  console.log('\n--- 4.1 二叉搜索树实现 ---');

  /**
   * 树节点类
   */
  class TreeNode {
    constructor(value) {
      this.value = value;   // 节点值
      this.left = null;     // 左子节点
      this.right = null;    // 右子节点
    }
  }

  /**
   * 二叉搜索树类
   */
  class BinarySearchTree {
    constructor() {
      this.root = null;  // 根节点
    }

    /**
     * 插入节点
     * @param {*} value - 要插入的值
     * @returns {BinarySearchTree}
     */
    insert(value) {
      const newNode = new TreeNode(value);

      // 空树，直接作为根节点
      if (this.root === null) {
        this.root = newNode;
        return this;
      }

      // 从根节点开始查找插入位置
      let current = this.root;
      while (true) {
        // 值相同，不插入（或根据需求处理）
        if (value === current.value) return this;

        // 值小于当前节点，往左走
        if (value < current.value) {
          if (current.left === null) {
            current.left = newNode;
            return this;
          }
          current = current.left;
        }
        // 值大于当前节点，往右走
        else {
          if (current.right === null) {
            current.right = newNode;
            return this;
          }
          current = current.right;
        }
      }
    }

    /**
     * 查找节点
     * @param {*} value - 要查找的值
     * @returns {TreeNode|null} 找到的节点
     */
    find(value) {
      let current = this.root;

      while (current !== null) {
        if (value === current.value) return current;   // 找到
        if (value < current.value) {
          current = current.left;  // 往左找
        } else {
          current = current.right; // 往右找
        }
      }

      return null; // 没找到
    }

    /**
     * 判断值是否存在
     * @param {*} value
     * @returns {boolean}
     */
    contains(value) {
      return this.find(value) !== null;
    }

    /**
     * 前序遍历（根 - 左 - 右）
     * @returns {Array} 遍历结果
     */
    preOrder() {
      const result = [];

      // 递归辅助函数
      function traverse(node) {
        if (node === null) return;
        result.push(node.value);   // 访问根节点
        traverse(node.left);       // 遍历左子树
        traverse(node.right);      // 遍历右子树
      }

      traverse(this.root);
      return result;
    }

    /**
     * 中序遍历（左 - 根 - 右）
     * 注意：BST 的中序遍历结果是升序排列的
     * @returns {Array}
     */
    inOrder() {
      const result = [];

      function traverse(node) {
        if (node === null) return;
        traverse(node.left);       // 遍历左子树
        result.push(node.value);   // 访问根节点
        traverse(node.right);      // 遍历右子树
      }

      traverse(this.root);
      return result;
    }

    /**
     * 后序遍历（左 - 右 - 根）
     * @returns {Array}
     */
    postOrder() {
      const result = [];

      function traverse(node) {
        if (node === null) return;
        traverse(node.left);       // 遍历左子树
        traverse(node.right);      // 遍历右子树
        result.push(node.value);   // 访问根节点
      }

      traverse(this.root);
      return result;
    }

    /**
     * 层序遍历（广度优先 BFS）
     * 从上到下、从左到右逐层遍历
     * @returns {Array}
     */
    levelOrder() {
      const result = [];
      if (this.root === null) return result;

      // 使用队列辅助
      const queue = [];
      queue.push(this.root);

      while (queue.length > 0) {
        const node = queue.shift();  // 出队
        result.push(node.value);

        // 左子节点入队
        if (node.left !== null) {
          queue.push(node.left);
        }
        // 右子节点入队
        if (node.right !== null) {
          queue.push(node.right);
        }
      }

      return result;
    }

    /**
     * 获取最小值（最左节点）
     * @returns {*}
     */
    min() {
      if (this.root === null) return undefined;
      let current = this.root;
      while (current.left !== null) {
        current = current.left;
      }
      return current.value;
    }

    /**
     * 获取最大值（最右节点）
     * @returns {*}
     */
    max() {
      if (this.root === null) return undefined;
      let current = this.root;
      while (current.right !== null) {
        current = current.right;
      }
      return current.value;
    }
  }

  // 二叉搜索树演示
  const bst = new BinarySearchTree();

  // 插入数据
  [10, 6, 15, 3, 8, 12, 20, 2, 4].forEach(val => bst.insert(val));

  console.log('  插入数据: 10, 6, 15, 3, 8, 12, 20, 2, 4');
  console.log('  前序遍历:', bst.preOrder());
  console.log('  中序遍历（升序）:', bst.inOrder());
  console.log('  后序遍历:', bst.postOrder());
  console.log('  层序遍历:', bst.levelOrder());

  console.log('  查找 8:', bst.contains(8));   // true
  console.log('  查找 99:', bst.contains(99)); // false

  console.log('  最小值:', bst.min()); // 2
  console.log('  最大值:', bst.max()); // 20

  // ============================================================
  // 第五部分：排序算法
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、常见排序算法');
  console.log('='.repeat(60));

  // 用于测试的数组
  const testArr = [64, 34, 25, 12, 22, 11, 90, 25];

  // ------------------------------------------------------------
  // 5.1 冒泡排序
  // ------------------------------------------------------------
  // 原理：重复遍历数组，比较相邻元素，如果顺序错误就交换
  // 每一轮会把最大的元素"冒泡"到最后
  // 时间复杂度：O(n^2)，空间复杂度：O(1)，稳定排序
  console.log('\n--- 5.1 冒泡排序 ---');

  function bubbleSort(arr) {
    const len = arr.length;
    // 外层循环：需要多少轮
    for (let i = 0; i < len - 1; i++) {
      let swapped = false; // 优化：标记本轮是否有交换
      // 内层循环：每轮比较的范围（末尾 i 个已经排好序）
      for (let j = 0; j < len - 1 - i; j++) {
        // 相邻元素比较，前 > 后则交换
        if (arr[j] > arr[j + 1]) {
          // 交换
          [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]];
          swapped = true;
        }
      }
      // 如果本轮没有交换，说明已经有序，提前结束
      if (!swapped) break;
    }
    return arr;
  }

  console.log('  原数组:', [...testArr]);
  console.log('  排序后:', bubbleSort([...testArr]));

  // ------------------------------------------------------------
  // 5.2 选择排序
  // ------------------------------------------------------------
  // 原理：每次从未排序部分选出最小（大）的元素，放到已排序部分的末尾
  // 时间复杂度：O(n^2)，空间复杂度：O(1)，不稳定排序
  console.log('\n--- 5.2 选择排序 ---');

  function selectionSort(arr) {
    const len = arr.length;

    for (let i = 0; i < len - 1; i++) {
      let minIndex = i; // 假设当前位置是最小值的索引
      // 在未排序部分找最小值
      for (let j = i + 1; j < len; j++) {
        if (arr[j] < arr[minIndex]) {
          minIndex = j;
        }
      }
      // 将最小值交换到已排序部分的末尾
      if (minIndex !== i) {
        [arr[i], arr[minIndex]] = [arr[minIndex], arr[i]];
      }
    }
    return arr;
  }

  console.log('  原数组:', [...testArr]);
  console.log('  排序后:', selectionSort([...testArr]));

  // ------------------------------------------------------------
  // 5.3 插入排序
  // ------------------------------------------------------------
  // 原理：将数组分为已排序和未排序两部分，逐个将未排序元素插入到已排序部分的正确位置
  // 时间复杂度：O(n^2)，空间复杂度：O(1)，稳定排序
  // 对于近乎有序的数组效率很高，是简单排序中最快的
  console.log('\n--- 5.3 插入排序 ---');

  function insertionSort(arr) {
    const len = arr.length;

    // 从第二个元素开始，逐个插入到前面已排序的部分
    for (let i = 1; i < len; i++) {
      const current = arr[i]; // 当前要插入的元素
      let j = i - 1;          // 已排序部分的最后一个位置

      // 从后往前找插入位置，比 current 大的元素都往后移
      while (j >= 0 && arr[j] > current) {
        arr[j + 1] = arr[j];  // 元素后移
        j--;
      }
      arr[j + 1] = current;   // 插入到正确位置
    }
    return arr;
  }

  console.log('  原数组:', [...testArr]);
  console.log('  排序后:', insertionSort([...testArr]));

  // ------------------------------------------------------------
  // 5.4 快速排序
  // ------------------------------------------------------------
  // 原理：分治思想，选一个基准值（pivot），将数组分成两部分：
  //       小于基准的放左边，大于基准的放右边，然后递归排序左右两部分
  // 平均时间复杂度：O(n log n)，最坏 O(n^2)
  // 空间复杂度：O(log n)（递归栈），不稳定排序
  console.log('\n--- 5.4 快速排序 ---');

  function quickSort(arr, left = 0, right = arr.length - 1) {
    if (left < right) {
      // 获取分区点索引
      const pivotIndex = partition(arr, left, right);
      // 递归排序左半部分
      quickSort(arr, left, pivotIndex - 1);
      // 递归排序右半部分
      quickSort(arr, pivotIndex + 1, right);
    }
    return arr;
  }

  /**
   * 分区函数：以最后一个元素为基准，将数组分为两部分
   * 返回基准元素的最终位置
   */
  function partition(arr, left, right) {
    const pivot = arr[right]; // 选最右边作为基准
    let i = left - 1;         // i 指向小于基准的区域的末尾

    // 遍历所有元素，将小于基准的放到左边
    for (let j = left; j < right; j++) {
      if (arr[j] < pivot) {
        i++;
        [arr[i], arr[j]] = [arr[j], arr[i]];
      }
    }

    // 将基准放到正确位置（i+1 就是基准的最终位置）
    [arr[i + 1], arr[right]] = [arr[right], arr[i + 1]];
    return i + 1;
  }

  console.log('  原数组:', [...testArr]);
  console.log('  排序后:', quickSort([...testArr]));

  // ------------------------------------------------------------
  // 5.5 归并排序
  // ------------------------------------------------------------
  // 原理：分治思想，将数组不断二分，直到每个子数组只有一个元素（天然有序），
  //       然后将有序子数组合并成更大的有序数组
  // 时间复杂度：O(n log n)（始终），空间复杂度：O(n)，稳定排序
  console.log('\n--- 5.5 归并排序 ---');

  function mergeSort(arr) {
    // 递归终止条件：数组长度为 1 时已经有序
    if (arr.length <= 1) return arr;

    // 将数组从中点分成两半
    const mid = Math.floor(arr.length / 2);
    const left = arr.slice(0, mid);
    const right = arr.slice(mid);

    // 递归排序左右两部分，然后合并
    return merge(mergeSort(left), mergeSort(right));
  }

  /**
   * 合并两个有序数组
   */
  function merge(left, right) {
    const result = [];
    let i = 0; // 左数组指针
    let j = 0; // 右数组指针

    // 比较两个数组的元素，将较小的放入结果
    while (i < left.length && j < right.length) {
      if (left[i] <= right[j]) {
        result.push(left[i]);
        i++;
      } else {
        result.push(right[j]);
        j++;
      }
    }

    // 将剩余元素（如果有）加入结果
    while (i < left.length) {
      result.push(left[i]);
      i++;
    }
    while (j < right.length) {
      result.push(right[j]);
      j++;
    }

    return result;
  }

  console.log('  原数组:', [...testArr]);
  console.log('  排序后:', mergeSort([...testArr]));

  // ------------------------------------------------------------
  // 5.6 排序算法对比
  // ------------------------------------------------------------
  console.log('\n--- 5.6 排序算法对比总结 ---');
  console.log('  | 算法     | 平均时间   | 最坏时间   | 空间   | 稳定性 |');
  console.log('  |---------|-----------|-----------|--------|--------|');
  console.log('  | 冒泡排序 | O(n²)     | O(n²)     | O(1)   | 稳定   |');
  console.log('  | 选择排序 | O(n²)     | O(n²)     | O(1)   | 不稳定 |');
  console.log('  | 插入排序 | O(n²)     | O(n²)     | O(1)   | 稳定   |');
  console.log('  | 快速排序 | O(n log n)| O(n²)     | O(log n)|不稳定|');
  console.log('  | 归并排序 | O(n log n)| O(n log n)| O(n)   | 稳定   |');
  console.log('  | 堆排序   | O(n log n)| O(n log n)| O(1)   | 不稳定 |');
  console.log('  | 计数排序 | O(n+k)    | O(n+k)    | O(k)   | 稳定   |');
  console.log('  稳定排序：相等元素排序后相对位置不变');

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('数据结构与算法 演示结束');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
