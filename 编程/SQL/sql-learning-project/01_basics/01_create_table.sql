-- =====================================================================
-- 01_create_table.sql - 建表语句与数据类型详解
-- 适用数据库：MySQL 8.0+ （部分语法在 PostgreSQL/SQLite 略有差异）
-- =====================================================================

-- ---------------------------------------------------------------------
-- 一、CREATE TABLE 基础语法
-- ---------------------------------------------------------------------
-- CREATE TABLE 用于在数据库中创建一张新表
-- 语法格式：
--   CREATE TABLE [IF NOT EXISTS] 表名 (
--       列名1 数据类型 [约束条件] [注释],
--       列名2 数据类型 [约束条件] [注释],
--       ...
--       [表级约束]
--   ) [表选项];
--
-- IF NOT EXISTS：如果表已存在则不创建，也不报错（避免报错中断）
-- ---------------------------------------------------------------------

-- 示例：创建一张学生表
CREATE TABLE IF NOT EXISTS students (
    -- 列定义：列名 + 数据类型 + 约束 + 注释
    student_id   INT          NOT NULL AUTO_INCREMENT COMMENT '学生ID，主键，自增',
    student_name VARCHAR(50)  NOT NULL                COMMENT '学生姓名，不允许为空',
    gender       CHAR(1)      DEFAULT 'M'             COMMENT '性别：M=男，F=女，默认为男',
    age          INT          NULL                    COMMENT '年龄，允许为空',
    email        VARCHAR(100) UNIQUE                  COMMENT '邮箱，唯一不重复',
    create_time  DATETIME     DEFAULT NOW()           COMMENT '创建时间，默认当前时间',
    -- 表级约束：在列定义之外声明的约束
    PRIMARY KEY (student_id)                          -- 设置 student_id 为主键
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='学生信息表';
-- ENGINE=InnoDB：指定存储引擎为 InnoDB（支持事务和外键）
-- DEFAULT CHARSET=utf8mb4：指定默认字符集为 utf8mb4（支持完整的 Unicode，包括 emoji）
-- COMMENT='...'：给整张表加注释说明


-- ---------------------------------------------------------------------
-- 二、常用数据类型详解
-- ---------------------------------------------------------------------

-- ===== 1. 整数类型 =====
-- TINYINT    : 1字节，范围 -128 ~ 127（有符号）/ 0 ~ 255（无符号），适用于年龄、状态等小数值
-- SMALLINT   : 2字节，范围 -32768 ~ 32767，适用于较小的计数
-- MEDIUMINT  : 3字节，范围约 -838万 ~ 838万
-- INT        : 4字节，范围约 -21亿 ~ 21亿，最常用的整数类型
-- BIGINT     : 8字节，范围极大，适用于主键ID、大数量计数
-- 用法：INT UNSIGNED 表示无符号（只能存正数，范围翻倍）

-- ===== 2. 字符串类型 =====
-- CHAR(n)    : 定长字符串，n 为字符数，最多 255 个字符。
--              特点：存储时固定长度，查询速度快，但浪费空间。适合存长度固定的内容，如手机号、身份证号。
-- VARCHAR(n) : 变长字符串，n 为最大字符数，最多 65535 字节。
--              特点：按实际内容长度存储，节省空间，但速度略慢。适合存姓名、地址等长度变化的内容。
-- TEXT       : 大文本类型，最多 65535 字符。适用于文章内容、备注等长文本。
--              还有 MEDIUMTEXT（16M）、LONGTEXT（4G）更大的版本。
-- ENUM       : 枚举类型，只能从指定值列表中选一个。如 ENUM('男','女','未知')

-- ===== 3. 日期时间类型 =====
-- DATE       : 日期，格式 'YYYY-MM-DD'，如 '2024-01-15'
-- TIME       : 时间，格式 'HH:MM:SS'，如 '14:30:00'
-- DATETIME   : 日期+时间，格式 'YYYY-MM-DD HH:MM:SS'，范围 '1000-01-01' 到 '9999-12-31'
-- TIMESTAMP  : 时间戳，格式同 DATETIME，但范围是 '1970-01-01' 到 '2038-01-19'
--              特点：会随时区变化，存储空间小（4字节），自动更新特性
-- YEAR       : 年份，1字节，存 1901 ~ 2155

-- ===== 4. 数值（小数）类型 =====
-- FLOAT      : 单精度浮点数，4字节，精度约 7 位小数。可能有精度丢失，不适合存金额。
-- DOUBLE     : 双精度浮点数，8字节，精度约 15 位小数。同样可能有精度丢失。
-- DECIMAL(m,d): 定点小数，精确存储。m 是总位数，d 是小数位数。
--              如 DECIMAL(10,2) 表示整数部分最多8位，小数2位。适合存金额、财务数据。

-- ===== 5. 布尔类型 =====
-- BOOLEAN / BOOL : 布尔类型，MySQL 中实际是 TINYINT(1)，0 表示 FALSE，非 0 表示 TRUE

-- ===== 6. 二进制与其他类型 =====
-- BLOB       : 二进制大对象，存图片、文件等二进制数据（一般不推荐直接存数据库）
-- JSON       : JSON 类型，MySQL 5.7+ 支持，可以存 JSON 格式数据并进行查询


-- ---------------------------------------------------------------------
-- 三、约束（Constraints）详解
-- ---------------------------------------------------------------------
-- 约束用于限制表中数据的类型和规则，保证数据的完整性和正确性。

-- ===== 1. PRIMARY KEY 主键约束 =====
-- 作用：唯一标识表中的每一行记录
-- 特点：
--   - 一个表只能有一个主键
--   - 主键值必须唯一，不能重复
--   - 主键不能为 NULL
--   - 主键默认会创建索引，加快查询速度
-- 主键可以是单列主键，也可以是多列组合主键（复合主键）

CREATE TABLE IF NOT EXISTS demo_primary_key (
    id   INT PRIMARY KEY AUTO_INCREMENT,  -- 单列主键，自增
    name VARCHAR(50)
);

-- 复合主键示例（多个字段组合作为唯一标识）
CREATE TABLE IF NOT EXISTS demo_composite_pk (
    user_id    INT NOT NULL,
    product_id INT NOT NULL,
    quantity   INT,
    PRIMARY KEY (user_id, product_id)  -- 两个字段组合作为主键
);

-- ===== 2. AUTO_INCREMENT 自增 =====
-- 作用：自动生成递增的唯一整数，常用于主键ID
-- 特点：
--   - 只能用于数值类型的列
--   - 每个表只能有一个自增列，且必须是键（主键或唯一键）
--   - 默认从 1 开始，每次 +1
--   - 删除记录后，自增计数器不会回退

-- ===== 3. NOT NULL 非空约束 =====
-- 作用：该列的值不允许为 NULL（空值）
-- 示例：name VARCHAR(50) NOT NULL 表示姓名字段必须填写

-- ===== 4. UNIQUE 唯一约束 =====
-- 作用：该列的所有值必须唯一，不能重复
-- 特点：
--   - 一个表可以有多个 UNIQUE 约束
--   - UNIQUE 列允许有 NULL 值（且多个 NULL 不算重复）
--   - 会自动创建唯一索引
-- 示例：email VARCHAR(100) UNIQUE

-- ===== 5. DEFAULT 默认值约束 =====
-- 作用：插入数据时如果不指定该列的值，则使用默认值
-- 示例：
--   status INT DEFAULT 1          状态默认为 1
--   create_time DATETIME DEFAULT NOW()  创建时间默认为当前时间

-- ===== 6. CHECK 检查约束 =====
-- 作用：限制列值的范围，满足条件才能插入
-- 注意：MySQL 8.0.16 之前版本会解析但不强制执行，8.0.16+ 开始支持
-- 示例：
CREATE TABLE IF NOT EXISTS demo_check (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    name    VARCHAR(50) NOT NULL,
    age     INT CHECK (age >= 0 AND age <= 150),   -- 年龄必须在 0~150 之间
    gender  CHAR(1) CHECK (gender IN ('M', 'F'))    -- 性别只能是 M 或 F
);

-- ===== 7. FOREIGN KEY 外键约束 =====
-- 作用：建立两张表之间的关联关系，保证引用完整性
-- 特点：
--   - 外键列的值必须在主表的主键列中存在（或为 NULL）
--   - 防止删除被引用的主表记录
--   - InnoDB 引擎才支持外键
-- 语法：FOREIGN KEY (本表列名) REFERENCES 主表名(主表列名)
--       [ON DELETE 动作] [ON UPDATE 动作]
-- 动作可选：
--   CASCADE  : 主表删除/更新时，从表对应记录也删除/更新
--   SET NULL : 主表删除/更新时，从表对应外键设为 NULL
--   RESTRICT : 禁止删除/更新有引用的主表记录（默认）
--   NO ACTION: 同 RESTRICT

-- 示例：班级表（主表）
CREATE TABLE IF NOT EXISTS classes (
    class_id   INT PRIMARY KEY AUTO_INCREMENT COMMENT '班级ID',
    class_name VARCHAR(50) NOT NULL COMMENT '班级名称'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 示例：学生表（从表，引用班级表）
CREATE TABLE IF NOT EXISTS students_with_fk (
    student_id   INT PRIMARY KEY AUTO_INCREMENT COMMENT '学生ID',
    student_name VARCHAR(50) NOT NULL COMMENT '学生姓名',
    class_id     INT COMMENT '所属班级ID',
    -- 外键约束：class_id 引用 classes 表的 class_id
    FOREIGN KEY (class_id) REFERENCES classes(class_id)
        ON DELETE SET NULL     -- 删除班级时，学生的 class_id 设为 NULL
        ON UPDATE CASCADE      -- 更新班级ID时，学生的 class_id 同步更新
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- ---------------------------------------------------------------------
-- 四、索引（Index）详解
-- ---------------------------------------------------------------------
-- 索引是一种数据结构，用于加快数据查询速度，类似书的目录。
-- 优点：大幅提高查询速度
-- 缺点：占用额外存储空间，插入/更新/删除时需要维护索引，速度变慢

-- ===== 1. 普通索引 INDEX =====
-- 最基本的索引，没有任何限制，只是加快查询速度
-- 可以在建表时创建，也可以后续添加

CREATE TABLE IF NOT EXISTS demo_index (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    name    VARCHAR(50) NOT NULL,
    email   VARCHAR(100),
    age     INT,
    -- 普通索引：加快按姓名查询的速度
    INDEX idx_name (name),
    -- 普通索引：加快按年龄查询的速度
    INDEX idx_age (age)
);

-- ===== 2. 唯一索引 UNIQUE INDEX =====
-- 索引列的值必须唯一，允许 NULL 值（多个 NULL 不算重复）
-- 兼具唯一约束和加速查询的作用

CREATE TABLE IF NOT EXISTS demo_unique_index (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    name    VARCHAR(50),
    email   VARCHAR(100),
    -- 唯一索引：邮箱不能重复，同时加快按邮箱查询
    UNIQUE INDEX uk_email (email)
);

-- ===== 3. 复合索引（联合索引） =====
-- 由多个字段组成的索引，遵循"最左前缀原则"
-- 如 INDEX idx_name_age (name, age)，相当于有了 (name) 和 (name, age) 两个索引

CREATE TABLE IF NOT EXISTS demo_composite_index (
    id          INT PRIMARY KEY AUTO_INCREMENT,
    first_name  VARCHAR(50),
    last_name   VARCHAR(50),
    age         INT,
    -- 复合索引：先按 first_name 排序，再按 last_name 排序
    INDEX idx_name (first_name, last_name)
);

-- ===== 4. 全文索引 FULLTEXT INDEX =====
-- 用于文本内容的关键字搜索，比 LIKE '%xxx%' 效率高很多
-- 只能用于 CHAR、VARCHAR、TEXT 类型的列

CREATE TABLE IF NOT EXISTS demo_fulltext (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    title   VARCHAR(200),
    content TEXT,
    -- 全文索引
    FULLTEXT INDEX ft_title_content (title, content)
);

-- ===== 5. 主键索引 =====
-- 主键自动会创建主键索引（聚簇索引），数据按主键顺序物理存储


-- ---------------------------------------------------------------------
-- 五、ALTER TABLE 修改表结构
-- ---------------------------------------------------------------------
-- ALTER TABLE 用于修改已存在的表结构，包括添加列、删除列、修改列、添加约束等

-- ===== 1. 添加列 ADD COLUMN =====
-- 语法：ALTER TABLE 表名 ADD COLUMN 列名 数据类型 [约束] [位置]
-- 位置可选：FIRST（最前面）、AFTER 某列（指定列之后），默认在最后

ALTER TABLE students ADD COLUMN phone VARCHAR(20) COMMENT '手机号码';
ALTER TABLE students ADD COLUMN address VARCHAR(200) NOT NULL AFTER email;  -- 在 email 列后面添加
ALTER TABLE students ADD COLUMN id_card VARCHAR(18) FIRST;  -- 在最前面添加

-- ===== 2. 删除列 DROP COLUMN =====
-- 语法：ALTER TABLE 表名 DROP COLUMN 列名
-- 注意：删除列会丢失该列的所有数据，谨慎操作！

ALTER TABLE students DROP COLUMN id_card;

-- ===== 3. 修改列 MODIFY COLUMN =====
-- 修改列的数据类型、约束等（不能改列名）
-- 语法：ALTER TABLE 表名 MODIFY COLUMN 列名 新数据类型 [新约束]

ALTER TABLE students MODIFY COLUMN phone VARCHAR(15) DEFAULT '' COMMENT '手机号码（修改后）';

-- ===== 4. 修改列名和类型 CHANGE COLUMN =====
-- 可以同时修改列名和数据类型
-- 语法：ALTER TABLE 表名 CHANGE COLUMN 旧列名 新列名 新数据类型 [约束]

ALTER TABLE students CHANGE COLUMN phone mobile VARCHAR(20) COMMENT '手机号（已改名）';

-- ===== 5. 重命名表 RENAME =====
-- 语法：ALTER TABLE 旧表名 RENAME TO 新表名
-- 或：RENAME TABLE 旧表名 TO 新表名

-- ALTER TABLE students RENAME TO student_info;

-- ===== 6. 添加/删除索引 =====
-- 添加索引
ALTER TABLE students ADD INDEX idx_mobile (mobile);
-- 添加唯一索引
ALTER TABLE students ADD UNIQUE INDEX uk_email (email);
-- 删除索引
ALTER TABLE students DROP INDEX idx_mobile;

-- ===== 7. 添加/删除主键 =====
-- 添加主键（表必须还没有主键）
-- ALTER TABLE demo_table ADD PRIMARY KEY (id);
-- 删除主键（如果主键是自增的，需要先去掉自增属性）
-- ALTER TABLE demo_table DROP PRIMARY KEY;

-- ===== 8. 添加/删除外键 =====
-- 添加外键
-- ALTER TABLE 从表 ADD CONSTRAINT 外键名 FOREIGN KEY (从表列) REFERENCES 主表(主表列);
-- 删除外键
-- ALTER TABLE 从表 DROP FOREIGN KEY 外键名;


-- ---------------------------------------------------------------------
-- 六、DROP TABLE 删除表 / TRUNCATE TABLE 清空表
-- ---------------------------------------------------------------------

-- ===== 1. DROP TABLE 删除表 =====
-- 作用：删除整张表，包括表结构、所有数据、索引、约束等
-- 语法：DROP TABLE [IF EXISTS] 表名 [, 表名2, ...]
-- 注意：删除操作不可恢复，极度危险！

-- DROP TABLE IF EXISTS demo_check;  -- 如果表存在则删除

-- ===== 2. TRUNCATE TABLE 清空表 =====
-- 作用：清空表中所有数据，但保留表结构
-- 语法：TRUNCATE TABLE 表名
-- 特点：
--   - 比 DELETE FROM 表名 速度快很多（因为不逐行删除，直接释放空间）
--   - 不能带 WHERE 条件，只能清空全部
--   - 自增计数器会重置为初始值
--   - 不能回滚（隐式提交事务）
--   - 如果表有外键引用，可能无法 TRUNCATE

-- TRUNCATE TABLE students;

-- ===== 3. DROP / TRUNCATE / DELETE 的区别 =====
--              DROP TABLE        TRUNCATE TABLE      DELETE FROM
--  删除对象     表+数据+结构       全部数据            指定行数据
--  速度         最快               快                  慢（逐行删）
--  能否回滚     不能               不能                可以（事务中）
--  自增重置     -                  重置为初始值        不重置
--  WHERE条件    -                  不支持              支持
--  触发触发器   -                  不触发              触发


-- =====================================================================
-- 练习题：
-- 1. 创建一张图书表 books，包含：图书ID(主键自增)、书名、作者、价格(DECIMAL)、出版日期、简介(TEXT)
-- 2. 给图书表添加一个"分类ID"字段
-- 3. 给图书表的书名字段添加索引
-- 4. 修改简介字段的数据类型为 VARCHAR(500)
-- 5. 删除刚才添加的分类ID字段
-- =====================================================================
