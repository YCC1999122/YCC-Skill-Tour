// ============================================================================
// 文件名: oop.h
// 描述: C++面向对象编程模块头文件
// 作者: C++学习项目
// 说明: 本头文件声明了面向对象演示所需的所有类和函数
// ============================================================================

#ifndef OOP_H
#define OOP_H
#pragma once

#include <string>  // 包含标准字符串库，用于 std::string

// ============================================================================
// 命名空间 oop
// 作用: 将面向对象相关的类和函数封装在独立的命名空间中
// ============================================================================
namespace oop {

// ----------------------------------------------------------------------------
// 类: Point
// 描述: 一个简单的点类，用于演示基本的类与对象概念
// 包含: 构造函数、析构函数、拷贝构造、移动构造、运算符重载
// ----------------------------------------------------------------------------
class Point {
private:
    int x;  // 私有成员：x坐标，封装的体现，外部不能直接访问
    int y;  // 私有成员：y坐标

public:
    // 默认构造函数
    // 作用: 创建一个默认的Point对象
    // 特点: 无参数，使用初始化列表初始化成员
    Point();

    // 带参数的构造函数
    // 作用: 使用指定的x和y坐标创建Point对象
    // 参数: x - x坐标, y - y坐标
    // 特点: 使用初始化列表（initializer list）进行成员初始化
    Point(int x, int y);

    // 析构函数
    // 作用: 对象销毁时自动调用，用于释放资源
    // 特点: 无参数，无返回值，函数名前加~
    ~Point();

    // 拷贝构造函数
    // 作用: 用一个已存在的对象创建新对象（深拷贝）
    // 参数: other - 要拷贝的源对象（const左值引用，保证不修改源对象）
    // 特点: 参数必须是const引用，否则会无限递归
    Point(const Point& other);

    // 移动构造函数
    // 作用: 将一个临时对象（右值）的资源转移到新对象
    // 参数: other - 要移动的源对象（右值引用，表示可以窃取其资源）
    // 特点: C++11引入，用于提高性能，避免不必要的拷贝
    Point(Point&& other) noexcept;

    // 拷贝赋值运算符
    // 作用: 将一个对象的值赋给另一个已存在的对象
    // 参数: other - 要赋值的源对象
    // 返回: *this 的引用，支持链式赋值
    Point& operator=(const Point& other);

    // 移动赋值运算符
    // 作用: 将一个临时对象的值移动给另一个已存在的对象
    // 参数: other - 要移动的源对象（右值引用）
    // 返回: *this 的引用
    Point& operator=(Point&& other) noexcept;

    // 加法运算符重载
    // 作用: 实现两个Point对象的相加（对应坐标相加）
    // 参数: other - 另一个Point对象
    // 返回: 新的Point对象（两坐标之和）
    Point operator+(const Point& other) const;

    // 输出函数（用于演示，实际中通常重载<<运算符）
    void print() const;

    // Getter方法 - 封装的体现：通过公共方法访问私有成员
    int getX() const { return x; }
    int getY() const { return y; }

    // Setter方法 - 封装的体现：可以在方法中添加验证逻辑
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

    // 友元函数声明
    // 作用: 授予函数或类访问私有成员的权限
    // 这里声明一个全局函数为友元，它可以直接访问Point的私有成员
    friend void printPointPrivate(const Point& p);
};

// ----------------------------------------------------------------------------
// 类: Shape（抽象基类/接口）
// 描述: 形状抽象类，用于演示继承、多态、纯虚函数
// 特点: 包含纯虚函数，不能实例化，作为接口使用
// ----------------------------------------------------------------------------
class Shape {
protected:
    // 保护成员：派生类可以直接访问，外部不能访问
    std::string name;  // 形状名称

public:
    // 构造函数
    // 参数: name - 形状名称
    Shape(const std::string& name);

    // 虚析构函数
    // 作用: 保证通过基类指针删除派生类对象时，能正确调用派生类的析构函数
    // 重要性: 只要类中有虚函数，析构函数就应该声明为虚函数
    virtual ~Shape();

    // 纯虚函数（pure virtual function）
    // 作用: 定义接口规范，派生类必须实现
    // 特点: 函数体为 = 0，包含纯虚函数的类是抽象类，不能实例化
    virtual double area() const = 0;

    // 纯虚函数：计算周长
    virtual double perimeter() const = 0;

    // 普通虚函数：可以有默认实现，派生类可以选择重写
    // 作用: 打印形状信息
    virtual void printInfo() const;

    // 非虚函数：静态绑定，不参与多态
    std::string getName() const { return name; }
};

// ----------------------------------------------------------------------------
// 类: Circle（继承自Shape）
// 描述: 圆形类，公共同继承Shape
// 特点: 实现纯虚函数 area() 和 perimeter()
// ----------------------------------------------------------------------------
class Circle : public Shape {
private:
    double radius;  // 半径，私有成员

public:
    // 构造函数
    // 参数: r - 半径
    // 注意: 调用基类构造函数初始化基类部分
    Circle(double r);

    // 析构函数
    ~Circle() override;

    // 重写（override）基类的纯虚函数
    // override关键字: 显式声明这是重写的虚函数，编译器会检查签名是否匹配
    double area() const override;
    double perimeter() const override;

    // 重写基类的虚函数
    void printInfo() const override;
};

// ----------------------------------------------------------------------------
// 类: Rectangle（继承自Shape）
// 描述: 矩形类，公共同继承Shape
// ----------------------------------------------------------------------------
class Rectangle : public Shape {
private:
    double width;   // 宽度
    double height;  // 高度

public:
    // 构造函数
    Rectangle(double w, double h);

    // 析构函数
    ~Rectangle() override;

    // 实现纯虚函数
    double area() const override;
    double perimeter() const override;
};

// ----------------------------------------------------------------------------
// 类: Base1, Base2（用于演示多重继承）
// ----------------------------------------------------------------------------
class Base1 {
protected:
    int value1;
public:
    Base1(int v) : value1(v) {}
    virtual void show1() const { /* 演示用 */ }
    virtual ~Base1() {}
};

class Base2 {
protected:
    int value2;
public:
    Base2(int v) : value2(v) {}
    virtual void show2() const { /* 演示用 */ }
    virtual ~Base2() {}
};

// 多重继承：同时继承两个基类
class DerivedMulti : public Base1, public Base2 {
public:
    DerivedMulti(int v1, int v2);
    void showBoth() const;
};

// ----------------------------------------------------------------------------
// 静态成员演示类
// ----------------------------------------------------------------------------
class Counter {
private:
    // 静态成员变量
    // 特点: 所有对象共享同一份数据，不属于某个具体对象
    // 必须在类外初始化（定义）
    static int count;

    int id;  // 普通成员变量，每个对象独立拥有

public:
    // 静态成员函数
    // 特点: 只能访问静态成员变量和其他静态成员函数
    // 调用方式: 可以通过类名直接调用，不需要对象
    static int getCount();

    // 构造函数：每创建一个对象，count加1
    Counter();

    // const成员函数：不会修改任何成员变量
    int getId() const { return id; }
};

// ----------------------------------------------------------------------------
// 演示函数声明
// ----------------------------------------------------------------------------

// 演示类与对象的基本概念
void demo_class_object();

// 演示封装、继承、多态
void demo_inheritance_polymorphism();

// 演示抽象类和接口
void demo_abstract_class();

// 演示多重继承
void demo_multiple_inheritance();

// 演示友元和静态成员
void demo_friend_static();

// 演示运算符重载
void demo_operator_overloading();

// 演示const成员函数
void demo_const_members();

// 运行所有OOP演示
void run_oop_demo();

} // namespace oop

#endif // OOP_H
