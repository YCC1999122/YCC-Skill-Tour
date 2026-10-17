<?php
// ============================================================
// PHP 面向对象学习 - Oop 类
// ============================================================
// 本文件是 OOP 模块的主演示类，整合所有面向对象知识点
// ============================================================

namespace App\Oop;

// 引入其他类（通过 use 关键字导入命名空间中的类）
// 也可以使用完全限定名称（如 \App\Oop\Person）直接访问
use App\Oop\Person;
use App\Oop\Student;
use App\Oop\UserInterface;
use App\Oop\AdminInterface;
use App\Oop\LoggerAwareInterface;
use App\Oop\LoggerTrait;
use App\Oop\TimestampTrait;
use App\Oop\Status;
use App\Oop\UserRole;
use App\Oop\MagicClass;

/**
 * Oop 类 - PHP 面向对象编程演示
 * 
 * 包含以下知识点：
 * - 类与对象
 * - 构造/析构函数
 * - 访问控制
 * - 继承
 * - 抽象类与接口
 * - Trait 代码复用
 * - 命名空间与自动加载
 * - 静态成员与后期静态绑定
 * - 类常量
 * - 魔术方法
 * - 匿名类
 * - 类型属性
 * - 枚举
 */
class Oop
{
    // ============================================================
    // 1. 类与对象
    // ============================================================

    /**
     * 演示类的实例化和基本使用
     */
    public function classesAndObjects(): void
    {
        echo "=== 类与对象 ===\n";

        // 使用 new 关键字创建对象（实例化）
        $person1 = new Person('张三', 25, '110101199901011234');
        $person2 = new Person('李四', 30, '110101199401015678');

        echo "创建了两个 Person 对象\n";

        // 访问对象的公开属性
        echo "访问公开属性：\n";
        echo "  person1->name = {$person1->name}\n";
        echo "  person2->name = {$person2->name}\n";

        // 调用对象的方法
        echo "\n调用对象方法：\n";
        echo "  " . $person1->say('你好！') . "\n";
        echo "  " . $person2->say('Hello!') . "\n";

        // 类常量访问
        echo "\n类常量：\n";
        echo "  Person::SPECIES = " . Person::SPECIES . "\n";

        // 静态属性和方法
        echo "\n静态属性和方法：\n";
        echo "  Person::\$count = " . Person::$count . "（已创建的对象数）\n";
        echo "  Person::getCount() = " . Person::getCount() . "\n";

        echo "\n";
    }

    // ============================================================
    // 2. 构造函数与析构函数
    // ============================================================

    /**
     * 演示构造函数和析构函数
     */
    public function constructAndDestruct(): void
    {
        echo "=== 构造函数与析构函数 ===\n";
        echo "构造函数在对象创建时自动调用\n";
        echo "析构函数在对象销毁时自动调用\n\n";

        // 创建对象时触发构造函数
        echo "创建对象：\n";
        $person = new Person('王五', 28, '110101199601019012');
        echo "\n";

        // 主动销毁对象（触发析构函数）
        echo "销毁对象：\n";
        unset($person);  // 或 $person = null;
        echo "  对象已被 unset\n";

        // 脚本结束时也会自动销毁所有对象
        echo "\n";
    }

    // ============================================================
    // 3. 访问控制
    // ============================================================

    /**
     * 演示访问控制修饰符
     * public / protected / private
     */
    public function accessControl(): void
    {
        echo "=== 访问控制 ===\n";

        $person = new Person('测试', 20, '123456');

        echo "public（公开）：\n";
        echo "  可以直接访问 \$person->name = {$person->name}\n";
        echo "  可以直接调用 \$person->say('hi') = " . $person->say('hi') . "\n";

        echo "\nprotected（受保护）：\n";
        echo "  外部不能直接访问 \$person->age（会报错）\n";
        echo "  通过 getter 访问：\$person->getAge() = " . $person->getAge() . "\n";
        $person->setAge(25);
        echo "  通过 setter 修改后：\$person->getAge() = " . $person->getAge() . "\n";

        echo "\nprivate（私有）：\n";
        echo "  外部不能直接访问 \$person->idCard（会报错）\n";
        echo "  子类也不能访问私有属性和方法\n";
        echo "  只能通过本类的公开方法间接访问\n";
        echo "  调用私有方法: " . $person->callPrivateMethod() . "\n";

        echo "\n";
    }

    // ============================================================
    // 4. 继承
    // ============================================================

    /**
     * 演示继承（extends）
     */
    public function inheritance(): void
    {
        echo "=== 继承 ===\n";

        // 创建子类对象
        $student = new Student('小明', 18, '123456', '清华大学', '2024001');

        echo "子类继承了父类的属性和方法：\n";
        echo "  继承的属性: name = {$student->name}\n";
        echo "  继承的方法: say() = " . $student->say('大家好') . "\n";

        echo "\n子类新增的属性和方法：\n";
        echo "  新增属性: school = {$student->school}\n";
        echo "  新增属性: studentId = {$student->studentId}\n";
        echo "  新增方法: study() = " . $student->study('PHP') . "\n";

        echo "\n方法重写（Override）：\n";
        echo "  Student 重写了 say() 方法\n";
        echo "  " . $student->say('我是学生') . "\n";

        echo "\n子类访问受保护成员：\n";
        echo "  " . $student->testProtectedAccess() . "\n";

        echo "\nfinal 关键字：\n";
        echo "  final 类不能被继承\n";
        echo "  final 方法不能被重写\n";
        echo "  student->getStudentInfo() = " . $student->getStudentInfo() . "\n";

        echo "\n";
    }

    // ============================================================
    // 5. 抽象类与接口
    // ============================================================

    /**
     * 演示抽象类（abstract）和接口（interface）
     */
    public function abstractAndInterface(): void
    {
        echo "=== 抽象类与接口 ===\n";

        // ---------- 抽象类 ----------
        echo "抽象类（abstract）：\n";
        echo "  抽象类不能被实例化\n";
        echo "  抽象类中可以有抽象方法（没有实现的方法）\n";
        echo "  子类必须实现所有抽象方法才能被实例化\n\n";

        // 定义一个抽象类（演示用，放在方法内部说明）
        echo "示例抽象类：\n";
        echo "  abstract class Shape {\n";
        echo "      abstract public function area(); // 抽象方法\n";
        echo "      abstract public function perimeter();\n";
        echo "  }\n";

        // ---------- 接口 ----------
        echo "\n接口（interface）：\n";
        echo "  接口定义了类应该实现的方法规范\n";
        echo "  接口中的方法都是公开的抽象方法\n";
        echo "  一个类可以实现（implements）多个接口\n";
        echo "  接口可以继承（extends）其他接口\n\n";

        // 演示接口的实现
        echo "演示：User 类实现 UserInterface 接口\n";

        // 定义一个实现接口的类
        $user = new class('admin', 'admin@example.com', '123456') implements UserInterface {
            private string $username;
            private string $email;
            private string $password;

            public function __construct(string $username, string $email, string $password)
            {
                $this->username = $username;
                $this->email = $email;
                $this->password = $password;
            }

            public function getUsername(): string
            {
                return $this->username;
            }

            public function getEmail(): string
            {
                return $this->email;
            }

            public function verifyPassword(string $password): bool
            {
                return $this->password === $password;
            }
        };

        echo "  getUsername() = " . $user->getUsername() . "\n";
        echo "  getEmail() = " . $user->getEmail() . "\n";
        echo "  verifyPassword('123456') = " . ($user->verifyPassword('123456') ? '正确' : '错误') . "\n";
        echo "  接口常量: UserInterface::USER_TYPE = " . UserInterface::USER_TYPE . "\n";

        echo "\n抽象类 vs 接口：\n";
        echo "  抽象类：可以有属性和具体方法，单继承\n";
        echo "  接口：只能有方法声明和常量，多实现\n";

        echo "\n";
    }

    // ============================================================
    // 6. Trait 代码复用
    // ============================================================

    /**
     * 演示 Trait 代码复用
     */
    public function traits(): void
    {
        echo "=== Trait 代码复用 ===\n";
        echo "Trait 用于解决 PHP 单继承的限制，实现横向代码复用\n\n";

        // 定义一个使用 Trait 的类
        $service = new class {
            // 使用 use 关键字引入 Trait
            use LoggerTrait;
            use TimestampTrait;

            public function doSomething(): void
            {
                // 可以直接使用 Trait 中的方法
                $this->log('开始执行任务');
                $this->setCreatedAt();
                echo "  执行了一些操作...\n";
                $this->log('任务完成');
            }
        };

        echo "创建使用了 LoggerTrait 和 TimestampTrait 的类\n";
        $service->doSomething();

        echo "\n调用 Trait 提供的方法：\n";
        echo "  getCreatedAt() = " . $service->getCreatedAt() . "\n";
        echo "  getLogs() 返回的日志数量：" . count($service->getLogs()) . "\n";
        echo "  打印所有日志：\n";
        $service->printLogs();

        echo "\nTrait 优先级：\n";
        echo "  当前类方法 > Trait 方法 > 父类方法\n";
        echo "  即：当前类会覆盖 Trait，Trait 会覆盖父类\n";

        echo "\n";
    }

    // ============================================================
    // 7. 命名空间与自动加载
    // ============================================================

    /**
     * 演示命名空间和自动加载
     */
    public function namespaceAndAutoload(): void
    {
        echo "=== 命名空间与自动加载 ===\n";

        echo "命名空间（namespace）：\n";
        echo "  用于组织代码，避免类名冲突\n";
        echo "  当前命名空间：" . __NAMESPACE__ . "\n";
        echo "  当前类：" . __CLASS__ . "\n";

        echo "\n自动加载（autoload）：\n";
        echo "  PHP 提供了 spl_autoload_register() 函数注册自动加载器\n";
        echo "  当使用未定义的类时，自动加载器会根据类名加载对应的文件\n";
        echo "  Composer 的 PSR-4 自动加载是最常用的标准\n";
        echo "  规则：命名空间前缀对应目录，类名对应文件名\n";
        echo "  例如：App\\Basics\\Basics → src/Basics/Basics.php\n";

        echo "\nuse 关键字：\n";
        echo "  use App\\Oop\\Person;  // 导入类\n";
        echo "  use App\\Oop\\Person as P; // 别名导入\n";
        echo "  导入后可以直接使用 Person，不需要写完整命名空间\n";

        echo "\n";
    }

    // ============================================================
    // 8. 静态成员与后期静态绑定
    // ============================================================

    /**
     * 演示静态成员和后期静态绑定（static:: vs self::）
     */
    public function staticAndLateBinding(): void
    {
        echo "=== 静态成员与后期静态绑定 ===\n";

        echo "静态属性和方法：\n";
        echo "  使用 static 关键字定义\n";
        echo "  属于类本身，不属于某个对象\n";
        echo "  通过 类名::属性名 / 类名::方法名() 访问\n";
        echo "  Person::\$count = " . Person::$count . "\n";
        echo "  Person::getCount() = " . Person::getCount() . "\n";

        echo "\nself:: vs static::（后期静态绑定）：\n";
        echo "  self:: 总是指向定义它的类\n";
        echo "  static:: 指向调用它的类（运行时确定）\n";
        echo "  这就是后期静态绑定（Late Static Binding）\n\n";

        // 演示后期静态绑定
        $base = new class {
            public static function whoAmI(): string
            {
                return '基类（self）';
            }

            public static function testSelf(): string
            {
                return self::whoAmI();
            }

            public static function testStatic(): string
            {
                return static::whoAmI();
            }
        };

        // 定义子类（通过继承）
        $childClass = new class extends $base {
            public static function whoAmI(): string
            {
                return '子类（static）';
            }
        };

        echo "演示：\n";
        echo "  子类调用 testSelf() = " . get_class($childClass)::testSelf() . "（self 指向定义类）\n";
        echo "  子类调用 testStatic() = " . get_class($childClass)::testStatic() . "（static 指向调用类）\n";

        echo "\n";
    }

    // ============================================================
    // 9. 魔术方法
    // ============================================================

    /**
     * 演示魔术方法
     */
    public function magicMethods(): void
    {
        echo "=== 魔术方法 ===\n";
        echo "魔术方法以 __ 开头，在特定情况下自动调用\n\n";

        $magic = new MagicClass();

        echo "__set / __get（属性重载）：\n";
        $magic->dynamicProp = '动态属性值';  // 触发 __set()
        echo "  读取: " . $magic->dynamicProp . "（触发 __get()）\n";

        echo "\n__isset / __unset：\n";
        echo "  isset(\$magic->dynamicProp): " . (isset($magic->dynamicProp) ? '存在' : '不存在') . "\n";
        unset($magic->dynamicProp);
        echo "  unset 后 isset: " . (isset($magic->dynamicProp) ? '存在' : '不存在') . "\n";

        echo "\n__call（方法重载）：\n";
        $result = $magic->nonExistentMethod('参数1', '参数2');
        echo "  返回: {$result}\n";

        echo "\n__callStatic（静态方法重载）：\n";
        $result = MagicClass::staticMethod();
        echo "  返回: {$result}\n";

        echo "\n__toString（对象转字符串）：\n";
        echo "  把对象当字符串输出: {$magic}\n";

        echo "\n__invoke（对象当函数调用）：\n";
        $result = $magic('arg1', 'arg2');
        echo "  返回: {$result}\n";

        echo "\n__clone（克隆对象）：\n";
        $cloned = clone $magic;
        echo "  克隆完成\n";

        echo "\n";
    }

    // ============================================================
    // 10. 匿名类
    // ============================================================

    /**
     * 演示匿名类（PHP 7.0+）
     */
    public function anonymousClasses(): void
    {
        echo "=== 匿名类 ===\n";
        echo "匿名类没有类名，用 new class 直接定义并实例化\n";
        echo "适合一次性使用的简单类\n\n";

        // 简单匿名类
        $obj = new class {
            public string $name = '匿名类对象';

            public function greet(): string
            {
                return 'Hello from ' . $this->name;
            }
        };

        echo "简单匿名类：\n";
        echo "  name = {$obj->name}\n";
        echo "  greet() = " . $obj->greet() . "\n";

        // 带构造函数的匿名类
        echo "\n带构造函数的匿名类：\n";
        $calculator = new class(10, 5) {
            private int $a;
            private int $b;

            public function __construct(int $a, int $b)
            {
                $this->a = $a;
                $this->b = $b;
            }

            public function sum(): int
            {
                return $this->a + $this->b;
            }
        };

        echo "  sum(10, 5) = " . $calculator->sum() . "\n";

        // 匿名类实现接口
        echo "\n匿名类实现接口：\n";
        echo "  前面的 UserInterface 示例已经演示过\n";
        echo "  new class(...) implements UserInterface { ... }\n";

        // 匿名类继承
        echo "\n匿名类继承：\n";
        $specialPerson = new class('匿名人', 25, '000000') extends Person {
            public function introduce(): string
            {
                return "我是{$this->name}，今年{$this->getAge()}岁";
            }
        };
        echo "  " . $specialPerson->introduce() . "\n";

        echo "\n";
    }

    // ============================================================
    // 11. 类型属性（PHP 7.4+）
    // ============================================================

    /**
     * 演示类型属性（Typed Properties）
     */
    public function typedProperties(): void
    {
        echo "=== 类型属性（PHP 7.4+）=== \n";
        echo "PHP 7.4 起支持在类属性中声明类型\n\n";

        echo "支持的类型：\n";
        echo "  - 标量类型：int, float, string, bool\n";
        echo "  - 复合类型：array, object\n";
        echo "  - 类/接口类型\n";
        echo "  - 特殊类型：self, parent, callable\n";
        echo "  - 可空类型：?Type（如 ?string）\n";
        echo "  - 联合类型：Type1|Type2（PHP 8.0+）\n";
        echo "  - 混合类型：mixed（PHP 8.0+）\n";
        echo "  - never 类型（PHP 8.1+）\n\n";

        echo "Person 类示例：\n";
        echo "  public string \$name;      // 字符串类型\n";
        echo "  protected int \$age;       // 整型\n";
        echo "  private string \$idCard;   // 私有字符串\n\n";

        echo "类型属性的好处：\n";
        echo "  - 代码更清晰，可读性更好\n";
        echo "  - 类型错误更早被发现\n";
        echo "  - IDE 可以提供更好的自动补全\n";

        echo "\n";
    }

    // ============================================================
    // 12. 枚举 Enum（PHP 8.1+）
    // ============================================================

    /**
     * 演示枚举（Enum）
     */
    public function enums(): void
    {
        echo "=== 枚举 Enum（PHP 8.1+）=== \n";
        echo "枚举用于定义一组固定的可选值\n\n";

        // 普通枚举
        echo "普通枚举：\n";
        $status = Status::Active;
        echo "  Status::Active = " . $status->name . "\n";
        echo "  label() = " . $status->label() . "\n";
        echo "  isActive() = " . ($status->isActive() ? '是' : '否') . "\n";

        // 遍历枚举
        echo "\n遍历所有状态：\n";
        foreach (Status::cases() as $s) {
            echo "  {$s->name} - {$s->label()}\n";
        }

        // 回退枚举
        echo "\n回退枚举（带值的枚举）：\n";
        $role = UserRole::Admin;
        echo "  UserRole::Admin->name = " . $role->name . "\n";
        echo "  UserRole::Admin->value = " . $role->value . "\n";
        echo "  Admin 的权限：" . implode(', ', $role->permissions()) . "\n";
        echo "  Admin 有 delete 权限吗？" . ($role->hasPermission('delete') ? '有' : '没有') . "\n";

        // 从值创建枚举
        echo "\n从值创建枚举：\n";
        $roleFromValue = UserRole::from('editor');  // 值不存在会抛异常
        echo "  UserRole::from('editor') = " . $roleFromValue->name . "\n";

        $roleTry = UserRole::tryFrom('invalid');  // 值不存在返回 null
        echo "  UserRole::tryFrom('invalid') = " . ($roleTry === null ? 'null（不存在）' : $roleTry->name) . "\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有面向对象编程演示
     */
    public function runAll(): void
    {
        $this->classesAndObjects();
        $this->constructAndDestruct();
        $this->accessControl();
        $this->inheritance();
        $this->abstractAndInterface();
        $this->traits();
        $this->namespaceAndAutoload();
        $this->staticAndLateBinding();
        $this->magicMethods();
        $this->anonymousClasses();
        $this->typedProperties();
        $this->enums();
    }
}
