<?php
// ============================================================
// PHP 学习项目 - 主入口文件
// ============================================================
// 这是项目的入口文件，提供命令行交互菜单
// 可以选择运行各个模块的演示代码
//
// 使用方法：
//   命令行运行：php public/index.php
//   然后根据菜单提示选择要运行的模块
// ============================================================

// 声明根命名空间（入口文件通常在全局命名空间）
// 也可以不写，默认就是全局命名空间

// ============================================================
// 自动加载设置
// ============================================================

// 项目根目录路径
// __DIR__ 是当前文件所在目录（public/）
// dirname() 获取上一级目录，即项目根目录
$projectRoot = dirname(__DIR__);

// 首先尝试加载 Composer 的自动加载器（vendor/autoload.php）
// 如果运行过 composer install，就会有这个文件
$composerAutoload = $projectRoot . '/vendor/autoload.php';
if (file_exists($composerAutoload)) {
    require_once $composerAutoload;
    echo "[自动加载] 已加载 Composer 自动加载器\n";
} else {
    // 如果没有 Composer，使用自定义的 spl_autoload_register 实现自动加载
    echo "[自动加载] Composer 未安装，使用自定义自动加载器\n";

    /**
     * 自定义自动加载函数
     * 根据 PSR-4 规范，将命名空间映射到目录
     * 
     * 命名空间 "App\" 对应 "src/" 目录
     * 例如：
     *   App\Basics\Basics  →  src/Basics/Basics.php
     *   App\Oop\Person     →  src/Oop/Person.php
     * 
     * @param string $className 完整的类名（包含命名空间）
     */
    spl_autoload_register(function (string $className): void {
        // 命名空间前缀
        $prefix = 'App\\';
        // 对应的基目录
        $base_dir = dirname(__DIR__) . '/src/';

        // 检查类名是否包含我们的命名空间前缀
        $prefix_len = strlen($prefix);
        if (strncmp($prefix, $className, $prefix_len) !== 0) {
            // 不是我们的命名空间，跳过（让其他自动加载器处理）
            return;
        }

        // 去掉前缀，获取相对类名
        $relative_class = substr($className, $prefix_len);

        // 将命名空间分隔符替换为目录分隔符，加上 .php 后缀
        $file = $base_dir . str_replace('\\', '/', $relative_class) . '.php';

        // 如果文件存在，就加载它
        if (file_exists($file)) {
            require_once $file;
        }
    });
}

// ============================================================
// 引入各个模块的类（通过 use 导入命名空间）
// ============================================================

use App\Basics\Basics;
use App\Arrays\Arrays;
use App\Oop\Oop;
use App\Database\Database;
use App\Advanced\Advanced;
use App\Patterns\Patterns;

// ============================================================
// 输出标题
// ============================================================

// 清屏（根据操作系统）
if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
    // Windows 系统
    system('cls');
} else {
    // Linux / macOS 系统
    system('clear');
}

// 打印标题
echo "╔══════════════════════════════════════════════════════════════╗\n";
echo "║           PHP 学习项目 - 交互式演示系统                      ║\n";
echo "╠══════════════════════════════════════════════════════════════╣\n";
echo "║  包含模块：基础语法 | 数组 | 面向对象 | 数据库 | 高级 | 设计模式 ║\n";
echo "╚══════════════════════════════════════════════════════════════╝\n";
echo "\n";

// ============================================================
// 菜单系统
// ============================================================

/**
 * 显示主菜单
 */
function showMainMenu(): void
{
    echo "请选择要学习的模块：\n\n";
    echo "  [1] 基础语法 (Basics)\n";
    echo "      - PHP标记、数据类型、变量、常量、运算符\n";
    echo "      - 控制流、函数、字符串操作、类型转换\n";
    echo "\n";
    echo "  [2] 数组与数据结构 (Arrays)\n";
    echo "      - 数组定义、增删改查、遍历、常用函数\n";
    echo "      - 排序、栈与队列、高阶函数\n";
    echo "\n";
    echo "  [3] 面向对象编程 (OOP)\n";
    echo "      - 类与对象、继承、抽象类、接口、Trait\n";
    echo "      - 命名空间、静态成员、魔术方法、枚举\n";
    echo "\n";
    echo "  [4] 数据库操作 (Database)\n";
    echo "      - PDO连接、预处理、CRUD、事务\n";
    echo "      - 错误处理、数据提取、MySQLi对比\n";
    echo "\n";
    echo "  [5] 高级特性 (Advanced)\n";
    echo "      - 异常、错误处理、闭包、生成器、反射\n";
    echo "      - 正则、会话、Cookie、文件上传、日期时间\n";
    echo "\n";
    echo "  [6] 设计模式 (Patterns)\n";
    echo "      - 单例、工厂、观察者、策略、装饰器、DI容器\n";
    echo "\n";
    echo "  [0] 运行全部演示\n";
    echo "  [q] 退出程序\n";
    echo "\n";
    echo "请输入选项: ";
}

/**
 * 显示子菜单（某个模块的子章节选择）
 * 
 * @param string $moduleName 模块名称
 * @param array $sections 章节列表
 * @return int|null 选择的章节索引，null 表示返回
 */
function showSubMenu(string $moduleName, array $sections): ?int
{
    echo "\n--- {$moduleName} 模块 ---\n\n";
    echo "选择要运行的章节：\n\n";
    foreach ($sections as $index => $section) {
        $num = $index + 1;
        echo "  [{$num}] {$section}\n";
    }
    echo "\n  [0] 运行全部章节\n";
    echo "  [b] 返回主菜单\n";
    echo "\n请输入选项: ";

    $input = trim(fgets(STDIN));

    if ($input === 'b' || $input === 'B') {
        return null;  // 返回主菜单
    }

    if ($input === '0') {
        return -1;  // 运行全部
    }

    if (is_numeric($input) && $input >= 1 && $input <= count($sections)) {
        return (int)$input - 1;  // 返回选中的索引
    }

    echo "无效的选项，请重新选择。\n";
    return showSubMenu($moduleName, $sections);  // 递归重新选择
}

/**
 * 暂停等待用户按键
 */
function pause(): void
{
    echo "\n按回车键继续...";
    fgets(STDIN);
}

// ============================================================
// 主循环
// ============================================================

while (true) {
    showMainMenu();

    // 读取用户输入
    // trim() 去除首尾空白和换行
    $choice = trim(fgets(STDIN));

    // 处理用户选择
    switch ($choice) {
        // ---------- 1. 基础语法 ----------
        case '1':
            $basics = new Basics();
            $sections = [
                'PHP标记与注释',
                '基本数据类型',
                '变量',
                '常量',
                '运算符',
                '控制流',
                '函数',
                '字符串操作',
                '类型转换与类型判断',
            ];

            $selected = showSubMenu('基础语法', $sections);
            if ($selected === null) {
                break;  // 返回主菜单
            }
            if ($selected === -1) {
                $basics->runAll();
            } else {
                $methods = [
                    'phpTagsAndComments',
                    'dataTypes',
                    'variables',
                    'constants',
                    'operators',
                    'controlFlow',
                    'functions',
                    'stringOperations',
                    'typeCastingAndChecking',
                ];
                $method = $methods[$selected];
                $basics->$method();
            }
            pause();
            break;

        // ---------- 2. 数组与数据结构 ----------
        case '2':
            $arrays = new Arrays();
            $sections = [
                '数组定义',
                '数组增删改查',
                '数组遍历',
                '常用数组函数',
                '数组排序',
                '栈与队列',
                '高阶数组函数',
            ];

            $selected = showSubMenu('数组与数据结构', $sections);
            if ($selected === null) {
                break;
            }
            if ($selected === -1) {
                $arrays->runAll();
            } else {
                $methods = [
                    'arrayDefinition',
                    'arrayCrud',
                    'arrayTraversal',
                    'commonFunctions',
                    'arraySorting',
                    'stackAndQueue',
                    'higherOrderFunctions',
                ];
                $method = $methods[$selected];
                $arrays->$method();
            }
            pause();
            break;

        // ---------- 3. 面向对象编程 ----------
        case '3':
            $oop = new Oop();
            $sections = [
                '类与对象',
                '构造与析构函数',
                '访问控制',
                '继承',
                '抽象类与接口',
                'Trait代码复用',
                '命名空间与自动加载',
                '静态成员与后期绑定',
                '魔术方法',
                '匿名类',
                '类型属性',
                '枚举Enum',
            ];

            $selected = showSubMenu('面向对象编程', $sections);
            if ($selected === null) {
                break;
            }
            if ($selected === -1) {
                $oop->runAll();
            } else {
                $methods = [
                    'classesAndObjects',
                    'constructAndDestruct',
                    'accessControl',
                    'inheritance',
                    'abstractAndInterface',
                    'traits',
                    'namespaceAndAutoload',
                    'staticAndLateBinding',
                    'magicMethods',
                    'anonymousClasses',
                    'typedProperties',
                    'enums',
                ];
                $method = $methods[$selected];
                $oop->$method();
            }
            pause();
            break;

        // ---------- 4. 数据库操作 ----------
        case '4':
            $database = new Database();
            $sections = [
                'PDO连接数据库',
                '预处理语句',
                'CRUD操作',
                '事务处理',
                '错误处理模式',
                '数据提取方式',
                'MySQLi扩展对比',
            ];

            $selected = showSubMenu('数据库操作', $sections);
            if ($selected === null) {
                break;
            }
            if ($selected === -1) {
                $database->runAll();
            } else {
                $methods = [
                    'connect',
                    'preparedStatements',
                    'crudOperations',
                    'transactions',
                    'errorModes',
                    'fetchMethods',
                    'mysqliComparison',
                ];
                $method = $methods[$selected];
                try {
                    // 数据库操作可能抛出异常
                    if ($method === 'connect') {
                        echo "注意：需要配置正确的数据库连接信息才能连接 MySQL\n";
                        echo "这里使用 SQLite 内存数据库进行演示：\n\n";
                        $database->connectSqlite();
                    } else {
                        // 确保先连接数据库（用 SQLite）
                        $database->connectSqlite();
                        $database->$method();
                    }
                } catch (\Exception $e) {
                    echo "数据库操作异常：" . $e->getMessage() . "\n";
                }
            }
            pause();
            break;

        // ---------- 5. 高级特性 ----------
        case '5':
            $advanced = new Advanced();
            $sections = [
                '异常处理',
                '错误处理',
                '闭包与匿名函数',
                '生成器yield',
                '反射Reflection',
                '正则表达式',
                '会话管理',
                'Cookie操作',
                '文件上传处理',
                '日期时间DateTime',
            ];

            $selected = showSubMenu('高级特性', $sections);
            if ($selected === null) {
                break;
            }
            if ($selected === -1) {
                $advanced->runAll();
            } else {
                $methods = [
                    'exceptionHandling',
                    'errorHandling',
                    'closures',
                    'generators',
                    'reflection',
                    'regex',
                    'sessions',
                    'cookies',
                    'fileUpload',
                    'dateTime',
                ];
                $method = $methods[$selected];
                $advanced->$method();
            }
            pause();
            break;

        // ---------- 6. 设计模式 ----------
        case '6':
            $patterns = new Patterns();
            $sections = [
                '单例模式',
                '工厂模式',
                '观察者模式',
                '策略模式',
                '装饰器模式',
                '依赖注入容器',
            ];

            $selected = showSubMenu('设计模式', $sections);
            if ($selected === null) {
                break;
            }
            if ($selected === -1) {
                $patterns->runAll();
            } else {
                $methods = [
                    'singleton',
                    'factory',
                    'observer',
                    'strategy',
                    'decorator',
                    'diContainer',
                ];
                $method = $methods[$selected];
                $patterns->$method();
            }
            pause();
            break;

        // ---------- 0. 运行全部 ----------
        case '0':
            echo "即将运行全部演示模块...\n\n";
            echo "--- 基础语法 ---\n";
            (new Basics())->runAll();
            echo "\n--- 数组与数据结构 ---\n";
            (new Arrays())->runAll();
            echo "\n--- 面向对象编程 ---\n";
            (new Oop())->runAll();
            echo "\n--- 数据库操作 ---\n";
            (new Database())->runAll();
            echo "\n--- 高级特性 ---\n";
            (new Advanced())->runAll();
            echo "\n--- 设计模式 ---\n";
            (new Patterns())->runAll();
            echo "\n=== 全部演示完成 ===\n";
            pause();
            break;

        // ---------- q. 退出 ----------
        case 'q':
        case 'Q':
            echo "\n感谢使用 PHP 学习项目，再见！\n\n";
            exit(0);

        // ---------- 无效选项 ----------
        default:
            echo "\n无效的选项 '{$choice}'，请重新选择。\n";
            pause();
            break;
    }
}
