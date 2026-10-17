// ============================================================================
// 文件名: patterns.h
// 描述: C++设计模式模块头文件
// 作者: C++学习项目
// 说明: 本头文件声明了设计模式演示所需的所有类和函数
// ============================================================================

#ifndef PATTERNS_H
#define PATTERNS_H
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <mutex>

// ============================================================================
// 命名空间 patterns
// 作用: 将设计模式相关的类和函数封装在独立的命名空间中
// ============================================================================
namespace patterns {

// ============================================================================
// 一、单例模式 (Singleton Pattern)
// 目的: 保证一个类只有一个实例，并提供一个全局访问点
// 应用场景: 配置管理、日志记录器、数据库连接池等
// ============================================================================

// 1. 饿汉式单例（Eager Singleton）
// 特点: 程序启动时就创建实例，线程安全（静态初始化）
// 缺点: 如果实例很大但从未使用，会浪费资源
class EagerSingleton {
private:
    static EagerSingleton instance;  // 静态实例，程序启动时就创建
    int data;

    // 私有构造函数：禁止外部直接创建对象
    EagerSingleton();

    // 禁止拷贝和赋值
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

public:
    // 获取单例实例的方法
    static EagerSingleton& getInstance();

    int getData() const { return data; }
    void setData(int value) { data = value; }
};

// 2. 懒汉式单例（Lazy Singleton）- 非线程安全版本
// 特点: 第一次使用时才创建实例（延迟加载）
// 缺点: 多线程环境下可能创建多个实例
class LazySingleton {
private:
    static LazySingleton* instance;  // 指针，初始为nullptr
    int data;

    LazySingleton();

    // 禁止拷贝和赋值
    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;

public:
    static LazySingleton* getInstance();
    int getData() const { return data; }
    void setData(int value) { data = value; }
};

// 3. 线程安全单例（Thread-Safe Singleton）
// 特点: 使用互斥锁保证线程安全，或使用Meyers' Singleton（局部静态变量）
// 推荐方式: C++11及以后，局部静态变量初始化是线程安全的
class ThreadSafeSingleton {
private:
    int data;

    ThreadSafeSingleton();

    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    // Meyers' Singleton: 函数内静态局部变量
    // C++11保证静态局部变量的初始化是线程安全的
    static ThreadSafeSingleton& getInstance();

    int getData() const { return data; }
    void setData(int value) { data = value; }
};

// ============================================================================
// 二、工厂模式 (Factory Pattern)
// 目的: 定义创建对象的接口，让子类决定实例化哪个类
// 应用场景: 需要灵活创建不同类型对象，且对象创建逻辑复杂时
// ============================================================================

// 产品基类
class Product {
public:
    virtual ~Product() {}
    virtual std::string getName() const = 0;
    virtual void use() const = 0;
};

// 具体产品A
class ProductA : public Product {
public:
    std::string getName() const override { return "ProductA"; }
    void use() const override;
};

// 具体产品B
class ProductB : public Product {
public:
    std::string getName() const override { return "ProductB"; }
    void use() const override;
};

// 1. 简单工厂（Simple Factory）
// 特点: 一个工厂类根据参数创建不同产品
// 缺点: 新增产品需要修改工厂类，违反开闭原则
class SimpleFactory {
public:
    // 根据产品类型创建产品
    // 参数: type - 产品类型 "A" 或 "B"
    static std::unique_ptr<Product> createProduct(const std::string& type);
};

// 2. 工厂方法（Factory Method）
// 特点: 定义工厂接口，每个具体产品对应一个具体工厂
// 优点: 新增产品只需新增对应工厂，符合开闭原则
class Factory {
public:
    virtual ~Factory() {}
    virtual std::unique_ptr<Product> createProduct() const = 0;
};

class FactoryA : public Factory {
public:
    std::unique_ptr<Product> createProduct() const override;
};

class FactoryB : public Factory {
public:
    std::unique_ptr<Product> createProduct() const override;
};

// 3. 抽象工厂（Abstract Factory）
// 特点: 创建一系列相关或相互依赖的产品族
// 应用场景: 需要创建多个相关产品，且产品族需要统一变化时

// 另一个产品族的基类
class Button {
public:
    virtual ~Button() {}
    virtual void render() const = 0;
};

class WindowsButton : public Button {
public:
    void render() const override;
};

class MacButton : public Button {
public:
    void render() const override;
};

// 抽象工厂接口
class GUIFactory {
public:
    virtual ~GUIFactory() {}
    virtual std::unique_ptr<Button> createButton() const = 0;
};

// Windows风格工厂
class WindowsFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() const override;
};

// Mac风格工厂
class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() const override;
};

// ============================================================================
// 三、观察者模式 (Observer Pattern)
// 目的: 定义对象间一对多的依赖关系，当一个对象状态改变时，
//      所有依赖它的对象都会收到通知并自动更新
// 应用场景: 事件处理系统、消息订阅、GUI事件等
// ============================================================================

// 前向声明（因为Observer和Subject相互引用）
class Observer;

// 主题（被观察者）接口
class Subject {
public:
    virtual ~Subject() {}

    // 注册观察者
    virtual void attach(std::shared_ptr<Observer> observer) = 0;

    // 移除观察者
    virtual void detach(std::shared_ptr<Observer> observer) = 0;

    // 通知所有观察者
    virtual void notify() = 0;
};

// 观察者接口
class Observer {
public:
    virtual ~Observer() {}

    // 收到通知时的更新方法
    // 参数: state - 主题的新状态
    virtual void update(int state) = 0;

    // 获取观察者名称（用于演示输出）
    virtual std::string getName() const = 0;
};

// 具体主题：天气预报站
class WeatherStation : public Subject {
private:
    int temperature;  // 温度（状态）
    std::vector<std::shared_ptr<Observer>> observers;  // 观察者列表

public:
    WeatherStation();

    void attach(std::shared_ptr<Observer> observer) override;
    void detach(std::shared_ptr<Observer> observer) override;
    void notify() override;

    // 设置温度（改变状态，触发通知）
    void setTemperature(int temp);

    int getTemperature() const { return temperature; }
};

// 具体观察者：手机显示屏
class PhoneDisplay : public Observer {
private:
    std::string name;

public:
    explicit PhoneDisplay(const std::string& n);

    void update(int state) override;
    std::string getName() const override { return name; }
};

// 具体观察者：电脑显示屏
class ComputerDisplay : public Observer {
private:
    std::string name;

public:
    explicit ComputerDisplay(const std::string& n);

    void update(int state) override;
    std::string getName() const override { return name; }
};

// ============================================================================
// 四、策略模式 (Strategy Pattern)
// 目的: 定义一系列算法，把它们一个个封装起来，并且使它们可以互相替换
// 应用场景: 需要在不同情况下使用不同算法/策略，且算法可能经常变化时
// ============================================================================

// 策略接口
class SortStrategy {
public:
    virtual ~SortStrategy() {}

    // 排序方法
    // 参数: data - 要排序的数据（引用传递，直接修改）
    virtual void sort(std::vector<int>& data) const = 0;

    // 获取策略名称
    virtual std::string getName() const = 0;
};

// 具体策略：冒泡排序
class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override;
    std::string getName() const override { return "冒泡排序"; }
};

// 具体策略：选择排序
class SelectionSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override;
    std::string getName() const override { return "选择排序"; }
};

// 具体策略：快速排序（使用STL的sort模拟）
class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override;
    std::string getName() const override { return "快速排序"; }
};

// 上下文类：使用策略的类
class Sorter {
private:
    std::shared_ptr<SortStrategy> strategy;  // 当前使用的策略

public:
    // 构造时指定策略
    explicit Sorter(std::shared_ptr<SortStrategy> s);

    // 切换策略
    void setStrategy(std::shared_ptr<SortStrategy> s);

    // 执行排序
    void performSort(std::vector<int>& data) const;
};

// ============================================================================
// 五、RAII模式 (Resource Acquisition Is Initialization)
// 目的: 利用对象生命周期管理资源，构造时获取资源，析构时释放资源
// 应用场景: 内存管理、文件句柄、互斥锁、数据库连接等
// 重要性: C++中最重要的编程范式之一，智能指针就是RAII的典型应用
// ============================================================================

// RAII文件管理类
class FileRAII {
private:
    FILE* file;  // C风格文件指针，作为资源

public:
    // 构造函数：打开文件（获取资源）
    // 参数: filename - 文件名, mode - 打开模式
    FileRAII(const char* filename, const char* mode);

    // 析构函数：关闭文件（释放资源）
    // 无论函数正常返回还是异常退出，析构函数都会被调用
    ~FileRAII();

    // 禁止拷贝（防止多次释放）
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;

    // 允许移动
    FileRAII(FileRAII&& other) noexcept;
    FileRAII& operator=(FileRAII&& other) noexcept;

    // 获取原始文件指针（慎用）
    FILE* get() const { return file; }

    // 检查文件是否成功打开
    bool isOpen() const { return file != nullptr; }
};

// RAII锁管理类
class LockRAII {
private:
    std::mutex& mtx;  // 互斥锁引用

public:
    // 构造函数：加锁
    explicit LockRAII(std::mutex& m);

    // 析构函数：解锁
    ~LockRAII();

    // 禁止拷贝和移动
    LockRAII(const LockRAII&) = delete;
    LockRAII& operator=(const LockRAII&) = delete;
};

// ----------------------------------------------------------------------------
// 演示函数声明
// ----------------------------------------------------------------------------

// 演示单例模式
void demo_singleton();

// 演示工厂模式
void demo_factory();

// 演示观察者模式
void demo_observer();

// 演示策略模式
void demo_strategy();

// 演示RAII模式
void demo_raii();

// 运行所有设计模式演示
void run_patterns_demo();

} // namespace patterns

#endif // PATTERNS_H
