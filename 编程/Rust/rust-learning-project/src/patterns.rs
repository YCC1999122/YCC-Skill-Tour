// ============================================================
// patterns.rs - 设计模式模块
// ============================================================
// 设计模式是解决常见软件设计问题的经过验证的解决方案
// Rust由于其独特的所有权系统和trait系统，实现设计模式的方式
// 与传统的面向对象语言（如Java、C++）有所不同
//
// 本模块演示在Rust中常用的设计模式：
//   - 单例模式（Singleton）
//   - 工厂模式（Factory）
//   - 策略模式（Strategy）
//   - 观察者模式（Observer）
//   - 建造者模式（Builder）
//   - RAII模式（Resource Acquisition Is Initialization）
// ============================================================

use std::sync::{Arc, Mutex, Once};

/// 设计模式模块的入口演示函数
pub fn run_patterns_demo() {
    println!("\n{}", "=".repeat(60));
    println!("设计模式演示");
    println!("{}", "=".repeat(60));

    singleton_demo();
    factory_demo();
    strategy_demo();
    observer_demo();
    builder_demo();
    raii_demo();
}

// ============================================================
// 1. 单例模式（Singleton）
// ============================================================
// 单例模式：确保一个类只有一个实例，并提供全局访问点
//
// 在Rust中实现单例的几种方式：
//   1. 使用 std::sync::Once + static mut（需要 unsafe）
//   2. 使用 lazy_static 或 once_cell crate（第三方库）
//   3. 使用 std::sync::LazyLock（Rust 1.80+ 稳定版）
//
// 这里使用标准库的 Once 来实现，不依赖外部 crate
// Once 可以确保某个初始化函数只执行一次（即使多线程同时调用）

/// 单例模式演示函数
fn singleton_demo() {
    println!("\n--- 1. 单例模式（Singleton）---");

    // ---- 单例结构体 ----
    // 这是我们要确保只有一个实例的类型
    #[derive(Debug)]
    struct AppConfig {
        database_url: String,
        max_connections: u32,
        log_level: String,
    }

    impl AppConfig {
        // 私有构造函数，外部不能直接创建实例
        fn new() -> Self {
            println!("  AppConfig 实例被创建（只执行一次）");
            AppConfig {
                database_url: String::from("postgres://localhost:5432/mydb"),
                max_connections: 10,
                log_level: String::from("info"),
            }
        }
    }

    // ---- 使用 Once 实现线程安全的单例 ----
    // Once: 确保初始化函数只调用一次的同步原语
    static mut INSTANCE: Option<Arc<AppConfig>> = None;
    static ONCE: Once = Once::new();

    // 获取单例实例的函数
    fn get_config() -> Arc<AppConfig> {
        // 这里使用 unsafe 是因为我们访问了 static mut 变量
        // 但 Once 保证了只会初始化一次，所以是安全的
        unsafe {
            ONCE.call_once(|| {
                // 这个闭包只会执行一次
                let config = AppConfig::new();
                INSTANCE = Some(Arc::new(config));
            });
            // clone Arc，增加引用计数
            INSTANCE.as_ref().unwrap().clone()
        }
    }

    // 多次调用 get_config，只会创建一次实例
    println!("第一次获取配置：");
    let config1 = get_config();
    println!("  配置: {:?}", config1);

    println!("第二次获取配置：");
    let config2 = get_config();
    println!("  配置: {:?}", config2);

    // 验证是同一个实例（Arc 指向同一个内存）
    println!("两个引用指向同一个实例吗？{}", Arc::ptr_eq(&config1, &config2));

    // ---- 多线程下的单例 ----
    // Once 是线程安全的，多个线程同时调用也只会初始化一次
    use std::thread;

    let mut handles = vec![];
    for i in 0..3 {
        let handle = thread::spawn(move || {
            let config = get_config();
            println!("  线程{}获取到配置，最大连接数: {}", i, config.max_connections);
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }

    println!("\n单例模式要点：");
    println!("  - 确保只有一个实例");
    println!("  - 提供全局访问点");
    println!("  - 使用 std::sync::Once 保证线程安全");
    println!("  - 实际项目推荐使用 once_cell 或 lazy_static crate");
}

// ============================================================
// 2. 工厂模式（Factory）
// ============================================================
// 工厂模式：定义一个创建对象的接口，让子类决定实例化哪个类
// 在Rust中，通常使用 trait + enum 或 trait 对象来实现
//
// 工厂模式的几种变体：
//   - 简单工厂：一个工厂函数，根据参数创建不同产品
//   - 工厂方法：每个产品有对应的工厂
//   - 抽象工厂：创建一系列相关的产品

/// 工厂模式演示函数
fn factory_demo() {
    println!("\n--- 2. 工厂模式（Factory）---");

    // ---- 定义产品 trait ----
    trait Vehicle {
        fn drive(&self) -> String;
        fn get_type(&self) -> &str;
    }

    // ---- 具体产品：汽车 ----
    struct Car {
        brand: String,
    }
    impl Vehicle for Car {
        fn drive(&self) -> String {
            format!("驾驶着{}品牌的汽车在公路上行驶", self.brand)
        }
        fn get_type(&self) -> &str {
            "汽车"
        }
    }

    // ---- 具体产品：自行车 ----
    struct Bicycle {
        color: String,
    }
    impl Vehicle for Bicycle {
        fn drive(&self) -> String {
            format!("骑着{}的自行车在小路上前行", self.color)
        }
        fn get_type(&self) -> &str {
            "自行车"
        }
    }

    // ---- 具体产品：卡车 ----
    struct Truck {
        capacity: u32,
    }
    impl Vehicle for Truck {
        fn drive(&self) -> String {
            format!("驾驶载重{}吨的卡车在高速上奔驰", self.capacity)
        }
        fn get_type(&self) -> &str {
            "卡车"
        }
    }

    // ---- 简单工厂 ----
    // 一个函数根据参数创建不同的产品
    fn create_vehicle(vehicle_type: &str) -> Option<Box<dyn Vehicle>> {
        match vehicle_type {
            "car" => Some(Box::new(Car {
                brand: String::from("特斯拉"),
            })),
            "bicycle" => Some(Box::new(Bicycle {
                color: String::from("红色"),
            })),
            "truck" => Some(Box::new(Truck {
                capacity: 20,
            })),
            _ => None,  // 未知类型返回 None
        }
    }

    // 使用工厂创建不同的交通工具
    let vehicles = vec!["car", "bicycle", "truck", "airplane"];

    for v_type in vehicles {
        match create_vehicle(v_type) {
            Some(vehicle) => {
                println!("创建了一辆{}: {}", vehicle.get_type(), vehicle.drive());
            }
            None => {
                println!("未知的交通工具类型: {}", v_type);
            }
        }
    }

    // ---- 工厂方法模式 ----
    // 每个产品有自己的工厂，工厂也实现统一的 trait
    trait VehicleFactory {
        fn create(&self) -> Box<dyn Vehicle>;
    }

    struct CarFactory {
        brand: String,
    }
    impl VehicleFactory for CarFactory {
        fn create(&self) -> Box<dyn Vehicle> {
            Box::new(Car {
                brand: self.brand.clone(),
            })
        }
    }

    struct BicycleFactory {
        color: String,
    }
    impl VehicleFactory for BicycleFactory {
        fn create(&self) -> Box<dyn Vehicle> {
            Box::new(Bicycle {
                color: self.color.clone(),
            })
        }
    }

    // 使用不同的工厂
    let factories: Vec<Box<dyn VehicleFactory>> = vec![
        Box::new(CarFactory { brand: String::from("比亚迪") }),
        Box::new(BicycleFactory { color: String::from("蓝色") }),
    ];

    println!("\n工厂方法模式：");
    for factory in &factories {
        let vehicle = factory.create();
        println!("  工厂生产: {}", vehicle.drive());
    }

    println!("\n工厂模式要点：");
    println!("  - 将对象创建与使用分离");
    println!("  - 简单工厂：一个函数根据参数创建不同产品");
    println!("  - 工厂方法：每个产品有对应的工厂");
    println!("  - 易于扩展新产品，符合开闭原则");
}

// ============================================================
// 3. 策略模式（Strategy）
// ============================================================
// 策略模式：定义一系列算法，把它们封装起来，并且使它们可以互相替换
// 策略模式让算法可以独立于使用它的客户而变化
//
// 在Rust中，策略模式通常通过 trait 实现：
//   - 定义一个 Strategy trait
//   - 每个具体策略实现这个 trait
//   - 上下文持有 trait 对象（或泛型 + trait bound）

/// 策略模式演示函数
fn strategy_demo() {
    println!("\n--- 3. 策略模式（Strategy）---");

    // ---- 定义策略 trait ----
    trait SortStrategy {
        fn sort(&self, data: &mut Vec<i32>);
        fn name(&self) -> &str;
    }

    // ---- 具体策略1：冒泡排序 ----
    struct BubbleSort;
    impl SortStrategy for BubbleSort {
        fn name(&self) -> &str {
            "冒泡排序"
        }

        fn sort(&self, data: &mut Vec<i32>) {
            let n = data.len();
            for i in 0..n {
                for j in 0..n - i - 1 {
                    if data[j] > data[j + 1] {
                        data.swap(j, j + 1);
                    }
                }
            }
        }
    }

    // ---- 具体策略2：选择排序 ----
    struct SelectionSort;
    impl SortStrategy for SelectionSort {
        fn name(&self) -> &str {
            "选择排序"
        }

        fn sort(&self, data: &mut Vec<i32>) {
            let n = data.len();
            for i in 0..n {
                let mut min_idx = i;
                for j in i + 1..n {
                    if data[j] < data[min_idx] {
                        min_idx = j;
                    }
                }
                data.swap(i, min_idx);
            }
        }
    }

    // ---- 具体策略3：快速排序（标准库）----
    struct StdSort;
    impl SortStrategy for StdSort {
        fn name(&self) -> &str {
            "标准库排序（快速排序）"
        }

        fn sort(&self, data: &mut Vec<i32>) {
            data.sort();  // 直接使用标准库的排序
        }
    }

    // ---- 上下文：排序器 ----
    // 持有一个策略对象，可以动态切换策略
    struct Sorter {
        strategy: Box<dyn SortStrategy>,
    }

    impl Sorter {
        fn new(strategy: Box<dyn SortStrategy>) -> Self {
            Sorter { strategy }
        }

        // 切换策略
        fn set_strategy(&mut self, strategy: Box<dyn SortStrategy>) {
            self.strategy = strategy;
        }

        // 执行排序
        fn sort(&self, data: &mut Vec<i32>) {
            println!("  使用策略：{}", self.strategy.name());
            self.strategy.sort(data);
        }
    }

    // 使用策略模式
    let data = vec![64, 34, 25, 12, 22, 11, 90];

    // 使用冒泡排序
    let mut sorter = Sorter::new(Box::new(BubbleSort));
    let mut data1 = data.clone();
    println!("原始数据: {:?}", data);
    sorter.sort(&mut data1);
    println!("  排序结果: {:?}", data1);

    // 切换到选择排序
    sorter.set_strategy(Box::new(SelectionSort));
    let mut data2 = data.clone();
    sorter.sort(&mut data2);
    println!("  排序结果: {:?}", data2);

    // 切换到标准库排序
    sorter.set_strategy(Box::new(StdSort));
    let mut data3 = data.clone();
    sorter.sort(&mut data3);
    println!("  排序结果: {:?}", data3);

    // ---- 使用泛型的策略模式（静态分发）----
    // 上面使用的是 trait 对象（动态分发）
    // 也可以使用泛型 + trait bound（静态分发，性能更好）
    struct GenericSorter<S: SortStrategy> {
        strategy: S,
    }

    impl<S: SortStrategy> GenericSorter<S> {
        fn new(strategy: S) -> Self {
            GenericSorter { strategy }
        }

        fn sort(&self, data: &mut Vec<i32>) {
            println!("  泛型策略：{}", self.strategy.name());
            self.strategy.sort(data);
        }
    }

    let generic_sorter = GenericSorter::new(StdSort);
    let mut data4 = data.clone();
    generic_sorter.sort(&mut data4);
    println!("  排序结果: {:?}", data4);

    println!("\n策略模式要点：");
    println!("  - 定义算法族，封装成策略");
    println!("  - 策略之间可以互相替换");
    println!("  - 动态切换算法，无需修改客户端");
    println!("  - trait对象：动态分发，灵活");
    println!("  - 泛型+trait：静态分发，高效");
}

// ============================================================
// 4. 观察者模式（Observer）
// ============================================================
// 观察者模式：定义对象间的一对多依赖关系，当一个对象状态改变时，
// 所有依赖它的对象都会收到通知并自动更新
//
// 别名：发布-订阅（Publish-Subscribe）模式
//
// 组成角色：
//   - Subject（主题/被观察者）：维护观察者列表，状态变化时通知观察者
//   - Observer（观察者）：定义更新接口，在收到通知时执行操作

/// 观察者模式演示函数
fn observer_demo() {
    println!("\n--- 4. 观察者模式（Observer）---");

    // ---- 定义观察者 trait ----
    trait Observer {
        // 当主题状态变化时，调用这个方法
        fn update(&self, temperature: f32, humidity: f32, pressure: f32);
        fn name(&self) -> &str;
    }

    // ---- 定义主题 trait ----
    trait Subject {
        // 注册观察者
        fn register_observer(&mut self, observer: Box<dyn Observer>);
        // 移除观察者
        fn remove_observer(&mut self, name: &str);
        // 通知所有观察者
        fn notify_observers(&self);
    }

    // ---- 具体主题：气象站 ----
    struct WeatherStation {
        observers: Vec<Box<dyn Observer>>,
        temperature: f32,
        humidity: f32,
        pressure: f32,
    }

    impl WeatherStation {
        fn new() -> Self {
            WeatherStation {
                observers: Vec::new(),
                temperature: 0.0,
                humidity: 0.0,
                pressure: 0.0,
            }
        }

        // 设置气象数据（模拟传感器更新）
        fn set_measurements(&mut self, temp: f32, humidity: f32, pressure: f32) {
            self.temperature = temp;
            self.humidity = humidity;
            self.pressure = pressure;
            self.notify_observers();  // 数据变化时通知所有观察者
        }
    }

    impl Subject for WeatherStation {
        fn register_observer(&mut self, observer: Box<dyn Observer>) {
            println!("  注册观察者：{}", observer.name());
            self.observers.push(observer);
        }

        fn remove_observer(&mut self, name: &str) {
            println!("  移除观察者：{}", name);
            self.observers.retain(|o| o.name() != name);
        }

        fn notify_observers(&self) {
            println!("  通知所有观察者：");
            for observer in &self.observers {
                observer.update(self.temperature, self.humidity, self.pressure);
            }
        }
    }

    // ---- 具体观察者1：当前天气显示 ----
    struct CurrentConditionsDisplay {
        name: String,
    }
    impl Observer for CurrentConditionsDisplay {
        fn name(&self) -> &str {
            &self.name
        }

        fn update(&self, temperature: f32, humidity: f32, _pressure: f32) {
            println!("    [{}] 当前天气：温度{:.1}°C，湿度{:.1}%",
                     self.name, temperature, humidity);
        }
    }

    // ---- 具体观察者2：天气预报显示 ----
    struct ForecastDisplay {
        name: String,
        last_pressure: Mutex<f32>,  // 记录上次气压，用于预测
    }
    impl Observer for ForecastDisplay {
        fn name(&self) -> &str {
            &self.name
        }

        fn update(&self, temperature: f32, _humidity: f32, pressure: f32) {
            let mut last = self.last_pressure.lock().unwrap();
            let forecast = if pressure > *last {
                "天气将好转"
            } else if pressure < *last {
                "天气将变差"
            } else {
                "天气保持不变"
            };
            println!("    [{}] 天气预报：{:.1}°C，气压{:.1}hPa，{}",
                     self.name, temperature, pressure, forecast);
            *last = pressure;
        }
    }

    // ---- 具体观察者3：统计显示 ----
    struct StatisticsDisplay {
        name: String,
        temps: Mutex<Vec<f32>>,
    }
    impl Observer for StatisticsDisplay {
        fn name(&self) -> &str {
            &self.name
        }

        fn update(&self, temperature: f32, _humidity: f32, _pressure: f32) {
            let mut temps = self.temps.lock().unwrap();
            temps.push(temperature);
            let avg: f32 = temps.iter().sum::<f32>() / temps.len() as f32;
            let max = temps.iter().cloned().fold(f32::NEG_INFINITY, f32::max);
            let min = temps.iter().cloned().fold(f32::INFINITY, f32::min);
            println!("    [{}] 温度统计：平均{:.1}°C，最高{:.1}°C，最低{:.1}°C",
                     self.name, avg, max, min);
        }
    }

    // 使用观察者模式
    println!("创建气象站：");
    let mut station = WeatherStation::new();

    // 注册观察者
    let current_display = CurrentConditionsDisplay {
        name: String::from("当前天气"),
    };
    let forecast_display = ForecastDisplay {
        name: String::from("天气预报"),
        last_pressure: Mutex::new(1013.0),
    };
    let stats_display = StatisticsDisplay {
        name: String::from("温度统计"),
        temps: Mutex::new(Vec::new()),
    };

    station.register_observer(Box::new(current_display));
    station.register_observer(Box::new(forecast_display));
    station.register_observer(Box::new(stats_display));

    // 更新气象数据
    println!("\n第一次更新数据：");
    station.set_measurements(25.3, 65.0, 1013.25);

    println!("\n第二次更新数据：");
    station.set_measurements(27.8, 70.0, 1015.50);

    println!("\n第三次更新数据：");
    station.set_measurements(24.1, 75.0, 1010.00);

    // 移除一个观察者
    println!("\n移除天气预报观察者：");
    station.remove_observer("天气预报");

    println!("\n第四次更新数据（天气预报不再接收通知）：");
    station.set_measurements(26.5, 68.0, 1012.00);

    println!("\n观察者模式要点：");
    println!("  - 一对多依赖，状态变化自动通知");
    println!("  - 主题和观察者松耦合");
    println!("  - 可以动态添加/移除观察者");
    println!("  - 别名：发布-订阅模式");
}

// ============================================================
// 5. 建造者模式（Builder）
// ============================================================
// 建造者模式：将一个复杂对象的构建与它的表示分离，
// 使得同样的构建过程可以创建不同的表示
//
// 适用场景：
//   - 对象构造参数很多，且很多有默认值
//   - 对象构造过程比较复杂
//   - 需要创建不同的表示形式
//
// 在Rust中，建造者模式非常常见：
//   - std::process::Command
//   - reqwest::RequestBuilder
//   - 等等...

/// 建造者模式演示函数
fn builder_demo() {
    println!("\n--- 5. 建造者模式（Builder）---");

    // ---- 目标产品：计算机 ----
    #[derive(Debug)]
    struct Computer {
        cpu: String,
        ram_gb: u32,
        storage_gb: u32,
        gpu: Option<String>,
        wifi: bool,
        bluetooth: bool,
        os: String,
    }

    // ---- 建造者 ----
    struct ComputerBuilder {
        cpu: String,
        ram_gb: u32,
        storage_gb: u32,
        gpu: Option<String>,
        wifi: bool,
        bluetooth: bool,
        os: String,
    }

    impl ComputerBuilder {
        // 新建建造者，设置必填字段和默认值
        fn new(cpu: &str) -> Self {
            ComputerBuilder {
                cpu: cpu.to_string(),
                ram_gb: 8,           // 默认8G内存
                storage_gb: 256,     // 默认256G存储
                gpu: None,           // 默认无独立显卡
                wifi: true,          // 默认有WiFi
                bluetooth: false,    // 默认无蓝牙
                os: String::from("Linux"),  // 默认Linux系统
            }
        }

        // 链式方法：设置内存（消耗 self，返回 Self）
        fn ram(mut self, gb: u32) -> Self {
            self.ram_gb = gb;
            self
        }

        // 设置存储
        fn storage(mut self, gb: u32) -> Self {
            self.storage_gb = gb;
            self
        }

        // 设置显卡
        fn gpu(mut self, model: &str) -> Self {
            self.gpu = Some(model.to_string());
            self
        }

        // 启用WiFi
        fn wifi(mut self, enabled: bool) -> Self {
            self.wifi = enabled;
            self
        }

        // 启用蓝牙
        fn bluetooth(mut self, enabled: bool) -> Self {
            self.bluetooth = enabled;
            self
        }

        // 设置操作系统
        fn os(mut self, os: &str) -> Self {
            self.os = os.to_string();
            self
        }

        // 最终构建方法
        fn build(self) -> Computer {
            Computer {
                cpu: self.cpu,
                ram_gb: self.ram_gb,
                storage_gb: self.storage_gb,
                gpu: self.gpu,
                wifi: self.wifi,
                bluetooth: self.bluetooth,
                os: self.os,
            }
        }
    }

    // 使用建造者创建不同配置的计算机
    println!("基础配置电脑：");
    let basic_pc = ComputerBuilder::new("Intel i3")
        .build();
    println!("  {:?}", basic_pc);

    println!("\n办公电脑：");
    let office_pc = ComputerBuilder::new("Intel i5")
        .ram(16)
        .storage(512)
        .wifi(true)
        .bluetooth(true)
        .os("Windows 11")
        .build();
    println!("  {:?}", office_pc);

    println!("\n游戏电脑：");
    let gaming_pc = ComputerBuilder::new("Intel i9")
        .ram(64)
        .storage(2000)
        .gpu("NVIDIA RTX 4090")
        .wifi(true)
        .bluetooth(true)
        .os("Windows 11")
        .build();
    println!("  {:?}", gaming_pc);

    println!("\n服务器：");
    let server = ComputerBuilder::new("AMD EPYC")
        .ram(256)
        .storage(8000)
        .wifi(false)
        .os("Ubuntu Server")
        .build();
    println!("  {:?}", server);

    println!("\n建造者模式要点：");
    println!("  - 将构建过程与表示分离");
    println!("  - 链式调用，代码可读性好");
    println!("  - 可选参数有默认值，灵活配置");
    println!("  - 复杂对象的构建更清晰");
    println!("  - Rust 标准库和生态中广泛使用");
}

// ============================================================
// 6. RAII模式
// ============================================================
// RAII（Resource Acquisition Is Initialization，资源获取即初始化）
// 是Rust中最核心的设计模式之一
//
// 核心思想：
//   - 资源在对象构造时获取
//   - 资源在对象析构时释放（Drop trait）
//   - 对象的生命周期与资源的生命周期绑定
//
// 好处：
//   - 自动管理资源，不会忘记释放
//   - 异常安全（即使panic也会释放资源）
//   - 不需要手动调用释放函数
//
// 典型应用：
//   - 内存管理（Box, Vec, String等）
//   - 文件句柄（File）
//   - 锁（MutexGuard, RwLockReadGuard等）
//   - 网络连接
//   - 等等...

/// RAII模式演示函数
fn raii_demo() {
    println!("\n--- 6. RAII模式（Drop trait）---");

    // ---- 自定义 RAII 资源：文件模拟 ----
    struct FileHandle {
        name: String,
    }

    impl FileHandle {
        // 构造函数：获取资源（打开文件）
        fn open(name: &str) -> Self {
            println!("  打开文件：{}", name);
            FileHandle {
                name: name.to_string(),
            }
        }

        // 方法：读取文件
        fn read(&self) {
            println!("  读取文件：{}", self.name);
        }
    }

    // Drop trait：释放资源（关闭文件）
    impl Drop for FileHandle {
        fn drop(&mut self) {
            println!("  自动关闭文件：{}", self.name);
        }
    }

    // 演示 RAII
    println!("进入内部作用域：");
    {
        let file = FileHandle::open("data.txt");
        file.read();
        // file 离开作用域时，自动调用 drop，关闭文件
    }
    println!("离开内部作用域，文件已自动关闭");

    // ---- RAII 与锁 ----
    // MutexGuard 就是 RAII 的典型例子
    // 获取锁时创建 MutexGuard，离开作用域时自动释放锁
    println!("\nRAII 与 Mutex 锁：");
    let mutex = Mutex::new(0);
    {
        println!("  获取锁...");
        let mut guard = mutex.lock().unwrap();
        *guard += 1;
        println!("  持有锁，修改数据: {}", guard);
        // guard 离开作用域，自动释放锁
    }
    println!("  锁已自动释放");
    println!("  数据: {}", mutex.lock().unwrap());

    // ---- RAII 与作用域守卫 ----
    // 使用 RAII 实现"作用域守卫"，确保某些操作一定会执行
    struct ScopeGuard<F: FnMut()> {
        on_drop: Option<F>,
    }

    impl<F: FnMut()> Drop for ScopeGuard<F> {
        fn drop(&mut self) {
            if let Some(mut f) = self.on_drop.take() {
                f();
            }
        }
    }

    fn defer<F: FnMut()>(f: F) -> ScopeGuard<F> {
        ScopeGuard { on_drop: Some(f) }
    }

    println!("\nRAII 作用域守卫（defer 模式）：");
    {
        let _guard = defer(|| {
            println!("  这是离开作用域时一定会执行的清理代码");
        });
        println!("  作用域内的代码...");
        println!("  更多代码...");
        // 即使这里 panic，guard 的 drop 也会执行
    }
    println!("离开作用域后");

    // ---- Rust 标准库中的 RAII ----
    println!("\n标准库中的RAII例子：");
    println!("  - Box<T> / Vec<T> / String：堆内存管理");
    println!("  - File：文件句柄");
    println!("  - MutexGuard / RwLockGuard：锁管理");
    println!("  - Thread / JoinHandle：线程管理");
    println!("  - scope guard 模式：确保清理代码执行");

    println!("\nRAII模式要点：");
    println!("  - 资源获取即初始化");
    println!("  - 对象析构时自动释放资源");
    println!("  - 不需要手动释放，不会忘记释放");
    println!("  - 异常安全（panic时也会释放）");
    println!("  - Rust最核心的设计模式之一");

    println!("\n设计模式模块演示完毕！");
    println!("核心要点：");
    println!("  - 单例：确保唯一实例，全局访问");
    println!("  - 工厂：封装对象创建过程");
    println!("  - 策略：算法族可互换");
    println!("  - 观察者：一对多通知");
    println!("  - 建造者：复杂对象分步构建");
    println!("  - RAII：资源自动管理（Rust核心模式）");
}
