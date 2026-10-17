// ============================================================================
// 文件名: patterns.cpp
// 描述: C++设计模式模块实现文件
// 作者: C++学习项目
// 说明: 本文件实现了各种设计模式的演示功能，每行代码都有详细中文注释
// ============================================================================

#include "patterns.h"

#include <iostream>
#include <algorithm>   // for_each
#include <cstdio>      // C文件操作

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

namespace patterns {

// ============================================================================
// 单例模式 - 饿汉式实现
// ============================================================================

// 静态实例初始化（程序启动时就创建）
// 注意：必须在类外定义静态成员变量
EagerSingleton EagerSingleton::instance;

// 私有构造函数
EagerSingleton::EagerSingleton() : data(0) {
    // 构造函数是私有的，外部不能直接创建对象
    // 只能通过 getInstance() 获取唯一实例
}

// 获取单例实例
EagerSingleton& EagerSingleton::getInstance() {
    // 直接返回静态实例
    // 因为静态实例在程序启动时就已经创建好了
    // 所以是线程安全的（静态初始化的线程安全性由C++运行时保证）
    return instance;
}

// ============================================================================
// 单例模式 - 懒汉式实现（非线程安全）
// ============================================================================

// 静态指针初始化（初始为nullptr，第一次使用时才创建）
LazySingleton* LazySingleton::instance = nullptr;

// 私有构造函数
LazySingleton::LazySingleton() : data(0) {
}

// 获取单例实例
LazySingleton* LazySingleton::getInstance() {
    // 检查是否已经创建
    // 注意：多线程环境下这里有竞态条件
    // 两个线程可能同时判断 instance == nullptr，然后都创建实例
    if (instance == nullptr) {
        instance = new LazySingleton();  // 第一次调用时才创建
    }
    return instance;
}

// ============================================================================
// 单例模式 - 线程安全实现（Meyers' Singleton）
// ============================================================================

// 私有构造函数
ThreadSafeSingleton::ThreadSafeSingleton() : data(0) {
}

// 获取单例实例
// 使用函数内的静态局部变量
// C++11及以后标准保证：静态局部变量的初始化是线程安全的
ThreadSafeSingleton& ThreadSafeSingleton::getInstance() {
    static ThreadSafeSingleton instance;  // 函数内静态变量，第一次调用时初始化
    return instance;
}

// ============================================================================
// 工厂模式 - 产品类实现
// ============================================================================

void ProductA::use() const {
    cout << "   使用 ProductA，做A类事情" << endl;
}

void ProductB::use() const {
    cout << "   使用 ProductB，做B类事情" << endl;
}

// ============================================================================
// 简单工厂实现
// ============================================================================

unique_ptr<Product> SimpleFactory::createProduct(const string& type) {
    // 根据类型参数创建不同的产品
    if (type == "A") {
        return make_unique<ProductA>();
    } else if (type == "B") {
        return make_unique<ProductB>();
    } else {
        // 未知类型，返回空指针
        return nullptr;
    }
}

// ============================================================================
// 工厂方法实现
// ============================================================================

unique_ptr<Product> FactoryA::createProduct() const {
    // FactoryA 负责创建 ProductA
    return make_unique<ProductA>();
}

unique_ptr<Product> FactoryB::createProduct() const {
    // FactoryB 负责创建 ProductB
    return make_unique<ProductB>();
}

// ============================================================================
// 抽象工厂实现
// ============================================================================

void WindowsButton::render() const {
    cout << "   渲染 Windows 风格按钮" << endl;
}

void MacButton::render() const {
    cout << "   渲染 Mac 风格按钮" << endl;
}

unique_ptr<Button> WindowsFactory::createButton() const {
    return make_unique<WindowsButton>();
}

unique_ptr<Button> MacFactory::createButton() const {
    return make_unique<MacButton>();
}

// ============================================================================
// 观察者模式实现
// ============================================================================

// 构造函数
WeatherStation::WeatherStation() : temperature(0) {
}

// 注册观察者
void WeatherStation::attach(shared_ptr<Observer> observer) {
    observers.push_back(observer);
    cout << "   注册观察者: " << observer->getName() << endl;
}

// 移除观察者
void WeatherStation::detach(shared_ptr<Observer> observer) {
    // 使用 erase-remove 惯用法移除观察者
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
    cout << "   移除观察者: " << observer->getName() << endl;
}

// 通知所有观察者
void WeatherStation::notify() {
    cout << "   天气站通知所有观察者..." << endl;
    for (const auto& observer : observers) {
        observer->update(temperature);  // 调用每个观察者的update方法
    }
}

// 设置温度（改变状态，触发通知）
void WeatherStation::setTemperature(int temp) {
    cout << "   天气站温度变化: " << temperature << " -> " << temp << endl;
    temperature = temp;
    notify();  // 状态改变后，通知所有观察者
}

// 手机显示屏实现
PhoneDisplay::PhoneDisplay(const string& n) : name(n) {
}

void PhoneDisplay::update(int state) {
    cout << "   [" << name << " 手机] 收到更新: 当前温度 " << state << "°C" << endl;
}

// 电脑显示屏实现
ComputerDisplay::ComputerDisplay(const string& n) : name(n) {
}

void ComputerDisplay::update(int state) {
    cout << "   [" << name << " 电脑] 收到更新: 当前温度 " << state << "°C" << endl;
}

// ============================================================================
// 策略模式实现
// ============================================================================

// 冒泡排序实现
void BubbleSort::sort(vector<int>& data) const {
    int n = data.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                // 交换相邻元素
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// 选择排序实现
void SelectionSort::sort(vector<int>& data) const {
    int n = data.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (data[j] < data[min_idx]) {
                min_idx = j;
            }
        }
        // 交换最小元素到已排序部分末尾
        int temp = data[i];
        data[i] = data[min_idx];
        data[min_idx] = temp;
    }
}

// 快速排序实现（使用STL的sort模拟）
void QuickSort::sort(vector<int>& data) const {
    std::sort(data.begin(), data.end());
}

// Sorter 上下文类构造函数
Sorter::Sorter(shared_ptr<SortStrategy> s) : strategy(s) {
}

// 切换策略
void Sorter::setStrategy(shared_ptr<SortStrategy> s) {
    strategy = s;
    cout << "   切换排序策略为: " << s->getName() << endl;
}

// 执行排序
void Sorter::performSort(vector<int>& data) const {
    if (strategy) {
        cout << "   使用 " << strategy->getName() << " 进行排序..." << endl;
        strategy->sort(data);  // 委托给策略对象执行
    }
}

// ============================================================================
// RAII模式 - 文件管理类实现
// ============================================================================

// 构造函数：打开文件
FileRAII::FileRAII(const char* filename, const char* mode) : file(nullptr) {
    file = fopen(filename, mode);  // 打开文件
    if (file) {
        cout << "   [FileRAII] 文件打开成功: " << filename << endl;
    } else {
        cout << "   [FileRAII] 文件打开失败: " << filename << endl;
    }
}

// 析构函数：关闭文件
FileRAII::~FileRAII() {
    if (file) {
        fclose(file);  // 关闭文件
        cout << "   [FileRAII] 文件已关闭" << endl;
    }
}

// 移动构造函数
FileRAII::FileRAII(FileRAII&& other) noexcept : file(other.file) {
    other.file = nullptr;  // 源对象置空，防止重复关闭
    cout << "   [FileRAII] 移动构造，文件资源转移" << endl;
}

// 移动赋值运算符
FileRAII& FileRAII::operator=(FileRAII&& other) noexcept {
    if (this != &other) {
        if (file) {
            fclose(file);  // 先释放自己的资源
        }
        file = other.file;  // 接管对方的资源
        other.file = nullptr;  // 对方置空
    }
    return *this;
}

// ============================================================================
// RAII模式 - 锁管理类实现
// ============================================================================

// 构造函数：加锁
LockRAII::LockRAII(std::mutex& m) : mtx(m) {
    mtx.lock();
    cout << "   [LockRAII] 加锁" << endl;
}

// 析构函数：解锁
LockRAII::~LockRAII() {
    mtx.unlock();
    cout << "   [LockRAII] 解锁" << endl;
}

// ============================================================================
// 演示函数实现
// ============================================================================

// ----------------------------------------------------------------------------
// 演示单例模式
// ----------------------------------------------------------------------------
void demo_singleton() {
    cout << "\n========== 单例模式 (Singleton Pattern) ==========" << endl;

    cout << "目的: 保证一个类只有一个实例，并提供全局访问点" << endl;
    cout << "应用场景: 配置管理、日志记录器、数据库连接池等" << endl;

    // 1. 饿汉式
    cout << "\n1. 饿汉式单例 (Eager Singleton):" << endl;
    cout << "   特点: 程序启动时就创建实例，线程安全" << endl;
    cout << "   缺点: 如果实例很大但从未使用，浪费资源" << endl;

    EagerSingleton& s1 = EagerSingleton::getInstance();
    EagerSingleton& s2 = EagerSingleton::getInstance();
    s1.setData(100);
    cout << "   s1.setData(100); s2.getData() = " << s2.getData() << endl;
    cout << "   &s1 = " << &s1 << ", &s2 = " << &s2 << endl;
    cout << "   地址相同，说明是同一个实例" << endl;

    // 2. 懒汉式
    cout << "\n2. 懒汉式单例 (Lazy Singleton):" << endl;
    cout << "   特点: 第一次使用时才创建（延迟加载）" << endl;
    cout << "   缺点: 多线程下不安全（可能创建多个实例）" << endl;

    LazySingleton* ls1 = LazySingleton::getInstance();
    LazySingleton* ls2 = LazySingleton::getInstance();
    ls1->setData(200);
    cout << "   ls1->setData(200); ls2->getData() = " << ls2->getData() << endl;
    cout << "   ls1 = " << ls1 << ", ls2 = " << ls2 << endl;
    cout << "   地址相同，说明是同一个实例" << endl;

    // 3. 线程安全单例
    cout << "\n3. 线程安全单例 (Meyers' Singleton) (推荐):" << endl;
    cout << "   特点: 函数内静态局部变量，C++11保证线程安全" << endl;
    cout << "   优点: 延迟加载 + 线程安全 + 实现简单" << endl;

    ThreadSafeSingleton& ts1 = ThreadSafeSingleton::getInstance();
    ThreadSafeSingleton& ts2 = ThreadSafeSingleton::getInstance();
    ts1.setData(300);
    cout << "   ts1.setData(300); ts2.getData() = " << ts2.getData() << endl;
    cout << "   &ts1 = " << &ts1 << ", &ts2 = " << &ts2 << endl;
    cout << "   地址相同，说明是同一个实例" << endl;

    cout << "\n4. 单例模式要点:" << endl;
    cout << "   - 构造函数私有（禁止外部创建）" << endl;
    cout << "   - 禁止拷贝和赋值（= delete）" << endl;
    cout << "   - 提供静态方法获取唯一实例" << endl;
    cout << "   - C++推荐使用 Meyers' Singleton（函数内静态变量）" << endl;
}

// ----------------------------------------------------------------------------
// 演示工厂模式
// ----------------------------------------------------------------------------
void demo_factory() {
    cout << "\n========== 工厂模式 (Factory Pattern) ==========" << endl;

    cout << "目的: 定义创建对象的接口，封装对象创建逻辑" << endl;
    cout << "应用场景: 对象创建逻辑复杂，需要灵活替换产品类型时" << endl;

    // 1. 简单工厂
    cout << "\n1. 简单工厂 (Simple Factory):" << endl;
    cout << "   一个工厂类根据参数创建不同产品" << endl;
    cout << "   缺点: 新增产品需要修改工厂类，违反开闭原则" << endl;

    auto productA = SimpleFactory::createProduct("A");
    auto productB = SimpleFactory::createProduct("B");

    if (productA) {
        cout << "   createProduct(\"A\"): " << productA->getName() << " - ";
        productA->use();
    }
    if (productB) {
        cout << "   createProduct(\"B\"): " << productB->getName() << " - ";
        productB->use();
    }

    // 2. 工厂方法
    cout << "\n2. 工厂方法 (Factory Method):" << endl;
    cout << "   定义工厂接口，每个产品对应一个具体工厂" << endl;
    cout << "   优点: 新增产品只需新增工厂类，符合开闭原则" << endl;

    // 工厂方法的使用：通过基类工厂指针创建产品
    unique_ptr<Factory> factoryA = make_unique<FactoryA>();
    unique_ptr<Factory> factoryB = make_unique<FactoryB>();

    auto pa = factoryA->createProduct();
    auto pb = factoryB->createProduct();

    cout << "   FactoryA 创建: " << pa->getName() << " - ";
    pa->use();
    cout << "   FactoryB 创建: " << pb->getName() << " - ";
    pb->use();

    // 3. 抽象工厂
    cout << "\n3. 抽象工厂 (Abstract Factory):" << endl;
    cout << "   创建一系列相关的产品（产品族）" << endl;
    cout << "   应用场景: UI主题切换（Windows风格 / Mac风格）" << endl;

    // Windows 风格工厂
    unique_ptr<GUIFactory> winFactory = make_unique<WindowsFactory>();
    auto winBtn = winFactory->createButton();
    cout << "   Windows 工厂创建按钮: ";
    winBtn->render();

    // Mac 风格工厂
    unique_ptr<GUIFactory> macFactory = make_unique<MacFactory>();
    auto macBtn = macFactory->createButton();
    cout << "   Mac 工厂创建按钮: ";
    macBtn->render();

    cout << "\n4. 三种工厂模式对比:" << endl;
    cout << "   简单工厂: 一个工厂创建多种产品，简单但不灵活" << endl;
    cout << "   工厂方法: 每个产品一个工厂，灵活但类数量多" << endl;
    cout << "   抽象工厂: 一个工厂创建产品族，用于系列产品" << endl;
}

// ----------------------------------------------------------------------------
// 演示观察者模式
// ----------------------------------------------------------------------------
void demo_observer() {
    cout << "\n========== 观察者模式 (Observer Pattern) ==========" << endl;

    cout << "目的: 定义一对多的依赖关系，一个对象状态变化时，" << endl;
    cout << "     所有依赖它的对象都会收到通知并自动更新" << endl;
    cout << "别名: 发布-订阅模式 (Publish-Subscribe)" << endl;
    cout << "应用场景: 事件处理、消息订阅、GUI事件通知等" << endl;

    // 创建主题（被观察者）：天气站
    auto weatherStation = make_shared<WeatherStation>();

    // 创建观察者：各种显示屏
    auto phone1 = make_shared<PhoneDisplay>("小明");
    auto phone2 = make_shared<PhoneDisplay>("小红");
    auto computer1 = make_shared<ComputerDisplay>("办公室");

    cout << "\n1. 注册观察者:" << endl;
    weatherStation->attach(phone1);
    weatherStation->attach(phone2);
    weatherStation->attach(computer1);

    cout << "\n2. 改变温度（触发通知）:" << endl;
    weatherStation->setTemperature(25);

    cout << "\n3. 移除一个观察者:" << endl;
    weatherStation->detach(phone2);

    cout << "\n4. 再次改变温度（只有注册的观察者收到通知）:" << endl;
    weatherStation->setTemperature(30);

    cout << "\n5. 观察者模式的优点:" << endl;
    cout << "   - 主题和观察者松耦合" << endl;
    cout << "   - 支持广播通信" << endl;
    cout << "   - 可以动态添加/移除观察者" << endl;
    cout << "   - 符合开闭原则" << endl;

    cout << "\n6. 观察者模式的缺点:" << endl;
    cout << "   - 如果观察者很多，通知可能耗时" << endl;
    cout << "   - 如果观察者和主题循环依赖，可能导致死循环" << endl;
    cout << "   - 观察者不知道主题是怎么变化的（只知道变化了）" << endl;
}

// ----------------------------------------------------------------------------
// 演示策略模式
// ----------------------------------------------------------------------------
void demo_strategy() {
    cout << "\n========== 策略模式 (Strategy Pattern) ==========" << endl;

    cout << "目的: 定义一系列算法，把它们封装起来，并且可以互相替换" << endl;
    cout << "应用场景: 需要在不同情况下使用不同算法，且算法可能变化时" << endl;
    cout << "核心思想: 把算法的使用和算法的实现分离" << endl;

    // 初始数据
    vector<int> data1 = {64, 34, 25, 12, 22, 11, 90};
    vector<int> data2 = data1;
    vector<int> data3 = data1;

    cout << "\n原始数据: ";
    for (int n : data1) cout << n << " ";
    cout << endl;

    // 创建不同的策略
    auto bubble = make_shared<BubbleSort>();
    auto selection = make_shared<SelectionSort>();
    auto quick = make_shared<QuickSort>();

    // 1. 使用冒泡排序
    cout << "\n1. 使用冒泡排序策略:" << endl;
    Sorter sorter(bubble);     // 创建上下文，指定初始策略
    sorter.performSort(data1);
    cout << "   排序结果: ";
    for (int n : data1) cout << n << " ";
    cout << endl;

    // 2. 切换到选择排序
    cout << "\n2. 切换到选择排序策略:" << endl;
    sorter.setStrategy(selection);  // 运行时切换策略
    sorter.performSort(data2);
    cout << "   排序结果: ";
    for (int n : data2) cout << n << " ";
    cout << endl;

    // 3. 切换到快速排序
    cout << "\n3. 切换到快速排序策略:" << endl;
    sorter.setStrategy(quick);
    sorter.performSort(data3);
    cout << "   排序结果: ";
    for (int n : data3) cout << n << " ";
    cout << endl;

    cout << "\n4. 策略模式的优点:" << endl;
    cout << "   - 算法可以自由切换" << endl;
    cout << "   - 避免使用多重条件判断" << endl;
    cout << "   - 扩展性好（新增策略只需新增类）" << endl;
    cout << "   - 算法实现和使用分离" << endl;

    cout << "\n5. 策略模式的缺点:" << endl;
    cout << "   - 策略类数量会增多" << endl;
    cout << "   - 客户端需要知道所有策略类才能选择" << endl;
    cout << "   - （可以配合工厂模式解决）" << endl;
}

// ----------------------------------------------------------------------------
// 演示RAII模式
// ----------------------------------------------------------------------------
void demo_raii() {
    cout << "\n========== RAII 模式 ==========" << endl;

    cout << "RAII: Resource Acquisition Is Initialization" << endl;
    cout << "     资源获取即初始化" << endl;
    cout << "核心思想: 利用对象生命周期管理资源" << endl;
    cout << "   - 构造函数中获取资源" << endl;
    cout << "   - 析构函数中释放资源" << endl;
    cout << "   - 对象离开作用域时自动调用析构函数" << endl;
    cout << "   - 即使发生异常，析构函数也会被调用（栈展开）" << endl;

    // 1. 文件管理的RAII
    cout << "\n1. RAII文件管理 (FileRAII):" << endl;
    cout << "   进入作用域，创建FileRAII对象:" << endl;
    {
        // 在作用域内创建文件管理对象
        FileRAII file("test_raii.txt", "w");
        if (file.isOpen()) {
            // 写入一些数据
            fprintf(file.get(), "Hello RAII!\n");
            fprintf(file.get(), "这是RAII模式的文件管理示例\n");
            cout << "   已向文件写入数据" << endl;
        }
        // 离开作用域时，file对象被销毁，析构函数自动关闭文件
        cout << "   即将离开作用域..." << endl;
    }
    cout << "   已离开作用域，文件已自动关闭" << endl;

    cout << "\n   RAII的优势（对比C语言fopen/fclose）:" << endl;
    cout << "   - 不需要手动调用fclose，不会忘记关闭" << endl;
    cout << "   - 即使中间发生异常，文件也会被正确关闭" << endl;
    cout << "   - 资源所有权明确（配合移动语义）" << endl;

    // 2. 锁管理的RAII
    cout << "\n2. RAII锁管理 (LockRAII):" << endl;
    std::mutex mtx;
    cout << "   进入临界区，创建LockRAII对象:" << endl;
    {
        LockRAII lock(mtx);  // 构造时加锁
        cout << "   临界区代码执行中..." << endl;
        // 临界区代码...
    }  // 离开作用域，析构时自动解锁
    cout << "   已离开临界区，锁已自动释放" << endl;

    cout << "\n3. 标准库中的RAII:" << endl;
    cout << "   - 智能指针: unique_ptr, shared_ptr (内存管理)" << endl;
    cout << "   - 锁管理: lock_guard, unique_lock (互斥锁管理)" << endl;
    cout << "   - 文件流: ifstream, ofstream (文件管理)" << endl;
    cout << "   - 容器: vector, string (内存管理)" << endl;

    cout << "\n4. RAII是C++最重要的编程范式之一:" << endl;
    cout << "   - 解决了资源泄漏问题" << endl;
    cout << "   - 异常安全的基础" << endl;
    cout << "   - 现代C++编程的基石" << endl;
}

// ============================================================================
// 函数: run_patterns_demo
// 描述: 运行所有设计模式演示（统一入口）
// ============================================================================
void run_patterns_demo() {
    cout << "\n" << string(60, '=') << endl;
    cout << "  C++ 设计模式演示" << endl;
    cout << string(60, '=') << endl;

    demo_singleton();
    demo_factory();
    demo_observer();
    demo_strategy();
    demo_raii();

    cout << "\n" << string(60, '=') << endl;
    cout << "  设计模式演示结束" << endl;
    cout << string(60, '=') << endl;
}

} // namespace patterns
