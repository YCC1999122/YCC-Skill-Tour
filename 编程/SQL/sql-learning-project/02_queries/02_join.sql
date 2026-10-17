-- =====================================================================
-- 02_join.sql - 表连接（JOIN）详解
-- 适用数据库：MySQL 8.0+
-- =====================================================================
-- JOIN 用于将两张或多张表的行组合在一起，基于它们之间的关联列
-- 常见连接类型：
--   INNER JOIN   内连接：只返回两表匹配的行
--   LEFT JOIN    左连接：返回左表全部 + 右表匹配的部分，右表不匹配的为 NULL
--   RIGHT JOIN   右连接：返回右表全部 + 左表匹配的部分，左表不匹配的为 NULL
--   FULL JOIN    全连接：返回两表所有行，不匹配的为 NULL（MySQL 不直接支持）
--   CROSS JOIN   交叉连接：两表的笛卡尔积（所有行的组合）
--   SELF JOIN    自连接：表自己和自己连接
-- =====================================================================

-- 准备演示数据：部门表 + 员工表
DROP TABLE IF EXISTS departments;
CREATE TABLE departments (
    dept_id     INT PRIMARY KEY AUTO_INCREMENT COMMENT '部门ID',
    dept_name   VARCHAR(50) NOT NULL UNIQUE COMMENT '部门名称',
    location    VARCHAR(50) COMMENT '部门所在地'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='部门表';

INSERT INTO departments (dept_name, location) VALUES
    ('技术部', '北京'),
    ('市场部', '上海'),
    ('人事部', '广州'),
    ('财务部', '杭州'),
    ('产品部', '深圳');  -- 产品部目前没有员工

DROP TABLE IF EXISTS employees_join;
CREATE TABLE employees_join (
    emp_id      INT PRIMARY KEY AUTO_INCREMENT COMMENT '员工ID',
    emp_name    VARCHAR(50) NOT NULL COMMENT '员工姓名',
    dept_id     INT COMMENT '部门ID（外键）',
    salary      DECIMAL(10,2) DEFAULT 0 COMMENT '工资',
    manager_id  INT COMMENT '上级ID（自引用）'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='员工表';

INSERT INTO employees_join (emp_name, dept_id, salary, manager_id) VALUES
    ('张三', 1, 12000.00, 3),
    ('李四', 2, 8000.00,  5),
    ('王五', 1, 18000.00, NULL),   -- 技术总监
    ('赵六', 3, 7000.00,  7),
    ('孙七', 2, 15000.00, NULL),   -- 市场总监
    ('周八', 1, 10000.00, 3),
    ('吴九', 3, 12000.00, NULL),   -- 人事总监
    ('郑十', 4, 9000.00,  9),
    ('冯十一', 4, 16000.00, NULL), -- 财务总监
    ('陈十二', NULL, 6000.00, NULL); -- 临时工，没有分配部门


-- ---------------------------------------------------------------------
-- 一、INNER JOIN 内连接
-- ---------------------------------------------------------------------
-- 语法：SELECT ... FROM 表A INNER JOIN 表B ON 连接条件;
-- 只返回两表中满足连接条件的行（交集）
-- INNER 关键字可以省略，直接写 JOIN 就是内连接

-- 查询员工及其所属部门名称
SELECT
    e.emp_id,
    e.emp_name,
    e.salary,
    d.dept_name,
    d.location
FROM employees_join e
INNER JOIN departments d
    ON e.dept_id = d.dept_id;  -- 连接条件：员工的部门ID = 部门的ID

-- 注意：
-- 1. 结果中不包含"陈十二"（没有部门，不匹配），也不包含"产品部"（没有员工，不匹配）
-- 2. ON 后面写连接条件，WHERE 后面写过滤条件
-- 3. 建议给表起别名，简化书写

-- 多条件连接
SELECT e.emp_name, d.dept_name, e.salary
FROM employees_join e
JOIN departments d
    ON e.dept_id = d.dept_id   -- 连接条件
WHERE e.salary > 10000        -- 过滤条件
ORDER BY e.salary DESC;


-- ---------------------------------------------------------------------
-- 二、LEFT JOIN 左连接（左外连接）
-- ---------------------------------------------------------------------
-- 语法：SELECT ... FROM 表A LEFT JOIN 表B ON 连接条件;
-- 返回左表（表A）的所有行，右表（表B）匹配的部分显示值，不匹配的显示 NULL
-- LEFT OUTER JOIN 和 LEFT JOIN 是一样的，OUTER 可以省略

-- 查询所有员工及其部门（包括没有部门的员工）
SELECT
    e.emp_id,
    e.emp_name,
    e.salary,
    d.dept_name,
    d.location
FROM employees_join e
LEFT JOIN departments d
    ON e.dept_id = d.dept_id;
-- 结果中"陈十二"会出现，但 dept_name 和 location 为 NULL

-- 找出没有分配部门的员工
SELECT e.emp_name
FROM employees_join e
LEFT JOIN departments d
    ON e.dept_id = d.dept_id
WHERE d.dept_id IS NULL;  -- 右表的主键/非空列为 NULL 表示没有匹配


-- ---------------------------------------------------------------------
-- 三、RIGHT JOIN 右连接（右外连接）
-- ---------------------------------------------------------------------
-- 语法：SELECT ... FROM 表A RIGHT JOIN 表B ON 连接条件;
-- 返回右表（表B）的所有行，左表（表A）匹配的部分显示值，不匹配的显示 NULL
-- RIGHT OUTER JOIN 和 RIGHT JOIN 是一样的

-- 查询所有部门及其员工（包括没有员工的部门）
SELECT
    d.dept_id,
    d.dept_name,
    d.location,
    e.emp_name,
    e.salary
FROM employees_join e
RIGHT JOIN departments d
    ON e.dept_id = d.dept_id;
-- 结果中"产品部"会出现，但员工相关列为 NULL

-- 找出没有员工的部门
SELECT d.dept_name
FROM employees_join e
RIGHT JOIN departments d
    ON e.dept_id = d.dept_id
WHERE e.emp_id IS NULL;

-- 注意：RIGHT JOIN 可以用 LEFT JOIN 改写（交换两表位置）
-- 上面的查询等价于：
-- SELECT d.dept_name FROM departments d LEFT JOIN employees_join e ON d.dept_id = e.dept_id WHERE e.emp_id IS NULL;


-- ---------------------------------------------------------------------
-- 四、FULL JOIN 全连接（全外连接）
-- ---------------------------------------------------------------------
-- 全连接返回两表的所有行，不匹配的部分为 NULL
-- 注意：MySQL 不直接支持 FULL JOIN，需要用 LEFT JOIN UNION RIGHT JOIN 模拟

-- 模拟 FULL JOIN：所有员工 + 所有部门，不匹配的显示 NULL
SELECT e.emp_name, d.dept_name
FROM employees_join e
LEFT JOIN departments d ON e.dept_id = d.dept_id

UNION  -- 去重合并

SELECT e.emp_name, d.dept_name
FROM employees_join e
RIGHT JOIN departments d ON e.dept_id = d.dept_id;

-- 注意：
-- UNION 会去除重复行（内连接匹配的部分两边都有，会合并成一行）
-- 如果用 UNION ALL 则不去重，结果会有重复行


-- ---------------------------------------------------------------------
-- 五、SELF JOIN 自连接
-- ---------------------------------------------------------------------
-- 表自己和自己连接，常用于处理有层级关系的数据（如员工-领导、分类-子分类）
-- 必须给表起不同的别名，否则无法区分

-- 查询员工及其上级领导的姓名
SELECT
    e.emp_name   AS 员工姓名,
    e.salary     AS 员工工资,
    m.emp_name   AS 领导姓名,
    m.salary     AS 领导工资
FROM employees_join e
LEFT JOIN employees_join m
    ON e.manager_id = m.emp_id;  -- 员工的上级ID = 领导的员工ID
-- e 代表员工，m 代表 manager（领导）
-- 最高领导（manager_id 为 NULL）的领导信息为 NULL

-- 多层自连接：员工 -> 直属领导 -> 大领导（如果有的话）
-- 这里只有两层，就不演示了

-- 查询工资比自己领导高的员工
SELECT
    e.emp_name   AS 员工,
    e.salary     AS 员工工资,
    m.emp_name   AS 领导,
    m.salary     AS 领导工资
FROM employees_join e
JOIN employees_join m
    ON e.manager_id = m.emp_id
WHERE e.salary > m.salary;


-- ---------------------------------------------------------------------
-- 六、CROSS JOIN 交叉连接（笛卡尔积）
-- ---------------------------------------------------------------------
-- 语法：SELECT ... FROM 表A CROSS JOIN 表B;
-- 返回两表所有行的组合，结果行数 = 表A行数 × 表B行数
-- 没有 ON 条件
-- 用途：生成测试数据、排列组合等

-- 员工和部门的所有组合
SELECT e.emp_name, d.dept_name
FROM employees_join e
CROSS JOIN departments d
ORDER BY e.emp_name;

-- CROSS JOIN 也可以写成隐式的：FROM 表A, 表B
-- SELECT e.emp_name, d.dept_name FROM employees_join e, departments d;

-- 注意：
-- 1. 交叉连接的结果数可能非常大，谨慎使用
-- 2. 如果在 WHERE 中加条件，效果等同于 INNER JOIN
--    SELECT * FROM a, b WHERE a.id = b.id;  等价于 INNER JOIN ON


-- ---------------------------------------------------------------------
-- 七、多表连接
-- ---------------------------------------------------------------------
-- 实际业务中经常需要连接 3 张甚至更多表
-- 连接顺序：可以是 A JOIN B ON ... JOIN C ON ...

-- 再加一张项目表
DROP TABLE IF EXISTS projects;
CREATE TABLE projects (
    project_id   INT PRIMARY KEY AUTO_INCREMENT COMMENT '项目ID',
    project_name VARCHAR(100) NOT NULL COMMENT '项目名称',
    dept_id      INT COMMENT '负责部门ID'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO projects (project_name, dept_id) VALUES
    ('系统升级', 1),
    ('营销活动', 2),
    ('薪酬改革', 3),
    ('财务系统', 4),
    ('新产品研发', 1),
    ('品牌推广', 2);

-- 查询：员工 -> 部门 -> 项目（三表连接）
SELECT
    e.emp_name     AS 员工姓名,
    d.dept_name    AS 部门名称,
    d.location     AS 部门地点,
    p.project_name AS 项目名称
FROM employees_join e
INNER JOIN departments d
    ON e.dept_id = d.dept_id
INNER JOIN projects p
    ON d.dept_id = p.dept_id
ORDER BY d.dept_name, e.emp_name;

-- 左连接也可以链式使用
-- 查询所有员工，及其部门（如果有），及其部门的项目（如果有）
SELECT
    e.emp_name,
    d.dept_name,
    p.project_name
FROM employees_join e
LEFT JOIN departments d
    ON e.dept_id = d.dept_id
LEFT JOIN projects p
    ON d.dept_id = p.dept_id;


-- ---------------------------------------------------------------------
-- 八、USING / NATURAL JOIN
-- ---------------------------------------------------------------------

-- ===== 1. USING 子句 =====
-- 当两张表的连接列名相同时，可以用 USING 简化 ON 条件
-- 语法：JOIN 表B USING (列名)
-- 相当于 ON 表A.列名 = 表B.列名

SELECT e.emp_name, d.dept_name
FROM employees_join e
JOIN departments d
    USING (dept_id);  -- 等价于 ON e.dept_id = d.dept_id

-- USING 的特点：
-- - 连接列在结果中只出现一次（ON 会出现两次，各属于不同的表）
-- - 可以同时指定多个列：USING (col1, col2)

-- ===== 2. NATURAL JOIN 自然连接 =====
-- 自动连接两张表中所有名字相同的列
-- 语法：NATURAL JOIN 表B
-- 相当于 USING (两表所有同名列)

SELECT e.emp_name, d.dept_name
FROM employees_join e
NATURAL JOIN departments d;

-- 注意：
-- - NATURAL JOIN 会自动匹配所有同名列，如果有意外的同名列（如 create_time），可能导致错误
-- - 不推荐在生产环境使用，不够明确，表结构变化可能影响结果


-- ---------------------------------------------------------------------
-- 九、连接的执行顺序与性能
-- ---------------------------------------------------------------------

-- SQL 语句的书写顺序：
--   SELECT -> FROM -> JOIN -> ON -> WHERE -> GROUP BY -> HAVING -> ORDER BY -> LIMIT

-- SQL 的逻辑执行顺序（大致）：
--   FROM/JOIN -> ON -> WHERE -> GROUP BY -> HAVING -> SELECT -> DISTINCT -> ORDER BY -> LIMIT

-- 连接的性能优化：
-- 1. 连接列最好有索引（主键、外键、索引列）
-- 2. 小表驱动大表（小表在前，大表在后），MySQL 优化器会自动处理
-- 3. 尽量用 INNER JOIN，避免 LEFT JOIN（左连接必须保留左表全部，优化空间小）
-- 4. 减少连接的表数量，连接越多性能越差
-- 5. ON 和 WHERE 的条件要分清：
--    - ON 中写连接条件
--    - WHERE 中写过滤条件
--    - 对于 LEFT JOIN，右表的过滤条件写在 ON 和 WHERE 中结果不同！

-- 重要区别：LEFT JOIN 中，右表的条件放在 ON vs WHERE
-- 条件在 ON 中：先按条件连接右表，不满足的右表显示 NULL（左表全部保留）
-- 条件在 WHERE 中：连接后再过滤，右表为 NULL 的行会被过滤掉（效果类似 INNER JOIN）

-- 示例：左连接 + 右表条件在 ON 中
SELECT e.emp_name, d.dept_name
FROM employees_join e
LEFT JOIN departments d
    ON e.dept_id = d.dept_id
    AND d.location = '北京';  -- 右表条件写在 ON 中
-- 结果：所有员工都在，非北京部门的 dept_name 为 NULL

-- 示例：左连接 + 右表条件在 WHERE 中
SELECT e.emp_name, d.dept_name
FROM employees_join e
LEFT JOIN departments d
    ON e.dept_id = d.dept_id
WHERE d.location = '北京';  -- 右表条件写在 WHERE 中
-- 结果：只有北京部门的员工，相当于内连接的效果


-- =====================================================================
-- 练习题：
-- 1. 查询所有员工的姓名、部门名称、工资
-- 2. 查询没有分配部门的员工姓名
-- 3. 查询没有员工的部门名称
-- 4. 查询每个员工及其上级领导的姓名（包括没有领导的）
-- 5. 查询每个部门的项目数量（包括没有项目的部门）
-- 6. 查询工资比部门平均工资高的员工（用自连接或子查询）
-- =====================================================================
