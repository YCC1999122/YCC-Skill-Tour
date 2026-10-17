-- =====================================================================
-- 03_subquery.sql - 子查询与联合查询详解
-- 适用数据库：MySQL 8.0+
-- =====================================================================
-- 子查询（Subquery）：嵌套在另一个查询中的查询
-- 按位置分类：
--   WHERE 子查询   子查询在 WHERE 条件中
--   FROM 子查询    子查询在 FROM 后面（作为临时表）
--   SELECT 子查询  子查询在 SELECT 列中（标量子查询）
--   EXISTS 子查询  子查询在 EXISTS 中
--
-- 按相关性分类：
--   非相关子查询   子查询可以独立运行，不依赖外层查询
--   相关子查询     子查询依赖外层查询的列，必须和外层一起执行
-- =====================================================================

-- 准备演示数据
DROP TABLE IF EXISTS emp_sub;
CREATE TABLE emp_sub (
    emp_id    INT PRIMARY KEY AUTO_INCREMENT,
    emp_name  VARCHAR(50) NOT NULL,
    dept      VARCHAR(50),
    salary    DECIMAL(10,2),
    hire_date DATE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO emp_sub (emp_name, dept, salary, hire_date) VALUES
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


-- ---------------------------------------------------------------------
-- 一、WHERE 子查询
-- ---------------------------------------------------------------------
-- 子查询出现在 WHERE 条件中，用于过滤数据

-- ===== 1. 标量子查询（子查询返回单个值）=====
-- 子查询只返回一行一列，可以用 = > < >= <= != 等比较运算符

-- 查询工资高于平均工资的员工
SELECT emp_name, salary
FROM emp_sub
WHERE salary > (SELECT AVG(salary) FROM emp_sub);
-- 子查询 (SELECT AVG(salary) FROM emp_sub) 返回平均工资（一个数值）
-- 这是一个非相关子查询，子查询可以独立运行

-- 查询工资最高的员工
SELECT emp_name, salary
FROM emp_sub
WHERE salary = (SELECT MAX(salary) FROM emp_sub);

-- ===== 2. 列子查询（子查询返回一列多行）=====
-- 子查询返回一列多行，需要用 IN / NOT IN / ANY / ALL 等运算符

-- 查询工资等于各部门最高工资的员工（有哪些人的工资是所在部门最高的）
SELECT emp_name, dept, salary
FROM emp_sub
WHERE (dept, salary) IN (
    SELECT dept, MAX(salary)
    FROM emp_sub
    GROUP BY dept
);
-- 这是行子查询（返回多列多行），用 IN 匹配整行

-- ===== 3. IN / NOT IN 子查询 =====
-- IN：外层列的值在子查询结果中
-- NOT IN：外层列的值不在子查询结果中

-- 查询技术部和财务部的员工（用 IN 子查询实现）
SELECT emp_name, dept
FROM emp_sub
WHERE dept IN (
    SELECT dept FROM emp_sub WHERE dept IN ('技术部', '财务部')
);
-- 这个例子只是演示，实际用 WHERE dept IN ('技术部', '财务部') 更简单

-- ===== 4. ANY / SOME 子查询 =====
-- ANY：外层值与子查询结果中任意一个满足条件即可
-- SOME 是 ANY 的别名，用法完全一样
-- 运算符：> ANY, < ANY, = ANY, >= ANY, <= ANY, != ANY

-- 查询工资比任意一个"技术部"员工低的员工（即只要比技术部最低工资低就行）
SELECT emp_name, dept, salary
FROM emp_sub
WHERE salary < ANY (
    SELECT salary FROM emp_sub WHERE dept = '技术部'
);
-- < ANY 表示小于子查询结果中的最大值（只要比最大的小就满足"比任意一个小"）
-- 等一下：salary < ANY (12000, 18000, 10000, 14000)
-- 只要小于其中任意一个就成立 → 小于最大值 18000 就成立
-- 所有工资 < 18000 的员工都会被选上

-- > ANY 表示大于最小值
-- = ANY 等价于 IN

-- ===== 5. ALL 子查询 =====
-- ALL：外层值与子查询结果中所有行都满足条件才返回 TRUE
-- 运算符：> ALL, < ALL, = ALL, >= ALL, <= ALL, != ALL

-- 查询工资比技术部所有人都高的员工（比技术部最高工资还高）
SELECT emp_name, dept, salary
FROM emp_sub
WHERE salary > ALL (
    SELECT salary FROM emp_sub WHERE dept = '技术部'
);
-- > ALL 表示大于子查询结果中的最大值
-- 技术部工资：12000, 18000, 10000, 14000 → 最大值 18000
-- 比 18000 高的才返回 → 结果为空（因为最高就是18000）

-- < ALL 表示小于最小值
-- != ALL 等价于 NOT IN

-- ===== 6. EXISTS / NOT EXISTS 子查询 =====
-- EXISTS：子查询至少返回一行则为 TRUE
-- NOT EXISTS：子查询没有返回行则为 TRUE
-- 特点：不关心子查询返回什么内容，只关心有没有行
-- 通常是相关子查询（依赖外层表的列）

-- 查询有工资大于 15000 员工的部门
SELECT DISTINCT dept
FROM emp_sub e1
WHERE EXISTS (
    SELECT 1 FROM emp_sub e2
    WHERE e2.dept = e1.dept  -- 关联外层的部门
      AND e2.salary > 15000
);
-- 相关子查询：e2.dept = e1.dept 引用了外层查询的 e1.dept
-- 执行过程：对外层每一行，执行子查询检查是否存在满足条件的行

-- NOT EXISTS：查询没有工资大于 15000 员工的部门
SELECT DISTINCT dept
FROM emp_sub e1
WHERE NOT EXISTS (
    SELECT 1 FROM emp_sub e2
    WHERE e2.dept = e1.dept
      AND e2.salary > 15000
);

-- EXISTS vs IN 的选择：
-- - 当子查询结果集很大时，EXISTS 通常更快（找到一条就停止）
-- - 当子查询结果集很小时，IN 可能更快
-- - 注意 NULL 的处理：IN 列表中有 NULL 时，NOT IN 结果为空


-- ---------------------------------------------------------------------
-- 二、FROM 子查询（派生表 / Derived Table）
-- ---------------------------------------------------------------------
-- 子查询出现在 FROM 后面，作为一张临时表使用
-- 必须给子查询起别名

-- 查询每个部门的平均工资，再找出平均工资大于 10000 的部门
SELECT dept, avg_salary
FROM (
    -- 子查询作为临时表，计算每个部门的平均工资
    SELECT dept, AVG(salary) AS avg_salary
    FROM emp_sub
    GROUP BY dept
) AS dept_avg  -- 必须起别名
WHERE avg_salary > 10000
ORDER BY avg_salary DESC;
-- 先执行子查询得到每个部门的平均工资（作为临时表）
-- 外层再对临时表进行过滤

-- 更复杂的例子：先按部门分组统计，再和员工表关联
SELECT e.emp_name, e.dept, e.salary, d.avg_salary, e.salary - d.avg_salary AS 与部门平均差值
FROM emp_sub e
JOIN (
    SELECT dept, AVG(salary) AS avg_salary
    FROM emp_sub
    GROUP BY dept
) d ON e.dept = d.dept
ORDER BY e.dept, e.salary DESC;

-- 注意：
-- 1. FROM 子查询必须有别名
-- 2. MySQL 8.0 开始支持 CTE（WITH 子句），可以更优雅地实现类似功能


-- ---------------------------------------------------------------------
-- 三、SELECT 子查询（标量子查询）
-- ---------------------------------------------------------------------
-- 子查询出现在 SELECT 列的位置，必须返回单行单列（标量值）
-- 通常是相关子查询

-- 查询每个员工的姓名、工资，以及其部门的平均工资
SELECT
    emp_name,
    salary,
    dept,
    (SELECT AVG(salary) FROM emp_sub WHERE dept = e.dept) AS 部门平均工资
FROM emp_sub e
ORDER BY dept;
-- 对每一行员工数据，执行一次子查询计算该员工所在部门的平均工资
-- 这是相关子查询，因为 WHERE dept = e.dept 引用了外层的 e.dept

-- 再比如：查询每个部门的人数（用 SELECT 子查询）
SELECT DISTINCT
    dept,
    (SELECT COUNT(*) FROM emp_sub WHERE dept = d.dept) AS 人数
FROM emp_sub d
ORDER BY dept;

-- 注意：SELECT 子查询必须确保只返回一行一列，否则会报错
-- 相关子查询的性能通常较差（每行都要执行一次子查询），能用 JOIN 改写尽量用 JOIN


-- ---------------------------------------------------------------------
-- 四、相关子查询 vs 非相关子查询
-- ---------------------------------------------------------------------

-- ===== 非相关子查询（Uncorrelated Subquery）=====
-- 子查询不依赖外层查询，可以独立运行
-- 执行顺序：先执行子查询，再用结果执行外层查询
-- 只执行一次

-- 示例：查询工资高于平均工资的员工
SELECT emp_name, salary
FROM emp_sub
WHERE salary > (SELECT AVG(salary) FROM emp_sub);
-- 子查询可以独立运行：SELECT AVG(salary) FROM emp_sub;

-- ===== 相关子查询（Correlated Subquery）=====
-- 子查询依赖外层查询的列，不能独立运行
-- 执行顺序：对外层每一行，执行一次子查询
-- 执行次数 = 外层行数

-- 示例：查询工资高于所在部门平均工资的员工
SELECT e.emp_name, e.dept, e.salary
FROM emp_sub e
WHERE e.salary > (
    SELECT AVG(salary)
    FROM emp_sub
    WHERE dept = e.dept  -- 引用了外层的 e.dept
);
-- 子查询不能独立运行（因为 e.dept 是外层的）
-- 外层有 10 行，子查询就要执行 10 次

-- 相关子查询通常可以用 JOIN 改写，性能更好：
SELECT e.emp_name, e.dept, e.salary
FROM emp_sub e
JOIN (
    SELECT dept, AVG(salary) AS avg_sal
    FROM emp_sub
    GROUP BY dept
) d ON e.dept = d.dept
WHERE e.salary > d.avg_sal;


-- ---------------------------------------------------------------------
-- 五、UNION / UNION ALL 联合查询
-- ---------------------------------------------------------------------
-- 将两个或多个 SELECT 语句的结果合并成一个结果集
-- 要求：
--   1. 每个 SELECT 的列数必须相同
--   2. 对应列的数据类型必须兼容
--   3. 结果的列名由第一个 SELECT 决定

-- ===== 1. UNION 去重合并 =====
-- 合并后去除重复的行

-- 技术部和市场部的员工（用 UNION 实现，实际用 IN 更简单）
SELECT emp_name, dept FROM emp_sub WHERE dept = '技术部'
UNION
SELECT emp_name, dept FROM emp_sub WHERE dept = '市场部'
ORDER BY dept, emp_name;
-- UNION 会去重，如果两边有相同的行，只保留一条

-- ===== 2. UNION ALL 不去重合并 =====
-- 直接合并，不去除重复行，速度更快

SELECT emp_name, dept FROM emp_sub WHERE dept = '技术部'
UNION ALL
SELECT emp_name, dept FROM emp_sub WHERE dept = '市场部'
ORDER BY dept, emp_name;

-- UNION vs UNION ALL 选择：
-- - 如果确定没有重复数据，或者重复数据不影响结果，优先用 UNION ALL（性能好）
-- - 如果需要去重，用 UNION

-- ===== 3. UNION 的排序 =====
-- ORDER BY 写在最后一个 SELECT 后面，对整个合并结果排序
-- 如果要对每个 SELECT 内部排序，需要用 LIMIT 包裹（否则 ORDER BY 会被忽略）

-- ===== 4. 典型应用场景 =====
-- 场景1：从多张结构相似的表中汇总数据
-- 场景2：将不同维度的统计结果合并展示
-- 场景3：模拟 FULL JOIN（LEFT JOIN UNION RIGHT JOIN）

-- 示例：统计各部门人数和总人数（部门明细 + 合计行）
SELECT dept, COUNT(*) AS 人数
FROM emp_sub
GROUP BY dept
UNION ALL
SELECT '合计' AS dept, COUNT(*) AS 人数
FROM emp_sub
ORDER BY 人数 DESC;


-- ---------------------------------------------------------------------
-- 六、INTERSECT 交集 / EXCEPT 差集（MySQL 模拟）
-- ---------------------------------------------------------------------
-- INTERSECT：返回两个查询结果的交集（两边都有的行）
-- EXCEPT / MINUS：返回第一个查询有但第二个查询没有的行
-- 注意：MySQL 不直接支持 INTERSECT 和 EXCEPT，需要用其他方式模拟

-- 准备第二张表
DROP TABLE IF EXISTS emp_sub2;
CREATE TABLE emp_sub2 LIKE emp_sub;
INSERT INTO emp_sub2 (emp_name, dept, salary, hire_date) VALUES
    ('张三', '技术部', 12000, '2019-03-15'),  -- 表1有
    ('李四', '市场部', 8000,  '2020-06-20'),  -- 表1有
    ('赵六', '人事部', 7000,  '2021-09-01'),  -- 表1有
    ('新员工A', '产品部', 9000, '2023-01-01'), -- 只有表2有
    ('新员工B', '运营部', 8500, '2023-02-01'); -- 只有表2有

-- ===== 1. 交集 INTERSECT 模拟 =====
-- 方法：用 INNER JOIN 或 IN 子查询

-- 用 INNER JOIN 模拟交集（两表都有的员工姓名）
SELECT DISTINCT e1.emp_name
FROM emp_sub e1
INNER JOIN emp_sub2 e2 ON e1.emp_name = e2.emp_name;

-- ===== 2. 差集 EXCEPT 模拟 =====
-- 方法：用 LEFT JOIN + IS NULL 或 NOT IN / NOT EXISTS

-- 用 LEFT JOIN 模拟差集（表1有但表2没有的员工）
SELECT e1.emp_name
FROM emp_sub e1
LEFT JOIN emp_sub2 e2 ON e1.emp_name = e2.emp_name
WHERE e2.emp_name IS NULL;

-- 用 NOT EXISTS 模拟差集
SELECT e1.emp_name
FROM emp_sub e1
WHERE NOT EXISTS (
    SELECT 1 FROM emp_sub2 e2 WHERE e2.emp_name = e1.emp_name
);


-- ---------------------------------------------------------------------
-- 七、WITH 公用表表达式（CTE，MySQL 8.0+）
-- ---------------------------------------------------------------------
-- WITH 子句可以定义一个或多个临时命名结果集，供后续查询使用
-- 优点：
--   1. 代码更清晰，可读性好
--   2. 可以引用多次，避免重复写子查询
--   3. 支持递归（WITH RECURSIVE）

-- 基本语法：
-- WITH cte_name AS (
--     SELECT ...
-- )
-- SELECT ... FROM cte_name;

-- 示例：先统计各部门平均工资，再查询高于平均的员工
WITH dept_avg AS (
    SELECT dept, AVG(salary) AS avg_salary
    FROM emp_sub
    GROUP BY dept
)
SELECT e.emp_name, e.dept, e.salary, d.avg_salary
FROM emp_sub e
JOIN dept_avg d ON e.dept = d.dept
WHERE e.salary > d.avg_salary
ORDER BY e.dept;

-- 多个 CTE：用逗号分隔
WITH
dept_stats AS (
    SELECT dept, COUNT(*) AS cnt, AVG(salary) AS avg_sal
    FROM emp_sub GROUP BY dept
),
high_sal_emp AS (
    SELECT emp_name, dept, salary
    FROM emp_sub WHERE salary > 12000
)
SELECT h.emp_name, h.dept, h.salary, d.avg_sal, d.cnt
FROM high_sal_emp h
JOIN dept_stats d ON h.dept = d.dept;

-- 递归 CTE（WITH RECURSIVE）：可以查询层级结构（如组织架构、树形分类）
-- 示例：生成 1 到 10 的数字序列
WITH RECURSIVE numbers AS (
    SELECT 1 AS n           -- 锚点成员：初始值
    UNION ALL
    SELECT n + 1 FROM numbers WHERE n < 10  -- 递归成员：每次 +1
)
SELECT n FROM numbers;


-- =====================================================================
-- 练习题：
-- 1. 用子查询找出工资最高的员工姓名
-- 2. 用相关子查询找出工资高于所在部门平均工资的员工
-- 3. 用 EXISTS 查询有员工工资大于 15000 的部门
-- 4. 用 FROM 子查询统计每个部门人数和平均工资，再过滤出人数>=3的部门
-- 5. 用 UNION ALL 生成"部门统计表 + 合计行"
-- 6. 用 CTE 改写第 4 题
-- =====================================================================
