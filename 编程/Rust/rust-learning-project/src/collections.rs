// ============================================================
// collections.rs - 集合与迭代器模块
// ============================================================
// Rust标准库提供了多种高效的集合类型
// 集合存储在堆上，大小可以动态变化
//
// 主要集合类型：
//   - Vec<T>：动态数组，顺序存储，可变长
//   - String：UTF-8编码的字符串
//   - HashMap<K, V>：哈希表，键值对存储
//   - VecDeque<T>：双端队列
//   - BTreeMap<K, V>：B树映射，有序键值对
//   - HashSet<T>：哈希集合
//   - BTreeSet<T>：B树集合
//   - LinkedList<T>：双向链表
//   - BinaryHeap<T>：二叉堆（优先队列）
//
// 迭代器（Iterator）：
//   - 所有集合都可以生成迭代器
//   - 迭代器实现了 Iterator trait
//   - 迭代器是惰性的（lazy），不消费就不会产生计算
// ============================================================

use std::collections::HashMap;  // 引入 HashMap（标准库的collections模块）

/// 集合与迭代器模块的入口演示函数
pub fn run_collections_demo() {
    println!("\n{}", "=".repeat(60));
    println!("集合与迭代器演示");
    println!("{}", "=".repeat(60));

    vec_demo();
    string_collections_demo();
    hashmap_demo();
    iterator_demo();
    iterator_adapter_demo();
    consuming_iterator_demo();
    performance_comparison_demo();
}

// ============================================================
// 1. Vec<T> 动态数组
// ============================================================
// Vec<T> 是Rust中最常用的集合类型，即动态数组
// 特点：
//   - 所有元素类型相同
//   - 在内存中连续存储
//   - 可以动态增长或缩小
//   - 从末尾添加/删除元素是 O(1)（均摊时间复杂度）
//   - 随机访问是 O(1)
//   - 在中间插入/删除是 O(n)
//
// 常用方法：
//   - Vec::new()：创建空向量
//   - vec![] 宏：创建带初始值的向量
//   - push()：在末尾添加元素
//   - pop()：从末尾移除并返回元素
//   - len()：元素个数
//   - is_empty()：是否为空
//   - get(index)：安全访问（返回Option，越界返回None）
//   - [index]：直接访问（越界会panic）

/// Vec<T> 动态数组演示函数
fn vec_demo() {
    println!("\n--- 1. Vec<T> 动态数组 ---");

    // ---- 创建 Vec ----
    // 方式1：Vec::new() 创建空向量
    let mut v: Vec<i32> = Vec::new();
    println!("空向量长度: {}", v.len());  // 0

    // 方式2：vec! 宏创建带初始值的向量
    let v2 = vec![1, 2, 3, 4, 5];
    println!("初始向量: {:?}", v2);

    // 方式3：vec![值; 长度] 创建指定长度的相同值
    let v3 = vec![0; 5];  // 5个0
    println!("相同初始值: {:?}", v3);

    // ---- 添加元素 ----
    v.push(1);  // 向空向量添加元素
    v.push(2);
    v.push(3);
    println!("添加元素后: {:?}", v);  // [1, 2, 3]

    // ---- 访问元素 ----
    // 方式1：使用 [] 索引（越界会 panic）
    let third: &i32 = &v[2];
    println!("第三个元素（索引2）: {}", third);  // 3

    // 方式2：使用 get() 方法（返回 Option，安全）
    match v.get(2) {
        Some(value) => println!("get(2) = {}", value),
        None => println!("索引越界"),
    }

    // 越界访问的对比
    // v[100];      // 会 panic
    let result = v.get(100);  // 返回 None，不会 panic
    println!("get(100) = {:?}", result);  // None

    // ---- 删除元素 ----
    let mut v4 = vec![1, 2, 3, 4, 5];

    // pop()：移除并返回最后一个元素（返回 Option）
    let last = v4.pop();
    println!("pop: {:?}, 剩余: {:?}", last, v4);  // Some(5), [1, 2, 3, 4]

    // remove(index)：移除指定位置的元素并返回
    // 后面的元素会前移，时间复杂度 O(n)
    let removed = v4.remove(1);
    println!("remove(1): {}, 剩余: {:?}", removed, v4);  // 2, [1, 3, 4]

    // ---- 遍历 Vec ----
    let v5 = vec![100, 32, 57];

    // 不可变遍历（只读）
    println!("遍历向量（不可变）:");
    for i in &v5 {
        println!("  {}", i);
    }

    // 可变遍历（可以修改元素）
    let mut v6 = vec![100, 32, 57];
    for i in &mut v6 {
        *i += 50;  // 通过解引用修改值
    }
    println!("修改后: {:?}", v6);  // [150, 82, 107]

    // ---- 用枚举存储多种类型 ----
    // Vec 只能存储同一类型的值
    // 如果需要存储多种类型，可以使用枚举
    #[derive(Debug)]
    enum SpreadsheetCell {
        Int(i32),
        Float(f64),
        Text(String),
    }

    let row = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Float(10.12),
        SpreadsheetCell::Text(String::from("blue")),
    ];
    println!("电子表格行: {:?}", row);
}

// ============================================================
// 2. String 字符串
// ============================================================
// String 是Rust标准库提供的可增长的UTF-8编码字符串类型
// 在基础语法中已介绍过基础用法，这里介绍更多集合相关的方法
//
// String 内部其实是 Vec<u8> 的封装
// 但它保证内容总是有效的UTF-8编码

/// String 字符串（集合视角）演示函数
fn string_collections_demo() {
    println!("\n--- 2. String 字符串 ---");

    // ---- 创建 String ----
    let mut s = String::new();           // 空字符串
    let data = "初始内容";
    let s2 = data.to_string();           // 从 &str 转换
    let s3 = String::from("直接创建");    // 直接从字面量创建

    println!("空字符串: '{}'", s);
    println!("to_string: '{}'", s2);
    println!("String::from: '{}'", s3);

    // ---- 更新字符串 ----
    s.push_str("hello");      // 追加字符串切片
    s.push('!');              // 追加单个字符
    println!("追加后: '{}'", s);  // "hello!"

    // + 运算符拼接（会取得左边字符串的所有权）
    let s4 = String::from("Hello, ");
    let s5 = String::from("world!");
    let s6 = s4 + &s5;        // s4 的所有权被移走了，之后不能再用
    println!("拼接: '{}'", s6);

    // format! 宏拼接（不会取得所有权）
    let s7 = String::from("tic");
    let s8 = String::from("tac");
    let s9 = String::from("toe");
    let formatted = format!("{}-{}-{}", s7, s8, s9);
    println!("format!: '{}'", formatted);

    // ---- 字符串长度 ----
    // len() 返回字节数，不是字符数
    let chinese = String::from("你好");
    println!("'你好' 的字节长度: {}", chinese.len());         // 6
    println!("'你好' 的字符数量: {}", chinese.chars().count()); // 2

    // ---- 字符串切片 ----
    // 必须在字符边界处切片，否则会 panic
    let s = String::from("你好世界");
    let first_two = &s[0..6];  // 前两个中文字符（每个3字节）
    println!("前两个字: '{}'", first_two);  // "你好"

    // ---- 遍历字符串 ----
    println!("遍历 chars():");
    for c in "你好".chars() {
        println!("  {}", c);
    }

    println!("遍历 bytes():");
    for b in "ab".bytes() {
        println!("  {}", b);
    }

    // ---- String 与 Vec<u8> 的关系 ----
    // String 内部是 Vec<u8>，可以相互转换
    let s = String::from("hello");
    let bytes = s.into_bytes();  // String -> Vec<u8>（会消耗 String）
    println!("字符串转字节: {:?}", bytes);  // [104, 101, 108, 108, 111]

    let bytes2 = vec![104, 101, 108, 108, 111];
    let s2 = String::from_utf8(bytes2).unwrap();  // Vec<u8> -> String
    println!("字节转字符串: '{}'", s2);  // "hello"
}

// ============================================================
// 3. HashMap<K, V> 哈希表
// ============================================================
// HashMap<K, V> 存储键值对（key-value pair）
// 通过哈希函数将键映射到内存位置，实现快速查找
//
// 特点：
//   - 键必须是唯一的
//   - 键必须实现 Hash 和 Eq trait
//   - 平均查找、插入、删除都是 O(1)
//   - 元素是无序的
//
// 常用方法：
//   - HashMap::new()：创建空哈希表
//   - insert(key, value)：插入键值对
//   - get(key)：获取值（返回 Option<&V>）
//   - remove(key)：删除键值对
//   - contains_key(key)：是否包含某个键
//   - len()：键值对数量
//   - iter()：迭代所有键值对

/// HashMap<K, V> 哈希表演示函数
fn hashmap_demo() {
    println!("\n--- 3. HashMap<K, V> 哈希表 ---");

    // ---- 创建 HashMap ----
    let mut scores: HashMap<String, i32> = HashMap::new();

    // ---- 插入键值对 ----
    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Red"), 50);
    println!("分数表: {:?}", scores);

    // ---- 访问值 ----
    // get() 返回 Option<&V>
    let team_name = String::from("Blue");
    let score = scores.get(&team_name);  // 传入引用
    match score {
        Some(s) => println!("{} 队的分数: {}", team_name, s),
        None => println!("没有找到 {} 队", team_name),
    }

    // ---- 遍历 HashMap ----
    println!("遍历所有键值对:");
    for (key, value) in &scores {
        println!("  {}: {}", key, value);
    }

    // ---- 更新 HashMap ----
    // 1. 覆盖已有值
    scores.insert(String::from("Blue"), 25);  // Blue 已存在，值被覆盖
    println!("覆盖后: {:?}", scores);

    // 2. 只在键不存在时插入（entry + or_insert）
    // entry API：返回 Entry 枚举，表示键是否存在
    scores.entry(String::from("Yellow")).or_insert(50);  // 不存在，插入
    scores.entry(String::from("Blue")).or_insert(50);    // 已存在，不插入
    println!("entry 操作后: {:?}", scores);
    // Blue 仍然是 25（因为已存在，or_insert 不会覆盖）
    // Yellow 是 50（新插入的）

    // 3. 根据旧值更新值
    let text = "hello world wonderful world";
    let mut map: HashMap<&str, i32> = HashMap::new();
    for word in text.split_whitespace() {
        // or_insert 返回值的可变引用
        let count = map.entry(word).or_insert(0);
        *count += 1;  // 通过解引用修改值
    }
    println!("单词计数: {:?}", map);

    // ---- 删除 ----
    scores.remove("Red");  // 删除 Red 键值对
    println!("删除 Red 后: {:?}", scores);

    // ---- 检查键是否存在 ----
    println!("包含 Blue 吗？{}", scores.contains_key("Blue"));
    println!("包含 Red 吗？{}", scores.contains_key("Red"));

    // ---- 容量管理 ----
    println!("HashMap 大小: {}", scores.len());
    println!("HashMap 是否为空？{}", scores.is_empty());
}

// ============================================================
// 4. 迭代器（Iterator trait）
// ============================================================
// 迭代器是Rust中非常重要的概念，提供了处理元素序列的统一方式
// 所有集合都可以生成迭代器
//
// Iterator trait 定义：
//   pub trait Iterator {
//       type Item;
//       fn next(&mut self) -> Option<Self::Item>;
//       // 还有许多默认实现的方法...
//   }
//
// 迭代器的三个重要方法：
//   - iter()：产生不可变引用的迭代器（&T）
//   - iter_mut()：产生可变引用的迭代器（&mut T）
//   - into_iter()：产生获取所有权的迭代器（T）
//
// 迭代器是惰性的（lazy）：
//   - 创建迭代器本身不消耗任何资源
//   - 只有调用消费适配器（consuming adaptor）时才真正开始迭代

/// 迭代器演示函数
fn iterator_demo() {
    println!("\n--- 4. 迭代器（Iterator trait）---");

    // ---- 基本迭代 ----
    let v = vec![1, 2, 3];

    // iter()：不可变引用迭代器
    println!("iter() 遍历:");
    for val in v.iter() {
        println!("  {}", val);  // val 是 &i32 类型
    }

    // iter_mut()：可变引用迭代器
    let mut v2 = vec![1, 2, 3];
    for val in v2.iter_mut() {
        *val *= 2;  // 修改原值
    }
    println!("iter_mut() 修改后: {:?}", v2);  // [2, 4, 6]

    // into_iter()：获取所有权的迭代器
    // 迭代后原向量不再可用（所有权被移走了）
    let v3 = vec![1, 2, 3];
    println!("into_iter() 遍历:");
    for val in v3.into_iter() {
        println!("  {}", val);  // val 是 i32 类型（获得所有权）
    }
    // 之后 v3 不能再使用了（所有权已转移）

    // ---- next() 方法 ----
    // Iterator trait 的核心方法是 next()
    // 返回 Option<Self::Item>：有值返回 Some(value)，没有返回 None
    let v4 = vec![1, 2, 3];
    let mut iter = v4.iter();  // 创建迭代器（必须是 mut，因为 next 会修改内部状态）

    println!("手动调用 next():");
    println!("  {:?}", iter.next());  // Some(1)
    println!("  {:?}", iter.next());  // Some(2)
    println!("  {:?}", iter.next());  // Some(3)
    println!("  {:?}", iter.next());  // None

    // ---- for 循环与迭代器 ----
    // for 循环本质上就是迭代器的语法糖
    // for x in collection { ... }
    // 等价于：
    // let mut iter = collection.into_iter();
    // while let Some(x) = iter.next() { ... }

    println!("迭代器的特点：");
    println!("  - 惰性：不消费就不计算");
    println!("  - 统一接口：所有集合使用相同的迭代器模式");
    println!("  - 零成本抽象：编译后和手写循环一样高效");
}

// ============================================================
// 5. 迭代器适配器
// ============================================================
// 迭代器适配器（Iterator Adapter）：将一个迭代器转换为另一个迭代器
// 适配器本身也是惰性的，必须用消费适配器才能真正执行
//
// 常用适配器：
//   - map：对每个元素应用函数，产生新的迭代器
//   - filter：根据条件过滤元素
//   - take(n)：取前 n 个元素
//   - skip(n)：跳过前 n 个元素
//   - enumerate：产生 (索引, 值) 对
//   - zip：将两个迭代器合并为元组迭代器
//   - chain：连接两个迭代器
//   - flat_map：扁平化嵌套的迭代器
//   - rev：反转迭代器

/// 迭代器适配器演示函数
fn iterator_adapter_demo() {
    println!("\n--- 5. 迭代器适配器 ---");

    // ---- map 适配器 ----
    // 对每个元素应用闭包，产生新的迭代器
    let v: Vec<i32> = vec![1, 2, 3];
    let mapped: Vec<i32> = v.iter()       // 创建迭代器
        .map(|x| x + 1)                   // map 适配器：每个元素加1
        .collect();                       // collect 消费迭代器，收集到 Vec
    println!("map: {:?} -> {:?}", v, mapped);  // [1, 2, 3] -> [2, 3, 4]

    // ---- filter 适配器 ----
    // 根据条件过滤元素，保留满足条件的
    let v = vec![1, 2, 3, 4, 5, 6];
    let evens: Vec<i32> = v.iter()
        .filter(|&x| x % 2 == 0)          // 只保留偶数
        .cloned()                         // 将 &i32 转为 i32
        .collect();
    println!("filter 偶数: {:?}", evens);  // [2, 4, 6]

    // ---- 链式调用多个适配器 ----
    // 迭代器适配器可以链式调用，形成数据处理流水线
    let v = vec![1, 2, 3, 4, 5];
    let result: Vec<i32> = v.iter()
        .map(|x| x * x)                    // 先平方
        .filter(|&x| x > 5)                // 再过滤大于5的
        .take(2)                           // 取前2个
        .collect();
    println!("链式调用: {:?}", result);    // [9, 16]
    // 解释：1->1(过滤掉), 2->4(过滤掉), 3->9(保留), 4->16(保留), 5->25(但只取前2个)

    // ---- enumerate 适配器 ----
    // 产生 (索引, 值) 元组
    let v = vec!['a', 'b', 'c'];
    println!("enumerate:");
    for (i, val) in v.iter().enumerate() {
        println!("  [{}] = {}", i, val);
    }

    // ---- zip 适配器 ----
    // 将两个迭代器合并为元组迭代器
    let names = vec!["Alice", "Bob", "Charlie"];
    let ages = vec![30, 25, 35];
    let pairs: Vec<(&str, i32)> = names.iter().zip(ages.iter()).collect();
    println!("zip: {:?}", pairs);

    // ---- chain 适配器 ----
    // 连接两个迭代器，先迭代完第一个再迭代第二个
    let a = vec![1, 2, 3];
    let b = vec![4, 5, 6];
    let chained: Vec<i32> = a.iter().chain(b.iter()).cloned().collect();
    println!("chain: {:?}", chained);  // [1, 2, 3, 4, 5, 6]

    // ---- rev 适配器 ----
    // 反转迭代器（需要实现 DoubleEndedIterator trait）
    let v = vec![1, 2, 3];
    let reversed: Vec<i32> = v.iter().rev().cloned().collect();
    println!("rev: {:?}", reversed);  // [3, 2, 1]

    // ---- fold 适配器 ----
    // fold 是一个非常强大的适配器，将迭代器累积为一个值
    // 第一个参数是初始值，第二个参数是闭包（累积器, 元素）-> 新累积器
    let v = vec![1, 2, 3, 4, 5];
    let sum = v.iter().fold(0, |acc, x| acc + x);  // 累加
    let product = v.iter().fold(1, |acc, x| acc * x);  // 累乘
    println!("fold 求和: {}", sum);       // 15
    println!("fold 求积: {}", product);   // 120

    println!("迭代器适配器：");
    println!("  - map/filter：转换和过滤");
    println!("  - take/skip：截取");
    println!("  - enumerate：带索引");
    println!("  - zip/chain：合并连接");
    println!("  - fold：累积计算");
    println!("  - 惰性执行，链式调用，代码简洁");
}

// ============================================================
// 6. 消费迭代器
// ============================================================
// 消费适配器（Consuming Adaptor）：消耗迭代器，产生最终结果
// 只有消费适配器才会真正触发迭代
//
// 常用消费适配器：
//   - collect()：收集到集合中
//   - sum()：求和
//   - product()：求积
//   - count()：计数
//   - find()：查找第一个满足条件的元素
//   - position()：查找第一个满足条件的元素的索引
//   - all()：是否所有元素都满足条件
//   - any()：是否有元素满足条件
//   - max()/min()：最大/最小值
//   - for_each()：对每个元素执行操作

/// 消费迭代器演示函数
fn consuming_iterator_demo() {
    println!("\n--- 6. 消费迭代器 ---");

    let v = vec![1, 2, 3, 4, 5];

    // ---- sum ----
    let sum: i32 = v.iter().sum();
    println!("sum: {}", sum);  // 15

    // ---- product ----
    let product: i32 = v.iter().product();
    println!("product: {}", product);  // 120

    // ---- count ----
    let count = v.iter().count();
    println!("count: {}", count);  // 5

    // ---- collect ----
    // collect 是最常用的消费适配器，可以收集到各种集合
    // 需要指定目标类型（类型标注或 turbofish ::<T>）
    let collected: Vec<i32> = v.iter().map(|x| x * 2).collect();
    println!("collect to Vec: {:?}", collected);

    // 使用 turbofish 语法指定类型
    let collected2 = v.iter().map(|x| x * 2).collect::<Vec<i32>>();
    println!("turbofish collect: {:?}", collected2);

    // ---- find ----
    // 查找第一个满足条件的元素，返回 Option<&T>
    let result = v.iter().find(|&&x| x > 3);
    println!("find(>3): {:?}", result);  // Some(4)

    // ---- position ----
    // 查找第一个满足条件的元素的索引，返回 Option<usize>
    let pos = v.iter().position(|&x| x == 3);
    println!("position(==3): {:?}", pos);  // Some(2)

    // ---- all / any ----
    let all_positive = v.iter().all(|&x| x > 0);
    let any_negative = v.iter().any(|&x| x < 0);
    println!("all(>0): {}", all_positive);    // true
    println!("any(<0): {}", any_negative);    // false

    // ---- max / min ----
    let max_val = v.iter().max();
    let min_val = v.iter().min();
    println!("max: {:?}", max_val);  // Some(5)
    println!("min: {:?}", min_val);  // Some(1)

    // ---- for_each ----
    // 对每个元素执行闭包（类似 for 循环，但函数式风格）
    print!("for_each: ");
    v.iter().for_each(|x| print!("{} ", x));
    println!();

    // ---- 综合示例 ----
    // 计算所有偶数的平方和
    let numbers = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let sum_of_squares_of_evens: i32 = numbers.iter()
        .filter(|&&x| x % 2 == 0)  // 过滤偶数
        .map(|&x| x * x)            // 平方
        .sum();                     // 求和
    println!("偶数平方和: {}", sum_of_squares_of_evens);  // 220
}

// ============================================================
// 7. Rust集合性能对比
// ============================================================
// 不同的集合类型有不同的性能特征，适用于不同场景
//
// | 集合类型       | 访问    | 查找    | 插入/删除(末尾) | 插入/删除(中间) |
// |---------------|--------|--------|----------------|----------------|
// | Vec<T>        | O(1)   | O(n)   | O(1)*          | O(n)           |
// | VecDeque<T>   | O(1)   | O(n)   | O(1)* (两端)   | O(n)           |
// | LinkedList<T> | O(n)   | O(n)   | O(1)           | O(1)           |
// | HashMap<K,V>  | N/A    | O(1)*  | O(1)*          | O(1)*          |
// | BTreeMap<K,V> | O(log n) | O(log n) | O(log n)     | O(log n)       |
// | HashSet<T>    | N/A    | O(1)*  | O(1)*          | O(1)*          |
// | BTreeSet<T>   | N/A    | O(log n) | O(log n)     | O(log n)       |
//
// * 表示均摊时间复杂度（amortized）
// 哈希表的 O(1) 是平均情况，最坏情况是 O(n)（哈希冲突严重时）

/// Rust集合性能对比演示函数
fn performance_comparison_demo() {
    println!("\n--- 7. Rust集合性能对比 ---");

    println!("性能对比（时间复杂度）：");
    println!("");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "集合类型", "随机访问", "查找", "末尾插入", "中间插入");
    println!("{}", "-".repeat(60));
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "Vec<T>", "O(1)", "O(n)", "O(1)*", "O(n)");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "VecDeque<T>", "O(1)", "O(n)", "O(1)*", "O(n)");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "LinkedList<T>", "O(n)", "O(n)", "O(1)", "O(1)");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "HashMap<K,V>", "N/A", "O(1)*", "O(1)*", "O(1)*");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "BTreeMap<K,V>", "O(log n)", "O(log n)", "O(log n)", "O(log n)");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "HashSet<T>", "N/A", "O(1)*", "O(1)*", "O(1)*");
    println!("{:<15} {:<10} {:<10} {:<12} {:<12}",
             "BTreeSet<T>", "N/A", "O(log n)", "O(log n)", "O(log n)");
    println!("");
    println!("* = 均摊时间复杂度");
    println!("");

    println!("选择建议：");
    println!("  - 需要顺序存储、随机访问：Vec<T>");
    println!("  - 需要两端增删：VecDeque<T>");
    println!("  - 需要键值对快速查找：HashMap<K, V>");
    println!("  - 需要有序键值对：BTreeMap<K, V>");
    println!("  - 需要去重集合：HashSet<T> / BTreeSet<T>");
    println!("  - 需要优先队列：BinaryHeap<T>");
    println!("  - 链表在Rust中用得很少（Vec通常更快）");

    println!("\n集合与迭代器模块演示完毕！");
    println!("核心要点：");
    println!("  - Vec / String / HashMap 是最常用的集合");
    println!("  - 迭代器是Rust函数式编程的核心");
    println!("  - 迭代器适配器（惰性）+ 消费适配器（触发）");
    println!("  - 零成本抽象：编译后和手写循环一样高效");
}
