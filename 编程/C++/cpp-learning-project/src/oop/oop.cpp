// ============================================================================
// 文件名: oop.cpp
// 描述: C++面向对象编程模块实现文件
// 作者: C++学习项目
// 说明: 本文件实现了面向对象的各种演示功能，每行代码都有详细中文注释
// ============================================================================

// 包含对应的头文件
#include "oop.h"

// 包含标准输入输出
#include <iostream>

// 包含字符串流（用于格式化输出）
#include <sstream>

// 引入常用名称
using std::cout;
using std::endl;
using std::string;

// 进入 oop 命名空间
namespace oop {

// ============================================================================
// Point 类的实现
// ============================================================================

// 默认构造函数
// 使用初始化列表初始化成员变量，比在函数体内赋值更高效
// 初始化列表的顺序应该与成员声明顺序一致（避免未定义行为）
Point::Point() : x(0), y(0) {
    // 构造函数体：对象创建完成后执行
    // 这里可以做一些初始化工作，但成员变量已经在初始化列表中初始化了
    cout << "   [Point 默认构造函数] 调用，x=" << x << ", y=" << y << endl;
}

// 带参数的构造函数
// 参数 x 和 y 与成员变量同名，使用 this 指针区分
// this 指针：指向当前对象的指针，每个成员函数都隐含拥有
Point::Point(int x, int y) : x(x), y(y) {
    // 初始化列表中的 x(x) 含义：
    //   - 外面的 x 是成员变量 this->x
    //   - 括号里的 x 是参数 x
    // 这是C++的惯用写法，虽然同名但不会混淆
    cout << "   [Point 带参构造函数] 调用，x=" << this->x << ", y=" << this->y << endl;
}

// 析构函数
// 对象生命周期结束时自动调用
// 作用：释放对象持有的资源（内存、文件句柄、网络连接等）
// 调用顺序：先调用派生类析构，再调用基类析构（与构造相反）
Point::~Point() {
    cout << "   [Point 析构函数] 调用，x=" << x << ", y=" << y << endl;
}

// 拷贝构造函数
// 用一个已存在的对象创建新对象
// 参数必须是 const 引用，否则会导致无限递归（传值需要调用拷贝构造）
Point::Point(const Point& other) : x(other.x), y(other.y) {
    cout << "   [Point 拷贝构造函数] 调用，从 (" << other.x << "," << other.y
         << ") 拷贝到新对象" << endl;
}

// 移动构造函数
// C++11 引入，用于将临时对象（右值）的资源"偷"过来
// noexcept 表示该函数不会抛出异常，有助于优化
Point::Point(Point&& other) noexcept : x(other.x), y(other.y) {
    // 这里只是简单的int，移动和拷贝性能差不多
    // 但对于持有动态内存的类（如string、vector），移动性能优势明显
    // 移动的核心是：接管资源指针，然后将源对象的指针置空
    cout << "   [Point 移动构造函数] 调用，从 (" << other.x << "," << other.y
         << ") 移动资源" << endl;

    // 将源对象置为有效状态（防止析构时出问题）
    // 对于简单类型，这一步不是必须的，但是好的习惯
    other.x = 0;
    other.y = 0;
}

// 拷贝赋值运算符
// 将一个对象的值赋给另一个已存在的对象
// 返回 *this 的引用，支持链式赋值（a = b = c）
Point& Point::operator=(const Point& other) {
    cout << "   [Point 拷贝赋值运算符] 调用" << endl;

    // 防止自赋值（a = a）
    if (this != &other) {
        // 释放当前对象的资源（如果有的话）
        // 然后拷贝源对象的资源
        x = other.x;
        y = other.y;
    }
    // 返回当前对象的引用
    return *this;
}

// 移动赋值运算符
// 将一个临时对象的值移动给另一个已存在的对象
Point& Point::operator=(Point&& other) noexcept {
    cout << "   [Point 移动赋值运算符] 调用" << endl;

    // 防止自赋值
    if (this != &other) {
        // 释放当前对象的资源
        // 接管源对象的资源
        x = other.x;
        y = other.y;

        // 将源对象置为有效状态
        other.x = 0;
        other.y = 0;
    }
    return *this;
}

// 加法运算符重载
// 实现两个 Point 对象相加
// const 成员函数：保证不会修改当前对象的状态
Point Point::operator+(const Point& other) const {
    // 创建一个新的 Point 对象返回
    // 对应坐标分别相加
    return Point(x + other.x, y + other.y);
}

// 打印点的信息
void Point::print() const {
    cout << "Point(" << x << ", " << y << ")";
}

// 友元函数的实现
// 注意：友元函数虽然在类中声明，但它不是成员函数
// 它可以直接访问类的私有成员
void printPointPrivate(const Point& p) {
    // 直接访问私有成员 x 和 y（因为是友元）
    cout << "   [友元函数] 直接访问私有成员: Point(" << p.x << ", " << p.y << ")" << endl;
}

// ============================================================================
// Shape 类的实现（抽象基类）
// ============================================================================

// 构造函数
// 初始化形状名称
Shape::Shape(const std::string& name) : name(name) {
    cout << "   [Shape 构造函数] 创建形状: " << name << endl;
}

// 虚析构函数
// 必须定义为虚函数，否则通过基类指针删除派生类对象时，只会调用基类析构
// 导致派生类部分的资源泄漏
Shape::~Shape() {
    cout << "   [Shape 析构函数] 销毁形状: " << name << endl;
}

// 普通虚函数：有默认实现
// 派生类可以选择重写（override），也可以直接继承使用
void Shape::printInfo() const {
    cout << "   形状名称: " << name << endl;
    cout << "   面积: " << area() << endl;
    cout << "   周长: " << perimeter() << endl;
}

// ============================================================================
// Circle 类的实现（继承自Shape）
// ============================================================================

// 构造函数
// 注意初始化列表中调用基类构造函数 Shape("Circle")
// 构造顺序：先基类，后派生类
Circle::Circle(double r) : Shape("Circle"), radius(r) {
    cout << "   [Circle 构造函数] 半径 = " << radius << endl;
}

// 析构函数
// 析构顺序：先派生类，后基类（与构造相反）
Circle::~Circle() {
    cout << "   [Circle 析构函数] 半径 = " << radius << endl;
}

// 重写 area() 纯虚函数
// override 关键字：显式声明这是重写的虚函数
// 编译器会检查函数签名是否与基类的虚函数匹配，不匹配则报错
double Circle::area() const {
    // 圆的面积公式：π * r²
    return 3.1415926535 * radius * radius;
}

// 重写 perimeter() 纯虚函数
double Circle::perimeter() const {
    // 圆的周长公式：2 * π * r
    return 2 * 3.1415926535 * radius;
}

// 重写 printInfo() 虚函数
// 调用基类的 printInfo()，然后添加 Circle 特有的信息
void Circle::printInfo() const {
    // 调用基类版本的 printInfo()
    Shape::printInfo();
    // 添加派生类特有的信息
    cout << "   半径: " << radius << endl;
}

// ============================================================================
// Rectangle 类的实现（继承自Shape）
// ============================================================================

// 构造函数
Rectangle::Rectangle(double w, double h)
    : Shape("Rectangle"),  // 调用基类构造函数
      width(w),            // 初始化宽度
      height(h)            // 初始化高度
{
    cout << "   [Rectangle 构造函数] 宽=" << width << ", 高=" << height << endl;
}

// 析构函数
Rectangle::~Rectangle() {
    cout << "   [Rectangle 析构函数] 宽=" << width << ", 高=" << height << endl;
}

// 实现 area() 纯虚函数
double Rectangle::area() const {
    return width * height;
}

// 实现 perimeter() 纯虚函数
double Rectangle::perimeter() const {
    return 2 * (width + height);
}

// ============================================================================
// 多重继承示例
// ============================================================================

// 多重继承的构造函数
// 需要显式调用所有基类的构造函数
DerivedMulti::DerivedMulti(int v1, int v2)
    : Base1(v1),   // 调用第一个基类的构造函数
      Base2(v2)    // 调用第二个基类的构造函数
{
    // 构造顺序按照继承声明的顺序：先Base1，后Base2，最后DerivedMulti
}

// 显示两个基类的值
void DerivedMulti::showBoth() const {
    cout << "   Base1::value1 = " << value1 << endl;
    cout << "   Base2::value2 = " << value2 << endl;
}

// ============================================================================
// Counter 类的静态成员变量
// 静态成员变量必须在类外（头文件或源文件中）定义和初始化
// ============================================================================
int Counter::count = 0;  // 静态成员变量的定义和初始化

// 静态成员函数的实现
// 注意：静态成员函数没有 this 指针，只能访问静态成员
int Counter::getCount() {
    return count;  // 返回静态成员变量 count
}

// 构造函数
Counter::Counter() : id(++count) {
    // 每创建一个对象，count 加1，然后赋值给 id
    // 前缀递增：先加1，再使用值
    cout << "   [Counter 构造函数] 创建对象 #" << id << ", 当前总数: " << count << endl;
}

// ============================================================================
// 演示函数实现
// ============================================================================

// ----------------------------------------------------------------------------
// 演示类与对象的基本概念
// ----------------------------------------------------------------------------
void demo_class_object() {
    cout << "\n========== 类与对象的基本概念 ==========" << endl;
    cout << "C++类 vs C结构体:" << endl;
    cout << "   - C: struct 只有数据，没有成员函数（严格来说）" << endl;
    cout << "   - C++: class 封装数据和方法，支持访问控制" << endl;
    cout << "   - C++ struct 和 class 几乎一样，默认访问权限不同" << endl;
    cout << "     (struct默认public, class默认private)" << endl;

    cout << "\n1. 默认构造函数:" << endl;
    Point p1;                 // 调用默认构造函数
    p1.print();
    cout << endl;

    cout << "\n2. 带参构造函数:" << endl;
    Point p2(3, 4);           // 调用带参构造函数
    p2.print();
    cout << endl;

    cout << "\n3. 拷贝构造函数:" << endl;
    Point p3 = p2;            // 调用拷贝构造函数
    cout << "   p3 是 p2 的拷贝: ";
    p3.print();
    cout << endl;

    cout << "\n4. 拷贝赋值运算符:" << endl;
    Point p4;                 // 先创建一个对象
    p4 = p2;                  // 调用拷贝赋值运算符
    cout << "   p4 赋值后: ";
    p4.print();
    cout << endl;

    cout << "\n5. 移动构造函数 (std::move):" << endl;
    Point p5 = std::move(p2); // 将 p2 的资源移动给 p5
    cout << "   p5 (移动自p2): ";
    p5.print();
    cout << endl;
    cout << "   p2 (被移动后): ";
    p2.print();
    cout << " (源对象处于有效但未指定的状态)" << endl;

    cout << "\n6. 封装（Getter/Setter）:" << endl;
    cout << "   私有成员 x, y 只能通过公共方法访问" << endl;
    cout << "   p1.getX() = " << p1.getX() << ", p1.getY() = " << p1.getY() << endl;
    p1.setX(100);
    p1.setY(200);
    cout << "   调用 setX(100), setY(200) 后: ";
    p1.print();
    cout << endl;
}

// ----------------------------------------------------------------------------
// 演示继承与多态
// ----------------------------------------------------------------------------
void demo_inheritance_polymorphism() {
    cout << "\n========== 继承与多态 ==========" << endl;

    cout << "\n1. 继承的概念:" << endl;
    cout << "   - 基类（父类）: 定义通用属性和行为" << endl;
    cout << "   - 派生类（子类）: 继承基类，添加特有的属性和行为" << endl;
    cout << "   - 代码复用、层次化设计" << endl;

    cout << "\n2. 构造和析构顺序:" << endl;
    cout << "   创建 Circle 对象:" << endl;
    {
        // 用花括号限定作用域，观察析构顺序
        Circle c(5.0);
        cout << "   对象创建完成" << endl;
    }  // 离开作用域，对象被销毁
    cout << "   对象销毁完成" << endl;
    cout << "   构造顺序: 基类 -> 派生类" << endl;
    cout << "   析构顺序: 派生类 -> 基类（与构造相反）" << endl;

    cout << "\n3. 多态（虚函数 + 基类指针）:" << endl;
    cout << "   多态: 同一接口，不同实现，运行时决定调用哪个版本" << endl;

    // 使用基类指针指向派生类对象
    // 这是多态的基础：基类指针可以指向任何派生类对象
    Shape* shape1 = new Circle(3.0);
    Shape* shape2 = new Rectangle(4.0, 5.0);

    cout << "   Shape* shape1 = new Circle(3.0):" << endl;
    cout << "   shape1->area() = " << shape1->area()
         << " (调用的是Circle::area()，动态绑定)" << endl;

    cout << "\n   Shape* shape2 = new Rectangle(4.0, 5.0):" << endl;
    cout << "   shape2->area() = " << shape2->area()
         << " (调用的是Rectangle::area()，动态绑定)" << endl;

    cout << "\n4. 虚函数表（vtable）概念:" << endl;
    cout << "   - 每个有虚函数的类都有一个虚函数表" << endl;
    cout << "   - 每个对象都有一个虚指针（vptr）指向所属类的虚表" << endl;
    cout << "   - 调用虚函数时，通过vptr找到vtable，再找到实际的函数地址" << endl;
    cout << "   - 这就是动态绑定（运行时多态）的实现原理" << endl;

    cout << "\n5. 虚析构函数的重要性:" << endl;
    cout << "   通过基类指针删除派生类对象时，如果析构函数不是虚函数，" << endl;
    cout << "   只会调用基类析构，导致派生类部分资源泄漏" << endl;
    delete shape1;  // 正确调用 Circle -> Shape 的析构
    delete shape2;  // 正确调用 Rectangle -> Shape 的析构
}

// ----------------------------------------------------------------------------
// 演示抽象类和接口
// ----------------------------------------------------------------------------
void demo_abstract_class() {
    cout << "\n========== 抽象类与接口 ==========" << endl;

    cout << "1. 抽象类的定义:" << endl;
    cout << "   - 包含至少一个纯虚函数的类" << endl;
    cout << "   - 纯虚函数: virtual 返回类型 函数名(参数) = 0;" << endl;
    cout << "   - 抽象类不能实例化（不能创建对象）" << endl;
    cout << "   - 用于定义接口规范，派生类必须实现所有纯虚函数" << endl;

    // Shape s("test");  // 错误：Shape是抽象类，不能实例化

    cout << "\n2. 接口的概念:" << endl;
    cout << "   - 全是纯虚函数的类称为接口类" << endl;
    cout << "   - 定义一组行为规范，不关心实现细节" << endl;
    cout << "   - C++没有专门的interface关键字，用纯虚类实现接口" << endl;

    cout << "\n3. 派生类必须实现所有纯虚函数才能实例化:" << endl;
    cout << "   Circle 实现了 area() 和 perimeter()，所以可以实例化" << endl;
    cout << "   如果有一个纯虚函数没实现，派生类仍然是抽象类" << endl;

    cout << "\n4. 多态数组（通过基类指针数组管理不同类型的对象）:" << endl;
    const int size = 3;
    Shape* shapes[size];      // 基类指针数组

    shapes[0] = new Circle(2.0);
    shapes[1] = new Rectangle(3.0, 4.0);
    shapes[2] = new Circle(5.0);

    cout << "   遍历形状数组，调用 printInfo():" << endl;
    for (int i = 0; i < size; ++i) {
        cout << "   --- 形状 " << (i+1) << " ---" << endl;
        shapes[i]->printInfo();  // 多态调用
    }

    // 释放内存
    for (int i = 0; i < size; ++i) {
        delete shapes[i];
    }
}

// ----------------------------------------------------------------------------
// 演示多重继承
// ----------------------------------------------------------------------------
void demo_multiple_inheritance() {
    cout << "\n========== 多重继承 ==========" << endl;

    cout << "1. 多重继承的概念:" << endl;
    cout << "   - 一个派生类同时继承多个基类" << endl;
    cout << "   - 语法: class Derived : public Base1, public Base2 { ... }" << endl;
    cout << "   - 优点: 可以复用多个类的代码" << endl;
    cout << "   - 缺点: 可能导致菱形继承（二义性），需要虚继承解决" << endl;

    cout << "\n2. 多重继承示例:" << endl;
    DerivedMulti obj(10, 20);
    cout << "   DerivedMulti obj(10, 20);" << endl;
    obj.showBoth();

    cout << "\n3. 虚继承（解决菱形继承问题）:" << endl;
    cout << "   菱形继承: A 是基类，B 和 C 继承 A，D 同时继承 B 和 C" << endl;
    cout << "   问题: D 中会有两份 A 的成员，产生二义性和冗余" << endl;
    cout << "   解决: 使用 virtual 继承（虚继承）" << endl;
    cout << "   语法: class B : virtual public A { ... }" << endl;
    cout << "         class C : virtual public A { ... }" << endl;
    cout << "   效果: D 中只有一份 A 的成员" << endl;
    cout << "   注意: 虚继承会带来一些额外开销，应谨慎使用多重继承" << endl;
}

// ----------------------------------------------------------------------------
// 演示友元和静态成员
// ----------------------------------------------------------------------------
void demo_friend_static() {
    cout << "\n========== 友元与静态成员 ==========" << endl;

    cout << "1. 友元函数:" << endl;
    cout << "   - 友元函数可以直接访问类的私有成员" << endl;
    cout << "   - 在类中用 friend 关键字声明" << endl;
    cout << "   - 友元不是成员函数，没有 this 指针" << endl;
    cout << "   - 用途: 运算符重载、某些需要访问内部状态的工具函数" << endl;

    Point p(10, 20);
    printPointPrivate(p);     // 调用友元函数

    cout << "\n2. 静态成员变量:" << endl;
    cout << "   - 所有对象共享同一份数据" << endl;
    cout << "   - 不属于某个具体对象，属于整个类" << endl;
    cout << "   - 必须在类外定义和初始化" << endl;
    cout << "   - 内存存储在全局数据区，不在对象内存中" << endl;

    cout << "\n   创建 Counter 对象前，count = " << Counter::getCount() << endl;
    {
        Counter c1;           // 对象 #1
        Counter c2;           // 对象 #2
        Counter c3;           // 对象 #3
        cout << "   创建3个对象后，count = " << Counter::getCount() << endl;
        cout << "   c1.getId() = " << c1.getId() << endl;
        cout << "   c2.getId() = " << c2.getId() << endl;
        cout << "   c3.getId() = " << c3.getId() << endl;
    }
    cout << "   对象销毁后，count = " << Counter::getCount()
         << " (注意：count不会自动减少，这是简化的示例)" << endl;

    cout << "\n3. 静态成员函数:" << endl;
    cout << "   - 只能访问静态成员变量和其他静态成员函数" << endl;
    cout << "   - 没有 this 指针" << endl;
    cout << "   - 可以通过类名直接调用，不需要对象" << endl;
    cout << "   - 调用方式: 类名::静态函数名()" << endl;
}

// ----------------------------------------------------------------------------
// 演示运算符重载
// ----------------------------------------------------------------------------
void demo_operator_overloading() {
    cout << "\n========== 运算符重载 ==========" << endl;

    cout << "1. 运算符重载的概念:" << endl;
    cout << "   - 让自定义类型支持运算符操作" << endl;
    cout << "   - 本质是函数重载，函数名为 operator+运算符" << endl;
    cout << "   - 不能发明新的运算符，只能重载已有的" << endl;
    cout << "   - 运算符的优先级、结合性、操作数个数不变" << endl;

    cout << "\n2. 加法运算符重载:" << endl;
    Point p1(1, 2);
    Point p2(3, 4);
    Point p3 = p1 + p2;       // 等价于 p1.operator+(p2)
    cout << "   p1 = Point(1, 2)" << endl;
    cout << "   p2 = Point(3, 4)" << endl;
    cout << "   p3 = p1 + p2 = ";
    p3.print();
    cout << " (x相加，y相加)" << endl;

    cout << "\n3. 可重载的运算符:" << endl;
    cout << "   算术: + - * / % ++ --" << endl;
    cout << "   关系: == != < > <= >=" << endl;
    cout << "   逻辑: && || !" << endl;
    cout << "   赋值: = += -= *= /=" << endl;
    cout << "   其他: [] () -> << >>" << endl;

    cout << "\n4. 不能重载的运算符:" << endl;
    cout << "   . (成员访问)  .* (成员指针访问)" << endl;
    cout << "   :: (作用域解析)  ?: (三目运算符)" << endl;
    cout << "   sizeof  typeid" << endl;

    cout << "\n5. 重载规则:" << endl;
    cout << "   - 至少有一个操作数是自定义类型（不能重载 int + int）" << endl;
    cout << "   - 不能改变运算符的语义（比如把 + 做成减法）" << endl;
    cout << "   - 建议保持运算符的直觉含义" << endl;
}

// ----------------------------------------------------------------------------
// 演示const成员函数
// ----------------------------------------------------------------------------
void demo_const_members() {
    cout << "\n========== const 成员函数 ==========" << endl;

    cout << "1. const成员函数的概念:" << endl;
    cout << "   - 在函数声明后加 const 关键字" << endl;
    cout << "   - 保证函数不会修改任何成员变量（mutable成员除外）" << endl;
    cout << "   - const对象只能调用const成员函数" << endl;
    cout << "   - 非const对象可以调用const和非const成员函数" << endl;

    cout << "\n2. const对象的限制:" << endl;
    const Point p(5, 10);     // const对象
    cout << "   const Point p(5, 10);" << endl;
    cout << "   p.getX() = " << p.getX() << " (可以调用const成员函数)" << endl;
    cout << "   p.getY() = " << p.getY() << " (可以调用const成员函数)" << endl;
    // p.setX(100);  // 错误：const对象不能调用非const成员函数

    cout << "\n3. 为什么需要const成员函数?" << endl;
    cout << "   - 语义明确：告诉调用者这个函数不会修改对象" << endl;
    cout << "   - 允许const对象调用" << endl;
    cout << "   - 编译器可以做更多优化" << endl;
    cout << "   - 原则：能加const的成员函数都应该加const" << endl;

    cout << "\n4. mutable 关键字:" << endl;
    cout << "   - 被mutable修饰的成员变量，在const成员函数中也可以修改" << endl;
    cout << "   - 用途: 缓存计算结果、互斥锁等不影响对象逻辑状态的成员" << endl;
}

// ============================================================================
// 函数: run_oop_demo
// 描述: 运行所有OOP演示（统一入口）
// ============================================================================
void run_oop_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "  C++ 面向对象编程演示" << endl;
    cout << string(60, '=') << endl;

    // 依次调用各个演示函数
    demo_class_object();
    demo_inheritance_polymorphism();
    demo_abstract_class();
    demo_multiple_inheritance();
    demo_friend_static();
    demo_operator_overloading();
    demo_const_members();

    cout << "\n" << string(60, '=') << endl;
    cout << "  面向对象编程演示结束" << endl;
    cout << string(60, '=') << endl;
}

} // namespace oop
