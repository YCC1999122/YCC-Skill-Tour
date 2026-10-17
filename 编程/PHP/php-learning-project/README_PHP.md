# PHP 学习项目

## 项目简介

本项目是一套完整的 PHP 学习教程，基于 PHP 8.1+（使用枚举等新特性），采用 Composer 进行依赖管理与 PSR-4 自动加载，从基础语法、数组操作到面向对象、PDO 数据库、高级特性与设计模式逐步进阶。每个 `.php` 文件都配有极其详细的逐行中文注释，从 `<?php` 标记到命名空间，从魔术方法到预处理语句都一一说明，适合零基础入门或从其他语言转型 Web 后端开发的学习者。

通过本项目的学习，你可以掌握 PHP 的核心知识体系：超全局变量、数组与高阶函数、面向对象（类/接口/Trait/枚举/魔术方法）、Composer 自动加载、PDO 数据库操作与防 SQL 注入、异常处理、闭包与生成器、反射与常用设计模式，并能够独立开发结构清晰、可维护的 PHP Web 应用。项目入口为命令行交互菜单（`public/index.php`），可自由选择运行各模块演示。

## 目录结构

```
php-learning-project/
├── composer.json                 # Composer 配置（PSR-4 自动加载 App\ → src/，要求 PHP>=8.1、ext-pdo、ext-mbstring）
├── public/
│   └── index.php                 # 主入口文件（Composer 自动加载 + 自定义 spl_autoload_register 双方案、交互菜单）
└── src/
│   ├── Basics/
│   │   └── Basics.php            # 基础语法（PHP 标记、数据类型、变量与作用域、常量、运算符、控制流、函数、字符串、类型转换）
│   ├── Arrays/
│   │   └── Arrays.php            # 数组与数据结构（索引/关联/多维数组、增删改查、排序、栈与队列、array_map/filter/reduce）
│   ├── Oop/
│   │   ├── Oop.php               # OOP 主演示类（整合类与对象、继承、抽象类、接口、Trait、命名空间、静态成员、魔术方法、匿名类、枚举）
│   │   ├── Person.php            # Person 基类（类常量、静态属性、构造/析构、访问控制、类型属性 PHP 7.4+）
│   │   ├── Student.php           # Student 子类（extends 继承、parent 关键字、方法重写、final 关键字）
│   │   ├── UserInterface.php     # 接口示例（UserInterface、AdminInterface 接口继承、LoggerAwareInterface）
│   │   ├── LoggerTrait.php       # Trait 代码复用（LoggerTrait 日志、TimestampTrait 时间戳）
│   │   ├── MagicClass.php        # 魔术方法演示（__get/__set/__isset/__unset/__call/__toString/__invoke/__clone 等）
│   │   └── Status.php            # 枚举示例（Status 普通枚举、UserRole 回退枚举 Backed Enum，PHP 8.1+）
│   ├── Database/
│   │   └── Database.php          # 数据库操作（PDO 连接、PDOStatement 预处理防注入、CRUD、事务、错误模式、fetch 系列、MySQLi 对比）
│   ├── Advanced/
│   │   └── Advanced.php          # 高级特性（异常、错误处理、闭包、生成器 yield、反射、正则、会话、Cookie、文件上传、DateTime）
│   └── Patterns/
│       └── Patterns.php          # 设计模式（单例、简单工厂/工厂方法、观察者、策略、装饰器、DI 依赖注入容器）
└── README_PHP.md                 # 本文件
```

> 说明：`vendor/`（Composer 依赖目录）与 `composer.lock` 为 `composer install` 后生成，已在上面的目录树中省略。

## 学习路径建议

### 第一阶段：基础语法（src/Basics/Basics.php）
- 运行菜单选项 1，配合源码逐行阅读

核心知识点：
- PHP 标记：`<?php ... ?>` 标准标记、`<?= ... ?>` 短输出标记（纯 PHP 文件建议省略结尾 `?>`）
- 八种数据类型：四种标量（int/float/string/bool）、两种复合（array/object）、两种特殊（resource/null）
- 变量以 `$` 开头、可变变量 `$$name`、变量作用域与 `global` 关键字
- 常量：`define()` 与 `const` 的区别、魔术常量 `__DIR__` / `__FILE__` / `__LINE__`
- 运算符：`==` 松散比较 vs `===` 严格比较、`<=>` 太空船运算符、`??` 空合并运算符
- 控制流：if/elseif/else、switch、match 表达式（PHP 8.0+）、for/foreach/while
- 函数：默认参数、可变参数 `...$args`、类型声明、返回类型、引用传递 `&$var`
- 字符串：单引号 vs 双引号（变量解析）、Heredoc/Nowdoc、常用字符串函数

### 第二阶段：数组与数据结构（src/Arrays/Arrays.php）
- 运行菜单选项 2，数组是 PHP 最核心、最强大的数据结构

核心知识点：
- PHP 数组统一了列表与字典：索引数组（数字键）与关联数组（字符串键）、多维数组
- 数组定义：`[1, 2, 3]` 短语法（PHP 5.4+）与 `array()` 旧语法
- 增删改查：`array_push` / `array_pop` / `array_shift` / `array_unshift` / `unset`
- 合并与拆分：`array_merge` / `array_merge_recursive` / `array_slice` / `array_chunk`
- 遍历：foreach 值遍历与引用遍历 `foreach ($arr as &$v)`（用后须 `unset($v)`）
- 高阶函数三剑客：`array_map`（映射）、`array_filter`（过滤）、`array_reduce`（归约）
- 排序家族：`sort` / `rsort` / `asort` / `ksort` / `usort`（保持键值关联的区别）
- 栈与队列：用数组函数模拟 LIFO/FIFO 数据结构

### 第三阶段：面向对象（src/Oop/）
- 运行菜单选项 3，按 Person → Student → 接口 → Trait → 魔术方法 → 枚举 → Oop.php 整合的顺序阅读

核心知识点：
- 类与对象：`new` 实例化、`$this` 伪变量、类常量 `const`、静态成员 `static` 与 `self::`
- 构造函数 `__construct` 与析构函数 `__destruct` 的调用时机
- 访问控制：`public` / `protected` / `private`，类型属性（PHP 7.4+）
- 继承 `extends`（PHP 单继承）、`parent::` 调用父类方法、方法重写、`final` 禁止继承/重写
- 抽象类 `abstract` 与接口 `interface`：接口可多重实现 `implements`，接口可继承接口
- Trait：横向代码复用机制，解决单继承限制，优先级"当前类 > Trait > 父类"
- 命名空间 `namespace` 与 `use` 导入：配合 PSR-4 自动加载组织代码
- 后期静态绑定 `static::` 与匿名类 `new class`
- 魔术方法：`__get` / `__set` / `__call` / `__toString` / `__invoke` / `__clone` 等的自动触发时机
- 枚举（PHP 8.1+）：普通枚举 `enum Status` 与回退枚举 `enum UserRole: string`

### 第四阶段：数据库操作（src/Database/Database.php）
- 运行菜单选项 4，PDO 是 PHP 操作数据库的官方推荐方式

核心知识点：
- PDO（PHP Data Objects）：统一的数据库访问抽象层，支持 MySQL/PostgreSQL/SQLite 等
- DSN 连接串：`mysql:host=...;dbname=...;charset=utf8mb4`
- 预处理：`prepare()` + `execute()`，参数绑定天然防止 SQL 注入（绝不要拼接 SQL 字符串）
- CRUD 操作：命名占位符 `:name` 与问号占位符 `?` 两种绑定方式
- 数据提取：`fetch` / `fetchAll` / `fetchColumn` 与 `FETCH_ASSOC` / `FETCH_OBJ` 模式
- 事务处理：`beginTransaction` / `commit` / `rollBack`，保证多步操作的原子性
- 错误处理模式：`PDO::ERRMODE_EXCEPTION` 异常模式（推荐）
- 与 MySQLi 扩展的对比与选型建议

### 第五阶段：高级特性（src/Advanced/Advanced.php）
- 运行菜单选项 5

核心知识点：
- 异常处理：`try` / `catch` / `finally`、自定义异常类、`set_exception_handler`
- 错误处理：错误级别、错误与异常的转换 `set_error_handler`
- 闭包与匿名函数：`use` 捕获外部变量、`bindTo` 绑定 `$this`
- 生成器 `yield`：惰性产生序列，处理大数据集时内存友好的迭代方案
- 反射 `ReflectionClass` / `ReflectionMethod`：运行时检查类结构（框架的基石）
- 正则表达式：`preg_match` / `preg_replace` / `preg_split`（PCRE 语法）
- 会话管理 `$_SESSION`、Cookie 操作 `setcookie` 与安全选项（HttpOnly/Secure/SameSite）
- 文件上传处理与 `DateTime` / `DateTimeZone` / `DateInterval` 日期时间处理

### 第六阶段：设计模式（src/Patterns/Patterns.php）
- 运行菜单选项 6，将 OOP 能力综合应用

核心知识点：
- 单例模式：私有构造函数 + 私有静态实例 + `getInstance()`（数据库连接、配置管理）
- 工厂模式：简单工厂与工厂方法，解耦对象创建
- 观察者模式：SplSubject/SplObserver 接口实现事件订阅
- 策略模式：用接口封装可互换的算法族
- 装饰器模式：动态扩展对象功能而不修改原类
- 依赖注入容器（DI Container）：控制反转思想的最小实现，理解现代框架（Laravel/Symfony）的核心

## 开发环境

### 1. PHP 解释器（必需，>= 8.1）
- **用途**：PHP 脚本的运行时，本项目使用枚举（8.1）、match（8.0）等新特性
- **下载地址**：https://www.php.net/downloads（Windows 用户选 Thread Safe 版 ZIP 解压并配置 PATH）
- **集成环境（省事之选）**：
  - XAMPP：https://www.apachefriends.org/（Apache + MySQL + PHP 一键安装）
  - Laragon：https://laragon.org/（轻量现代，Windows 体验极佳）
  - phpStudy：https://www.xp.cn/（国产中文集成环境）
- **验证安装**：`php -v`
- **特点**：PHP 既可命令行运行脚本（CLI 模式），也可嵌入 Web 服务器处理 HTTP 请求

### 2. Composer（依赖管理与自动加载）
- **用途**：PHP 的事实标准包管理器，管理依赖并生成 PSR-4 自动加载器
- **下载地址**：https://getcomposer.org/download/
- **常用命令**：`composer install`（安装依赖并生成 vendor/autoload.php）、`composer dump-autoload`（重新生成自动加载映射）
- **说明**：本项目入口 `public/index.php` 会自动检测——有 vendor/autoload.php 则用 Composer 加载，否则回退到内置的自定义 `spl_autoload_register` 加载器，两种方式都能跑通

### 3. 内置 Web 服务器（php -S）
- **用途**：开发调试用的轻量 Web 服务器，无需配置 Apache/Nginx
- **使用方式**：`php -S localhost:8000 -t public`（将 public 目录作为文档根目录）
- **特点**：单线程，仅用于开发环境；本项目交互菜单基于命令行，直接 `php public/index.php` 运行即可

### 4. IDE 选择
- **PHPStorm**（专业首选）：https://www.jetbrains.com/phpstorm/（JetBrains 出品，PHP 支持最完善的 IDE，收费；学生可免费申请教育授权）
- **VS Code**（免费之选）：https://code.visualstudio.com/（配合 Intelephense 扩展可获得智能提示、跳转定义、类型检查）
- **PhpStorm 特点**：内置数据库工具、Composer 集成、Xdebug 调试、框架深度支持（Laravel/Symfony）

### 5. 项目运行方式
| 场景 | 命令 |
|------|------|
| 命令行运行交互菜单 | `php public/index.php` |
| 安装依赖（可选） | `composer install` |
| 作为 Web 服务运行 | `php -S localhost:8000 -t public` |
| 检查语法 | `php -l src/Basics/Basics.php` |
| 运行测试（需先装依赖） | `vendor/bin/phpunit` |

## PHP 基础知识

### 超全局变量
超全局变量在任何作用域中直接可用，是 PHP 处理 Web 请求的入口：
| 变量 | 说明 |
|------|------|
| `$_GET` | URL 查询参数（GET 请求数据） |
| `$_POST` | POST 表单数据 |
| `$_REQUEST` | GET + POST + COOKIE 的合集（不建议依赖） |
| `$_SERVER` | 服务器与请求信息（REQUEST_METHOD、REQUEST_URI 等） |
| `$_SESSION` | 会话数据（需 `session_start()`） |
| `$_COOKIE` | 客户端 Cookie |
| `$_FILES` | 文件上传信息 |
| `$_ENV` / `$GLOBALS` | 环境变量 / 全局符号表 |

### Composer 与 PSR-4 自动加载
- PSR-4 规范：完全限定类名（含命名空间）与文件路径一一对应
- 本项目映射：`"App\\": "src/"`，即 `App\Basics\Basics` → `src/Basics/Basics.php`
- 目录名与命名空间段、文件名与类名必须**大小写敏感地一致**（Linux 服务器强制，Windows 开发易埋坑）
- 自动加载原理：`spl_autoload_register` 注册回调，在类首次使用时按需 `require` 对应文件，告别满屏 require

### Trait vs 接口 vs 抽象类
| 特性 | 接口 interface | 抽象类 abstract class | Trait |
|------|---------------|----------------------|-------|
| 多继承/多实现 | 可实现多个 | 只能继承一个 | 可使用多个 |
| 属性 | 不能有 | 可以有 | 可以有 |
| 方法实现 | 不能有（PHP 8.1 前） | 可以有 | 可以有 |
| 实例化 | 不能 | 不能 | 不能 |
| 定位 | 定义契约 | 定义骨架 | 横向代码复用 |

### 常用魔术方法速查
| 方法 | 触发时机 |
|------|----------|
| `__construct` / `__destruct` | 对象创建 / 销毁时 |
| `__get` / `__set` | 访问 / 赋值不可访问属性时 |
| `__isset` / `__unset` | 对不可访问属性调用 isset / unset 时 |
| `__call` / `__callStatic` | 调用不可访问的实例方法 / 静态方法时 |
| `__toString` | 对象被当作字符串使用时（如 echo） |
| `__invoke` | 对象被当作函数调用时 |
| `__clone` | 对象被 clone 时 |

### PDO 预处理防注入要点
- 原则：**SQL 结构与数据分离**——`prepare('SELECT * FROM user WHERE id = :id')`，数据只通过 `execute(['id' => $id])` 传入
- 错误写法：`'SELECT * FROM user WHERE id = ' . $_GET['id']`（拼接用户输入，必然注入）
- 连接后务必设置 `$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION)`，让错误以异常形式抛出

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.php` 文件的逐行中文注释，每个知识点都标注了版本要求（如"PHP 8.1+"）与正确/错误写法对照，不要跳过。
2. **先跑起来再读**：先执行 `php public/index.php` 通过菜单运行各模块看输出，再回头精读源码。
3. **动手改数据**：把示例中的字符串、数组、SQL 参数改一改再运行，观察输出变化。
4. **查官方手册**：PHP 官方手册（php.net）质量极高且每个函数页都有用户贡献示例，养成直接查手册的习惯是 PHP 工程师的基本功。
5. **循序渐进**：基础语法 → 数组 → OOP → 数据库 → 高级 → 模式，OOP 是后面所有章节的地基，务必吃透。
6. **重视安全**：从第一天起就把"预处理防注入、输出转义防 XSS、密码用 password_hash"当作默认习惯，而不是事后再补。

## 注意事项

### 1. == 与 === 的陷阱
- `==` 松散比较有复杂的类型转换规则：`'0' == false` 为 true、`'abc' == 0` 在 PHP 7 为 true（PHP 8 已改为 false）
- 永远优先使用 `===` 严格比较（值与类型都相同才相等），需要类型转换时显式转换
- PHP 8 的 `match` 表达式默认严格比较，是比 `switch` 更安全的选择

### 2. foreach 引用遍历的坑
- `foreach ($arr as &$v)` 使用引用遍历后，变量 `$v` 仍指向数组最后一个元素
- 后续若复用 `$v` 会意外修改数组尾部元素，遍历结束必须 `unset($v)`
- 这是 PHP 最著名的面试题与线上 Bug 来源之一

### 3. SQL 注入必须零容忍
- 任何来自 `$_GET` / `$_POST` / `$_COOKIE` 的数据都不可信，绝不允许拼接进 SQL
- 统一使用 PDO 预处理，整数参数也建议绑定而非拼接（或至少强制 `(int)` 转换）
- 输出到 HTML 的内容用 `htmlspecialchars()` 转义，防止 XSS

### 4. 命名空间与目录必须一致
- PSR-4 要求命名空间大小写与目录完全一致，Windows 文件系统不区分大小写，本地能跑不代表 Linux 服务器能跑
- 新建类时先确认命名空间与路径对应关系：`App\Oop\Person` 必须位于 `src/Oop/Person.php`
- 修改目录结构后运行 `composer dump-autoload` 刷新映射

### 5. 版本兼容意识
- 本项目使用 PHP 8.1+ 特性（枚举、readonly 属性等），部署前确认服务器 PHP 版本（`php -v`）
- composer.json 中 `"php": ">=8.1"` 会在 install 时强制校验版本
- 学习旧代码时注意区分 PHP 5 时代写法（mysql_* 函数、array() 语法）与现代写法，mysql_* 扩展已在 PHP 7 移除

## 学习资源推荐

### 推荐书籍
1. 《Modern PHP》- Josh Lockhart（现代 PHP 开发最佳实践，必读）
2. 《PHP 和 MySQL Web 开发》（第 5 版）- Luke Welling, Laura Thomson（经典入门"圣经"）
3. 《Head First PHP & MySQL》- Lynn Beighley（图文并茂的零基础入门）
4. 《深入 PHP：面向对象、模式与实践》- Matt Zandstra（OOP 与设计模式进阶）
5. 《Laravel 入门与实战》- Laravel 是学完本项目后最值得学习的框架

### 在线教程网址
- PHP 官方手册（中文）：https://www.php.net/manual/zh/
- Composer 官方网站：https://getcomposer.org/（中文网：https://www.phpcomposer.com/）
- PHP-FIG PSR 规范（PSR-4 等）：https://www.php-fig.org/psr/
- PHP The Right Way（最佳实践指南）：https://phptherightway.com/
- 菜鸟教程 PHP：https://www.runoob.com/php/php-tutorial.html
- Laravel 官方文档（进阶方向）：https://laravel.com/docs（中文社区：https://learnku.com/laravel）

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
