// ============================================================================
// 文件名: advanced.h
// 描述: C++高级特性模块头文件
// 作者: C++学习项目
// 说明: 本头文件声明了高级特性演示所需的所有类和函数
// ============================================================================

#ifndef ADVANCED_H
#define ADVANCED_H
#pragma once

#include <memory>     // 智能指针头文件（unique_ptr, shared_ptr, weak_ptr）
#include <functional> // 函数对象和lambda相关
#include <string>
#include <stdexcept>  // 标准异常类

// ============================================================================
// 命名空间 advanced
// 作用: 将高级特性相关的类和函数封装在独立的命名空间中
// ============================================================================
namespace advanced {

// ----------------------------------------------------------------------------
// 函数: demo_smart_pointers
// 描述: 演示智能指针的使用
// 内容:
//   - unique_ptr: 独占所有权，不可复制，可移动，轻量级
//   - shared_ptr: 共享所有权，引用计数，可复制
//   - weak_ptr: 弱引用，不增加引用计数，用于打破循环引用
//   - make_unique/make_shared: 推荐的创建方式（更安全、更高效）
// ----------------------------------------------------------------------------
void demo_smart_pointers();

// ----------------------------------------------------------------------------
// 函数: demo_lambda
// 描述: 演示Lambda表达式与闭包
// 内容:
//   - Lambda语法：[捕获列表](参数列表) mutable -> 返回类型 { 函数体 }
//   - 值捕获、引用捕获、隐式捕获
//   - 泛型Lambda（C++14 auto参数）
//   - Lambda作为回调函数使用
//   - 闭包的概念（捕获外部变量的函数对象）
// ----------------------------------------------------------------------------
void demo_lambda();

// ----------------------------------------------------------------------------
// 函数: demo_move_semantics
// 描述: 演示移动语义与转移构造
// 内容:
//   - 左值与右值的概念
//   - 移动构造函数和移动赋值运算符
//   - std::move 的作用（将左值转换为右值引用）
//   - 移动语义的性能优势（避免深拷贝）
//   - noexcept 关键字的作用
// ----------------------------------------------------------------------------
void demo_move_semantics();

// ----------------------------------------------------------------------------
// 函数: demo_templates
// 描述: 演示模板编程
// 内容:
//   - 函数模板：通用算法，类型参数化
//   - 类模板：通用数据结构
//   - 模板特化：对特定类型提供特殊实现
//   - 模板参数推导
//   - 可变参数模板（简要介绍）
// ----------------------------------------------------------------------------
void demo_templates();

// ----------------------------------------------------------------------------
// 函数: demo_exception_handling
// 描述: 演示异常处理
// 内容:
//   - try/catch/throw 基本用法
//   - 标准异常类（std::exception及其派生类）
//   - 自定义异常类（继承std::exception）
//   - 异常规格说明（noexcept）
//   - RAII与异常安全
// ----------------------------------------------------------------------------
void demo_exception_handling();

// ----------------------------------------------------------------------------
// 函数: demo_type_deduction
// 描述: 演示类型推导（auto、decltype）
// 内容:
//   - auto 的类型推导规则
//   - decltype 的使用场景
//   - auto vs decltype 的区别
//   - 返回类型后置与尾置返回类型
//   - decltype(auto)（C++14）
// ----------------------------------------------------------------------------
void demo_type_deduction();

// ============================================================================
// 类模板演示：SimpleStack（简单栈）
// 描述: 一个简单的通用栈类模板，用于演示类模板的定义和使用
// 模板参数: T - 栈中存储的元素类型
// ============================================================================
template <typename T>
class SimpleStack {
private:
    T* data;        // 指向动态分配数组的指针
    int top;        // 栈顶索引
    int capacity;   // 栈的容量

public:
    // 构造函数
    // 参数: cap - 栈的初始容量，默认为10
    explicit SimpleStack(int cap = 10);

    // 析构函数
    ~SimpleStack();

    // 拷贝构造函数
    SimpleStack(const SimpleStack& other);

    // 移动构造函数
    SimpleStack(SimpleStack&& other) noexcept;

    // 入栈
    void push(const T& value);

    // 出栈
    T pop();

    // 查看栈顶元素
    T peek() const;

    // 判断栈是否为空
    bool isEmpty() const { return top == -1; }

    // 获取栈的大小
    int size() const { return top + 1; }
};

// ============================================================================
// 自定义异常类：StackEmptyException
// 描述: 栈为空时抛出的异常，继承自标准异常类 std::runtime_error
// 设计思路: 继承标准异常类，可以使用 what() 方法获取异常信息
// ============================================================================
class StackEmptyException : public std::runtime_error {
public:
    // 构造函数
    // 参数: msg - 异常信息
    explicit StackEmptyException(const std::string& msg)
        : std::runtime_error(msg) {}  // 调用基类构造函数
};

// ============================================================================
// 自定义异常类：StackFullException
// 描述: 栈满时抛出的异常
// ============================================================================
class StackFullException : public std::runtime_error {
public:
    explicit StackFullException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// ----------------------------------------------------------------------------
// 函数模板演示：my_swap（通用交换函数）
// 描述: 交换两个值的函数模板
// 模板参数: T - 值的类型
// 参数: a, b - 要交换的两个值（引用传递）
// ----------------------------------------------------------------------------
template <typename T>
void my_swap(T& a, T& b);

// ----------------------------------------------------------------------------
// 函数模板演示：get_max（获取较大值）
// 描述: 返回两个值中较大的那个
// ----------------------------------------------------------------------------
template <typename T>
T get_max(const T& a, const T& b);

// ----------------------------------------------------------------------------
// 函数: run_advanced_demo
// 描述: 运行所有高级特性演示（统一入口函数）
// ----------------------------------------------------------------------------
void run_advanced_demo();

// ============================================================================
// 类模板的实现
// 注意: 类模板的实现必须放在头文件中，因为编译器需要看到完整定义才能实例化
// ============================================================================

// 构造函数实现
template <typename T>
SimpleStack<T>::SimpleStack(int cap)
    : data(new T[cap]),   // 动态分配数组
      top(-1),            // 栈顶初始化为-1，表示空栈
      capacity(cap)       // 设置容量
{
}

// 析构函数实现
template <typename T>
SimpleStack<T>::~SimpleStack() {
    delete[] data;  // 释放动态分配的数组内存
}

// 拷贝构造函数实现（深拷贝）
template <typename T>
SimpleStack<T>::SimpleStack(const SimpleStack& other)
    : data(new T[other.capacity]),  // 分配新的内存
      top(other.top),               // 复制栈顶索引
      capacity(other.capacity)      // 复制容量
{
    // 逐个复制元素
    for (int i = 0; i <= top; ++i) {
        data[i] = other.data[i];
    }
}

// 移动构造函数实现
template <typename T>
SimpleStack<T>::SimpleStack(SimpleStack&& other) noexcept
    : data(other.data),     // 直接接管源对象的资源（指针赋值，不拷贝数据）
      top(other.top),       // 复制栈顶索引
      capacity(other.capacity)  // 复制容量
{
    // 将源对象的指针置空，防止析构时释放内存
    other.data = nullptr;
    other.top = -1;
    other.capacity = 0;
}

// 入栈实现
template <typename T>
void SimpleStack<T>::push(const T& value) {
    // 检查栈是否已满
    if (top + 1 >= capacity) {
        throw StackFullException("栈已满，无法入栈");
    }
    data[++top] = value;  // 栈顶上移，存入新元素
}

// 出栈实现
template <typename T>
T SimpleStack<T>::pop() {
    // 检查栈是否为空
    if (top < 0) {
        throw StackEmptyException("栈为空，无法出栈");
    }
    return data[top--];  // 返回栈顶元素，栈顶下移
}

// 查看栈顶元素实现
template <typename T>
T SimpleStack<T>::peek() const {
    if (top < 0) {
        throw StackEmptyException("栈为空，无栈顶元素");
    }
    return data[top];
}

// 函数模板：my_swap 实现
template <typename T>
void my_swap(T& a, T& b) {
    T temp = a;  // 保存a的值到临时变量
    a = b;       // 将b的值赋给a
    b = temp;    // 将临时变量（原a的值）赋给b
}

// 函数模板：get_max 实现
template <typename T>
T get_max(const T& a, const T& b) {
    return (a > b) ? a : b;  // 三元运算符返回较大值
}

} // namespace advanced

#endif // ADVANCED_H
