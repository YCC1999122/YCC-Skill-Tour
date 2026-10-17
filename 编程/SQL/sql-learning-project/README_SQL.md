# SQL 学习项目

一个完整的 SQL 学习资源库，从基础语法到高级特性，从数据库设计到性能优化，循序渐进，每一行代码都配有详细的中文注释。

## 目录结构

```
sql-learning-project/
├── 01_basics/              # 基础语法
│   ├── 01_create_table.sql # 建表、数据类型、约束、索引、ALTER
│   └── 02_crud.sql         # 增删改查（INSERT/SELECT/UPDATE/DELETE）
├── 02_queries/             # 查询进阶
│   ├── 01_query_advanced.sql # 复杂条件、聚合、分组、CASE WHEN
│   ├── 02_join.sql         # 表连接（INNER/LEFT/RIGHT/FULL/SELF/CROSS）
│   └── 03_subquery.sql     # 子查询、联合查询、CTE
├── 03_advanced/            # 高级特性
│   ├── 01_functions.sql    # 函数大全（字符串/数值/日期/窗口函数）
│   ├── 02_stored_procedure.sql # 存储过程、函数、触发器、视图、游标
│   └── 03_transaction.sql  # 事务、隔离级别、锁机制、MVCC
├── 04_design/              # 数据库设计
│   └── 01_database_design.sql # 范式、ER模型、表关系、索引设计
├── 05_optimization/        # 性能优化
│   └── 01_performance.sql  # EXPLAIN、索引优化、SQL优化、慢查询
├── 06_examples/            # 综合案例
│   └── 01_ecommerce.sql    # 电商系统完整案例
└── README_SQL.md           # 本文件
```

## 学习路径建议

### 第一阶段：基础入门（01_basics）

先掌握最核心的 SQL 语法，这是一切的基础。

1. **01_create_table.sql** — 学会建表
   - 理解各种数据类型的区别和使用场景
   - 掌握主键、外键、各种约束的作用
   - 了解索引的基本概念
   - 学会用 ALTER 修改表结构

2. **02_crud.sql** — 学会增删改查
   - INSERT：单行插入、批量插入、插入或更新
   - SELECT：WHERE 条件、ORDER BY 排序、LIMIT 分页
   - UPDATE：安全更新的注意事项
   - DELETE：和 TRUNCATE 的区别

### 第二阶段：查询进阶（02_queries）

掌握复杂查询，这是日常开发中用得最多的部分。

1. **01_query_advanced.sql** — 深入查询
   - WHERE 复杂条件组合
   - 聚合函数（COUNT/SUM/AVG/MIN/MAX/GROUP_CONCAT）
   - GROUP BY 分组 + HAVING 过滤
   - CASE WHEN 条件表达式

2. **02_join.sql** — 多表连接
   - INNER JOIN / LEFT JOIN / RIGHT JOIN 的区别
   - 自连接、交叉连接
   - 多表连接技巧
   - ON 和 WHERE 的区别（非常重要！）

3. **03_subquery.sql** — 子查询
   - WHERE / FROM / SELECT 子查询
   - 相关子查询 vs 非相关子查询
   - IN / EXISTS / ANY / ALL
   - UNION / UNION ALL
   - CTE（WITH 子句）

### 第三阶段：高级特性（03_advanced）

深入了解数据库的高级功能。

1. **01_functions.sql** — 函数
   - 字符串函数、数值函数、日期函数
   - 流程控制函数（IF/IFNULL/COALESCE）
   - 窗口函数（MySQL 8.0+ 重点）

2. **02_stored_procedure.sql** — 可编程对象
   - 存储过程和存储函数
   - 触发器
   - 视图
   - 游标

3. **03_transaction.sql** — 事务与锁
   - ACID 四大特性
   - 四种隔离级别
   - 锁机制（行锁/表锁/乐观锁/悲观锁）
   - 死锁与 MVCC

### 第四阶段：数据库设计（04_design）

学会设计合理的数据库表结构。

- 三大范式（1NF/2NF/3NF）和 BCNF
- ER 模型
- 三种表关系（一对一/一对多/多对多）
- 主键、外键、索引设计原则
- 中间表设计
- 反范式化设计

### 第五阶段：性能优化（05_optimization）

掌握 SQL 优化技巧，写出高效的 SQL。

- EXPLAIN 执行计划解读（重中之重）
- 索引优化（最左前缀、覆盖索引、索引失效场景）
- SQL 优化技巧（分页、JOIN、GROUP BY 等）
- 慢查询分析方法
- 分库分表概念

### 第六阶段：综合实战（06_examples）

通过完整案例将所学知识融会贯通。

- 电商系统完整设计
- 用户、商品、订单、购物车等核心表
- 常用业务查询语句
- 下单存储过程（事务 + 扣库存）

## 推荐数据库软件

### 1. MySQL（最推荐入门）

**特点：**
- 最流行的开源关系型数据库
- 社区活跃，资料丰富，遇到问题容易找到答案
- 就业市场需求大
- 本项目所有 SQL 基于 MySQL 8.0 编写

**版本选择：** MySQL 8.0+（支持窗口函数、CTE 等高级特性）

**下载地址：** https://dev.mysql.com/downloads/mysql/

**图形化工具：**
- **Navicat**（收费，功能强大，界面友好）
- **DBeaver**（免费开源，支持多种数据库）
- **MySQL Workbench**（官方免费，功能齐全）
- **DataGrip**（JetBrains 出品，收费，适合 IDEA 用户）

### 2. PostgreSQL

**特点：**
- 功能最强大的开源数据库，被称为"开源界的 Oracle"
- 对 SQL 标准支持最完整
- 支持更多高级特性（更丰富的窗口函数、JSON、全文搜索等）
- 适合学习数据库原理和复杂查询

**适用场景：** 复杂数据分析、GIS、需要高级特性的项目

**下载地址：** https://www.postgresql.org/download/

**图形化工具：** pgAdmin（官方免费）、DBeaver、DataGrip

### 3. SQLite

**特点：**
- 轻量级嵌入式数据库，不需要服务器进程
- 一个文件就是一个数据库，零配置
- 适合移动端、桌面应用、小型项目、学习测试

**适用场景：** 快速原型、移动端、嵌入式设备、学习 SQL 语法

**下载地址：** https://www.sqlite.org/download.html

**图形化工具：** DB Browser for SQLite、DBeaver

### 4. 其他数据库

- **Oracle**：企业级数据库，功能强大但收费，大型传统企业使用较多
- **SQL Server**：微软出品，Windows 生态友好，.NET 技术栈常用
- **Redis**：NoSQL 键值数据库，常做缓存，不算传统关系型数据库
- **MongoDB**：文档型 NoSQL 数据库

## 学习方法建议

### 1. 动手实践，不要光看
SQL 是一门实践性很强的语言，光看语法是学不会的。
- 每学一个知识点，就在数据库中实际执行一遍
- 修改参数，观察结果有什么不同
- 完成每个文件末尾的练习题

### 2. 循序渐进，不要跳级
- 先把基础语法练熟，再学高级特性
- 理解 JOIN 之后再学子查询
- 掌握基本查询后再学优化

### 3. 理解原理，不要死记
- 理解索引为什么能加速（B+树结构）
- 理解事务隔离级别解决什么问题
- 理解范式背后的思想（减少冗余，保证一致性）

### 4. 多做实战项目
- 跟着电商案例走一遍
- 自己设计一个博客系统、图书馆系统等
- 在实际项目中遇到问题再回头查资料

## 注意事项

1. **本项目 SQL 主要针对 MySQL 8.0+**，如果使用其他数据库（PostgreSQL、SQLite 等），部分语法可能略有差异，如：
   - 自增：MySQL 用 AUTO_INCREMENT，PostgreSQL 用 SERIAL 或 GENERATED AS IDENTITY，SQLite 用 AUTOINCREMENT
   - 字符串拼接：MySQL 用 CONCAT()，PostgreSQL 也用 CONCAT() 或 ||，SQL Server 用 +
   - 分页：MySQL 用 LIMIT，PostgreSQL 用 LIMIT / OFFSET，SQL Server 用 TOP 或 OFFSET FETCH
   - 存储过程语法：不同数据库差异较大

2. **执行 SQL 前请确认当前数据库**，避免在生产库上误操作。

3. **UPDATE / DELETE 一定要加 WHERE 条件**，执行前先用 SELECT 确认影响行数。

4. **DROP / TRUNCATE 操作极度危险**，执行前请三思，最好先备份。

## 学习资源推荐

- **MySQL 官方文档**：https://dev.mysql.com/doc/refman/8.0/en/ （最权威）
- **SQLZoo**：https://sqlzoo.net/ （交互式 SQL 练习）
- **LeetCode 数据库专题**：https://leetcode.cn/problemset/database/ （面试刷题）
- **菜鸟教程 SQL 篇**：https://www.runoob.com/sql/sql-tutorial.html （入门教程）

---

祝学习顺利！有问题可以在每个 SQL 文件的练习题部分先自己思考，再对照验证。
