// ============================================================================
// 文件名: stl_containers.cpp
// 描述: C++ STL容器与算法模块实现文件
// 作者: C++学习项目
// 说明: 本文件实现了STL容器与算法的各种演示功能，每行代码都有详细中文注释
// ============================================================================

#include "stl_containers.h"

#include <iostream>
#include <numeric>    // 数值算法（accumulate等）
#include <iterator>   // 迭代器辅助工具

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::deque;
using std::array;

namespace stl_demo {

// ============================================================================
// 函数: demo_sequence_containers
// 描述: 演示顺序容器：vector, list, deque, array
// ============================================================================
void demo_sequence_containers() {
    cout << "\n========== 顺序容器 (Sequence Containers) ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. vector（动态数组）
    // 特点:
    //   - 连续内存存储，支持随机访问（O(1)）
    //   - 尾部插入删除高效（均摊O(1)）
    //   - 中间插入删除需要移动元素（O(n)）
    //   - 容量自动增长（通常2倍扩容）
    // 适用场景: 大多数情况的首选，数据量不确定，需要随机访问
    // ------------------------------------------------------------------------
    cout << "\n1. vector (动态数组):" << endl;

    // 创建vector的几种方式
    vector<int> vec1;         // 空vector
    vector<int> vec2(5);      // 5个元素，默认初始化为0
    vector<int> vec3(5, 10);  // 5个元素，每个都是10
    vector<int> vec4 = {1, 2, 3, 4, 5};  // 初始化列表

    cout << "   vec1 (空): size = " << vec1.size()
         << ", capacity = " << vec1.capacity() << endl;
    cout << "   vec2 (5个默认值): size = " << vec2.size()
         << ", capacity = " << vec2.capacity() << endl;
    cout << "   vec3 (5个10): size = " << vec3.size()
         << ", capacity = " << vec3.capacity() << endl;
    cout << "   vec4 = {1,2,3,4,5}: size = " << vec4.size()
         << ", capacity = " << vec4.capacity() << endl;

    // 常用操作
    vector<int> nums;
    cout << "\n   常用操作演示:" << endl;

    // push_back: 尾部添加元素
    for (int i = 1; i <= 5; ++i) {
        nums.push_back(i * 10);
        cout << "   push_back(" << i*10 << "): size=" << nums.size()
             << ", capacity=" << nums.capacity() << endl;
    }

    // 访问元素
    cout << "   nums[2] = " << nums[2] << " (下标访问，不检查越界)" << endl;
    cout << "   nums.at(2) = " << nums.at(2) << " (at()访问，越界抛异常)" << endl;
    cout << "   nums.front() = " << nums.front() << " (第一个元素)" << endl;
    cout << "   nums.back() = " << nums.back() << " (最后一个元素)" << endl;

    // 插入和删除
    nums.insert(nums.begin() + 2, 99);  // 在第3个位置插入99
    cout << "   insert(begin+2, 99) 后: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    nums.erase(nums.begin() + 1);       // 删除第2个元素
    cout << "   erase(begin+1) 后: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    nums.pop_back();                    // 删除最后一个元素
    cout << "   pop_back() 后: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // 容量管理
    nums.reserve(20);                   // 预分配容量，避免多次扩容
    cout << "   reserve(20) 后: size=" << nums.size()
         << ", capacity=" << nums.capacity() << endl;

    nums.shrink_to_fit();               // 释放多余容量（C++11）
    cout << "   shrink_to_fit() 后: size=" << nums.size()
         << ", capacity=" << nums.capacity() << endl;

    // ------------------------------------------------------------------------
    // 2. list（双向链表）
    // 特点:
    //   - 非连续内存，每个节点有前后指针
    //   - 任意位置插入删除都是O(1)（只要有迭代器）
    //   - 不支持随机访问（访问元素需要遍历，O(n)）
    //   - 每个元素有额外的指针开销
    // 适用场景: 频繁在中间插入删除，不需要随机访问
    // ------------------------------------------------------------------------
    cout << "\n2. list (双向链表):" << endl;

    list<int> lst = {3, 1, 4, 1, 5, 9, 2, 6};
    cout << "   初始 list: ";
    for (int n : lst) cout << n << " ";
    cout << endl;

    // list 特有的操作
    lst.push_front(0);      // 头部插入（vector没有）
    cout << "   push_front(0): ";
    for (int n : lst) cout << n << " ";
    cout << endl;

    lst.pop_front();        // 头部删除（vector没有）
    cout << "   pop_front(): ";
    for (int n : lst) cout << n << " ";
    cout << endl;

    // 排序（list有自己的sort成员函数，因为不能随机访问）
    lst.sort();
    cout << "   sort() 排序后: ";
    for (int n : lst) cout << n << " ";
    cout << endl;

    // 去重（必须先排序）
    lst.unique();
    cout << "   unique() 去重后: ";
    for (int n : lst) cout << n << " ";
    cout << endl;

    // 不能用下标访问，只能用迭代器遍历
    cout << "   注意: list不支持随机访问（不能用 []）" << endl;

    // ------------------------------------------------------------------------
    // 3. deque（双端队列）
    // 特点:
    //   - 双端都可以高效插入删除（O(1)）
    //   - 支持随机访问（O(1)，但比vector稍慢）
    //   - 内部是分段连续存储（中控器+缓冲区）
    //   - 没有 capacity 和 reserve（不连续存储）
    // 适用场景: 双端操作频繁，如队列、滑动窗口
    // ------------------------------------------------------------------------
    cout << "\n3. deque (双端队列):" << endl;

    deque<int> dq = {2, 3, 4};
    cout << "   初始 deque: ";
    for (int n : dq) cout << n << " ";
    cout << endl;

    dq.push_front(1);       // 头部插入
    dq.push_back(5);        // 尾部插入
    cout << "   push_front(1), push_back(5): ";
    for (int n : dq) cout << n << " ";
    cout << endl;

    dq.pop_front();         // 头部删除
    dq.pop_back();          // 尾部删除
    cout << "   pop_front(), pop_back(): ";
    for (int n : dq) cout << n << " ";
    cout << endl;

    cout << "   dq[1] = " << dq[1] << " (支持随机访问)" << endl;
    cout << "   deque 没有 capacity() 和 reserve()" << endl;

    // ------------------------------------------------------------------------
    // 4. array（固定大小数组）
    // 特点:
    //   - 封装了C风格数组，大小在编译时确定
    //   - 支持STL容器接口（begin/end/size等）
    //   - 栈上分配，性能与原始数组一样
    //   - 大小不能改变
    // 适用场景: 大小固定的小数组，替代原始数组
    // ------------------------------------------------------------------------
    cout << "\n4. array (固定大小数组) (C++11):" << endl;

    array<int, 5> arr = {5, 4, 3, 2, 1};  // 大小是模板参数，编译时确定
    cout << "   初始 array: ";
    for (int n : arr) cout << n << " ";
    cout << endl;

    cout << "   arr.size() = " << arr.size() << " (编译时确定，不能改变)" << endl;
    cout << "   arr[2] = " << arr[2] << endl;
    cout << "   arr.front() = " << arr.front() << endl;
    cout << "   arr.back() = " << arr.back() << endl;

    // 可以直接访问底层数组
    int* raw_ptr = arr.data();
    cout << "   arr.data() 返回原始指针: " << raw_ptr[0] << endl;

    // 与C风格数组对比
    // C风格: int carr[5] = {5,4,3,2,1};
    // 缺点: 不知道大小，不支持赋值，传递给函数时退化为指针
    cout << "   对比C风格数组: array知道大小，支持赋值，有STL接口" << endl;
}

// ============================================================================
// 函数: demo_associative_containers
// 描述: 演示关联容器：map, unordered_map, set, unordered_set
// ============================================================================
void demo_associative_containers() {
    cout << "\n========== 关联容器 (Associative Containers) ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. map（有序映射）
    // 特点:
    //   - 键值对存储（key-value）
    //   - 按键排序（默认升序，红黑树实现）
    //   - 查找、插入、删除都是 O(log n)
    //   - 键唯一，不能重复
    // 适用场景: 需要按键排序，或者需要范围查找
    // ------------------------------------------------------------------------
    cout << "\n1. map (有序映射，红黑树实现):" << endl;

    // 创建map
    std::map<string, int> scores;

    // 插入元素的几种方式
    scores["Alice"] = 95;              // 使用[]运算符插入或修改
    scores.insert(std::make_pair("Bob", 87));  // insert插入
    scores.emplace("Charlie", 92);     // emplace插入（C++11，更高效）

    cout << "   插入三个学生成绩:" << endl;
    for (const auto& pair : scores) {
        cout << "     " << pair.first << ": " << pair.second << endl;
    }
    cout << "   (注意: map按键自动排序，所以顺序是 Alice, Bob, Charlie)" << endl;

    // 查找元素
    auto it = scores.find("Bob");
    if (it != scores.end()) {
        cout << "   find(\"Bob\"): 找到，分数 = " << it->second << endl;
    } else {
        cout << "   find(\"Bob\"): 未找到" << endl;
    }

    // [] 运算符的副作用：如果key不存在，会自动插入默认值
    // 所以查找时最好用 find()，不要用 []
    cout << "   scores[\"David\"] = " << scores["David"]
         << " (注意: []会自动插入默认值0)" << endl;

    // 删除元素
    scores.erase("David");
    cout << "   erase(\"David\") 后，size = " << scores.size() << endl;

    // 范围查找（map的有序特性）
    cout << "   map的优势: 支持范围查找（按key区间遍历）" << endl;

    // ------------------------------------------------------------------------
    // 2. unordered_map（无序映射）
    // 特点:
    //   - 键值对存储，无序
    //   - 哈希表实现，平均 O(1) 查找
    //   - 最坏情况 O(n)（哈希冲突严重时）
    //   - C++11 引入
    // 适用场景: 只需要查找，不需要排序，追求性能
    // ------------------------------------------------------------------------
    cout << "\n2. unordered_map (无序映射，哈希表实现) (C++11):" << endl;

    std::unordered_map<string, int> hash_scores;
    hash_scores["Alice"] = 95;
    hash_scores["Bob"] = 87;
    hash_scores["Charlie"] = 92;

    cout << "   三个学生成绩 (无序):" << endl;
    for (const auto& pair : hash_scores) {
        cout << "     " << pair.first << ": " << pair.second << endl;
    }
    cout << "   (注意: 输出顺序不确定，取决于哈希函数和桶的数量)" << endl;

    // 哈希表相关信息
    cout << "   bucket_count() = " << hash_scores.bucket_count()
         << " (桶的数量)" << endl;
    cout << "   load_factor() = " << hash_scores.load_factor()
         << " (负载因子 = 元素数 / 桶数)" << endl;

    cout << "   unordered_map vs map:" << endl;
    cout << "     - 查找性能: unordered_map 平均O(1) > map O(log n)" << endl;
    cout << "     - 有序性: map 有序，unordered_map 无序" << endl;
    cout << "     - 内存: unordered_map 通常占用更多" << endl;

    // ------------------------------------------------------------------------
    // 3. set（有序集合）
    // 特点:
    //   - 只有键，没有值（或者说键就是值）
    //   - 元素不重复
    //   - 红黑树实现，有序
    // 适用场景: 去重、集合运算
    // ------------------------------------------------------------------------
    cout << "\n3. set (有序集合):" << endl;

    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    cout << "   初始: {3,1,4,1,5,9,2,6,5,3,5}，存入set后: ";
    for (int n : s) cout << n << " ";
    cout << endl;
    cout << "   (自动去重，自动排序，size = " << s.size() << ")" << endl;

    // 插入元素
    s.insert(7);
    s.insert(7);            // 重复插入，不会有效果
    cout << "   insert(7) 两次后: ";
    for (int n : s) cout << n << " ";
    cout << " (7只出现一次)" << endl;

    // 查找元素
    if (s.count(5)) {       // count返回0或1，用于判断是否存在
        cout << "   set中存在元素5" << endl;
    }

    // ------------------------------------------------------------------------
    // 4. unordered_set（无序集合）
    // 特点:
    //   - 哈希表实现，无序
    //   - 平均 O(1) 查找
    // ------------------------------------------------------------------------
    cout << "\n4. unordered_set (无序集合) (C++11):" << endl;

    std::unordered_set<int> us = {3, 1, 4, 1, 5};
    cout << "   unordered_set 元素: ";
    for (int n : us) cout << n << " ";
    cout << endl;
    cout << "   (无序，自动去重)" << endl;

    cout << "\n5. 关联容器总结:" << endl;
    cout << "                有序(红黑树)    无序(哈希表)" << endl;
    cout << "   键值对       map            unordered_map" << endl;
    cout << "   单值(集合)   set            unordered_set" << endl;
    cout << "   时间复杂度   O(log n)       平均O(1)，最坏O(n)" << endl;
}

// ============================================================================
// 函数: demo_string_operations
// 描述: 演示string类的常用操作
// ============================================================================
void demo_string_operations() {
    cout << "\n========== string 操作 ==========" << endl;

    cout << "C++ string vs C字符串:" << endl;
    cout << "   - C: char* 字符数组，手动管理内存，容易越界" << endl;
    cout << "   - C++: std::string 类，自动管理内存，丰富的成员函数" << endl;

    // 创建字符串
    string s1 = "Hello";              // 拷贝初始化
    string s2("World");               // 直接初始化
    string s3(5, 'A');               // 重复字符
    string s4 = s1 + " " + s2;        // 字符串拼接

    cout << "\n1. 创建与拼接:" << endl;
    cout << "   s1 = \"" << s1 << "\"" << endl;
    cout << "   s2 = \"" << s2 << "\"" << endl;
    cout << "   s3 = string(5, 'A') = \"" << s3 << "\"" << endl;
    cout << "   s4 = s1 + \" \" + s2 = \"" << s4 << "\"" << endl;

    // 访问字符
    cout << "\n2. 访问字符:" << endl;
    cout << "   s1[0] = " << s1[0] << " (下标访问)" << endl;
    cout << "   s1.at(1) = " << s1.at(1) << " (at()访问，越界抛异常)" << endl;
    cout << "   s1.front() = " << s1.front() << " (第一个字符)" << endl;
    cout << "   s1.back() = " << s1.back() << " (最后一个字符)" << endl;

    // 常用属性
    cout << "\n3. 常用属性:" << endl;
    cout << "   s4.size() = " << s4.size() << " (字符串长度)" << endl;
    cout << "   s4.length() = " << s4.length() << " (和size()一样)" << endl;
    cout << "   s4.empty() = " << (s4.empty() ? "true" : "false") << endl;
    cout << "   s4.capacity() = " << s4.capacity() << " (容量)" << endl;

    // 查找
    cout << "\n4. 查找操作:" << endl;
    string text = "Hello World, Hello C++";
    cout << "   text = \"" << text << "\"" << endl;

    // find: 查找第一次出现的位置
    size_t pos = text.find("Hello");
    cout << "   find(\"Hello\") = " << pos << " (第一次出现的位置)" << endl;

    // rfind: 从后往前查找
    pos = text.rfind("Hello");
    cout << "   rfind(\"Hello\") = " << pos << " (最后一次出现的位置)" << endl;

    // find_first_of: 查找任一字符第一次出现的位置
    pos = text.find_first_of("aeiou");
    cout << "   find_first_of(\"aeiou\") = " << pos
         << " (第一个元音字母位置: '" << text[pos] << "')" << endl;

    // 替换
    cout << "\n5. 替换与截取:" << endl;
    string s5 = text;
    s5.replace(0, 5, "Hi");  // 从位置0开始，替换5个字符为"Hi"
    cout << "   replace(0, 5, \"Hi\"): \"" << s5 << "\"" << endl;

    // substr: 截取子串
    string sub = text.substr(6, 5);  // 从位置6开始，截取5个字符
    cout << "   substr(6, 5): \"" << sub << "\"" << endl;

    // 插入和删除
    cout << "\n6. 插入与删除:" << endl;
    string s6 = "Hello World";
    s6.insert(5, " Beautiful");  // 在位置5插入
    cout << "   insert(5, \" Beautiful\"): \"" << s6 << "\"" << endl;

    s6.erase(5, 10);              // 从位置5删除10个字符
    cout << "   erase(5, 10): \"" << s6 << "\"" << endl;

    // 比较
    cout << "\n7. 比较操作:" << endl;
    string a = "apple";
    string b = "banana";
    cout << "   \"apple\" == \"banana\": " << (a == b ? "true" : "false") << endl;
    cout << "   \"apple\" < \"banana\": " << (a < b ? "true" : "false")
         << " (字典序比较)" << endl;

    // 与C字符串互转
    cout << "\n8. 与C字符串互转:" << endl;
    const char* c_str = s1.c_str();  // 返回const char*
    cout << "   s1.c_str() = \"" << c_str << "\"" << endl;
    cout << "   注意: c_str()返回的指针由string管理，不要手动释放" << endl;

    // 数值转换（C++11）
    cout << "\n9. 数值转换 (C++11):" << endl;
    string num_str = "12345";
    int num = std::stoi(num_str);    // string转int
    cout << "   stoi(\"" << num_str << "\") = " << num << endl;

    double dbl = std::stod("3.14");  // string转double
    cout << "   stod(\"3.14\") = " << dbl << endl;

    string back = std::to_string(42);  // 数值转string
    cout << "   to_string(42) = \"" << back << "\"" << endl;
}

// ============================================================================
// 函数: demo_iterators
// 描述: 演示迭代器的使用和迭代器失效问题
// ============================================================================
void demo_iterators() {
    cout << "\n========== 迭代器 (Iterators) ==========" << endl;

    cout << "1. 迭代器的概念:" << endl;
    cout << "   - 类似指针，用于遍历容器中的元素" << endl;
    cout << "   - begin() 返回指向第一个元素的迭代器" << endl;
    cout << "   - end() 返回指向最后一个元素之后位置的迭代器（尾后迭代器）" << endl;
    cout << "   - 所有STL容器都支持迭代器，算法通过迭代器操作容器" << endl;

    // 基本用法
    vector<int> vec = {10, 20, 30, 40, 50};
    cout << "\n2. 基本遍历:" << endl;
    cout << "   使用迭代器遍历: ";
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";       // 解引用迭代器获取元素
    }
    cout << endl;

    // C++11: auto 简化迭代器声明
    cout << "   使用auto简化: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // const_iterator: 只读迭代器
    cout << "   const_iterator (只读): ";
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        // *it = 100;  // 错误：const迭代器不能修改元素
        cout << *it << " ";
    }
    cout << endl;

    // 反向迭代器
    cout << "   反向迭代器 (从后往前): ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 迭代器分类
    cout << "\n3. 迭代器分类（功能从弱到强）:" << endl;
    cout << "   - 输入迭代器: 只能读，只能向前（单遍扫描）" << endl;
    cout << "   - 输出迭代器: 只能写，只能向前（单遍扫描）" << endl;
    cout << "   - 前向迭代器: 可读写，只能向前（多遍扫描）" << endl;
    cout << "   - 双向迭代器: 可读写，可前后移动（list, map, set）" << endl;
    cout << "   - 随机访问迭代器: 可读写，可任意跳转（vector, deque, array, string）" << endl;

    // 迭代器运算（随机访问迭代器支持）
    cout << "\n4. 随机访问迭代器运算 (vector):" << endl;
    auto it = vec.begin();
    cout << "   it = begin(), *it = " << *it << endl;
    cout << "   it + 2 = " << *(it + 2) << " (可以直接加数字)" << endl;
    cout << "   it[3] = " << it[3] << " (支持下标访问)" << endl;
    cout << "   end() - begin() = " << (vec.end() - vec.begin())
         << " (可以相减得到距离)" << endl;

    // 迭代器失效
    cout << "\n5. 迭代器失效问题:" << endl;
    cout << "   - 修改容器结构（插入、删除、扩容）可能导致迭代器失效" << endl;
    cout << "   - vector 插入导致扩容时，所有迭代器失效" << endl;
    cout << "   - vector 中间插入删除时，插入点之后的迭代器失效" << endl;
    cout << "   - list 插入删除时，只有指向被删元素的迭代器失效" << endl;
    cout << "   - 正确做法: 使用操作返回的迭代器" << endl;

    // 正确遍历删除的示例
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "\n   正确的遍历删除示例（删除所有偶数）:" << endl;
    cout << "   原数组: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // 正确方式：使用 erase 的返回值
    for (auto it = nums.begin(); it != nums.end(); ) {
        if (*it % 2 == 0) {
            it = nums.erase(it);  // erase返回下一个有效迭代器
        } else {
            ++it;                 // 不删除时才递增
        }
    }

    cout << "   删除偶数后: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    cout << "   错误写法: for循环中++it，erase后迭代器已失效" << endl;
}

// ============================================================================
// 函数: demo_algorithms
// 描述: 演示常用STL算法
// ============================================================================
void demo_algorithms() {
    cout << "\n========== STL 算法 (Algorithms) ==========" << endl;

    cout << "STL算法的特点:" << endl;
    cout << "   - 通用算法，不依赖具体容器" << endl;
    cout << "   - 通过迭代器操作容器，算法和容器分离" << endl;
    cout << "   - 头文件: <algorithm>（大多数）, <numeric>（数值算法）" << endl;

    vector<int> vec = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    cout << "\n原始数据: ";
    for (int n : vec) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 1. sort: 排序
    // 原型: sort(begin, end, comp)
    // 复杂度: O(n log n)
    // 要求: 随机访问迭代器
    // ------------------------------------------------------------------------
    cout << "\n1. sort 排序:" << endl;

    vector<int> v1 = vec;
    std::sort(v1.begin(), v1.end());  // 默认升序
    cout << "   sort (升序): ";
    for (int n : v1) cout << n << " ";
    cout << endl;

    vector<int> v2 = vec;
    std::sort(v2.begin(), v2.end(), std::greater<int>());  // 降序
    cout << "   sort (降序): ";
    for (int n : v2) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 2. find / find_if: 查找
    // 原型: find(begin, end, value)
    // 复杂度: O(n)
    // ------------------------------------------------------------------------
    cout << "\n2. find / find_if 查找:" << endl;

    auto it = std::find(vec.begin(), vec.end(), 7);
    if (it != vec.end()) {
        cout << "   find(7): 找到，位置 " << (it - vec.begin()) << endl;
    }

    // find_if: 按条件查找（第一个大于5的元素）
    it = std::find_if(vec.begin(), vec.end(),
                       [](int x) { return x > 5; });
    if (it != vec.end()) {
        cout << "   find_if(>5): 找到 " << *it
             << "，位置 " << (it - vec.begin()) << endl;
    }

    // ------------------------------------------------------------------------
    // 3. copy: 复制
    // 原型: copy(src_begin, src_end, dest_begin)
    // 注意: 目标容器必须有足够空间
    // ------------------------------------------------------------------------
    cout << "\n3. copy 复制:" << endl;

    vector<int> dest(10);  // 先分配空间
    std::copy(vec.begin(), vec.end(), dest.begin());
    cout << "   copy 到 dest: ";
    for (int n : dest) cout << n << " ";
    cout << endl;

    // back_inserter: 插入迭代器，自动调用push_back
    vector<int> dest2;
    std::copy(vec.begin(), vec.end(), std::back_inserter(dest2));
    cout << "   back_inserter 复制: ";
    for (int n : dest2) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 4. transform: 变换
    // 原型: transform(src_begin, src_end, dest_begin, func)
    // 作用: 对每个元素应用函数，结果存入目标容器
    // ------------------------------------------------------------------------
    cout << "\n4. transform 变换:" << endl;

    vector<int> transformed;
    // 每个元素乘2
    std::transform(vec.begin(), vec.end(),
                   std::back_inserter(transformed),
                   [](int x) { return x * 2; });
    cout << "   每个元素乘2: ";
    for (int n : transformed) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 5. for_each: 遍历
    // 原型: for_each(begin, end, func)
    // 作用: 对每个元素执行函数
    // ------------------------------------------------------------------------
    cout << "\n5. for_each 遍历:" << endl;
    cout << "   打印每个元素: ";
    std::for_each(vec.begin(), vec.end(),
                   [](int x) { cout << x << " "; });
    cout << endl;

    // ------------------------------------------------------------------------
    // 6. accumulate: 累加 (在 <numeric> 中)
    // 原型: accumulate(begin, end, init)
    // 作用: 计算所有元素的和（加上初始值）
    // ------------------------------------------------------------------------
    cout << "\n6. accumulate 累加:" << endl;
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    cout << "   所有元素之和 = " << sum << endl;

    // 自定义累积操作
    int product = std::accumulate(vec.begin(), vec.end(), 1,
                                   std::multiplies<int>());
    cout << "   所有元素之积 = " << product << endl;

    // ------------------------------------------------------------------------
    // 7. remove / remove_if: 移除
    // 注意: remove 不会真的删除元素，只是把要保留的移到前面
    //       真正删除需要配合 erase（erase-remove 惯用法）
    // ------------------------------------------------------------------------
    cout << "\n7. remove / erase-remove 惯用法:" << endl;

    vector<int> v3 = vec;
    cout << "   原始: ";
    for (int n : v3) cout << n << " ";
    cout << endl;

    // remove 返回新的逻辑末尾
    auto new_end = std::remove(v3.begin(), v3.end(), 5);
    cout << "   remove(5) 后 (未真正删除): ";
    for (int n : v3) cout << n << " ";
    cout << " (后面是残留数据)" << endl;

    // erase-remove 惯用法：真正删除元素
    v3.erase(new_end, v3.end());
    cout << "   erase 后 (真正删除): ";
    for (int n : v3) cout << n << " ";
    cout << endl;

    // remove_if: 按条件删除
    vector<int> v4 = vec;
    v4.erase(std::remove_if(v4.begin(), v4.end(),
                              [](int x) { return x % 2 == 0; }),
             v4.end());
    cout << "   remove_if 删除偶数后: ";
    for (int n : v4) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 8. reverse: 反转
    // ------------------------------------------------------------------------
    cout << "\n8. reverse 反转:" << endl;
    vector<int> v5 = vec;
    std::reverse(v5.begin(), v5.end());
    cout << "   反转后: ";
    for (int n : v5) cout << n << " ";
    cout << endl;

    // ------------------------------------------------------------------------
    // 9. count / count_if: 计数
    // ------------------------------------------------------------------------
    cout << "\n9. count / count_if 计数:" << endl;
    int cnt = std::count(vec.begin(), vec.end(), 5);
    cout << "   count(5) = " << cnt << endl;

    int cnt_if = std::count_if(vec.begin(), vec.end(),
                                [](int x) { return x > 5; });
    cout << "   count_if(>5) = " << cnt_if << endl;

    cout << "\n10. STL算法分类总结:" << endl;
    cout << "    - 非修改式: for_each, find, count, ..." << endl;
    cout << "    - 修改式: copy, transform, fill, replace, ..." << endl;
    cout << "    - 排序: sort, stable_sort, partial_sort, ..." << endl;
    cout << "    - 二分查找: binary_search, lower_bound, upper_bound" << endl;
    cout << "    - 集合: set_union, set_intersection, ..." << endl;
    cout << "    - 数值: accumulate, inner_product, ..." << endl;
}

// ============================================================================
// 函数: run_stl_demo
// 描述: 运行所有STL演示（统一入口）
// ============================================================================
void run_stl_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "  C++ STL 容器与算法演示" << endl;
    cout << string(60, '=') << endl;

    demo_sequence_containers();
    demo_associative_containers();
    demo_string_operations();
    demo_iterators();
    demo_algorithms();

    cout << "\n" << string(60, '=') << endl;
    cout << "  STL容器与算法演示结束" << endl;
    cout << string(60, '=') << endl;
}

} // namespace stl_demo
