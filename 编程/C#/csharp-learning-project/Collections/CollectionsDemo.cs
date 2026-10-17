// ============================================================
// CollectionsDemo.cs - 集合与泛型演示
// 演示：泛型基础、常用集合类、LINQ、Lambda表达式
// ============================================================

namespace CSharpLearning.Collections;

/// <summary>
/// 集合与泛型演示类
/// </summary>
public class CollectionsDemo
{
    /// <summary>
    /// 运行所有集合与泛型演示
    /// </summary>
    public static void RunAllDemos()
    {
        Console.WriteLine("========== 集合与泛型演示 ==========");
        Console.WriteLine();

        DemoGenericsBasics();        // 泛型基础
        Console.WriteLine();

        DemoList();                  // List<T>
        Console.WriteLine();

        DemoDictionary();            // Dictionary<TKey, TValue>
        Console.WriteLine();

        DemoHashSet();               // HashSet<T>
        Console.WriteLine();

        DemoQueueStack();            // Queue<T> 和 Stack<T>
        Console.WriteLine();

        DemoLinkedList();            // LinkedList<T>
        Console.WriteLine();

        DemoArrayVsListPerformance();// 数组 vs List 性能
        Console.WriteLine();

        DemoLambda();                // Lambda 表达式
        Console.WriteLine();

        DemoLinqQuerySyntax();       // LINQ 查询语法
        Console.WriteLine();

        DemoLinqMethodSyntax();      // LINQ 方法语法
        Console.WriteLine();

        DemoLinqAdvanced();          // LINQ 进阶（分组、连接等）
        Console.WriteLine();

        Console.WriteLine("========== 集合与泛型演示结束 ==========");
    }

    // ============================================================
    // 1. 泛型基础
    // ============================================================
    
    /// <summary>
    /// 演示泛型基础
    /// 泛型：在定义方法、类、接口时不指定具体类型，使用时再指定
    /// 好处：类型安全、代码复用、避免装箱拆箱
    /// </summary>
    public static void DemoGenericsBasics()
    {
        Console.WriteLine("--- 泛型基础演示 ---");

        // ---- 泛型方法 ----
        Console.WriteLine("泛型方法：");
        
        // 调用泛型方法，可以显式指定类型参数
        int maxInt = GenericUtils.Max<int>(10, 20);
        Console.WriteLine($"  Max<int>(10, 20) = {maxInt}");

        // 也可以让编译器推断类型（类型推断）
        string maxStr = GenericUtils.Max("apple", "banana");
        Console.WriteLine($"  Max(\"apple\", \"banana\") = {maxStr}（类型推断）");

        // 泛型方法的类型推断让代码更简洁
        double maxDouble = GenericUtils.Max(3.14, 2.71);
        Console.WriteLine($"  Max(3.14, 2.71) = {maxDouble}");

        // 泛型交换方法
        int a = 5, b = 10;
        Console.WriteLine($"\n  交换前：a={a}, b={b}");
        GenericUtils.Swap(ref a, ref b);
        Console.WriteLine($"  交换后：a={a}, b={b}");

        string s1 = "hello", s2 = "world";
        Console.WriteLine($"\n  交换前：s1=\"{s1}\", s2=\"{s2}\"");
        GenericUtils.Swap(ref s1, ref s2);
        Console.WriteLine($"  交换后：s1=\"{s1}\", s2=\"{s2}\"");

        // ---- 泛型类 ----
        Console.WriteLine("\n泛型类：");
        
        // 整数类型的泛型栈
        GenericStack<int> intStack = new GenericStack<int>(5);
        intStack.Push(10);
        intStack.Push(20);
        intStack.Push(30);
        Console.WriteLine($"  int栈：弹出 {intStack.Pop()}，弹出 {intStack.Pop()}");
        Console.WriteLine($"  栈中元素个数：{intStack.Count}");

        // 字符串类型的泛型栈
        GenericStack<string> strStack = new GenericStack<string>(5);
        strStack.Push("C#");
        strStack.Push("Java");
        Console.WriteLine($"  string栈：弹出 \"{strStack.Pop()}\"");

        // ---- 泛型约束 ----
        Console.WriteLine("\n泛型约束：");
        
        // where T : class 约束 T 必须是引用类型
        // where T : struct 约束 T 必须是值类型
        // where T : new() 约束 T 必须有无参构造函数
        // where T : 基类名 约束 T 必须继承自指定基类
        // where T : 接口名 约束 T 必须实现指定接口

        // 创建一个需要无参构造函数约束的工厂
        var person = Factory.Create<SamplePerson>();
        Console.WriteLine($"  工厂创建：{person.Name}");
    }

    // ============================================================
    // 2. List<T>
    // ============================================================
    
    /// <summary>
    /// 演示 List&lt;T&gt; 泛型列表
    /// List 是最常用的集合，动态大小的数组
    /// 内部使用数组实现，自动扩容
    /// </summary>
    public static void DemoList()
    {
        Console.WriteLine("--- List<T> 演示 ---");

        // 创建 List
        List<string> fruits = new List<string>();

        // 添加元素
        fruits.Add("苹果");      // Add: 在末尾添加一个元素
        fruits.Add("香蕉");
        fruits.Add("橙子");
        Console.WriteLine($"添加后数量：{fruits.Count}");

        // 批量添加
        fruits.AddRange(new string[] { "葡萄", "西瓜" });  // AddRange: 添加一组元素
        Console.WriteLine($"AddRange后数量：{fruits.Count}");

        // 插入元素
        fruits.Insert(1, "草莓");  // Insert: 在指定位置插入
        Console.WriteLine($"Insert后第2个元素：{fruits[1]}");

        // 访问元素（通过索引，和数组一样）
        Console.WriteLine($"第1个元素：{fruits[0]}");
        Console.WriteLine($"最后1个元素：{fruits[^1]}");  // C# 8+ 索引运算符

        // 查找元素
        Console.WriteLine($"\n是否包含\"香蕉\"：{fruits.Contains("香蕉")}");
        Console.WriteLine($"\"橙子\"的索引：{fruits.IndexOf("橙子")}");
        Console.WriteLine($"第一个以\"草\"开头的：{fruits.Find(f => f.StartsWith("草"))}");

        // 查找所有符合条件的元素
        List<string> longNames = fruits.FindAll(f => f.Length > 2);
        Console.WriteLine($"名字长度>2的水果有：{string.Join(", ", longNames)}");

        // 删除元素
        fruits.Remove("香蕉");        // Remove: 移除指定元素
        fruits.RemoveAt(0);            // RemoveAt: 移除指定索引的元素
        Console.WriteLine($"\n删除后列表：{string.Join(", ", fruits)}");

        // 排序
        fruits.Sort();  // 默认排序
        Console.WriteLine($"排序后：{string.Join(", ", fruits)}");

        // 反转
        fruits.Reverse();
        Console.WriteLine($"反转后：{string.Join(", ", fruits)}");

        // 遍历方式
        Console.WriteLine("\n遍历方式1 - for循环：");
        for (int i = 0; i < fruits.Count; i++)
        {
            Console.Write($"  {fruits[i]}");
        }

        Console.WriteLine("\n遍历方式2 - foreach循环（最常用）：");
        foreach (string fruit in fruits)
        {
            Console.Write($"  {fruit}");
        }

        // 清空
        fruits.Clear();
        Console.WriteLine($"\n\n清空后数量：{fruits.Count}");
    }

    // ============================================================
    // 3. Dictionary<TKey, TValue>
    // ============================================================
    
    /// <summary>
    /// 演示 Dictionary&lt;TKey, TValue&gt; 字典
    /// 键值对集合，通过键快速查找值（哈希表实现）
    /// 键必须唯一，查找时间复杂度接近 O(1)
    /// </summary>
    public static void DemoDictionary()
    {
        Console.WriteLine("--- Dictionary<TKey, TValue> 演示 ---");

        // 创建字典
        Dictionary<string, int> scores = new Dictionary<string, int>();

        // 添加键值对
        scores.Add("张三", 95);   // Add: 添加键值对，如果键已存在会抛异常
        scores.Add("李四", 88);
        scores["王五"] = 76;      // 索引器方式：键不存在则添加，已存在则更新

        Console.WriteLine($"字典元素数量：{scores.Count}");

        // 访问值（通过键）
        Console.WriteLine($"张三的分数：{scores["张三"]}");

        // 安全地访问值（TryGetValue）
        // 如果直接用 scores["不存在的人"] 会抛异常，TryGetValue 更安全
        if (scores.TryGetValue("赵六", out int zhaoScore))
        {
            Console.WriteLine($"赵六的分数：{zhaoScore}");
        }
        else
        {
            Console.WriteLine("字典中没有赵六");
        }

        // 判断键/值是否存在
        Console.WriteLine($"\n是否包含键\"张三\"：{scores.ContainsKey("张三")}");
        Console.WriteLine($"是否包含值100：{scores.ContainsValue(100)}");

        // 遍历字典
        Console.WriteLine("\n遍历键值对：");
        foreach (KeyValuePair<string, int> kvp in scores)
        {
            Console.WriteLine($"  {kvp.Key}: {kvp.Key} 的分数是 {kvp.Value}");
        }

        // 只遍历键
        Console.WriteLine("\n所有键（学生姓名）：");
        foreach (string key in scores.Keys)
        {
            Console.Write($"  {key}");
        }

        // 只遍历值
        Console.WriteLine("\n所有值（分数）：");
        foreach (int value in scores.Values)
        {
            Console.Write($"  {value}");
        }

        // 删除元素
        scores.Remove("李四");  // Remove: 按键删除
        Console.WriteLine($"\n\n删除李四后，字典数量：{scores.Count}");

        // ---- 复杂字典示例 ----
        Console.WriteLine("\n复杂字典示例（学生成绩册）：");
        
        // 字典的值也可以是集合类型
        Dictionary<string, List<int>> studentScores = new Dictionary<string, List<int>>();
        
        studentScores["小明"] = new List<int> { 90, 85, 92 };
        studentScores["小红"] = new List<int> { 88, 91, 87 };
        studentScores["小刚"] = new List<int> { 76, 80, 85 };

        foreach (var student in studentScores)
        {
            double avg = student.Value.Average();
            Console.WriteLine($"  {student.Key}：平均分 {avg:F1}，各科 {string.Join(", ", student.Value)}");
        }
    }

    // ============================================================
    // 4. HashSet<T>
    // ============================================================
    
    /// <summary>
    /// 演示 HashSet&lt;T&gt; 哈希集
    /// 不包含重复元素的集合，基于哈希表实现
    /// 主要用于：去重、集合运算（交集、并集、差集）
    /// 添加、删除、查找的时间复杂度都是 O(1)
    /// </summary>
    public static void DemoHashSet()
    {
        Console.WriteLine("--- HashSet<T> 演示 ---");

        // 创建 HashSet
        HashSet<int> set1 = new HashSet<int>();

        // 添加元素
        set1.Add(1);
        set1.Add(2);
        set1.Add(3);
        set1.Add(2);  // 重复元素不会被添加（不会报错，只是被忽略）
        set1.Add(4);
        set1.Add(5);

        Console.WriteLine($"set1 元素（共{set1.Count}个）：{string.Join(", ", set1)}");
        // 注意：HashSet 不保证元素顺序

        // 查找元素（非常快，O(1)）
        Console.WriteLine($"\n是否包含3：{set1.Contains(3)}");
        Console.WriteLine($"是否包含10：{set1.Contains(10)}");

        // 第二个集合
        HashSet<int> set2 = new HashSet<int> { 4, 5, 6, 7, 8 };
        Console.WriteLine($"set2 元素：{string.Join(", ", set2)}");

        // ---- 集合运算 ----
        Console.WriteLine("\n集合运算：");

        // 并集（Union）：两个集合中所有不重复的元素
        HashSet<int> union = new HashSet<int>(set1);
        union.UnionWith(set2);
        Console.WriteLine($"  并集：{string.Join(", ", union)}");

        // 交集（Intersect）：两个集合中都有的元素
        HashSet<int> intersect = new HashSet<int>(set1);
        intersect.IntersectWith(set2);
        Console.WriteLine($"  交集：{string.Join(", ", intersect)}");

        // 差集（Except）：在set1中但不在set2中的元素
        HashSet<int> except = new HashSet<int>(set1);
        except.ExceptWith(set2);
        Console.WriteLine($"  差集(set1 - set2)：{string.Join(", ", except)}");

        // 对称差集：只在其中一个集合中出现的元素（不同时出现）
        HashSet<int> symmetricExcept = new HashSet<int>(set1);
        symmetricExcept.SymmetricExceptWith(set2);
        Console.WriteLine($"  对称差集：{string.Join(", ", symmetricExcept)}");

        // ---- 去重应用 ----
        Console.WriteLine("\n去重应用：");
        List<int> numbersWithDup = new List<int> { 1, 2, 2, 3, 3, 3, 4, 5, 5 };
        HashSet<int> uniqueNumbers = new HashSet<int>(numbersWithDup);
        Console.WriteLine($"  原列表（{numbersWithDup.Count}个）：{string.Join(", ", numbersWithDup)}");
        Console.WriteLine($"  去重后（{uniqueNumbers.Count}个）：{string.Join(", ", uniqueNumbers)}");
    }

    // ============================================================
    // 5. Queue<T> 和 Stack<T>
    // ============================================================
    
    /// <summary>
    /// 演示 Queue&lt;T&gt; 队列和 Stack&lt;T&gt; 栈
    /// Queue: 先进先出（FIFO）
    /// Stack: 后进先出（LIFO）
    /// </summary>
    public static void DemoQueueStack()
    {
        Console.WriteLine("--- Queue<T> 和 Stack<T> 演示 ---");

        // ---- Queue<T> 队列 ----
        Console.WriteLine("Queue<T> 队列（先进先出 FIFO）：");
        Queue<string> queue = new Queue<string>();

        // 入队（Enqueue）：在队列末尾添加元素
        queue.Enqueue("第一个人");
        queue.Enqueue("第二个人");
        queue.Enqueue("第三个人");
        Console.WriteLine($"  队列元素数量：{queue.Count}");
        Console.WriteLine($"  队列元素：{string.Join(", ", queue)}");

        // 查看队首元素（Peek）：不删除
        Console.WriteLine($"  队首元素（Peek）：{queue.Peek()}");

        // 出队（Dequeue）：移除并返回队首元素
        Console.WriteLine($"  出队：{queue.Dequeue()}");
        Console.WriteLine($"  出队：{queue.Dequeue()}");
        Console.WriteLine($"  出队后剩余：{string.Join(", ", queue)}");

        // ---- Stack<T> 栈 ----
        Console.WriteLine("\nStack<T> 栈（后进先出 LIFO）：");
        Stack<string> stack = new Stack<string>();

        // 入栈（Push）：在栈顶添加元素
        stack.Push("第一个盘子");
        stack.Push("第二个盘子");
        stack.Push("第三个盘子");
        Console.WriteLine($"  栈元素数量：{stack.Count}");
        Console.WriteLine($"  栈元素（从顶到底）：{string.Join(", ", stack)}");

        // 查看栈顶元素（Peek）：不删除
        Console.WriteLine($"  栈顶元素（Peek）：{stack.Peek()}");

        // 出栈（Pop）：移除并返回栈顶元素
        Console.WriteLine($"  出栈：{stack.Pop()}");
        Console.WriteLine($"  出栈：{stack.Pop()}");
        Console.WriteLine($"  出栈后剩余：{string.Join(", ", stack)}");

        // ---- 实际应用 ----
        Console.WriteLine("\n典型应用：");
        Console.WriteLine("  Queue: 消息队列、任务调度、打印队列、BFS广度优先搜索");
        Console.WriteLine("  Stack: 函数调用栈、撤销操作、表达式求值、DFS深度优先搜索");
    }

    // ============================================================
    // 6. LinkedList<T>
    // ============================================================
    
    /// <summary>
    /// 演示 LinkedList&lt;T&gt; 双向链表
    /// 每个元素包含指向前一个和后一个元素的引用
    /// 优点：插入和删除快（O(1)，如果知道节点位置）
    /// 缺点：随机访问慢（O(n)），占用内存略多
    /// </summary>
    public static void DemoLinkedList()
    {
        Console.WriteLine("--- LinkedList<T> 演示 ---");

        // 创建链表
        LinkedList<string> linkedList = new LinkedList<string>();

        // 添加元素
        linkedList.AddLast("A");  // AddLast: 添加到末尾
        linkedList.AddLast("B");
        linkedList.AddLast("C");
        linkedList.AddFirst("Z"); // AddFirst: 添加到开头

        Console.WriteLine($"链表元素：{string.Join(" <-> ", linkedList)}");

        // 访问首尾节点
        Console.WriteLine($"第一个节点：{linkedList.First?.Value}");
        Console.WriteLine($"最后一个节点：{linkedList.Last?.Value}");

        // 查找节点（返回 LinkedListNode）
        LinkedListNode<string>? nodeB = linkedList.Find("B");
        if (nodeB != null)
        {
            Console.WriteLine($"\nB节点的前一个：{nodeB.Previous?.Value}");
            Console.WriteLine($"B节点的后一个：{nodeB.Next?.Value}");

            // 在B之前插入
            linkedList.AddBefore(nodeB, "X");
            Console.WriteLine($"在B前插入X后：{string.Join(" <-> ", linkedList)}");

            // 在B之后插入
            linkedList.AddAfter(nodeB, "Y");
            Console.WriteLine($"在B后插入Y后：{string.Join(" <-> ", linkedList)}");
        }

        // 删除节点
        linkedList.Remove("Z");  // 按值删除
        Console.WriteLine($"\n删除Z后：{string.Join(" <-> ", linkedList)}");

        linkedList.RemoveFirst();  // 删除第一个
        Console.WriteLine($"删除第一个后：{string.Join(" <-> ", linkedList)}");

        linkedList.RemoveLast();   // 删除最后一个
        Console.WriteLine($"删除最后一个后：{string.Join(" <-> ", linkedList)}");

        // 遍历链表
        Console.WriteLine("\n从前往后遍历：");
        var current = linkedList.First;
        while (current != null)
        {
            Console.Write($"  {current.Value}");
            current = current.Next;
        }

        Console.WriteLine("\n从后往前遍历：");
        current = linkedList.Last;
        while (current != null)
        {
            Console.Write($"  {current.Value}");
            current = current.Previous;
        }
        Console.WriteLine();
    }

    // ============================================================
    // 7. 数组 vs List 性能
    // ============================================================
    
    /// <summary>
    /// 演示数组和 List 的性能对比
    /// 数组：固定大小，访问快，但是不能动态扩容
    /// List：动态大小，基于数组实现，访问接近数组速度，但有扩容开销
    /// </summary>
    public static void DemoArrayVsListPerformance()
    {
        Console.WriteLine("--- 数组 vs List 性能对比 ---");

        int size = 1000000;  // 100万条数据
        Console.WriteLine($"测试数据量：{size:N0} 个整数");

        // ---- 数组：创建和填充 ----
        var sw1 = System.Diagnostics.Stopwatch.StartNew();
        int[] arr = new int[size];
        for (int i = 0; i < size; i++)
        {
            arr[i] = i;
        }
        sw1.Stop();
        Console.WriteLine($"\n数组：创建+填充 耗时 {sw1.ElapsedMilliseconds} ms");

        // ---- List：创建和填充 ----
        var sw2 = System.Diagnostics.Stopwatch.StartNew();
        List<int> list = new List<int>(size);  // 指定初始容量可以减少扩容开销
        for (int i = 0; i < size; i++)
        {
            list.Add(i);
        }
        sw2.Stop();
        Console.WriteLine($"List（指定初始容量）：创建+填充 耗时 {sw2.ElapsedMilliseconds} ms");

        // ---- List 不指定初始容量 ----
        var sw3 = System.Diagnostics.Stopwatch.StartNew();
        List<int> list2 = new List<int>();  // 不指定初始容量，会多次扩容
        for (int i = 0; i < size; i++)
        {
            list2.Add(i);
        }
        sw3.Stop();
        Console.WriteLine($"List（不指定初始容量）：创建+填充 耗时 {sw3.ElapsedMilliseconds} ms");

        // ---- 读取性能 ----
        sw1.Restart();
        long sum1 = 0;
        for (int i = 0; i < size; i++)
        {
            sum1 += arr[i];
        }
        sw1.Stop();
        Console.WriteLine($"\n数组：遍历求和 耗时 {sw1.ElapsedMilliseconds} ms，和={sum1:N0}");

        sw2.Restart();
        long sum2 = 0;
        for (int i = 0; i < size; i++)
        {
            sum2 += list[i];
        }
        sw2.Stop();
        Console.WriteLine($"List：遍历求和 耗时 {sw2.ElapsedMilliseconds} ms，和={sum2:N0}");

        Console.WriteLine("\n结论：");
        Console.WriteLine("  - 数组访问略快，但大小固定");
        Console.WriteLine("  - List使用更方便（动态大小、丰富方法），性能接近数组");
        Console.WriteLine("  - 如果知道元素数量，给List指定初始容量可以避免扩容开销");
        Console.WriteLine("  - 大多数场景下推荐使用 List<T>");
    }

    // ============================================================
    // 8. Lambda 表达式
    // ============================================================
    
    /// <summary>
    /// 演示 Lambda 表达式
    /// Lambda 是一种匿名函数，可以用简短的语法创建委托
    /// 语法：(参数列表) => 表达式或语句块
    /// 常用于 LINQ、事件处理、异步编程等
    /// </summary>
    public static void DemoLambda()
    {
        Console.WriteLine("--- Lambda 表达式演示 ---");

        // ---- Lambda 基本语法 ----
        Console.WriteLine("Lambda 基本语法：");

        // 无参数
        Action greet = () => Console.WriteLine("  Hello Lambda!");
        greet();

        // 一个参数（可以省略括号）
        Action<string> print = msg => Console.WriteLine($"  {msg}");
        print("一个参数的Lambda");

        // 多个参数
        Func<int, int, int> add = (a, b) => a + b;  // 表达式体
        Console.WriteLine($"  add(3, 5) = {add(3, 5)}");

        // 语句体（用大括号）
        Func<int, int, int> multiply = (a, b) =>
        {
            int result = a * b;
            return result;
        };
        Console.WriteLine($"  multiply(3, 5) = {multiply(3, 5)}");

        // ---- 常用委托类型 ----
        Console.WriteLine("\n常用内置委托类型：");

        // Func<TResult> / Func<T, TResult> 等：有返回值的委托
        Func<int, string> intToStr = n => n.ToString();
        Console.WriteLine($"  Func: {intToStr(123)}");

        // Action / Action<T> 等：无返回值的委托
        Action<int> printNumber = n => Console.WriteLine($"  Action: {n}");
        printNumber(456);

        // Predicate<T>：返回 bool 的委托（判断条件）
        Predicate<int> isEven = n => n % 2 == 0;
        Console.WriteLine($"  Predicate: 4是偶数吗？{isEven(4)}");

        // ---- Lambda 与集合方法结合 ----
        Console.WriteLine("\nLambda 与集合方法结合：");

        List<int> numbers = new List<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        // FindAll：查找所有符合条件的元素
        var evens = numbers.FindAll(n => n % 2 == 0);
        Console.WriteLine($"  偶数：{string.Join(", ", evens)}");

        // ForEach：对每个元素执行操作
        Console.Write("  每个数乘2：");
        numbers.ForEach(n => Console.Write($"{n * 2} "));
        Console.WriteLine();

        // Exists：是否存在符合条件的元素
        bool hasGreaterThan10 = numbers.Exists(n => n > 10);
        Console.WriteLine($"  存在大于10的数吗？{hasGreaterThan10}");

        // ---- 闭包（Closure）----
        Console.WriteLine("\n闭包演示：");
        int multiplier = 3;  // 外部变量
        Func<int, int> multiplierFunc = x => x * multiplier;  // Lambda 捕获了外部变量
        Console.WriteLine($"  multiplier=3, multiplierFunc(5) = {multiplierFunc(5)}");

        multiplier = 5;  // 修改外部变量
        Console.WriteLine($"  multiplier=5, multiplierFunc(5) = {multiplierFunc(5)}");
        Console.WriteLine("  （闭包捕获的是变量本身，不是变量的当前值）");
    }

    // ============================================================
    // 9. LINQ 查询语法
    // ============================================================
    
    /// <summary>
    /// 演示 LINQ 查询语法（Query Syntax）
    /// 类似 SQL 的语法风格，from/where/select/orderby/group/join
    /// 查询语法在编译时会转换为方法语法
    /// </summary>
    public static void DemoLinqQuerySyntax()
    {
        Console.WriteLine("--- LINQ 查询语法演示 ---");

        // 示例数据：学生列表
        List<Student> students = GetSampleStudents();
        Console.WriteLine($"共有 {students.Count} 名学生");

        // ---- 基本查询：from + where + select ----
        // from: 指定数据源和范围变量
        // where: 过滤条件
        // select: 选择结果
        Console.WriteLine("\n1. 查询年龄大于20岁的学生：");
        var olderStudents = from s in students
                            where s.Age > 20
                            select s;

        foreach (var s in olderStudents)
        {
            Console.WriteLine($"  {s.Name} - {s.Age}岁 - {s.Major}");
        }

        // ---- 排序：orderby ----
        Console.WriteLine("\n2. 按分数降序排列的学生：");
        var sortedByScore = from s in students
                            orderby s.Score descending
                            select new { s.Name, s.Score };  // 匿名类型

        foreach (var s in sortedByScore)
        {
            Console.WriteLine($"  {s.Name}: {s.Score}分");
        }

        // ---- 分组：group by ----
        Console.WriteLine("\n3. 按专业分组的学生：");
        var studentsByMajor = from s in students
                              group s by s.Major into g
                              orderby g.Key
                              select new { Major = g.Key, Count = g.Count(), Students = g };

        foreach (var group in studentsByMajor)
        {
            Console.WriteLine($"  【{group.Major}】共{group.Count}人：");
            foreach (var s in group.Students)
            {
                Console.WriteLine($"    - {s.Name}（{s.Score}分）");
            }
        }

        // ---- 多条件查询 ----
        Console.WriteLine("\n4. 查询计算机专业且分数大于80的学生：");
        var csHighScore = from s in students
                          where s.Major == "计算机" && s.Score > 80
                          orderby s.Score descending
                          select s;

        foreach (var s in csHighScore)
        {
            Console.WriteLine($"  {s.Name}: {s.Score}分");
        }

        // ---- 量词操作 ----
        Console.WriteLine("\n5. 量词操作：");
        bool allPass = (from s in students select s).All(s => s.Score >= 60);
        bool anyFail = (from s in students select s).Any(s => s.Score < 60);
        int totalScore = (from s in students select s.Score).Sum();
        double avgScore = (from s in students select s.Score).Average();

        Console.WriteLine($"  所有学生都及格了吗？{allPass}");
        Console.WriteLine($"  有不及格的学生吗？{anyFail}");
        Console.WriteLine($"  总分数：{totalScore}");
        Console.WriteLine($"  平均分数：{avgScore:F2}");
    }

    // ============================================================
    // 10. LINQ 方法语法
    // ============================================================
    
    /// <summary>
    /// 演示 LINQ 方法语法（Method Syntax）
    /// 使用扩展方法和 Lambda 表达式
    /// 方法语法比查询语法更灵活，功能更完整
    /// </summary>
    public static void DemoLinqMethodSyntax()
    {
        Console.WriteLine("--- LINQ 方法语法演示 ---");

        List<Student> students = GetSampleStudents();

        // ---- Where：过滤 ----
        Console.WriteLine("\n1. Where - 过滤：");
        var mathStudents = students.Where(s => s.Major == "数学");
        foreach (var s in mathStudents)
        {
            Console.WriteLine($"  {s.Name}");
        }

        // ---- Select：投影（转换）----
        Console.WriteLine("\n2. Select - 投影：");
        var names = students.Select(s => s.Name);  // 只选择名字
        Console.WriteLine($"  所有姓名：{string.Join(", ", names)}");

        var nameAndAge = students.Select(s => new { 姓名 = s.Name, 年龄 = s.Age });
        foreach (var item in nameAndAge.Take(3))  // Take: 取前N个
        {
            Console.WriteLine($"  {item.姓名}: {item.年龄}岁");
        }

        // ---- OrderBy/OrderByDescending：排序 ----
        Console.WriteLine("\n3. OrderBy - 排序：");
        var sortedByAge = students.OrderBy(s => s.Age);
        foreach (var s in sortedByAge)
        {
            Console.WriteLine($"  {s.Name}: {s.Age}岁");
        }

        // ---- First/FirstOrDefault/Single/SingleOrDefault ----
        Console.WriteLine("\n4. 取单个元素：");
        var first = students.First();  // 第一个元素
        Console.WriteLine($"  First(): {first.Name}");

        var firstMath = students.FirstOrDefault(s => s.Major == "数学");
        Console.WriteLine($"  FirstOrDefault(数学): {firstMath?.Name ?? "没有"}");

        // ---- Skip/Take：分页 ----
        Console.WriteLine("\n5. Skip/Take - 分页：");
        int pageSize = 3;
        int pageIndex = 1;  // 第2页（从0开始）
        var page = students.Skip(pageIndex * pageSize).Take(pageSize);
        Console.WriteLine($"  第{pageIndex + 1}页（每页{pageSize}个）：");
        foreach (var s in page)
        {
            Console.WriteLine($"    {s.Name}");
        }

        // ---- Distinct：去重 ----
        Console.WriteLine("\n6. Distinct - 去重：");
        var majors = students.Select(s => s.Major).Distinct();
        Console.WriteLine($"  所有专业：{string.Join(", ", majors)}");

        // ---- 链式调用：方法语法的优势 ----
        // 可以链式调用多个方法，流畅的语法
        Console.WriteLine("\n7. 链式调用（复杂查询）：");
        var result = students
            .Where(s => s.Score >= 80)                    // 筛选80分以上
            .OrderByDescending(s => s.Score)              // 按分数降序
            .ThenBy(s => s.Name)                          // 分数相同按姓名升序
            .Select(s => new { s.Name, s.Major, s.Score }) // 投影
            .Take(5);                                     // 取前5个

        foreach (var s in result)
        {
            Console.WriteLine($"  {s.Name}（{s.Major}）: {s.Score}分");
        }

        // ---- 聚合方法 ----
        Console.WriteLine("\n8. 聚合方法：");
        Console.WriteLine($"  Count: {students.Count}");
        Console.WriteLine($"  Count(分数>80): {students.Count(s => s.Score > 80)}");
        Console.WriteLine($"  Sum(分数): {students.Sum(s => s.Score)}");
        Console.WriteLine($"  Average(分数): {students.Average(s => s.Score):F2}");
        Console.WriteLine($"  Max(分数): {students.Max(s => s.Score)}");
        Console.WriteLine($"  Min(分数): {students.Min(s => s.Score)}");
    }

    // ============================================================
    // 11. LINQ 进阶
    // ============================================================
    
    /// <summary>
    /// 演示 LINQ 进阶用法：分组、连接、集合操作等
    /// </summary>
    public static void DemoLinqAdvanced()
    {
        Console.WriteLine("--- LINQ 进阶演示 ---");

        List<Student> students = GetSampleStudents();

        // ---- GroupBy：分组 ----
        Console.WriteLine("\n1. GroupBy 分组：");
        var groupByMajor = students.GroupBy(s => s.Major);
        foreach (var group in groupByMajor)
        {
            Console.WriteLine($"  【{group.Key}】{group.Count()}人");
            Console.WriteLine($"    最高分：{group.Max(s => s.Score)}");
            Console.WriteLine($"    平均分：{group.Average(s => s.Score):F2}");
        }

        // ---- ToLookup：即时查找（类似分组，但结果可直接索引）----
        Console.WriteLine("\n2. ToLookup 查找：");
        var lookupByMajor = students.ToLookup(s => s.Major);
        Console.WriteLine($"  计算机专业人数：{lookupByMajor["计算机"].Count()}");
        Console.WriteLine($"  数学专业第一个学生：{lookupByMajor["数学"].First().Name}");

        // ---- 两个集合的连接（Join）----
        Console.WriteLine("\n3. Join 内连接：");

        // 课程列表
        List<Course> courses = new List<Course>
        {
            new Course { CourseId = 1, CourseName = "高等数学", Major = "数学" },
            new Course { CourseId = 2, CourseName = "数据结构", Major = "计算机" },
            new Course { CourseId = 3, CourseName = "英语精读", Major = "英语" },
            new Course { CourseId = 4, CourseName = "英语写作", Major = "英语" }
        };

        // 按专业连接学生和课程
        var studentCourse = students.Join(
            courses,             // 第二个集合
            s => s.Major,        // 第一个集合的连接键
            c => c.Major,        // 第二个集合的连接键
            (s, c) => new        // 结果选择器
            {
                StudentName = s.Name,
                s.Major,
                CourseName = c.CourseName
            });

        foreach (var item in studentCourse)
        {
            Console.WriteLine($"  {item.StudentName}（{item.Major}）- {item.CourseName}");
        }

        // ---- GroupJoin：分组连接（左外连接）----
        Console.WriteLine("\n4. GroupJoin 分组连接：");
        var majorWithCourses = courses
            .GroupBy(c => c.Major)
            .Select(g => new { Major = g.Key, Courses = g.ToList() });

        foreach (var mc in majorWithCourses)
        {
            Console.WriteLine($"  【{mc.Major}】课程：{string.Join(", ", mc.Courses.Select(c => c.CourseName))}");
        }

        // ---- 集合操作 ----
        Console.WriteLine("\n5. 集合操作：");
        int[] arr1 = { 1, 2, 3, 4, 5 };
        int[] arr2 = { 4, 5, 6, 7, 8 };

        Console.WriteLine($"  数组1：{string.Join(", ", arr1)}");
        Console.WriteLine($"  数组2：{string.Join(", ", arr2)}");
        Console.WriteLine($"  并集：{string.Join(", ", arr1.Union(arr2))}");
        Console.WriteLine($"  交集：{string.Join(", ", arr1.Intersect(arr2))}");
        Console.WriteLine($"  差集(arr1-arr2)：{string.Join(", ", arr1.Except(arr2))}");

        // ---- Zip：将两个序列按位置配对 ----
        Console.WriteLine("\n6. Zip 配对：");
        string[] names = { "张三", "李四", "王五" };
        int[] scores = { 90, 85, 95 };
        var zipped = names.Zip(scores, (name, score) => $"{name}: {score}分");
        foreach (var item in zipped)
        {
            Console.WriteLine($"  {item}");
        }

        // ---- 延迟执行 vs 立即执行 ----
        Console.WriteLine("\n7. 延迟执行（Lazy Evaluation）：");
        Console.WriteLine("  LINQ查询在遍历结果时才真正执行（延迟执行）");
        Console.WriteLine("  ToList/ToArray/Count/Sum等会立即执行");
        Console.WriteLine("  延迟执行的好处：可以链式组合、节省内存");
    }

    // ============================================================
    // 辅助方法与类
    // ============================================================

    /// <summary>
    /// 获取示例学生数据
    /// </summary>
    private static List<Student> GetSampleStudents()
    {
        return new List<Student>
        {
            new Student { Name = "张三", Age = 19, Major = "计算机", Score = 92 },
            new Student { Name = "李四", Age = 21, Major = "数学", Score = 85 },
            new Student { Name = "王五", Age = 20, Major = "计算机", Score = 78 },
            new Student { Name = "赵六", Age = 22, Major = "英语", Score = 90 },
            new Student { Name = "钱七", Age = 18, Major = "数学", Score = 65 },
            new Student { Name = "孙八", Age = 20, Major = "计算机", Score = 88 },
            new Student { Name = "周九", Age = 21, Major = "英语", Score = 72 },
            new Student { Name = "吴十", Age = 19, Major = "数学", Score = 95 }
        };
    }
}

// ============================================================
// 泛型工具类
// ============================================================

/// <summary>
/// 泛型工具类
/// 包含各种泛型方法的演示
/// </summary>
public static class GenericUtils
{
    /// <summary>
    /// 泛型方法：返回两个值中较大的那个
    /// where T : IComparable<T> 是泛型约束
    /// 它要求类型 T 必须实现 IComparable<T> 接口（才能比较大小）
    /// </summary>
    /// <typeparam name="T">类型参数，可以是任何实现了IComparable的类型</typeparam>
    /// <param name="a">第一个值</param>
    /// <param name="b">第二个值</param>
    /// <returns>较大的值</returns>
    public static T Max<T>(T a, T b) where T : IComparable<T>
    {
        // CompareTo 返回大于0表示 a > b，等于0表示相等，小于0表示 a < b
        return a.CompareTo(b) > 0 ? a : b;
    }

    /// <summary>
    /// 泛型方法：交换两个变量的值
    /// 使用 ref 参数实现交换
    /// 这个方法没有泛型约束，适用于任何类型
    /// </summary>
    public static void Swap<T>(ref T a, ref T b)
    {
        T temp = a;  // 用泛型类型T声明临时变量
        a = b;
        b = temp;
    }
}

/// <summary>
/// 泛型栈类
/// 演示泛型类的定义
/// </summary>
/// <typeparam name="T">栈中元素的类型</typeparam>
public class GenericStack<T>
{
    private T[] _items;      // 存储元素的数组
    private int _count;      // 当前元素数量

    /// <summary>
    /// 当前元素个数
    /// </summary>
    public int Count => _count;

    /// <summary>
    /// 构造函数
    /// </summary>
    /// <param name="capacity">初始容量</param>
    public GenericStack(int capacity)
    {
        _items = new T[capacity];  // 创建泛型数组
        _count = 0;
    }

    /// <summary>
    /// 入栈
    /// </summary>
    public void Push(T item)
    {
        if (_count >= _items.Length)
        {
            // 扩容（这里简化处理，实际List<T>的扩容策略更复杂）
            Array.Resize(ref _items, _items.Length * 2);
        }
        _items[_count++] = item;
    }

    /// <summary>
    /// 出栈
    /// </summary>
    public T Pop()
    {
        if (_count == 0)
            throw new InvalidOperationException("栈为空");

        T item = _items[--_count];
        _items[_count] = default(T)!;  // 清空引用（对于引用类型帮助GC）
        return item;
    }

    /// <summary>
    /// 查看栈顶元素
    /// </summary>
    public T Peek()
    {
        if (_count == 0)
            throw new InvalidOperationException("栈为空");
        return _items[_count - 1];
    }
}

/// <summary>
/// 工厂类 - 演示 new() 约束
/// </summary>
public static class Factory
{
    /// <summary>
    /// 创建对象的泛型方法
    /// where T : new() 约束：T 必须有一个无参数的构造函数
    /// 这样才能在方法内部使用 new T() 创建对象
    /// </summary>
    public static T Create<T>() where T : new()
    {
        return new T();
    }
}

/// <summary>
/// 示例人类 - 用于泛型约束演示
/// </summary>
public class SamplePerson
{
    public string Name { get; set; } = "默认姓名";
}

// ============================================================
// LINQ 演示用的数据类
// ============================================================

/// <summary>
/// 学生类（LINQ演示用）
/// </summary>
public class Student
{
    public string Name { get; set; } = string.Empty;
    public int Age { get; set; }
    public string Major { get; set; } = string.Empty;
    public int Score { get; set; }
}

/// <summary>
/// 课程类（LINQ连接演示用）
/// </summary>
public class Course
{
    public int CourseId { get; set; }
    public string CourseName { get; set; } = string.Empty;
    public string Major { get; set; } = string.Empty;
}
