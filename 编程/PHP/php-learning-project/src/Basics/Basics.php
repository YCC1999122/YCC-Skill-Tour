<?php
// ============================================================
// PHP 基础语法学习 - Basics 类
// ============================================================
// 本文件包含 PHP 基础语法的所有核心知识点
// 每个知识点都配有详细的中文注释和可运行的示例代码
// ============================================================

// 声明命名空间：App\Basics
// 命名空间用于组织代码，避免类名、函数名冲突
// 对应 PSR-4 自动加载规范，文件路径为 src/Basics/Basics.php
namespace App\Basics;

/**
 * Basics 类 - PHP 基础语法演示
 * 
 * 包含以下知识点：
 * - PHP 标记与注释
 * - 基本数据类型
 * - 变量与作用域
 * - 常量
 * - 运算符
 * - 控制流
 * - 函数
 * - 字符串操作
 * - 类型转换与判断
 */
class Basics
{
    // ============================================================
    // 1. PHP 标记与注释
    // ============================================================

    /**
     * 演示 PHP 标记和注释的使用
     * 
     * PHP 标记：
     * - 标准标记：<?php ... ?> （推荐使用）
     * - 短标记：<? ... ?> （需要 php.ini 中 short_open_tag=On）
     * - 短输出标记：<?= ... ?> （等同于 <?php echo ... ?>）
     * 
     * 注释类型：
     * - 单行注释：// 或 #
     * - 多行注释：/* ... * /
     * - 文档注释：/** ... * / （用于生成文档）
     */
    public function phpTagsAndComments(): void
    {
        // 这是单行注释（// 风格）
        # 这也是单行注释（# 风格，不推荐使用）

        /*
         * 这是多行注释
         * 可以写很多行
         * 通常用于临时注释大块代码
         */

        echo "=== PHP 标记与注释 ===\n";
        echo "PHP 代码从 <?php 开始，到 ?> 结束\n";
        echo "单行注释使用 // \n";
        echo "多行注释使用 /* */ \n\n";
    }

    // ============================================================
    // 2. 基本数据类型
    // ============================================================

    /**
     * 演示 PHP 的基本数据类型
     * 
     * PHP 支持 8 种原始数据类型：
     * - 标量类型：int, float, string, bool
     * - 复合类型：array, object
     * - 特殊类型：resource, NULL
     * - 伪类型：callable, mixed, void 等
     */
    public function dataTypes(): void
    {
        echo "=== 基本数据类型 ===\n";

        // ---------- 整型（Integer）----------
        // 整型可以是十进制、十六进制（0x开头）、八进制（0开头）、二进制（0b开头）
        $intDecimal = 123;        // 十进制整数
        $intHex = 0x1A;           // 十六进制整数（等于十进制的 26）
        $intOctal = 0123;         // 八进制整数（等于十进制的 83）
        $intBinary = 0b1010;      // 二进制整数（等于十进制的 10）

        echo "整型示例：\n";
        echo "  十进制: {$intDecimal}\n";
        echo "  十六进制 0x1A: {$intHex}\n";
        echo "  八进制 0123: {$intOctal}\n";
        echo "  二进制 0b1010: {$intBinary}\n";

        // ---------- 浮点型（Float/Double）----------
        // 浮点数即实数，可以用小数或科学计数法表示
        $floatNormal = 3.14;      // 普通浮点数
        $floatScientific = 1.5e3; // 科学计数法（1.5 * 10^3 = 1500）

        echo "浮点型示例：\n";
        echo "  普通浮点数: {$floatNormal}\n";
        echo "  科学计数法 1.5e3: {$floatScientific}\n";

        // ---------- 字符串（String）----------
        // 字符串有四种定义方式：单引号、双引号、heredoc、nowdoc
        $strSingle = '单引号字符串';          // 单引号：不解析变量和转义字符（除了 \\ 和 \'）
        $strDouble = "双引号字符串";          // 双引号：解析变量和转义字符
        $strHeredoc = <<<EOT
这是 Heredoc 语法
类似于双引号字符串，会解析变量
适合大段文本
EOT;

        echo "字符串示例：\n";
        echo "  单引号: {$strSingle}\n";
        echo "  双引号: {$strDouble}\n";
        echo "  Heredoc: " . trim($strHeredoc) . "\n";

        // ---------- 布尔型（Boolean）----------
        // 只有两个值：true 和 false（不区分大小写）
        $boolTrue = true;
        $boolFalse = false;

        echo "布尔型示例：\n";
        echo "  true: " . ($boolTrue ? '真' : '假') . "\n";
        echo "  false: " . ($boolFalse ? '真' : '假') . "\n";

        // ---------- 数组（Array）----------
        // 数组是复合类型，可以存储多个值
        $arraySimple = [1, 2, 3];  // 索引数组
        $arrayAssoc = [             // 关联数组
            'name' => '张三',
            'age' => 25
        ];

        echo "数组示例：\n";
        echo "  索引数组: " . implode(', ', $arraySimple) . "\n";
        echo "  关联数组: name={$arrayAssoc['name']}, age={$arrayAssoc['age']}\n";

        // ---------- 对象（Object）----------
        // 对象是类的实例
        $obj = new \stdClass();     // 创建一个标准空对象
        $obj->name = 'PHP学习';     // 给对象添加属性

        echo "对象示例：\n";
        echo "  对象属性 name: {$obj->name}\n";

        // ---------- NULL ----------
        // NULL 表示一个变量没有值，不区分大小写
        $nullVar = null;

        echo "NULL 示例：\n";
        echo "  null变量是否为空: " . ($nullVar === null ? '是' : '否') . "\n";

        // ---------- 类型查看函数 ----------
        echo "\n类型查看函数：\n";
        echo "  gettype(\$intDecimal): " . gettype($intDecimal) . "\n"; // 获取变量类型
        echo "  var_dump(\$floatNormal): ";
        var_dump($floatNormal); // 打印变量的详细信息（类型和值）

        echo "\n";
    }

    // ============================================================
    // 3. 变量
    // ============================================================

    /**
     * 演示 PHP 变量的使用
     * 
     * PHP 变量特点：
     * - 以 $ 符号开头
     * - 大小写敏感
     * - 不需要声明类型（弱类型语言）
     * - 第一次赋值时自动创建
     */
    public function variables(): void
    {
        echo "=== 变量 ===\n";

        // ---------- 变量定义 ----------
        // 变量以 $ 开头，后面跟变量名
        // 变量名必须以字母或下划线开头，不能以数字开头
        $name = '张三';           // 普通变量
        $_privateVar = '私有风格';  // 下划线开头的变量
        $userName = '李四';        // 驼峰命名法
        $user_age = 30;            // 下划线命名法

        echo "变量定义：\n";
        echo "  name: {$name}\n";
        echo "  userName: {$userName}\n";
        echo "  user_age: {$user_age}\n";

        // ---------- 可变变量（变量的变量）----------
        // 变量名可以动态指定
        $normalVar = 'hello';       // 普通变量
        $$normalVar = 'world';      // 可变变量，相当于 $hello = 'world'

        echo "可变变量：\n";
        echo "  \$normalVar = {$normalVar}\n";
        echo "  \$\$normalVar = 'world' → \$hello = {$hello}\n";

        // ---------- 变量作用域 ----------
        // 函数内部的变量是局部变量，函数外部的是全局变量
        $globalVar = '我是全局变量';

        /**
         * 演示变量作用域的内部函数
         */
        $scopeDemo = function () use ($globalVar) {
            $localVar = '我是局部变量';
            echo "  函数内部局部变量: {$localVar}\n";
            echo "  通过 use 关键字使用外部变量: {$globalVar}\n";
        };

        echo "变量作用域：\n";
        $scopeDemo();

        // ---------- 超全局变量 ----------
        // 超全局变量在脚本的任何地方都可以访问
        echo "超全局变量（部分演示）：\n";
        echo "  \$_SERVER['PHP_SELF']: " . ($_SERVER['PHP_SELF'] ?? 'CLI模式下不可用') . "\n";
        echo "  \$_SERVER['SCRIPT_NAME']: " . ($_SERVER['SCRIPT_NAME'] ?? 'CLI模式下不可用') . "\n";
        echo "  其他超全局变量：\$_GET, \$_POST, \$_SESSION, \$_COOKIE, \$_FILES, \$_ENV, \$_REQUEST, \$GLOBALS\n";

        echo "\n";
    }

    // ============================================================
    // 4. 常量
    // ============================================================

    /**
     * 演示 PHP 常量的定义和使用
     * 
     * 常量特点：
     * - 常量值不能改变
     * - 常量不需要 $ 符号
     * - 常量可以在任何地方定义和访问
     * - 常量通常用大写字母命名
     */
    public function constants(): void
    {
        echo "=== 常量 ===\n";

        // ---------- define() 定义常量 ----------
        // define(常量名, 常量值, 是否大小写不敏感)
        // 第三个参数默认为 false，表示大小写敏感
        define('SITE_NAME', 'PHP学习网站');          // 定义普通常量
        define('VERSION', '1.0.0');                  // 版本号常量
        define('MAX_SIZE', 1024 * 1024);             // 可以使用表达式

        echo "define() 定义的常量：\n";
        echo "  SITE_NAME: " . SITE_NAME . "\n";
        echo "  VERSION: " . VERSION . "\n";
        echo "  MAX_SIZE: " . MAX_SIZE . " 字节\n";

        // ---------- const 关键字定义常量 ----------
        // const 必须在最顶层作用域定义（不能在函数、循环、if 中）
        // const 是语言结构，比 define() 稍快
        const APP_NAME = 'PHP Learning Project';
        const APP_VERSION = '2.0.0';

        echo "const 定义的常量：\n";
        echo "  APP_NAME: " . self::APP_NAME . "\n";  // 类常量用 self:: 访问
        echo "  APP_VERSION: " . self::APP_VERSION . "\n";

        // ---------- 系统常量 ----------
        echo "系统常用常量：\n";
        echo "  PHP_VERSION: " . PHP_VERSION . "\n";      // PHP 版本
        echo "  PHP_OS: " . PHP_OS . "\n";                // 操作系统
        echo "  PHP_INT_MAX: " . PHP_INT_MAX . "\n";      // 最大整数
        echo "  PHP_EOL: 行结束符\n";                      // 行结束符（换行）
        echo "  DIRECTORY_SEPARATOR: " . DIRECTORY_SEPARATOR . "\n"; // 目录分隔符

        // ---------- 魔术常量 ----------
        echo "魔术常量：\n";
        echo "  __LINE__: " . __LINE__ . "\n";           // 当前行号
        echo "  __FILE__: " . __FILE__ . "\n";           // 当前文件完整路径
        echo "  __DIR__: " . __DIR__ . "\n";             // 当前文件所在目录
        echo "  __FUNCTION__: " . __FUNCTION__ . "\n";   // 当前函数名
        echo "  __CLASS__: " . __CLASS__ . "\n";         // 当前类名
        echo "  __METHOD__: " . __METHOD__ . "\n";       // 当前方法名（类::方法）
        echo "  __NAMESPACE__: " . __NAMESPACE__ . "\n"; // 当前命名空间

        echo "\n";
    }

    // ============================================================
    // 5. 运算符
    // ============================================================

    /**
     * 演示 PHP 的各种运算符
     */
    public function operators(): void
    {
        echo "=== 运算符 ===\n";

        $a = 10;
        $b = 3;

        // ---------- 算术运算符 ----------
        echo "算术运算符（a=10, b=3）：\n";
        echo "  加法 a + b = " . ($a + $b) . "\n";      // 加
        echo "  减法 a - b = " . ($a - $b) . "\n";      // 减
        echo "  乘法 a * b = " . ($a * $b) . "\n";      // 乘
        echo "  除法 a / b = " . ($a / $b) . "\n";      // 除
        echo "  取模 a % b = " . ($a % $b) . "\n";      // 取余
        echo "  幂运算 a ** b = " . ($a ** $b) . "\n";  // 幂（a 的 b 次方）
        echo "  自增 ++a = " . (++$a) . "（a现在=" . $a . "）\n";  // 前置自增
        echo "  自减 --a = " . (--$a) . "（a现在=" . $a . "）\n";  // 前置自减

        // ---------- 比较运算符 ----------
        echo "\n比较运算符：\n";
        echo "  10 == '10' : " . (10 == '10' ? 'true（值相等）' : 'false') . "\n";     // 等于（只比较值，自动类型转换）
        echo "  10 === '10': " . (10 === '10' ? 'true' : 'false（类型不同）') . "\n";  // 全等于（值和类型都相等）
        echo "  10 != '10' : " . (10 != '10' ? 'true' : 'false（值相等）') . "\n";     // 不等于
        echo "  10 !== '10': " . (10 !== '10' ? 'true（类型不同）' : 'false') . "\n";  // 不全等于
        echo "  10 > 5     : " . (10 > 5 ? 'true' : 'false') . "\n";                    // 大于
        echo "  10 < 5     : " . (10 < 5 ? 'true' : 'false') . "\n";                    // 小于
        echo "  10 >= 10   : " . (10 >= 10 ? 'true' : 'false') . "\n";                  // 大于等于
        echo "  10 <= 10   : " . (10 <= 10 ? 'true' : 'false') . "\n";                  // 小于等于

        // ---------- 太空船运算符 <=>（PHP 7.0+）----------
        // 左边 < 右边 返回 -1，相等返回 0，左边 > 右边返回 1
        echo "\n太空船运算符 <=>：\n";
        echo "  1 <=> 2  = " . (1 <=> 2) . "（左边小）\n";
        echo "  2 <=> 2  = " . (2 <=> 2) . "（相等）\n";
        echo "  3 <=> 2  = " . (3 <=> 2) . "（左边大）\n";

        // ---------- 逻辑运算符 ----------
        $x = true;
        $y = false;
        echo "\n逻辑运算符：\n";
        echo "  true && false = " . ($x && $y ? 'true' : 'false') . "（与运算，都真才真）\n";
        echo "  true || false = " . ($x || $y ? 'true' : 'false') . "（或运算，一真则真）\n";
        echo "  !true         = " . (!$x ? 'true' : 'false') . "（非运算，取反）\n";
        echo "  true xor true = " . ($x xor true ? 'true' : 'false') . "（异或，不同才真）\n";

        // ---------- 字符串运算符 ----------
        echo "\n字符串运算符：\n";
        $str1 = 'Hello';
        $str2 = 'World';
        echo "  'Hello' . ' ' . 'World' = " . ($str1 . ' ' . $str2) . "（拼接）\n";
        $str1 .= ' PHP';  // 拼接赋值
        echo "  \$str1 .= ' PHP' 后，\$str1 = {$str1}\n";

        // ---------- 三元运算符 ----------
        // 条件 ? 值1 : 值2  （条件为真返回值1，否则返回值2）
        echo "\n三元运算符：\n";
        $age = 20;
        $status = $age >= 18 ? '成年' : '未成年';
        echo "  age=20, status = age >= 18 ? '成年' : '未成年' → {$status}\n";

        // ---------- 空合并运算符 ??（PHP 7.0+）----------
        // 如果第一个值存在且不为 null 就返回它，否则返回第二个值
        echo "\n空合并运算符 ??：\n";
        $undefinedVar = $notSet ?? '默认值';
        echo "  \$notSet ?? '默认值' → {$undefinedVar}\n";

        $userName = '张三';
        $displayName = $userName ?? '访客';
        echo "  \$userName ?? '访客' → {$displayName}\n";

        // 空合并赋值运算符 ??=（PHP 7.4+）
        $testVar = null;
        $testVar ??= '初始值';
        echo "  \$testVar ??= '初始值' → {$testVar}\n";

        echo "\n";
    }

    // ============================================================
    // 6. 控制流
    // ============================================================

    /**
     * 演示 PHP 的控制流语句
     */
    public function controlFlow(): void
    {
        echo "=== 控制流 ===\n";

        // ---------- if / else / elseif ----------
        echo "if/else/elseif：\n";
        $score = 85;
        if ($score >= 90) {
            echo "  分数 {$score}：优秀\n";
        } elseif ($score >= 80) {
            echo "  分数 {$score}：良好\n";
        } elseif ($score >= 60) {
            echo "  分数 {$score}：及格\n";
        } else {
            echo "  分数 {$score}：不及格\n";
        }

        // ---------- switch ----------
        echo "\nswitch：\n";
        $day = 3;
        switch ($day) {
            case 1:
                echo "  星期一\n";
                break;  // break 用于跳出 switch，否则会继续执行后面的 case
            case 2:
                echo "  星期二\n";
                break;
            case 3:
                echo "  星期三\n";
                break;
            default:        // 默认分支，所有 case 都不匹配时执行
                echo "  其他日期\n";
                break;
        }

        // PHP 8.0+ match 表达式（switch 的表达式版本）
        echo "\nmatch 表达式（PHP 8.0+）：\n";
        $num = 2;
        $result = match ($num) {
            1 => '一',
            2 => '二',
            3 => '三',
            default => '其他'
        };
        echo "  match({$num}) → {$result}\n";

        // ---------- for 循环 ----------
        echo "\nfor 循环（1到5）：\n  ";
        for ($i = 1; $i <= 5; $i++) {
            echo $i . " ";
        }
        echo "\n";

        // ---------- while 循环 ----------
        echo "while 循环（1到5）：\n  ";
        $i = 1;
        while ($i <= 5) {
            echo $i . " ";
            $i++;
        }
        echo "\n";

        // ---------- do-while 循环 ----------
        // 先执行一次循环体，再判断条件（循环体至少执行一次）
        echo "do-while 循环（1到5）：\n  ";
        $i = 1;
        do {
            echo $i . " ";
            $i++;
        } while ($i <= 5);
        echo "\n";

        // ---------- foreach 循环 ----------
        echo "foreach 遍历数组：\n";
        $fruits = ['苹果', '香蕉', '橘子'];
        foreach ($fruits as $fruit) {
            echo "  {$fruit}\n";
        }

        // foreach 遍历关联数组（同时获取键和值）
        $person = ['name' => '张三', 'age' => 25, 'city' => '北京'];
        echo "foreach 遍历关联数组：\n";
        foreach ($person as $key => $value) {
            echo "  {$key}: {$value}\n";
        }

        // ---------- break 和 continue ----------
        echo "break 和 continue：\n";
        echo "  打印1-10，跳过5，遇到8停止：";
        for ($i = 1; $i <= 10; $i++) {
            if ($i == 5) {
                continue;  // continue 跳过本次循环，继续下一次
            }
            if ($i == 8) {
                break;     // break 终止整个循环
            }
            echo $i . " ";
        }
        echo "\n";

        echo "\n";
    }

    // ============================================================
    // 7. 函数
    // ============================================================

    /**
     * 演示 PHP 函数的使用
     */
    public function functions(): void
    {
        echo "=== 函数 ===\n";

        // ---------- 函数定义 ----------
        // 使用 function 关键字定义函数
        echo "函数定义：\n";

        /**
         * 简单的求和函数
         * 
         * @param int $a 第一个数
         * @param int $b 第二个数
         * @return int 两数之和
         */
        function add(int $a, int $b): int
        {
            return $a + $b;
        }

        echo "  add(3, 5) = " . add(3, 5) . "\n";

        // ---------- 参数默认值 ----------
        echo "\n参数默认值：\n";

        /**
         * 带默认参数的问候函数
         * 有默认值的参数必须放在没有默认值的参数后面
         * 
         * @param string $name 姓名
         * @param string $greeting 问候语，默认为"你好"
         * @return string
         */
        function greet(string $name, string $greeting = '你好'): string
        {
            return $greeting . '，' . $name . '！';
        }

        echo "  greet('张三') = " . greet('张三') . "\n";
        echo "  greet('李四', '早上好') = " . greet('李四', '早上好') . "\n";

        // ---------- 可变参数（... 运算符）----------
        echo "\n可变参数（...）：\n";

        /**
         * 计算所有参数的和
         * ...$nums 表示接收任意数量的参数，作为数组使用
         * 
         * @param int ...$nums 可变数量的整数参数
         * @return int
         */
        function sumAll(int ...$nums): int
        {
            $sum = 0;
            foreach ($nums as $n) {
                $sum += $n;
            }
            return $sum;
        }

        echo "  sumAll(1, 2, 3) = " . sumAll(1, 2, 3) . "\n";
        echo "  sumAll(1, 2, 3, 4, 5) = " . sumAll(1, 2, 3, 4, 5) . "\n";

        // 参数解包（把数组展开为函数参数）
        $numbers = [10, 20, 30];
        echo "  sumAll(...[10, 20, 30]) = " . sumAll(...$numbers) . "\n";

        // ---------- 引用参数 & ----------
        echo "\n引用参数 &：\n";

        /**
         * 使用引用传递参数，函数内部修改会影响外部变量
         * & 符号表示按引用传递
         * 
         * @param int &$num 引用参数
         */
        function addTen(int &$num): void
        {
            $num += 10;
        }

        $myNum = 5;
        echo "  调用前 \$myNum = {$myNum}\n";
        addTen($myNum);
        echo "  调用 addTen(\$myNum) 后，\$myNum = {$myNum}\n";

        // ---------- 返回类型声明 ----------
        echo "\n返回类型声明：\n";

        /**
         * 返回值类型声明为 string
         * PHP 7.0+ 支持标量类型声明
         * PHP 7.1+ 支持 void 类型
         * 
         * @param float $price 价格
         * @return string 格式化后的价格字符串
         */
        function formatPrice(float $price): string
        {
            return '¥' . number_format($price, 2);
        }

        echo "  formatPrice(99.5) = " . formatPrice(99.5) . "\n";

        // ---------- 匿名函数（闭包）----------
        echo "\n匿名函数：\n";
        $double = function ($n) {
            return $n * 2;
        };
        echo "  \$double(5) = " . $double(5) . "\n";

        // 箭头函数（PHP 7.4+）
        $triple = fn($n) => $n * 3;
        echo "  箭头函数 fn(\$n) => \$n * 3; (5) = " . $triple(5) . "\n";

        echo "\n";
    }

    // ============================================================
    // 8. 字符串操作
    // ============================================================

    /**
     * 演示 PHP 字符串操作
     */
    public function stringOperations(): void
    {
        echo "=== 字符串操作 ===\n";
        $str = 'Hello, World!';

        // ---------- 字符串拼接 ----------
        echo "字符串拼接：\n";
        echo "  'Hello' . ' ' . 'World' = " . ('Hello' . ' ' . 'World') . "\n";

        // ---------- 字符串长度 ----------
        echo "\n字符串长度：\n";
        echo "  strlen('{$str}') = " . strlen($str) . "（字节数）\n";
        echo "  mb_strlen('你好世界') = " . mb_strlen('你好世界') . "（字符数，多字节安全）\n";

        // ---------- 字符串截取 ----------
        echo "\n字符串截取：\n";
        echo "  substr('{$str}', 0, 5) = " . substr($str, 0, 5) . "\n";       // 从开头截取5个字符
        echo "  substr('{$str}', 7) = " . substr($str, 7) . "\n";             // 从第7个字符到结尾
        echo "  mb_substr('你好世界', 0, 2) = " . mb_substr('你好世界', 0, 2) . "（多字节安全）\n";

        // ---------- 字符串查找 ----------
        echo "\n字符串查找：\n";
        echo "  strpos('{$str}', 'World') = " . strpos($str, 'World') . "（第一次出现的位置）\n";
        echo "  strrpos('{$str}', 'o') = " . strrpos($str, 'o') . "（最后一次出现的位置）\n";
        echo "  strstr('{$str}', ',') = " . strstr($str, ',') . "（从匹配位置到结尾）\n";

        // ---------- 字符串替换 ----------
        echo "\n字符串替换：\n";
        echo "  str_replace('World', 'PHP', '{$str}') = " . str_replace('World', 'PHP', $str) . "\n";
        echo "  str_ireplace('hello', 'Hi', '{$str}') = " . str_ireplace('hello', 'Hi', $str) . "（不区分大小写）\n";

        // ---------- 字符串格式化 ----------
        echo "\n字符串格式化：\n";
        echo "  sprintf('今天是%d年%d月%d日', 2024, 1, 15) = " . sprintf('今天是%d年%d月%d日', 2024, 1, 15) . "\n";
        echo "  number_format(1234567.89, 2) = " . number_format(1234567.89, 2) . "\n";

        // ---------- 大小写转换 ----------
        echo "\n大小写转换：\n";
        echo "  strtolower('HELLO') = " . strtolower('HELLO') . "\n";
        echo "  strtoupper('hello') = " . strtoupper('hello') . "\n";
        echo "  ucfirst('hello') = " . ucfirst('hello') . "\n";
        echo "  ucwords('hello world') = " . ucwords('hello world') . "\n";

        // ---------- 去除空白 ----------
        echo "\n去除空白：\n";
        $trimStr = '  hello  ';
        echo "  trim('  hello  ') = '" . trim($trimStr) . "'（两端）\n";
        echo "  ltrim('  hello  ') = '" . ltrim($trimStr) . "'（左端）\n";
        echo "  rtrim('  hello  ') = '" . rtrim($trimStr) . "'（右端）\n";

        // ---------- 字符串分割与连接 ----------
        echo "\n字符串分割与连接：\n";
        $csv = '苹果,香蕉,橘子';
        $fruits = explode(',', $csv);  // 分割字符串为数组
        echo "  explode(',', '{$csv}') = [" . implode(', ', $fruits) . "]\n";
        echo "  implode('-', \$fruits) = " . implode('-', $fruits) . "（数组转字符串）\n";

        // ---------- 正则匹配 ----------
        echo "\n正则匹配：\n";
        $email = 'user@example.com';
        $pattern = '/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/';
        echo "  preg_match 验证邮箱 '{$email}': " . (preg_match($pattern, $email) ? '有效' : '无效') . "\n";

        // 正则替换
        $text = 'Hello 123 World 456';
        echo "  preg_replace('/\d+/', 'X', '{$text}') = " . preg_replace('/\d+/', 'X', $text) . "\n";

        echo "\n";
    }

    // ============================================================
    // 9. 类型转换与类型判断
    // ============================================================

    /**
     * 演示 PHP 类型转换与类型判断
     */
    public function typeCastingAndChecking(): void
    {
        echo "=== 类型转换与类型判断 ===\n";

        // ---------- 自动类型转换 ----------
        echo "自动类型转换（PHP 是弱类型语言）：\n";
        echo "  '10' + 5 = " . ('10' + 5) . "（字符串转数字）\n";
        echo "  true + 1 = " . (true + 1) . "（true 转 1）\n";
        echo "  false + 1 = " . (false + 1) . "（false 转 0）\n";

        // ---------- 强制类型转换 ----------
        echo "\n强制类型转换：\n";
        $numStr = '123.45abc';
        echo "  (int)'123.45abc' = " . (int)$numStr . "（转整型）\n";
        echo "  (float)'123.45abc' = " . (float)$numStr . "（转浮点型）\n";
        echo "  (string)123 = '" . (string)123 . "'（转字符串）\n";
        echo "  (bool)0 = " . ((bool)0 ? 'true' : 'false') . "（0 转布尔为 false）\n";
        echo "  (bool)'0' = " . ((bool)'0' ? 'true' : 'false') . "（'0' 转布尔也为 false）\n";
        echo "  (array)'hello' = [" . implode(', ', (array)'hello') . "]（转数组）\n";

        // 使用函数转换
        echo "\n类型转换函数：\n";
        echo "  intval('123') = " . intval('123') . "\n";
        echo "  floatval('3.14') = " . floatval('3.14') . "\n";
        echo "  strval(123) = '" . strval(123) . "'\n";
        echo "  boolval(0) = " . (boolval(0) ? 'true' : 'false') . "\n";

        // ---------- 类型判断函数 ----------
        echo "\n类型判断函数：\n";
        $testInt = 42;
        $testString = 'hello';
        $testArray = [1, 2];
        $testNull = null;

        echo "  is_int(42): " . (is_int($testInt) ? '是' : '否') . "\n";
        echo "  is_string('hello'): " . (is_string($testString) ? '是' : '否') . "\n";
        echo "  is_array([1,2]): " . (is_array($testArray) ? '是' : '否') . "\n";
        echo "  is_null(null): " . (is_null($testNull) ? '是' : '否') . "\n";
        echo "  is_numeric('123.45'): " . (is_numeric('123.45') ? '是' : '否') . "\n";
        echo "  is_bool(true): " . (is_bool(true) ? '是' : '否') . "\n";
        echo "  is_object(new stdClass()): " . (is_object(new \stdClass()) ? '是' : '否') . "\n";
        echo "  empty(''): " . (empty('') ? '是空的' : '不是空的') . "\n";
        echo "  isset(\$testInt): " . (isset($testInt) ? '已设置' : '未设置') . "\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有基础语法演示
     * 依次调用各个方法，展示所有知识点
     */
    public function runAll(): void
    {
        $this->phpTagsAndComments();
        $this->dataTypes();
        $this->variables();
        $this->constants();
        $this->operators();
        $this->controlFlow();
        $this->functions();
        $this->stringOperations();
        $this->typeCastingAndChecking();
    }
}
