<?php
// ============================================================
// PHP 设计模式学习 - Patterns 类
// ============================================================
// 本文件包含常用的 PHP 设计模式示例
// 设计模式是解决常见软件设计问题的最佳实践
// ============================================================

namespace App\Patterns;

/**
 * Patterns 类 - PHP 设计模式演示
 * 
 * 包含以下设计模式：
 * - 单例模式（Singleton）
 * - 工厂模式（简单工厂 / 工厂方法）
 * - 观察者模式（Observer）
 * - 策略模式（Strategy）
 * - 装饰器模式（Decorator）
 * - 依赖注入容器（DI Container）
 */
class Patterns
{
    // ============================================================
    // 1. 单例模式
    // ============================================================

    /**
     * 演示单例模式
     * 
     * 单例模式（Singleton）：
     * - 确保一个类只有一个实例
     * - 提供一个全局访问点
     * - 常用于数据库连接、配置管理、日志记录等
     */
    public function singleton(): void
    {
        echo "=== 单例模式（Singleton）=== \n";
        echo "确保一个类只有一个实例，并提供全局访问点\n\n";

        echo "单例类的特点：\n";
        echo "  - 构造函数设为私有（禁止外部 new）\n";
        echo "  - 提供静态方法获取实例\n";
        echo "  - 实例存储在静态属性中\n";
        echo "  - 禁止克隆和反序列化\n\n";

        // 单例类定义
        $singletonClass = new class {
            // 存储单例实例的静态属性
            private static ?self $instance = null;

            // 实例属性
            private string $config;

            // 私有构造函数：禁止外部 new
            private function __construct()
            {
                $this->config = '配置数据';
                echo "    [单例] 构造函数被调用（只会执行一次）\n";
            }

            // 禁止克隆
            private function __clone() {}

            // 禁止反序列化
            public function __wakeup() {}

            /**
             * 获取单例实例的静态方法
             * 这是访问单例的唯一入口
             * 
             * @return self
             */
            public static function getInstance(): self
            {
                // 如果实例不存在，则创建
                if (self::$instance === null) {
                    self::$instance = new self();
                }
                return self::$instance;
            }

            // 业务方法
            public function getConfig(): string
            {
                return $this->config;
            }

            public function setConfig(string $config): void
            {
                $this->config = $config;
            }
        };

        echo "演示：\n";

        // 第一次获取实例（创建）
        echo "  第一次获取实例：\n";
        $instance1 = $singletonClass::getInstance();
        echo "    实例ID: " . spl_object_id($instance1) . "\n";
        echo "    配置: " . $instance1->getConfig() . "\n";

        // 修改配置
        $instance1->setConfig('新的配置数据');

        // 第二次获取实例（同一个）
        echo "  第二次获取实例：\n";
        $instance2 = $singletonClass::getInstance();
        echo "    实例ID: " . spl_object_id($instance2) . "\n";
        echo "    配置: " . $instance2->getConfig() . "（与第一个实例共享）\n";

        echo "\n  两个实例是否相同：" . ($instance1 === $instance2 ? '是（同一个对象）' : '否') . "\n";

        echo "\n适用场景：\n";
        echo "  - 数据库连接（避免多次连接）\n";
        echo "  - 配置管理器（全局唯一配置）\n";
        echo "  - 日志记录器（统一日志输出）\n";
        echo "  - 缓存管理器（统一缓存管理）\n";

        echo "\n";
    }

    // ============================================================
    // 2. 工厂模式
    // ============================================================

    /**
     * 演示工厂模式
     * 
     * 工厂模式（Factory）：
     * - 定义创建对象的接口，让子类决定实例化哪个类
     * - 将对象的创建和使用分离
     * - 降低耦合度，便于扩展
     */
    public function factory(): void
    {
        echo "=== 工厂模式（Factory）=== \n";

        // ---------- 产品接口 ----------
        interface Logger
        {
            public function log(string $message): string;
        }

        // 具体产品1
        $fileLoggerClass = new class implements Logger {
            public function log(string $message): string
            {
                return "[文件日志] {$message}";
            }
        };

        // 具体产品2
        $dbLoggerClass = new class implements Logger {
            public function log(string $message): string
            {
                return "[数据库日志] {$message}";
            }
        };

        // 具体产品3
        $emailLoggerClass = new class implements Logger {
            public function log(string $message): string
            {
                return "[邮件日志] {$message}";
            }
        };

        // ---------- 简单工厂 ----------
        echo "1. 简单工厂模式：\n";
        echo "   一个工厂类根据参数创建不同的产品\n\n";

        $simpleFactory = new class($fileLoggerClass, $dbLoggerClass, $emailLoggerClass) {
            private $fileLoggerClass;
            private $dbLoggerClass;
            private $emailLoggerClass;

            public function __construct($f, $d, $e)
            {
                $this->fileLoggerClass = $f;
                $this->dbLoggerClass = $d;
                $this->emailLoggerClass = $e;
            }

            /**
             * 工厂方法：根据类型创建对应的日志对象
             * 
             * @param string $type 日志类型
             * @return Logger
             */
            public function createLogger(string $type): Logger
            {
                return match ($type) {
                    'file'  => $this->fileLoggerClass,
                    'db'    => $this->dbLoggerClass,
                    'email' => $this->emailLoggerClass,
                    default => throw new \InvalidArgumentException("未知的日志类型：{$type}")
                };
            }
        };

        echo "   使用简单工厂：\n";
        $logger1 = $simpleFactory->createLogger('file');
        echo "     createLogger('file')  → " . $logger1->log('测试消息') . "\n";

        $logger2 = $simpleFactory->createLogger('db');
        echo "     createLogger('db')    → " . $logger2->log('测试消息') . "\n";

        $logger3 = $simpleFactory->createLogger('email');
        echo "     createLogger('email') → " . $logger3->log('测试消息') . "\n";

        // ---------- 工厂方法 ----------
        echo "\n2. 工厂方法模式：\n";
        echo "   定义工厂接口，每个具体工厂创建对应的产品\n";
        echo "   比简单工厂更灵活，符合开闭原则\n\n";

        // 工厂接口
        interface LoggerFactory
        {
            public function createLogger(): Logger;
        }

        // 具体工厂1
        $fileLoggerFactory = new class($fileLoggerClass) implements LoggerFactory {
            private $fileLoggerClass;
            public function __construct($c) { $this->fileLoggerClass = $c; }
            public function createLogger(): Logger
            {
                return $this->fileLoggerClass;
            }
        };

        // 具体工厂2
        $dbLoggerFactory = new class($dbLoggerClass) implements LoggerFactory {
            private $dbLoggerClass;
            public function __construct($c) { $this->dbLoggerClass = $c; }
            public function createLogger(): Logger
            {
                return $this->dbLoggerClass;
            }
        };

        echo "   使用工厂方法：\n";
        $factory = $fileLoggerFactory;
        $logger = $factory->createLogger();
        echo "     文件工厂 → " . $logger->log('你好') . "\n";

        $factory = $dbLoggerFactory;
        $logger = $factory->createLogger();
        echo "     数据库工厂 → " . $logger->log('你好') . "\n";

        echo "\n工厂模式优点：\n";
        echo "  - 解耦：对象创建和使用分离\n";
        echo "  - 扩展方便：新增产品只需添加工厂\n";
        echo "  - 屏蔽产品实现：调用者只关心接口\n";

        echo "\n";
    }

    // ============================================================
    // 3. 观察者模式
    // ============================================================

    /**
     * 演示观察者模式
     * 
     * 观察者模式（Observer）：
     * - 定义对象间的一对多依赖关系
     * - 当一个对象状态改变时，所有依赖它的对象都会收到通知
     * - PHP 内置了 SplSubject 和 SplObserver 接口
     */
    public function observer(): void
    {
        echo "=== 观察者模式（Observer）=== \n";
        echo "定义一对多的依赖关系，主题变化时通知所有观察者\n";
        echo "PHP 内置了 SplSubject 和 SplObserver 接口\n\n";

        // ---------- 主题（被观察者）----------
        echo "主题（Subject）- 被观察者：\n";
        echo "  维护观察者列表\n";
        echo "  提供添加/删除观察者的方法\n";
        echo "  状态变化时通知所有观察者\n\n";

        // 具体主题：用户登录管理器
        $userLogin = new class implements \SplSubject {
            // 存储观察者
            private \SplObjectStorage $observers;
            // 用户信息
            private string $username = '';
            private bool $isLoggedIn = false;

            public function __construct()
            {
                $this->observers = new \SplObjectStorage();
            }

            /**
             * 添加观察者
             */
            public function attach(\SplObserver $observer): void
            {
                $this->observers->attach($observer);
            }

            /**
             * 移除观察者
             */
            public function detach(\SplObserver $observer): void
            {
                $this->observers->detach($observer);
            }

            /**
             * 通知所有观察者
             */
            public function notify(): void
            {
                foreach ($this->observers as $observer) {
                    $observer->update($this);
                }
            }

            /**
             * 用户登录（触发状态变化）
             */
            public function login(string $username): void
            {
                $this->username = $username;
                $this->isLoggedIn = true;
                echo "  [主题] 用户 {$username} 登录了\n";
                echo "  [主题] 通知所有观察者...\n";
                $this->notify();  // 状态变化，通知观察者
            }

            // 获取状态的方法
            public function getUsername(): string { return $this->username; }
            public function isLoggedIn(): bool { return $this->isLoggedIn; }
        };

        // ---------- 观察者 ----------
        echo "观察者（Observer）：\n";
        echo "  实现 update() 方法，接收主题的通知\n\n";

        // 具体观察者1：日志记录
        $logObserver = new class implements \SplObserver {
            public function update(\SplSubject $subject): void
            {
                $username = $subject->getUsername();
                echo "    [观察者-日志] 记录登录日志：用户 {$username} 登录\n";
            }
        };

        // 具体观察者2：发送通知
        $notifyObserver = new class implements \SplObserver {
            public function update(\SplSubject $subject): void
            {
                $username = $subject->getUsername();
                echo "    [观察者-通知] 发送登录通知给 {$username}\n";
            }
        };

        // 具体观察者3：统计
        $statsObserver = new class implements \SplObserver {
            public int $loginCount = 0;

            public function update(\SplSubject $subject): void
            {
                if ($subject->isLoggedIn()) {
                    $this->loginCount++;
                    echo "    [观察者-统计] 登录次数 +1，总计：{$this->loginCount}\n";
                }
            }
        };

        // ---------- 使用演示 ----------
        echo "演示：\n";

        // 添加观察者
        $userLogin->attach($logObserver);
        $userLogin->attach($notifyObserver);
        $userLogin->attach($statsObserver);
        echo "  添加了 3 个观察者\n\n";

        // 触发登录（通知所有观察者）
        $userLogin->login('张三');

        echo "\n  移除统计观察者后再次登录：\n";
        $userLogin->detach($statsObserver);
        $userLogin->login('李四');

        echo "\n观察者模式优点：\n";
        echo "  - 主题和观察者松耦合\n";
        echo "  - 支持广播通信\n";
        echo "  - 符合开闭原则（新增观察者无需修改主题）\n";

        echo "\n适用场景：\n";
        echo "  - 事件监听系统\n";
        echo "  - 消息通知\n";
        echo "  - 数据变更同步\n";
        echo "  - MVC 模式（Model 变化通知 View 更新）\n";

        echo "\n";
    }

    // ============================================================
    // 4. 策略模式
    // ============================================================

    /**
     * 演示策略模式
     * 
     * 策略模式（Strategy）：
     * - 定义一系列算法，把它们封装起来，并且使它们可以互相替换
     * - 算法的变化独立于使用算法的客户
     * - 替代大量的 if-else 或 switch
     */
    public function strategy(): void
    {
        echo "=== 策略模式（Strategy）=== \n";
        echo "封装一系列算法，使它们可以互相替换\n";
        echo "算法变化独立于使用算法的客户端\n\n";

        // ---------- 策略接口 ----------
        echo "策略接口：\n\n";

        // 支付策略接口
        interface PaymentStrategy
        {
            /**
             * 支付方法
             * 
             * @param float $amount 支付金额
             * @return string 支付结果
             */
            public function pay(float $amount): string;

            /**
             * 获取支付方式名称
             */
            public function getName(): string;
        }

        // ---------- 具体策略 ----------
        echo "具体策略：\n\n";

        // 策略1：支付宝
        $alipay = new class implements PaymentStrategy {
            public function pay(float $amount): string
            {
                return "使用支付宝支付 ¥{$amount}";
            }
            public function getName(): string
            {
                return '支付宝';
            }
        };

        // 策略2：微信支付
        $wechatPay = new class implements PaymentStrategy {
            public function pay(float $amount): string
            {
                return "使用微信支付 ¥{$amount}";
            }
            public function getName(): string
            {
                return '微信支付';
            }
        };

        // 策略3：银行卡
        $bankCard = new class implements PaymentStrategy {
            public function pay(float $amount): string
            {
                return "使用银行卡支付 ¥{$amount}";
            }
            public function getName(): string
            {
                return '银行卡';
            }
        };

        echo "  - 支付宝支付策略\n";
        echo "  - 微信支付策略\n";
        echo "  - 银行卡支付策略\n\n";

        // ---------- 上下文（使用策略的类）----------
        echo "上下文（Context）：\n";
        echo "  持有策略对象的引用\n";
        echo "  将请求委托给策略对象处理\n\n";

        // 购物车类（上下文）
        $shoppingCart = new class {
            // 当前使用的支付策略
            private ?PaymentStrategy $paymentStrategy = null;

            /**
             * 设置支付策略（运行时可以切换）
             */
            public function setPaymentStrategy(PaymentStrategy $strategy): void
            {
                $this->paymentStrategy = $strategy;
            }

            /**
             * 结账（使用策略执行支付）
             */
            public function checkout(float $total): string
            {
                if ($this->paymentStrategy === null) {
                    throw new \RuntimeException('请先选择支付方式');
                }
                return $this->paymentStrategy->pay($total);
            }
        };

        // ---------- 使用演示 ----------
        echo "演示：\n";

        // 使用支付宝支付
        $shoppingCart->setPaymentStrategy($alipay);
        echo "  " . $shoppingCart->checkout(99.5) . "\n";

        // 切换为微信支付
        $shoppingCart->setPaymentStrategy($wechatPay);
        echo "  " . $shoppingCart->checkout(199.0) . "\n";

        // 切换为银行卡
        $shoppingCart->setPaymentStrategy($bankCard);
        echo "  " . $shoppingCart->checkout(299.0) . "\n";

        echo "\n策略模式优点：\n";
        echo "  - 避免大量 if-else / switch\n";
        echo "  - 算法可以独立变化和扩展\n";
        echo "  - 运行时可以切换策略\n";
        echo "  - 符合开闭原则\n";

        echo "\n与 if-else 对比：\n";
        echo "  // 传统方式（每加一种支付方式都要修改这里）\n";
        echo "  if (\$type == 'alipay') { ... }\n";
        echo "  elseif (\$type == 'wechat') { ... }\n";
        echo "  elseif (\$type == 'bank') { ... }\n\n";
        echo "  // 策略模式（新增只需加新策略类，无需修改原有代码）\n";
        echo "  \$cart->setPaymentStrategy(new Alipay());\n";
        echo "  \$cart->checkout(100);\n";

        echo "\n";
    }

    // ============================================================
    // 5. 装饰器模式
    // ============================================================

    /**
     * 演示装饰器模式
     * 
     * 装饰器模式（Decorator）：
     * - 动态地给一个对象添加一些额外的职责
     * - 比继承更灵活（可以组合多个装饰）
     * - 不改变原有类的结构
     */
    public function decorator(): void
    {
        echo "=== 装饰器模式（Decorator）=== \n";
        echo "动态地给对象添加额外功能\n";
        echo "比继承更灵活，可以组合多个装饰器\n\n";

        // ---------- 组件接口 ----------
        echo "组件接口：定义核心功能\n\n";

        // 咖啡接口
        interface Coffee
        {
            public function cost(): float;
            public function getDescription(): string;
        }

        // ---------- 具体组件 ----------
        echo "具体组件：基础实现\n\n";

        // 基础咖啡
        $simpleCoffee = new class implements Coffee {
            public function cost(): float
            {
                return 10.0;
            }
            public function getDescription(): string
            {
                return '基础咖啡';
            }
        };

        echo "  基础咖啡: ¥10\n\n";

        // ---------- 装饰器抽象类 ----------
        echo "装饰器抽象类：\n";
        echo "  持有组件引用\n";
        echo "  实现组件接口\n";
        echo "  将操作委托给组件，并添加额外功能\n\n";

        // 咖啡装饰器抽象类
        abstract class CoffeeDecorator implements Coffee
        {
            protected Coffee $coffee;  // 持有被装饰的对象

            public function __construct(Coffee $coffee)
            {
                $this->coffee = $coffee;
            }
        }

        // ---------- 具体装饰器 ----------
        echo "具体装饰器：\n\n";

        // 装饰器1：加牛奶
        $milkDecorator = new class($simpleCoffee) extends CoffeeDecorator {
            public function cost(): float
            {
                return $this->coffee->cost() + 3.0;  // 加牛奶 +3元
            }
            public function getDescription(): string
            {
                return $this->coffee->getDescription() . ' + 牛奶';
            }
        };

        // 装饰器2：加糖
        $sugarDecorator = function ($coffee) {
            return new class($coffee) extends CoffeeDecorator {
                public function cost(): float
                {
                    return $this->coffee->cost() + 1.0;  // 加糖 +1元
                }
                public function getDescription(): string
                {
                    return $this->coffee->getDescription() . ' + 糖';
                }
            };
        };

        // 装饰器3：加奶泡
        $whipDecorator = function ($coffee) {
            return new class($coffee) extends CoffeeDecorator {
                public function cost(): float
                {
                    return $this->coffee->cost() + 2.5;  // 加奶泡 +2.5元
                }
                public function getDescription(): string
                {
                    return $this->coffee->getDescription() . ' + 奶泡';
                }
            };
        };

        echo "  + 牛奶: ¥3\n";
        echo "  + 糖: ¥1\n";
        echo "  + 奶泡: ¥2.5\n\n";

        // ---------- 使用演示 ----------
        echo "演示：\n";

        // 基础咖啡
        echo "  1. 基础咖啡: " . $simpleCoffee->getDescription() . " = ¥" . $simpleCoffee->cost() . "\n";

        // 加牛奶
        $coffeeWithMilk = $milkDecorator;
        echo "  2. 咖啡 + 牛奶: " . $coffeeWithMilk->getDescription() . " = ¥" . $coffeeWithMilk->cost() . "\n";

        // 加牛奶 + 糖
        $coffeeWithMilkSugar = $sugarDecorator($coffeeWithMilk);
        echo "  3. 咖啡 + 牛奶 + 糖: " . $coffeeWithMilkSugar->getDescription() . " = ¥" . $coffeeWithMilkSugar->cost() . "\n";

        // 全部加
        $fullCoffee = $whipDecorator($sugarDecorator($milkDecorator));
        echo "  4. 咖啡 + 牛奶 + 糖 + 奶泡: " . $fullCoffee->getDescription() . " = ¥" . $fullCoffee->cost() . "\n";

        echo "\n装饰器模式优点：\n";
        echo "  - 比继承更灵活（运行时动态添加功能）\n";
        echo "  - 可以组合多个装饰器\n";
        echo "  - 符合开闭原则\n";
        echo "  - 装饰器和被装饰者可以独立变化\n";

        echo "\n与继承对比：\n";
        echo "  继承：每种组合一个类（类爆炸）\n";
        echo "    MilkCoffee, SugarCoffee, MilkSugarCoffee, WhipCoffee...\n\n";
        echo "  装饰器：动态组合，灵活搭配\n";
        echo "    new Whip(new Sugar(new Milk(new SimpleCoffee())))\n";

        echo "\n";
    }

    // ============================================================
    // 6. 依赖注入容器
    // ============================================================

    /**
     * 演示依赖注入容器（简单实现）
     * 
     * 依赖注入（Dependency Injection）：
     * - 类的依赖由外部注入，而不是自己创建
     * - 降低耦合，便于测试和扩展
     * - 容器负责创建和管理对象及其依赖
     */
    public function diContainer(): void
    {
        echo "=== 依赖注入容器（DI Container）=== \n";
        echo "依赖注入：类的依赖由外部注入，而非自己创建\n";
        echo "容器负责创建和管理对象及其依赖关系\n\n";

        // ---------- 服务类 ----------
        echo "服务类示例：\n\n";

        // 日志服务
        interface LoggerService
        {
            public function log(string $message): void;
        }

        $fileLoggerService = new class implements LoggerService {
            public function log(string $message): void
            {
                echo "      [文件日志] {$message}\n";
            }
        };

        // 数据库服务
        $databaseService = new class($fileLoggerService) {
            private LoggerService $logger;
            private bool $connected = false;

            public function __construct(LoggerService $logger)
            {
                $this->logger = $logger;
            }

            public function connect(): void
            {
                $this->connected = true;
                $this->logger->log('数据库已连接');
            }

            public function query(string $sql): void
            {
                $this->logger->log("执行查询: {$sql}");
            }

            public function isConnected(): bool
            {
                return $this->connected;
            }
        };

        // 用户服务（依赖数据库服务和日志服务）
        $userService = new class($databaseService, $fileLoggerService) {
            private $db;
            private $logger;

            public function __construct($db, LoggerService $logger)
            {
                $this->db = $db;
                $this->logger = $logger;
            }

            public function register(string $username): void
            {
                $this->logger->log("注册用户: {$username}");
                $this->db->query("INSERT INTO users (name) VALUES ('{$username}')");
            }
        };

        echo "  LoggerService    - 日志服务\n";
        echo "  DatabaseService  - 数据库服务（依赖 LoggerService）\n";
        echo "  UserService      - 用户服务（依赖 DatabaseService 和 LoggerService）\n\n";

        // ---------- 简单 DI 容器 ----------
        echo "简单 DI 容器实现：\n\n";

        $container = new class {
            // 存储已创建的实例（单例）
            private array $instances = [];
            // 存储服务定义（闭包）
            private array $definitions = [];

            /**
             * 注册服务定义
             * 
             * @param string $name 服务名称
             * @param callable $definition 创建服务的闭包
             */
            public function set(string $name, callable $definition): void
            {
                $this->definitions[$name] = $definition;
            }

            /**
             * 获取服务实例
             * 
             * @param string $name 服务名称
             * @return mixed
             */
            public function get(string $name): mixed
            {
                // 如果已经有实例，直接返回（单例）
                if (isset($this->instances[$name])) {
                    return $this->instances[$name];
                }

                // 如果有定义，创建实例
                if (isset($this->definitions[$name])) {
                    $instance = $this->definitions[$name]($this);
                    $this->instances[$name] = $instance;
                    return $instance;
                }

                throw new \Exception("服务 '{$name}' 未注册");
            }

            /**
             * 检查服务是否已注册
             */
            public function has(string $name): bool
            {
                return isset($this->definitions[$name]) || isset($this->instances[$name]);
            }
        };

        echo "  容器功能：\n";
        echo "    - set() 注册服务定义\n";
        echo "    - get() 获取服务实例（自动解析依赖）\n";
        echo "    - 自动单例（同一个服务只创建一次）\n\n";

        // ---------- 使用演示 ----------
        echo "演示：\n\n";

        // 注册服务
        echo "  1. 注册服务定义：\n";
        echo "     logger → FileLogger\n";
        echo "     db → DatabaseService（依赖 logger）\n";
        echo "     userService → UserService（依赖 db 和 logger）\n\n";

        $container->set('logger', function ($c) use ($fileLoggerService) {
            return $fileLoggerService;
        });

        $container->set('db', function ($c) use ($fileLoggerService) {
            // 从容器中获取 logger 依赖
            $db = new class($c->get('logger')) {
                private $logger;
                private $connected = false;
                public function __construct($l) { $this->logger = $l; }
                public function connect() { $this->connected = true; $this->logger->log('数据库已连接'); }
                public function query($sql) { $this->logger->log("执行查询: {$sql}"); }
                public function isConnected() { return $this->connected; }
            };
            $db->connect();
            return $db;
        });

        $container->set('userService', function ($c) {
            return new class($c->get('db'), $c->get('logger')) {
                private $db;
                private $logger;
                public function __construct($db, $l) { $this->db = $db; $this->logger = $l; }
                public function register($username) {
                    $this->logger->log("注册用户: {$username}");
                    $this->db->query("INSERT INTO users (name) VALUES ('{$username}')");
                }
            };
        });

        echo "  2. 从容器获取 userService（自动解析所有依赖）：\n";
        $user = $container->get('userService');
        $user->register('张三');

        echo "\n  3. 再次获取 db（单例，同一个实例）：\n";
        $db = $container->get('db');
        echo "     数据库连接状态: " . ($db->isConnected() ? '已连接（复用同一个实例）' : '未连接') . "\n";

        echo "\n依赖注入优点：\n";
        echo "  - 解耦：类不需要知道依赖如何创建\n";
        echo "  - 易于测试：可以轻松替换为 Mock 对象\n";
        echo "  - 易于扩展：更换实现只需修改容器配置\n";
        echo "  - 统一管理：对象生命周期由容器统一管理\n";

        echo "\nDI 容器的核心功能：\n";
        echo "  - 服务注册（bind/set）\n";
        echo "  - 服务解析（make/get）\n";
        echo "  - 自动注入（通过反射分析依赖）\n";
        echo "  - 生命周期管理（单例/原型）\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有设计模式演示
     */
    public function runAll(): void
    {
        $this->singleton();
        $this->factory();
        $this->observer();
        $this->strategy();
        $this->decorator();
        $this->diContainer();
    }
}
