/**
 * ============================================================
 * 文件名：design_patterns.js
 * 描述：JavaScript 设计模式详解 - 单例、工厂、观察者、策略、装饰器、代理模式
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件讲解 JavaScript 中常用的 6 种设计模式：
 * 1. 单例模式（Singleton）
 * 2. 工厂模式（Factory）
 * 3. 观察者模式（Observer / 发布-订阅）
 * 4. 策略模式（Strategy）
 * 5. 装饰器模式（Decorator）
 * 6. 代理模式（Proxy）
 *
 * 运行方式：node patterns/design_patterns.js
 */

'use strict';

module.exports = function demoDesignPatterns() {
  console.log('='.repeat(60));
  console.log('一、单例模式（Singleton）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 单例模式介绍
  // ------------------------------------------------------------
  // 单例模式保证一个类只有一个实例，并提供一个访问它的全局访问点
  // 应用场景：全局状态管理、配置对象、数据库连接池、日志对象等
  console.log('\n--- 1.1 单例模式介绍 ---');
  console.log('  定义：保证一个类只有一个实例，并提供全局访问点');
  console.log('  应用：全局状态管理、配置对象、数据库连接、日志系统等');
  console.log('  核心：构造函数只能被 new 一次，后续都返回同一个实例');

  // ------------------------------------------------------------
  // 1.2 闭包实现单例
  // ------------------------------------------------------------
  console.log('\n--- 1.2 闭包实现单例 ---');

  const SingletonClosure = (function () {
    // 闭包中的私有变量，保存唯一实例
    let instance = null;

    // 真正的构造函数（对外不可见）
    function Singleton(name) {
      this.name = name;
      this.createTime = Date.now();
    }

    Singleton.prototype.sayName = function () {
      console.log(`  单例名称: ${this.name}`);
    };

    // 返回获取实例的方法
    return {
      getInstance: function (name) {
        // 只有第一次调用时创建实例
        if (instance === null) {
          instance = new Singleton(name);
        }
        return instance;
      }
    };
  })();

  const s1 = SingletonClosure.getInstance('第一个实例');
  const s2 = SingletonClosure.getInstance('第二个实例');
  console.log('  s1 === s2:', s1 === s2); // true（同一个实例）
  console.log('  s1.name:', s1.name);     // '第一个实例'（第一次创建时的名字）
  s1.sayName();

  // ------------------------------------------------------------
  // 1.3 ES6 class 实现单例
  // ------------------------------------------------------------
  console.log('\n--- 1.3 ES6 class 实现单例 ---');

  class SingletonClass {
    // 静态属性，保存唯一实例
    static instance = null;

    constructor(name) {
      // 如果已经有实例，直接返回
      if (SingletonClass.instance) {
        return SingletonClass.instance;
      }
      // 第一次创建，初始化
      this.name = name;
      this.data = {};
      SingletonClass.instance = this;
    }

    setData(key, value) {
      this.data[key] = value;
    }

    getData(key) {
      return this.data[key];
    }
  }

  const sc1 = new SingletonClass('Class 单例');
  const sc2 = new SingletonClass('另一个');
  console.log('  sc1 === sc2:', sc1 === sc2); // true
  sc1.setData('count', 100);
  console.log('  sc2.getData("count"):', sc2.getData('count')); // 100（共享数据）

  // ------------------------------------------------------------
  // 1.4 模块模式单例（最简单）
  // ------------------------------------------------------------
  console.log('\n--- 1.4 模块模式单例 ---');

  // 利用 ES6 模块的特性，import 的模块只会执行一次
  // 直接导出一个对象，天然就是单例
  const globalConfig = {
    appName: '学习项目',
    version: '1.0.0',
    env: 'development',
    getConfig(key) {
      return this[key];
    }
  };
  console.log('  全局配置单例:');
  console.log('    appName:', globalConfig.appName);
  console.log('    version:', globalConfig.version);
  console.log('  （模块导出的对象天然是单例）');

  // ============================================================
  // 第二部分：工厂模式（Factory）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、工厂模式（Factory）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 工厂模式介绍
  // ------------------------------------------------------------
  // 工厂模式定义一个创建对象的接口，让子类决定实例化哪个类
  // 将对象的创建和使用分离，降低耦合
  // 应用场景：需要根据不同条件创建不同对象时
  console.log('\n--- 2.1 工厂模式介绍 ---');
  console.log('  定义：定义创建对象的接口，由工厂决定实例化哪个类');
  console.log('  好处：将对象创建与使用分离，降低耦合，易于扩展');
  console.log('  分类：简单工厂、工厂方法、抽象工厂');

  // ------------------------------------------------------------
  // 2.2 简单工厂模式
  // ------------------------------------------------------------
  // 一个工厂对象根据不同参数创建不同产品
  console.log('\n--- 2.2 简单工厂模式 ---');

  // 产品基类
  class Animal {
    constructor(name) {
      this.name = name;
    }
    speak() {
      // 抽象方法，子类实现
      throw new Error('子类必须实现 speak 方法');
    }
  }

  // 具体产品类
  class Dog extends Animal {
    speak() {
      console.log(`  ${this.name}: 汪汪汪！`);
    }
    fetch() {
      console.log(`  ${this.name} 去捡球了`);
    }
  }

  class Cat extends Animal {
    speak() {
      console.log(`  ${this.name}: 喵喵喵~`);
    }
    scratch() {
      console.log(`  ${this.name} 在磨爪子`);
    }
  }

  class Bird extends Animal {
    speak() {
      console.log(`  ${this.name}: 叽叽喳喳`);
    }
    fly() {
      console.log(`  ${this.name} 飞走了`);
    }
  }

  // 简单工厂：一个方法根据类型创建不同对象
  class AnimalFactory {
    // 静态方法，直接通过类名调用
    static createAnimal(type, name) {
      switch (type) {
        case 'dog':
          return new Dog(name);
        case 'cat':
          return new Cat(name);
        case 'bird':
          return new Bird(name);
        default:
          throw new Error(`不支持的动物类型: ${type}`);
      }
    }
  }

  // 使用工厂创建对象，不需要知道具体的类名
  const myDog = AnimalFactory.createAnimal('dog', '旺财');
  const myCat = AnimalFactory.createAnimal('cat', '咪咪');
  const myBird = AnimalFactory.createAnimal('bird', '小黄');

  myDog.speak();
  myCat.speak();
  myBird.speak();

  console.log('  调用者不需要知道 Dog/Cat/Bird 类，只需要和工厂打交道');

  // ------------------------------------------------------------
  // 2.3 工厂方法模式
  // ------------------------------------------------------------
  // 定义工厂基类，每个具体产品有对应的具体工厂
  // 比简单工厂更灵活，增加新产品只需要增加对应的工厂，不需要修改已有代码
  console.log('\n--- 2.3 工厂方法模式 ---');

  // 工厂基类
  class AnimalFactoryBase {
    createAnimal(name) {
      // 工厂方法，由子类实现
      throw new Error('子类必须实现 createAnimal 方法');
    }
  }

  // 具体工厂：狗工厂
  class DogFactory extends AnimalFactoryBase {
    createAnimal(name) {
      return new Dog(name);
    }
  }

  // 具体工厂：猫工厂
  class CatFactory extends AnimalFactoryBase {
    createAnimal(name) {
      return new Cat(name);
    }
  }

  // 使用
  const dogFactory = new DogFactory();
  const dog = dogFactory.createAnimal('小黑');
  dog.speak();

  const catFactory = new CatFactory();
  const cat = catFactory.createAnimal('小白');
  cat.speak();

  console.log('  增加新动物时，只需添加产品类和对应的工厂类，符合开闭原则');

  // ============================================================
  // 第三部分：观察者模式（Observer）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、观察者模式（发布-订阅）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 观察者模式介绍
  // ------------------------------------------------------------
  // 观察者模式定义了对象间一对多的依赖关系
  // 当一个对象（主题/发布者）状态改变时，所有依赖它的对象（观察者/订阅者）都会收到通知
  // 应用场景：事件系统、消息队列、MVC 模式、Vue/React 的响应式等
  console.log('\n--- 3.1 观察者模式介绍 ---');
  console.log('  定义：对象间一对多的依赖关系，主题变化时通知所有观察者');
  console.log('  别名：发布-订阅模式（Publish-Subscribe）');
  console.log('  组成：主题（Subject）+ 观察者（Observer）');
  console.log('  应用：事件系统、消息队列、响应式框架等');

  // ------------------------------------------------------------
  // 3.2 经典观察者模式实现
  // ------------------------------------------------------------
  console.log('\n--- 3.2 经典观察者模式 ---');

  /**
   * 主题（Subject）类：被观察的对象
   * 维护观察者列表，提供添加、删除、通知观察者的方法
   */
  class Subject {
    constructor() {
      this.observers = []; // 观察者列表
    }

    /**
     * 添加观察者
     * @param {Object} observer - 观察者对象，必须有 update 方法
     */
    addObserver(observer) {
      // 检查是否已经添加过
      if (!this.observers.includes(observer)) {
        this.observers.push(observer);
      }
    }

    /**
     * 移除观察者
     * @param {Object} observer
     */
    removeObserver(observer) {
      const index = this.observers.indexOf(observer);
      if (index !== -1) {
        this.observers.splice(index, 1);
      }
    }

    /**
     * 通知所有观察者
     * @param {*} data - 通知的数据
     */
    notifyObservers(data) {
      this.observers.forEach(observer => {
        observer.update(data); // 调用观察者的 update 方法
      });
    }
  }

  /**
   * 观察者（Observer）类
   * 实现 update 方法，在收到通知时执行
   */
  class Observer {
    constructor(name) {
      this.name = name;
    }

    update(data) {
      console.log(`  观察者 ${this.name} 收到通知: ${data}`);
    }
  }

  // 具体主题：新闻发布器
  class NewsPublisher extends Subject {
    constructor() {
      super();
      this.news = '';
    }

    publishNews(news) {
      this.news = news;
      console.log(`  [发布新闻]: ${news}`);
      this.notifyObservers(news); // 发布新闻时通知所有观察者
    }
  }

  // 使用示例
  const publisher = new NewsPublisher();

  const reader1 = new Observer('读者A');
  const reader2 = new Observer('读者B');
  const reader3 = new Observer('读者C');

  publisher.addObserver(reader1);
  publisher.addObserver(reader2);

  publisher.publishNews('JavaScript 发布新版本！');

  console.log('  --- 移除读者A，添加读者C ---');
  publisher.removeObserver(reader1);
  publisher.addObserver(reader3);

  publisher.publishNews('TypeScript 5.0 正式发布');

  // ------------------------------------------------------------
  // 3.3 发布-订阅模式（Event Emitter）
  // ------------------------------------------------------------
  // 更灵活的观察者模式，支持按事件类型订阅
  // 这是 JavaScript 中最常用的模式，Node.js 的 EventEmitter 就是这种
  console.log('\n--- 3.3 发布-订阅模式（Event Emitter）---');

  /**
   * 事件发射器（简化版 EventEmitter）
   * 支持按事件名订阅和发布
   */
  class EventEmitter {
    constructor() {
      // 事件映射表：{ eventName: [callback1, callback2, ...] }
      this.events = {};
    }

    /**
     * 订阅事件
     * @param {string} eventName - 事件名
     * @param {Function} callback - 回调函数
     */
    on(eventName, callback) {
      // 如果该事件还没有订阅者，创建数组
      if (!this.events[eventName]) {
        this.events[eventName] = [];
      }
      this.events[eventName].push(callback);
      return this; // 支持链式调用
    }

    /**
     * 取消订阅
     * @param {string} eventName
     * @param {Function} callback
     */
    off(eventName, callback) {
      if (!this.events[eventName]) return this;

      // 过滤掉要移除的回调
      this.events[eventName] = this.events[eventName].filter(
        fn => fn !== callback
      );
      return this;
    }

    /**
     * 只订阅一次
     * @param {string} eventName
     * @param {Function} callback
     */
    once(eventName, callback) {
      // 创建一个包装函数，执行后自动取消订阅
      const wrapper = (...args) => {
        callback(...args);
        this.off(eventName, wrapper);
      };
      this.on(eventName, wrapper);
      return this;
    }

    /**
     * 发布事件（触发）
     * @param {string} eventName
     * @param  {...any} args - 传递给回调的参数
     */
    emit(eventName, ...args) {
      if (!this.events[eventName]) return false;

      // 执行该事件的所有回调
      this.events[eventName].forEach(callback => {
        callback(...args);
      });
      return true;
    }
  }

  // 使用示例
  const emitter = new EventEmitter();

  // 订阅 'data' 事件
  emitter.on('data', data => {
    console.log('  监听器1收到数据:', data);
  });

  const listener2 = data => {
    console.log('  监听器2收到数据:', data);
  };
  emitter.on('data', listener2);

  // 订阅 'error' 事件
  emitter.on('error', err => {
    console.log('  发生错误:', err.message);
  });

  // 订阅一次
  emitter.once('login', user => {
    console.log('  用户首次登录:', user);
  });

  console.log('  --- 第一次 emit data ---');
  emitter.emit('data', 'Hello World');

  console.log('  --- 移除监听器2，第二次 emit data ---');
  emitter.off('data', listener2);
  emitter.emit('data', 'Second message');

  console.log('  --- emit error ---');
  emitter.emit('error', new Error('测试错误'));

  console.log('  --- 第一次 emit login ---');
  emitter.emit('login', '张三');
  console.log('  --- 第二次 emit login（once 已移除，无输出）---');
  emitter.emit('login', '李四');

  // ============================================================
  // 第四部分：策略模式（Strategy）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、策略模式（Strategy）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 策略模式介绍
  // ------------------------------------------------------------
  // 策略模式定义了一系列算法，将每个算法封装起来，使它们可以互相替换
  // 算法的变化独立于使用算法的客户
  // 应用场景：多种支付方式、多种排序算法、表单校验规则等
  console.log('\n--- 4.1 策略模式介绍 ---');
  console.log('  定义：定义一系列算法，封装起来，使它们可以互相替换');
  console.log('  好处：算法可以自由切换，避免大量 if-else，易于扩展');
  console.log('  组成：策略类（封装具体算法）+ 环境类（持有策略引用）');

  // ------------------------------------------------------------
  // 4.2 策略模式实现（计算奖金）
  // ------------------------------------------------------------
  console.log('\n--- 4.2 策略模式：奖金计算 ---');

  // 策略类：各种奖金计算算法
  const bonusStrategies = {
    // 绩效 S：4 倍工资
    S: salary => salary * 4,
    // 绩效 A：3 倍工资
    A: salary => salary * 3,
    // 绩效 B：2 倍工资
    B: salary => salary * 2,
    // 绩效 C：1 倍工资
    C: salary => salary * 1,
    // 绩效 D：没有奖金
    D: () => 0
  };

  // 环境类：奖金计算器
  class BonusCalculator {
    constructor(strategy) {
      this.strategy = strategy; // 当前使用的策略
    }

    // 切换策略
    setStrategy(strategy) {
      this.strategy = strategy;
    }

    // 计算奖金
    calculate(salary) {
      return this.strategy(salary);
    }
  }

  // 使用
  const calculator = new BonusCalculator(bonusStrategies.A);
  console.log('  绩效 A，工资 10000，奖金:', calculator.calculate(10000)); // 30000

  calculator.setStrategy(bonusStrategies.S);
  console.log('  绩效 S，工资 10000，奖金:', calculator.calculate(10000)); // 40000

  calculator.setStrategy(bonusStrategies.D);
  console.log('  绩效 D，工资 10000，奖金:', calculator.calculate(10000)); // 0

  // 增加新策略非常简单，不需要修改已有代码
  bonusStrategies['S+'] = salary => salary * 6;
  calculator.setStrategy(bonusStrategies['S+']);
  console.log('  绩效 S+，工资 10000，奖金:', calculator.calculate(10000)); // 60000

  // ------------------------------------------------------------
  // 4.3 策略模式应用：表单验证
  // ------------------------------------------------------------
  console.log('\n--- 4.3 策略模式：表单验证 ---');

  // 验证策略
  const validateStrategies = {
    // 非空验证
    isNonEmpty(value, errorMsg) {
      if (value === '' || value == null) {
        return errorMsg;
      }
    },
    // 最小长度验证
    minLength(value, length, errorMsg) {
      if (value.length < length) {
        return errorMsg;
      }
    },
    // 手机号验证
    isMobile(value, errorMsg) {
      if (!/^1[3-9]\d{9}$/.test(value)) {
        return errorMsg;
      }
    },
    // 邮箱验证
    isEmail(value, errorMsg) {
      if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(value)) {
        return errorMsg;
      }
    }
  };

  // 验证器类
  class Validator {
    constructor() {
      this.rules = []; // 验证规则列表
    }

    /**
     * 添加验证规则
     * @param {*} value - 要验证的值
     * @param {Array} rules - 规则数组，每项包含 strategy 和 errorMsg
     */
    add(value, rules) {
      rules.forEach(rule => {
        this.rules.push({
          value,
          strategy: rule.strategy,
          params: rule.params || [],
          errorMsg: rule.errorMsg
        });
      });
    }

    /**
     * 执行验证
     * @returns {string|undefined} 第一个错误信息，全部通过返回 undefined
     */
    validate() {
      for (const rule of this.rules) {
        const strategyFn = validateStrategies[rule.strategy];
        if (!strategyFn) {
          throw new Error(`未知的验证策略: ${rule.strategy}`);
        }
        // 调用策略方法，传入值 + 参数 + 错误信息
        const errorMsg = strategyFn(rule.value, ...rule.params, rule.errorMsg);
        if (errorMsg) {
          return errorMsg; // 发现错误，立即返回
        }
      }
    }
  }

  // 使用示例
  function validateForm(formData) {
    const validator = new Validator();

    // 用户名验证
    validator.add(formData.username, [
      { strategy: 'isNonEmpty', errorMsg: '用户名不能为空' },
      { strategy: 'minLength', params: [3], errorMsg: '用户名长度不能少于 3 位' }
    ]);

    // 手机号验证
    validator.add(formData.phone, [
      { strategy: 'isNonEmpty', errorMsg: '手机号不能为空' },
      { strategy: 'isMobile', errorMsg: '请输入正确的手机号' }
    ]);

    // 邮箱验证
    validator.add(formData.email, [
      { strategy: 'isEmail', errorMsg: '请输入正确的邮箱' }
    ]);

    return validator.validate();
  }

  const formData = {
    username: 'ab',
    phone: '1380013800',
    email: 'invalid-email'
  };

  const errorMsg = validateForm(formData);
  console.log('  表单验证结果:', errorMsg || '验证通过');

  // ============================================================
  // 第五部分：装饰器模式（Decorator）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、装饰器模式（Decorator）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 装饰器模式介绍
  // ------------------------------------------------------------
  // 装饰器模式动态地给对象添加额外的职责，而不改变其结构
  // 比继承更灵活，可以按需组合功能
  // 应用场景：日志记录、性能统计、权限校验、缓存等
  console.log('\n--- 5.1 装饰器模式介绍 ---');
  console.log('  定义：动态地给对象添加额外的功能，不改变其结构');
  console.log('  好处：比继承更灵活，可以按需组合，符合开闭原则');
  console.log('  类比：就像给手机装手机壳，可以加保护、加支架、加挂绳，功能可以叠加');

  // ------------------------------------------------------------
  // 5.2 函数装饰器
  // ------------------------------------------------------------
  console.log('\n--- 5.2 函数装饰器 ---');

  // 原始函数：计算两数之和
  function add(a, b) {
    return a + b;
  }

  // 装饰器函数：给函数添加日志功能
  function withLogging(fn, name) {
    return function (...args) {
      console.log(`  [日志] ${name} 被调用，参数:`, args);
      const result = fn.apply(this, args);
      console.log(`  [日志] ${name} 返回结果:`, result);
      return result;
    };
  }

  // 装饰器函数：给函数添加性能统计
  function withPerformance(fn, name) {
    return function (...args) {
      const start = Date.now();
      const result = fn.apply(this, args);
      const end = Date.now();
      console.log(`  [性能] ${name} 耗时: ${end - start}ms`);
      return result;
    };
  }

  // 装饰 add 函数
  const loggedAdd = withLogging(add, 'add');
  console.log('  调用带日志的 add:');
  loggedAdd(3, 5);

  // 多层装饰（叠加功能）
  const perfLoggedAdd = withPerformance(withLogging(add, 'add'), 'add');
  console.log('  调用带性能+日志的 add:');
  perfLoggedAdd(10, 20);

  // ------------------------------------------------------------
  // 5.3 类装饰器（装饰器模式）
  // ------------------------------------------------------------
  console.log('\n--- 5.3 类装饰器 ---');

  // 基础组件：咖啡
  class Coffee {
    getCost() {
      return 10; // 基础咖啡 10 元
    }
    getDescription() {
      return '基础咖啡';
    }
  }

  // 装饰器基类：持有被装饰对象的引用
  class CoffeeDecorator {
    constructor(coffee) {
      this.coffee = coffee; // 被装饰的对象
    }

    getCost() {
      return this.coffee.getCost(); // 委托给被装饰对象
    }

    getDescription() {
      return this.coffee.getDescription();
    }
  }

  // 具体装饰器：加牛奶
  class MilkDecorator extends CoffeeDecorator {
    getCost() {
      return super.getCost() + 3; // 加 3 元
    }
    getDescription() {
      return super.getDescription() + ' + 牛奶';
    }
  }

  // 具体装饰器：加糖
  class SugarDecorator extends CoffeeDecorator {
    getCost() {
      return super.getCost() + 1; // 加 1 元
    }
    getDescription() {
      return super.getDescription() + ' + 糖';
    }
  }

  // 具体装饰器：加奶泡
  class WhipDecorator extends CoffeeDecorator {
    getCost() {
      return super.getCost() + 5; // 加 5 元
    }
    getDescription() {
      return super.getDescription() + ' + 奶泡';
    }
  }

  // 使用：可以自由组合装饰
  let coffee = new Coffee();
  console.log(`  ${coffee.getDescription()}: ${coffee.getCost()} 元`);

  // 加牛奶
  coffee = new MilkDecorator(coffee);
  console.log(`  ${coffee.getDescription()}: ${coffee.getCost()} 元`);

  // 再加糖
  coffee = new SugarDecorator(coffee);
  console.log(`  ${coffee.getDescription()}: ${coffee.getCost()} 元`);

  // 再加奶泡
  coffee = new WhipDecorator(coffee);
  console.log(`  ${coffee.getDescription()}: ${coffee.getCost()} 元`);

  // ============================================================
  // 第六部分：代理模式（Proxy）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('六、代理模式（Proxy）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 6.1 代理模式介绍
  // ------------------------------------------------------------
  // 代理模式为其他对象提供一种代理，以控制对这个对象的访问
  // 代理对象在客户端和目标对象之间起到中介作用
  // 应用场景：虚拟代理、缓存代理、保护代理、远程代理等
  console.log('\n--- 6.1 代理模式介绍 ---');
  console.log('  定义：为对象提供代理，控制对该对象的访问');
  console.log('  组成：抽象主题 + 真实主题 + 代理');
  console.log('  分类：虚拟代理、缓存代理、保护代理、远程代理等');
  console.log('  ES6 提供了原生 Proxy 对象，可以方便地实现代理');

  // ------------------------------------------------------------
  // 6.2 虚拟代理（图片懒加载）
  // ------------------------------------------------------------
  // 虚拟代理：在需要的时候才创建真实对象
  // 经典应用：图片懒加载，先显示占位图，图片加载完成后再替换
  console.log('\n--- 6.2 虚拟代理：图片懒加载 ---');

  // 真实图片类
  class RealImage {
    constructor(url) {
      this.url = url;
      console.log(`  正在加载图片: ${url}`);
      // 模拟加载耗时
      this.loaded = false;
      setTimeout(() => {
        this.loaded = true;
        console.log(`  图片加载完成: ${url}`);
      }, 50);
    }

    display() {
      if (this.loaded) {
        console.log(`  显示图片: ${this.url}`);
      } else {
        console.log(`  图片加载中，请稍候...`);
      }
    }
  }

  // 代理图片类（虚拟代理）
  class ProxyImage {
    constructor(url) {
      this.url = url;
      this.realImage = null; // 初始时不创建真实对象
    }

    display() {
      if (this.realImage === null) {
        // 第一次访问时才创建真实对象
        console.log(`  [代理] 首次访问，开始加载图片...`);
        this.realImage = new RealImage(this.url);
      }
      this.realImage.display();
    }
  }

  console.log('  创建代理图片对象（未加载）');
  const image = new ProxyImage('https://example.com/photo.jpg');
  console.log('  第一次调用 display（触发加载）：');
  image.display();
  console.log('  第二次调用 display（已加载）：');
  setTimeout(() => {
    image.display();
  }, 100);

  // ------------------------------------------------------------
  // 6.3 缓存代理
  // ------------------------------------------------------------
  // 缓存代理：为开销大的运算结果提供缓存，下次调用直接返回缓存
  console.log('\n--- 6.3 缓存代理 ---');

  /**
   * 创建缓存代理
   * @param {Function} fn - 要缓存的函数
   * @returns {Function} 带缓存的函数
   */
  function createCacheProxy(fn) {
    const cache = new Map(); // 缓存存储

    return function (...args) {
      // 将参数序列化为缓存键（简单情况）
      const key = JSON.stringify(args);

      if (cache.has(key)) {
        console.log(`  [缓存命中] 参数: ${key}`);
        return cache.get(key);
      }

      console.log(`  [缓存未命中] 计算中... 参数: ${key}`);
      const result = fn.apply(this, args);
      cache.set(key, result);
      return result;
    };
  }

  // 一个耗时的计算函数
  function expensiveCalculation(a, b) {
    // 模拟复杂计算
    return a * b + Math.pow(a, b) % 1000;
  }

  // 创建带缓存的版本
  const cachedCalculation = createCacheProxy(expensiveCalculation);

  console.log('  第一次调用：');
  console.log('  结果:', cachedCalculation(3, 4));

  console.log('  第二次调用（相同参数）：');
  console.log('  结果:', cachedCalculation(3, 4));

  console.log('  第三次调用（不同参数）：');
  console.log('  结果:', cachedCalculation(5, 2));

  // ------------------------------------------------------------
  // 6.4 保护代理（权限控制）
  // ------------------------------------------------------------
  // 保护代理：控制对真实对象的访问权限
  console.log('\n--- 6.4 保护代理：权限控制 ---');

  // 真实对象：用户数据
  const userData = {
    name: '张三',
    age: 28,
    password: 'secret123', // 敏感信息
    email: 'zhangsan@example.com',
    role: 'user'
  };

  // 保护代理：使用 ES6 Proxy 实现
  const protectedUser = new Proxy(userData, {
    // 拦截读取操作
    get(target, prop) {
      // 敏感属性禁止访问
      if (prop === 'password') {
        console.log('  [保护代理] 无权访问敏感属性: password');
        return '******';
      }
      return target[prop];
    },
    // 拦截修改操作
    set(target, prop, value) {
      // 角色属性不能修改
      if (prop === 'role') {
        console.log('  [保护代理] 无权修改角色');
        return false;
      }
      // 年龄必须是合理值
      if (prop === 'age') {
        if (typeof value !== 'number' || value < 0 || value > 150) {
          console.log('  [保护代理] 年龄值不合法');
          return false;
        }
      }
      target[prop] = value;
      return true;
    },
    // 拦截删除操作
    deleteProperty(target, prop) {
      if (prop === 'name' || prop === 'email') {
        console.log(`  [保护代理] 不能删除属性: ${prop}`);
        return false;
      }
      delete target[prop];
      return true;
    }
  });

  console.log('  访问 name:', protectedUser.name);
  console.log('  访问 password:', protectedUser.password); // 被保护
  console.log('  修改 age 为 30:');
  protectedUser.age = 30;
  console.log('  age:', protectedUser.age);
  console.log('  修改 role 为 admin:');
  protectedUser.role = 'admin'; // 被拒绝
  console.log('  role:', protectedUser.role);

  // ============================================================
  // 设计模式总结
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('七、设计模式总结');
  console.log('='.repeat(60));

  console.log(`
  创建型模式（关注对象创建）：
    - 单例模式：保证唯一实例
    - 工厂模式：封装对象创建过程

  结构型模式（关注类/对象的组合）：
    - 代理模式：控制对象访问
    - 装饰器模式：动态添加功能
    - 适配器模式、桥接模式、组合模式、享元模式...

  行为型模式（关注对象间的通信）：
    - 观察者模式：一对多通知
    - 策略模式：算法封装与切换
    - 命令模式、迭代器模式、模板方法、状态模式...

  设计原则（SOLID）：
    S - 单一职责原则：一个类只做一件事
    O - 开闭原则：对扩展开放，对修改关闭
    L - 里氏替换原则：子类可以替换父类
    I - 接口隔离原则：接口要小而专一
    D - 依赖倒置原则：依赖抽象，不依赖具体
`);

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('='.repeat(60));
  console.log('设计模式 演示结束');
  console.log('（注意：部分异步输出可能在后面）');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
