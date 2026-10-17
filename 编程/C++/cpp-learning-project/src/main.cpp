// ============================================================================
// 文件名: main.cpp
// 描述: C++学习项目 - 主程序入口
// 作者: C++学习项目
// 说明: 提供交互式菜单，用户可以选择运行各个模块的演示
// ============================================================================

// 包含各模块的头文件
#include "basics.h"
#include "oop.h"
#include "stl_containers.h"
#include "advanced.h"
#include "patterns.h"

// 包含标准输入输出
#include <iostream>
#include <string>
#include <limits>   // 用于 numeric_limits，清空输入缓冲区

// 使用常用名称
using std::cout;
using std::cin;
using std::endl;
using std::string;

// ============================================================================
// 函数: print_banner
// 描述: 打印程序标题横幅
// ============================================================================
void print_banner() {
    cout << endl;
    cout << "============================================================" << endl;
    cout << "                                                            " << endl;
    cout << "           C++ 学习项目 - 交互式演示程序                    " << endl;
    cout << "                                                            " << endl;
    cout << "   从C语言过渡到C++：掌握现代C++核心特性                    " << endl;
    cout << "                                                            " << endl;
    cout << "============================================================" << endl;
}

// ============================================================================
// 函数: print_menu
// 描述: 打印主菜单选项
// ============================================================================
void print_menu() {
    cout << endl;
    cout << "-------------------------- 主菜单 --------------------------" << endl;
    cout << endl;
    cout << "   1. 基础语法演示 (Basics)" << endl;
    cout << "      - 数据类型、auto/decltype、初始化列表" << endl;
    cout << "      - 引用、左值引用、右值引用" << endl;
    cout << "      - 范围for循环、结构化绑定" << endl;
    cout << "      - 命名空间、内联命名空间" << endl;
    cout << endl;
    cout << "   2. 面向对象编程 (OOP)" << endl;
    cout << "      - 类与对象、构造/析构/拷贝/移动" << endl;
    cout << "      - 封装/继承/多态、虚函数、虚表" << endl;
    cout << "      - 抽象类、接口、纯虚函数" << endl;
    cout << "      - 多重继承、虚继承" << endl;
    cout << "      - 友元、静态成员、const成员" << endl;
    cout << "      - 运算符重载" << endl;
    cout << endl;
    cout << "   3. STL容器与算法 (STL)" << endl;
    cout << "      - 顺序容器: vector/list/deque/array" << endl;
    cout << "      - 关联容器: map/unordered_map/set/unordered_set" << endl;
    cout << "      - string操作" << endl;
    cout << "      - 迭代器、迭代器失效" << endl;
    cout << "      - STL算法: sort/find/copy/transform等" << endl;
    cout << endl;
    cout << "   4. 高级特性 (Advanced)" << endl;
    cout << "      - 智能指针: unique_ptr/shared_ptr/weak_ptr" << endl;
    cout << "      - Lambda表达式与闭包" << endl;
    cout << "      - 移动语义与转移构造" << endl;
    cout << "      - 模板: 函数模板、类模板" << endl;
    cout << "      - 异常处理: try/catch/throw、自定义异常" << endl;
    cout << "      - 类型推导: auto、decltype" << endl;
    cout << endl;
    cout << "   5. 设计模式 (Patterns)" << endl;
    cout << "      - 单例模式（饿汉/懒汉/线程安全）" << endl;
    cout << "      - 工厂模式（简单工厂/工厂方法/抽象工厂）" << endl;
    cout << "      - 观察者模式" << endl;
    cout << "      - 策略模式" << endl;
    cout << "      - RAII模式" << endl;
    cout << endl;
    cout << "   6. 运行全部演示" << endl;
    cout << endl;
    cout << "   0. 退出程序" << endl;
    cout << endl;
    cout << "-----------------------------------------------------------" << endl;
}

// ============================================================================
// 函数: get_user_choice
// 描述: 获取用户输入的菜单选项
// 返回: 用户选择的整数
// 注意: 处理非法输入，保证程序健壮性
// ============================================================================
int get_user_choice() {
    int choice;
    cout << "请输入您的选择 (0-6): ";

    // 循环直到获取有效输入
    while (!(cin >> choice)) {
        // 输入失败（如输入了非数字）
        cin.clear();  // 清除错误标志
        // 忽略当前行剩余的所有字符
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "输入无效，请输入一个数字 (0-6): ";
    }

    // 忽略输入行剩余的内容（防止用户输入了多余字符）
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return choice;
}

// ============================================================================
// 函数: wait_for_enter
// 描述: 等待用户按回车键继续
// 作用: 让用户有时间阅读输出内容
// ============================================================================
void wait_for_enter() {
    cout << endl;
    cout << "按回车键继续...";
    cin.get();  // 等待用户输入回车键
}

// ============================================================================
// 函数: run_basics
// 描述: 运行基础语法演示
// ============================================================================
void run_basics() {
    cout << "\n正在运行：基础语法演示..." << endl;
    basics::run_basics_demo();  // 调用basics命名空间的统一入口
    wait_for_enter();
}

// ============================================================================
// 函数: run_oop
// 描述: 运行面向对象编程演示
// ============================================================================
void run_oop() {
    cout << "\n正在运行：面向对象编程演示..." << endl;
    oop::run_oop_demo();        // 调用oop命名空间的统一入口
    wait_for_enter();
}

// ============================================================================
// 函数: run_stl
// 描述: 运行STL容器与算法演示
// ============================================================================
void run_stl() {
    cout << "\n正在运行：STL容器与算法演示..." << endl;
    stl_demo::run_stl_demo();   // 调用stl_demo命名空间的统一入口
    wait_for_enter();
}

// ============================================================================
// 函数: run_advanced
// 描述: 运行高级特性演示
// ============================================================================
void run_advanced() {
    cout << "\n正在运行：高级特性演示..." << endl;
    advanced::run_advanced_demo();  // 调用advanced命名空间的统一入口
    wait_for_enter();
}

// ============================================================================
// 函数: run_patterns
// 描述: 运行设计模式演示
// ============================================================================
void run_patterns() {
    cout << "\n正在运行：设计模式演示..." << endl;
    patterns::run_patterns_demo();  // 调用patterns命名空间的统一入口
    wait_for_enter();
}

// ============================================================================
// 函数: run_all
// 描述: 依次运行所有模块的演示
// ============================================================================
void run_all() {
    cout << "\n正在运行：全部演示..." << endl;
    cout << "========================================" << endl;
    cout << "  注意：全部演示内容较多，可能需要一些时间" << endl;
    cout << "========================================" << endl;

    // 依次运行每个模块
    basics::run_basics_demo();
    wait_for_enter();

    oop::run_oop_demo();
    wait_for_enter();

    stl_demo::run_stl_demo();
    wait_for_enter();

    advanced::run_advanced_demo();
    wait_for_enter();

    patterns::run_patterns_demo();
    wait_for_enter();
}

// ============================================================================
// 主函数 main
// 描述: 程序入口，显示菜单并处理用户选择
// 参数: argc - 命令行参数个数, argv - 命令行参数数组
// 返回: 程序退出状态码（0表示正常退出）
// ============================================================================
int main(int argc, char* argv[]) {
    // 打印欢迎横幅
    print_banner();

    // 主循环：不断显示菜单直到用户选择退出
    bool running = true;        // 程序运行标志
    while (running) {
        // 显示菜单
        print_menu();

        // 获取用户选择
        int choice = get_user_choice();

        // 根据选择执行相应操作
        switch (choice) {
            case 1:
                run_basics();       // 基础语法
                break;
            case 2:
                run_oop();          // 面向对象
                break;
            case 3:
                run_stl();          // STL容器与算法
                break;
            case 4:
                run_advanced();     // 高级特性
                break;
            case 5:
                run_patterns();     // 设计模式
                break;
            case 6:
                run_all();          // 全部演示
                break;
            case 0:
                // 用户选择退出
                running = false;
                cout << endl;
                cout << "============================================================" << endl;
                cout << "                                                            " << endl;
                cout << "   感谢使用 C++ 学习项目！                                   " << endl;
                cout << "   祝你学习愉快，编程进步！                                  " << endl;
                cout << "                                                            " << endl;
                cout << "============================================================" << endl;
                cout << endl;
                break;
            default:
                // 无效选项
                cout << "\n无效的选择，请输入 0-6 之间的数字。" << endl;
                break;
        }
    }

    return 0;  // 程序正常退出
}
