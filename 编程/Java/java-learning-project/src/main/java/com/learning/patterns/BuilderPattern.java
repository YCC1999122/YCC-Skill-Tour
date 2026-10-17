/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

/**
 * ============================================================
 * 建造者模式（Builder Pattern）
 *
 * 【定义】
 * 将一个复杂对象的构建与其表示分离，使得同样的构建过程可以创建不同的表示
 *
 * 【什么时候用建造者模式】
 * - 对象的创建过程很复杂，有很多参数
 * - 参数多，且很多参数有默认值（用构造方法会有很多重载）
 * - 需要创建不可变对象（创建后不能修改）
 * - 想要链式调用，代码更优雅
 *
 * 【常见实现方式】
 * 1. 经典建造者模式：Director + Builder + ConcreteBuilder + Product
 *    - 比较复杂，实际使用较少
 * 2. 简化版（Lombok @Builder风格）：在类内部定义静态内部类Builder
 *    - 更常用，更简洁
 *
 * 【优点】
 * - 封装性好，构建和表示分离
 * - 扩展性好，各个具体建造者相互独立
 * - 可以逐步构建对象，控制构建过程
 * - 链式调用，代码优雅易读
 * - 对象创建后不可变（线程安全）
 *
 * 【缺点】
 * - 会多出一个Builder类
 * - 如果产品内部变化复杂，可能需要很多建造者类
 *
 * 【JDK中的应用】
 * - StringBuilder / StringBuffer（append链式调用）
 * - java.util.stream.Stream.Builder
 * - 各种XxxBuilder类
 *
 * 【和工厂模式的区别】
 * - 工厂模式：关注"创建什么产品"，一步创建
 * - 建造者模式：关注"如何组装产品"，分步构建，关注组装过程
 * ============================================================
 */
public class BuilderPattern {

    /**
     * 产品类：用户
     * 一个复杂对象，有很多属性
     * 构造方法私有，只能通过Builder创建
     */
    static class User {
        // 必填属性
        private final String username;   // 用户名（必填）
        private final String password;   // 密码（必填）

        // 可选属性（有默认值）
        private final String email;      // 邮箱（可选）
        private final String phone;      // 电话（可选）
        private final int age;           // 年龄（可选）
        private final String address;    // 地址（可选）
        private final boolean vip;       // 是否VIP（可选）

        /**
         * 私有构造方法：只能通过Builder创建
         * 从Builder对象中获取所有属性值
         */
        private User(UserBuilder builder) {
            this.username = builder.username;
            this.password = builder.password;
            this.email = builder.email;
            this.phone = builder.phone;
            this.age = builder.age;
            this.address = builder.address;
            this.vip = builder.vip;
        }

        // 只提供getter方法，不提供setter，保证不可变
        public String getUsername() { return username; }
        public String getPassword() { return password; }
        public String getEmail() { return email; }
        public String getPhone() { return phone; }
        public int getAge() { return age; }
        public String getAddress() { return address; }
        public boolean isVip() { return vip; }

        /**
         * 提供一个静态方法，返回Builder对象
         * 方便链式调用的开始
         */
        public static UserBuilder builder() {
            return new UserBuilder();
        }

        @Override
        public String toString() {
            return "User{" +
                    "username='" + username + '\'' +
                    ", password='***'" +
                    ", email='" + email + '\'' +
                    ", phone='" + phone + '\'' +
                    ", age=" + age +
                    ", address='" + address + '\'' +
                    ", vip=" + vip +
                    '}';
        }

        /**
         * 静态内部类：建造者（Builder）
         * 和产品类有相同的属性
         * 每个设置属性的方法都返回this，实现链式调用
         */
        public static class UserBuilder {
            // 和产品类相同的属性
            private String username;
            private String password;
            private String email = "";     // 默认值
            private String phone = "";     // 默认值
            private int age = 0;           // 默认值
            private String address = "";   // 默认值
            private boolean vip = false;   // 默认值

            // 构造方法私有化，通过User.builder()创建
            private UserBuilder() {
            }

            /**
             * 设置用户名
             * 返回this，实现链式调用
             */
            public UserBuilder username(String username) {
                this.username = username;
                return this;
            }

            public UserBuilder password(String password) {
                this.password = password;
                return this;
            }

            public UserBuilder email(String email) {
                this.email = email;
                return this;
            }

            public UserBuilder phone(String phone) {
                this.phone = phone;
                return this;
            }

            public UserBuilder age(int age) {
                this.age = age;
                return this;
            }

            public UserBuilder address(String address) {
                this.address = address;
                return this;
            }

            public UserBuilder vip(boolean vip) {
                this.vip = vip;
                return this;
            }

            /**
             * 构建方法：创建并返回产品对象
             * 可以在这里做参数校验
             */
            public User build() {
                // 必填参数校验
                if (username == null || username.isEmpty()) {
                    throw new IllegalArgumentException("用户名不能为空");
                }
                if (password == null || password.length() < 6) {
                    throw new IllegalArgumentException("密码长度不能少于6位");
                }
                return new User(this);  // 将Builder传给User的私有构造方法
            }
        }
    }

    /**
     * 演示建造者模式
     */
    public static void demonstrate() {
        System.out.println("【建造者模式演示】");

        // --- 完整参数构建 ---
        System.out.println("【完整参数构建用户】");
        User user1 = User.builder()
                .username("zhangsan")
                .password("123456")
                .email("zhangsan@example.com")
                .phone("13800138000")
                .age(25)
                .address("北京市朝阳区")
                .vip(true)
                .build();
        System.out.println(user1);

        // --- 部分参数构建（使用默认值） ---
        System.out.println("\n【部分参数构建（使用默认值）】");
        User user2 = User.builder()
                .username("lisi")
                .password("abcdef")
                .build();  // 只填必填项，可选参数使用默认值
        System.out.println(user2);

        // --- 链式调用的优雅性 ---
        System.out.println("\n【链式调用非常优雅清晰】");
        User user3 = User.builder()
                .username("wangwu")
                .password("password")
                .email("wangwu@test.com")
                .age(30)
                .vip(false)
                .build();
        System.out.println(user3);

        // --- 参数校验 ---
        System.out.println("\n【参数校验演示】");
        try {
            User.builder()
                    .username("")  // 空用户名，会抛异常
                    .password("123")
                    .build();
        } catch (IllegalArgumentException e) {
            System.out.println("捕获异常: " + e.getMessage());
        }

        System.out.println("\n【建造者模式总结】");
        System.out.println("适合：参数多、很多可选参数、需要不可变对象的场景");
        System.out.println("优点：链式调用优雅、可读性好、对象不可变、参数可校验");
        System.out.println("Lombok的@Builder注解就是自动生成这样的建造者代码");
        System.out.println("对比工厂模式：建造者关注构建过程，工厂关注创建结果");
    }
}
