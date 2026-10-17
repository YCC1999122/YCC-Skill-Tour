-- =====================================================================
-- 01_query_advanced.sql - 查询进阶：复杂条件、聚合、分组、CASE WHEN
-- 适用数据库：MySQL 8.0+
-- =====================================================================

-- 准备演示数据：员工表 + 订单表
DROP TABLE IF EXISTS emp_adv;
CREATE TABLE emp_adv (
    emp_id      INT PRIMARY KEY AUTO_INCREMENT COMMENT '员工ID',
    emp_name    VARCHAR(50) NOT NULL COMMENT '员工姓名',
    dept        VARCHAR(50) DEFAULT '未分配' COMMENT '部门',
    salary      DECIMAL(10,2) DEFAULT 0 COMMENT '工资',
    gender      CHAR(1) DEFAULT 'M' COMMENT '性别',
    city        VARCHAR(50) COMMENT '所在城市',
    hire_date   DATE COMMENT '入职日期',
    manager_id  INT COMMENT '上级领导ID'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO emp_adv (emp_name, dept, salary, gender, city, hire_date, manager_id) VALUES
    ('张三', '技术部', 12000.00, 'M', '北京', '2019-03-15', 3),
    ('李四', '市场部', 8000.00,  'F', '上海', '2020-06-20', 5),
    ('王五', '技术部', 18000.00, 'M', '北京', '2017-01-10', NULL),  -- 技术总监，无上级
    ('赵六', '人事部', 7000.00,  'F', '广州', '2021-09-01', 7),
    ('孙七', '市场部', 15000.00, 'M', '上海', '2018-05-22', NULL),  -- 市场总监
    ('周八', '技术部', 10000.00, 'F', '深圳', '2020-11-30', 3),
    ('吴九', '人事部', 12000.00, 'M', '北京', '2019-07-15', NULL),  -- 人事总监
    ('郑十', '财务部', 9000.00,  'F', '杭州', '2021-02-14', 9),
    ('冯十一', '财务部', 16000.00, 'M', '北京', '2018-08-08', NULL), -- 财务总监
    ('陈十二', '技术部', 14000.00, 'M', '上海', '2019-12-25', 3),
    ('褚十三', '市场部', 7500.00, 'F', '广州', '2022-04-10', 5),
    ('卫十四', '技术部', 11000.00, 'M', '深圳', '2021-06-18', 3);


-- ---------------------------------------------------------------------
-- 一、WHERE 复杂条件
-- ---------------------------------------------------------------------

-- ===== 1. AND 与 OR 组合使用 =====
-- 注意：AND 的优先级高于 OR，建议用括号明确分组
-- 查询：技术部工资大于10000，或市场部工资大于8000的员工
SELECT emp_name, dept, salary
FROM emp_adv
WHERE (dept = '技术部' AND salary > 10000)
   OR (dept = '市场部' AND salary > 8000);

-- ===== 2. IN / NOT IN =====
-- IN 用于匹配值列表中的任意一个
-- 查询：北京、上海、深圳的员工
SELECT emp_name, city FROM emp_adv WHERE city IN ('北京', '上海', '深圳');

-- NOT IN 排除列表中的值
-- 查询：不在北京的员工
SELECT emp_name, city FROM emp_adv WHERE city NOT IN ('北京');

-- 注意：NOT IN 如果列表中包含 NULL，结果会为空（因为 NULL 比较结果为 UNKNOWN）

-- ===== 3. BETWEEN ... AND ... =====
-- 查询工资在 8000 到 15000 之间的员工（包含边界）
SELECT emp_name, salary FROM emp_adv WHERE salary BETWEEN 8000 AND 15000;

-- 日期范围：查询 2019 年入职的员工
SELECT emp_name, hire_date FROM emp_adv
WHERE hire_date BETWEEN '2019-01-01' AND '2019-12-31';

-- ===== 4. LIKE 模糊查询 =====
-- % 匹配任意长度（包括0）的任意字符
-- _ 匹配任意单个字符

-- 姓"张"的员工
SELECT emp_name FROM emp_adv WHERE emp_name LIKE '张%';

-- 名字包含"十"的员工
SELECT emp_name FROM emp_adv WHERE emp_name LIKE '%十%';

-- 名字第三个字是"一"的员工（前两个任意，第三个是"一"，后面任意）
SELECT emp_name FROM emp_adv WHERE emp_name LIKE '__一%';

-- 转义：如果要匹配 % 或 _ 本身，需要用转义符
-- SELECT * FROM table WHERE col LIKE '%\%%' ESCAPE '\';  -- 匹配包含 % 的字符串

-- ===== 5. IS NULL / IS NOT NULL =====
-- NULL 表示"未知"或"不存在"，不能用 = 或 != 比较
-- 查询没有上级的员工（总监级别）
SELECT emp_name, manager_id FROM emp_adv WHERE manager_id IS NULL;

-- 查询有上级的员工
SELECT emp_name, manager_id FROM emp_adv WHERE manager_id IS NOT NULL;

-- ===== 6. EXISTS 存在性判断 =====
-- EXISTS (子查询)：子查询返回至少一行则为 TRUE
-- 常用于判断"是否存在满足条件的记录"
-- 查询：是否有工资大于 15000 的员工
SELECT emp_name, salary FROM emp_adv e
WHERE EXISTS (
    SELECT 1 FROM emp_adv WHERE salary > 15000 AND emp_id = e.emp_id
);
-- 注：EXISTS 只关心是否有行，不关心返回什么内容，所以通常写 SELECT 1 或 SELECT *


-- ---------------------------------------------------------------------
-- 二、别名 AS
-- ---------------------------------------------------------------------
-- 别名可以给列或表起一个简短的名字，提高可读性，简化书写
-- AS 关键字可以省略

-- ===== 1. 列别名 =====
-- 给查询结果的列起别名
SELECT
    emp_name   AS '员工姓名',   -- AS 关键字 + 别名
    salary     工资,            -- 省略 AS，直接写别名
    salary * 12 AS 年薪         -- 计算列也可以起别名
FROM emp_adv;

-- 别名可以包含空格或特殊字符，需要用引号括起来
-- 别名在 ORDER BY 中可以使用，但在 WHERE 中不能用（因为 WHERE 在 SELECT 之前执行）

-- ===== 2. 表别名 =====
-- 给表起别名，在多表查询时特别有用
SELECT e.emp_name, e.salary
FROM emp_adv e  -- e 就是 emp_adv 的别名
WHERE e.dept = '技术部';


-- ---------------------------------------------------------------------
-- 三、DISTINCT 去重
-- ---------------------------------------------------------------------
-- DISTINCT 用于去除查询结果中的重复行
-- 语法：SELECT DISTINCT 列1, 列2, ... FROM 表名;

-- 查询有哪些部门（去重）
SELECT DISTINCT dept FROM emp_adv;

-- 查询有哪些城市（去重）
SELECT DISTINCT city FROM emp_adv;

-- 多列去重：部门+城市的组合不重复
SELECT DISTINCT dept, city FROM emp_adv ORDER BY dept;

-- DISTINCT 必须放在所有列名的最前面
-- DISTINCT 对 NULL 的处理：多个 NULL 视为相同，只保留一个


-- ---------------------------------------------------------------------
-- 四、聚合函数（Aggregate Functions）
-- ---------------------------------------------------------------------
-- 聚合函数对一组行进行计算，返回一个结果
-- 常见聚合函数：COUNT / SUM / AVG / MIN / MAX / GROUP_CONCAT

-- ===== 1. COUNT 计数 =====
-- COUNT(*)        统计行数（包括 NULL 值的行）
-- COUNT(列名)     统计该列非 NULL 值的行数
-- COUNT(DISTINCT 列名)  统计该列不同值的数量

SELECT
    COUNT(*) AS 总人数,
    COUNT(manager_id) AS 有上级的人数,  -- NULL 不计入
    COUNT(DISTINCT dept) AS 部门数量
FROM emp_adv;

-- ===== 2. SUM 求和 =====
-- 计算数值列的总和，NULL 值会被忽略
SELECT
    SUM(salary) AS 工资总和,
    SUM(CASE WHEN gender = 'M' THEN salary ELSE 0 END) AS 男员工工资总和
FROM emp_adv;

-- ===== 3. AVG 平均值 =====
SELECT
    AVG(salary) AS 平均工资,
    AVG(CASE WHEN dept = '技术部' THEN salary END) AS 技术部平均工资
FROM emp_adv;

-- ===== 4. MIN / MAX 最小/最大值 =====
SELECT
    MIN(salary) AS 最低工资,
    MAX(salary) AS 最高工资,
    MIN(hire_date) AS 最早入职日期,
    MAX(hire_date) AS 最晚入职日期
FROM emp_adv;

-- ===== 5. GROUP_CONCAT 分组拼接 =====
-- MySQL 特有函数：将分组内的某个列的值拼接成一个字符串
-- 语法：GROUP_CONCAT([DISTINCT] 列名 [ORDER BY 排序列] [SEPARATOR '分隔符'])

-- 查询每个部门的员工姓名列表
SELECT dept, GROUP_CONCAT(emp_name ORDER BY emp_name SEPARATOR '、') AS 员工列表
FROM emp_adv
GROUP BY dept;
-- 默认分隔符是逗号 ,
-- 可以用 DISTINCT 去重，用 ORDER BY 排序


-- ---------------------------------------------------------------------
-- 五、GROUP BY 分组
-- ---------------------------------------------------------------------
-- GROUP BY 按照一个或多个列对数据进行分组，然后对每个组使用聚合函数
-- 语法：SELECT 分组列, 聚合函数 FROM 表名 GROUP BY 分组列;

-- ===== 1. 按单列分组 =====
-- 统计每个部门的人数和平均工资
SELECT
    dept            AS 部门,
    COUNT(*)        AS 人数,
    AVG(salary)     AS 平均工资,
    SUM(salary)     AS 工资总额
FROM emp_adv
GROUP BY dept;

-- ===== 2. 按多列分组 =====
-- 统计每个部门、每个性别的人数
SELECT
    dept    AS 部门,
    gender  AS 性别,
    COUNT(*) AS 人数
FROM emp_adv
GROUP BY dept, gender
ORDER BY dept, gender;

-- ===== 3. GROUP BY 的规则 =====
-- SELECT 中的列要么出现在 GROUP BY 中，要么被聚合函数包裹
-- 否则结果不确定（MySQL 的 ONLY_FULL_GROUP_BY 模式下会报错）

-- 错误示例（非标准）：
-- SELECT dept, emp_name, AVG(salary) FROM emp_adv GROUP BY dept;
-- 因为每个部门有多个员工，emp_name 取哪个不确定

-- ===== 4. WITH ROLLUP 汇总行 =====
-- 在 GROUP BY 结果的基础上，增加一行汇总数据
SELECT
    dept        AS 部门,
    COUNT(*)    AS 人数,
    SUM(salary) AS 工资总额
FROM emp_adv
GROUP BY dept WITH ROLLUP;
-- 最后一行 dept 为 NULL，人数是总人数，工资总额是总和


-- ---------------------------------------------------------------------
-- 六、HAVING 分组后过滤
-- ---------------------------------------------------------------------
-- HAVING 用于对 GROUP BY 分组后的结果进行过滤
-- WHERE 是分组前过滤（在 GROUP BY 之前执行）
-- HAVING 是分组后过滤（在 GROUP BY 之后执行）
-- HAVING 中可以使用聚合函数，WHERE 中不可以

-- 查询人数大于 3 人的部门
SELECT dept, COUNT(*) AS 人数
FROM emp_adv
GROUP BY dept
HAVING COUNT(*) > 3;

-- 查询平均工资大于 10000 的部门，按平均工资降序
SELECT dept, AVG(salary) AS 平均工资
FROM emp_adv
GROUP BY dept
HAVING AVG(salary) > 10000
ORDER BY 平均工资 DESC;

-- WHERE + GROUP BY + HAVING 组合使用
-- 查询：2019年及以后入职的员工，按部门分组，筛选出平均工资大于9000的部门
SELECT dept, AVG(salary) AS 平均工资, COUNT(*) AS 人数
FROM emp_adv
WHERE hire_date >= '2019-01-01'   -- 先过滤：只看2019年后入职的
GROUP BY dept                      -- 再分组
HAVING AVG(salary) > 9000         -- 再过滤分组结果
ORDER BY 平均工资 DESC;            -- 最后排序


-- ---------------------------------------------------------------------
-- 七、ORDER BY 多字段排序
-- ---------------------------------------------------------------------

-- 先按部门升序，同部门按工资降序，同工资按入职日期升序
SELECT emp_name, dept, salary, hire_date
FROM emp_adv
ORDER BY
    dept ASC,
    salary DESC,
    hire_date ASC;

-- 可以用列的序号排序（不推荐，可读性差，列顺序变了会出错）
-- SELECT emp_name, dept, salary FROM emp_adv ORDER BY 2, 3 DESC;

-- ORDER BY 可以配合 FIELD() 函数自定义排序顺序
-- 按指定的部门顺序排序
SELECT emp_name, dept, salary
FROM emp_adv
ORDER BY FIELD(dept, '技术部', '市场部', '财务部', '人事部'), salary DESC;
-- FIELD(dept, '技术部', '市场部', ...) 返回 dept 在列表中的位置，不在列表中的排最前面或最后面


-- ---------------------------------------------------------------------
-- 八、LIMIT 分页
-- ---------------------------------------------------------------------

-- 语法1：LIMIT count           返回前 count 条
-- 语法2：LIMIT offset, count   跳过 offset 条，返回 count 条
-- 语法3：LIMIT count OFFSET offset  （同上，不同写法）

-- 工资最高的5名员工
SELECT emp_name, salary FROM emp_adv ORDER BY salary DESC LIMIT 5;

-- 分页：每页4条，第2页
SELECT emp_name, dept, salary
FROM emp_adv
ORDER BY emp_id
LIMIT 4, 4;  -- 跳过前4条，取4条

-- 注意：LIMIT 必须在 ORDER BY 之后使用，否则结果没有意义
-- 大数据量分页优化：当 offset 很大时，LIMIT 10000, 10 会很慢
-- 优化思路：先用子查询找到偏移位置的ID，再用 ID 过滤
-- SELECT * FROM emp_adv WHERE emp_id > (SELECT emp_id FROM emp_adv ORDER BY emp_id LIMIT 10000, 1) LIMIT 10;


-- ---------------------------------------------------------------------
-- 九、CASE WHEN 条件表达式
-- ---------------------------------------------------------------------
-- CASE WHEN 类似编程语言中的 if-else，根据条件返回不同的值
-- 有两种写法：简单 CASE 和搜索 CASE

-- ===== 1. 简单 CASE 表达式 =====
-- 语法：CASE 表达式 WHEN 值1 THEN 结果1 WHEN 值2 THEN 结果2 ... ELSE 默认值 END
-- 特点：只能做等值比较

SELECT
    emp_name,
    gender,
    CASE gender
        WHEN 'M' THEN '男'
        WHEN 'F' THEN '女'
        ELSE '未知'
    END AS 性别
FROM emp_adv;

-- ===== 2. 搜索 CASE 表达式（更灵活）=====
-- 语法：CASE WHEN 条件1 THEN 结果1 WHEN 条件2 THEN 结果2 ... ELSE 默认值 END
-- 特点：可以写任意条件，每个 WHEN 是独立的判断

SELECT
    emp_name,
    salary,
    CASE
        WHEN salary >= 15000 THEN '高薪'
        WHEN salary >= 10000 THEN '中等'
        WHEN salary >= 8000  THEN '普通'
        ELSE '偏低'
    END AS 工资等级
FROM emp_adv
ORDER BY salary DESC;

-- ===== 3. CASE WHEN 在聚合函数中的应用 =====
-- 统计每个部门的男女人数（行转列）
SELECT
    dept AS 部门,
    COUNT(*) AS 总人数,
    SUM(CASE WHEN gender = 'M' THEN 1 ELSE 0 END) AS 男员工数,
    SUM(CASE WHEN gender = 'F' THEN 1 ELSE 0 END) AS 女员工数,
    SUM(CASE WHEN gender = 'M' THEN salary ELSE 0 END) / NULLIF(SUM(CASE WHEN gender = 'M' THEN 1 ELSE 0 END), 0) AS 男员工平均工资
FROM emp_adv
GROUP BY dept;

-- ===== 4. CASE WHEN 在 ORDER BY 中的应用 =====
-- 自定义排序：总监排在前面，其他人按工资排序
SELECT emp_name, dept, manager_id, salary
FROM emp_adv
ORDER BY
    CASE WHEN manager_id IS NULL THEN 0 ELSE 1 END,  -- 总监排前面
    salary DESC;


-- =====================================================================
-- 练习题：
-- 1. 查询工资前3高的员工姓名和工资
-- 2. 统计每个城市的员工数量，按数量降序排列
-- 3. 查询平均工资最高的部门
-- 4. 用 CASE WHEN 给每个员工评定工资等级（>=15000 A, >=10000 B, >=8000 C, <8000 D）
-- 5. 查询每个部门男女员工的平均工资（行转列，部门为行，男女各一列）
-- 6. 查询有员工人数大于等于3人的部门，按人数降序
-- =====================================================================
