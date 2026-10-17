<?php
// ============================================================
// PHP 高级特性学习 - Advanced 类
// ============================================================
// 本文件包含 PHP 高级特性的核心知识点
// ============================================================

namespace App\Advanced;

use Exception;
use DateTime;
use DateTimeZone;
use DateInterval;

/**
 * Advanced 类 - PHP 高级特性演示
 * 
 * 包含以下知识点：
 * - 异常处理
 * - 错误处理
 * - 闭包与匿名函数
 * - 生成器 yield
 * - 反射 Reflection
 * - 正则表达式
 * - 会话管理
 * - Cookie 操作
 * - 文件上传处理
 * - 日期时间 DateTime 类
 */
class Advanced
{
    // ============================================================
    // 1. 异常处理
    // ============================================================

    /**
     * 演示异常处理
     * 
     * 异常处理用于处理程序运行时的异常情况
     * 使程序能够优雅地处理错误，而不是直接崩溃
     */
    public function exceptionHandling(): void
    {
        echo "=== 异常处理 ===\n";

        // ---------- try-catch-finally ----------
        echo "try-catch-finally 结构：\n";

        try {
            // 尝试执行可能抛出异常的代码
            echo "  进入 try 块\n";

            // throw 关键字抛出异常
            // Exception 是所有异常的基类
            throw new Exception('这是一个测试异常');

            // throw 后面的代码不会执行
            echo "  这行不会执行\n";

        } catch (Exception $e) {
            // 捕获异常并处理
            // $e 是捕获到的异常对象
            echo "  捕获到异常：" . $e->getMessage() . "\n";
            echo "  异常文件：" . $e->getFile() . "\n";
            echo "  异常行号：" . $e->getLine() . "\n";
            // $e->getTraceAsString() 可以获取完整的调用栈
        } finally {
            // finally 块中的代码无论是否发生异常都会执行
            // 通常用于清理资源（关闭文件、数据库连接等）
            echo "  finally 块：总是会执行\n";
        }

        // ---------- 多个 catch ----------
        echo "\n多个 catch 块（捕获不同类型的异常）：\n";

        // 自定义异常类
        class ValidationException extends Exception {}
        class DatabaseException extends Exception {}

        try {
            // 模拟数据库异常
            throw new DatabaseException('数据库连接失败');
        } catch (ValidationException $e) {
            echo "  捕获验证异常：" . $e->getMessage() . "\n";
        } catch (DatabaseException $e) {
            echo "  捕获数据库异常：" . $e->getMessage() . "\n";
        } catch (Exception $e) {
            echo "  捕获通用异常：" . $e->getMessage() . "\n";
        }

        // ---------- 自定义异常 ----------
        echo "\n自定义异常：\n";

        /**
         * 自定义异常类
         * 继承自 Exception，可以添加自定义的属性和方法
         */
        class CustomException extends Exception
        {
            // 自定义属性
            private string $errorCode;

            public function __construct(string $message, string $errorCode = 'CUSTOM_ERROR')
            {
                parent::__construct($message);
                $this->errorCode = $errorCode;
            }

            // 自定义方法
            public function getErrorCode(): string
            {
                return $this->errorCode;
            }

            // 自定义错误详情
            public function getDetails(): string
            {
                return "[{$this->errorCode}] {$this->message} in {$this->file}:{$this->line}";
            }
        }

        try {
            throw new CustomException('自定义异常测试', 'ERR_001');
        } catch (CustomException $e) {
            echo "  捕获自定义异常：\n";
            echo "    消息: " . $e->getMessage() . "\n";
            echo "    错误码: " . $e->getErrorCode() . "\n";
            echo "    详情: " . $e->getDetails() . "\n";
        }

        echo "\n";
    }

    // ============================================================
    // 2. 错误处理
    // ============================================================

    /**
     * 演示 PHP 错误处理
     * 
     * PHP 错误 vs 异常：
     * - 错误：由 PHP 内核产生，如语法错误、未定义变量等
     * - 异常：由程序主动 throw 抛出
     */
    public function errorHandling(): void
    {
        echo "=== 错误处理 ===\n";

        // ---------- 错误级别 ----------
        echo "常见错误级别：\n";
        echo "  E_ERROR     - 致命错误（脚本终止）\n";
        echo "  E_WARNING   - 警告（脚本继续）\n";
        echo "  E_PARSE     - 语法解析错误\n";
        echo "  E_NOTICE    - 通知（可能是问题也可能不是）\n";
        echo "  E_STRICT    - 编码标准建议\n";
        echo "  E_ALL       - 所有错误和警告\n\n";

        // ---------- error_reporting() ----------
        echo "error_reporting() - 设置错误报告级别：\n";
        echo "  error_reporting(E_ALL);           // 显示所有错误\n";
        echo "  error_reporting(E_ALL & ~E_NOTICE); // 显示除了 NOTICE 之外的错误\n";
        echo "  error_reporting(0);               // 关闭所有错误显示\n\n";

        // ---------- set_error_handler() ----------
        echo "set_error_handler() - 自定义错误处理函数：\n";
        echo "  可以接管 PHP 的错误处理\n";
        echo "  注意：只能处理非致命错误（E_WARNING, E_NOTICE 等）\n";
        echo "  致命错误（E_ERROR）无法通过这个函数捕获\n\n";

        // 演示设置错误处理器
        set_error_handler(function ($errno, $errstr, $errfile, $errline) {
            echo "  [自定义错误处理] 错误号: {$errno}, 消息: {$errstr}\n";
            echo "    文件: {$errfile}, 行号: {$errline}\n";
            // 返回 true 表示错误已处理，PHP 不再继续处理
            return true;
        });

        // 触发一个通知级别的错误
        $undefinedVar;  // 未定义变量，会触发 E_NOTICE
        echo "  触发了一个未定义变量的错误（已被自定义处理器捕获）\n";

        // 恢复默认错误处理器
        restore_error_handler();

        // ---------- 错误 vs 异常 ----------
        echo "\n错误 vs 异常：\n";
        echo "  - 错误：PHP 运行时产生，旧式错误处理方式\n";
        echo "  - 异常：面向对象的错误处理，可捕获可传递\n";
        echo "  - ErrorException：可以将错误转换为异常\n";
        echo "  - PHP 7+ 中，很多致命错误也变成了 Error 类的异常\n";

        echo "\n";
    }

    // ============================================================
    // 3. 闭包与匿名函数
    // ============================================================

    /**
     * 演示闭包与匿名函数
     * 
     * 匿名函数（Anonymous functions），也叫闭包函数（closures）
     * 是没有名称的函数，可以赋值给变量或作为参数传递
     */
    public function closures(): void
    {
        echo "=== 闭包与匿名函数 ===\n";

        // ---------- 基本匿名函数 ----------
        echo "基本匿名函数：\n";

        // 将匿名函数赋值给变量
        $greet = function ($name) {
            return "Hello, {$name}!";
        };

        echo "  \$greet('World') = " . $greet('World') . "\n";

        // 作为回调函数使用
        $numbers = [1, 2, 3, 4, 5];
        $squared = array_map(function ($n) {
            return $n * $n;
        }, $numbers);
        echo "  array_map 匿名函数求平方: [" . implode(', ', $squared) . "]\n";

        // ---------- use 关键字（继承父作用域变量）----------
        echo "\nuse 关键字 - 闭包继承父作用域变量：\n";

        $prefix = 'Hello, ';  // 父作用域的变量

        $sayHello = function ($name) use ($prefix) {
            // use 关键字用于从父作用域继承变量
            // 默认是按值传递（复制一份）
            return $prefix . $name;
        };

        echo "  \$sayHello('张三') = " . $sayHello('张三') . "\n";

        // use 引用传递（&）
        echo "\nuse 引用传递（&）：\n";
        $count = 0;

        $increment = function () use (&$count) {
            // & 表示按引用传递，可以修改外部变量
            $count++;
        };

        $increment();
        $increment();
        $increment();
        echo "  调用 3 次后，\$count = {$count}\n";

        // ---------- 闭包的绑定 ----------
        echo "\n闭包的绑定（bindTo）：\n";

        // 定义一个闭包，引用 $this
        $closure = function () {
            return '访问对象属性：' . $this->name;
        };

        // 创建一个对象
        $obj = new \stdClass();
        $obj->name = '测试对象';

        // 将闭包绑定到对象上，闭包中的 $this 就指向该对象
        $boundClosure = $closure->bindTo($obj);
        echo "  绑定后调用: " . $boundClosure() . "\n";

        // ---------- 箭头函数（PHP 7.4+）----------
        echo "\n箭头函数（PHP 7.4+）：\n";
        echo "  语法：fn(参数) => 表达式\n";
        echo "  特点：单行表达式，自动返回值，自动 use 父作用域变量\n\n";

        $add = fn($a, $b) => $a + $b;
        echo "  \$add = fn(\$a, \$b) => \$a + \$b;\n";
        echo "  \$add(3, 5) = " . $add(3, 5) . "\n";

        // 箭头函数自动捕获父作用域变量（不需要 use）
        $factor = 2;
        $multiply = fn($n) => $n * $factor;
        echo "  箭头函数自动 use 外部变量: \$multiply(5) = " . $multiply(5) . "\n";

        echo "\n";
    }

    // ============================================================
    // 4. 生成器 yield
    // ============================================================

    /**
     * 演示生成器（Generator）
     * 
     * 生成器是 PHP 5.5+ 引入的特性
     * 使用 yield 关键字创建生成器函数
     * 生成器可以节省内存，特别适合处理大数据集
     */
    public function generators(): void
    {
        echo "=== 生成器 yield ===\n";
        echo "生成器是一种特殊的迭代器，使用 yield 关键字\n";
        echo "优点：节省内存，不需要一次性加载所有数据到内存\n\n";

        // ---------- 基本生成器 ----------
        echo "基本生成器：\n";

        /**
         * 生成器函数 - 生成指定范围的数字
         * 调用生成器函数返回一个 Generator 对象
         * 
         * @param int $start 起始值
         * @param int $end 结束值
         * @return \Generator
         */
        function xrange(int $start, int $end): \Generator
        {
            for ($i = $start; $i <= $end; $i++) {
                // yield 关键字产生一个值并暂停执行
                // 下次迭代时从暂停处继续
                yield $i;
            }
        }

        echo "  xrange(1, 5) 生成 1 到 5 的数字：\n  ";
        foreach (xrange(1, 5) as $num) {
            echo $num . " ";
        }
        echo "\n";

        // ---------- 生成器 vs 数组 ----------
        echo "\n生成器 vs 数组（内存对比）：\n";
        echo "  range(1, 1000000) 需要内存存储全部 100 万个数字\n";
        echo "  xrange(1, 1000000) 只需要存储当前值，内存几乎不增长\n";
        echo "  处理大数据集时优势明显\n";

        // ---------- 带键的生成器 ----------
        echo "\n带键的生成器（yield 键 => 值）：\n";

        function getUserData(): \Generator
        {
            yield 'name' => '张三';
            yield 'age' => 25;
            yield 'email' => 'zhangsan@example.com';
        }

        foreach (getUserData() as $key => $value) {
            echo "  {$key}: {$value}\n";
        }

        // ---------- 从生成器返回值（PHP 7+）----------
        echo "\n生成器返回值（PHP 7+ 的 return）：\n";

        function genWithReturn(): \Generator
        {
            yield 1;
            yield 2;
            yield 3;
            return '完成';  // 生成器的返回值
        }

        $gen = genWithReturn();
        foreach ($gen as $val) {
            echo "  yield: {$val}\n";
        }
        echo "  return: " . $gen->getReturn() . "\n";

        // ---------- yield from（PHP 7+）----------
        echo "\nyield from（PHP 7+）- 委托生成器：\n";

        function innerGen(): \Generator
        {
            yield 'a';
            yield 'b';
        }

        function outerGen(): \Generator
        {
            yield 'start';
            yield from innerGen();  // 委托给另一个生成器
            yield 'end';
        }

        echo "  outerGen() 输出：\n  ";
        foreach (outerGen() as $val) {
            echo $val . " ";
        }
        echo "\n";

        echo "\n";
    }

    // ============================================================
    // 5. 反射 Reflection
    // ============================================================

    /**
     * 演示反射（Reflection）
     * 
     * 反射 API 可以反向分析类、接口、函数、方法、扩展等
     * 常用于框架、依赖注入容器、ORM 等
     */
    public function reflection(): void
    {
        echo "=== 反射 Reflection ===\n";
        echo "反射 API 用于分析代码结构（类、方法、属性等）\n";
        echo "常用于框架、DI 容器、ORM、文档生成等场景\n\n";

        // 演示用的类
        $demoClass = new class('张三', 25) {
            public string $name;
            private int $age;

            public function __construct(string $name, int $age)
            {
                $this->name = $name;
                $this->age = $age;
            }

            public function greet(): string
            {
                return "Hello, {$this->name}!";
            }

            private function secret(): void
            {
                // 私有方法
            }
        };

        // ---------- ReflectionClass - 类反射 ----------
        echo "ReflectionClass - 类反射：\n";
        $refClass = new \ReflectionClass($demoClass);

        echo "  类名: " . $refClass->getName() . "\n";
        echo "  是否匿名类: " . ($refClass->isAnonymous() ? '是' : '否') . "\n";
        echo "  属性数量: " . count($refClass->getProperties()) . "\n";
        echo "  方法数量: " . count($refClass->getMethods()) . "\n";

        // 获取构造函数
        $constructor = $refClass->getConstructor();
        echo "  构造函数参数数量: " . ($constructor ? $constructor->getNumberOfParameters() : 0) . "\n";

        // ---------- ReflectionMethod - 方法反射 ----------
        echo "\nReflectionMethod - 方法反射：\n";
        $refMethod = $refClass->getMethod('greet');

        echo "  方法名: " . $refMethod->getName() . "\n";
        echo "  是否公开: " . ($refMethod->isPublic() ? '是' : '否') . "\n";
        echo "  返回值类型: " . ($refMethod->hasReturnType() ? $refMethod->getReturnType() : '无') . "\n";

        // 调用方法
        $result = $refMethod->invoke($demoClass);
        echo "  调用结果: {$result}\n";

        // ---------- ReflectionProperty - 属性反射 ----------
        echo "\nReflectionProperty - 属性反射：\n";
        $refProp = $refClass->getProperty('name');

        echo "  属性名: " . $refProp->getName() . "\n";
        echo "  类型: " . ($refProp->hasType() ? $refProp->getType() : '无') . "\n";
        echo "  值: " . $refProp->getValue($demoClass) . "\n";

        // 访问私有属性（需要先设置可访问）
        $privateProp = $refClass->getProperty('age');
        $privateProp->setAccessible(true);  // 设置为可访问
        echo "  私有属性 age 的值: " . $privateProp->getValue($demoClass) . "\n";

        // ---------- 反射的应用场景 ----------
        echo "\n反射的应用场景：\n";
        echo "  1. 依赖注入容器：根据类型提示自动注入依赖\n";
        echo "  2. ORM 框架：根据类属性映射数据库表字段\n";
        echo "  3. 路由框架：解析控制器方法的参数\n";
        echo "  4. 单元测试：测试私有方法\n";
        echo "  5. 文档生成：自动生成 API 文档\n";

        echo "\n";
    }

    // ============================================================
    // 6. 正则表达式
    // ============================================================

    /**
     * 演示正则表达式
     * 
     * PHP 使用 PCRE（Perl Compatible Regular Expressions）正则语法
     * 主要函数：preg_match, preg_match_all, preg_replace, preg_split
     */
    public function regex(): void
    {
        echo "=== 正则表达式 ===\n";
        echo "PHP 使用 PCRE 正则引擎，函数以 preg_ 开头\n\n";

        // ---------- preg_match() - 匹配一次 ----------
        echo "preg_match() - 正则匹配（只匹配一次）：\n";

        $email = 'user@example.com';
        // 邮箱验证正则（简化版）
        $pattern = '/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/';

        $isValid = preg_match($pattern, $email);
        echo "  验证邮箱 '{$email}': " . ($isValid ? '有效' : '无效') . "\n";

        // 捕获组
        $str = '今天是 2024-01-15，天气晴朗';
        $pattern = '/(\d{4})-(\d{2})-(\d{2})/';
        $matches = [];
        if (preg_match($pattern, $str, $matches)) {
            echo "  匹配日期：\n";
            echo "    完整匹配: {$matches[0]}\n";
            echo "    年: {$matches[1]}\n";
            echo "    月: {$matches[2]}\n";
            echo "    日: {$matches[3]}\n";
        }

        // ---------- preg_match_all() - 匹配所有 ----------
        echo "\npreg_match_all() - 匹配所有出现：\n";

        $text = '苹果10元，香蕉20元，橘子15元';
        $pattern = '/(\d+)元/';
        $allMatches = [];
        preg_match_all($pattern, $text, $allMatches);

        echo "  匹配所有价格：\n";
        foreach ($allMatches[1] as $price) {
            echo "    {$price} 元\n";
        }
        echo "  共找到 " . count($allMatches[0]) . " 个匹配\n";

        // ---------- preg_replace() - 正则替换 ----------
        echo "\npreg_replace() - 正则替换：\n";

        $text = 'Hello   World   123';
        echo "  原始: '{$text}'\n";

        // 多个空格替换为单个空格
        $result = preg_replace('/\s+/', ' ', $text);
        echo "  多空格变单空格: '{$result}'\n";

        // 替换数字为 X
        $result = preg_replace('/\d/', 'X', $text);
        echo "  数字替换为 X: '{$result}'\n";

        // ---------- preg_split() - 正则分割 ----------
        echo "\npreg_split() - 正则分割：\n";

        $csv = 'a, b, c; d; e';
        $parts = preg_split('/[,;]\s*/', $csv);
        echo "  按逗号和分号分割: [" . implode(', ', $parts) . "]\n";

        // ---------- 常用正则模式 ----------
        echo "\n常用正则模式：\n";
        echo "  /^1[3-9]\\d{9}$/      - 手机号\n";
        echo "  /^\\d{6}$/             - 6位数字验证码\n";
        echo "  /^\\d{15}|\\d{18}$/    - 身份证号（简化）\n";
        echo "  /^[\x{4e00}-\x{9fa5}]+$/u - 纯中文\n";
        echo "  /<[^>]+>/              - HTML标签\n";

        echo "\n正则修饰符：\n";
        echo "  i - 不区分大小写\n";
        echo "  m - 多行模式\n";
        echo "  s - 点号匹配换行\n";
        echo "  u - UTF-8 模式（处理中文等多字节字符）\n";
        echo "  x - 忽略空白和注释\n";

        echo "\n";
    }

    // ============================================================
    // 7. 会话管理
    // ============================================================

    /**
     * 演示会话管理（Session）
     * 
     * Session 用于在服务器端存储用户数据，跨页面保持状态
     */
    public function sessions(): void
    {
        echo "=== 会话管理 Session ===\n";
        echo "Session 用于在服务器端存储用户数据，保持会话状态\n\n";

        echo "基本使用：\n";
        echo "  1. session_start() - 启动会话（必须在输出之前调用）\n";
        echo "  2. \$_SESSION - 超全局数组，存储会话数据\n";
        echo "  3. 会话通过 Cookie 中的 PHPSESSID 标识用户\n\n";

        // 启动会话（如果还没启动的话）
        if (session_status() === PHP_SESSION_NONE) {
            // 命令行模式下可能无法正常工作
            if (php_sapi_name() !== 'cli') {
                session_start();
            }
        }

        echo "会话操作：\n";
        echo "  // 存储数据\n";
        echo "  \$_SESSION['user_id'] = 123;\n";
        echo "  \$_SESSION['username'] = '张三';\n\n";
        echo "  // 读取数据\n";
        echo "  \$userId = \$_SESSION['user_id'] ?? null;\n\n";
        echo "  // 删除数据\n";
        echo "  unset(\$_SESSION['username']);\n\n";
        echo "  // 销毁会话\n";
        echo "  session_destroy();\n";

        echo "\nSession 配置（php.ini）：\n";
        echo "  session.save_path     - 会话文件存储路径\n";
        echo "  session.save_handler  - 存储方式（files, redis, memcached）\n";
        echo "  session.gc_maxlifetime - 最大生命周期（秒）\n";
        echo "  session.cookie_lifetime - Cookie 有效期\n";
        echo "  session.cookie_secure  - 仅 HTTPS 传输\n";
        echo "  session.cookie_httponly - 禁止 JS 访问（防 XSS）\n";

        echo "\nSession 安全：\n";
        echo "  - 使用 session_regenerate_id() 定期更新会话ID\n";
        echo "  - 设置 cookie_httponly 防止 XSS 窃取\n";
        echo "  - 设置 cookie_secure 只在 HTTPS 下传输\n";
        echo "  - 敏感操作前验证用户身份\n";

        echo "\n";
    }

    // ============================================================
    // 8. Cookie 操作
    // ============================================================

    /**
     * 演示 Cookie 操作
     * 
     * Cookie 是存储在客户端（浏览器）的小型数据
     * 用于跟踪用户、记住偏好设置等
     */
    public function cookies(): void
    {
        echo "=== Cookie 操作 ===\n";
        echo "Cookie 存储在客户端（浏览器），用于跟踪用户状态\n\n";

        echo "设置 Cookie：\n";
        echo "  setcookie(name, value, expire, path, domain, secure, httponly)\n\n";
        echo "  参数说明：\n";
        echo "    name     - Cookie 名称\n";
        echo "    value    - Cookie 值\n";
        echo "    expire   - 过期时间（Unix 时间戳）\n";
        echo "    path     - 有效路径（'/' 表示整个网站）\n";
        echo "    domain   - 有效域名\n";
        echo "    secure   - 是否仅 HTTPS 传输\n";
        echo "    httponly - 是否禁止 JS 访问\n\n";

        echo "示例：\n";
        echo "  // 设置一个 1 小时后过期的 Cookie\n";
        echo "  setcookie('username', '张三', time() + 3600, '/');\n\n";
        echo "  // 读取 Cookie\n";
        echo "  \$username = \$_COOKIE['username'] ?? '访客';\n\n";
        echo "  // 删除 Cookie（设置过期时间为过去的时间）\n";
        echo "  setcookie('username', '', time() - 3600, '/');\n";

        echo "\nCookie vs Session：\n";
        echo "  +-----------+-------------+-------------+\n";
        echo "  | 特性      | Cookie      | Session     |\n";
        echo "  +-----------+-------------+-------------+\n";
        echo "  | 存储位置  | 客户端      | 服务器端    |\n";
        echo "  | 安全性    | 较低        | 较高        |\n";
        echo "  | 存储大小  | 约 4KB      | 无限制      |\n";
        echo "  | 有效期    | 可设置      | 会话结束    |\n";
        echo "  | 用途      | 记住偏好    | 登录状态    |\n";
        echo "  +-----------+-------------+-------------+\n";

        echo "\n注意：\n";
        echo "  - setcookie() 必须在任何输出之前调用\n";
        echo "  - Cookie 值在 \$_COOKIE 中读取\n";
        echo "  - 不要在 Cookie 中存储敏感信息（密码等）\n";
        echo "  - Cookie 可以被用户禁用或修改\n";

        echo "\n";
    }

    // ============================================================
    // 9. 文件上传处理
    // ============================================================

    /**
     * 演示文件上传处理
     */
    public function fileUpload(): void
    {
        echo "=== 文件上传处理 ===\n";
        echo "PHP 通过 \$_FILES 超全局变量处理文件上传\n\n";

        echo "HTML 表单：\n";
        echo "  <form method=\"post\" enctype=\"multipart/form-data\">\n";
        echo "      <input type=\"file\" name=\"avatar\">\n";
        echo "      <input type=\"submit\" value=\"上传\">\n";
        echo "  </form>\n\n";
        echo "  注意：必须设置 enctype=\"multipart/form-data\"\n\n";

        echo "\$_FILES 数组结构：\n";
        echo "  \$_FILES['avatar']['name']     - 原始文件名\n";
        echo "  \$_FILES['avatar']['type']     - MIME 类型\n";
        echo "  \$_FILES['avatar']['tmp_name'] - 临时文件路径\n";
        echo "  \$_FILES['avatar']['error']    - 错误码\n";
        echo "  \$_FILES['avatar']['size']     - 文件大小（字节）\n\n";

        echo "错误码：\n";
        echo "  UPLOAD_ERR_OK (0)        - 上传成功\n";
        echo "  UPLOAD_ERR_INI_SIZE (1)  - 超过 php.ini 限制\n";
        echo "  UPLOAD_ERR_FORM_SIZE (2) - 超过表单 MAX_FILE_SIZE 限制\n";
        echo "  UPLOAD_ERR_PARTIAL (3)   - 文件只有部分被上传\n";
        echo "  UPLOAD_ERR_NO_FILE (4)   - 没有文件被上传\n";
        echo "  UPLOAD_ERR_NO_TMP_DIR (6)- 找不到临时文件夹\n";
        echo "  UPLOAD_ERR_CANT_WRITE (7)- 文件写入失败\n\n";

        echo "处理步骤：\n";
        echo "  1. 检查错误码\n";
        echo "  2. 验证文件类型（不要信任 \$_FILES['type']）\n";
        echo "  3. 验证文件大小\n";
        echo "  4. 生成安全的文件名（防止路径遍历攻击）\n";
        echo "  5. move_uploaded_file() 移动到目标位置\n\n";

        echo "示例代码：\n";
        echo "  if (\$_FILES['avatar']['error'] === UPLOAD_ERR_OK) {\n";
        echo "      \$tmpName = \$_FILES['avatar']['tmp_name'];\n";
        echo "      \$dest = 'uploads/' . uniqid() . '.png';\n";
        echo "      move_uploaded_file(\$tmpName, \$dest);\n";
        echo "  }\n";

        echo "\n安全注意：\n";
        echo "  - 不要将上传文件放在 web 可访问目录的根路径\n";
        echo "  - 不要使用原始文件名（可能包含恶意路径）\n";
        echo "  - 验证文件真实类型（使用 finfo 或 getimagesize）\n";
        echo "  - 限制文件大小\n";
        echo "  - 上传目录禁止执行 PHP 脚本\n";

        echo "\n";
    }

    // ============================================================
    // 10. 日期时间 DateTime 类
    // ============================================================

    /**
     * 演示 DateTime 类
     * 
     * PHP 5.2+ 引入的面向对象的日期时间处理方式
     * 比 date() / strtotime() 更强大、更易读
     */
    public function dateTime(): void
    {
        echo "=== 日期时间 DateTime 类 ===\n";
        echo "DateTime 是 PHP 面向对象的日期时间处理类\n\n";

        // ---------- 创建 DateTime 对象 ----------
        echo "创建 DateTime 对象：\n";

        $now = new DateTime();  // 当前时间
        echo "  new DateTime() = " . $now->format('Y-m-d H:i:s') . "\n";

        $specific = new DateTime('2024-01-15 10:30:00');  // 指定时间
        echo "  new DateTime('2024-01-15 10:30:00') = " . $specific->format('Y-m-d H:i:s') . "\n";

        // 带时区
        $beijing = new DateTime('now', new DateTimeZone('Asia/Shanghai'));
        echo "  带时区（Asia/Shanghai）= " . $beijing->format('Y-m-d H:i:s') . "\n";

        // ---------- 格式化输出 ----------
        echo "\n格式化输出 format()：\n";
        $dt = new DateTime('2024-01-15 14:30:45');
        echo "  Y-m-d              = " . $dt->format('Y-m-d') . "\n";
        echo "  Y/m/d H:i:s        = " . $dt->format('Y/m/d H:i:s') . "\n";
        echo "  Y年m月d日 H时i分   = " . $dt->format('Y年m月d日 H时i分') . "\n";
        echo "  l, F j, Y          = " . $dt->format('l, F j, Y') . "\n";

        // ---------- 日期修改 ----------
        echo "\n日期修改：\n";
        $dt = new DateTime('2024-01-15');

        // 修改日期
        $dt->setDate(2024, 12, 25);
        echo "  setDate(2024, 12, 25) = " . $dt->format('Y-m-d') . "\n";

        // 修改时间
        $dt->setTime(14, 30, 0);
        echo "  setTime(14, 30, 0)   = " . $dt->format('Y-m-d H:i:s') . "\n";

        // ---------- 日期运算 ----------
        echo "\n日期运算：\n";
        $dt = new DateTime('2024-01-15');

        // 添加时间
        $dt->add(new DateInterval('P10D'));  // 加 10 天（P10D = Period 10 Days）
        echo "  add 10天 = " . $dt->format('Y-m-d') . "\n";

        // 减少时间
        $dt->sub(new DateInterval('P1M'));   // 减 1 个月
        echo "  sub 1个月 = " . $dt->format('Y-m-d') . "\n";

        // modify 方法（字符串方式）
        $dt->modify('+1 week');
        echo "  modify('+1 week') = " . $dt->format('Y-m-d') . "\n";

        // ---------- 日期比较 ----------
        echo "\n日期比较：\n";
        $date1 = new DateTime('2024-01-15');
        $date2 = new DateTime('2024-12-25');

        echo "  date1 = " . $date1->format('Y-m-d') . "\n";
        echo "  date2 = " . $date2->format('Y-m-d') . "\n";
        echo "  date1 < date2: " . ($date1 < $date2 ? '是' : '否') . "\n";

        // 计算差值
        $diff = $date1->diff($date2);
        echo "  差值：{$diff->y}年 {$diff->m}月 {$diff->d}日\n";
        echo "  总天数：" . $diff->days . " 天\n";

        // ---------- 常用格式字符 ----------
        echo "\n常用格式字符：\n";
        echo "  Y - 4位年（2024）\n";
        echo "  m - 月份（01-12）\n";
        echo "  d - 日期（01-31）\n";
        echo "  H - 小时（00-23，24小时制）\n";
        echo "  h - 小时（01-12，12小时制）\n";
        echo "  i - 分钟（00-59）\n";
        echo "  s - 秒（00-59）\n";
        echo "  l - 星期几的完整文本\n";
        echo "  F - 月份的完整文本\n";
        echo "  w - 星期几的数字（0=周日, 6=周六）\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有高级特性演示
     */
    public function runAll(): void
    {
        $this->exceptionHandling();
        $this->errorHandling();
        $this->closures();
        $this->generators();
        $this->reflection();
        $this->regex();
        $this->sessions();
        $this->cookies();
        $this->fileUpload();
        $this->dateTime();
    }
}
