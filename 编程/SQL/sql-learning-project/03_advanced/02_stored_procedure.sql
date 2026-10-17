-- =====================================================================
-- 02_stored_procedure.sql - 存储过程、函数、触发器、视图、游标
-- 适用数据库：MySQL 8.0+
-- =====================================================================
-- 注意：在 MySQL 中创建存储过程/函数/触发器时，需要先修改语句分隔符
-- 因为存储过程内部用了分号 ;，如果不修改，MySQL 遇到分号就会认为语句结束
-- 解决方法：用 DELIMITER 临时改变分隔符，创建完再改回来
-- =====================================================================


-- ---------------------------------------------------------------------
-- 一、存储过程（Stored Procedure）
-- ---------------------------------------------------------------------
-- 存储过程是一组预编译的 SQL 语句集合，保存在数据库中，可以反复调用
-- 优点：
--   1. 减少网络传输（一次调用，多条 SQL 在服务器端执行）
--   2. 提高性能（预编译，执行计划缓存）
--   3. 代码复用，封装复杂逻辑
--   4. 安全性（可以只授予执行存储过程的权限，不授予表权限）
-- 缺点：
--   1. 调试困难
--   2. 移植性差（不同数据库语法不同）
--   3. 版本管理麻烦

-- ===== 1. 创建存储过程 =====
-- 语法：
--   CREATE PROCEDURE 过程名([参数列表])
--   BEGIN
--       SQL语句;
--   END;
--
-- 参数类型：
--   IN      输入参数（默认），调用时传入，过程内可以读取
--   OUT     输出参数，过程内赋值，调用后可以获取
--   INOUT   输入输出参数，既传入又传出

-- 先修改分隔符为 //
DELIMITER //

-- 示例1：无参存储过程 - 查询所有员工
CREATE PROCEDURE IF NOT EXISTS sp_get_all_employees()
BEGIN
    -- 过程体：可以包含多条 SQL
    SELECT * FROM emp_window ORDER BY emp_id;
END //

-- 示例2：带 IN 参数的存储过程 - 按部门查询员工
CREATE PROCEDURE IF NOT EXISTS sp_get_emp_by_dept(
    IN p_dept VARCHAR(50)  -- p_ 前缀表示 parameter，便于区分
)
BEGIN
    SELECT emp_id, emp_name, salary, hire_date
    FROM emp_window
    WHERE dept = p_dept
    ORDER BY salary DESC;
END //

-- 示例3：带 OUT 参数的存储过程 - 统计部门人数和平均工资
CREATE PROCEDURE IF NOT EXISTS sp_get_dept_stats(
    IN  p_dept       VARCHAR(50),
    OUT p_total      INT,
    OUT p_avg_salary DECIMAL(10,2)
)
BEGIN
    SELECT COUNT(*), AVG(salary)
    INTO p_total, p_avg_salary   -- 将查询结果赋值给 OUT 参数
    FROM emp_window
    WHERE dept = p_dept;
END //

-- 示例4：带 INOUT 参数的存储过程
CREATE PROCEDURE IF NOT EXISTS sp_salary_increase(
    INOUT p_salary DECIMAL(10,2),
    IN    p_rate   DECIMAL(5,2)
)
BEGIN
    SET p_salary = p_salary * (1 + p_rate / 100);
END //

-- 恢复分隔符为 ;
DELIMITER ;

-- ===== 2. 调用存储过程 =====

-- 调用无参过程
CALL sp_get_all_employees();

-- 调用带 IN 参数的过程
CALL sp_get_emp_by_dept('技术部');

-- 调用带 OUT 参数的过程
-- 需要先定义用户变量接收结果
SET @total = 0;
SET @avg_sal = 0;
CALL sp_get_dept_stats('技术部', @total, @avg_sal);
SELECT @total AS 人数, @avg_sal AS 平均工资;

-- 调用带 INOUT 参数的过程
SET @sal = 10000;
CALL sp_salary_increase(@sal, 10);
SELECT @sal AS 涨薪后工资;  -- 11000

-- ===== 3. 查看/删除存储过程 =====

-- 查看所有存储过程
SHOW PROCEDURE STATUS WHERE Db = DATABASE();

-- 查看存储过程的创建语句
SHOW CREATE PROCEDURE sp_get_dept_stats;

-- 删除存储过程
-- DROP PROCEDURE IF EXISTS sp_get_all_employees;


-- ---------------------------------------------------------------------
-- 二、存储函数（Stored Function）
-- ---------------------------------------------------------------------
-- 存储函数和存储过程类似，但有以下区别：
--   - 函数必须有返回值（RETURNS 指定类型，RETURN 返回值）
--   - 函数只能有 IN 参数（不能有 OUT/INOUT）
--   - 函数可以像普通函数一样在 SELECT 中调用
--   - 函数一般用于计算并返回一个值

DELIMITER //

-- 示例1：简单函数 - 根据工资计算等级
CREATE FUNCTION IF NOT EXISTS fn_get_salary_level(p_salary DECIMAL(10,2))
RETURNS VARCHAR(20)  -- 返回值类型
DETERMINISTIC        -- 确定性函数（相同输入总是产生相同输出）
BEGIN
    DECLARE v_level VARCHAR(20);  -- 声明局部变量

    IF p_salary >= 15000 THEN
        SET v_level = '高薪';
    ELSEIF p_salary >= 10000 THEN
        SET v_level = '中等';
    ELSEIF p_salary >= 8000 THEN
        SET v_level = '普通';
    ELSE
        SET v_level = '偏低';
    END IF;

    RETURN v_level;  -- 返回结果
END //

-- 示例2：计算工龄（年）
CREATE FUNCTION IF NOT EXISTS fn_calc_work_years(p_hire_date DATE)
RETURNS INT
DETERMINISTIC
BEGIN
    RETURN TIMESTAMPDIFF(YEAR, p_hire_date, CURDATE());
END //

DELIMITER ;

-- 调用函数（和内置函数一样）
SELECT
    emp_name,
    salary,
    fn_get_salary_level(salary) AS 工资等级,
    hire_date,
    fn_calc_work_years(hire_date) AS 工龄年
FROM emp_window
ORDER BY salary DESC;

-- 查看函数
SHOW FUNCTION STATUS WHERE Db = DATABASE();
SHOW CREATE FUNCTION fn_get_salary_level;

-- 删除函数
-- DROP FUNCTION IF EXISTS fn_get_salary_level;


-- ---------------------------------------------------------------------
-- 三、变量
-- ---------------------------------------------------------------------

-- ===== 1. 用户变量（会话变量）=====
-- 以 @ 开头，在当前会话中有效
-- 不需要声明，直接赋值即可使用
SET @name = '张三';
SET @age = 25;
SELECT @name, @age;

-- 也可以用 SELECT ... INTO 赋值
SELECT COUNT(*) INTO @emp_count FROM emp_window;
SELECT @emp_count;

-- ===== 2. 局部变量 =====
-- 在存储过程/函数内部用 DECLARE 声明
-- 只在 BEGIN...END 块内有效
-- 必须声明在块的最前面（所有其他语句之前）

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_var_demo()
BEGIN
    -- 声明局部变量（必须在最前面）
    DECLARE v_name VARCHAR(50) DEFAULT '未知';
    DECLARE v_age  INT DEFAULT 0;
    DECLARE v_salary DECIMAL(10,2);

    -- 变量赋值（SET 方式）
    SET v_name = '测试员工';
    SET v_age = 30;
    SET v_salary = 10000;

    -- 也可以用 SELECT ... INTO 赋值
    SELECT AVG(salary) INTO v_salary FROM emp_window;

    -- 使用变量
    SELECT v_name AS 姓名, v_age AS 年龄, v_salary AS 平均工资;
END //
DELIMITER ;

CALL sp_var_demo();

-- ===== 3. 系统变量 =====
-- MySQL 内置的变量，影响服务器行为
-- 全局变量（GLOBAL）：影响整个服务器
-- 会话变量（SESSION）：只影响当前连接

-- 查看系统变量
SHOW VARIABLES LIKE 'character%';  -- 查看字符集相关变量
SHOW VARIABLES LIKE 'max_connections';

-- 查看全局变量
-- SHOW GLOBAL VARIABLES LIKE '%xxx%';


-- ---------------------------------------------------------------------
-- 四、流程控制
-- ---------------------------------------------------------------------

-- ===== 1. IF 语句 =====
-- 语法：
--   IF 条件1 THEN
--       语句;
--   ELSEIF 条件2 THEN
--       语句;
--   ELSE
--       语句;
--   END IF;

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_if_demo(IN p_score INT)
BEGIN
    DECLARE v_grade VARCHAR(10);

    IF p_score >= 90 THEN
        SET v_grade = '优秀';
    ELSEIF p_score >= 80 THEN
        SET v_grade = '良好';
    ELSEIF p_score >= 60 THEN
        SET v_grade = '及格';
    ELSE
        SET v_grade = '不及格';
    END IF;

    SELECT p_score AS 分数, v_grade AS 等级;
END //
DELIMITER ;

CALL sp_if_demo(85);

-- ===== 2. CASE 语句 =====
-- 语法1（简单 CASE）：
--   CASE 表达式
--       WHEN 值1 THEN 语句;
--       WHEN 值2 THEN 语句;
--       ELSE 语句;
--   END CASE;
--
-- 语法2（搜索 CASE）：
--   CASE
--       WHEN 条件1 THEN 语句;
--       WHEN 条件2 THEN 语句;
--       ELSE 语句;
--   END CASE;

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_case_demo(IN p_day INT)
BEGIN
    DECLARE v_day_name VARCHAR(10);

    CASE p_day
        WHEN 1 THEN SET v_day_name = '星期一';
        WHEN 2 THEN SET v_day_name = '星期二';
        WHEN 3 THEN SET v_day_name = '星期三';
        WHEN 4 THEN SET v_day_name = '星期四';
        WHEN 5 THEN SET v_day_name = '星期五';
        WHEN 6 THEN SET v_day_name = '星期六';
        WHEN 7 THEN SET v_day_name = '星期日';
        ELSE SET v_day_name = '无效';
    END CASE;

    SELECT p_day AS 数字, v_day_name AS 星期;
END //
DELIMITER ;

CALL sp_case_demo(3);

-- ===== 3. WHILE 循环 =====
-- 先判断条件，再执行循环体（条件为真才执行）
-- 语法：
--   WHILE 条件 DO
--       循环体;
--   END WHILE;

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_while_demo(IN p_n INT)
BEGIN
    DECLARE v_i INT DEFAULT 1;
    DECLARE v_sum INT DEFAULT 0;

    WHILE v_i <= p_n DO
        SET v_sum = v_sum + v_i;
        SET v_i = v_i + 1;
    END WHILE;

    SELECT p_n AS 上限, v_sum AS 累加和;
END //
DELIMITER ;

CALL sp_while_demo(100);  -- 1+2+...+100 = 5050

-- ===== 4. REPEAT 循环 =====
-- 先执行循环体，再判断条件（至少执行一次）
-- 语法：
--   REPEAT
--       循环体;
--   UNTIL 条件 END REPEAT;
-- 注意：UNTIL 后面没有分号

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_repeat_demo(IN p_n INT)
BEGIN
    DECLARE v_i INT DEFAULT 1;
    DECLARE v_sum INT DEFAULT 0;

    REPEAT
        SET v_sum = v_sum + v_i;
        SET v_i = v_i + 1;
    UNTIL v_i > p_n END REPEAT;  -- 直到条件为真时退出循环

    SELECT p_n AS 上限, v_sum AS 累加和;
END //
DELIMITER ;

CALL sp_repeat_demo(10);

-- ===== 5. LOOP 循环 + LEAVE / ITERATE =====
-- LOOP 是死循环，需要用 LEAVE 退出
-- LEAVE  相当于 break，退出循环
-- ITERATE 相当于 continue，跳过本次循环剩余部分，继续下一次

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_loop_demo(IN p_n INT)
BEGIN
    DECLARE v_i INT DEFAULT 1;
    DECLARE v_sum INT DEFAULT 0;

    loop_label: LOOP  -- 循环标签，用于 LEAVE 和 ITERATE
        IF v_i > p_n THEN
            LEAVE loop_label;  -- 退出循环
        END IF;

        IF v_i % 2 = 0 THEN
            SET v_i = v_i + 1;
            ITERATE loop_label;  -- 跳过偶数
        END IF;

        SET v_sum = v_sum + v_i;
        SET v_i = v_i + 1;
    END LOOP;

    SELECT p_n AS 上限, v_sum AS 奇数和;
END //
DELIMITER ;

CALL sp_loop_demo(10);  -- 1+3+5+7+9 = 25


-- ---------------------------------------------------------------------
-- 五、游标（Cursor）
-- ---------------------------------------------------------------------
-- 游标用于逐行处理查询结果集，在存储过程中遍历数据
-- 游标使用步骤：
--   1. DECLARE 声明游标
--   2. OPEN 打开游标
--   3. FETCH 获取一行数据
--   4. CLOSE 关闭游标
--
-- 注意：游标性能较差，尽量用集合操作代替，除非必须逐行处理

DELIMITER //
CREATE PROCEDURE IF NOT EXISTS sp_cursor_demo()
BEGIN
    -- 声明变量（必须在游标之前）
    DECLARE v_done INT DEFAULT FALSE;
    DECLARE v_name VARCHAR(50);
    DECLARE v_dept VARCHAR(50);
    DECLARE v_salary DECIMAL(10,2);
    DECLARE v_total DECIMAL(10,2) DEFAULT 0;
    DECLARE v_count INT DEFAULT 0;

    -- 1. 声明游标
    DECLARE cur_emp CURSOR FOR
        SELECT emp_name, dept, salary
        FROM emp_window
        WHERE dept = '技术部'
        ORDER BY salary DESC;

    -- 2. 声明 NOT FOUND 处理程序（游标遍历完后触发）
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET v_done = TRUE;

    -- 3. 打开游标
    OPEN cur_emp;

    -- 4. 循环遍历
    read_loop: LOOP
        -- 获取一行数据到变量中
        FETCH cur_emp INTO v_name, v_dept, v_salary;

        -- 如果没有更多数据，退出循环
        IF v_done THEN
            LEAVE read_loop;
        END IF;

        -- 处理每行数据
        SET v_total = v_total + v_salary;
        SET v_count = v_count + 1;
        SELECT CONCAT('员工：', v_name, '，部门：', v_dept, '，工资：', v_salary) AS 行数据;
    END LOOP;

    -- 输出统计
    SELECT v_count AS 员工数, v_total AS 工资总额, v_total / v_count AS 平均工资;

    -- 5. 关闭游标
    CLOSE cur_emp;
END //
DELIMITER ;

-- 调用（会返回多行结果）
-- CALL sp_cursor_demo();


-- ---------------------------------------------------------------------
-- 六、触发器（Trigger）
-- ---------------------------------------------------------------------
-- 触发器是在表发生 INSERT / UPDATE / DELETE 操作时自动执行的一段代码
-- 触发时机：
--   BEFORE  在操作之前执行
--   AFTER   在操作之后执行
--
-- 触发事件：
--   INSERT  插入时触发
--   UPDATE  更新时触发
--   DELETE  删除时触发
--
-- 每个表最多有 6 种触发器：BEFORE INSERT, AFTER INSERT, BEFORE UPDATE, AFTER UPDATE, BEFORE DELETE, AFTER DELETE
--
-- 触发器中可以使用：
--   NEW.列名  新值（INSERT/UPDATE 中有）
--   OLD.列名  旧值（UPDATE/DELETE 中有）

-- 准备演示表：员工表 + 工资变更日志表
DROP TABLE IF EXISTS emp_trigger;
CREATE TABLE emp_trigger (
    emp_id   INT PRIMARY KEY AUTO_INCREMENT,
    emp_name VARCHAR(50) NOT NULL,
    salary   DECIMAL(10,2),
    dept     VARCHAR(50)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE IF EXISTS salary_log;
CREATE TABLE salary_log (
    log_id     INT PRIMARY KEY AUTO_INCREMENT,
    emp_id     INT,
    emp_name   VARCHAR(50),
    old_salary DECIMAL(10,2),
    new_salary DECIMAL(10,2),
    change_type VARCHAR(20),
    change_time DATETIME DEFAULT NOW()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DELIMITER //

-- 示例1：INSERT 触发器 - 插入员工后记录日志
CREATE TRIGGER IF NOT EXISTS trg_emp_after_insert
AFTER INSERT ON emp_trigger
FOR EACH ROW  -- 每行触发一次
BEGIN
    INSERT INTO salary_log (emp_id, emp_name, old_salary, new_salary, change_type)
    VALUES (NEW.emp_id, NEW.emp_name, NULL, NEW.salary, '新增');
END //

-- 示例2：UPDATE 触发器 - 修改工资后记录变更
CREATE TRIGGER IF NOT EXISTS trg_emp_after_update
AFTER UPDATE ON emp_trigger
FOR EACH ROW
BEGIN
    -- 只有工资变化时才记录
    IF OLD.salary <> NEW.salary THEN
        INSERT INTO salary_log (emp_id, emp_name, old_salary, new_salary, change_type)
        VALUES (NEW.emp_id, NEW.emp_name, OLD.salary, NEW.salary, '修改');
    END IF;
END //

-- 示例3：DELETE 触发器 - 删除员工前记录日志
CREATE TRIGGER IF NOT EXISTS trg_emp_before_delete
BEFORE DELETE ON emp_trigger
FOR EACH ROW
BEGIN
    INSERT INTO salary_log (emp_id, emp_name, old_salary, new_salary, change_type)
    VALUES (OLD.emp_id, OLD.emp_name, OLD.salary, NULL, '删除');
END //

-- 示例4：BEFORE INSERT 触发器 - 数据校验/自动填充
CREATE TRIGGER IF NOT EXISTS trg_emp_before_insert
BEFORE INSERT ON emp_trigger
FOR EACH ROW
BEGIN
    -- 工资不能为负数
    IF NEW.salary < 0 THEN
        SET NEW.salary = 0;
    END IF;
    -- 部门默认为"未分配"
    IF NEW.dept IS NULL OR NEW.dept = '' THEN
        SET NEW.dept = '未分配';
    END IF;
END //

DELIMITER ;

-- 测试触发器
INSERT INTO emp_trigger (emp_name, salary, dept) VALUES ('张三', 10000, '技术部');
INSERT INTO emp_trigger (emp_name, salary, dept) VALUES ('李四', 8000, '市场部');
INSERT INTO emp_trigger (emp_name, salary, dept) VALUES ('王五', -500, NULL);  -- 触发 BEFORE INSERT 修正

UPDATE emp_trigger SET salary = 12000 WHERE emp_name = '张三';

DELETE FROM emp_trigger WHERE emp_name = '李四';

-- 查看日志
SELECT * FROM salary_log;
SELECT * FROM emp_trigger;

-- 查看触发器
SHOW TRIGGERS;

-- 删除触发器
-- DROP TRIGGER IF EXISTS trg_emp_after_insert;


-- ---------------------------------------------------------------------
-- 七、视图（View）
-- ---------------------------------------------------------------------
-- 视图是一张虚拟的表，其内容由查询定义
-- 视图本身不存储数据，数据来自底层的表
-- 优点：
--   1. 简化复杂查询
--   2. 安全性（可以只给用户看部分列/行）
--   3. 逻辑独立性（表结构变了，视图可以不变）

-- ===== 1. 创建视图 =====
-- 语法：CREATE VIEW 视图名 AS SELECT 查询语句;

-- 示例1：技术部员工视图
CREATE OR REPLACE VIEW v_tech_emp AS
SELECT emp_id, emp_name, salary, hire_date
FROM emp_window
WHERE dept = '技术部'
ORDER BY salary DESC;

-- 使用视图（和表一样）
SELECT * FROM v_tech_emp;

-- 示例2：部门统计视图
CREATE OR REPLACE VIEW v_dept_stats AS
SELECT
    dept            AS 部门,
    COUNT(*)        AS 人数,
    AVG(salary)     AS 平均工资,
    MAX(salary)     AS 最高工资,
    MIN(salary)     AS 最低工资,
    SUM(salary)     AS 工资总额
FROM emp_window
GROUP BY dept;

SELECT * FROM v_dept_stats ORDER BY 人数 DESC;

-- ===== 2. 视图的更新 =====
-- 简单视图可以更新（INSERT/UPDATE/DELETE），但有限制：
--   - 不能有聚合函数
--   - 不能有 DISTINCT / GROUP BY / HAVING / UNION
--   - 不能有子查询
--   - 必须包含基表中所有 NOT NULL 的列
-- 复杂视图一般只读

-- 更新简单视图
UPDATE v_tech_emp SET salary = 13000 WHERE emp_name = '张三';
-- 实际上是更新底层的 emp_window 表

-- ===== 3. 查看/删除视图 =====

-- 查看所有视图
SHOW TABLE STATUS WHERE Comment = 'view';
-- 或
SHOW FULL TABLES WHERE Table_type = 'VIEW';

-- 查看视图定义
SHOW CREATE VIEW v_dept_stats;

-- 删除视图
-- DROP VIEW IF EXISTS v_tech_emp;

-- ===== 4. WITH CHECK OPTION =====
-- 防止通过视图插入/更新不在视图可见范围内的数据
-- 例如：技术部视图，如果不加 WITH CHECK OPTION，可以插入"市场部"的员工（虽然视图看不到）
-- 加上 WITH CHECK OPTION 后，插入/更新必须满足视图的 WHERE 条件

CREATE OR REPLACE VIEW v_tech_emp_check AS
SELECT emp_id, emp_name, dept, salary
FROM emp_window
WHERE dept = '技术部'
WITH CHECK OPTION;

-- 尝试插入非技术部的员工会报错（因为 dept != '技术部'，不满足视图条件）
-- INSERT INTO v_tech_emp_check (emp_name, dept, salary) VALUES ('测试', '市场部', 8000);
-- 报错：CHECK OPTION failed


-- =====================================================================
-- 练习题：
-- 1. 创建一个存储过程，传入员工ID，返回该员工的姓名、部门、工资
-- 2. 创建一个函数，传入入职日期，返回工龄（格式：x年x个月）
-- 3. 创建一个触发器，员工表插入后自动在日志表中记录
-- 4. 创建一个视图，显示每个部门工资前3名的员工
-- 5. 用 WHILE 循环编写存储过程，计算斐波那契数列第 n 项
-- =====================================================================
