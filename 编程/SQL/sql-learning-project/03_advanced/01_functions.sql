-- =====================================================================
-- 01_functions.sql - MySQL 函数大全（字符串/数值/日期/聚合/流程控制/窗口函数）
-- 适用数据库：MySQL 8.0+
-- =====================================================================

-- ---------------------------------------------------------------------
-- 一、字符串函数
-- ---------------------------------------------------------------------
-- 用于处理字符串类型的数据

-- 演示数据
SET @str = '  Hello, World!  ';
SET @name = '张三';

-- ===== 1. CONCAT 字符串拼接 =====
-- CONCAT(str1, str2, ...) 将多个字符串拼接在一起
-- 如果任一参数为 NULL，结果为 NULL
SELECT CONCAT('姓名：', @name, '，年龄：', 25) AS 拼接结果;
-- 结果：姓名：张三，年龄：25

-- CONCAT_WS(separator, str1, str2, ...) 带分隔符的拼接
-- WS = With Separator
-- 第一个参数是分隔符，会自动跳过 NULL 值
SELECT CONCAT_WS('-', '2024', '01', '15') AS 日期拼接;
-- 结果：2024-01-15

-- ===== 2. SUBSTRING 截取子串 =====
-- SUBSTRING(str, pos, length) 或 SUBSTRING(str FROM pos FOR length)
-- pos 从 1 开始；如果 pos 为负数，则从右边开始数
SELECT
    SUBSTRING('Hello World', 1, 5)   AS 截取前5个,   -- Hello
    SUBSTRING('Hello World', 7)      AS 从第7个开始,  -- World
    SUBSTRING('Hello World', -5)     AS 倒数5个;     -- World

-- LEFT(str, n)  从左边截取 n 个字符
-- RIGHT(str, n) 从右边截取 n 个字符
SELECT LEFT('Hello', 2) AS 左2, RIGHT('Hello', 2) AS 右2;

-- ===== 3. LENGTH / CHAR_LENGTH 字符串长度 =====
-- LENGTH()       返回字节长度（UTF-8下一个中文占3字节）
-- CHAR_LENGTH()  返回字符长度（不管是中文还是英文，都算一个字符）
SELECT
    LENGTH('Hello')       AS 英文字节数,   -- 5
    LENGTH('张三')         AS 中文字节数,   -- 6
    CHAR_LENGTH('Hello')  AS 英文字符数,   -- 5
    CHAR_LENGTH('张三')    AS 中文字符数;   -- 2

-- ===== 4. UPPER / LOWER 大小写转换 =====
SELECT
    UPPER('hello') AS 转大写,   -- HELLO
    LOWER('WORLD') AS 转小写;   -- world

-- ===== 5. TRIM 去除空格 =====
-- TRIM(str)          去除两端空格
-- LTRIM(str)         去除左端空格
-- RTRIM(str)         去除右端空格
-- TRIM(BOTH/LEADING/TRAILING 字符 FROM str)  去除指定字符
SELECT
    TRIM(@str)             AS 去两端空格,     -- 'Hello, World!'
    LTRIM(@str)            AS 去左端空格,
    RTRIM(@str)            AS 去右端空格,
    TRIM(BOTH '!' FROM '!Hello!') AS 去两端感叹号;  -- Hello

-- ===== 6. REPLACE 替换字符串 =====
-- REPLACE(str, from_str, to_str)  将 str 中的 from_str 替换为 to_str
SELECT REPLACE('Hello World', 'World', 'MySQL') AS 替换结果;
-- 结果：Hello MySQL

-- ===== 7. REVERSE 反转字符串 =====
SELECT REVERSE('ABCDE') AS 反转;  -- EDCBA

-- ===== 8. FIND_IN_SET 查找集合中的位置 =====
-- FIND_IN_SET(str, strlist)  返回 str 在逗号分隔的字符串列表中的位置（从1开始）
-- 找不到返回 0
SELECT
    FIND_IN_SET('苹果', '苹果,香蕉,橘子') AS 位置1,  -- 1
    FIND_IN_SET('香蕉', '苹果,香蕉,橘子') AS 位置2,  -- 2
    FIND_IN_SET('葡萄', '苹果,香蕉,橘子') AS 位置3;  -- 0
-- 常用于 ENUM 类型或逗号分隔的标签字段

-- ===== 9. INSTR / LOCATE 查找子串位置 =====
-- INSTR(str, substr) 返回 substr 在 str 中第一次出现的位置
-- LOCATE(substr, str) 同上，参数顺序不同
SELECT
    INSTR('Hello World', 'World') AS 位置1,  -- 7
    LOCATE('lo', 'Hello World')   AS 位置2;  -- 4

-- ===== 10. LPAD / RPAD 填充字符串 =====
-- LPAD(str, len, padstr)  左边填充 padstr，使总长度为 len
-- RPAD(str, len, padstr)  右边填充
SELECT
    LPAD('123', 6, '0') AS 左补零,  -- 000123
    RPAD('abc', 6, '*') AS 右补星;  -- abc***

-- ===== 11. ELT 按索引取值 =====
-- ELT(n, str1, str2, ...) 返回第 n 个字符串
SELECT
    ELT(1, 'A', 'B', 'C') AS 第1个,  -- A
    ELT(3, 'A', 'B', 'C') AS 第3个;  -- C

-- ===== 12. FORMAT 数字格式化 =====
-- FORMAT(number, decimals) 将数字格式化为千分位字符串
SELECT FORMAT(1234567.89, 2) AS 格式化数字;  -- 1,234,567.89


-- ---------------------------------------------------------------------
-- 二、数值函数
-- ---------------------------------------------------------------------

-- ===== 1. ABS 绝对值 =====
SELECT ABS(-10) AS 绝对值;  -- 10

-- ===== 2. ROUND 四舍五入 =====
-- ROUND(x, d)  x 四舍五入保留 d 位小数
-- d 可以为负数，表示整数部分的位数
SELECT
    ROUND(3.14159, 2)  AS 保留2位,   -- 3.14
    ROUND(3.14159, 0)  AS 取整,      -- 3
    ROUND(1234.56, -2) AS 取到百位;  -- 1200

-- ===== 3. CEIL / CEILING 向上取整 =====
-- 取大于等于 x 的最小整数
SELECT
    CEIL(3.1)   AS 向上取整1,  -- 4
    CEIL(-3.1)  AS 向上取整2;  -- -3

-- ===== 4. FLOOR 向下取整 =====
-- 取小于等于 x 的最大整数
SELECT
    FLOOR(3.9)   AS 向下取整1,  -- 3
    FLOOR(-3.9)  AS 向下取整2;  -- -4

-- ===== 5. MOD 取模（求余）=====
-- MOD(x, y)  或 x % y  求 x 除以 y 的余数
SELECT
    MOD(10, 3) AS 取余1,  -- 1
    10 % 3     AS 取余2;  -- 1

-- ===== 6. RAND 随机数 =====
-- RAND() 返回 0（包含）到 1（不包含）之间的随机浮点数
-- RAND(seed) 指定种子，相同种子产生相同随机序列
SELECT
    RAND()          AS 随机数,
    FLOOR(RAND() * 100) AS 0到99随机整数;

-- 随机取一条记录
-- SELECT * FROM 表名 ORDER BY RAND() LIMIT 1;
-- 注意：ORDER BY RAND() 在大表中性能很差

-- ===== 7. POWER 幂运算 =====
-- POWER(x, y)  或 POW(x, y)  计算 x 的 y 次方
SELECT
    POWER(2, 10) AS 2的10次方,  -- 1024
    POW(10, 3)   AS 10的3次方;  -- 1000

-- ===== 8. SQRT 平方根 =====
SELECT SQRT(16) AS 平方根;  -- 4

-- ===== 9. SIGN 符号函数 =====
-- 正数返回 1，负数返回 -1，0 返回 0
SELECT SIGN(10), SIGN(-5), SIGN(0);


-- ---------------------------------------------------------------------
-- 三、日期函数
-- ---------------------------------------------------------------------

SET @now = NOW();
SET @date = '2024-01-15';
SET @datetime = '2024-01-15 14:30:00';

-- ===== 1. 获取当前日期时间 =====
SELECT
    NOW()              AS 当前日期时间,   -- 2024-01-15 14:30:00
    CURDATE()          AS 当前日期,        -- 2024-01-15
    CURTIME()          AS 当前时间,        -- 14:30:00
    SYSDATE()          AS 系统时间;        -- 同 NOW()，但执行时才获取（NOW()是语句开始时获取）

-- ===== 2. DATE_ADD / DATE_SUB 日期加减 =====
-- DATE_ADD(date, INTERVAL value unit)
-- unit 可选：YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, WEEK 等
SELECT
    DATE_ADD(@date, INTERVAL 10 DAY)      AS 加10天,
    DATE_ADD(@date, INTERVAL 1 MONTH)     AS 加1个月,
    DATE_SUB(@date, INTERVAL 1 YEAR)      AS 减1年,
    DATE_ADD(@datetime, INTERVAL 2 HOUR)  AS 加2小时;

-- 简写形式：
-- SELECT @date + INTERVAL 10 DAY;
-- SELECT @date - INTERVAL 1 MONTH;

-- ===== 3. DATEDIFF / TIMESTAMPDIFF 日期差 =====
-- DATEDIFF(date1, date2)  返回 date1 - date2 的天数
SELECT
    DATEDIFF('2024-02-01', '2024-01-15') AS 相差天数;  -- 17

-- TIMESTAMPDIFF(unit, start, end)  返回 end - start，单位由 unit 指定
SELECT
    TIMESTAMPDIFF(DAY,   '2024-01-01', '2024-01-15') AS 差_天,    -- 14
    TIMESTAMPDIFF(MONTH, '2024-01-01', '2024-03-15') AS 差_月,    -- 2
    TIMESTAMPDIFF(YEAR,  '2020-01-01', '2024-01-15') AS 差_年;    -- 4

-- ===== 4. DATE_FORMAT 日期格式化 =====
-- 常用格式符：
--   %Y 四位年  %y 两位年
--   %m 两位月  %c 月份(无前导0)  %M 月份英文名
--   %d 两位日  %e 日期(无前导0)
--   %H 24小时  %h 12小时
--   %i 分钟    %s 秒
--   %W 星期名  %w 星期数字(0=周日)
SELECT
    DATE_FORMAT(@datetime, '%Y-%m-%d')           AS 年月日,
    DATE_FORMAT(@datetime, '%Y年%m月%d日')        AS 中文日期,
    DATE_FORMAT(@datetime, '%Y-%m-%d %H:%i:%s')  AS 完整时间,
    DATE_FORMAT(@datetime, '%W')                  AS 星期几;

-- ===== 5. EXTRACT 提取日期部分 =====
-- EXTRACT(unit FROM date) 提取日期的指定部分
SELECT
    EXTRACT(YEAR FROM @datetime)    AS 年,
    EXTRACT(MONTH FROM @datetime)   AS 月,
    EXTRACT(DAY FROM @datetime)     AS 日,
    EXTRACT(HOUR FROM @datetime)    AS 时;

-- 也可以用单独的函数：
SELECT
    YEAR(@datetime)   AS 年,
    MONTH(@datetime)  AS 月,
    DAY(@datetime)    AS 日,
    HOUR(@datetime)   AS 时,
    MINUTE(@datetime) AS 分,
    SECOND(@datetime) AS 秒;

-- ===== 6. DAYNAME / MONTHNAME 名称 =====
SELECT
    DAYNAME(@date)   AS 星期名,     -- Monday
    MONTHNAME(@date) AS 月份名;     -- January

-- ===== 7. LAST_DAY 当月最后一天 =====
SELECT LAST_DAY(@date) AS 当月最后一天;  -- 2024-01-31

-- ===== 8. DATE 日期转换 =====
-- DATE(expr) 提取日期部分
SELECT
    DATE(@datetime)   AS 提取日期,   -- 2024-01-15
    TIME(@datetime)   AS 提取时间;   -- 14:30:00

-- ===== 9. STR_TO_DATE 字符串转日期 =====
-- STR_TO_DATE(str, format) 按指定格式将字符串转为日期
SELECT STR_TO_DATE('15-01-2024', '%d-%m-%Y') AS 字符串转日期;

-- ===== 10. UNIX_TIMESTAMP / FROM_UNIXTIME 时间戳 =====
SELECT
    UNIX_TIMESTAMP(@datetime)        AS 时间戳秒数,
    FROM_UNIXTIME(1705300200)        AS 时间戳转日期;


-- ---------------------------------------------------------------------
-- 四、聚合函数进阶
-- ---------------------------------------------------------------------
-- 基础聚合函数在 01_query_advanced.sql 中已讲过（COUNT/SUM/AVG/MIN/MAX/GROUP_CONCAT）

-- 演示数据
DROP TABLE IF EXISTS sales_func;
CREATE TABLE sales_func (
    id        INT PRIMARY KEY AUTO_INCREMENT,
    product   VARCHAR(50),
    category  VARCHAR(50),
    amount    DECIMAL(10,2),
    sale_date DATE
);

INSERT INTO sales_func (product, category, amount, sale_date) VALUES
    ('商品A', '电子产品', 1000, '2024-01-05'),
    ('商品B', '服装', 500, '2024-01-10'),
    ('商品C', '电子产品', 2000, '2024-01-15'),
    ('商品A', '电子产品', 1000, '2024-02-05'),
    ('商品B', '服装', 600, '2024-02-10'),
    ('商品D', '食品', 200, '2024-02-20'),
    ('商品C', '电子产品', 1500, '2024-03-01'),
    ('商品E', '服装', 800, '2024-03-10');

-- ===== 1. COUNT(DISTINCT 列) 去重计数 =====
SELECT
    COUNT(DISTINCT category) AS 类别数,
    COUNT(DISTINCT product)  AS 商品数
FROM sales_func;

-- ===== 2. SUM(DISTINCT 列) 去重求和 =====
-- 注意：实际业务中很少用，因为相同值加一次还是加多次通常需要都算
SELECT SUM(DISTINCT amount) AS 去重求和 FROM sales_func;

-- ===== 3. GROUP_CONCAT 分组拼接（进阶）=====
-- 语法：GROUP_CONCAT([DISTINCT] 列 [ORDER BY 列] [SEPARATOR '分隔符'])
SELECT
    category,
    GROUP_CONCAT(DISTINCT product ORDER BY product SEPARATOR '、') AS 商品列表
FROM sales_func
GROUP BY category;

-- ===== 4. JSON_ARRAYAGG / JSON_OBJECTAGG（MySQL 5.7+）=====
-- 将分组结果聚合为 JSON 数组/对象
SELECT
    category,
    JSON_ARRAYAGG(product) AS 商品数组
FROM sales_func
GROUP BY category;


-- ---------------------------------------------------------------------
-- 五、流程控制函数
-- ---------------------------------------------------------------------

-- ===== 1. IF 函数 =====
-- IF(condition, true_value, false_value)
-- 类似三元运算符：condition ? true_value : false_value
SELECT
    emp_name,
    salary,
    IF(salary >= 10000, '高薪', '普通') AS 工资等级
FROM emp_sub;

-- ===== 2. IFNULL 函数 =====
-- IFNULL(expr, value)  如果 expr 为 NULL，返回 value；否则返回 expr
-- 常用于将 NULL 替换为默认值
SELECT
    emp_name,
    IFNULL(dept, '未分配') AS 部门  -- 如果部门为 NULL，显示"未分配"
FROM emp_sub;

-- ===== 3. NULLIF 函数 =====
-- NULLIF(expr1, expr2)  如果 expr1 = expr2，返回 NULL；否则返回 expr1
SELECT
    NULLIF(10, 10) AS 相等返回NULL,   -- NULL
    NULLIF(10, 20) AS 不等返回前者;   -- 10

-- 应用场景：防止除零错误
-- SELECT amount / NULLIF(quantity, 0) FROM ...;  -- quantity 为 0 时返回 NULL，不报错

-- ===== 4. COALESCE 函数 =====
-- COALESCE(val1, val2, val3, ...)
-- 返回参数列表中第一个非 NULL 的值
-- 相当于 IFNULL 的多参数版本
SELECT
    COALESCE(NULL, NULL, '第一个非空', '第二个') AS 结果;  -- 第一个非空

-- 应用场景：优先级取值
-- SELECT COALESCE(手机号, 邮箱, 座机) AS 联系方式 FROM 用户表;

-- ===== 5. CASE WHEN 表达式 =====
-- 在 01_query_advanced.sql 中已详细讲解
-- 简单 CASE 和搜索 CASE 两种写法
SELECT
    product,
    amount,
    CASE
        WHEN amount >= 1500 THEN '高'
        WHEN amount >= 800  THEN '中'
        ELSE '低'
    END AS 价格等级
FROM sales_func;


-- ---------------------------------------------------------------------
-- 六、窗口函数（Window Functions，MySQL 8.0+）
-- ---------------------------------------------------------------------
-- 窗口函数对一组行（窗口）进行计算，但为每一行返回结果（不像 GROUP BY 合并行）
-- 语法：函数名() OVER (
--           [PARTITION BY 列名, ...]    -- 分区，类似 GROUP BY
--           [ORDER BY 列名 [ASC|DESC], ...]  -- 排序
--           [窗口框架定义]
--       )

-- 演示数据
DROP TABLE IF EXISTS emp_window;
CREATE TABLE emp_window (
    emp_id   INT PRIMARY KEY AUTO_INCREMENT,
    emp_name VARCHAR(50),
    dept     VARCHAR(50),
    salary   DECIMAL(10,2),
    hire_date DATE
);

INSERT INTO emp_window (emp_name, dept, salary, hire_date) VALUES
    ('张三', '技术部', 12000, '2019-03-15'),
    ('李四', '市场部', 8000,  '2020-06-20'),
    ('王五', '技术部', 18000, '2017-01-10'),
    ('赵六', '人事部', 7000,  '2021-09-01'),
    ('孙七', '市场部', 15000, '2018-05-22'),
    ('周八', '技术部', 10000, '2020-11-30'),
    ('吴九', '人事部', 12000, '2019-07-15'),
    ('郑十', '财务部', 9000,  '2021-02-14'),
    ('冯十一', '财务部', 16000, '2018-08-08'),
    ('陈十二', '技术部', 14000, '2019-12-25');

-- ===== 1. 排名函数：ROW_NUMBER / RANK / DENSE_RANK =====

-- ROW_NUMBER()：行号，相同值也有不同的行号（1,2,3,4,...）
-- RANK()：排名，相同值并列，跳过后续名次（1,1,3,4,...）
-- DENSE_RANK()：密集排名，相同值并列，不跳过名次（1,1,2,3,...）

SELECT
    emp_name,
    dept,
    salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC)  AS 行号,
    RANK()       OVER (ORDER BY salary DESC)  AS 排名,
    DENSE_RANK() OVER (ORDER BY salary DESC)  AS 密集排名
FROM emp_window
ORDER BY salary DESC;

-- 分区排名：每个部门内部排名
SELECT
    emp_name,
    dept,
    salary,
    ROW_NUMBER() OVER (PARTITION BY dept ORDER BY salary DESC) AS 部门内序号,
    RANK()       OVER (PARTITION BY dept ORDER BY salary DESC) AS 部门内排名
FROM emp_window
ORDER BY dept, salary DESC;

-- ===== 2. 偏移函数：LAG / LEAD =====
-- LAG(列, offset, default)  获取当前行前面第 offset 行的值
-- LEAD(列, offset, default) 获取当前行后面第 offset 行的值

SELECT
    emp_name,
    dept,
    salary,
    hire_date,
    LAG(salary, 1, 0) OVER (PARTITION BY dept ORDER BY hire_date) AS 前一位入职工资,
    LEAD(salary, 1, 0) OVER (PARTITION BY dept ORDER BY hire_date) AS 后一位入职工资
FROM emp_window
ORDER BY dept, hire_date;

-- ===== 3. 聚合窗口函数：SUM / AVG / COUNT / MIN / MAX OVER =====
-- 在窗口内计算聚合值，每行都返回结果

SELECT
    emp_name,
    dept,
    salary,
    SUM(salary) OVER (PARTITION BY dept) AS 部门工资总额,
    AVG(salary) OVER (PARTITION BY dept) AS 部门平均工资,
    COUNT(*)   OVER (PARTITION BY dept) AS 部门人数
FROM emp_window
ORDER BY dept, salary DESC;

-- 累加求和（累计到当前行）
SELECT
    emp_name,
    dept,
    salary,
    hire_date,
    SUM(salary) OVER (PARTITION BY dept ORDER BY hire_date) AS 部门累计工资
FROM emp_window
ORDER BY dept, hire_date;

-- ===== 4. FIRST_VALUE / LAST_VALUE =====
-- FIRST_VALUE(列)  窗口内第一行的值
-- LAST_VALUE(列)   窗口内最后一行的值

SELECT
    emp_name,
    dept,
    salary,
    FIRST_VALUE(emp_name) OVER (PARTITION BY dept ORDER BY salary DESC) AS 部门最高工资员工,
    LAST_VALUE(emp_name)  OVER (PARTITION BY dept ORDER BY salary DESC
        RANGE BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS 部门最低工资员工
FROM emp_window
ORDER BY dept, salary DESC;

-- ===== 5. NTILE 分桶 =====
-- NTILE(n) 将数据分成 n 组，每组尽量平均
SELECT
    emp_name,
    salary,
    NTILE(4) OVER (ORDER BY salary DESC) AS 四分位
FROM emp_window;
-- 四分位：前 25% 为 1，25%-50% 为 2，以此类推

-- ===== 6. 窗口框架（Window Frame）=====
-- 定义窗口的范围，语法：
--   ROWS/RANGE BETWEEN 边界1 AND 边界2
-- 边界：
--   UNBOUNDED PRECEDING   从分区第一行开始
--   n PRECEDING           当前行之前第 n 行
--   CURRENT ROW           当前行
--   n FOLLOWING           当前行之后第 n 行
--   UNBOUNDED FOLLOWING   到分区最后一行
--
-- 默认框架（有 ORDER BY 时）：
--   RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
-- 默认框架（没有 ORDER BY 时）：
--   RANGE BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING

-- 示例：移动平均（每行取前一行、当前行、后一行的平均值）
SELECT
    emp_name,
    dept,
    salary,
    hire_date,
    AVG(salary) OVER (
        PARTITION BY dept
        ORDER BY hire_date
        ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING
    ) AS 三人移动平均
FROM emp_window
ORDER BY dept, hire_date;


-- =====================================================================
-- 练习题：
-- 1. 用字符串函数把 "hello world" 变成 "Hello World"（每个单词首字母大写）
-- 2. 计算 2024-01-15 到 2024-12-31 相差多少天
-- 3. 查询每个员工的工资，以及其在部门内的排名（用 RANK）
-- 4. 查询每个部门工资最高的 2 名员工（用 ROW_NUMBER + 子查询）
-- 5. 用 LAG 函数计算每个员工与同部门前一位入职员工的工资差
-- 6. 用窗口函数计算每个员工工资占部门总工资的百分比
-- =====================================================================
