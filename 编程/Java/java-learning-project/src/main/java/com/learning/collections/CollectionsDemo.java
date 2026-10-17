/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.collections;

/**
 * ============================================================
 * 导入需要的类
 * java.util包包含了Java集合框架的大部分类
 * ============================================================
 */
import java.util.*;
import java.util.stream.Collectors;

/**
 * ============================================================
 * CollectionsDemo 类：Java集合框架演示
 *
 * 【Java集合框架（Java Collections Framework, JCF）】
 * 是Java提供的一套用于存储和操作数据的类库，主要由接口和实现类组成
 *
 * 【集合 vs 数组】
 * 数组：长度固定，只能存相同类型，可以存基本类型和引用类型
 * 集合：长度可变，可以存不同类型（一般用泛型限制），只能存引用类型
 *
 * 【集合框架的两大接口体系】
 * 1. Collection：单列集合，存储单个元素
 *    - List：有序、可重复（ArrayList, LinkedList, Vector）
 *    - Set：无序、不可重复（HashSet, LinkedHashSet, TreeSet）
 *    - Queue：队列（LinkedList, PriorityQueue等）
 * 2. Map：双列集合，存储键值对（key-value）
 *    - HashMap, LinkedHashMap, TreeMap, Hashtable
 * ============================================================
 */
public class CollectionsDemo {

    /**
     * main方法：程序入口
     */
    public static void main(String[] args) {
        System.out.println("========== Java集合框架演示 ==========");

        demonstrateList();            // 演示List集合
        demonstrateSet();             // 演示Set集合
        demonstrateMap();             // 演示Map集合
        demonstrateQueue();           // 演示Queue队列
        demonstrateIterator();        // 演示迭代器
        demonstrateCollections();     // 演示Collections工具类
        demonstrateGeneric();         // 演示泛型
        demonstrateStream();          // 演示Stream API

        System.out.println("========== 集合框架演示结束 ==========\n");
    }

    /**
     * ============================================================
     * 演示List集合
     *
     * 【List接口特点】
     * - 有序：元素有添加顺序，存储顺序和取出顺序一致
     * - 可重复：可以存储重复的元素
     * - 有索引：每个元素有下标，可以通过下标访问元素
     *
     * 【List的三个主要实现类对比】
     *              ArrayList       LinkedList       Vector
     * 底层结构      数组            双向链表          数组
     * 随机访问      快（O(1)）      慢（O(n)）        快（O(1)）
     * 增删（头/中） 慢（需要移动）   快（O(1)）        慢
     * 增删（尾部）   快（扩容除外）   快                快
     * 线程安全      否              否                是（synchronized）
     * 效率          高              较高              低
     * 初始容量      10（懒加载）    -                 10
     * 扩容倍数      1.5倍           -                 2倍
     *
     * 【使用场景】
     * - ArrayList：查询多、增删少（大部分场景）
     * - LinkedList：频繁在头尾增删，需要栈/队列特性
     * - Vector：需要线程安全（不推荐，推荐用Collections.synchronizedList）
     * ============================================================
     */
    public static void demonstrateList() {
        System.out.println("\n--- 1. List 集合 ---");

        // --- ArrayList 演示 ---
        System.out.println("【ArrayList（数组实现，查询快，增删慢）】");
        // 创建ArrayList：默认初始容量10（Java 8+是懒加载，第一次add才初始化）
        List<String> arrayList = new ArrayList<>();

        // add：添加元素
        arrayList.add("Java");       // 尾部添加
        arrayList.add("Python");
        arrayList.add("C++");
        arrayList.add(1, "Go");      // 指定位置插入

        System.out.println("ArrayList内容: " + arrayList);
        System.out.println("ArrayList大小: " + arrayList.size());

        // get：通过下标获取元素（ArrayList的优势，O(1)）
        System.out.println("第2个元素: " + arrayList.get(1));

        // set：修改指定位置的元素
        arrayList.set(0, "Java 17");
        System.out.println("修改后: " + arrayList);

        // remove：删除元素
        arrayList.remove(2);         // 按下标删除
        arrayList.remove("C++");     // 按元素删除（删除第一个匹配的）
        System.out.println("删除后: " + arrayList);

        // contains：判断是否包含
        System.out.println("是否包含Java: " + arrayList.contains("Java 17"));

        // indexOf：查找元素位置，找不到返回-1
        System.out.println("Go的位置: " + arrayList.indexOf("Go"));

        // --- LinkedList 演示 ---
        System.out.println("\n【LinkedList（链表实现，增删快）】");
        LinkedList<String> linkedList = new LinkedList<>();

        // LinkedList特有的方法（因为实现了Deque接口）
        linkedList.addFirst("A");    // 头部添加
        linkedList.addLast("Z");     // 尾部添加
        linkedList.addFirst("B");
        System.out.println("LinkedList内容: " + linkedList);
        System.out.println("第一个元素: " + linkedList.getFirst());
        System.out.println("最后一个元素: " + linkedList.getLast());

        // 可以当作栈（Stack）使用
        linkedList.push("Top");      // 入栈（相当于addFirst）
        System.out.println("push后: " + linkedList);
        String top = linkedList.pop(); // 出栈（相当于removeFirst）
        System.out.println("pop出栈: " + top + "，弹出后: " + linkedList);

        // 可以当作队列（Queue）使用
        linkedList.offer("End");     // 入队（尾部添加）
        System.out.println("offer后: " + linkedList);
        String first = linkedList.poll(); // 出队（头部移除）
        System.out.println("poll出队: " + first + "，出队后: " + linkedList);

        // --- Vector 演示（了解即可，不推荐使用） ---
        System.out.println("\n【Vector（线程安全，效率低，已过时）】");
        Vector<String> vector = new Vector<>();
        vector.add("元素1");
        vector.add("元素2");
        System.out.println("Vector内容: " + vector);
        System.out.println("Vector的方法都加了synchronized，是线程安全的");
        System.out.println("但效率低，推荐用Collections.synchronizedList代替");
    }

    /**
     * ============================================================
     * 演示Set集合
     *
     * 【Set接口特点】
     * - 无序：元素没有添加顺序（HashSet），存储顺序和取出顺序可能不一致
     * - 不可重复：不能存储重复的元素（通过equals和hashCode判断）
     * - 没有索引：没有下标，不能通过下标访问元素
     *
     * 【Set的三个主要实现类对比】
     *               HashSet        LinkedHashSet       TreeSet
     * 底层结构       哈希表          哈希表+双向链表       红黑树
     * 有序性         无序           有序（插入顺序）       有序（自然/自定义排序）
     * 增删查效率     O(1)           O(1)                 O(log n)
     * 允许null       允许1个        允许1个              不允许（Comparable）
     * 线程安全       否             否                   否
     *
     * 【HashSet如何保证元素不重复】
     * 1. 先调用元素的hashCode()方法计算哈希值，确定存储位置
     * 2. 如果该位置没有元素，直接存入
     * 3. 如果该位置有元素，再调用equals()方法比较
     * 4. 如果equals返回true，认为重复，不存入
     * 5. 如果equals返回false，以链表/红黑树形式存储（哈希冲突）
     *
     * 所以：自定义类要想正确存入HashSet，必须重写hashCode()和equals()方法
     * ============================================================
     */
    public static void demonstrateSet() {
        System.out.println("\n--- 2. Set 集合 ---");

        // --- HashSet 演示 ---
        System.out.println("【HashSet（哈希表实现，无序，不重复）】");
        Set<String> hashSet = new HashSet<>();

        // 添加元素
        hashSet.add("苹果");
        hashSet.add("香蕉");
        hashSet.add("橙子");
        hashSet.add("苹果");  // 重复元素，添加失败，返回false
        hashSet.add(null);    // 可以存一个null

        System.out.println("HashSet内容: " + hashSet);
        System.out.println("HashSet大小: " + hashSet.size());
        System.out.println("注意：输出顺序和添加顺序可能不一致（无序）");

        // 遍历Set（没有下标，只能用增强for或迭代器）
        System.out.print("遍历HashSet: ");
        for (String fruit : hashSet) {
            System.out.print(fruit + " ");
        }
        System.out.println();

        // --- LinkedHashSet 演示 ---
        System.out.println("\n【LinkedHashSet（哈希表+链表，有序，不重复）】");
        // LinkedHashSet是HashSet的子类，底层用LinkedHashMap实现
        // 在HashSet基础上多了一个双向链表维护插入顺序
        Set<String> linkedHashSet = new LinkedHashSet<>();
        linkedHashSet.add("北京");
        linkedHashSet.add("上海");
        linkedHashSet.add("广州");
        linkedHashSet.add("深圳");
        linkedHashSet.add("北京");  // 重复元素，添加失败

        System.out.println("LinkedHashSet内容: " + linkedHashSet);
        System.out.println("注意：输出顺序和添加顺序一致（有序）");

        // --- TreeSet 演示 ---
        System.out.println("\n【TreeSet（红黑树实现，排序，不重复）】");
        // TreeSet底层是TreeMap（红黑树），元素会自动排序
        // 元素必须实现Comparable接口，或者构造时传入Comparator比较器
        Set<Integer> treeSet = new TreeSet<>();
        treeSet.add(50);
        treeSet.add(20);
        treeSet.add(80);
        treeSet.add(10);
        treeSet.add(20);  // 重复元素

        System.out.println("TreeSet内容: " + treeSet);
        System.out.println("注意：元素自动按自然顺序排序（升序）");

        // TreeSet的特殊方法（因为实现了NavigableSet接口）
        TreeSet<Integer> treeSet2 = new TreeSet<>();
        treeSet2.addAll(Arrays.asList(10, 20, 30, 40, 50));
        System.out.println("TreeSet: " + treeSet2);
        System.out.println("第一个（最小）: " + treeSet2.first());
        System.out.println("最后一个（最大）: " + treeSet2.last());
        System.out.println("小于等于30的最大元素: " + treeSet2.floor(30));
        System.out.println("大于等于35的最小元素: " + treeSet2.ceiling(35));
        System.out.println("小于30的最大元素: " + treeSet2.lower(30));
        System.out.println("大于30的最小元素: " + treeSet2.higher(30));

        // 自定义排序：通过Comparator比较器
        System.out.println("\n【TreeSet自定义排序（倒序）】");
        Set<String> reverseSet = new TreeSet<>(Comparator.reverseOrder());
        reverseSet.add("banana");
        reverseSet.add("apple");
        reverseSet.add("cherry");
        System.out.println("倒序排列: " + reverseSet);

        // 自定义对象的排序
        System.out.println("\n【TreeSet存储自定义对象】");
        Set<Student> studentSet = new TreeSet<>();  // Student必须实现Comparable
        studentSet.add(new Student("张三", 85));
        studentSet.add(new Student("李四", 92));
        studentSet.add(new Student("王五", 78));
        System.out.println("按分数排序的学生: " + studentSet);
    }

    /**
     * ============================================================
     * 演示Map集合
     *
     * 【Map接口特点】
     * - 双列集合：存储键值对（key-value）
     * - key不允许重复，value可以重复
     * - 一个key只能对应一个value
     *
     * 【Map的四个主要实现类对比】
     *                HashMap       LinkedHashMap     TreeMap         Hashtable
     * 底层结构        哈希表+红黑树   哈希表+双向链表+    红黑树           哈希表
     *                            红黑树
     * 有序性          无序          有序（插入/访问）    有序（排序）      无序
     * 允许null键/值    各1个         各1个              不允许null键      都不允许
     * 线程安全         否            否                 否               是
     * 效率             高            略低               较低             低
     *
     * 【HashMap的底层原理（重要）】
     * - Java 8以前：数组 + 链表
     * - Java 8及以后：数组 + 链表 + 红黑树
     *   - 当链表长度超过阈值（8）且数组长度超过64时，链表转为红黑树
     *   - 当红黑树节点数少于6时，转回链表
     *   - 目的：解决哈希冲突严重时的查询效率问题（链表O(n) → 红黑树O(log n)）
     *
     * 【HashMap的重要参数】
     * - 初始容量（initialCapacity）：默认16，必须是2的幂
     * - 加载因子（loadFactor）：默认0.75
     * - 扩容阈值（threshold）= 容量 × 加载因子
     * - 当元素个数超过阈值时，数组扩容为原来的2倍，并重新哈希（rehash）
     * ============================================================
     */
    public static void demonstrateMap() {
        System.out.println("\n--- 3. Map 集合 ---");

        // --- HashMap 演示 ---
        System.out.println("【HashMap（最常用，无序，key不重复）】");
        // 创建HashMap，泛型指定key和value的类型
        Map<String, Integer> hashMap = new HashMap<>();

        // put：添加键值对，如果key已存在则覆盖旧值，并返回旧值
        hashMap.put("语文", 90);
        hashMap.put("数学", 85);
        hashMap.put("英语", 95);
        Integer oldValue = hashMap.put("数学", 88);  // key重复，覆盖旧值
        System.out.println("HashMap内容: " + hashMap);
        System.out.println("数学的旧值: " + oldValue + "（被88覆盖）");

        // get：根据key获取value
        System.out.println("语文成绩: " + hashMap.get("语文"));
        System.out.println("物理成绩: " + hashMap.get("物理"));  // key不存在返回null

        // getOrDefault：获取value，不存在则返回默认值（Java 8+）
        System.out.println("物理成绩（默认0）: " + hashMap.getOrDefault("物理", 0));

        // containsKey / containsValue：判断是否包含
        System.out.println("包含'英语'键: " + hashMap.containsKey("英语"));
        System.out.println("包含90值: " + hashMap.containsValue(90));

        // remove：删除键值对
        hashMap.remove("英语");
        System.out.println("删除英语后: " + hashMap);

        // size：获取键值对数量
        System.out.println("键值对数量: " + hashMap.size());

        // Map的三种遍历方式
        System.out.println("\n【Map的三种遍历方式】");

        // 方式1：遍历key集合（keySet）
        System.out.print("遍历key: ");
        for (String key : hashMap.keySet()) {
            System.out.print(key + " ");
        }
        System.out.println();

        // 方式2：遍历value集合（values）
        System.out.print("遍历value: ");
        for (Integer value : hashMap.values()) {
            System.out.print(value + " ");
        }
        System.out.println();

        // 方式3：遍历键值对（entrySet），推荐（效率最高）
        System.out.println("遍历Entry（键值对）:");
        for (Map.Entry<String, Integer> entry : hashMap.entrySet()) {
            System.out.println("  " + entry.getKey() + " → " + entry.getValue());
        }

        // --- LinkedHashMap 演示 ---
        System.out.println("\n【LinkedHashMap（有序，可以按插入或访问顺序）】");
        // LinkedHashMap继承自HashMap，多了一个双向链表维护顺序
        // 默认按插入顺序排序
        Map<String, String> linkedHashMap = new LinkedHashMap<>();
        linkedHashMap.put("name", "张三");
        linkedHashMap.put("age", "25");
        linkedHashMap.put("city", "北京");
        System.out.println("LinkedHashMap: " + linkedHashMap);
        System.out.println("注意：输出顺序和插入顺序一致");

        // 按访问顺序排序（accessOrder=true）：最近访问的放最后
        System.out.println("\n【LinkedHashMap按访问顺序排序】");
        LinkedHashMap<String, String> accessMap = new LinkedHashMap<>(16, 0.75f, true);
        accessMap.put("a", "1");
        accessMap.put("b", "2");
        accessMap.put("c", "3");
        System.out.println("初始顺序: " + accessMap);
        accessMap.get("a");  // 访问a，a会移到最后
        accessMap.get("b");  // 访问b，b会移到最后
        System.out.println("访问a、b后: " + accessMap + "（最近访问的在最后）");

        // --- TreeMap 演示 ---
        System.out.println("\n【TreeMap（红黑树实现，按key排序）】");
        // TreeMap按key的自然顺序或自定义比较器排序
        Map<String, Integer> treeMap = new TreeMap<>();
        treeMap.put("banana", 20);
        treeMap.put("apple", 10);
        treeMap.put("cherry", 30);
        System.out.println("TreeMap: " + treeMap);
        System.out.println("注意：key按字母顺序自动排序");

        // --- Hashtable 演示（了解即可，已过时） ---
        System.out.println("\n【Hashtable（线程安全，已过时）】");
        Hashtable<String, String> hashtable = new Hashtable<>();
        hashtable.put("key1", "value1");
        System.out.println("Hashtable: " + hashtable);
        System.out.println("Hashtable的方法都加了synchronized，线程安全但效率低");
        System.out.println("不推荐使用，推荐ConcurrentHashMap（并发包中）");
    }

    /**
     * ============================================================
     * 演示Queue队列
     *
     * 【Queue接口特点】
     * - 队列是一种先进先出（FIFO）的数据结构
     * - 常见实现：LinkedList、PriorityQueue、ArrayDeque
     *
     * 【Queue的常用方法】
     *          抛出异常        返回特殊值
     * 插入      add(e)         offer(e)
     * 移除      remove()       poll()
     * 检查      element()      peek()
     *
     * 【PriorityQueue（优先队列）】
     * - 不是按插入顺序，而是按优先级（自然顺序或Comparator）出队
     * - 底层是堆（完全二叉树）
     *
     * 【Deque（双端队列）】
     * - 两端都可以添加和删除元素
     * - 可以当作队列使用，也可以当作栈使用
     * - 实现类：ArrayDeque、LinkedList
     * ============================================================
     */
    public static void demonstrateQueue() {
        System.out.println("\n--- 4. Queue 队列 ---");

        // --- 普通队列（FIFO） ---
        System.out.println("【Queue（先进先出 FIFO）】");
        Queue<String> queue = new LinkedList<>();

        // 入队
        queue.offer("顾客1");
        queue.offer("顾客2");
        queue.offer("顾客3");
        System.out.println("队列内容: " + queue);

        // 查看队首元素（不删除）
        System.out.println("队首元素: " + queue.peek());

        // 出队
        System.out.println("出队: " + queue.poll());
        System.out.println("出队后: " + queue);

        // --- PriorityQueue（优先队列） ---
        System.out.println("\n【PriorityQueue（优先队列，按优先级出队）】");
        // 默认按自然顺序（小的优先级高）
        PriorityQueue<Integer> pq = new PriorityQueue<>();
        pq.offer(30);
        pq.offer(10);
        pq.offer(20);
        System.out.print("优先队列出队顺序（从小到大）: ");
        while (!pq.isEmpty()) {
            System.out.print(pq.poll() + " ");
        }
        System.out.println();

        // 自定义优先级（从大到小）
        PriorityQueue<Integer> pq2 = new PriorityQueue<>(Comparator.reverseOrder());
        pq2.offer(30);
        pq2.offer(10);
        pq2.offer(20);
        System.out.print("优先队列出队顺序（从大到小）: ");
        while (!pq2.isEmpty()) {
            System.out.print(pq2.poll() + " ");
        }
        System.out.println();

        // --- Deque（双端队列） ---
        System.out.println("\n【Deque（双端队列）】");
        Deque<String> deque = new ArrayDeque<>();
        deque.offerFirst("左");
        deque.offerLast("右");
        deque.offerFirst("最左");
        System.out.println("双端队列: " + deque);
        System.out.println("左端出队: " + deque.pollFirst());
        System.out.println("右端出队: " + deque.pollLast());

        // Deque当作栈使用（LIFO）
        System.out.println("\n【Deque当作栈（后进先出 LIFO）】");
        Deque<Integer> stack = new ArrayDeque<>();
        stack.push(1);  // 入栈
        stack.push(2);
        stack.push(3);
        System.out.println("栈内容: " + stack);
        System.out.println("出栈: " + stack.pop());
        System.out.println("出栈后: " + stack);
    }

    /**
     * ============================================================
     * 演示迭代器 Iterator 和 ListIterator
     *
     * 【迭代器（Iterator）是什么】
     * - 是一种设计模式，用于遍历集合中的元素
     * - 把遍历逻辑和集合本身分离，不同集合可以用统一的方式遍历
     * - 所有实现了Collection接口的集合都有迭代器
     *
     * 【Iterator的三个方法】
     * - hasNext()：判断是否还有下一个元素
     * - next()：返回下一个元素，并移动指针
     * - remove()：删除刚返回的元素（可选操作）
     *
     * 【ListIterator】
     * - 是Iterator的子接口，只能用于List集合
     * - 增加了：向前遍历、添加元素、设置元素、获取索引等功能
     *
     * 【增强for循环（for-each）】
     * - Java 5引入，底层也是迭代器
     * - 语法简洁，但不能在遍历中删除元素（会报ConcurrentModificationException）
     * - 遍历中删除元素应该用迭代器的remove方法
     * ============================================================
     */
    public static void demonstrateIterator() {
        System.out.println("\n--- 5. 迭代器 Iterator ---");

        List<String> list = new ArrayList<>();
        list.add("苹果");
        list.add("香蕉");
        list.add("橙子");
        list.add("葡萄");

        // --- 使用Iterator遍历 ---
        System.out.println("【Iterator遍历】");
        Iterator<String> it = list.iterator();  // 获取迭代器
        while (it.hasNext()) {                   // 判断是否有下一个元素
            String fruit = it.next();            // 获取下一个元素
            System.out.print(fruit + " ");
        }
        System.out.println();

        // --- 使用迭代器删除元素（安全的方式） ---
        System.out.println("\n【使用迭代器删除元素（安全）】");
        System.out.println("删除前: " + list);
        Iterator<String> it2 = list.iterator();
        while (it2.hasNext()) {
            String fruit = it2.next();
            if (fruit.equals("香蕉")) {
                it2.remove();  // 使用迭代器的remove方法删除（安全）
            }
        }
        System.out.println("删除香蕉后: " + list);

        // 注意：增强for循环中删除元素会抛ConcurrentModificationException
        // 因为增强for底层是迭代器，但删除用的是集合的remove方法，导致modCount不一致

        // --- ListIterator 演示 ---
        System.out.println("\n【ListIterator（双向遍历）】");
        List<String> list2 = new ArrayList<>(Arrays.asList("A", "B", "C", "D"));
        ListIterator<String> listIt = list2.listIterator();

        System.out.print("正向遍历: ");
        while (listIt.hasNext()) {
            System.out.print(listIt.next() + "[" + listIt.nextIndex() + "] ");
        }
        System.out.println();

        System.out.print("逆向遍历: ");
        while (listIt.hasPrevious()) {
            System.out.print(listIt.previous() + "[" + listIt.previousIndex() + "] ");
        }
        System.out.println();

        // ListIterator可以在遍历中添加和修改
        ListIterator<String> listIt2 = list2.listIterator();
        while (listIt2.hasNext()) {
            String s = listIt2.next();
            if (s.equals("B")) {
                listIt2.add("B+");  // 在当前元素后面添加
            }
        }
        System.out.println("添加B+后: " + list2);
    }

    /**
     * ============================================================
     * 演示Collections工具类
     *
     * 【Collections工具类是什么】
     * - java.util.Collections是一个工具类，专门用来操作集合
     * - 里面都是静态方法，提供了各种集合操作的功能
     * - 注意和Collection接口区分（少了个s）
     *
     * 【常用功能】
     * - 排序：sort, reverse, shuffle, swap
     * - 查找：binarySearch, max, min
     * - 复制：copy
     * - 填充：fill
     * - 同步集合：synchronizedList, synchronizedSet, synchronizedMap
     * - 不可变集合：unmodifiableList, unmodifiableSet, unmodifiableMap
     * - 空集合：emptyList, emptySet, emptyMap
     * - 单元素集合：singleton, singletonList, singletonMap
     * ============================================================
     */
    public static void demonstrateCollections() {
        System.out.println("\n--- 6. Collections 工具类 ---");

        List<Integer> list = new ArrayList<>(Arrays.asList(5, 3, 8, 1, 9, 2, 7, 4, 6));
        System.out.println("原始列表: " + list);

        // --- 排序相关 ---
        System.out.println("\n【排序操作】");

        // sort：排序（升序）
        List<Integer> sortedList = new ArrayList<>(list);
        Collections.sort(sortedList);
        System.out.println("sort升序: " + sortedList);

        // sort + 自定义比较器
        Collections.sort(sortedList, Collections.reverseOrder());
        System.out.println("sort降序: " + sortedList);

        // reverse：反转
        List<Integer> reversedList = new ArrayList<>(list);
        Collections.reverse(reversedList);
        System.out.println("reverse反转: " + reversedList);

        // shuffle：随机打乱
        List<Integer> shuffledList = new ArrayList<>(list);
        Collections.shuffle(shuffledList);
        System.out.println("shuffle打乱: " + shuffledList);

        // swap：交换两个位置的元素
        List<Integer> swapList = new ArrayList<>(list);
        Collections.swap(swapList, 0, list.size() - 1);
        System.out.println("swap交换首尾: " + swapList);

        // --- 查找相关 ---
        System.out.println("\n【查找操作】");
        Collections.sort(sortedList);
        // binarySearch：二分查找（必须先排序）
        int index = Collections.binarySearch(sortedList, 5);
        System.out.println("binarySearch查找5的位置: " + index);

        // max / min：求最大值和最小值
        System.out.println("max最大值: " + Collections.max(list));
        System.out.println("min最小值: " + Collections.min(list));

        // --- 其他操作 ---
        System.out.println("\n【其他操作】");

        // frequency：统计元素出现次数
        List<String> strList = Arrays.asList("a", "b", "a", "c", "a");
        System.out.println("列表: " + strList);
        System.out.println("'a'出现次数: " + Collections.frequency(strList, "a"));

        // fill：用指定元素填充整个列表
        List<Integer> fillList = new ArrayList<>(Arrays.asList(1, 2, 3));
        Collections.fill(fillList, 0);
        System.out.println("fill填充0: " + fillList);

        // copy：复制列表（目标列表长度必须 >= 源列表）
        List<Integer> srcList = Arrays.asList(10, 20, 30);
        List<Integer> destList = new ArrayList<>(Arrays.asList(0, 0, 0, 0, 0));
        Collections.copy(destList, srcList);
        System.out.println("copy复制后: " + destList);

        // --- 同步集合（不推荐，推荐用并发包的类） ---
        System.out.println("\n【同步集合】");
        List<String> syncList = Collections.synchronizedList(new ArrayList<>());
        System.out.println("Collections.synchronizedList() 返回线程安全的List");
        System.out.println("但内部用synchronized实现，效率不如ConcurrentHashMap等并发包类");

        // --- 不可变集合 ---
        System.out.println("\n【不可变集合】");
        List<String> unmodList = Collections.unmodifiableList(Arrays.asList("a", "b", "c"));
        System.out.println("unmodifiableList返回的集合不能修改");
        // unmodList.add("d");  // 会抛出UnsupportedOperationException
    }

    /**
     * ============================================================
     * 演示泛型
     *
     * 【泛型（Generics）是什么】
     * - Java 5引入的特性，参数化类型，把类型当作参数传递
     * - 格式：<类型参数>，如List<String>表示只能存String的List
     * - 好处：
     *   1. 类型安全：编译时检查类型，避免运行时ClassCastException
     *   2. 避免强制类型转换：取元素时不需要强转
     *   3. 代码复用：一套代码可以适用于多种类型
     *
     * 【泛型的三种使用方式】
     * 1. 泛型类：类定义时有类型参数
     * 2. 泛型方法：方法定义时有类型参数
     * 3. 泛型接口：接口定义时有类型参数
     *
     * 【通配符（Wildcard）】
     * - ?：无限制通配符，表示任意类型
     * - ? extends T：上界通配符，表示T或T的子类（可以读，不能写）
     * - ? super T：下界通配符，表示T或T的父类（可以写，读受限）
     * - PECS原则：Producer Extends, Consumer Super
     *   生产者（读取）用extends，消费者（写入）用super
     *
     * 【泛型擦除（Type Erasure）】
     * - 泛型只在编译期有效，运行时会被擦除（替换为Object或上界）
     * - 所以运行时List<String>和List<Integer>是同一个类（都是List.class）
     * - 这也是为什么不能用new T()、不能用instanceof <T>等的原因
     * ============================================================
     */
    public static void demonstrateGeneric() {
        System.out.println("\n--- 7. 泛型（Generics）---");

        // --- 泛型类的使用 ---
        System.out.println("【泛型类】");
        // 创建一个存储String的Box
        Box<String> stringBox = new Box<>("Hello");
        System.out.println("String Box内容: " + stringBox.getItem());

        // 创建一个存储Integer的Box
        Box<Integer> intBox = new Box<>(123);
        System.out.println("Integer Box内容: " + intBox.getItem());

        // --- 泛型方法的使用 ---
        System.out.println("\n【泛型方法】");
        // 泛型方法可以自动推断类型参数
        Integer[] intArray = {1, 2, 3, 4, 5};
        String[] strArray = {"a", "b", "c"};

        System.out.print("打印Integer数组: ");
        printArray(intArray);  // 类型推断为Integer
        System.out.print("打印String数组: ");
        printArray(strArray);  // 类型推断为String

        // 泛型方法求中间元素
        Integer midInt = getMiddle(intArray);
        System.out.println("Integer数组中间元素: " + midInt);

        // --- 通配符演示 ---
        System.out.println("\n【通配符 ? extends（上界通配符）】");
        // ? extends Number 表示可以是Number或Number的子类
        List<Integer> intList = Arrays.asList(1, 2, 3);
        List<Double> doubleList = Arrays.asList(1.1, 2.2, 3.3);

        printNumberList(intList);     // Integer是Number的子类，可以
        printNumberList(doubleList);  // Double是Number的子类，可以

        System.out.println("\n【通配符 ? super（下界通配符）】");
        // ? super Integer 表示可以是Integer或Integer的父类
        List<Number> numberList = new ArrayList<>();
        List<Object> objectList = new ArrayList<>();

        addInteger(numberList);       // Number是Integer的父类，可以
        addInteger(objectList);       // Object是Integer的父类，可以
        System.out.println("numberList添加后: " + numberList);
        System.out.println("objectList添加后: " + objectList);

        System.out.println("\n【无限制通配符 ?】");
        // ? 表示任意类型
        List<String> sList = Arrays.asList("a", "b");
        List<Integer> iList = Arrays.asList(1, 2);
        printAnyList(sList);
        printAnyList(iList);

        // --- 泛型擦除验证 ---
        System.out.println("\n【泛型擦除】");
        // 运行时List<String>和List<Integer>的Class对象是同一个
        Class<?> c1 = new ArrayList<String>().getClass();
        Class<?> c2 = new ArrayList<Integer>().getClass();
        System.out.println("ArrayList<String>.class == ArrayList<Integer>.class: " + (c1 == c2));
        System.out.println("因为泛型在运行时被擦除了，两者都是ArrayList.class");
    }

    /**
     * 泛型类：Box（箱子），可以存放任意类型的物品
     *
     * @param <T> 类型参数，表示箱子中存放的物品类型
     */
    static class Box<T> {
        // 成员变量的类型用T表示
        private T item;

        // 构造方法，参数类型也是T
        public Box(T item) {
            this.item = item;
        }

        // getter方法，返回类型是T
        public T getItem() {
            return item;
        }

        // setter方法，参数类型是T
        public void setItem(T item) {
            this.item = item;
        }
    }

    /**
     * 泛型方法：打印任意类型的数组
     *
     * 【泛型方法的定义】
     * 修饰符 <类型参数> 返回类型 方法名(参数列表)
     * 类型参数放在修饰符和返回类型之间
     *
     * @param arr 要打印的数组
     * @param <T> 数组元素的类型
     */
    public static <T> void printArray(T[] arr) {
        for (T element : arr) {
            System.out.print(element + " ");
        }
        System.out.println();
    }

    /**
     * 泛型方法：获取数组的中间元素
     *
     * @param arr 数组
     * @param <T> 数组元素类型
     * @return 中间元素
     */
    public static <T> T getMiddle(T[] arr) {
        return arr[arr.length / 2];
    }

    /**
     * 上界通配符 ? extends Number
     * 可以读取元素（都是Number类型），但不能添加元素（不知道具体子类型）
     * 即：Producer Extends（生产者，往外拿数据）
     */
    public static void printNumberList(List<? extends Number> list) {
        System.out.print("列表内容: ");
        for (Number num : list) {  // 可以读，元素至少是Number类型
            System.out.print(num + " ");
        }
        System.out.println();
        // list.add(1);  // 编译错误：不能添加，因为不知道具体是什么子类型
    }

    /**
     * 下界通配符 ? super Integer
     * 可以添加Integer或其子类型的元素，但读取受限（只能当Object用）
     * 即：Consumer Super（消费者，往里放数据）
     */
    public static void addInteger(List<? super Integer> list) {
        list.add(10);  // 可以添加Integer类型
        list.add(20);
        // Integer i = list.get(0);  // 编译错误：读出来只能当Object用
    }

    /**
     * 无限制通配符 ?
     * 可以接收任意类型的List，但只能做一些与类型无关的操作
     */
    public static void printAnyList(List<?> list) {
        System.out.println("列表大小: " + list.size());
        // 读出来只能当Object用
        Object first = list.get(0);
        System.out.println("第一个元素（Object）: " + first);
        // list.add(?);  // 不能添加任何元素（null除外）
    }

    /**
     * ============================================================
     * 演示Stream API（Java 8+）
     *
     * 【Stream API是什么】
     * - Java 8引入的用于操作集合的API，支持函数式编程
     * - Stream不是集合，不存储数据，而是对数据进行计算/加工
     * - Stream操作分为三类：
     *   1. 创建流：stream()、parallelStream()等
     *   2. 中间操作：filter、map、sorted、distinct等（返回新的Stream，可以链式调用）
     *   3. 终止操作：forEach、collect、reduce、count等（产生结果，流关闭）
     *
     * 【Stream的特点】
     * - 惰性求值：中间操作只是记录操作，不会立即执行，等到终止操作才执行
     * - 流只能消费一次：遍历过一次就用完了，需要重新创建
     * - 内部迭代：迭代由Stream内部完成，不需要写for循环
     *
     * 【常用中间操作】
     * - filter：过滤，保留满足条件的元素
     * - map：映射，将元素转换为另一种形式
     * - flatMap：扁平化映射，将流中的每个元素都换成另一个流
     * - distinct：去重
     * - sorted：排序
     * - limit：取前n个元素
     * - skip：跳过前n个元素
     * - peek：遍历并执行操作（调试用，不改变流）
     *
     * 【常用终止操作】
     * - forEach：遍历每个元素
     * - collect：收集结果（转成List/Set/Map等）
     * - reduce：归约，将元素组合成一个值
     * - count：统计元素个数
     * - max/min：找最大/最小值
     * - anyMatch/allMatch/noneMatch：匹配判断
     * - findFirst/findAny：查找元素
     * ============================================================
     */
    public static void demonstrateStream() {
        System.out.println("\n--- 8. Stream API（Java 8+）---");

        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        System.out.println("原始数据: " + numbers);

        // --- filter 过滤 ---
        System.out.println("\n【filter 过滤】");
        // 过滤出偶数
        List<Integer> evenNumbers = numbers.stream()
                .filter(n -> n % 2 == 0)       // 中间操作：过滤偶数
                .collect(Collectors.toList()); // 终止操作：收集为List
        System.out.println("偶数: " + evenNumbers);

        // --- map 映射 ---
        System.out.println("\n【map 映射（每个数乘2）】");
        List<Integer> doubled = numbers.stream()
                .map(n -> n * 2)                // 中间操作：每个元素乘2
                .collect(Collectors.toList());
        System.out.println("每个数乘2: " + doubled);

        // --- sorted 排序 ---
        System.out.println("\n【sorted 排序（倒序）】");
        List<Integer> sortedDesc = numbers.stream()
                .sorted(Comparator.reverseOrder())  // 中间操作：降序排序
                .collect(Collectors.toList());
        System.out.println("倒序排列: " + sortedDesc);

        // --- distinct 去重 ---
        System.out.println("\n【distinct 去重】");
        List<Integer> duplicates = Arrays.asList(1, 2, 2, 3, 3, 3, 4);
        List<Integer> unique = duplicates.stream()
                .distinct()                     // 中间操作：去重
                .collect(Collectors.toList());
        System.out.println("去重前: " + duplicates);
        System.out.println("去重后: " + unique);

        // --- limit 和 skip ---
        System.out.println("\n【limit 和 skip】");
        List<Integer> limited = numbers.stream()
                .skip(3)                         // 中间操作：跳过前3个
                .limit(5)                        // 中间操作：取前5个
                .collect(Collectors.toList());
        System.out.println("skip(3).limit(5): " + limited);

        // --- reduce 归约 ---
        System.out.println("\n【reduce 归约（求和）】");
        int sum = numbers.stream()
                .reduce(0, (a, b) -> a + b);    // 终止操作：归约求和
        System.out.println("所有数的和: " + sum);

        // --- count / max / min ---
        System.out.println("\n【count、max、min】");
        long count = numbers.stream().count();
        int max = numbers.stream().max(Integer::compareTo).orElse(0);
        int min = numbers.stream().min(Integer::compareTo).orElse(0);
        System.out.println("个数: " + count + ", 最大值: " + max + ", 最小值: " + min);

        // --- 匹配操作 ---
        System.out.println("\n【匹配操作】");
        boolean allEven = numbers.stream().allMatch(n -> n % 2 == 0);
        boolean anyEven = numbers.stream().anyMatch(n -> n % 2 == 0);
        boolean noneNegative = numbers.stream().noneMatch(n -> n < 0);
        System.out.println("所有数都是偶数: " + allEven);
        System.out.println("有至少一个偶数: " + anyEven);
        System.out.println("没有负数: " + noneNegative);

        // --- 链式操作（综合示例） ---
        System.out.println("\n【链式综合示例】");
        // 找出大于5的偶数，乘以10，倒序排列，取前3个
        List<Integer> result = numbers.stream()
                .filter(n -> n > 5)           // 过滤大于5的
                .filter(n -> n % 2 == 0)      // 过滤偶数
                .map(n -> n * 10)             // 乘以10
                .sorted(Comparator.reverseOrder()) // 倒序
                .limit(3)                     // 取前3个
                .collect(Collectors.toList());
        System.out.println("综合操作结果: " + result);

        // --- 复杂对象的Stream操作 ---
        System.out.println("\n【复杂对象的Stream操作】");
        List<Student> students = Arrays.asList(
                new Student("张三", 85),
                new Student("李四", 92),
                new Student("王五", 78),
                new Student("赵六", 95),
                new Student("钱七", 60)
        );

        // 找出分数大于80的学生，按分数降序，收集姓名列表
        List<String> topNames = students.stream()
                .filter(s -> s.getScore() > 80)        // 过滤分数>80
                .sorted((s1, s2) -> s2.getScore() - s1.getScore()) // 按分数降序
                .map(Student::getName)                  // 映射为姓名
                .collect(Collectors.toList());
        System.out.println("分数大于80的学生（按分数降序）: " + topNames);

        // 计算平均分
        double avgScore = students.stream()
                .mapToInt(Student::getScore)           // 转为IntStream
                .average()                              // 求平均值
                .orElse(0.0);                           // 如果为空返回0
        System.out.println("平均分: " + avgScore);

        // --- 并行流 ---
        System.out.println("\n【并行流 parallelStream】");
        // 并行流使用多线程处理，适合大数据量的情况
        // 注意：并行流不一定更快，数据量小的时候可能更慢（线程切换开销）
        long parallelSum = numbers.parallelStream()
                .mapToLong(Integer::longValue)
                .sum();
        System.out.println("并行流求和: " + parallelSum);
        System.out.println("并行流会使用多线程，适合大数据量计算");
    }

    /**
     * Student学生类：用于演示Set排序和Stream操作
     * 实现Comparable接口，用于TreeSet排序
     */
    static class Student implements Comparable<Student> {
        private String name;
        private int score;

        public Student(String name, int score) {
            this.name = name;
            this.score = score;
        }

        public String getName() {
            return name;
        }

        public int getScore() {
            return score;
        }

        /**
         * 实现Comparable接口的compareTo方法
         * 定义比较规则：按分数升序排列
         *
         * @param other 另一个学生对象
         * @return 负数表示this小，0表示相等，正数表示this大
         */
        @Override
        public int compareTo(Student other) {
            // 按分数升序
            return Integer.compare(this.score, other.score);
            // 如果分数相同，按姓名排序（避免分数相同被认为相等而丢失元素）
            // int scoreComp = Integer.compare(this.score, other.score);
            // return scoreComp != 0 ? scoreComp : this.name.compareTo(other.name);
        }

        @Override
        public String toString() {
            return name + "(" + score + "分)";
        }
    }
}
