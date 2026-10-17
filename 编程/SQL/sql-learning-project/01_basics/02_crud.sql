-- =====================================================================
-- 02_crud.sql - 数据增删改查（CRUD）详解
-- 适用数据库：MySQL 8.0+
-- =====================================================================
-- CRUD 是数据库最基本的四种操作：
--   C (Create)  - INSERT 插入数据
--   R (Read)    - SELECT 查询数据
--   U (Update)  - UPDATE 更新数据
--   D (Delete)  - DELETE 删除数据
-- =====================================================================

-- 先准备一张演示用的员工表
DROP TABLE IF EXISTS employees;
CREATE TABLE employees (
    emp_id      INT          PRIMARY KEY AUTO_INCREMENT COMMENT '员工ID，主键自增',
    emp_name    VARCHAR(50)  NOT NULL COMMENT '员工姓名',
    department  VARCHAR(50)  DEFAULT '未分配' COMMENT '所属部门',
    salary      DECIMAL(10,2) DEFAULT 0 COMMENT '工资',
    gender      CHAR(1)      DEFAULT 'M' COMMENT '性别 M=男 F=女',
    hire_date   DATE         COMMENT '入职日期',
    email       VARCHAR(100) UNIQUE COMMENT '邮箱（唯一）',
    is_active   TINYINT(1)   DEFAULT 1 COMMENT '是否在职 1=在职 0=离职',
    create_time DATETIME     DEFAULT NOW() COMMENT '创建时间'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='员工表';


-- ---------------------------------------------------------------------
-- 一、INSERT INTO 插入数据（Create）
-- ---------------------------------------------------------------------

-- ===== 1. 基本插入（指定列名）=====
-- 语法：INSERT INTO 表名 (列1, 列2, ...) VALUES (值1, 值2, ...);
-- 优点：列的顺序可以和表结构不一致，只要值和列一一对应即可
-- 特点：只插入指定的列，其他列使用默认值或 NULL

INSERT INTO employees (emp_name, department, salary, gender, hire_date, email)
VALUES ('张三', '技术部', 8000.00, 'M', '2020-01-15', 'zhangsan@example.com');
-- 说明：emp_id 是自增的，不需要手动赋值
--       is_active 有默认值 1，create_time 有默认值 NOW()，都会自动填充

-- ===== 2. 省略列名的插入 =====
-- 语法：INSERT INTO 表名 VALUES (值1, 值2, ...);
-- 注意：必须按照表中列的顺序给所有列赋值，包括自增列（可以用 NULL 或 0 让它自增）

INSERT INTO employees
VALUES (NULL, '李四', '市场部', 7500.00, 'F', '2021-03-20', 'lisi@example.com', 1, NOW());

-- ===== 3. 批量插入（一次插入多行）=====
-- 语法：INSERT INTO 表名 (列1, 列2, ...) VALUES (值1, 值2, ...), (值1, 值2, ...), ...;
-- 优点：比逐条插入效率高很多，减少网络往返和日志写入次数

INSERT INTO employees (emp_name, department, salary, gender, hire_date, email)
VALUES
    ('王五', '技术部', 9000.00, 'M', '2019-06-10', 'wangwu@example.com'),
    ('赵六', '人事部', 6500.00, 'F', '2022-02-28', 'zhaoliu@example.com'),
    ('孙七', '财务部', 7000.00, 'M', '2021-08-15', 'sunqi@example.com'),
    ('周八', '技术部', 12000.00, 'M', '2018-05-01', 'zhouba@example.com'),
    ('吴九', '市场部', 8500.00, 'F', '2020-11-11', 'wujiu@example.com'),
    ('郑十', '技术部', 10000.00, 'F', '2019-09-09', 'zhengshi@example.com');

-- ===== 4. INSERT ... SELECT 从其他表复制数据 =====
-- 语法：INSERT INTO 目标表 (列1, 列2, ...) SELECT 列1, 列2, ... FROM 源表 WHERE 条件;
-- 作用：将查询结果直接插入到另一张表中

-- 先建一张临时表
CREATE TABLE IF NOT EXISTS emp_backup LIKE employees;

-- 把技术部的员工复制到备份表
INSERT INTO emp_backup (emp_name, department, salary, gender, hire_date, email)
SELECT emp_name, department, salary, gender, hire_date, email
FROM employees
WHERE department = '技术部';

-- ===== 5. INSERT ... ON DUPLICATE KEY UPDATE 插入或更新 =====
-- 语法：INSERT INTO 表名 (列1, 列2, ...) VALUES (值1, 值2, ...)
--       ON DUPLICATE KEY UPDATE 列1=新值1, 列2=新值2, ...;
-- 作用：如果插入时因为主键或唯一键冲突导致插入失败，则执行更新操作
-- 应用场景：数据不存在则插入，已存在则更新（"upsert" = update + insert）

-- 示例：邮箱是唯一键，如果邮箱已存在则更新姓名和工资
INSERT INTO employees (emp_name, department, salary, gender, hire_date, email)
VALUES ('张三（已更新）', '技术部', 9500.00, 'M', '2020-01-15', 'zhangsan@example.com')
ON DUPLICATE KEY UPDATE
    emp_name = VALUES(emp_name),   -- 使用 INSERT 语句中指定的值更新
    salary = VALUES(salary);       -- VALUES(列名) 表示取本次 INSERT 中该列的值

-- ===== 6. REPLACE INTO 替换插入 =====
-- 语法：REPLACE INTO 表名 (列1, 列2, ...) VALUES (值1, 值2, ...);
-- 作用：如果主键/唯一键冲突，则先删除旧记录再插入新记录
-- 注意：和 ON DUPLICATE KEY UPDATE 不同，REPLACE 是先删后插，自增ID会变化

-- REPLACE INTO employees (emp_name, email) VALUES ('测试', 'test@example.com');


-- ---------------------------------------------------------------------
-- 二、SELECT 查询数据（Read）
-- ---------------------------------------------------------------------

-- ===== 1. 基本查询 =====
-- 语法：SELECT 列1, 列2, ... FROM 表名 [WHERE 条件];

-- 查询所有列（不推荐在生产环境使用，影响性能且不明确）
SELECT * FROM employees;

-- 查询指定列（推荐，效率高，可读性好）
SELECT emp_id, emp_name, salary FROM employees;

-- ===== 2. WHERE 条件查询 =====
-- WHERE 用于过滤数据，只返回满足条件的记录

-- 单条件查询：查询技术部的员工
SELECT emp_name, salary FROM employees WHERE department = '技术部';

-- 多条件 AND：查询技术部且工资大于8000的员工
SELECT emp_name, salary FROM employees WHERE department = '技术部' AND salary > 8000;

-- 多条件 OR：查询技术部或市场部的员工
SELECT emp_name, department FROM employees WHERE department = '技术部' OR department = '市场部';

-- 比较运算符：> < >= <= = !=（或 <>）
SELECT emp_name, salary FROM employees WHERE salary >= 8000;

-- ===== 3. ORDER BY 排序 =====
-- 语法：ORDER BY 列名 [ASC|DESC], 列名2 [ASC|DESC], ...
-- ASC  升序（从小到大），默认值
-- DESC 降序（从大到小）

-- 按工资降序排列
SELECT emp_name, salary FROM employees ORDER BY salary DESC;

-- 按部门升序，同部门按工资降序（多字段排序）
SELECT emp_name, department, salary
FROM employees
ORDER BY department ASC, salary DESC;

-- ===== 4. LIMIT 分页/限制行数 =====
-- 语法1：LIMIT n                    返回前 n 条
-- 语法2：LIMIT offset, count        跳过 offset 条，返回 count 条（用于分页）
-- 注意：offset 从 0 开始

-- 查询工资最高的3名员工
SELECT emp_name, salary FROM employees ORDER BY salary DESC LIMIT 3;

-- 分页：每页3条，第2页（跳过前3条，取3条）
SELECT emp_name, salary FROM employees ORDER BY emp_id LIMIT 3, 3;

-- 分页公式：第 pageNo 页，每页 pageSize 条 → LIMIT (pageNo-1)*pageSize, pageSize

-- ===== 5. LIKE 模糊查询 =====
-- 通配符：
--   %  匹配任意长度的任意字符（包括0个字符）
--   _  匹配任意单个字符

-- 查询姓"张"的员工
SELECT emp_name FROM employees WHERE emp_name LIKE '张%';

-- 查询邮箱包含 "example" 的员工
SELECT emp_name, email FROM employees WHERE email LIKE '%example%';

-- 查询姓名第二个字是"三"的员工
SELECT emp_name FROM employees WHERE emp_name LIKE '_三%';

-- ===== 6. IN / NOT IN 查询 =====
-- IN：值在指定列表中
-- NOT IN：值不在指定列表中

-- 查询技术部和财务部的员工
SELECT emp_name, department FROM employees WHERE department IN ('技术部', '财务部');

-- 查询不在技术部也不在市场部的员工
SELECT emp_name, department FROM employees WHERE department NOT IN ('技术部', '市场部');

-- ===== 7. BETWEEN ... AND ... 范围查询 =====
-- 查询工资在7000到10000之间的员工（包含两端）
SELECT emp_name, salary FROM employees WHERE salary BETWEEN 7000 AND 10000;

-- 等价于
SELECT emp_name, salary FROM employees WHERE salary >= 7000 AND salary <= 10000;

-- ===== 8. IS NULL / IS NOT NULL 空值查询 =====
-- 注意：NULL 不能用 = 或 != 比较，必须用 IS NULL / IS NOT NULL

-- 查询入职日期为空的员工
SELECT emp_name FROM employees WHERE hire_date IS NULL;

-- 查询入职日期不为空的员工
SELECT emp_name, hire_date FROM employees WHERE hire_date IS NOT NULL;


-- ---------------------------------------------------------------------
-- 三、UPDATE 更新数据（Update）
-- ---------------------------------------------------------------------

-- 语法：UPDATE 表名 SET 列1=值1, 列2=值2, ... [WHERE 条件];
-- 注意：
--   - 必须加 WHERE 条件，否则会更新整张表的所有数据！
--   - 可以同时更新多个列，用逗号分隔

-- ===== 1. 更新单行 =====
-- 将张三的工资改为 8500
UPDATE employees SET salary = 8500 WHERE emp_name = '张三';

-- ===== 2. 更新多行 =====
-- 将技术部所有人的工资增加 500
UPDATE employees SET salary = salary + 500 WHERE department = '技术部';

-- ===== 3. 同时更新多个列 =====
UPDATE employees
SET department = '产品部',
    salary = salary * 1.1
WHERE emp_name = '赵六';

-- ===== 4. 安全更新（重要！）=====
-- 执行 UPDATE 前，建议先用 SELECT 确认影响的行数：
-- SELECT * FROM employees WHERE department = '技术部';
-- 确认无误后再执行 UPDATE

-- MySQL 可以开启安全模式防止误更新：
-- SET SQL_SAFE_UPDATES = 1;  -- 开启后，不带 WHERE 的 UPDATE/DELETE 会报错


-- ---------------------------------------------------------------------
-- 四、DELETE 删除数据（Delete）
-- ---------------------------------------------------------------------

-- 语法：DELETE FROM 表名 [WHERE 条件];
-- 注意：
--   - 必须加 WHERE 条件，否则会删除整张表的所有数据！
--   - DELETE 是逐行删除，速度较慢，可以回滚（在事务中）
--   - 自增计数器不会因为 DELETE 而重置

-- ===== 1. 删除指定行 =====
-- 删除姓名为"测试"的员工
DELETE FROM employees WHERE emp_name = '测试';

-- ===== 2. 删除多行 =====
-- 删除所有离职员工
DELETE FROM employees WHERE is_active = 0;

-- ===== 3. 安全删除（重要！）=====
-- 执行 DELETE 前，建议先用 SELECT 确认影响的行数：
-- SELECT * FROM employees WHERE is_active = 0;

-- ===== 4. DELETE vs TRUNCATE =====
-- DELETE   : 逐行删除，可以带 WHERE，可以回滚，不重置自增，速度慢
-- TRUNCATE : 整表清空，不能带 WHERE，不能回滚，重置自增，速度快
-- 参考 01_create_table.sql 中的详细对比


-- ---------------------------------------------------------------------
-- 五、批量操作性能对比
-- ---------------------------------------------------------------------

-- 批量 INSERT 比逐条 INSERT 快很多
-- 推荐写法（一次性插入）：
-- INSERT INTO table (col1, col2) VALUES (v1, v2), (v3, v4), ...;

-- 批量 UPDATE 可以用 CASE WHEN 或临时表优化
-- 示例：用 CASE WHEN 批量更新不同员工的工资
UPDATE employees
SET salary = CASE emp_name
    WHEN '张三' THEN 9000
    WHEN '李四' THEN 8000
    WHEN '王五' THEN 9500
    ELSE salary  -- 其他人工资不变，必须有，否则会被设为 NULL
END
WHERE emp_name IN ('张三', '李四', '王五');

-- 批量 DELETE 同理，尽量用 IN 条件一次删除多条，而不是循环逐条删


-- =====================================================================
-- 练习题：
-- 1. 向员工表插入3条新员工记录
-- 2. 查询所有"技术部"员工的姓名和工资，按工资降序排列
-- 3. 查询工资在 6000-9000 之间，且性别为女的员工
-- 4. 查询第2页数据（每页3条），按入职日期排序
-- 5. 将"市场部"所有人的工资增加 10%
-- 6. 删除工资低于 7000 的员工（先用 SELECT 确认）
-- =====================================================================
