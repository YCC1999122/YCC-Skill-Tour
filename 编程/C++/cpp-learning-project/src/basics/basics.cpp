// ============================================================================
// 文件名: basics.cpp
// 描述: C++基础语法模块实现文件
// 作者: C++学习项目
// 说明: 本文件实现了基础语法的各种演示功能，每行代码都有详细中文注释
// ============================================================================

// 包含对应的头文件
// 使用双引号表示包含项目内的头文件（相对路径）
#include "basics.h"

// 包含标准输入输出流库
// 使用尖括号表示包含标准库或系统头文件
#include <iostream>

// 包含向量容器（用于范围for循环演示）
#include <vector>

// 包含元组（用于结构化绑定演示）
#include <tuple>

// 包含映射容器（用于结构化绑定演示）
#include <map>

// 包含字符串类
#include <string>

// 使用 std 命名空间中的常用名称
// 注意: 在实际大型项目中不建议使用 using namespace std; 会导致命名冲突
// 这里为了演示代码简洁，使用具体名称的using声明
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::map;
using std::tuple;
using std::make_tuple;
using std::get;

// 进入 basics 命名空间
// 所有在本文件中定义的函数和变量都属于这个命名空间
namespace basics {

// ============================================================================
// 函数: demo_data_types
// 描述: 演示C++基本数据类型、auto/decltype、初始化列表
// ============================================================================
void demo_data_types() {
    // 打印函数标题，使用分隔线使输出更清晰
    cout << "\n========== 基本数据类型与初始化 ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. 基本数据类型
    // C++在C语言基础上增加了一些新的类型和特性
    // ------------------------------------------------------------------------

    // bool 类型：C++内置的布尔类型，值为 true 或 false
    // C语言中没有bool，通常用 int 或 typedef 定义
    bool flag = true;          // 布尔类型，值为真
    cout << "1. bool 类型:" << endl;
    cout << "   bool flag = true;  flag = " << flag
         << " (true输出为1, false输出为0)" << endl;
    cout << "   std::boolalpha 格式化后: " << std::boolalpha << flag << endl;
    cout << "   std::noboolalpha 恢复: " << std::noboolalpha << flag << endl;

    // char 类型：字符类型，占1字节
    char ch = 'A';             // 普通字符
    wchar_t wch = L'A';        // 宽字符（Windows下2字节，Linux下4字节）
    char16_t ch16 = u'A';      // UTF-16字符（C++11）
    char32_t ch32 = U'A';      // UTF-32字符（C++11）
    cout << "\n2. 字符类型:" << endl;
    cout << "   char ch = 'A';       ch = " << ch << " (ASCII码: " << (int)ch << ")" << endl;
    cout << "   wchar_t wch = L'A';  宽字符" << endl;

    // 整数类型
    int a = 10;                // 基本整型，通常4字节
    short s = 5;               // 短整型，通常2字节
    long l = 100L;             // 长整型，至少4字节
    long long ll = 1000LL;     // 长长整型（C++11），至少8字节
    unsigned int ui = 100u;    // 无符号整型
    cout << "\n3. 整数类型:" << endl;
    cout << "   int a = " << a << endl;
    cout << "   short s = " << s << endl;
    cout << "   long l = " << l << endl;
    cout << "   long long ll = " << ll << " (C++11)" << endl;
    cout << "   unsigned int ui = " << ui << endl;

    // 浮点类型
    float f = 3.14f;           // 单精度浮点数，通常4字节
    double d = 3.14159;        // 双精度浮点数，通常8字节
    long double ld = 3.14159L; // 扩展精度浮点数
    cout << "\n4. 浮点类型:" << endl;
    cout << "   float f = " << f << "f" << endl;
    cout << "   double d = " << d << endl;
    cout << "   long double ld = " << ld << "L" << endl;

    // nullptr_t: 空指针类型（C++11）
    // nullptr 是 C++11 引入的空指针常量，比 C 语言的 NULL 更安全
    // NULL 在C++中实际上就是 0，可能导致函数重载的歧义
    int* ptr = nullptr;        // 空指针，类型为 nullptr_t
    cout << "\n5. 空指针类型 (C++11):" << endl;
    cout << "   int* ptr = nullptr;  (推荐使用nullptr代替NULL)" << endl;
    cout << "   nullptr的优势: 类型安全，不会和整数0混淆" << endl;

    // ------------------------------------------------------------------------
    // 2. auto 关键字（C++11）
    // 作用: 让编译器自动推导变量的类型
    // 原理: 根据初始化表达式的类型来确定变量类型
    // 注意: auto 推导的变量必须立即初始化
    // ------------------------------------------------------------------------
    cout << "\n========== auto 类型推导 (C++11) ==========" << endl;

    auto auto_int = 42;        // 推导为 int
    auto auto_double = 3.14;   // 推导为 double
    auto auto_char = 'X';      // 推导为 char
    auto auto_str = string("hello"); // 推导为 std::string

    cout << "auto auto_int = 42;       类型: int, 值: " << auto_int << endl;
    cout << "auto auto_double = 3.14;  类型: double, 值: " << auto_double << endl;
    cout << "auto auto_char = 'X';     类型: char, 值: " << auto_char << endl;
    cout << "auto auto_str = string(\"hello\"); 类型: std::string, 值: " << auto_str << endl;

    // auto 与引用、指针结合
    int x = 10;
    auto* auto_ptr = &x;       // 显式声明指针，推导为 int*
    auto& auto_ref = x;        // 显式声明引用，推导为 int&
    const auto auto_const = x; // 推导为 const int
    cout << "\nauto与指针/引用/const结合:" << endl;
    cout << "   auto* auto_ptr = &x;   类型: int*" << endl;
    cout << "   auto& auto_ref = x;    类型: int&" << endl;
    cout << "   const auto auto_const = x;  类型: const int" << endl;

    // ------------------------------------------------------------------------
    // 3. decltype 关键字（C++11）
    // 作用: 查询表达式的类型，但不执行表达式（类似typeof的增强版）
    // 应用场景: 模板编程、返回类型推导、泛型编程
    // ------------------------------------------------------------------------
    cout << "\n========== decltype 类型查询 (C++11) ==========" << endl;

    int decl_x = 5;
    double decl_y = 3.14;

    // decltype(expr) 返回 expr 的类型
    // decl_x 是 int，所以 decltype(decl_x) 就是 int
    decltype(decl_x) decl_a = 10;  // decl_a 的类型是 int

    // 表达式类型推导
    // decl_x + decl_y 的结果类型是 double（int + double 会类型提升）
    decltype(decl_x + decl_y) decl_sum = decl_x + decl_y;

    // 引用类型推导
    // 如果表达式是左值，且加上括号，decltype 会推导为引用类型
    decltype((decl_x)) decl_ref = decl_x;  // 类型是 int&（注意双重括号）

    cout << "int decl_x = 5; double decl_y = 3.14;" << endl;
    cout << "decltype(decl_x) decl_a = 10;       类型: int, 值: " << decl_a << endl;
    cout << "decltype(decl_x + decl_y) decl_sum;  类型: double, 值: " << decl_sum << endl;
    cout << "decltype((decl_x)) decl_ref = decl_x; 类型: int& (双重括号推导出引用)" << endl;

    // auto 与 decltype 的区别：
    // - auto 会忽略引用和顶层const
    // - decltype 会保留引用和const
    cout << "\nauto vs decltype 区别:" << endl;
    cout << "   auto: 用于变量类型推导，会忽略引用和顶层const" << endl;
    cout << "   decltype: 用于查询表达式类型，保留引用和const" << endl;

    // ------------------------------------------------------------------------
    // 4. 初始化列表（C++11）
    // 作用: 统一的初始化语法，可以用于各种类型
    // 形式: 使用花括号 {} 进行初始化
    // 优点: 防止类型收窄（narrowing conversion），语法统一
    // ------------------------------------------------------------------------
    cout << "\n========== 初始化列表 (C++11) ==========" << endl;

    // 传统C风格初始化
    int c_style = 10;
    cout << "传统初始化: int c_style = 10;  值: " << c_style << endl;

    // C++11 列表初始化（统一初始化）
    int list_init1{10};        // 直接列表初始化
    int list_init2 = {20};     // 拷贝列表初始化
    cout << "列表初始化: int list_init1{10};  值: " << list_init1 << endl;
    cout << "列表初始化: int list_init2 = {20}; 值: " << list_init2 << endl;

    // 防止类型收窄（编译错误）
    // int narrow{3.14};  // 错误：从 double 到 int 的收窄转换
    // 传统方式会静默截断：int narrow = 3.14; // narrow = 3
    cout << "列表初始化的优势: 防止类型收窄（如 int x{3.14} 会编译报错）" << endl;

    // 容器的列表初始化
    vector<int> vec = {1, 2, 3, 4, 5};  // vector 使用初始化列表
    cout << "vector<int> vec = {1, 2, 3, 4, 5};  向量大小: " << vec.size() << endl;

    // 数组的列表初始化
    int arr[]{1, 2, 3, 4, 5};  // 自动推导数组大小
    cout << "int arr[]{1, 2, 3, 4, 5};  数组第一个元素: " << arr[0] << endl;

    // 结构体/POD类型的列表初始化
    struct Point { int x; int y; };
    Point p{10, 20};           // 结构体聚合初始化
    cout << "Point p{10, 20};  p.x = " << p.x << ", p.y = " << p.y << endl;
}

// ============================================================================
// 函数: demo_references
// 描述: 演示引用、左值引用、右值引用
// ============================================================================
void demo_references() {
    cout << "\n========== 引用、左值引用、右值引用 ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. 左值引用（普通引用）
    // 定义: 左值是可以取地址、有名字的表达式
    // 左值引用: 对左值的引用，相当于变量的别名
    // 语法: Type& ref = lvalue;
    // ------------------------------------------------------------------------
    cout << "\n1. 左值引用 (lvalue reference):" << endl;

    int original = 10;         // original 是一个左值（有名字、有地址）
    int& ref = original;       // ref 是 original 的左值引用（别名）

    cout << "   int original = 10;" << endl;
    cout << "   int& ref = original;  // ref 是 original 的别名" << endl;
    cout << "   original = " << original << ", ref = " << ref << endl;

    // 通过引用修改原变量
    ref = 20;                   // 修改 ref 就是修改 original
    cout << "   ref = 20;  // 通过引用修改原变量" << endl;
    cout << "   original = " << original << ", ref = " << ref << endl;
    cout << "   &original = " << &original << ", &ref = " << &ref
         << " (地址相同，说明是同一个变量)" << endl;

    // const 左值引用
    // 特点: 不能通过引用修改原变量
    const int& const_ref = original;
    cout << "\n   const int& const_ref = original;  // const引用，只读" << endl;
    cout << "   const_ref = " << const_ref << " (不能通过const_ref修改值)" << endl;

    // ------------------------------------------------------------------------
    // 2. 右值引用（C++11）
    // 定义: 右值是临时的、没有名字的、不能取地址的表达式
    // 右值引用: 对右值的引用，用于移动语义和完美转发
    // 语法: Type&& rref = rvalue;
    // 作用: 可以窃取临时对象的资源，避免深拷贝，提高性能
    // ------------------------------------------------------------------------
    cout << "\n2. 右值引用 (rvalue reference) (C++11):" << endl;

    int&& rref = 100;          // 100 是右值（字面量，临时值）
    cout << "   int&& rref = 100;  // 右值引用绑定到临时值" << endl;
    cout << "   rref = " << rref << endl;

    // 右值引用可以修改右值
    rref = 200;
    cout << "   rref = 200;  // 右值引用可以修改绑定的值" << endl;
    cout << "   rref = " << rref << endl;

    // std::move 的作用：将左值转换为右值引用
    // 注意: std::move 本身不移动任何东西，只是类型转换
    int move_var = 300;
    int&& move_ref = std::move(move_var);  // 将左值转为右值引用
    cout << "\n   int move_var = 300;" << endl;
    cout << "   int&& move_ref = std::move(move_var);  // std::move将左值转为右值引用" << endl;
    cout << "   move_ref = " << move_ref << endl;
    cout << "   注意: std::move只是类型转换，不实际移动数据" << endl;

    // ------------------------------------------------------------------------
    // 3. 左值引用和右值引用的区别总结
    // ------------------------------------------------------------------------
    cout << "\n3. 左值 vs 右值 总结:" << endl;
    cout << "   左值 (lvalue): 有名字、有地址、可以赋值" << endl;
    cout << "   右值 (rvalue): 临时值、无名字、不能取地址（如字面量、临时对象）" << endl;
    cout << "   左值引用: Type&  - 只能绑定到左值（const左值引用可绑定到右值）" << endl;
    cout << "   右值引用: Type&& - 只能绑定到右值，用于移动语义" << endl;

    // const左值引用可以绑定到右值（这是很重要的特性）
    const int& crref = 42;     // const左值引用可以绑定到右值
    cout << "\n   const int& crref = 42;  // const左值引用可以绑定到右值" << endl;
    cout << "   crref = " << crref << " (这是C++中函数参数常用const T&的原因)" << endl;
}

// ============================================================================
// 函数: demo_range_for_structured_binding
// 描述: 演示范围for循环和结构化绑定
// ============================================================================
void demo_range_for_structured_binding() {
    cout << "\n========== 范围for循环与结构化绑定 ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. 范围for循环（C++11）
    // 作用: 遍历容器或数组的每个元素，语法更简洁
    // 语法: for (元素声明 : 容器/数组) { ... }
    // 原理: 底层使用迭代器实现，等价于 for (auto it = begin; it != end; ++it)
    // ------------------------------------------------------------------------
    cout << "\n1. 范围for循环 (range-based for loop) (C++11):" << endl;

    // 遍历数组
    int arr[] = {1, 2, 3, 4, 5};
    cout << "   遍历数组: ";
    for (int num : arr) {       // 每个元素按值拷贝
        cout << num << " ";
    }
    cout << endl;

    // 遍历vector
    vector<string> fruits = {"apple", "banana", "cherry"};
    cout << "   遍历vector: ";
    for (const auto& fruit : fruits) {  // 使用const引用避免拷贝，更高效
        cout << fruit << " ";
    }
    cout << endl;

    // 修改元素（使用引用）
    vector<int> nums = {1, 2, 3, 4, 5};
    cout << "   修改元素前: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    for (int& n : nums) {       // 使用引用可以修改元素
        n *= 2;
    }
    cout << "   每个元素乘2后: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // 初始化列表直接遍历
    cout << "   遍历初始化列表: ";
    for (auto val : {10, 20, 30, 40, 50}) {
        cout << val << " ";
    }
    cout << endl;

    // 与C语言的对比：
    // C语言: for (int i = 0; i < size; i++) { ... arr[i] ... }
    // C++范围for: for (auto& item : container) { ... item ... }

    // ------------------------------------------------------------------------
    // 2. 结构化绑定（C++17）
    // 作用: 将结构体、元组、数组等的多个成员绑定到多个变量
    // 语法: auto [var1, var2, ...] = 结构;
    // 原理: 编译器生成隐藏变量，然后将各个成员绑定到声明的变量名
    // ------------------------------------------------------------------------
    cout << "\n2. 结构化绑定 (structured binding) (C++17):" << endl;

    // 结构体的结构化绑定
    struct Point { int x; int y; };
    Point p{10, 20};
    auto [px, py] = p;         // 将p.x和p.y绑定到px和py
    cout << "   struct Point { int x; int y; }; Point p{10, 20};" << endl;
    cout << "   auto [px, py] = p;  // 结构化绑定" << endl;
    cout << "   px = " << px << ", py = " << py << endl;

    // 元组的结构化绑定
    auto t = make_tuple(1, 2.5, string("hello"));
    auto [ti, td, ts] = t;     // 绑定元组的三个元素
    cout << "\n   auto t = make_tuple(1, 2.5, string(\"hello\"));" << endl;
    cout << "   auto [ti, td, ts] = t;  // 绑定元组元素" << endl;
    cout << "   ti = " << ti << ", td = " << td << ", ts = " << ts << endl;

    // map的结构化绑定（遍历键值对）
    map<string, int> scores = {{"Alice", 95}, {"Bob", 87}, {"Charlie", 92}};
    cout << "\n   map<string, int> scores 遍历:" << endl;
    for (const auto& [name, score] : scores) {  // 结构化绑定+范围for
        cout << "      " << name << ": " << score << endl;
    }
    cout << "   说明: map的元素是pair<const Key, Value>，结构化绑定非常方便" << endl;

    // 数组的结构化绑定
    int arr2[3] = {10, 20, 30};
    auto [a, b, c] = arr2;     // 绑定数组元素
    cout << "\n   int arr2[3] = {10, 20, 30};" << endl;
    cout << "   auto [a, b, c] = arr2;  // 绑定数组元素" << endl;
    cout << "   a = " << a << ", b = " << b << ", c = " << c << endl;
}

// ============================================================================
// 函数: demo_namespaces
// 描述: 演示命名空间和内联命名空间
// ============================================================================
void demo_namespaces() {
    cout << "\n========== 命名空间与内联命名空间 ==========" << endl;

    // ------------------------------------------------------------------------
    // 1. 命名空间的基本概念
    // 作用: 将相关的函数、类、变量组织在一起，避免命名冲突
    // 类比C语言: C语言中通过函数名前缀来区分（如 module_function）
    //           C++使用命名空间，更优雅，支持嵌套
    // ------------------------------------------------------------------------
    cout << "\n1. 命名空间基本用法:" << endl;

    // 定义一个命名空间
    namespace math {
        const double PI = 3.1415926535;
        double add(double a, double b) { return a + b; }
        double multiply(double a, double b) { return a * b; }
    }

    // 使用 :: 运算符访问命名空间成员
    cout << "   math::PI = " << math::PI << endl;
    cout << "   math::add(3.0, 4.0) = " << math::add(3.0, 4.0) << endl;
    cout << "   math::multiply(3.0, 4.0) = " << math::multiply(3.0, 4.0) << endl;

    // 另一个命名空间（同名函数不会冲突）
    namespace physics {
        const double PI = 3.14159;
        double add(double a, double b) { return a + b; }
    }
    cout << "\n   两个命名空间可以有同名成员，不会冲突:" << endl;
    cout << "   math::PI = " << math::PI << endl;
    cout << "   physics::PI = " << physics::PI << endl;

    // 嵌套命名空间
    namespace company {
        namespace department {
            namespace team {
                int member_count = 10;
            }
        }
    }
    cout << "\n   嵌套命名空间:" << endl;
    cout << "   company::department::team::member_count = "
         << company::department::team::member_count << endl;

    // C++17 嵌套命名空间的简洁写法（注释中演示，因为编译器可能不支持C++17）
    // namespace company::department::team { ... }

    // ------------------------------------------------------------------------
    // 2. 内联命名空间（C++11）
    // 作用: 内联命名空间的成员可以直接在外层命名空间中访问
    // 应用: 版本管理（默认使用最新版本，旧版本放在非内联命名空间中）
    // ------------------------------------------------------------------------
    cout << "\n2. 内联命名空间 (inline namespace) (C++11):" << endl;

    // 定义一个带版本的命名空间
    namespace graphics {
        // 内联命名空间：默认版本
        inline namespace v2 {
            void draw() { cout << "      调用 graphics::v2::draw() (新版本)" << endl; }
        }
        // 非内联命名空间：旧版本
        namespace v1 {
            void draw() { cout << "      调用 graphics::v1::draw() (旧版本)" << endl; }
        }
    }

    cout << "   内联命名空间的成员可以直接通过外层命名空间访问:" << endl;
    graphics::draw();          // 调用的是 v2::draw()（因为v2是inline的）

    cout << "   显式指定版本:" << endl;
    graphics::v1::draw();      // 调用旧版本
    graphics::v2::draw();      // 调用新版本

    cout << "\n   内联命名空间的典型用途: API版本管理" << endl;
    cout << "   - 默认使用最新版本（内联）" << endl;
    cout << "   - 用户可以显式指定旧版本" << endl;
    cout << "   - 升级时只需修改inline即可切换默认版本" << endl;

    // ------------------------------------------------------------------------
    // 3. 无名命名空间（匿名命名空间）
    // 作用: 内部链接，只在当前编译单元可见，替代C语言中的static函数
    // 优点: 比static更灵活，可以包含类、变量、函数等
    // ------------------------------------------------------------------------
    cout << "\n3. 无名命名空间 (unnamed namespace):" << endl;
    cout << "   作用: 替代C语言中的static，内部链接，只在当前文件可见" << endl;
    cout << "   语法: namespace { ... }  // 没有名字" << endl;
    cout << "   对比C语言: static int helper() { ... }" << endl;
    cout << "   C++方式: namespace { int helper() { ... } }" << endl;
}

// ============================================================================
// 函数: run_basics_demo
// 描述: 运行所有基础语法演示（统一入口）
// ============================================================================
void run_basics_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "  C++ 基础语法演示" << endl;
    cout << string(60, '=') << endl;

    // 依次调用各个演示函数
    demo_data_types();
    demo_references();
    demo_range_for_structured_binding();
    demo_namespaces();

    cout << "\n" << string(60, '=') << endl;
    cout << "  基础语法演示结束" << endl;
    cout << string(60, '=') << endl;
}

} // namespace basics
