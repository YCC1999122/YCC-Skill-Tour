-- =====================================================================
-- 01_database_design.sql - 数据库设计：范式、ER模型、表关系、索引设计
-- 适用数据库：MySQL 8.0+
-- =====================================================================
-- 数据库设计是软件开发中非常重要的环节，好的设计能：
--   - 减少数据冗余，节省存储空间
--   - 保证数据一致性和完整性
--   - 提高查询效率
--   - 便于维护和扩展
-- =====================================================================


-- ---------------------------------------------------------------------
-- 一、数据库设计流程
-- ---------------------------------------------------------------------
--
-- 1. 需求分析
--    - 了解业务需求，确定需要存储哪些数据
--    - 确定数据之间的关系
--    - 确定数据的访问模式（读多还是写多，查询频率等）
--
-- 2. 概念设计（ER 模型）
--    - 识别实体（Entity）
--    - 识别实体的属性（Attribute）
--    - 识别实体之间的关系（Relationship）
--    - 画出 ER 图
--
-- 3. 逻辑设计
--    - 将 ER 模型转换为关系模式（表结构）
--    - 进行范式化设计
--    - 确定主键、外键
--
-- 4. 物理设计
--    - 选择数据库产品（MySQL / PostgreSQL / Oracle 等）
--    - 确定具体的数据类型
--    - 设计索引
--    - 考虑分区、分表等优化策略
--
-- 5. 实施与维护
--    - 建表、建索引
--    - 导入数据
--    - 监控性能，持续优化


-- ---------------------------------------------------------------------
-- 二、ER 模型（实体-关系模型）
-- ---------------------------------------------------------------------
--
-- ER 模型的三个基本要素：
--
-- 1. 实体（Entity）
--    - 客观存在并可相互区别的事物
--    - 例如：学生、课程、员工、部门、商品、订单
--    - 在 ER 图中用矩形表示
--    - 对应数据库中的表
--
-- 2. 属性（Attribute）
--    - 实体所具有的某一特性
--    - 例如：学生有学号、姓名、性别、年龄等属性
--    - 在 ER 图中用椭圆表示
--    - 对应数据库中的列
--    - 分类：
--      - 简单属性 vs 复合属性（如地址可以拆分为省、市、区）
--      - 单值属性 vs 多值属性（如一个人可能有多个手机号）
--      - 派生属性（如年龄可以由出生日期计算得出）
--
-- 3. 关系（Relationship）
--    - 实体之间的联系
--    - 例如：学生选课程（学生和课程之间的关系）
--    - 在 ER 图中用菱形表示
--    - 对应数据库中的外键或中间表
--
-- 关系的类型（按数量分）：
--   - 一对一（1:1）      一个 A 对应一个 B，一个 B 对应一个 A
--   - 一对多（1:N）      一个 A 对应多个 B，一个 B 对应一个 A
--   - 多对多（M:N）      一个 A 对应多个 B，一个 B 对应多个 A


-- ---------------------------------------------------------------------
-- 三、范式（Normal Form）
-- ---------------------------------------------------------------------
-- 范式是关系数据库设计的规范，用来减少数据冗余，保证数据完整性
-- 级别越高，冗余越少，但查询可能越复杂（需要更多 JOIN）
-- 实际开发中通常达到 3NF 或 BCNF 即可，有时会适当反范式化提高查询性能

-- ===== 第一范式（1NF）：原子性 =====
-- 要求：每一列都是不可再分的原子值
-- 简单说：列不能再拆分成更小的列
--
-- 反例（不满足1NF）：
--   学生表：
--   学号   姓名   联系方式
--   001    张三   手机:13800138000,邮箱:zhangsan@xx.com
--   联系方式这一列包含了手机和邮箱，可以再拆分，不满足 1NF
--
-- 正例（满足1NF）：
--   学号   姓名   手机号           邮箱
--   001    张三   13800138000    zhangsan@xx.com

-- 不满足 1NF 的表设计（反例）
CREATE TABLE IF NOT EXISTS bad_student_1nf (
    student_id INT PRIMARY KEY,
    name       VARCHAR(50),
    contact    VARCHAR(200)  -- 联系方式，包含手机和邮箱，可再分，不满足1NF
);

-- 满足 1NF 的表设计（正例）
CREATE TABLE IF NOT EXISTS good_student_1nf (
    student_id INT PRIMARY KEY,
    name       VARCHAR(50),
    phone      VARCHAR(20),   -- 手机号，原子值
    email      VARCHAR(100)   -- 邮箱，原子值
);

-- ===== 第二范式（2NF）：消除部分依赖 =====
-- 要求：在满足 1NF 的基础上，所有非主键列完全依赖于主键（而不是只依赖主键的一部分）
-- 适用于有复合主键的表
-- 简单说：非主键列必须依赖整个主键，不能只依赖主键的一部分
--
-- 反例（不满足2NF）：
--   选课表（学号, 课程号, 成绩, 课程名称, 学生姓名）
--   主键是 (学号, 课程号) 复合主键
--   问题：
--     - 学生姓名只依赖学号（主键的一部分），不依赖课程号
--     - 课程名称只依赖课程号（主键的一部分），不依赖学号
--     - 只有成绩完全依赖整个主键
--   这就是部分依赖，不满足 2NF
--
-- 正例（满足2NF，拆成三张表）：
--   学生表（学号, 学生姓名）   主键：学号
--   课程表（课程号, 课程名称）  主键：课程号
--   选课表（学号, 课程号, 成绩） 主键：(学号, 课程号)

-- 不满足 2NF 的表设计（反例）
CREATE TABLE IF NOT EXISTS bad_course_2nf (
    student_id   INT,
    course_id    INT,
    score        INT,
    student_name VARCHAR(50),  -- 只依赖 student_id，不依赖 course_id
    course_name  VARCHAR(100), -- 只依赖 course_id，不依赖 student_id
    PRIMARY KEY (student_id, course_id)
);

-- 满足 2NF 的表设计（正例，拆表）
CREATE TABLE IF NOT EXISTS students_2nf (
    student_id   INT PRIMARY KEY,
    student_name VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS courses_2nf (
    course_id   INT PRIMARY KEY,
    course_name VARCHAR(100) NOT NULL
);

CREATE TABLE IF NOT EXISTS student_course_2nf (
    student_id INT,
    course_id  INT,
    score      INT,
    PRIMARY KEY (student_id, course_id),
    FOREIGN KEY (student_id) REFERENCES students_2nf(student_id),
    FOREIGN KEY (course_id)  REFERENCES courses_2nf(course_id)
);

-- ===== 第三范式（3NF）：消除传递依赖 =====
-- 要求：在满足 2NF 的基础上，非主键列不传递依赖于主键
-- 简单说：非主键列只能直接依赖主键，不能依赖其他非主键列
--
-- 反例（不满足3NF）：
--   员工表（员工ID, 姓名, 部门ID, 部门名称, 部门地址）
--   主键是员工ID
--   问题：
--     - 部门名称、部门地址依赖于部门ID
--     - 部门ID依赖于员工ID
--     - 所以 部门名称 传递依赖于 员工ID（员工ID → 部门ID → 部门名称）
--
-- 正例（满足3NF，拆成两张表）：
--   部门表（部门ID, 部门名称, 部门地址）  主键：部门ID
--   员工表（员工ID, 姓名, 部门ID）        主键：员工ID，外键：部门ID

-- 不满足 3NF 的表设计（反例）
CREATE TABLE IF NOT EXISTS bad_emp_3nf (
    emp_id      INT PRIMARY KEY,
    emp_name    VARCHAR(50),
    dept_id     INT,
    dept_name   VARCHAR(50),  -- 依赖 dept_id，传递依赖于主键
    dept_loc    VARCHAR(50)   -- 依赖 dept_id，传递依赖于主键
);

-- 满足 3NF 的表设计（正例，拆表）
CREATE TABLE IF NOT EXISTS departments_3nf (
    dept_id   INT PRIMARY KEY,
    dept_name VARCHAR(50) NOT NULL,
    dept_loc  VARCHAR(50)
);

CREATE TABLE IF NOT EXISTS employees_3nf (
    emp_id   INT PRIMARY KEY,
    emp_name VARCHAR(50) NOT NULL,
    dept_id  INT,
    FOREIGN KEY (dept_id) REFERENCES departments_3nf(dept_id)
);

-- ===== 巴斯-科德范式（BCNF）：消除主属性对主键的部分依赖和传递依赖 =====
-- 3NF 的增强版，也叫 3.5NF
-- 要求：在满足 3NF 的基础上，任何属性（包括主属性）都不传递依赖于主键
-- 简单说：每个决定因素都必须是候选键
--
-- 例子：
--   仓库管理表（仓库ID, 管理员ID, 物品ID, 数量）
--   一个仓库只有一个管理员，一个管理员只管理一个仓库
--   候选键有两个：(仓库ID, 物品ID) 和 (管理员ID, 物品ID)
--   问题：仓库ID → 管理员ID，管理员ID → 仓库ID
--   主属性（仓库ID、管理员ID、物品ID）之间存在依赖，不满足 BCNF
--
-- 拆分为 BCNF：
--   仓库表（仓库ID, 管理员ID） 主键：仓库ID
--   库存表（仓库ID, 物品ID, 数量）主键：(仓库ID, 物品ID)

-- ===== 范式总结 =====
-- 1NF：列不可再分（原子性）
-- 2NF：消除部分依赖（非主键列必须依赖整个主键，适用于复合主键）
-- 3NF：消除传递依赖（非主键列只能直接依赖主键）
-- BCNF：消除主属性对主键的部分/传递依赖
--
-- 范式化程度越高，数据冗余越少，数据一致性越好，但查询可能需要更多 JOIN
-- 实际开发中，通常做到 3NF 即可，有时会适当反范式化（加冗余字段）来优化查询性能


-- ---------------------------------------------------------------------
-- 四、常见表关系
-- ---------------------------------------------------------------------

-- ===== 1. 一对一（1:1）=====
-- 描述：A 表中的一条记录对应 B 表中的一条记录，反之亦然
-- 实现方式：
--   - 外键 + 唯一约束：在一张表中设外键，外键列加 UNIQUE
--   - 主键关联：两张表共享同一个主键（B 表的主键同时是外键）
--
-- 使用场景：
--   - 垂直分表：把不常用的大字段拆分出去（如用户基本信息 + 用户详情）
--   - 安全考虑：敏感字段单独放一张表

-- 示例：用户表 + 用户详情表（一对一）
CREATE TABLE IF NOT EXISTS users_1to1 (
    user_id   INT PRIMARY KEY AUTO_INCREMENT,
    username  VARCHAR(50) NOT NULL UNIQUE,
    password  VARCHAR(100) NOT NULL,
    email     VARCHAR(100),
    create_time DATETIME DEFAULT NOW()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户表（主表）';

CREATE TABLE IF NOT EXISTS user_profiles_1to1 (
    user_id   INT PRIMARY KEY,  -- 主键同时是外键
    real_name VARCHAR(50),
    id_card   VARCHAR(18),
    address   VARCHAR(200),
    avatar    VARCHAR(200),
    bio       TEXT,
    FOREIGN KEY (user_id) REFERENCES users_1to1(user_id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户详情表（从表，一对一）';

-- ===== 2. 一对多（1:N）=====
-- 描述：A 表中的一条记录对应 B 表中的多条记录，B 表中的一条记录只对应 A 表中的一条记录
-- 实现方式：在多的一方（B 表）添加外键，指向一的一方（A 表）的主键
-- 这是最常见的关系

-- 示例：部门表 + 员工表（一对多：一个部门有多个员工，一个员工属于一个部门）
CREATE TABLE IF NOT EXISTS depts_1ton (
    dept_id   INT PRIMARY KEY AUTO_INCREMENT,
    dept_name VARCHAR(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='部门表（一的一方）';

CREATE TABLE IF NOT EXISTS emps_1ton (
    emp_id    INT PRIMARY KEY AUTO_INCREMENT,
    emp_name  VARCHAR(50) NOT NULL,
    salary    DECIMAL(10,2),
    dept_id   INT,  -- 外键，在多的一方
    FOREIGN KEY (dept_id) REFERENCES depts_1ton(dept_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='员工表（多的一方）';

-- ===== 3. 多对多（M:N）=====
-- 描述：A 表中的一条记录对应 B 表中的多条记录，B 表中的一条记录也对应 A 表中的多条记录
-- 实现方式：需要一张中间表（关联表/连接表），中间表有两个外键，分别指向两张主表
-- 中间表的主键可以是两个外键的组合（复合主键）

-- 示例：学生表 + 课程表 + 选课中间表（多对多：一个学生选多门课，一门课有多个学生选）
CREATE TABLE IF NOT EXISTS students_mton (
    student_id   INT PRIMARY KEY AUTO_INCREMENT,
    student_name VARCHAR(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='学生表';

CREATE TABLE IF NOT EXISTS courses_mton (
    course_id   INT PRIMARY KEY AUTO_INCREMENT,
    course_name VARCHAR(100) NOT NULL,
    credit      INT DEFAULT 0 COMMENT '学分'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='课程表';

-- 中间表（选课表）
CREATE TABLE IF NOT EXISTS student_course_mton (
    student_id INT NOT NULL,
    course_id  INT NOT NULL,
    score      INT COMMENT '成绩',
    enroll_time DATETIME DEFAULT NOW(),
    PRIMARY KEY (student_id, course_id),  -- 复合主键
    FOREIGN KEY (student_id) REFERENCES students_mton(student_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses_mton(course_id)
        ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='选课表（中间表）';

-- 查询：张三选了哪些课
-- SELECT s.student_name, c.course_name, sc.score
-- FROM student_course_mton sc
-- JOIN students_mton s ON sc.student_id = s.student_id
-- JOIN courses_mton c ON sc.course_id = c.course_id
-- WHERE s.student_name = '张三';


-- ---------------------------------------------------------------------
-- 五、主键设计
-- ---------------------------------------------------------------------
-- 主键（Primary Key）是表中每行记录的唯一标识
-- 设计原则：
--   1. 唯一性：主键值必须唯一，不能重复
--   2. 非空性：主键不能为 NULL
--   3. 稳定性：主键值尽量不要变更
--   4. 简洁性：主键尽量简单，占用空间小

-- ===== 1. 自然主键 vs 代理主键 =====
--
-- 自然主键（Natural Key）：
--   - 使用业务中本身就有的唯一标识作为主键
--   - 例如：身份证号、学号、工号、邮箱
--   - 优点：有业务含义
--   - 缺点：可能变化（如工号调整）、可能比较长、可能复合
--
-- 代理主键（Surrogate Key / 逻辑主键）：
--   - 专门为了做主键而添加的、没有业务含义的字段
--   - 例如：自增 ID、UUID
--   - 优点：稳定、简单、不随业务变化
--   - 缺点：多了一列，没有业务含义
--
-- 推荐：实际开发中优先使用代理主键（自增 INT/BIGINT）

-- ===== 2. 自增主键（AUTO_INCREMENT）=====
-- 优点：简单、性能好、按顺序插入（B+树索引分裂少）
-- 缺点：
--   - 分布式环境下不好用（多个数据库实例会冲突）
--   - 数据量特别大时可能不够用（INT 最大约 21 亿）
-- 建议：
--   - 一般表用 INT 即可（21 亿足够大多数场景）
--   - 超大表用 BIGINT
--   - 分布式系统可以用雪花算法（Snowflake）生成分布式 ID

-- ===== 3. UUID 主键 =====
-- 优点：全局唯一，适合分布式
-- 缺点：
--   - 占用空间大（36 字符或 16 字节二进制）
--   - 无序，插入时索引频繁分裂，性能差
--   - 没有业务含义
-- 优化：可以用 UUID v1 变体或有序 UUID（如 MySQL 8.0 的 UUID_TO_BIN 函数）

-- ===== 4. 复合主键 =====
-- 由多个字段组成的主键
-- 适用于：中间表（多对多关系表）、关联表
-- 注意：
--   - 复合主键中的每个字段都不能为 NULL
--   - 组合后必须唯一
--   - 作为外键引用时比较麻烦（需要多个字段）


-- ---------------------------------------------------------------------
-- 六、外键设计
-- ---------------------------------------------------------------------
-- 外键（Foreign Key）用于建立和加强两个表之间的链接
-- 设计原则：
--   1. 外键列的数据类型必须和主键列一致
--   2. 外键列建议建索引（MySQL 会自动建，但其他数据库不一定）
--   3. 合理选择 ON DELETE / ON UPDATE 动作

-- ON DELETE / ON UPDATE 选项：
--   CASCADE    : 主表删除/更新时，从表对应记录也删除/更新
--   SET NULL   : 主表删除/更新时，从表外键设为 NULL（外键列必须允许 NULL）
--   RESTRICT   : 禁止删除/更新有从表记录的主表记录（默认）
--   NO ACTION  : 同 RESTRICT
--   SET DEFAULT: 设为默认值（InnoDB 不支持）

-- 实际开发中关于外键的争议：
-- 支持用外键：
--   - 保证数据一致性，防止脏数据
--   - 数据库层面保证，比应用层更可靠
-- 反对用外键：
--   - 影响性能（每次插入/更新都要检查外键约束）
--   - 分库分表时外键无法使用
--   - 增加死锁概率
--   - 数据迁移麻烦
-- 建议：
--   - 中小项目、数据一致性要求高的场景可以用外键
--   - 大型互联网项目通常不用外键，在应用层保证数据一致性


-- ---------------------------------------------------------------------
-- 七、索引设计原则
-- ---------------------------------------------------------------------
-- 索引是提高查询性能的重要手段，但也不是越多越好
-- 索引的代价：
--   - 占用磁盘空间
--   - 插入/更新/删除时需要维护索引，降低写性能
-- 设计原则：

-- ===== 1. 适合建索引的场景 =====
--   - WHERE 条件中经常出现的列
--   - JOIN 连接中用到的列（外键列）
--   - ORDER BY / GROUP BY 中用到的列
--   - 区分度高的列（如手机号、邮箱，值不重复的比例高）
--   - 经常需要排序、分组的列

-- ===== 2. 不适合建索引的场景 =====
--   - 数据量很小的表（几十/几百行，全表扫描更快）
--   - 区分度低的列（如性别、状态，只有少数几个值）
--   - 频繁更新的列（维护索引代价大）
--   - 不会出现在 WHERE/JOIN/ORDER BY 中的列

-- ===== 3. 索引设计技巧 =====
--
-- (1) 优先考虑复合索引，遵循最左前缀原则
--     例如 INDEX(a, b, c) 相当于有了 INDEX(a), INDEX(a,b), INDEX(a,b,c)
--     把最常用的查询条件放在最左边
--
-- (2) 覆盖索引（Covering Index）
--     索引中包含了查询需要的所有列，不需要回表查询
--     例如：SELECT name, age FROM users WHERE email = 'xxx';
--     建索引 INDEX(email, name, age) 就是覆盖索引
--
-- (3) 前缀索引
--     对于长字符串列，可以只索引前 N 个字符，节省索引空间
--     例如：INDEX(email(10)) - 只索引邮箱前10个字符
--     注意：前缀索引用不了 ORDER BY 和 GROUP BY
--
-- (4) 唯一索引 vs 普通索引
--     业务上唯一的列建议建唯一索引（既加速查询又保证数据唯一性）
--
-- (5) 索引列尽量不要参与运算
--     WHERE YEAR(create_time) = 2024 会导致索引失效
--     改写成 WHERE create_time >= '2024-01-01' AND create_time < '2025-01-01'
--
-- (6) 避免索引失效的场景（详见 05_optimization/01_performance.sql）

-- ===== 4. 索引数量控制 =====
--   - 单表索引数量建议不超过 5-6 个
--   - 删除没用的索引（定期检查）


-- ---------------------------------------------------------------------
-- 八、中间表设计
-- ---------------------------------------------------------------------
-- 中间表（也叫关联表、连接表）用于实现多对多关系

-- 设计要点：
-- 1. 表名：通常由两张主表的名字组合而成，如 user_role、student_course
-- 2. 列：至少包含两个外键列，分别指向两张主表
-- 3. 主键：
--    - 复合主键（两个外键组合）
--    - 或单独加一个代理主键（自增ID）
-- 4. 可以添加额外的业务字段（如创建时间、状态、其他属性）
-- 5. 两个外键都要建索引

-- 示例：用户-角色中间表（多对多）
CREATE TABLE IF NOT EXISTS user_role (
    id          INT PRIMARY KEY AUTO_INCREMENT COMMENT '代理主键',
    user_id     INT NOT NULL COMMENT '用户ID',
    role_id     INT NOT NULL COMMENT '角色ID',
    create_time DATETIME DEFAULT NOW() COMMENT '分配时间',
    create_by   INT COMMENT '创建人',
    -- 唯一约束：一个用户不能重复分配同一个角色
    UNIQUE KEY uk_user_role (user_id, role_id),
    -- 外键
    FOREIGN KEY (user_id) REFERENCES users_1to1(user_id) ON DELETE CASCADE,
    FOREIGN KEY (role_id) REFERENCES roles_design(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户-角色关联表';

-- 角色表（配合上面的中间表使用）
CREATE TABLE IF NOT EXISTS roles_design (
    id          INT PRIMARY KEY AUTO_INCREMENT,
    role_name   VARCHAR(50) NOT NULL UNIQUE COMMENT '角色名称',
    description VARCHAR(200) COMMENT '角色描述'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='角色表';


-- ---------------------------------------------------------------------
-- 九、反范式化设计
-- ---------------------------------------------------------------------
-- 范式化减少冗余，但有时查询需要 JOIN 多张表，影响性能
-- 反范式化：适当增加冗余字段，减少 JOIN，提高查询性能
-- 本质：以空间换时间

-- 常见的反范式化手段：
--
-- 1. 冗余字段
--    例如：订单表中冗余存储用户名称、商品名称（虽然可以 JOIN 用户表/商品表得到）
--    好处：查询订单列表时不需要 JOIN 用户表和商品表
--    代价：数据更新时需要同时更新冗余字段，可能不一致
--
-- 2. 汇总字段
--    例如：用户表中加一个"订单总数"字段，订单表中加一个"商品数量"字段
--    好处：统计时不需要 COUNT()/SUM()
--    代价：需要维护，插入/删除时要更新
--
-- 3. 水平分表/垂直分表（详见优化篇）

-- 示例：订单表（反范式化设计）
CREATE TABLE IF NOT EXISTS orders_denorm (
    order_id      BIGINT PRIMARY KEY,
    user_id       BIGINT NOT NULL COMMENT '用户ID',
    user_name     VARCHAR(50) COMMENT '用户名（冗余）',  -- 冗余字段
    total_amount  DECIMAL(12,2) NOT NULL COMMENT '订单总金额',
    status        TINYINT DEFAULT 0 COMMENT '订单状态',
    create_time   DATETIME DEFAULT NOW()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单表（反范式化）';
-- 查询订单列表时，直接能看到用户名，不需要 JOIN 用户表
-- 但如果用户改了名字，需要同时更新订单表中的冗余用户名

-- 反范式化的取舍：
--   - 读多写少的场景适合反范式化
--   - 一致性要求高的场景慎用
--   - 冗余字段尽量选更新频率低的


-- =====================================================================
-- 练习题：
-- 1. 设计一个简单的图书管理系统，画出 ER 图并写出建表 SQL
--    实体：图书、读者、借阅记录
--    关系：一个读者可以借多本书，一本书可以被多人借阅（不同时间）
-- 2. 指出下面的表设计不满足第几范式，如何修改？
--    表：订单（订单号, 客户名, 客户地址, 商品名, 商品价格, 数量, 下单时间）
-- 3. 设计一个博客系统的表结构（用户、文章、评论、分类）
-- 4. 简述索引设计的基本原则
-- 5. 什么情况下适合使用反范式化设计？
-- =====================================================================
