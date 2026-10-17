-- =====================================================================
-- 01_performance.sql - SQL 性能优化：EXPLAIN、索引优化、SQL优化
-- 适用数据库：MySQL 8.0+ / InnoDB 引擎
-- =====================================================================

-- 准备演示数据
DROP TABLE IF EXISTS orders_opt;
CREATE TABLE orders_opt (
    order_id     BIGINT PRIMARY KEY AUTO_INCREMENT COMMENT '订单ID',
    user_id      BIGINT NOT NULL COMMENT '用户ID',
    order_no     VARCHAR(32) NOT NULL COMMENT '订单号',
    total_amount DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '订单金额',
    status       TINYINT NOT NULL DEFAULT 0 COMMENT '状态：0待付款 1已付款 2已发货 3已完成 4已取消',
    pay_time     DATETIME COMMENT '支付时间',
    create_time  DATETIME NOT NULL DEFAULT NOW() COMMENT '创建时间',
    update_time  DATETIME NOT NULL DEFAULT NOW() ON UPDATE NOW() COMMENT '更新时间'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单表';

-- 插入一些测试数据（实际优化场景下数据量要大才明显）
INSERT INTO orders_opt (user_id, order_no, total_amount, status, pay_time, create_time)
SELECT
    (RAND() * 1000) + 1,                                     -- 1~1000 随机用户
    CONCAT('ORD', LPAD((@row:=@row+1), 10, '0')),            -- 订单号
    ROUND(RAND() * 5000 + 100, 2),                           -- 100~5100 随机金额
    FLOOR(RAND() * 5),                                       -- 0~4 随机状态
    DATE_ADD('2024-01-01', INTERVAL RAND() * 365 DAY),       -- 随机日期
    DATE_ADD('2024-01-01', INTERVAL RAND() * 365 DAY)
FROM
    (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3 UNION ALL SELECT 4
     UNION ALL SELECT 5 UNION ALL SELECT 6 UNION ALL SELECT 7
     UNION ALL SELECT 8 UNION ALL SELECT 9 UNION ALL SELECT 10) a,
    (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3 UNION ALL SELECT 4
     UNION ALL SELECT 5 UNION ALL SELECT 6 UNION ALL SELECT 7
     UNION ALL SELECT 8 UNION ALL SELECT 9 UNION ALL SELECT 10) b,
    (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3 UNION ALL SELECT 4
     UNION ALL SELECT 5 UNION ALL SELECT 6 UNION ALL SELECT 7
     UNION ALL SELECT 8 UNION ALL SELECT 9 UNION ALL SELECT 10) c,
    (SELECT @row := 0) d;
-- 生成 1000 条测试数据（如果数据量不够，可以再运行几次）

-- 建一些索引用于演示
CREATE INDEX idx_user_id ON orders_opt(user_id);
CREATE INDEX idx_status ON orders_opt(status);
CREATE INDEX idx_create_time ON orders_opt(create_time);
CREATE UNIQUE INDEX uk_order_no ON orders_opt(order_no);
CREATE INDEX idx_user_status ON orders_opt(user_id, status);


-- ---------------------------------------------------------------------
-- 一、EXPLAIN 执行计划解读
-- ---------------------------------------------------------------------
-- EXPLAIN 可以查看 MySQL 如何执行一条 SQL 语句，是优化 SQL 的最重要工具
-- 语法：EXPLAIN SELECT ...;
-- 也可以用于 UPDATE/DELETE 等（8.0+ 支持）

-- 基本用法
EXPLAIN SELECT * FROM orders_opt WHERE order_id = 100;

-- EXPLAIN ANALYZE（MySQL 8.0.18+）：实际执行并输出执行计划和耗时
-- EXPLAIN ANALYZE SELECT * FROM orders_opt WHERE user_id = 100;

-- ===== EXPLAIN 输出列详解 =====
--
-- 1. id
--    查询的序号，标识操作的执行顺序
--    id 越大越先执行；id 相同，从上到下执行
--
-- 2. select_type
--    查询类型，常见值：
--    - SIMPLE         简单查询，不包含子查询或 UNION
--    - PRIMARY        最外层查询（有子查询时）
--    - SUBQUERY       子查询（非相关子查询）
--    - DEPENDENT SUBQUERY  相关子查询
--    - DERIVED        派生表（FROM 子查询）
--    - UNION          UNION 中的第二个及之后的查询
--    - UNION RESULT   UNION 的结果
--
-- 3. table
--    输出行对应的表名，可能是别名、派生表、联合结果等
--
-- 4. partitions
--    查询涉及的分区，没有分区则为 NULL
--
-- 5. type（重要！访问类型）
--    表示 MySQL 在表中找到所需行的方式，性能从好到差大致排序：
--
--    system > const > eq_ref > ref > range > index > ALL
--
--    - system      表中只有一行（系统表），极少出现
--    - const       通过主键或唯一索引匹配，最多返回一行
--                  例如：WHERE 主键 = 值
--    - eq_ref      多表连接时，使用主键或唯一索引作为连接条件
--                  对于前表的每一行，后表只匹配一行
--    - ref         使用非唯一索引，可能返回多行
--                  例如：WHERE 普通索引列 = 值
--    - ref_or_null 类似 ref，但还包含 NULL 值的查询
--    - range       索引范围扫描，常见于 BETWEEN, >, <, IN 等
--    - index       全索引扫描，遍历整个索引树（比全表扫描快，因为索引通常比数据小）
--    - ALL         全表扫描，性能最差，需要优化
--
--    优化目标：至少达到 range 级别，最好是 ref 或 eq_ref
--
-- 6. possible_keys
--    可能用到的索引（候选索引）
--    MySQL 会从这些索引中选一个最优的
--    如果为 NULL，说明没有可用的索引
--
-- 7. key
--    实际使用的索引
--    如果为 NULL，说明没有使用索引（全表扫描）
--
-- 8. key_len
--    使用的索引的字节数
--    可以用来判断复合索引实际使用了多少列
--    计算方法：
--      - INT        : 4 字节
--      - BIGINT     : 8 字节
--      - CHAR(n)    : n × 字符集字节数（utf8mb4 是 4）
--      - VARCHAR(n) : n × 字符集字节数 + 2（变长）
--      - 允许 NULL  : +1 字节
--
-- 9. ref
--    显示索引的哪一列被用来和索引比较（常量 const 或其他表的列）
--
-- 10. rows（重要！预估扫描行数）
--     MySQL 估算要找到结果需要扫描多少行
--     数值越小越好，越大说明性能越差
--
-- 11. filtered
--     通过条件过滤后剩余行的百分比（估算）
--     值越高越好，100% 表示没有过滤
--
-- 12. Extra（重要！额外信息）
--     包含一些重要的附加信息，常见值：
--
--     - Using index          覆盖索引，查询的列都在索引中，不需要回表（性能好）
--     - Using where          存储引擎层筛选后，在 Server 层再用 WHERE 过滤
--     - Using index condition  索引条件下推（ICP），在索引层面就用 WHERE 过滤
--     - Using temporary      使用了临时表（常见于 GROUP BY / ORDER BY 不同列时，性能差）
--     - Using filesort       使用了文件排序（ORDER BY 没有用到索引，性能差）
--     - Using join buffer    连接时使用了连接缓冲区
--     - Impossible WHERE     WHERE 条件永远为 FALSE
--     - Select tables optimized away  聚合函数在索引上直接计算，不需要扫描表
--     - NULL                 正常情况

-- ===== 示例 1：主键查询 =====
EXPLAIN SELECT * FROM orders_opt WHERE order_id = 100;
-- type: const （主键等值查询）
-- key: PRIMARY
-- rows: 1

-- ===== 示例 2：唯一索引查询 =====
EXPLAIN SELECT * FROM orders_opt WHERE order_no = 'ORD0000000100';
-- type: const （唯一索引等值查询）
-- key: uk_order_no

-- ===== 示例 3：普通索引查询 =====
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100;
-- type: ref （普通索引等值查询）
-- key: idx_user_id

-- ===== 示例 4：覆盖索引 =====
EXPLAIN SELECT user_id, status FROM orders_opt WHERE user_id = 100;
-- type: ref
-- Extra: Using index （查询列都在索引中，不需要回表）
-- 注意：idx_user_status(user_id, status) 索引包含了 user_id 和 status

-- ===== 示例 5：范围查询 =====
EXPLAIN SELECT * FROM orders_opt WHERE user_id BETWEEN 100 AND 200;
-- type: range
-- key: idx_user_id

-- ===== 示例 6：全表扫描 =====
EXPLAIN SELECT * FROM orders_opt WHERE total_amount > 1000;
-- type: ALL （total_amount 没有索引，全表扫描）
-- key: NULL
-- rows: 数据总行数，性能差

-- ===== 示例 7：Using filesort =====
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100 ORDER BY create_time;
-- Extra 中可能有 Using filesort（排序没有用到索引）

-- ===== 示例 8：Using temporary =====
EXPLAIN SELECT status, COUNT(*) FROM orders_opt GROUP BY status;
-- Extra 中可能有 Using temporary（分组用到了临时表）


-- ---------------------------------------------------------------------
-- 二、索引优化
-- ---------------------------------------------------------------------

-- ===== 1. 最左前缀原则 =====
-- 复合索引按照最左边的列开始匹配
-- 例如：INDEX(a, b, c)
-- 可以用到索引的查询：
--   WHERE a = 1
--   WHERE a = 1 AND b = 2
--   WHERE a = 1 AND b = 2 AND c = 3
--   WHERE a = 1 AND c = 3  (只有 a 会用到索引，c 用不到)
-- 不能用到索引的查询：
--   WHERE b = 2
--   WHERE c = 3
--   WHERE b = 2 AND c = 3

-- 验证：复合索引 idx_user_status(user_id, status)
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100 AND status = 1;
-- type: ref, key: idx_user_status（完整用到索引）

EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100;
-- type: ref, key: idx_user_status（用到了索引的第一列）

EXPLAIN SELECT * FROM orders_opt WHERE status = 1;
-- type: ALL, key: NULL（跳过了第一列 user_id，索引用不上，全表扫描）
-- 注意：如果 status 列有单独索引，则会用那个索引

EXPLAIN SELECT * FROM orders_opt WHERE status = 1 AND user_id = 100;
-- type: ref, key: idx_user_status（MySQL 优化器会调整条件顺序，还是能用索引）

-- ===== 2. 覆盖索引（Covering Index）=====
-- 索引包含了查询需要的所有列，不需要回表查询数据
-- 标识：Extra 中有 "Using index"
-- 优点：减少回表，性能提升明显

-- 普通查询（需要回表）
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100;
-- Extra: NULL（需要回表取其他列）

-- 覆盖索引（不需要回表）
EXPLAIN SELECT user_id, status FROM orders_opt WHERE user_id = 100;
-- Extra: Using index（查询列都在索引中）

-- 优化思路：把经常查询的列加到索引中，变成覆盖索引
-- 但索引列不是越多越好，索引太大会占用更多空间和维护成本

-- ===== 3. 索引失效的常见场景 =====

-- (1) 索引列上使用函数或运算
-- 索引列参与了计算，索引用不上
EXPLAIN SELECT * FROM orders_opt WHERE YEAR(create_time) = 2024;
-- type: ALL，索引失效

-- 优化方式：改成范围查询
EXPLAIN SELECT * FROM orders_opt
WHERE create_time >= '2024-01-01' AND create_time < '2025-01-01';
-- type: range，使用索引

-- (2) 隐式类型转换
-- 索引列是字符串，但查询条件传的是数字（或反之）
-- 例如 order_no 是 VARCHAR 类型
EXPLAIN SELECT * FROM orders_opt WHERE order_no = 12345;
-- type: ALL，发生隐式转换，索引失效

-- 优化：类型保持一致
EXPLAIN SELECT * FROM orders_opt WHERE order_no = 'ORD0000000123';
-- type: const，使用索引

-- (3) 不符合最左前缀原则
-- 前面讲过，跳过了复合索引的左边列，索引失效

-- (4) 使用 LIKE '%xxx' 前缀模糊查询
-- % 在前面无法使用索引（因为索引是按前缀排序的）
EXPLAIN SELECT * FROM orders_opt WHERE order_no LIKE '%123';
-- type: ALL，索引失效

-- 优化：尽量用前缀匹配 LIKE 'xxx%'
EXPLAIN SELECT * FROM orders_opt WHERE order_no LIKE 'ORD0000%';
-- type: range，可以用索引

-- (5) 使用 OR 连接的条件中，有一列没有索引
-- 如果 OR 的两边都有索引，可能会用索引合并（index merge）
-- 如果有一边没有索引，整个查询索引失效
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100 OR total_amount > 1000;
-- type: ALL（total_amount 没有索引，OR 导致整体全表扫描）

-- 优化方式：把 OR 改成 UNION（两边都走索引）
EXPLAIN
SELECT * FROM orders_opt WHERE user_id = 100
UNION ALL
SELECT * FROM orders_opt WHERE total_amount > 1000;
-- 不过 UNION ALL 的第二条还是全表扫描，因为 total_amount 没索引
-- 根本解决：给 total_amount 加索引，或者改写业务逻辑

-- (6) NOT IN / != / <>
-- 这些操作通常会导致索引失效（变成全表扫描或范围扫描）
EXPLAIN SELECT * FROM orders_opt WHERE user_id != 100;
-- type: ALL 或 range，取决于数据分布

-- (7) IS NOT NULL
-- IS NULL 可能用到索引，IS NOT NULL 通常不会用索引（大部分行都不为空时）
EXPLAIN SELECT * FROM orders_opt WHERE pay_time IS NOT NULL;
-- type: ALL，索引失效（大部分数据都不为空，全表扫描更快）

-- (8) 使用 SELECT *
-- 虽然不会直接导致索引失效，但会导致无法使用覆盖索引
-- 建议：明确列出需要的列

-- ===== 4. 索引合并（Index Merge）=====
-- MySQL 可以同时使用多个索引，然后合并结果
-- 常见于 OR 条件（各列有独立索引）
-- Extra 中会显示：Using union(...) 或 Using sort_union(...) 或 Using intersect(...)

-- 示例（需要 status 有独立索引，user_id 有独立索引）
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100 OR status = 1;
-- 可能出现 Using index merge

-- ===== 5. 索引条件下推（ICP - Index Condition Pushdown）=====
-- 将 WHERE 条件中能使用索引的部分下推到存储引擎层
-- 减少回表次数和 Server 层过滤的数据量
-- MySQL 5.6+ 默认开启
-- Extra 中显示：Using index condition

EXPLAIN SELECT * FROM orders_opt WHERE user_id > 100 AND user_id < 200 AND status = 1;
-- Extra: Using index condition（在索引层面就用 status 过滤了一部分）


-- ---------------------------------------------------------------------
-- 三、SQL 优化技巧
-- ---------------------------------------------------------------------

-- ===== 1. 避免 SELECT *，明确查询列 =====
-- 不好：
SELECT * FROM orders_opt WHERE user_id = 100;
-- 好：
SELECT order_id, user_id, order_no, total_amount, status
FROM orders_opt WHERE user_id = 100;
-- 好处：
--   - 可能用到覆盖索引
--   - 减少网络传输数据量
--   - 表结构变更时更安全

-- ===== 2. 用 UNION ALL 代替 OR =====
-- OR 可能导致索引失效，UNION ALL 两边可以分别走索引
-- 前提：两边的条件列都有索引

-- 慢：
EXPLAIN SELECT * FROM orders_opt WHERE user_id = 100 OR create_time > '2024-06-01';

-- 快（两个条件分别走索引，然后合并）：
EXPLAIN
SELECT * FROM orders_opt WHERE user_id = 100
UNION
SELECT * FROM orders_opt WHERE create_time > '2024-06-01';

-- 注意：UNION 会去重，UNION ALL 不去重（更快），根据业务选择

-- ===== 3. 用 JOIN 代替子查询 =====
-- 某些情况下 JOIN 比子查询效率高（MySQL 优化器会自动优化，但不是所有情况）
-- 子查询可能产生临时表，数据量大时性能差

-- 子查询方式：
EXPLAIN SELECT * FROM orders_opt
WHERE user_id IN (SELECT user_id FROM orders_opt WHERE status = 3);

-- JOIN 方式（通常更好）：
EXPLAIN SELECT DISTINCT o1.*
FROM orders_opt o1
JOIN orders_opt o2 ON o1.user_id = o2.user_id
WHERE o2.status = 3;

-- ===== 4. 批量操作代替逐条操作 =====
-- 批量 INSERT / UPDATE / DELETE 比逐条执行快很多
-- 减少网络往返、减少事务开销、减少日志写入

-- 不好（循环执行）：
-- INSERT INTO orders_opt (...) VALUES (...);
-- INSERT INTO orders_opt (...) VALUES (...);
-- INSERT INTO orders_opt (...) VALUES (...);

-- 好（批量）：
-- INSERT INTO orders_opt (...) VALUES (...), (...), (...);

-- ===== 5. 分页优化 =====
-- 深分页问题：LIMIT 100000, 10 会很慢，因为要先扫描 100000 行再丢掉

-- 慢（深分页）：
-- SELECT * FROM orders_opt ORDER BY order_id LIMIT 100000, 10;

-- 优化方式1：记录上一页的最大ID，用 WHERE 过滤
-- SELECT * FROM orders_opt WHERE order_id > 上一页最大ID ORDER BY order_id LIMIT 10;
-- 前提：有自增主键，且按主键排序
-- 适合："加载更多"、"下一页"场景
-- 不适合：跳页场景

-- 优化方式2：子查询先查 ID，再 JOIN 回表
-- SELECT o.* FROM orders_opt o
-- JOIN (SELECT order_id FROM orders_opt ORDER BY order_id LIMIT 100000, 10) t
-- ON o.order_id = t.order_id;
-- 原理：子查询走覆盖索引，比回表扫 100000 行快

-- ===== 6. GROUP BY 优化 =====
-- GROUP BY 默认会排序，如果不需要排序，可以加 ORDER BY NULL
-- 避免 Using filesort

-- 默认会排序：
EXPLAIN SELECT status, COUNT(*) FROM orders_opt GROUP BY status;

-- 不排序（MySQL 8.0 之前有效，8.0 之后 GROUP BY 默认不排序了）：
-- SELECT status, COUNT(*) FROM orders_opt GROUP BY status ORDER BY NULL;

-- 优化：GROUP BY 的列尽量有索引

-- ===== 7. ORDER BY 优化 =====
-- ORDER BY 的列有索引时，可以利用索引的有序性，避免文件排序

-- 有索引的排序：
EXPLAIN SELECT * FROM orders_opt ORDER BY create_time LIMIT 100;
-- 可能走索引，没有 Using filesort

-- 多字段排序要和复合索引顺序一致才能用索引
-- 例如有索引 idx_user_create(user_id, create_time)
-- ORDER BY user_id, create_time  可以用索引
-- ORDER BY create_time, user_id  不能用索引（顺序反了）

-- ===== 8. 避免在 WHERE 中对索引列进行计算/函数操作 =====
-- 前面索引失效部分已讲，这里再强调一次

-- 慢：
-- SELECT * FROM orders_opt WHERE DATE(create_time) = '2024-06-01';

-- 快：
-- SELECT * FROM orders_opt
-- WHERE create_time >= '2024-06-01' AND create_time < '2024-06-02';

-- ===== 9. 用 EXISTS 代替 IN =====
-- 当子查询结果集很大时，EXISTS 通常比 IN 快
-- EXISTS 找到一条匹配就停止，IN 需要把子查询结果全部找出来

-- 子查询结果集大时，EXISTS 更好：
-- SELECT * FROM orders_opt o
-- WHERE EXISTS (SELECT 1 FROM other_table t WHERE t.order_id = o.order_id);

-- 子查询结果集小时，IN 可能更好
-- 根据实际情况选择，MySQL 优化器也会做优化

-- ===== 10. 适当使用 LIMIT =====
-- 只取需要的行数，减少数据传输
-- 特别是在确认数据是否存在时，用 LIMIT 1

-- 判断是否存在：
-- 慢：SELECT COUNT(*) FROM orders_opt WHERE user_id = 100;
-- 快：SELECT 1 FROM orders_opt WHERE user_id = 100 LIMIT 1;


-- ---------------------------------------------------------------------
-- 四、慢查询分析
-- ---------------------------------------------------------------------

-- ===== 1. 开启慢查询日志 =====
-- 慢查询日志会记录执行时间超过指定阈值的 SQL

-- 查看慢查询相关配置
SHOW VARIABLES LIKE 'slow_query%';
SHOW VARIABLES LIKE 'long_query_time';

-- 临时开启（重启后失效）
-- SET GLOBAL slow_query_log = 'ON';
-- SET GLOBAL long_query_time = 1;  -- 超过 1 秒的记录到慢查询日志

-- 永久开启需要修改配置文件 my.cnf / my.ini：
-- [mysqld]
-- slow_query_log = ON
-- slow_query_log_file = /path/to/slow.log
-- long_query_time = 1
-- log_queries_not_using_indexes = ON  -- 没有使用索引的查询也记录（可选）

-- ===== 2. 分析慢查询日志 =====
-- 工具：mysqldumpslow（MySQL 自带）
-- 示例：
-- mysqldumpslow -s t -t 10 /path/to/slow.log  -- 按时间排序，取前10条
-- mysqldumpslow -s c -t 10 /path/to/slow.log  -- 按次数排序，取前10条

-- 第三方工具：pt-query-digest（Percona Toolkit），功能更强大

-- ===== 3. Show Profile =====
-- 查看 SQL 执行各阶段的耗时

-- 开启 profiling
SET profiling = 1;

-- 执行 SQL
SELECT COUNT(*) FROM orders_opt WHERE status = 1;

-- 查看所有查询的 profile
SHOW PROFILES;

-- 查看指定查询的详细耗时（Query_ID 从上面的结果中取）
-- SHOW PROFILE FOR QUERY 1;

-- 查看 CPU、IO 等详细信息
-- SHOW PROFILE CPU, BLOCK IO FOR QUERY 1;

-- 关闭
SET profiling = 0;

-- 注意：MySQL 8.0 已弃用 SHOW PROFILE，推荐用 Performance Schema


-- ---------------------------------------------------------------------
-- 五、分库分表概念
-- ---------------------------------------------------------------------
-- 当单表数据量太大（千万级以上），即使有索引查询也会变慢
-- 分库分表是解决海量数据的方案

-- ===== 1. 垂直分库 =====
-- 按业务模块拆分，不同模块的表放到不同的数据库
-- 例如：用户库、订单库、商品库
-- 目的：减轻单库压力，按业务解耦

-- ===== 2. 垂直分表 =====
-- 将一张大表按列拆分成多张表
-- 例如：用户基本信息表 + 用户详情表（大字段放详情表）
-- 目的：减少单表数据量，常用字段查询更快，冷热数据分离

-- ===== 3. 水平分库 =====
-- 同一张表的数据，按某个规则分散到多个数据库
-- 例如：按用户 ID 取模，user_id % 4 = 0 放库0，=1 放库1...
-- 目的：解决单库数据量太大的问题

-- ===== 4. 水平分表 =====
-- 同一张表的数据，按某个规则分散到同库的多张表
-- 例如：orders_0, orders_1, orders_2, orders_3
-- 目的：解决单表数据量太大的问题

-- ===== 5. 分片策略 =====
-- 常见的分片键（Sharding Key）选择：
--   - RANGE 分片：按范围分，如按时间（每月一张表）、按ID范围
--   - HASH 分片：取模/一致性哈希，数据分布均匀
--   - 按业务维度：如按地区、按租户
--
-- 分片键选择原则：
--   - 尽量让查询只路由到一个分片（避免跨分片查询）
--   - 数据分布均匀
--   - 尽量不变更

-- ===== 6. 分库分表带来的问题 =====
--   - 跨分片 JOIN 困难
--   - 分布式事务（XA 事务、TCC、SAGA 等）
--   - 跨分片分页、排序、聚合
--   - 扩容麻烦（数据迁移）
--   - 全局唯一 ID（雪花算法等）
--
-- 中间件：
--   - ShardingSphere（Sharding-JDBC / Sharding-Proxy）
--   - MyCat
--   - 数据库自带：MySQL Cluster、PostgreSQL Citus 等


-- =====================================================================
-- 练习题：
-- 1. 用 EXPLAIN 分析下面的 SQL，指出问题并优化
--    SELECT * FROM orders WHERE YEAR(create_time) = 2024 AND status = 1;
-- 2. 复合索引 (a, b, c) 哪些查询能用到索引？哪些用不到？
-- 3. 什么是覆盖索引？有什么好处？
-- 4. 列举 5 种索引失效的场景及优化方法
-- 5. 深分页 LIMIT 100000, 10 如何优化？
-- 6. 什么情况下需要考虑分库分表？
-- =====================================================================
