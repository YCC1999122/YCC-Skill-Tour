-- =====================================================================
-- 03_transaction.sql - 事务、隔离级别、锁机制详解
-- 适用数据库：MySQL 8.0+ / InnoDB 引擎
-- =====================================================================
-- 事务（Transaction）是一组 SQL 操作，这些操作要么全部成功，要么全部失败
-- 事务是数据库保证数据一致性和完整性的重要机制
-- =====================================================================


-- ---------------------------------------------------------------------
-- 一、事务的四大特性（ACID）
-- ---------------------------------------------------------------------
--
-- 1. 原子性（Atomicity）
--    事务是一个不可分割的工作单元，事务中的操作要么全部提交成功，要么全部回滚失败
--    实现：通过 undo log（回滚日志）实现，记录修改前的状态，失败时回滚
--
-- 2. 一致性（Consistency）
--    事务执行前后，数据库从一个一致性状态转换到另一个一致性状态
--    例如：转账操作，转账前后总金额不变
--    一致性是事务的最终目的，其他三个特性都是为了保证一致性
--
-- 3. 隔离性（Isolation）
--    多个并发事务之间相互隔离，一个事务的执行不能被其他事务干扰
--    实现：通过锁机制和 MVCC（多版本并发控制）实现
--    隔离级别决定了隔离的程度
--
-- 4. 持久性（Durability）
--    事务一旦提交，对数据的修改就是永久性的，即使系统崩溃也不会丢失
--    实现：通过 redo log（重做日志）实现，修改先写日志，再刷盘
--
-- 总结：
--   原子性 - undo log
--   持久性 - redo log
--   隔离性 - 锁 + MVCC
--   一致性 - 是目的，由前三者共同保证


-- ---------------------------------------------------------------------
-- 二、事务基本操作
-- ---------------------------------------------------------------------

-- ===== 1. 开启事务 =====
-- MySQL 默认自动提交（autocommit=1），每条 SQL 都是一个独立事务
-- 手动控制事务需要先关闭自动提交或显式开启事务

-- 方式1：BEGIN 或 START TRANSACTION
BEGIN;      -- 开启事务
-- 或
START TRANSACTION;

-- 方式2：设置 autocommit = 0
SET autocommit = 0;  -- 关闭自动提交，之后的 SQL 都在同一个事务中

-- ===== 2. 提交事务 =====
COMMIT;  -- 提交事务，所有修改永久生效

-- ===== 3. 回滚事务 =====
ROLLBACK;  -- 回滚事务，撤销所有修改

-- ===== 4. 完整示例：转账 =====

-- 准备演示表
DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    name    VARCHAR(50) NOT NULL,
    balance DECIMAL(10,2) DEFAULT 0 COMMENT '余额'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO accounts (name, balance) VALUES
    ('张三', 10000.00),
    ('李四', 5000.00);

-- 正常情况：张三给李四转 1000 元
BEGIN;
UPDATE accounts SET balance = balance - 1000 WHERE name = '张三';  -- 张三扣 1000
UPDATE accounts SET balance = balance + 1000 WHERE name = '李四';  -- 李四加 1000
-- 检查无误后提交
COMMIT;

-- 查看结果
SELECT * FROM accounts;

-- 异常情况：转账过程中出错，需要回滚
BEGIN;
UPDATE accounts SET balance = balance - 2000 WHERE name = '张三';
-- 模拟出错：比如程序崩溃、网络中断等
-- 此时还没 COMMIT，事务未完成
ROLLBACK;  -- 回滚，张三的钱回到原来的数额

-- 验证回滚结果
SELECT * FROM accounts;


-- ---------------------------------------------------------------------
-- 三、保存点（SAVEPOINT）
-- ---------------------------------------------------------------------
-- 保存点可以将事务分成几个部分，回滚时只回滚到某个保存点，而不是整个事务
-- 语法：
--   SAVEPOINT 保存点名;      -- 创建保存点
--   ROLLBACK TO SAVEPOINT 保存点名;  -- 回滚到指定保存点
--   RELEASE SAVEPOINT 保存点名;      -- 删除保存点

BEGIN;

UPDATE accounts SET balance = balance - 500 WHERE name = '张三';
SAVEPOINT sp1;  -- 保存点1

UPDATE accounts SET balance = balance - 300 WHERE name = '张三';
SAVEPOINT sp2;  -- 保存点2

UPDATE accounts SET balance = balance - 200 WHERE name = '张三';

-- 发现最后一笔有问题，回滚到保存点 sp2（撤销最后 200 的扣款）
ROLLBACK TO SAVEPOINT sp2;

-- 再检查一下，发现第二笔也有问题，回滚到 sp1
ROLLBACK TO SAVEPOINT sp1;

-- 确认无误后提交（只提交保存点之前的操作）
COMMIT;

-- 查看结果：张三应该只扣了 500
SELECT * FROM accounts;


-- ---------------------------------------------------------------------
-- 四、并发事务的问题
-- ---------------------------------------------------------------------
-- 多个事务同时执行时，如果没有适当的隔离，可能出现以下问题：
--
-- 1. 脏读（Dirty Read）
--    事务A读取了事务B未提交的修改，事务B回滚了，事务A读到的就是脏数据
--    例如：B更新了数据但未提交，A读到了更新后的数据，B回滚，A读到的是无效数据
--
-- 2. 不可重复读（Non-Repeatable Read）
--    事务A在同一个事务内两次读取同一行数据，结果不一样
--    原因是两次读取之间，事务B修改了这行数据并提交了
--    重点：UPDATE / DELETE 操作导致
--
-- 3. 幻读（Phantom Read）
--    事务A在同一个事务内两次执行相同的范围查询，第二次查询多了或少了行
--    原因是两次查询之间，事务B插入/删除了满足条件的行
--    重点：INSERT / DELETE 操作导致（行数量变化）
--
-- 4. 丢失更新（Lost Update）
--    两个事务同时修改同一行数据，后提交的覆盖了先提交的修改
--    例如：A和B都读取了同一行，A修改并提交，B也修改并提交，A的修改被覆盖了

-- 总结对比：
--   脏读        读到了未提交的数据
--   不可重复读  同一行数据两次读不一样（UPDATE/DELETE）
--   幻读        同一范围查询两次结果行数不一样（INSERT/DELETE）
--   丢失更新    两个事务同时修改，后者覆盖前者


-- ---------------------------------------------------------------------
-- 五、事务隔离级别
-- ---------------------------------------------------------------------
-- SQL 标准定义了 4 种隔离级别，从低到高：
--
-- 1. READ UNCOMMITTED（读未提交）
--    - 最低隔离级别
--    - 可能出现：脏读、不可重复读、幻读
--    - 实际很少用
--
-- 2. READ COMMITTED（读已提交）
--    - 只能读到已经提交的数据
--    - 解决了脏读
--    - 可能出现：不可重复读、幻读
--    - Oracle、SQL Server 默认级别
--
-- 3. REPEATABLE READ（可重复读）
--    - 同一个事务内，多次读取同一行数据结果一致
--    - 解决了脏读、不可重复读
--    - 可能出现：幻读（MySQL InnoDB 通过 MVCC + Next-Key Lock 基本解决了幻读）
--    - MySQL 默认隔离级别
--
-- 4. SERIALIZABLE（串行化）
--    - 最高隔离级别，所有事务串行执行
--    - 解决了所有问题
--    - 性能最差，并发能力最低
--    - 实际很少用

-- ===== 查看当前隔离级别 =====
SELECT @@transaction_isolation;  -- MySQL 8.0+
-- 或
SELECT @@tx_isolation;  -- MySQL 5.x

-- ===== 设置隔离级别 =====
-- 语法：SET [SESSION|GLOBAL] TRANSACTION ISOLATION LEVEL 级别;

-- 设置当前会话的隔离级别
SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
SET SESSION TRANSACTION ISOLATION LEVEL REPEATABLE READ;
SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE;

-- 查看全局隔离级别
SELECT @@global.transaction_isolation;

-- ===== 各隔离级别与问题对照表 =====
-- 隔离级别             脏读    不可重复读    幻读
-- READ UNCOMMITTED     可能      可能        可能
-- READ COMMITTED       不会      可能        可能
-- REPEATABLE READ      不会      不会        可能(MySQL基本解决)
-- SERIALIZABLE         不会      不会        不会

-- ===== 演示：脏读（需要 READ UNCOMMITTED 级别）=====
-- 连接1：
-- SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
-- BEGIN;
-- UPDATE accounts SET balance = balance + 1000 WHERE name = '张三';  -- 未提交
--
-- 连接2：
-- SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
-- SELECT * FROM accounts WHERE name = '张三';  -- 读到了连接1未提交的数据（脏读）
--
-- 连接1：
-- ROLLBACK;  -- 回滚
--
-- 连接2读到的数据是无效的（脏数据）

-- ===== 演示：不可重复读（需要 READ COMMITTED 级别）=====
-- 连接1：
-- SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
-- BEGIN;
-- SELECT * FROM accounts WHERE name = '张三';  -- 第一次读，假设余额 9500
--
-- 连接2：
-- UPDATE accounts SET balance = balance + 1000 WHERE name = '张三';
-- COMMIT;
--
-- 连接1：
-- SELECT * FROM accounts WHERE name = '张三';  -- 第二次读，余额变了（不可重复读）
-- COMMIT;

-- ===== 演示：幻读 =====
-- 在 REPEATABLE READ 级别下，普通 SELECT 是快照读，不会有幻读
-- 但当前读（SELECT ... FOR UPDATE / INSERT / UPDATE）可能出现幻读
-- MySQL InnoDB 通过 Next-Key Lock（Gap Lock + Record Lock）在一定程度上解决了幻读


-- ---------------------------------------------------------------------
-- 六、锁机制
-- ---------------------------------------------------------------------
-- 锁是实现事务隔离的重要手段
-- 按粒度分类：
--   表锁（Table Lock）  锁定整张表，粒度大，冲突概率高，并发度低
--   行锁（Row Lock）    锁定具体的行，粒度小，冲突概率低，并发度高
--
-- 按类型分类：
--   共享锁（Shared Lock, S锁）  读锁，多个事务可以同时持有
--   排他锁（Exclusive Lock, X锁） 写锁，只能有一个事务持有
--
-- 按思想分类：
--   悲观锁（Pessimistic Locking）  认为并发冲突很常见，操作前先加锁
--   乐观锁（Optimistic Locking）   认为并发冲突不多，操作时检查版本号

-- ===== 1. 表锁 =====
-- 手动加表锁（一般不用，DML 语句自动加行锁）
-- LOCK TABLES 表名 READ/WRITE;
-- UNLOCK TABLES;

-- 读锁（共享锁）：加了读锁后，其他连接可以读但不能写
-- LOCK TABLES accounts READ;
-- SELECT * FROM accounts;  -- 可以读
-- UPDATE accounts SET balance = balance + 100 WHERE id = 1;  -- 会阻塞
-- UNLOCK TABLES;

-- 写锁（排他锁）：加了写锁后，其他连接既不能读也不能写
-- LOCK TABLES accounts WRITE;
-- UNLOCK TABLES;

-- ===== 2. 行锁 =====
-- InnoDB 支持行锁，MyISAM 不支持
-- 行锁是在索引上加的，如果没有索引，会升级为表锁

-- 共享锁（S锁）：SELECT ... LOCK IN SHARE MODE;  -- MySQL 5.x
-- 或：SELECT ... FOR SHARE;  -- MySQL 8.0+
-- 加了 S 锁后，其他事务可以加 S 锁读，但不能加 X 锁写

-- 排他锁（X锁）：SELECT ... FOR UPDATE;
-- 加了 X 锁后，其他事务既不能加 S 锁也不能加 X 锁
-- UPDATE/DELETE/INSERT 语句会自动加 X 锁

-- 示例：悲观锁 - 扣库存场景
BEGIN;
-- 先查库存并加锁（防止其他事务同时修改）
SELECT balance FROM accounts WHERE id = 1 FOR UPDATE;
-- 检查余额是否足够
-- 执行扣款
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
COMMIT;

-- ===== 3. 间隙锁（Gap Lock）=====
-- InnoDB 在 REPEATABLE READ 级别下，为了解决幻读引入了间隙锁
-- 间隙锁锁定的是索引之间的间隙，防止新数据插入
-- Next-Key Lock = Record Lock + Gap Lock（行锁 + 间隙锁）

-- 例如：索引有 1, 5, 10 三个值
-- 间隙有：(-∞,1), (1,5), (5,10), (10, +∞)
-- 如果 WHERE id = 5 FOR UPDATE，会锁定 (1, 10) 这个范围
-- 这样其他事务就无法在这个范围内插入新数据，防止幻读

-- ===== 4. 意向锁（Intention Lock）=====
-- 意向锁是表级锁，用来表示某个事务打算给表中的行加什么锁
-- 意向共享锁（IS）：打算给行加 S 锁
-- 意向排他锁（IX）：打算给行加 X 锁
-- 作用：快速判断表锁和行锁是否冲突，不需要逐行检查


-- ---------------------------------------------------------------------
-- 七、悲观锁 vs 乐观锁
-- ---------------------------------------------------------------------

-- ===== 1. 悲观锁 =====
-- 思想：认为并发冲突很常见，操作数据前先加锁
-- 实现：SELECT ... FOR UPDATE
-- 适用场景：写操作多，冲突概率高的场景
-- 缺点：需要加锁，有性能开销，可能导致死锁

-- 示例：扣减库存
BEGIN;
-- 查询库存并加排他锁
SELECT stock FROM products WHERE id = 1 FOR UPDATE;
-- 检查库存是否充足
-- 扣减库存
UPDATE products SET stock = stock - 1 WHERE id = 1;
COMMIT;

-- ===== 2. 乐观锁 =====
-- 思想：认为并发冲突不多，不加锁，提交时检查是否有冲突
-- 实现：版本号机制或时间戳机制
-- 适用场景：读操作多，冲突概率低的场景
-- 优点：不加锁，性能好
-- 缺点：冲突时需要重试

-- 示例：用版本号实现乐观锁
-- 表中增加 version 字段
-- ALTER TABLE accounts ADD COLUMN version INT DEFAULT 0 COMMENT '版本号';

-- 第一次读取
-- SELECT balance, version FROM accounts WHERE id = 1;
-- 假设读到 balance = 9000, version = 1

-- 更新时带上 version 条件
-- UPDATE accounts SET balance = balance - 100, version = version + 1
-- WHERE id = 1 AND version = 1;
--
-- 如果影响行数为 0，说明 version 变了，有其他事务修改过，需要重试


-- ---------------------------------------------------------------------
-- 八、死锁
-- ---------------------------------------------------------------------
-- 死锁：两个或多个事务互相等待对方释放锁，导致都无法继续执行
--
-- 死锁产生的四个必要条件：
--   1. 互斥条件：锁不能共享，一个资源只能被一个事务持有
--   2. 请求与保持条件：事务持有已有锁的同时，请求新的锁
--   3. 不剥夺条件：已持有的锁不能被强行剥夺，只能自己释放
--   4. 循环等待条件：事务之间形成循环等待链
--
-- 死锁的解决：
--   1. 预防：破坏四个必要条件之一
--      - 按固定顺序访问资源（破坏循环等待）
--      - 一次申请所有需要的锁（破坏请求与保持）
--   2. 检测与恢复：
--      - InnoDB 自动检测死锁，选择一个事务回滚（通常选择代价最小的）
--      - 设置锁等待超时时间（innodb_lock_wait_timeout）
--   3. 优化：
--      - 尽量使用索引，避免全表扫描导致的表锁
--      - 事务尽量小，尽快提交，减少锁持有时间
--      - 避免大事务
--      - 尽量用更低的隔离级别

-- 死锁示例（需要两个连接）：
-- 连接1：
-- BEGIN;
-- UPDATE accounts SET balance = balance + 100 WHERE id = 1;  -- 锁定 id=1
-- UPDATE accounts SET balance = balance + 100 WHERE id = 2;  -- 等待连接2释放 id=2 的锁
--
-- 连接2：
-- BEGIN;
-- UPDATE accounts SET balance = balance + 100 WHERE id = 2;  -- 锁定 id=2
-- UPDATE accounts SET balance = balance + 100 WHERE id = 1;  -- 等待连接1释放 id=1 的锁
--
-- 结果：死锁！InnoDB 会检测到并回滚其中一个事务

-- 查看死锁信息
SHOW ENGINE INNODB STATUS;  -- 查看最近的死锁日志

-- 查看当前锁等待
SELECT * FROM performance_schema.data_locks;
SELECT * FROM performance_schema.data_lock_waits;


-- ---------------------------------------------------------------------
-- 九、MVCC（多版本并发控制）
-- ---------------------------------------------------------------------
-- MVCC（Multi-Version Concurrency Control）是 InnoDB 实现隔离级别的核心机制
-- 基本思想：每行数据都有多个版本，读操作读取合适的版本，写操作创建新版本
-- 好处：读不加锁，读写不冲突，提高并发性能

-- 实现原理（简化版）：
-- 1. 每行数据都有隐藏字段：
--    - DB_TRX_ID：创建/最近修改这行数据的事务ID
--    - DB_ROLL_PTR：回滚指针，指向 undo log 中的旧版本
--
-- 2. undo log（回滚日志）：保存数据的历史版本，形成版本链
--
-- 3. Read View（读视图）：决定事务能看到哪些版本
--    - m_ids：生成 Read View 时活跃的事务ID列表
--    - min_trx_id：最小活跃事务ID
--    - max_trx_id：下一个要分配的事务ID
--    - creator_trx_id：创建 Read View 的事务ID
--
-- 4. 可见性规则（从版本链头开始找）：
--    - 版本的 trx_id < min_trx_id → 可见（事务已经提交了）
--    - 版本的 trx_id >= max_trx_id → 不可见（事务是之后启动的）
--    - 版本的 trx_id 在 m_ids 中 → 不可见（事务还在活跃，未提交）
--    - 版本的 trx_id 不在 m_ids 中且 trx_id < max_trx_id → 可见（事务已提交）
--    - 如果当前版本不可见，就通过 DB_ROLL_PTR 找上一个版本，直到找到可见的

-- 快照读 vs 当前读：
-- - 快照读（一致性非锁定读）：普通 SELECT，读取的是快照版本，不加锁
--   在 READ COMMITTED 级别下，每次 SELECT 都生成新的 Read View
--   在 REPEATABLE READ 级别下，事务中第一个 SELECT 生成 Read View，后续复用
--
-- - 当前读（锁定读）：SELECT ... FOR UPDATE / UPDATE / DELETE / INSERT
--   读取最新版本，并加锁


-- =====================================================================
-- 练习题：
-- 1. 编写一个事务：张三给李四转账 500 元，要求用事务保证原子性
-- 2. 用保存点实现：先扣 100，再扣 200，回滚到扣 100 之后的状态
-- 3. 简述四种隔离级别分别解决了什么问题
-- 4. 悲观锁和乐观锁的区别和适用场景
-- 5. 死锁产生的四个条件是什么，如何预防死锁
-- =====================================================================
