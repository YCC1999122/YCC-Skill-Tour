/**
 * ============================================================
 *  TypeScript 设计模式详解
 * ============================================================
 *
 * 本文件详细介绍常用的设计模式，并结合 TypeScript 的类型系统实现
 * 包括：
 * 1. 单例模式（Singleton）
 * 2. 工厂模式（Factory）
 * 3. 观察者模式（Observer）
 * 4. 策略模式（Strategy）
 * 5. 装饰器模式（Decorator）
 * 6. 代理模式（Proxy）
 * ============================================================
 */

// ============================================================
//  1. 单例模式（Singleton）
// ============================================================

// 单例模式保证一个类只有一个实例，并提供一个全局访问点
// 常见应用：配置管理、数据库连接池、日志记录器等

/**
 * 单例类 - 配置管理器
 * 确保整个应用中只有一个配置管理器实例
 */
class ConfigurationManager {
  // 静态属性，保存唯一实例
  private static instance: ConfigurationManager | null = null;

  // 配置数据
  private config: Map<string, string> = new Map();

  // 构造函数设为私有，防止外部直接 new
  private constructor() {
    // 初始化默认配置
    this.config.set('appName', 'TypeScript Learning Project');
    this.config.set('version', '1.0.0');
    this.config.set('environment', 'development');
  }

  /**
   * 获取唯一实例的方法
   * 如果实例不存在，则创建；如果已存在，则返回已有实例
   * @returns ConfigurationManager 的唯一实例
   */
  static getInstance(): ConfigurationManager {
    if (ConfigurationManager.instance === null) {
      ConfigurationManager.instance = new ConfigurationManager();
    }
    return ConfigurationManager.instance;
  }

  /**
   * 获取配置项
   * @param key 配置键
   * @returns 配置值，如果不存在则返回 undefined
   */
  get(key: string): string | undefined {
    return this.config.get(key);
  }

  /**
   * 设置配置项
   * @param key 配置键
   * @param value 配置值
   */
  set(key: string, value: string): void {
    this.config.set(key, value);
  }

  /**
   * 获取所有配置
   */
  getAll(): Record<string, string> {
    const result: Record<string, string> = {};
    this.config.forEach((value, key) => {
      result[key] = value;
    });
    return result;
  }
}

// ---------- 单例模式的使用 ----------
// 多次调用 getInstance() 得到的是同一个实例

// const config1 = ConfigurationManager.getInstance();
// const config2 = ConfigurationManager.getInstance();
// console.log(config1 === config2);  // true，是同一个实例

// ============================================================
//  2. 工厂模式（Factory Pattern）
// ============================================================

// 工厂模式定义了一个创建对象的接口，但由子类决定实例化哪个类
// 工厂方法让类的实例化推迟到子类中进行

// ---------- 简单工厂模式 ----------
// 最简单的工厂模式，由一个工厂类根据参数创建不同的对象

// 产品接口
interface Logger {
  log(message: string): void;
  error(message: string): void;
}

// 具体产品 1：控制台日志记录器
class ConsoleLogger implements Logger {
  log(message: string): void {
    console.log(`[LOG] ${message}`);
  }

  error(message: string): void {
    console.error(`[ERROR] ${message}`);
  }
}

// 具体产品 2：文件日志记录器
class FileLogger implements Logger {
  private filePath: string;

  constructor(filePath: string) {
    this.filePath = filePath;
  }

  log(message: string): void {
    console.log(`[FILE LOG - ${this.filePath}] ${message}`);
    // 实际项目中这里会写入文件
  }

  error(message: string): void {
    console.error(`[FILE ERROR - ${this.filePath}] ${message}`);
  }
}

// 具体产品 3：数据库日志记录器
class DatabaseLogger implements Logger {
  private connectionString: string;

  constructor(connectionString: string) {
    this.connectionString = connectionString;
  }

  log(message: string): void {
    console.log(`[DB LOG - ${this.connectionString}] ${message}`);
    // 实际项目中这里会写入数据库
  }

  error(message: string): void {
    console.error(`[DB ERROR - ${this.connectionString}] ${message}`);
  }
}

// 日志类型枚举
type LoggerType = 'console' | 'file' | 'database';

// 简单工厂类
class LoggerFactory {
  /**
   * 根据类型创建对应的日志记录器
   * @param type 日志记录器类型
   * @param options 配置选项
   * @returns 日志记录器实例
   */
  static createLogger(type: 'console'): ConsoleLogger;
  static createLogger(type: 'file', filePath: string): FileLogger;
  static createLogger(type: 'database', connectionString: string): DatabaseLogger;
  static createLogger(type: LoggerType, options?: string): Logger {
    switch (type) {
      case 'console':
        return new ConsoleLogger();
      case 'file':
        return new FileLogger(options || 'app.log');
      case 'database':
        return new DatabaseLogger(options || 'mongodb://localhost:27017');
      default:
        throw new Error(`不支持的日志类型：${type}`);
    }
  }
}

// ---------- 工厂方法模式 ----------
// 定义一个创建对象的接口，让子类决定实例化哪一个类

// 抽象工厂
abstract class LoggerFactoryMethod {
  abstract createLogger(): Logger;

  log(message: string): void {
    const logger = this.createLogger();
    logger.log(message);
  }
}

// 具体工厂 1：控制台日志工厂
class ConsoleLoggerFactory extends LoggerFactoryMethod {
  createLogger(): Logger {
    return new ConsoleLogger();
  }
}

// 具体工厂 2：文件日志工厂
class FileLoggerFactory extends LoggerFactoryMethod {
  private filePath: string;

  constructor(filePath: string) {
    super();
    this.filePath = filePath;
  }

  createLogger(): Logger {
    return new FileLogger(this.filePath);
  }
}

// ============================================================
//  3. 观察者模式（Observer Pattern）
// ============================================================

// 观察者模式定义了一种一对多的依赖关系
// 当一个对象的状态发生改变时，所有依赖它的对象都会得到通知并自动更新
// 也叫发布-订阅模式（Publish-Subscribe）

// ---------- 带类型的观察者模式 ----------

/**
 * 观察者接口
 * @template T 通知的数据类型
 */
interface Observer<T> {
  /**
   * 接收通知的方法
   * @param data 通知的数据
   */
  update(data: T): void;
}

/**
 * 主题（被观察者）接口
 * @template T 通知的数据类型
 */
interface Subject<T> {
  /**
   * 注册观察者
   * @param observer 观察者
   */
  subscribe(observer: Observer<T>): void;

  /**
   * 移除观察者
   * @param observer 观察者
   */
  unsubscribe(observer: Observer<T>): void;

  /**
   * 通知所有观察者
   * @param data 通知的数据
   */
  notify(data: T): void;
}

/**
 * 具体主题：新闻发布者
 * 发布新闻，通知所有订阅者
 * @template T 新闻数据类型
 */
class NewsPublisher<T> implements Subject<T> {
  // 观察者列表
  private observers: Observer<T>[] = [];

  /**
   * 订阅新闻
   * @param observer 观察者（订阅者）
   */
  subscribe(observer: Observer<T>): void {
    this.observers.push(observer);
  }

  /**
   * 取消订阅
   * @param observer 观察者（订阅者）
   */
  unsubscribe(observer: Observer<T>): void {
    const index = this.observers.indexOf(observer);
    if (index !== -1) {
      this.observers.splice(index, 1);
    }
  }

  /**
   * 发布新闻，通知所有订阅者
   * @param news 新闻数据
   */
  notify(news: T): void {
    this.observers.forEach(observer => {
      observer.update(news);
    });
  }

  /**
   * 获取订阅者数量
   */
  getSubscriberCount(): number {
    return this.observers.length;
  }
}

// 新闻数据类型
interface News {
  title: string;
  content: string;
  author: string;
  date: Date;
}

/**
 * 具体观察者：新闻订阅者
 */
class NewsSubscriber implements Observer<News> {
  private name: string;

  constructor(name: string) {
    this.name = name;
  }

  /**
   * 接收新闻通知
   * @param news 新闻数据
   */
  update(news: News): void {
    console.log(`[${this.name}] 收到新闻：${news.title}`);
    console.log(`  作者：${news.author}`);
    console.log(`  内容：${news.content.substring(0, 30)}...`);
  }
}

// ============================================================
//  4. 策略模式（Strategy Pattern）
// ============================================================

// 策略模式定义了一系列算法，并将每个算法封装起来，使它们可以互相替换
// 策略模式让算法的变化独立于使用算法的客户

// ---------- 支付策略示例 ----------

/**
 * 支付策略接口
 * 所有支付方式都必须实现这个接口
 */
interface PaymentStrategy {
  /**
   * 支付方法
   * @param amount 支付金额
   * @returns 支付是否成功
   */
  pay(amount: number): boolean;

  /**
   * 获取支付方式名称
   */
  getName(): string;
}

// 具体策略 1：支付宝支付
class AlipayStrategy implements PaymentStrategy {
  private account: string;

  constructor(account: string) {
    this.account = account;
  }

  pay(amount: number): boolean {
    console.log(`使用支付宝支付 ¥${amount}`);
    console.log(`账户：${this.account}`);
    // 实际支付逻辑...
    return true;
  }

  getName(): string {
    return '支付宝';
  }
}

// 具体策略 2：微信支付
class WeChatPayStrategy implements PaymentStrategy {
  private wechatId: string;

  constructor(wechatId: string) {
    this.wechatId = wechatId;
  }

  pay(amount: number): boolean {
    console.log(`使用微信支付 ¥${amount}`);
    console.log(`微信号：${this.wechatId}`);
    // 实际支付逻辑...
    return true;
  }

  getName(): string {
    return '微信支付';
  }
}

// 具体策略 3：银行卡支付
class BankCardStrategy implements PaymentStrategy {
  private cardNumber: string;
  private cardHolder: string;

  constructor(cardNumber: string, cardHolder: string) {
    this.cardNumber = cardNumber;
    this.cardHolder = cardHolder;
  }

  pay(amount: number): boolean {
    console.log(`使用银行卡支付 ¥${amount}`);
    console.log(`卡号：**** **** **** ${this.cardNumber.slice(-4)}`);
    console.log(`持卡人：${this.cardHolder}`);
    // 实际支付逻辑...
    return true;
  }

  getName(): string {
    return '银行卡';
  }
}

/**
 * 支付上下文（使用策略的类）
 * 维护对策略对象的引用，通过策略执行具体操作
 */
class PaymentContext {
  private strategy: PaymentStrategy;

  constructor(strategy: PaymentStrategy) {
    this.strategy = strategy;
  }

  /**
   * 设置支付策略（可以在运行时切换）
   * @param strategy 新的支付策略
   */
  setStrategy(strategy: PaymentStrategy): void {
    this.strategy = strategy;
  }

  /**
   * 执行支付
   * @param amount 支付金额
   * @returns 支付是否成功
   */
  executePayment(amount: number): boolean {
    console.log(`准备通过${this.strategy.getName()}支付...`);
    const result = this.strategy.pay(amount);
    console.log(result ? '支付成功！' : '支付失败！');
    return result;
  }
}

// ============================================================
//  5. 装饰器模式（Decorator Pattern）
// ============================================================

// 装饰器模式动态地给一个对象添加一些额外的职责
// 就增加功能来说，装饰器模式比生成子类更为灵活
// 注意：这里的装饰器模式是设计模式，与 TypeScript 的装饰器语法不同

// ---------- 咖啡装饰器示例 ----------

/**
 * 咖啡组件接口
 */
interface Coffee {
  /**
   * 获取描述
   */
  getDescription(): string;

  /**
   * 获取价格
   */
  cost(): number;
}

// 具体组件：基础咖啡
class SimpleCoffee implements Coffee {
  getDescription(): string {
    return '基础咖啡';
  }

  cost(): number {
    return 10;
  }
}

/**
 * 咖啡装饰器基类
 * 实现 Coffee 接口，并持有一个 Coffee 实例
 */
abstract class CoffeeDecorator implements Coffee {
  protected coffee: Coffee;

  constructor(coffee: Coffee) {
    this.coffee = coffee;
  }

  abstract getDescription(): string;
  abstract cost(): number;
}

// 具体装饰器 1：加牛奶
class MilkDecorator extends CoffeeDecorator {
  getDescription(): string {
    return this.coffee.getDescription() + ' + 牛奶';
  }

  cost(): number {
    return this.coffee.cost() + 3;
  }
}

// 具体装饰器 2：加糖
class SugarDecorator extends CoffeeDecorator {
  getDescription(): string {
    return this.coffee.getDescription() + ' + 糖';
  }

  cost(): number {
    return this.coffee.cost() + 1;
  }
}

// 具体装饰器 3：加奶泡
class WhippedCreamDecorator extends CoffeeDecorator {
  getDescription(): string {
    return this.coffee.getDescription() + ' + 奶泡';
  }

  cost(): number {
    return this.coffee.cost() + 5;
  }
}

// 具体装饰器 4：加巧克力
class ChocolateDecorator extends CoffeeDecorator {
  getDescription(): string {
    return this.coffee.getDescription() + ' + 巧克力';
  }

  cost(): number {
    return this.coffee.cost() + 4;
  }
}

// ============================================================
//  6. 代理模式（Proxy Pattern）
// ============================================================

// 代理模式为其他对象提供一种代理以控制对这个对象的访问
// 代理对象在客户端和目标对象之间起到中介作用

// ---------- 图片代理示例（虚拟代理）----------

/**
 * 图片接口
 */
interface Image {
  /**
   * 显示图片
   */
  display(): void;
}

// 真实图片类（开销较大）
class RealImage implements Image {
  private fileName: string;

  constructor(fileName: string) {
    this.fileName = fileName;
    this.loadFromDisk();  // 加载图片是一个耗时操作
  }

  /**
   * 从磁盘加载图片（模拟耗时操作）
   */
  private loadFromDisk(): void {
    console.log(`正在从磁盘加载图片：${this.fileName}`);
    // 模拟加载时间...
  }

  display(): void {
    console.log(`显示图片：${this.fileName}`);
  }
}

/**
 * 图片代理类
 * 控制对真实图片的访问，实现延迟加载
 */
class ProxyImage implements Image {
  private fileName: string;
  private realImage: RealImage | null = null;

  constructor(fileName: string) {
    this.fileName = fileName;
  }

  /**
   * 显示图片
   * 第一次调用时才创建真实图片对象（延迟加载）
   */
  display(): void {
    if (this.realImage === null) {
      this.realImage = new RealImage(this.fileName);
    }
    this.realImage.display();
  }

  /**
   * 获取文件名（不需要加载真实图片）
   */
  getFileName(): string {
    return this.fileName;
  }

  /**
   * 检查是否已加载
   */
  isLoaded(): boolean {
    return this.realImage !== null;
  }
}

// ---------- 缓存代理示例 ----------

/**
 * 数据服务接口
 */
interface DataService {
  /**
   * 获取数据
   * @param key 数据键
   */
  getData(key: string): Promise<string>;
}

// 真实数据服务（模拟远程 API 调用）
class RealDataService implements DataService {
  async getData(key: string): Promise<string> {
    console.log(`[RealDataService] 从远程服务器获取数据：${key}`);
    // 模拟网络延迟
    await new Promise(resolve => setTimeout(resolve, 100));
    return `数据：${key} - ${Date.now()}`;
  }
}

/**
 * 缓存代理
 * 在真实服务前加一层缓存
 */
class CachedDataService implements DataService {
  private realService: RealDataService;
  private cache: Map<string, string> = new Map();

  constructor(realService: RealDataService) {
    this.realService = realService;
  }

  async getData(key: string): Promise<string> {
    // 先检查缓存
    if (this.cache.has(key)) {
      console.log(`[CachedDataService] 从缓存获取数据：${key}`);
      return this.cache.get(key)!;
    }

    // 缓存不存在，调用真实服务
    const data = await this.realService.getData(key);
    // 存入缓存
    this.cache.set(key, data);
    return data;
  }

  /**
   * 清除指定缓存
   */
  clearCache(key?: string): void {
    if (key) {
      this.cache.delete(key);
      console.log(`已清除缓存：${key}`);
    } else {
      this.cache.clear();
      console.log('已清除所有缓存');
    }
  }

  /**
   * 获取缓存大小
   */
  getCacheSize(): number {
    return this.cache.size;
  }
}

// ============================================================
//  演示函数：运行所有设计模式示例
// ============================================================

export function runPatternsDemo(): void {
  console.log('========== 设计模式演示 ==========');

  // 1. 单例模式
  console.log('1. 单例模式:');
  const config1 = ConfigurationManager.getInstance();
  const config2 = ConfigurationManager.getInstance();
  console.log('   config1 === config2:', config1 === config2);
  console.log('   配置:', config1.getAll());
  config1.set('debug', 'true');
  console.log('   设置后配置:', config2.getAll());  // config2 也能看到更新

  // 2. 工厂模式
  console.log('\n2. 工厂模式:');
  const consoleLogger = LoggerFactory.createLogger('console');
  consoleLogger.log('这是一条控制台日志');

  const fileLogger = LoggerFactory.createLogger('file', 'app.log');
  fileLogger.log('这是一条文件日志');

  // 工厂方法模式
  console.log('\n   工厂方法模式:');
  const consoleFactory = new ConsoleLoggerFactory();
  consoleFactory.log('通过工厂方法创建的日志');

  // 3. 观察者模式
  console.log('\n3. 观察者模式:');
  const newsPublisher = new NewsPublisher<News>();

  const subscriber1 = new NewsSubscriber('张三');
  const subscriber2 = new NewsSubscriber('李四');
  const subscriber3 = new NewsSubscriber('王五');

  newsPublisher.subscribe(subscriber1);
  newsPublisher.subscribe(subscriber2);
  newsPublisher.subscribe(subscriber3);

  console.log(`   订阅者数量：${newsPublisher.getSubscriberCount()}`);

  const news: News = {
    title: 'TypeScript 5.0 发布',
    content: 'TypeScript 5.0 带来了许多新特性，包括装饰器、const 类型参数等...',
    author: '微软',
    date: new Date()
  };

  console.log('   发布新闻...');
  newsPublisher.notify(news);

  // 取消一个订阅
  newsPublisher.unsubscribe(subscriber2);
  console.log(`   取消订阅后数量：${newsPublisher.getSubscriberCount()}`);

  // 4. 策略模式
  console.log('\n4. 策略模式:');
  const payment = new PaymentContext(new AlipayStrategy('user@example.com'));
  payment.executePayment(100);

  console.log('');
  payment.setStrategy(new WeChatPayStrategy('wx_123456'));
  payment.executePayment(200);

  console.log('');
  payment.setStrategy(new BankCardStrategy('6222021234567890', '张三'));
  payment.executePayment(300);

  // 5. 装饰器模式
  console.log('\n5. 装饰器模式:');
  // 基础咖啡
  let coffee: Coffee = new SimpleCoffee();
  console.log(`   ${coffee.getDescription()} - ¥${coffee.cost()}`);

  // 加牛奶
  coffee = new MilkDecorator(coffee);
  console.log(`   ${coffee.getDescription()} - ¥${coffee.cost()}`);

  // 加糖
  coffee = new SugarDecorator(coffee);
  console.log(`   ${coffee.getDescription()} - ¥${coffee.cost()}`);

  // 加奶泡
  coffee = new WhippedCreamDecorator(coffee);
  console.log(`   ${coffee.getDescription()} - ¥${coffee.cost()}`);

  // 6. 代理模式
  console.log('\n6. 代理模式:');
  console.log('   --- 虚拟代理（图片延迟加载） ---');
  const image1 = new ProxyImage('photo1.jpg');
  const image2 = new ProxyImage('photo2.jpg');

  console.log('   图片1已加载？', image1.isLoaded());
  console.log('   图片2已加载？', image2.isLoaded());

  console.log('   第一次显示图片1...');
  image1.display();
  console.log('   图片1已加载？', image1.isLoaded());

  console.log('   第二次显示图片1（已加载，不需要重新加载）...');
  image1.display();

  console.log('\n   --- 缓存代理 ---');
  const realService = new RealDataService();
  const cachedService = new CachedDataService(realService);

  // 模拟异步操作（同步演示中直接调用）
  console.log('   第一次获取数据（从远程）...');
  cachedService.getData('user/1').then(data1 => {
    console.log('   数据:', data1);
    console.log('   第二次获取相同数据（从缓存）...');
    return cachedService.getData('user/1');
  }).then(data2 => {
    console.log('   数据:', data2);
    console.log('   缓存大小:', cachedService.getCacheSize());
  });

  console.log('\n========== 设计模式演示结束 ==========\n');
}
