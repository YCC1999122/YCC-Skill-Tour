// ============================================================
// package structures 数据结构模块
// 使用 Go 实现常见的数据结构和算法
// 包括：链表、栈、队列、二叉搜索树、堆/优先队列、排序算法
// ============================================================
package structures

import (
	"container/heap"
	"fmt"
)

// ============================================================
// RunStructuresDemo 是数据结构模块的入口函数
// ============================================================
func RunStructuresDemo() {
	fmt.Println("\n========== 数据结构演示 ==========")

	demonstrateLinkedList()  // 演示链表
	demonstrateStack()       // 演示栈
	demonstrateQueue()       // 演示队列
	demonstrateBST()         // 演示二叉搜索树
	demonstrateHeap()        // 演示堆/优先队列
	demonstrateSorting()     // 演示排序算法
}

// ============================================================
// 1. 链表（Linked List）
// ============================================================
// 链表是一种线性数据结构，由节点组成，每个节点包含数据和指向下一个节点的指针
// 与数组相比：
// 优点：插入删除快（O(1)，如果知道位置），不需要连续内存
// 缺点：随机访问慢（O(n)），额外的指针开销

// ListNode 链表节点
type ListNode struct {
	Val  int       // 节点存储的值
	Next *ListNode // 指向下一个节点的指针
}

// LinkedList 单链表
type LinkedList struct {
	head *ListNode // 头节点指针
	size int       // 链表大小
}

// NewLinkedList 创建一个新的空链表
func NewLinkedList() *LinkedList {
	return &LinkedList{
		head: nil,
		size: 0,
	}
}

// AddAtHead 在链表头部添加节点
// 时间复杂度：O(1)
func (l *LinkedList) AddAtHead(val int) {
	// 创建新节点
	newNode := &ListNode{
		Val:  val,
		Next: l.head, // 新节点的 Next 指向原来的头节点
	}
	// 头节点更新为新节点
	l.head = newNode
	l.size++
}

// AddAtTail 在链表尾部添加节点
// 时间复杂度：O(n)（因为需要遍历到尾部）
func (l *LinkedList) AddAtTail(val int) {
	newNode := &ListNode{Val: val}

	// 如果链表为空，新节点就是头节点
	if l.head == nil {
		l.head = newNode
	} else {
		// 遍历到最后一个节点
		current := l.head
		for current.Next != nil {
			current = current.Next
		}
		// 将最后一个节点的 Next 指向新节点
		current.Next = newNode
	}
	l.size++
}

// AddAtIndex 在指定索引位置添加节点
// 索引从 0 开始
// 时间复杂度：O(n)
func (l *LinkedList) AddAtIndex(index int, val int) bool {
	// 检查索引是否合法
	if index < 0 || index > l.size {
		return false
	}

	// 索引为 0，在头部添加
	if index == 0 {
		l.AddAtHead(val)
		return true
	}

	// 找到索引位置的前一个节点
	current := l.head
	for i := 0; i < index-1; i++ {
		current = current.Next
	}

	// 插入新节点
	newNode := &ListNode{Val: val}
	newNode.Next = current.Next
	current.Next = newNode
	l.size++
	return true
}

// DeleteAtIndex 删除指定索引位置的节点
// 时间复杂度：O(n)
func (l *LinkedList) DeleteAtIndex(index int) bool {
	if index < 0 || index >= l.size {
		return false
	}

	// 删除头节点
	if index == 0 {
		l.head = l.head.Next
		l.size--
		return true
	}

	// 找到索引位置的前一个节点
	current := l.head
	for i := 0; i < index-1; i++ {
		current = current.Next
	}

	// 跳过要删除的节点
	current.Next = current.Next.Next
	l.size--
	return true
}

// Get 获取指定索引位置的节点值
// 时间复杂度：O(n)
func (l *LinkedList) Get(index int) (int, bool) {
	if index < 0 || index >= l.size {
		return 0, false
	}

	current := l.head
	for i := 0; i < index; i++ {
		current = current.Next
	}
	return current.Val, true
}

// Size 获取链表大小
func (l *LinkedList) Size() int {
	return l.size
}

// Print 打印链表
func (l *LinkedList) Print() {
	current := l.head
	fmt.Print("    链表: ")
	for current != nil {
		fmt.Printf("%d -> ", current.Val)
		current = current.Next
	}
	fmt.Println("nil")
}

// demonstrateLinkedList 演示链表
func demonstrateLinkedList() {
	fmt.Println("\n--- 1. 链表 LinkedList ---")

	list := NewLinkedList()

	// 头部添加
	list.AddAtHead(1)
	list.AddAtHead(2)
	list.AddAtHead(3)
	fmt.Println("  头部添加 1, 2, 3：")
	list.Print()
	fmt.Printf("    大小: %d\n", list.Size())

	// 尾部添加
	list.AddAtTail(4)
	list.AddAtTail(5)
	fmt.Println("  尾部添加 4, 5：")
	list.Print()

	// 指定位置添加
	list.AddAtIndex(2, 100)
	fmt.Println("  索引2处插入 100：")
	list.Print()

	// 获取元素
	if val, ok := list.Get(2); ok {
		fmt.Printf("  获取索引2的值: %d\n", val)
	}

	// 删除元素
	list.DeleteAtIndex(2)
	fmt.Println("  删除索引2的元素：")
	list.Print()

	// 删除头节点
	list.DeleteAtIndex(0)
	fmt.Println("  删除头节点：")
	list.Print()
}

// ============================================================
// 2. 栈（Stack）
// ============================================================
// 栈是一种后进先出（LIFO）的数据结构
// 只能在栈顶进行插入和删除操作
// 常见操作：Push（入栈）、Pop（出栈）、Peek（查看栈顶）
//
// 使用切片实现栈非常简单高效

// Stack 栈（使用切片实现）
type Stack struct {
	items []int // 底层使用切片存储
}

// NewStack 创建一个新的空栈
func NewStack() *Stack {
	return &Stack{
		items: make([]int, 0),
	}
}

// Push 入栈
// 时间复杂度：O(1)（均摊）
func (s *Stack) Push(val int) {
	s.items = append(s.items, val)
}

// Pop 出栈
// 返回栈顶元素并移除
// 时间复杂度：O(1)
func (s *Stack) Pop() (int, bool) {
	if len(s.items) == 0 {
		return 0, false // 栈为空
	}

	// 获取最后一个元素（栈顶）
	top := s.items[len(s.items)-1]
	// 移除最后一个元素
	s.items = s.items[:len(s.items)-1]
	return top, true
}

// Peek 查看栈顶元素（不移除）
func (s *Stack) Peek() (int, bool) {
	if len(s.items) == 0 {
		return 0, false
	}
	return s.items[len(s.items)-1], true
}

// IsEmpty 判断栈是否为空
func (s *Stack) IsEmpty() bool {
	return len(s.items) == 0
}

// Size 获取栈大小
func (s *Stack) Size() int {
	return len(s.items)
}

// demonstrateStack 演示栈
func demonstrateStack() {
	fmt.Println("\n--- 2. 栈 Stack ---")

	stack := NewStack()

	// 入栈
	stack.Push(10)
	stack.Push(20)
	stack.Push(30)
	fmt.Printf("  入栈 10, 20, 30 后，大小: %d\n", stack.Size())

	// 查看栈顶
	if top, ok := stack.Peek(); ok {
		fmt.Printf("  栈顶元素: %d\n", top)
	}

	// 出栈
	fmt.Print("  出栈顺序: ")
	for !stack.IsEmpty() {
		if val, ok := stack.Pop(); ok {
			fmt.Printf("%d ", val)
		}
	}
	fmt.Println()
	fmt.Printf("  出栈后是否为空: %t\n", stack.IsEmpty())

	// 栈的应用场景：
	// 1. 函数调用栈
	// 2. 表达式求值（逆波兰表达式）
	// 3. 括号匹配
	// 4. 浏览器前进后退
	// 5. 深度优先搜索（DFS）
}

// ============================================================
// 3. 队列（Queue）
// ============================================================
// 队列是一种先进先出（FIFO）的数据结构
// 只能在队尾插入，队头删除
// 常见操作：Enqueue（入队）、Dequeue（出队）、Front（查看队头）
//
// 使用切片实现队列，为了提高效率可以使用环形缓冲
// 这里使用简单的切片实现

// Queue 队列（使用切片实现）
type Queue struct {
	items []int // 底层使用切片存储
}

// NewQueue 创建一个新的空队列
func NewQueue() *Queue {
	return &Queue{
		items: make([]int, 0),
	}
}

// Enqueue 入队（在队尾添加）
// 时间复杂度：O(1)（均摊）
func (q *Queue) Enqueue(val int) {
	q.items = append(q.items, val)
}

// Dequeue 出队（移除队头元素）
// 时间复杂度：O(n)（因为要移动所有元素，简单实现）
// 优化方案：使用环形缓冲或链表实现
func (q *Queue) Dequeue() (int, bool) {
	if len(q.items) == 0 {
		return 0, false
	}

	front := q.items[0]
	q.items = q.items[1:] // 移除第一个元素
	return front, true
}

// Front 查看队头元素
func (q *Queue) Front() (int, bool) {
	if len(q.items) == 0 {
		return 0, false
	}
	return q.items[0], true
}

// IsEmpty 判断队列是否为空
func (q *Queue) IsEmpty() bool {
	return len(q.items) == 0
}

// Size 获取队列大小
func (q *Queue) Size() int {
	return len(q.items)
}

// demonstrateQueue 演示队列
func demonstrateQueue() {
	fmt.Println("\n--- 3. 队列 Queue ---")

	queue := NewQueue()

	// 入队
	queue.Enqueue(10)
	queue.Enqueue(20)
	queue.Enqueue(30)
	fmt.Printf("  入队 10, 20, 30 后，大小: %d\n", queue.Size())

	// 查看队头
	if front, ok := queue.Front(); ok {
		fmt.Printf("  队头元素: %d\n", front)
	}

	// 出队
	fmt.Print("  出队顺序: ")
	for !queue.IsEmpty() {
		if val, ok := queue.Dequeue(); ok {
			fmt.Printf("%d ", val)
		}
	}
	fmt.Println()
	fmt.Printf("  出队后是否为空: %t\n", queue.IsEmpty())

	// 队列的应用场景：
	// 1. 任务调度
	// 2. 消息队列
	// 3. 广度优先搜索（BFS）
	// 4. 缓冲区
}

// ============================================================
// 4. 二叉搜索树（Binary Search Tree, BST）
// ============================================================
// 二叉搜索树是一种特殊的二叉树，满足：
// - 左子树所有节点的值 < 根节点的值
// - 右子树所有节点的值 > 根节点的值
// - 左右子树也都是二叉搜索树
//
// 平均时间复杂度：查找、插入、删除都是 O(log n)
// 最坏情况（退化成链表）：O(n)

// TreeNode 二叉树节点
type TreeNode struct {
	Val   int       // 节点值
	Left  *TreeNode // 左子节点
	Right *TreeNode // 右子节点
}

// BST 二叉搜索树
type BST struct {
	root *TreeNode // 根节点
}

// NewBST 创建一个新的空二叉搜索树
func NewBST() *BST {
	return &BST{root: nil}
}

// Insert 插入节点
// 时间复杂度：平均 O(log n)，最坏 O(n)
func (b *BST) Insert(val int) {
	b.root = b.insert(b.root, val)
}

// insert 递归插入辅助函数
func (b *BST) insert(node *TreeNode, val int) *TreeNode {
	// 找到空位置，创建新节点
	if node == nil {
		return &TreeNode{Val: val}
	}

	// 值小于当前节点，插入左子树
	if val < node.Val {
		node.Left = b.insert(node.Left, val)
	} else if val > node.Val {
		// 值大于当前节点，插入右子树
		node.Right = b.insert(node.Right, val)
	}
	// 值相等，不插入（BST 通常不允许重复值）

	return node
}

// Search 查找节点
// 时间复杂度：平均 O(log n)，最坏 O(n)
func (b *BST) Search(val int) bool {
	return b.search(b.root, val)
}

// search 递归查找辅助函数
func (b *BST) search(node *TreeNode, val int) bool {
	// 节点为空，未找到
	if node == nil {
		return false
	}

	// 找到目标值
	if val == node.Val {
		return true
	}

	// 值小于当前节点，去左子树找
	if val < node.Val {
		return b.search(node.Left, val)
	}
	// 值大于当前节点，去右子树找
	return b.search(node.Right, val)
}

// Delete 删除节点
// 时间复杂度：平均 O(log n)，最坏 O(n)
func (b *BST) Delete(val int) {
	b.root = b.delete(b.root, val)
}

// delete 递归删除辅助函数
// 删除节点有三种情况：
// 1. 叶子节点：直接删除
// 2. 只有一个子节点：用子节点替换
// 3. 有两个子节点：用右子树的最小值（或左子树的最大值）替换
func (b *BST) delete(node *TreeNode, val int) *TreeNode {
	if node == nil {
		return nil
	}

	// 找到要删除的节点
	if val < node.Val {
		node.Left = b.delete(node.Left, val)
	} else if val > node.Val {
		node.Right = b.delete(node.Right, val)
	} else {
		// 找到了要删除的节点

		// 情况1：叶子节点（没有子节点）
		if node.Left == nil && node.Right == nil {
			return nil
		}

		// 情况2：只有一个子节点
		if node.Left == nil {
			return node.Right // 只有右子节点
		}
		if node.Right == nil {
			return node.Left // 只有左子节点
		}

		// 情况3：有两个子节点
		// 找到右子树的最小值（后继节点）
		minNode := b.findMin(node.Right)
		// 用后继节点的值替换当前节点
		node.Val = minNode.Val
		// 删除后继节点
		node.Right = b.delete(node.Right, minNode.Val)
	}

	return node
}

// findMin 查找最小节点（最左节点）
func (b *BST) findMin(node *TreeNode) *TreeNode {
	for node.Left != nil {
		node = node.Left
	}
	return node
}

// PreOrder 前序遍历（根-左-右）
func (b *BST) PreOrder() []int {
	var result []int
	b.preOrder(b.root, &result)
	return result
}

func (b *BST) preOrder(node *TreeNode, result *[]int) {
	if node == nil {
		return
	}
	*result = append(*result, node.Val) // 访问根节点
	b.preOrder(node.Left, result)      // 遍历左子树
	b.preOrder(node.Right, result)     // 遍历右子树
}

// InOrder 中序遍历（左-根-右）
// 二叉搜索树的中序遍历结果是有序的
func (b *BST) InOrder() []int {
	var result []int
	b.inOrder(b.root, &result)
	return result
}

func (b *BST) inOrder(node *TreeNode, result *[]int) {
	if node == nil {
		return
	}
	b.inOrder(node.Left, result)      // 遍历左子树
	*result = append(*result, node.Val) // 访问根节点
	b.inOrder(node.Right, result)     // 遍历右子树
}

// PostOrder 后序遍历（左-右-根）
func (b *BST) PostOrder() []int {
	var result []int
	b.postOrder(b.root, &result)
	return result
}

func (b *BST) postOrder(node *TreeNode, result *[]int) {
	if node == nil {
		return
	}
	b.postOrder(node.Left, result)   // 遍历左子树
	b.postOrder(node.Right, result)  // 遍历右子树
	*result = append(*result, node.Val) // 访问根节点
}

// demonstrateBST 演示二叉搜索树
func demonstrateBST() {
	fmt.Println("\n--- 4. 二叉搜索树 BST ---")

	bst := NewBST()

	// 插入节点
	values := []int{50, 30, 70, 20, 40, 60, 80}
	for _, v := range values {
		bst.Insert(v)
	}
	fmt.Printf("  插入节点: %v\n", values)

	// 遍历
	fmt.Printf("  前序遍历: %v\n", bst.PreOrder())
	fmt.Printf("  中序遍历: %v (BST的中序是有序的)\n", bst.InOrder())
	fmt.Printf("  后序遍历: %v\n", bst.PostOrder())

	// 查找
	fmt.Printf("  查找 40: %t\n", bst.Search(40))
	fmt.Printf("  查找 99: %t\n", bst.Search(99))

	// 删除叶子节点
	bst.Delete(20)
	fmt.Printf("  删除 20（叶子）后中序: %v\n", bst.InOrder())

	// 删除只有一个子节点的节点
	bst.Delete(30)
	fmt.Printf("  删除 30（一个子节点）后中序: %v\n", bst.InOrder())

	// 删除有两个子节点的节点
	bst.Delete(50)
	fmt.Printf("  删除 50（两个子节点）后中序: %v\n", bst.InOrder())
}

// ============================================================
// 5. 堆 / 优先队列（使用 container/heap）
// ============================================================
// 堆是一种特殊的完全二叉树，分为：
// - 最大堆：父节点 >= 子节点
// - 最小堆：父节点 <= 子节点
//
// Go 标准库提供了 container/heap 接口
// 只需要实现 heap.Interface 即可使用堆操作
//
// heap.Interface 包含五个方法：
// - Len() int
// - Less(i, j int) bool
// - Swap(i, j int)
// - Push(x interface{})
// - Pop() interface{}
//
// 注意：Push 和 Pop 由 heap 包调用，
// 使用者应该调用 heap.Push 和 heap.Pop

// IntHeap 最小堆（int 类型）
// 使用切片作为底层存储
type IntHeap []int

// Len 返回堆大小
func (h IntHeap) Len() int {
	return len(h)
}

// Less 比较两个元素的大小
// 这里实现的是最小堆（i < j 返回 true）
// 如果要实现最大堆，改为 h[i] > h[j]
func (h IntHeap) Less(i, j int) bool {
	return h[i] < h[j] // 最小堆
}

// Swap 交换两个元素
func (h IntHeap) Swap(i, j int) {
	h[i], h[j] = h[j], h[i]
}

// Push 添加元素
// 注意：这个方法由 heap 包内部调用，不要直接调用
// 应该调用 heap.Push(h, x)
func (h *IntHeap) Push(x interface{}) {
	*h = append(*h, x.(int))
}

// Pop 移除并返回最小元素
// 注意：这个方法由 heap 包内部调用，不要直接调用
// 应该调用 heap.Pop(h)
func (h *IntHeap) Pop() interface{} {
	old := *h
	n := len(old)
	x := old[n-1]     // 最后一个元素（最小元素已经被交换到这里）
	*h = old[:n-1]    // 移除最后一个元素
	return x
}

// PriorityItem 优先队列元素
type PriorityItem struct {
	Value    string // 元素值
	Priority int    // 优先级（数值越小优先级越高）
}

// PriorityQueue 优先队列
type PriorityQueue []*PriorityItem

func (pq PriorityQueue) Len() int {
	return len(pq)
}

func (pq PriorityQueue) Less(i, j int) bool {
	// 优先级数值越小，优先级越高
	return pq[i].Priority < pq[j].Priority
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}

func (pq *PriorityQueue) Push(x interface{}) {
	item := x.(*PriorityItem)
	*pq = append(*pq, item)
}

func (pq *PriorityQueue) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	old[n-1] = nil // 避免内存泄漏
	*pq = old[:n-1]
	return item
}

// demonstrateHeap 演示堆/优先队列
func demonstrateHeap() {
	fmt.Println("\n--- 5. 堆 / 优先队列 Heap ---")

	// ---------- 最小堆 ----------
	fmt.Println("  最小堆：")

	h := &IntHeap{2, 1, 5, 6, 3, 4}
	heap.Init(h) // 初始化堆（建堆）
	fmt.Printf("    初始化后: %v\n", *h)

	// 插入元素
	heap.Push(h, 0)
	fmt.Printf("    Push 0 后: %v\n", *h)

	// 弹出最小元素
	fmt.Print("    弹出顺序: ")
	for h.Len() > 0 {
		val := heap.Pop(h).(int)
		fmt.Printf("%d ", val)
	}
	fmt.Println()

	// ---------- 优先队列 ----------
	fmt.Println("\n  优先队列：")

	pq := &PriorityQueue{}
	heap.Init(pq)

	// 添加任务（优先级数值越小优先级越高）
	heap.Push(pq, &PriorityItem{Value: "紧急任务", Priority: 1})
	heap.Push(pq, &PriorityItem{Value: "普通任务", Priority: 3})
	heap.Push(pq, &PriorityItem{Value: "高优先级任务", Priority: 2})
	heap.Push(pq, &PriorityItem{Value: "低优先级任务", Priority: 5})
	heap.Push(pq, &PriorityItem{Value: "中等任务", Priority: 4})

	fmt.Println("    按优先级处理：")
	for pq.Len() > 0 {
		item := heap.Pop(pq).(*PriorityItem)
		fmt.Printf("      优先级%d: %s\n", item.Priority, item.Value)
	}

	// 堆的应用场景：
	// 1. 优先队列
	// 2. 堆排序
	// 3. Top K 问题
	// 4. 调度算法
}

// ============================================================
// 6. 排序算法实现
// ============================================================

// ---------- 冒泡排序（Bubble Sort）----------
// 原理：重复遍历数组，比较相邻元素，如果顺序错误就交换
// 时间复杂度：O(n^2)
// 空间复杂度：O(1)
// 稳定性：稳定

// BubbleSort 冒泡排序
func BubbleSort(arr []int) []int {
	n := len(arr)
	// 复制一份，不修改原数组
	result := make([]int, n)
	copy(result, arr)

	for i := 0; i < n-1; i++ {
		swapped := false
		// 每轮将最大的元素"冒泡"到最后
		for j := 0; j < n-i-1; j++ {
			if result[j] > result[j+1] {
				// 交换
				result[j], result[j+1] = result[j+1], result[j]
				swapped = true
			}
		}
		// 如果本轮没有交换，说明已经有序，可以提前退出
		if !swapped {
			break
		}
	}
	return result
}

// ---------- 选择排序（Selection Sort）----------
// 原理：每次从未排序部分找到最小元素，放到已排序部分的末尾
// 时间复杂度：O(n^2)
// 空间复杂度：O(1)
// 稳定性：不稳定

// SelectionSort 选择排序
func SelectionSort(arr []int) []int {
	n := len(arr)
	result := make([]int, n)
	copy(result, arr)

	for i := 0; i < n-1; i++ {
		minIdx := i // 最小元素索引
		// 在未排序部分找最小元素
		for j := i + 1; j < n; j++ {
			if result[j] < result[minIdx] {
				minIdx = j
			}
		}
		// 交换到已排序部分的末尾
		result[i], result[minIdx] = result[minIdx], result[i]
	}
	return result
}

// ---------- 插入排序（Insertion Sort）----------
// 原理：将未排序元素插入到已排序部分的正确位置
// 时间复杂度：O(n^2)，最好 O(n)（已经有序时）
// 空间复杂度：O(1)
// 稳定性：稳定
// 特点：对于小规模或基本有序的数据效率很高

// InsertionSort 插入排序
func InsertionSort(arr []int) []int {
	n := len(arr)
	result := make([]int, n)
	copy(result, arr)

	for i := 1; i < n; i++ {
		key := result[i] // 当前要插入的元素
		j := i - 1       // 已排序部分的最后一个元素索引

		// 将大于 key 的元素向后移动
		for j >= 0 && result[j] > key {
			result[j+1] = result[j]
			j--
		}
		// 插入 key 到正确位置
		result[j+1] = key
	}
	return result
}

// ---------- 快速排序（Quick Sort）----------
// 原理：选择一个基准元素，将数组分为两部分（小于基准和大于基准），递归排序
// 时间复杂度：平均 O(n log n)，最坏 O(n^2)
// 空间复杂度：O(log n)（递归栈）
// 稳定性：不稳定

// QuickSort 快速排序
func QuickSort(arr []int) []int {
	result := make([]int, len(arr))
	copy(result, arr)
	quickSortHelper(result, 0, len(result)-1)
	return result
}

// quickSortHelper 快速排序递归函数
func quickSortHelper(arr []int, low, high int) {
	if low < high {
		// 分区，获取基准元素的最终位置
		pivot := partition(arr, low, high)
		// 递归排序左半部分
		quickSortHelper(arr, low, pivot-1)
		// 递归排序右半部分
		quickSortHelper(arr, pivot+1, high)
	}
}

// partition 分区函数
// 选择最后一个元素作为基准，将数组分为两部分
func partition(arr []int, low, high int) int {
	pivot := arr[high] // 选择最后一个元素作为基准
	i := low - 1       // i 是小于基准的区域的边界

	for j := low; j < high; j++ {
		if arr[j] <= pivot {
			i++
			arr[i], arr[j] = arr[j], arr[i]
		}
	}
	// 将基准元素放到正确位置
	arr[i+1], arr[high] = arr[high], arr[i+1]
	return i + 1
}

// ---------- 归并排序（Merge Sort）----------
// 原理：分治法，将数组分成两半，分别排序后合并
// 时间复杂度：O(n log n)
// 空间复杂度：O(n)
// 稳定性：稳定

// MergeSort 归并排序
func MergeSort(arr []int) []int {
	n := len(arr)
	if n <= 1 {
		result := make([]int, n)
		copy(result, arr)
		return result
	}

	// 分成两半
	mid := n / 2
	left := MergeSort(arr[:mid])   // 递归排序左半部分
	right := MergeSort(arr[mid:])  // 递归排序右半部分

	// 合并两个有序数组
	return merge(left, right)
}

// merge 合并两个有序数组
func merge(left, right []int) []int {
	result := make([]int, 0, len(left)+len(right))
	i, j := 0, 0

	// 比较两个数组的元素，按顺序合并
	for i < len(left) && j < len(right) {
		if left[i] <= right[j] {
			result = append(result, left[i])
			i++
		} else {
			result = append(result, right[j])
			j++
		}
	}

	// 追加剩余元素
	result = append(result, left[i:]...)
	result = append(result, right[j:]...)

	return result
}

// demonstrateSorting 演示排序算法
func demonstrateSorting() {
	fmt.Println("\n--- 6. 排序算法 ---")

	original := []int{64, 34, 25, 12, 22, 11, 90, 45, 33, 77}
	fmt.Printf("  原始数组: %v\n", original)

	// 冒泡排序
	fmt.Printf("  冒泡排序: %v\n", BubbleSort(original))

	// 选择排序
	fmt.Printf("  选择排序: %v\n", SelectionSort(original))

	// 插入排序
	fmt.Printf("  插入排序: %v\n", InsertionSort(original))

	// 快速排序
	fmt.Printf("  快速排序: %v\n", QuickSort(original))

	// 归并排序
	fmt.Printf("  归并排序: %v\n", MergeSort(original))

	// 排序算法比较：
	// 算法        平均时间    最坏时间    空间    稳定性
	// 冒泡排序     O(n^2)     O(n^2)    O(1)     稳定
	// 选择排序     O(n^2)     O(n^2)    O(1)    不稳定
	// 插入排序     O(n^2)     O(n^2)    O(1)     稳定
	// 快速排序   O(n log n)   O(n^2)  O(log n)  不稳定
	// 归并排序   O(n log n)  O(n log n)  O(n)    稳定
	//
	// 选择建议：
	// - 小规模数据：插入排序（常数因子小）
	// - 通用场景：快速排序（平均性能最好）
	// - 需要稳定：归并排序
	// - 内存受限：堆排序（空间 O(1)）
}
