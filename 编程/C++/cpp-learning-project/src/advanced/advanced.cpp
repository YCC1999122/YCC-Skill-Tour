// ============================================================================
// 文件名: advanced.cpp
// 描述: C++高级特性模块实现文件
// 作者: C++学习项目
// 说明: 本文件实现了高级特性的各种演示功能，每行代码都有详细中文注释
// ============================================================================

#include "advanced.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <typeinfo>   // 用于 typeid 查看类型

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

namespace advanced {

// ============================================================================
// 函数: demo_smart_pointers
// 描述: 演示智能指针的使用
// ============================================================================
void demo_smart_pointers() {
    cout << "\n========== 智能指针 (Smart Pointers) ==========" << endl;

    cout << "为什么需要智能指针?" << endl;
    cout << "   - C语言/C++手动管理内存: new/delete, malloc/free" << endl;
    cout << "   - 容易忘记释放 -> 内存泄漏" << endl;
    cout << "   - 异常时可能跳过delete -> 内存泄漏" << endl;
    cout << "   - 多次释放 -> 程序崩溃" << endl;
    cout << "   - 智能指针利用RAII，自动管理内存生命周期" << endl;

    // ------------------------------------------------------------------------
    // 1. unique_ptr（独占智能指针）
    // 特点:
    //   - 独占所有权，同一时间只能有一个unique_ptr指向该对象
    //   - 不可复制，只能移动
    //   - 轻量级，开销很小（和裸指针差不多）
    //   - 析构时自动delete指向的对象
    // 适用场景: 大多数场景的首选，明确单一所有权
    // ------------------------------------------------------------------------
    cout << "\n1. unique_ptr (独占所有权) (C++11):" << endl;

    // 创建方式1: 使用 new 直接初始化
    unique_ptr<int> p1(new int(42));
    cout << "   unique_ptr<int> p1(new int(42));" << endl;
    cout << "   *p1 = " << *p1 << endl;

    // 创建方式2: 使用 make_unique（推荐，C++14）
    // 更安全（异常安全），更高效，写法更简洁
    auto p2 = make_unique<int>(100);
    cout << "   auto p2 = make_unique<int>(100);  // 推荐方式" << endl;
    cout << "   *p2 = " << *p2 << endl;

    // unique_ptr 不可复制（编译错误）
    // unique_ptr<int> p3 = p1;  // 错误：拷贝构造被删除

    // unique_ptr 可以移动（转移所有权）
    cout << "\n   移动语义（转移所有权）:" << endl;
    cout << "   移动前: p1 = " << (p1 ? "非空" : "空")
         << ", *p1 = " << *p1 << endl;
    unique_ptr<int> p3 = std::move(p1);  // 所有权从p1转移到p3
    cout << "   unique_ptr<int> p3 = std::move(p1);" << endl;
    cout << "   移动后: p1 = " << (p1 ? "非空" : "空")
         << " (p1失去所有权)" << endl;
    cout << "   移动后: p3 = " << (p3 ? "非空" : "空")
         << ", *p3 = " << *p3 << " (p3获得所有权)" << endl;

    // 自定义删除器
    cout << "\n   unique_ptr 的优势:" << endl;
    cout << "   - 轻量，几乎零开销" << endl;
    cout << "   - 明确表达所有权语义" << endl;
    cout << "   - 自动释放内存，防止泄漏" << endl;

    // ------------------------------------------------------------------------
    // 2. shared_ptr（共享智能指针）
    // 特点:
    //   - 共享所有权，多个shared_ptr可以指向同一个对象
    //   - 使用引用计数，最后一个shared_ptr销毁时释放对象
    //   - 有额外开销（引用计数的原子操作，控制块内存）
    // 适用场景: 需要多个指针共享同一对象的所有权
    // ------------------------------------------------------------------------
    cout << "\n2. shared_ptr (共享所有权) (C++11):" << endl;

    // 创建方式: 使用 make_shared（推荐）
    auto sp1 = make_shared<int>(200);
    cout << "   auto sp1 = make_shared<int>(200);" << endl;
    cout << "   *sp1 = " << *sp1 << endl;
    cout << "   sp1.use_count() = " << sp1.use_count()
         << " (引用计数)" << endl;

    // 拷贝 shared_ptr，引用计数+1
    shared_ptr<int> sp2 = sp1;
    cout << "\n   shared_ptr<int> sp2 = sp1;  // 拷贝，引用计数+1" << endl;
    cout << "   sp1.use_count() = " << sp1.use_count() << endl;
    cout << "   sp2.use_count() = " << sp2.use_count() << endl;

    // 再创建一个
    shared_ptr<int> sp3 = sp1;
    cout << "   再创建sp3后:" << endl;
    cout << "   sp1.use_count() = " << sp1.use_count() << endl;

    // 重置一个shared_ptr
    sp2.reset();              // sp2释放所有权，引用计数-1
    cout << "\n   sp2.reset();  // 释放所有权" << endl;
    cout << "   sp1.use_count() = " << sp1.use_count() << endl;
    cout << "   sp2 = " << (sp2 ? "非空" : "空") << endl;

    cout << "\n   shared_ptr 的工作原理:" << endl;
    cout << "   - 每个shared_ptr对象包含两个指针:" << endl;
    cout << "     1. 指向对象的指针" << endl;
    cout << "     2. 指向控制块的指针（包含引用计数、删除器等）" << endl;
    cout << "   - 拷贝时引用计数原子+1" << endl;
    cout << "   - 销毁时引用计数原子-1，减到0时释放对象" << endl;

    // ------------------------------------------------------------------------
    // 3. weak_ptr（弱引用指针）
    // 特点:
    //   - 不拥有对象，不增加引用计数
    //   - 用于观察shared_ptr管理的对象
    //   - 可以判断对象是否还存在
    //   - 不能直接访问对象，需要先lock()升级为shared_ptr
    // 适用场景: 打破循环引用、缓存观察者模式
    // ------------------------------------------------------------------------
    cout << "\n3. weak_ptr (弱引用) (C++11):" << endl;

    auto sp = make_shared<int>(300);
    weak_ptr<int> wp = sp;    // weak_ptr 不增加引用计数

    cout << "   auto sp = make_shared<int>(300);" << endl;
    cout << "   weak_ptr<int> wp = sp;" << endl;
    cout << "   sp.use_count() = " << sp.use_count()
         << " (weak_ptr不增加引用计数)" << endl;

    // 使用 lock() 获取 shared_ptr
    if (auto tmp = wp.lock()) {
        cout << "   wp.lock() 成功，*tmp = " << *tmp << endl;
    } else {
        cout << "   wp.lock() 失败，对象已释放" << endl;
    }

    // 释放shared_ptr后，weak_ptr可以检测到
    sp.reset();
    cout << "   sp.reset();  // 释放对象" << endl;
    if (auto tmp = wp.lock()) {
        cout << "   wp.lock() 成功" << endl;
    } else {
        cout << "   wp.lock() 失败，对象已释放 (expired = "
             << wp.expired() << ")" << endl;
    }

    cout << "\n   weak_ptr 的用途:" << endl;
    cout << "   - 打破 shared_ptr 的循环引用" << endl;
    cout << "   - 观察者模式（不影响被观察者生命周期）" << endl;
    cout << "   - 缓存（对象可能被释放，需要先检查）" << endl;

    // ------------------------------------------------------------------------
    // 4. 循环引用问题
    // ------------------------------------------------------------------------
    cout << "\n4. 循环引用问题 (shared_ptr的陷阱):" << endl;
    cout << "   两个对象互相用shared_ptr指向对方" << endl;
    cout << "   -> 引用计数永远不会到0" << endl;
    cout << "   -> 内存泄漏" << endl;
    cout << "   解决方法: 一方使用weak_ptr" << endl;

    cout << "\n5. 智能指针使用建议:" << endl;
    cout << "   - 优先使用 unique_ptr，需要共享时用 shared_ptr" << endl;
    cout << "   - 优先使用 make_unique / make_shared 创建" << endl;
    cout << "   - 避免手动 new/delete" << endl;
    cout << "   - 不要用同一个裸指针初始化多个智能指针" << endl;
}

// ============================================================================
// 函数: demo_lambda
// 描述: 演示Lambda表达式与闭包
// ============================================================================
void demo_lambda() {
    cout << "\n========== Lambda 表达式 (C++11) ==========" << endl;

    cout << "什么是Lambda?" << endl;
    cout << "   - 匿名函数对象，可以内联定义和使用" << endl;
    cout << "   - 语法: [捕获列表](参数列表) mutable -> 返回类型 { 函数体 }" << endl;
    cout << "   - 编译器会为每个lambda生成一个唯一的类（闭包类）" << endl;
    cout << "   - lambda对象称为闭包（closure）" << endl;

    // ------------------------------------------------------------------------
    // 1. 基本语法
    // ------------------------------------------------------------------------
    cout << "\n1. 基本语法:" << endl;

    // 最简单的lambda：无参数，无返回值
    auto hello = []() {
        cout << "   Hello, Lambda!" << endl;
    };
    hello();

    // 有参数和返回值
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "   add(3, 4) = " << add(3, 4) << endl;

    // 返回类型可以省略（自动推导，C++14起完全支持）
    auto multiply = [](int a, int b) {
        return a * b;
    };
    cout << "   multiply(3, 4) = " << multiply(3, 4) << endl;

    // ------------------------------------------------------------------------
    // 2. 捕获列表
    // 作用: lambda可以捕获外部作用域的变量
    // 方式:
    //   []        - 不捕获任何变量
    //   [=]       - 值捕获所有外部变量
    //   [&]       - 引用捕获所有外部变量
    //   [x, &y]   - x值捕获，y引用捕获
    //   [=, &y]   - 默认值捕获，y例外用引用
    //   [&, x]    - 默认引用捕获，x例外用值
    //   [this]    - 捕获this指针（成员函数中使用）
    // ------------------------------------------------------------------------
    cout << "\n2. 捕获列表:" << endl;

    int x = 10;
    int y = 20;

    // 值捕获：拷贝外部变量，lambda内修改不影响外部
    auto value_capture = [x]() {
        cout << "   值捕获: x = " << x << " (是外部x的拷贝)" << endl;
        // x = 100;  // 错误：值捕获的变量默认是const的
    };
    value_capture();

    // 引用捕获：引用外部变量，修改会影响外部
    auto ref_capture = [&y]() {
        y = 200;
        cout << "   引用捕获: 修改y为200" << endl;
    };
    ref_capture();
    cout << "   外部y现在 = " << y << endl;

    // mutable 关键字：允许修改值捕获的变量（修改的是拷贝，不影响外部）
    auto mutable_lambda = [x]() mutable {
        x = 999;  // 可以修改，因为有mutable
        cout << "   mutable lambda: x(拷贝) = " << x << endl;
    };
    mutable_lambda();
    cout << "   外部x仍然 = " << x << " (mutable修改的是内部拷贝)" << endl;

    // 混合捕获
    int a = 1, b = 2, c = 3;
    auto mixed = [a, &b, &c]() {
        cout << "   混合捕获: a(值)=" << a
             << ", b(引用)=" << b
             << ", c(引用)=" << c << endl;
    };
    mixed();

    // 默认捕获
    auto default_val = [=]() {  // 所有外部变量值捕获
        cout << "   [=]默认值捕获: x=" << x << ", y=" << y << endl;
    };
    default_val();

    // ------------------------------------------------------------------------
    // 3. Lambda 作为算法参数
    // 这是lambda最常用的场景
    // ------------------------------------------------------------------------
    cout << "\n3. Lambda 作为STL算法参数:" << endl;

    vector<int> nums = {5, 2, 8, 1, 9, 3};
    cout << "   原始数据: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // 自定义排序（降序）
    std::sort(nums.begin(), nums.end(),
              [](int a, int b) { return a > b; });
    cout << "   降序排序: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // 条件查找（第一个大于5的数）
    auto it = std::find_if(nums.begin(), nums.end(),
                            [](int n) { return n > 5; });
    if (it != nums.end()) {
        cout << "   第一个大于5的数: " << *it << endl;
    }

    // 计数（偶数的个数）
    int even_count = std::count_if(nums.begin(), nums.end(),
                                    [](int n) { return n % 2 == 0; });
    cout << "   偶数个数: " << even_count << endl;

    // ------------------------------------------------------------------------
    // 4. 泛型Lambda (C++14)
    // 使用 auto 作为参数类型，相当于模板
    // ------------------------------------------------------------------------
    cout << "\n4. 泛型Lambda (C++14):" << endl;

    auto generic_add = [](auto a, auto b) {
        return a + b;
    };

    cout << "   generic_add(3, 4) = " << generic_add(3, 4) << " (int)" << endl;
    cout << "   generic_add(3.14, 2.72) = "
         << generic_add(3.14, 2.72) << " (double)" << endl;
    cout << "   generic_add(string(\"hello \"), string(\"world\")) = "
         << generic_add(string("hello "), string("world")) << " (string)" << endl;
    cout << "   相当于模板函数，编译器为每种参数类型生成一个实例" << endl;

    // ------------------------------------------------------------------------
    // 5. 闭包的概念
    // ------------------------------------------------------------------------
    cout << "\n5. 闭包（Closure）的概念:" << endl;
    cout << "   - lambda表达式创建的函数对象称为闭包" << endl;
    cout << "   - 闭包 = 函数 + 环境（捕获的变量）" << endl;
    cout << "   - 闭包可以像普通对象一样传递、存储、调用" << endl;
    cout << "   - 本质: 编译器生成一个匿名类，重载operator()" << endl;

    // 闭包的典型应用：状态保持
    auto counter = [count = 0]() mutable {
        return ++count;
    };
    cout << "   状态保持闭包（计数器）:" << endl;
    cout << "   counter() = " << counter() << endl;
    cout << "   counter() = " << counter() << endl;
    cout << "   counter() = " << counter() << endl;
    cout << "   (闭包内部的count变量在调用之间保持状态)" << endl;
}

// ============================================================================
// 函数: demo_move_semantics
// 描述: 演示移动语义与转移构造
// ============================================================================
void demo_move_semantics() {
    cout << "\n========== 移动语义 (Move Semantics) (C++11) ==========" << endl;

    cout << "为什么需要移动语义?" << endl;
    cout << "   - 传统拷贝构造是深拷贝，对于大对象开销很大" << endl;
    cout << "   - 很多时候拷贝的源是临时对象（右值），用完就销毁" << endl;
    cout << "   - 既然源对象马上要销毁，不如直接'偷'它的资源" << endl;
    cout << "   - 移动语义就是做这件事：转移资源所有权，避免深拷贝" << endl;

    // ------------------------------------------------------------------------
    // 1. 左值与右值的再理解
    // ------------------------------------------------------------------------
    cout << "\n1. 左值 vs 右值:" << endl;
    cout << "   左值 (lvalue): 有身份（可以取地址）、可以移动" << endl;
    cout << "   右值 (rvalue): 没有身份（临时对象）、将要被移动" << endl;
    cout << "   简单判断: 可以用&取地址的是左值，否则是右值" << endl;

    int a = 10;               // a 是左值，10 是右值
    cout << "   int a = 10;  // a是左值，10是右值（字面量）" << endl;
    cout << "   &a = " << &a << " (左值可以取地址)" << endl;
    // &10;  // 错误：右值不能取地址

    // 函数返回值是右值（临时对象）
    cout << "   函数返回值、临时对象都是右值" << endl;

    // ------------------------------------------------------------------------
    // 2. 移动构造和移动赋值
    // 用SimpleStack类模板演示
    // ------------------------------------------------------------------------
    cout << "\n2. 移动构造演示 (SimpleStack<int>):" << endl;

    // 创建一个栈
    SimpleStack<int> s1(5);
    for (int i = 1; i <= 3; ++i) {
        s1.push(i * 10);
    }
    cout << "   s1 创建完成，size = " << s1.size() << endl;

    // 拷贝构造（深拷贝）
    SimpleStack<int> s2 = s1;
    cout << "   s2 拷贝自 s1，size = " << s2.size()
         << " (深拷贝，数据完全独立)" << endl;

    // 移动构造（转移资源）
    cout << "\n   SimpleStack<int> s3 = std::move(s1);  // 移动构造" << endl;
    SimpleStack<int> s3 = std::move(s1);
    cout << "   移动后:" << endl;
    cout << "   s3.size() = " << s3.size()
         << " (s3获得了s1的资源)" << endl;
    cout << "   s1.isEmpty() = " << (s1.isEmpty() ? "true" : "false")
         << " (s1的资源被移走了)" << endl;

    cout << "\n   移动构造 vs 拷贝构造:" << endl;
    cout << "   - 拷贝构造: 分配新内存，复制所有数据 (O(n))" << endl;
    cout << "   - 移动构造: 只复制指针，源对象指针置空 (O(1))" << endl;
    cout << "   - 对于大对象，移动性能提升非常明显" << endl;

    // ------------------------------------------------------------------------
    // 3. std::move 的作用
    // ------------------------------------------------------------------------
    cout << "\n3. std::move 的作用:" << endl;
    cout << "   - std::move 并不移动任何东西！" << endl;
    cout << "   - 它只是将左值转换为右值引用（类型转换）" << endl;
    cout << "   - 转换为右值引用后，编译器会选择移动构造/移动赋值" << endl;
    cout << "   - 真正的移动操作在移动构造函数/移动赋值运算符中" << endl;

    int move_x = 42;
    cout << "\n   int move_x = 42;  // move_x 是左值" << endl;
    cout << "   std::move(move_x) 将其转换为右值引用" << endl;
    cout << "   注意: 调用std::move后，不应该再使用move_x的值" << endl;
    cout << "   (除非你明确知道移动后的状态)" << endl;

    // ------------------------------------------------------------------------
    // 4. noexcept 关键字
    // ------------------------------------------------------------------------
    cout << "\n4. noexcept 关键字:" << endl;
    cout << "   - 声明函数不会抛出异常" << endl;
    cout << "   - 移动构造函数通常应该声明为noexcept" << endl;
    cout << "   - 原因: STL容器（如vector）在扩容时，" << endl;
    cout << "     只有移动构造是noexcept的才会使用移动，否则使用拷贝" << endl;
    cout << "     （因为移动过程中抛异常会导致资源泄漏）" << endl;

    // ------------------------------------------------------------------------
    // 5. 移动语义的应用场景
    // ------------------------------------------------------------------------
    cout << "\n5. 移动语义的典型应用场景:" << endl;
    cout << "   - 函数返回大对象（现在编译器会做RVO/NRVO优化，移动是兜底）" << endl;
    cout << "   - 容器元素插入（插入临时对象时用移动）" << endl;
    cout << "   - 所有权转移（如unique_ptr的设计）" << endl;
    cout << "   - 算法中交换对象（std::swap利用移动）" << endl;

    // vector的push_back和emplace_back
    cout << "\n6. vector的push_back vs emplace_back:" << endl;
    cout << "   - push_back: 先构造临时对象，再移动/拷贝到容器中" << endl;
    cout << "   - emplace_back: 直接在容器内存中构造对象，零拷贝" << endl;
    cout << "   - emplace_back 通常更高效（C++11引入）" << endl;
}

// ============================================================================
// 函数: demo_templates
// 描述: 演示模板编程
// ============================================================================
void demo_templates() {
    cout << "\n========== 模板 (Templates) ==========" << endl;

    cout << "什么是模板?" << endl;
    cout << "   - 类型参数化，编写与类型无关的通用代码" << endl;
    cout << "   - 编译时生成具体类型的代码（实例化）" << endl;
    cout << "   - C++泛型编程的基础" << endl;
    cout << "   - 对比C语言: C中只能用void*或宏，不安全也不直观" << endl;

    // ------------------------------------------------------------------------
    // 1. 函数模板
    // ------------------------------------------------------------------------
    cout << "\n1. 函数模板:" << endl;

    // 函数模板实例化：编译器根据参数类型推导模板参数
    int ia = 3, ib = 7;
    cout << "   int: my_swap(ia=" << ia << ", ib=" << ib << ") -> ";
    my_swap(ia, ib);
    cout << "ia=" << ia << ", ib=" << ib << endl;

    double da = 3.14, db = 2.72;
    cout << "   double: my_swap(da=" << da << ", db=" << db << ") -> ";
    my_swap(da, db);
    cout << "da=" << da << ", db=" << db << endl;

    string sa = "hello", sb = "world";
    cout << "   string: my_swap(sa=\"" << sa << "\", sb=\"" << sb << "\") -> ";
    my_swap(sa, sb);
    cout << "sa=\"" << sa << "\", sb=\"" << sb << "\"" << endl;

    // get_max 函数模板
    cout << "\n   get_max(3, 7) = " << get_max(3, 7) << endl;
    cout << "   get_max(3.14, 2.72) = " << get_max(3.14, 2.72) << endl;
    cout << "   get_max(string(\"apple\"), string(\"banana\")) = "
         << get_max(string("apple"), string("banana")) << endl;

    cout << "\n   函数模板的原理:" << endl;
    cout << "   - 编译器遇到调用时，根据参数类型实例化具体函数" << endl;
    cout << "   - 每种类型生成一份独立的代码" << endl;
    cout << "   - 这也是为什么模板实现必须放在头文件中" << endl;

    // ------------------------------------------------------------------------
    // 2. 类模板
    // 使用 SimpleStack<T> 类模板演示
    // ------------------------------------------------------------------------
    cout << "\n2. 类模板 (SimpleStack<T>):" << endl;

    // int 类型的栈
    cout << "   SimpleStack<int> int_stack(5):" << endl;
    SimpleStack<int> int_stack(5);
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    cout << "   push 10, 20, 30 后，size = " << int_stack.size() << endl;
    cout << "   top = " << int_stack.peek() << endl;
    cout << "   pop = " << int_stack.pop() << endl;
    cout << "   pop后 size = " << int_stack.size() << endl;

    // string 类型的栈（同一个模板，不同类型）
    cout << "\n   SimpleStack<string> str_stack(3):" << endl;
    SimpleStack<string> str_stack(3);
    str_stack.push("hello");
    str_stack.push("world");
    cout << "   push \"hello\", \"world\" 后，size = " << str_stack.size() << endl;
    cout << "   top = " << str_stack.peek() << endl;

    // ------------------------------------------------------------------------
    // 3. 模板特化
    // 对特定类型提供特殊实现
    // ------------------------------------------------------------------------
    cout << "\n3. 模板特化:" << endl;
    cout << "   - 全特化: 对特定类型提供完全不同的实现" << endl;
    cout << "   - 偏特化: 对部分类型参数提供特殊实现" << endl;
    cout << "   - 用途: 优化某些类型的实现，或处理特殊情况" << endl;

    // 这里演示一个简单的特化概念
    cout << "   示例: 对于指针类型，可能需要特化处理（深拷贝 vs 浅拷贝）" << endl;
    cout << "   语法: template<> class ClassName<SpecialType> { ... }" << endl;

    // ------------------------------------------------------------------------
    // 4. STL中的模板
    // ------------------------------------------------------------------------
    cout << "\n4. STL就是模板的典型应用:" << endl;
    cout << "   - 容器: vector<T>, list<T>, map<K,V>, ..." << endl;
    cout << "   - 算法: sort, find, copy, ..." << endl;
    cout << "   - 迭代器: 连接容器和算法的桥梁" << endl;
    cout << "   - 智能指针: unique_ptr<T>, shared_ptr<T>, ..." << endl;

    cout << "\n5. 模板的优缺点:" << endl;
    cout << "   优点:" << endl;
    cout << "     - 类型安全（比void*好）" << endl;
    cout << "     - 代码复用（一套代码多种类型）" << endl;
    cout << "     - 编译时计算（模板元编程）" << endl;
    cout << "     - 性能好（编译时展开，没有运行时开销）" << endl;
    cout << "   缺点:" << endl;
    cout << "     - 编译时间长" << endl;
    cout << "     - 代码膨胀（每种类型生成一份代码）" << endl;
    cout << "     - 错误信息难懂" << endl;
    cout << "     - 实现必须在头文件中" << endl;
}

// ============================================================================
// 函数: demo_exception_handling
// 描述: 演示异常处理
// ============================================================================
void demo_exception_handling() {
    cout << "\n========== 异常处理 (Exception Handling) ==========" << endl;

    cout << "C++异常处理 vs C语言错误处理:" << endl;
    cout << "   - C: 返回错误码（-1、NULL、errno），容易忽略" << endl;
    cout << "   - C++: try/catch/throw，错误无法被忽略，类型安全" << endl;
    cout << "   - 异常会沿着调用栈向上传播，直到被捕获" << endl;

    // ------------------------------------------------------------------------
    // 1. 基本用法：try/catch/throw
    // ------------------------------------------------------------------------
    cout << "\n1. 基本用法 try/catch/throw:" << endl;

    // 正常情况
    cout << "   正常情况 (除以2):" << endl;
    try {
        int a = 10, b = 2;
        if (b == 0) {
            throw "除数不能为零";  // 抛出异常
        }
        cout << "   10 / 2 = " << (a / b) << endl;
    } catch (const char* msg) {
        cout << "   捕获异常: " << msg << endl;
    }

    // 异常情况
    cout << "\n   异常情况 (除以0):" << endl;
    try {
        int a = 10, b = 0;
        if (b == 0) {
            throw "除数不能为零";  // 抛出C风格字符串异常
        }
        cout << "   10 / 0 = " << (a / b) << endl;  // 不会执行到这里
    } catch (const char* msg) {
        cout << "   捕获异常: " << msg << endl;
    }

    // ------------------------------------------------------------------------
    // 2. 标准异常类
    // C++标准库定义了一套异常类，都继承自 std::exception
    // ------------------------------------------------------------------------
    cout << "\n2. 标准异常类 (std::exception):" << endl;
    cout << "   std::exception 是所有标准异常的基类" << endl;
    cout << "   常用派生类:" << endl;
    cout << "     - std::runtime_error: 运行时错误（可恢复）" << endl;
    cout << "     - std::logic_error: 逻辑错误（程序bug）" << endl;
    cout << "     - std::out_of_range: 越界访问" << endl;
    cout << "     - std::bad_alloc: 内存分配失败" << endl;

    // 捕获标准异常
    try {
        vector<int> v = {1, 2, 3};
        cout << "   访问 v.at(10): ";
        int val = v.at(10);     // at()越界会抛 out_of_range
        cout << val << endl;
    } catch (const std::out_of_range& e) {
        cout << "捕获 out_of_range: " << e.what() << endl;
    } catch (const std::exception& e) {
        cout << "捕获其他标准异常: " << e.what() << endl;
    }

    // ------------------------------------------------------------------------
    // 3. 自定义异常类
    // 使用 SimpleStack 演示自定义异常
    // ------------------------------------------------------------------------
    cout << "\n3. 自定义异常类:" << endl;
    cout << "   建议继承 std::exception 或其子类" << endl;

    // 栈空异常
    SimpleStack<int> empty_stack(5);
    try {
        cout << "   从空栈 pop: ";
        empty_stack.pop();
    } catch (const StackEmptyException& e) {
        cout << "捕获 StackEmptyException: " << e.what() << endl;
    }

    // 栈满异常
    SimpleStack<int> full_stack(3);
    try {
        full_stack.push(1);
        full_stack.push(2);
        full_stack.push(3);
        cout << "   向满栈 push 第4个元素: ";
        full_stack.push(4);
    } catch (const StackFullException& e) {
        cout << "捕获 StackFullException: " << e.what() << endl;
    }

    // ------------------------------------------------------------------------
    // 4. 异常传播和栈展开
    // ------------------------------------------------------------------------
    cout << "\n4. 异常传播与栈展开:" << endl;
    cout << "   - 异常抛出后，沿着调用栈向上查找匹配的catch" << endl;
    cout << "   - 沿途离开的作用域，局部对象会被正确析构（栈展开）" << endl;
    cout << "   - 如果main函数还没捕获，程序调用std::terminate终止" << endl;

    // ------------------------------------------------------------------------
    // 5. RAII与异常安全
    // ------------------------------------------------------------------------
    cout << "\n5. RAII与异常安全:" << endl;
    cout << "   - 有了栈展开，RAII类能保证资源一定会被释放" << endl;
    cout << "   - 即使发生异常，析构函数也会被调用" << endl;
    cout << "   - 这就是为什么推荐用智能指针而不是裸new/delete" << endl;
    cout << "   - 反例: new了对象后抛异常，delete被跳过 -> 内存泄漏" << endl;

    // ------------------------------------------------------------------------
    // 6. noexcept
    // ------------------------------------------------------------------------
    cout << "\n6. noexcept 说明符:" << endl;
    cout << "   - 声明函数不会抛出异常" << endl;
    cout << "   - 如果noexcept函数内部抛了异常，直接调用std::terminate" << endl;
    cout << "   - 用途: 优化（编译器可以做更多优化）、语义明确" << endl;
    cout << "   - 移动构造函数应该声明为noexcept（否则vector扩容时用拷贝）" << endl;

    cout << "\n7. 异常使用建议:" << endl;
    cout << "   - 用于真正异常的情况，不要用于正常流程控制" << endl;
    cout << "   - 抛异常用值，捕获用const引用" << endl;
    cout << "   - 自定义异常继承标准异常类" << endl;
    cout << "   - 配合RAII保证资源安全" << endl;
}

// ============================================================================
// 函数: demo_type_deduction
// 描述: 演示类型推导（auto、decltype）
// ============================================================================
void demo_type_deduction() {
    cout << "\n========== 类型推导 (Type Deduction) ==========" << endl;

    cout << "C++是静态类型语言，每个变量的类型在编译时就确定了" << endl;
    cout << "auto和decltype让编译器帮我们推导类型，代码更简洁" << endl;

    // ------------------------------------------------------------------------
    // 1. auto 的类型推导规则
    // ------------------------------------------------------------------------
    cout << "\n1. auto 类型推导规则:" << endl;

    // 规则1: auto 推导会忽略引用
    int x = 10;
    int& ref = x;
    auto a1 = ref;           // 推导为 int（忽略引用）
    cout << "   int& ref = x; auto a1 = ref; -> a1 类型是 int" << endl;
    cout << "   (auto 会忽略引用)" << endl;

    // 如果想要引用，需要显式声明
    auto& a2 = ref;          // 推导为 int&
    cout << "   auto& a2 = ref; -> a2 类型是 int&" << endl;

    // 规则2: auto 会忽略顶层const
    const int cx = 20;
    auto a3 = cx;            // 推导为 int（忽略顶层const）
    cout << "\n   const int cx = 20; auto a3 = cx; -> a3 类型是 int" << endl;
    cout << "   (auto 会忽略顶层const)" << endl;

    // 想要保留const，需要显式声明
    const auto a4 = cx;      // 推导为 const int
    cout << "   const auto a4 = cx; -> a4 类型是 const int" << endl;

    // 规则3: 指针和auto
    int* px = &x;
    auto a5 = px;            // 推导为 int*
    cout << "\n   int* px = &x; auto a5 = px; -> a5 类型是 int*" << endl;

    auto* a6 = &x;           // 显式声明指针，推导为 int*
    cout << "   auto* a6 = &x; -> a6 类型是 int*" << endl;

    // 规则4: auto 与数组、函数
    int arr[] = {1, 2, 3};
    auto a7 = arr;           // 数组退化为指针，推导为 int*
    cout << "\n   int arr[] = {1,2,3}; auto a7 = arr; -> a7 类型是 int*" << endl;
    cout << "   (数组名退化为指针)" << endl;

    // ------------------------------------------------------------------------
    // 2. decltype 的类型推导
    // decltype(expr) 返回表达式expr的类型
    // 与auto的区别: decltype会保留引用和const
    // ------------------------------------------------------------------------
    cout << "\n2. decltype 类型推导:" << endl;

    int dx = 10;
    const int& dr = dx;

    decltype(dx) d1 = 20;    // dx 是 int，所以 d1 是 int
    cout << "   int dx = 10; decltype(dx) d1 = 20; -> d1 类型是 int" << endl;

    decltype(dr) d2 = dx;    // dr 是 const int&，所以 d2 是 const int&
    cout << "   const int& dr = dx; decltype(dr) d2 = dx; -> d2 类型是 const int&" << endl;
    cout << "   (decltype 保留引用和const)" << endl;

    // 特殊情况：表达式是左值且加括号，推导为引用
    decltype((dx)) d3 = dx;  // (dx)是左值表达式，推导为 int&
    cout << "\n   decltype((dx)) d3 = dx; -> d3 类型是 int&" << endl;
    cout << "   (双重括号推导出引用类型)" << endl;

    // ------------------------------------------------------------------------
    // 3. auto vs decltype 对比
    // ------------------------------------------------------------------------
    cout << "\n3. auto vs decltype 对比:" << endl;
    cout << "   auto:" << endl;
    cout << "     - 从初始化表达式推导变量类型" << endl;
    cout << "     - 会忽略引用和顶层const" << endl;
    cout << "     - 主要用于变量声明简化" << endl;
    cout << "   decltype:" << endl;
    cout << "     - 查询表达式的类型（不执行表达式）" << endl;
    cout << "     - 保留引用和const" << endl;
    cout << "     - 主要用于模板编程、返回类型推导" << endl;

    // ------------------------------------------------------------------------
    // 4. 返回类型后置 / 尾置返回类型 (C++11)
    // ------------------------------------------------------------------------
    cout << "\n4. 尾置返回类型 (C++11):" << endl;
    cout << "   语法: auto func(参数) -> 返回类型 { ... }" << endl;
    cout << "   用途: 返回类型依赖参数类型时，配合decltype使用" << endl;
    cout << "   示例: template<typename T, typename U>" << endl;
    cout << "         auto add(T t, U u) -> decltype(t + u) {" << endl;
    cout << "             return t + u;" << endl;
    cout << "         }" << endl;

    // ------------------------------------------------------------------------
    // 5. decltype(auto) (C++14)
    // ------------------------------------------------------------------------
    cout << "\n5. decltype(auto) (C++14):" << endl;
    cout << "   - 结合auto的推导和decltype的保留规则" << endl;
    cout << "   - 用于返回类型推导，保留引用和const" << endl;
    cout << "   - 示例: decltype(auto) func() { return expr; }" << endl;

    // ------------------------------------------------------------------------
    // 6. 类型查看（辅助演示）
    // ------------------------------------------------------------------------
    cout << "\n6. 查看类型的方法:" << endl;
    cout << "   - typeid(x).name(): 运行时类型信息（名字可能被修饰）" << endl;
    cout << "   - 编译期查看: 使用编译器特定方法或故意制造编译错误" << endl;

    cout << "   typeid(auto变量).name() 示例:" << endl;
    auto demo_int = 42;
    auto demo_double = 3.14;
    auto demo_str = string("hello");
    cout << "     auto(42) -> " << typeid(demo_int).name() << endl;
    cout << "     auto(3.14) -> " << typeid(demo_double).name() << endl;
    cout << "     auto(string) -> " << typeid(demo_str).name() << endl;
}

// ============================================================================
// 函数: run_advanced_demo
// 描述: 运行所有高级特性演示（统一入口）
// ============================================================================
void run_advanced_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "  C++ 高级特性演示" << endl;
    cout << string(60, '=') << endl;

    demo_smart_pointers();
    demo_lambda();
    demo_move_semantics();
    demo_templates();
    demo_exception_handling();
    demo_type_deduction();

    cout << "\n" << string(60, '=') << endl;
    cout << "  高级特性演示结束" << endl;
    cout << string(60, '=') << endl;
}

} // namespace advanced
