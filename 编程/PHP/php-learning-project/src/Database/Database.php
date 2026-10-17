<?php
// ============================================================
// PHP 数据库操作学习 - Database 类
// ============================================================
// 本文件包含 PHP 数据库操作的核心知识点
// 主要讲解 PDO（PHP Data Objects）扩展
// PDO 是 PHP 操作数据库的推荐方式，支持多种数据库
// ============================================================

namespace App\Database;

use PDO;
use PDOStatement;
use PDOException;

/**
 * Database 类 - PHP 数据库操作演示
 * 
 * 包含以下知识点：
 * - PDO 连接数据库
 * - PDOStatement 预处理（防SQL注入）
 * - CRUD 操作
 * - 事务处理
 * - 错误处理模式
 * - fetch / fetchAll / fetchColumn
 * - MySQLi 扩展对比
 */
class Database
{
    // PDO 连接实例
    private ?PDO $pdo = null;

    // 数据库配置
    private string $host;
    private string $dbname;
    private string $username;
    private string $password;
    private string $charset;

    /**
     * 构造函数 - 设置数据库配置
     * 
     * @param string $host 数据库主机地址
     * @param string $dbname 数据库名
     * @param string $username 用户名
     * @param string $password 密码
     * @param string $charset 字符集
     */
    public function __construct(
        string $host = 'localhost',
        string $dbname = 'test',
        string $username = 'root',
        string $password = '',
        string $charset = 'utf8mb4'
    ) {
        $this->host = $host;
        $this->dbname = $dbname;
        $this->username = $username;
        $this->password = $password;
        $this->charset = $charset;
    }

    // ============================================================
    // 1. PDO 连接数据库
    // ============================================================

    /**
     * 连接数据库
     * 
     * PDO 连接的 DSN（Data Source Name）格式：
     * mysql:host=主机名;dbname=数据库名;charset=字符集
     * 
     * PDO 构造函数参数：
     * - DSN：数据源名称
     * - username：用户名
     * - password：密码
     * - options：驱动选项数组
     * 
     * @return PDO PDO 实例
     * @throws PDOException 连接失败时抛出异常
     */
    public function connect(): PDO
    {
        echo "=== PDO 连接数据库 ===\n";

        // 构建 DSN（数据源名称）
        // mysql:host=localhost;dbname=test;charset=utf8mb4
        $dsn = "mysql:host={$this->host};dbname={$this->dbname};charset={$this->charset}";

        echo "DSN: {$dsn}\n";
        echo "用户名: {$this->username}\n";

        // PDO 选项配置
        $options = [
            // 设置错误模式为异常模式（推荐）
            // PDO::ERRMODE_SILENT  - 静默模式，只设置错误代码（默认）
            // PDO::ERRMODE_WARNING - 警告模式，发出 PHP 警告
            // PDO::ERRMODE_EXCEPTION - 异常模式，抛出 PDOException 异常
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,

            // 设置默认的提取模式为关联数组
            // PDO::FETCH_ASSOC - 关联数组（列名 => 值）
            // PDO::FETCH_NUM   - 索引数组（数字索引）
            // PDO::FETCH_BOTH  - 两者都有（默认）
            // PDO::FETCH_OBJ   - 匿名对象
            // PDO::FETCH_CLASS - 指定类的对象
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,

            // 禁用模拟预处理（使用数据库原生的预处理，更安全）
            // 当 PHP 版本较老或数据库不支持预处理时，PDO 会模拟预处理
            // 设置为 false 强制使用真正的预处理，防止 SQL 注入
            PDO::ATTR_EMULATE_PREPARES => false,
        ];

        echo "PDO 选项：\n";
        echo "  - ATTR_ERRMODE: ERRMODE_EXCEPTION（异常模式）\n";
        echo "  - ATTR_DEFAULT_FETCH_MODE: FETCH_ASSOC（关联数组）\n";
        echo "  - ATTR_EMULATE_PREPARES: false（禁用模拟预处理）\n";

        try {
            // 创建 PDO 实例，建立数据库连接
            $this->pdo = new PDO($dsn, $this->username, $this->password, $options);
            echo "\n数据库连接成功！\n";
        } catch (PDOException $e) {
            // 连接失败时抛出异常
            echo "\n数据库连接失败：" . $e->getMessage() . "\n";
            throw $e;
        }

        echo "\n";
        return $this->pdo;
    }

    /**
     * 演示连接 SQLite（文件数据库，无需安装）
     * 用于演示目的，实际运行时可以连接 SQLite
     */
    public function connectSqlite(): PDO
    {
        echo "=== PDO 连接 SQLite 演示 ===\n";
        echo "SQLite 是文件型数据库，无需单独安装服务\n";

        // SQLite DSN 格式：sqlite:数据库文件路径
        $dsn = 'sqlite::memory:';  // 内存数据库，临时存储
        $this->pdo = new PDO($dsn);
        $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $this->pdo->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

        echo "SQLite 内存数据库连接成功\n\n";
        return $this->pdo;
    }

    // ============================================================
    // 2. PDOStatement 预处理语句（防 SQL 注入）
    // ============================================================

    /**
     * 演示预处理语句（Prepared Statement）
     * 
     * 预处理语句的好处：
     * 1. 防止 SQL 注入（参数与 SQL 语句分离）
     * 2. 多次执行时性能更好（语句只解析一次）
     * 3. 代码更清晰易读
     * 
     * 参数绑定方式：
     * - 命名占位符：:name, :age
     * - 问号占位符：?, ?
     */
    public function preparedStatements(): void
    {
        echo "=== 预处理语句（防止 SQL 注入）===\n";

        // 确保已连接数据库
        if (!$this->pdo) {
            $this->connectSqlite();
        }

        // 创建测试表
        $this->pdo->exec("
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT NOT NULL UNIQUE,
                age INTEGER
            )
        ");
        echo "创建测试表 users\n\n";

        // ---------- 命名占位符 ----------
        echo "命名占位符（:name 格式）：\n";

        // 1. 准备 SQL 语句
        // :name 和 :email 是命名占位符
        $sql = "INSERT INTO users (name, email, age) VALUES (:name, :email, :age)";
        $stmt = $this->pdo->prepare($sql);
        echo "  SQL: {$sql}\n";

        // 2. 绑定参数（方式一：bindParam）
        // bindParam 绑定的是变量引用，变量变化会影响结果
        $name = '张三';
        $email = 'zhangsan@example.com';
        $age = 25;
        $stmt->bindParam(':name', $name, PDO::PARAM_STR);   // 指定类型为字符串
        $stmt->bindParam(':email', $email, PDO::PARAM_STR);
        $stmt->bindParam(':age', $age, PDO::PARAM_INT);     // 指定类型为整数
        $stmt->execute();
        echo "  绑定参数并执行：插入 {$name}\n";

        // 3. 执行时传入参数（方式二：execute 数组）
        // 更简洁，推荐使用
        $stmt->execute([
            ':name'  => '李四',
            ':email' => 'lisi@example.com',
            ':age'   => 30
        ]);
        echo "  execute 数组传参：插入 李四\n";

        // ---------- 问号占位符 ----------
        echo "\n问号占位符（? 格式）：\n";
        $sql2 = "INSERT INTO users (name, email, age) VALUES (?, ?, ?)";
        $stmt2 = $this->pdo->prepare($sql2);
        echo "  SQL: {$sql2}\n";

        $stmt2->execute(['王五', 'wangwu@example.com', 28]);
        echo "  执行：插入 王五（按顺序对应）\n";

        // ---------- SQL 注入演示 ----------
        echo "\n为什么预处理能防止 SQL 注入？\n";
        echo "  传统方式：直接拼接 SQL，恶意输入会改变 SQL 结构\n";
        echo "  例如：\$name = \"' OR '1'='1\"\n";
        echo "  SELECT * FROM users WHERE name = '{\$name}'\n";
        echo "  变成：SELECT * FROM users WHERE name = '' OR '1'='1'（危险！）\n\n";
        echo "  预处理方式：SQL 语句和参数分离发送\n";
        echo "  参数永远被当作数据处理，不会被解析为 SQL 代码\n";
        echo "  即使参数中有 SQL 关键字，也只是普通字符串\n";

        echo "\n";
    }

    // ============================================================
    // 3. CRUD 操作
    // ============================================================

    /**
     * 演示 CRUD 操作
     * Create（创建）、Read（读取）、Update（更新）、Delete（删除）
     */
    public function crudOperations(): void
    {
        echo "=== CRUD 操作 ===\n";

        if (!$this->pdo) {
            $this->connectSqlite();
            $this->preparedStatements();  // 确保有数据
        }

        // ---------- Create（创建/插入）----------
        echo "1. Create（创建）- INSERT\n";

        $sql = "INSERT INTO users (name, email, age) VALUES (:name, :email, :age)";
        $stmt = $this->pdo->prepare($sql);
        $stmt->execute([
            ':name'  => '赵六',
            ':email' => 'zhaoliu@example.com',
            ':age'   => 22
        ]);

        // lastInsertId() 获取最后插入的自增 ID
        $lastId = $this->pdo->lastInsertId();
        echo "  插入成功，ID: {$lastId}\n";
        echo "  受影响行数：" . $stmt->rowCount() . "\n";

        // ---------- Read（读取/查询）----------
        echo "\n2. Read（读取）- SELECT\n";

        // 查询单条记录
        echo "  查询单条记录（WHERE id = 1）：\n";
        $sql = "SELECT * FROM users WHERE id = :id";
        $stmt = $this->pdo->prepare($sql);
        $stmt->execute([':id' => 1]);
        $user = $stmt->fetch();  // fetch() 获取一行
        echo "    " . json_encode($user, JSON_UNESCAPED_UNICODE) . "\n";

        // 查询多条记录
        echo "  查询所有记录：\n";
        $sql = "SELECT * FROM users";
        $stmt = $this->pdo->query($sql);  // query() 用于执行没有参数的查询
        $users = $stmt->fetchAll();       // fetchAll() 获取所有行
        foreach ($users as $u) {
            echo "    [{$u['id']}] {$u['name']} - {$u['email']} ({$u['age']}岁)\n";
        }
        echo "  共 " . count($users) . " 条记录\n";

        // ---------- Update（更新）----------
        echo "\n3. Update（更新）- UPDATE\n";

        $sql = "UPDATE users SET age = :age WHERE id = :id";
        $stmt = $this->pdo->prepare($sql);
        $stmt->execute([
            ':age' => 26,
            ':id'  => 1
        ]);
        echo "  更新 ID=1 的用户年龄为 26\n";
        echo "  受影响行数：" . $stmt->rowCount() . "\n";

        // ---------- Delete（删除）----------
        echo "\n4. Delete（删除）- DELETE\n";

        $sql = "DELETE FROM users WHERE id = :id";
        $stmt = $this->pdo->prepare($sql);
        $stmt->execute([':id' => 2]);
        echo "  删除 ID=2 的用户\n";
        echo "  受影响行数：" . $stmt->rowCount() . "\n";

        echo "\n";
    }

    // ============================================================
    // 4. 事务处理
    // ============================================================

    /**
     * 演示事务处理
     * 
     * 事务的四大特性（ACID）：
     * - 原子性（Atomicity）：要么全部成功，要么全部失败
     * - 一致性（Consistency）：事务前后数据保持一致
     * - 隔离性（Isolation）：多个事务之间互不干扰
     * - 持久性（Durability）：事务提交后数据永久保存
     */
    public function transactions(): void
    {
        echo "=== 事务处理 ===\n";

        if (!$this->pdo) {
            $this->connectSqlite();
        }

        echo "事务操作步骤：\n";
        echo "  1. beginTransaction() - 开启事务\n";
        echo "  2. 执行多条 SQL 语句\n";
        echo "  3. commit() - 提交事务（全部成功）\n";
        echo "  4. rollBack() - 回滚事务（出错时撤销所有操作）\n\n";

        try {
            // 开启事务
            $this->pdo->beginTransaction();
            echo "beginTransaction() - 事务已开启\n";

            // 操作1：插入新用户
            $sql1 = "INSERT INTO users (name, email, age) VALUES (:name, :email, :age)";
            $stmt1 = $this->pdo->prepare($sql1);
            $stmt1->execute([':name' => '事务用户1', ':email' => 'tx1@example.com', ':age' => 20]);
            echo "  操作1：插入事务用户1 成功\n";

            // 操作2：插入另一个用户
            $sql2 = "INSERT INTO users (name, email, age) VALUES (:name, :email, :age)";
            $stmt2 = $this->pdo->prepare($sql2);
            $stmt2->execute([':name' => '事务用户2', ':email' => 'tx2@example.com', ':age' => 22]);
            echo "  操作2：插入事务用户2 成功\n";

            // 提交事务
            $this->pdo->commit();
            echo "commit() - 事务已提交，两个操作都生效了\n";

        } catch (PDOException $e) {
            // 出错时回滚
            if ($this->pdo->inTransaction()) {
                $this->pdo->rollBack();
            }
            echo "rollBack() - 事务回滚，所有操作都撤销了\n";
            echo "错误信息：" . $e->getMessage() . "\n";
        }

        echo "\n事务使用场景：\n";
        echo "  - 银行转账（扣款和收款必须同时成功或同时失败）\n";
        echo "  - 订单创建（订单表和库存表必须一致）\n";
        echo "  - 批量数据操作（保证数据完整性）\n";

        echo "\n";
    }

    // ============================================================
    // 5. 错误处理模式
    // ============================================================

    /**
     * 演示 PDO 的三种错误处理模式
     */
    public function errorModes(): void
    {
        echo "=== PDO 错误处理模式 ===\n";

        echo "三种错误模式：\n\n";

        echo "1. PDO::ERRMODE_SILENT（静默模式 - 默认）\n";
        echo "   - 不报错，只设置错误代码\n";
        echo "   - 需要手动检查 errorCode() 和 errorInfo()\n";
        echo "   - 不推荐使用，容易忽略错误\n\n";

        echo "2. PDO::ERRMODE_WARNING（警告模式）\n";
        echo "   - 发出 PHP 警告（E_WARNING）\n";
        echo "   - 脚本继续执行\n";
        echo "   - 适合调试，不适合生产环境\n\n";

        echo "3. PDO::ERRMODE_EXCEPTION（异常模式 - 推荐）\n";
        echo "   - 抛出 PDOException 异常\n";
        echo "   - 可以用 try-catch 捕获和处理\n";
        echo "   - 错误信息详细，便于调试和日志记录\n";
        echo "   - 生产环境推荐使用\n\n";

        echo "设置方式：\n";
        echo "  // 构造时设置\n";
        echo "  \$pdo = new PDO(\$dsn, \$user, \$pass, [\n";
        echo "      PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION\n";
        echo "  ]);\n\n";
        echo "  // 或者连接后设置\n";
        echo "  \$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);\n";

        echo "\n";
    }

    // ============================================================
    // 6. fetch / fetchAll / fetchColumn
    // ============================================================

    /**
     * 演示各种数据提取方式
     */
    public function fetchMethods(): void
    {
        echo "=== 数据提取方式 ===\n";

        if (!$this->pdo) {
            $this->connectSqlite();
            $this->preparedStatements();
        }

        $stmt = $this->pdo->query("SELECT * FROM users LIMIT 3");

        // ---------- fetch() - 获取一行 ----------
        echo "1. fetch() - 获取一行数据：\n";

        // FETCH_ASSOC：关联数组
        $row = $stmt->fetch(PDO::FETCH_ASSOC);
        echo "   FETCH_ASSOC: " . json_encode($row, JSON_UNESCAPED_UNICODE) . "\n";

        // FETCH_NUM：索引数组
        $row = $stmt->fetch(PDO::FETCH_NUM);
        echo "   FETCH_NUM: [" . implode(', ', $row) . "]\n";

        // FETCH_BOTH：两者都有（默认）
        $row = $stmt->fetch(PDO::FETCH_BOTH);
        echo "   FETCH_BOTH: 既有字符串键也有数字键\n";

        // ---------- fetchAll() - 获取所有行 ----------
        echo "\n2. fetchAll() - 获取所有行：\n";
        $stmt = $this->pdo->query("SELECT id, name FROM users LIMIT 3");
        $all = $stmt->fetchAll(PDO::FETCH_ASSOC);
        foreach ($all as $row) {
            echo "   [{$row['id']}] {$row['name']}\n";
        }

        // FETCH_COLUMN：只获取某一列
        $stmt = $this->pdo->query("SELECT name FROM users LIMIT 3");
        $names = $stmt->fetchAll(PDO::FETCH_COLUMN, 0);
        echo "   FETCH_COLUMN (name列): [" . implode(', ', $names) . "]\n";

        // FETCH_KEY_PAIR：第一列作为键，第二列作为值
        $stmt = $this->pdo->query("SELECT id, name FROM users LIMIT 3");
        $pairs = $stmt->fetchAll(PDO::FETCH_KEY_PAIR);
        echo "   FETCH_KEY_PAIR (id=>name): ";
        foreach ($pairs as $id => $name) {
            echo "{$id}:{$name} ";
        }
        echo "\n";

        // ---------- fetchColumn() - 获取下一行的某一列 ----------
        echo "\n3. fetchColumn() - 获取下一行的指定列：\n";
        $stmt = $this->pdo->query("SELECT COUNT(*) FROM users");
        $count = $stmt->fetchColumn();  // 默认第 0 列
        echo "   用户总数：{$count}\n";

        // ---------- fetchObject() - 获取为对象 ----------
        echo "\n4. fetchObject() - 获取为对象：\n";
        $stmt = $this->pdo->query("SELECT * FROM users LIMIT 1");
        $obj = $stmt->fetchObject();
        echo "   对象属性: id={$obj->id}, name={$obj->name}, email={$obj->email}\n";

        echo "\n";
    }

    // ============================================================
    // 7. MySQLi 扩展对比
    // ============================================================

    /**
     * PDO vs MySQLi 对比
     */
    public function mysqliComparison(): void
    {
        echo "=== PDO vs MySQLi ===\n";

        echo "PDO（PHP Data Objects）：\n";
        echo "  - 支持多种数据库（MySQL, PostgreSQL, SQLite, Oracle 等）\n";
        echo "  - 面向对象 API\n";
        echo "  - 预处理语句，防止 SQL 注入\n";
        echo "  - 事务支持\n";
        echo "  - 错误处理灵活（三种模式）\n";
        echo "  - 推荐使用，更通用\n\n";

        echo "MySQLi（MySQL Improved）：\n";
        echo "  - 只支持 MySQL 数据库\n";
        echo "  - 提供面向对象和过程化两种 API\n";
        echo "  - 也支持预处理语句\n";
        echo "  - 也支持事务\n";
        echo "  - 功能针对 MySQL 做了优化\n\n";

        echo "对比表：\n";
        echo "  +----------------+---------+--------+\n";
        echo "  | 特性           | PDO     | MySQLi |\n";
        echo "  +----------------+---------+--------+\n";
        echo "  | 数据库支持     | 12种+   | 仅MySQL|\n";
        echo "  | 面向对象       | 是      | 是     |\n";
        echo "  | 过程化         | 否      | 是     |\n";
        echo "  | 预处理语句     | 是      | 是     |\n";
        echo "  | 事务支持       | 是      | 是     |\n";
        echo "  | 命名占位符     | 是      | 否     |\n";
        echo "  +----------------+---------+--------+\n\n";

        echo "推荐：\n";
        echo "  - 需要支持多种数据库 → PDO\n";
        echo "  - 只使用 MySQL 且需要特殊功能 → MySQLi\n";
        echo "  - 一般项目推荐使用 PDO，更通用、更现代\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有数据库操作演示
     * 使用 SQLite 内存数据库进行演示，无需实际 MySQL 环境
     */
    public function runAll(): void
    {
        $this->errorModes();
        $this->preparedStatements();
        $this->crudOperations();
        $this->fetchMethods();
        $this->transactions();
        $this->mysqliComparison();
    }

    /**
     * 析构函数 - 关闭数据库连接
     */
    public function __destruct()
    {
        // PDO 连接会在对象销毁时自动关闭
        // 也可以手动设置为 null 来关闭
        $this->pdo = null;
    }
}
