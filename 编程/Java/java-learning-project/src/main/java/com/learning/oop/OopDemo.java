/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.oop;

/**
 * ============================================================
 * OopDemo 类：面向对象编程演示
 *
 * 【面向对象编程（OOP）的四大特性】
 * 1. 封装（Encapsulation）：隐藏内部实现，只暴露必要的接口
 * 2. 继承（Inheritance）：子类继承父类的属性和方法，实现代码复用
 * 3. 多态（Polymorphism）：同一个行为在不同对象上有不同的表现
 * 4. 抽象（Abstraction）：提取共性，忽略细节，用抽象类/接口描述
 *
 * 这个类将逐一演示这些特性以及相关的Java语法
 * ============================================================
 */
public class OopDemo {

    /**
     * 静态成员变量：属于类的变量，所有对象共享
     * 这里用来计数创建了多少个OopDemo对象（演示用）
     */
    private static int demoCount = 0;

    /**
     * main方法：程序入口
     */
    public static void main(String[] args) {
        System.out.println("========== 面向对象编程演示 ==========");

        demonstrateEncapsulation();   // 演示封装
        demonstrateInheritance();     // 演示继承
        demonstratePolymorphism();    // 演示多态
        demonstrateInterface();       // 演示接口
        demonstrateStaticFinal();     // 演示static和final关键字
        demonstrateInnerClass();      // 演示内部类
        demonstrateEnum();            // 演示枚举

        System.out.println("========== 面向对象演示结束 ==========\n");
    }

    /**
     * ============================================================
     * 演示封装特性
     *
     * 【封装（Encapsulation）】
     * - 将类的成员变量隐藏起来（private），不让外部直接访问
     * - 通过公共的方法（getter/setter）来访问和修改
     * - 好处：
     *   1. 提高安全性：可以对数据进行校验
     *   2. 提高复用性：内部实现可以自由修改，不影响外部使用
     *   3. 降低耦合：外部不需要知道内部细节
     *
     * 【四种访问修饰符的可见性】
     *           本类   同包   子类   其他包
     * public     ✓      ✓      ✓      ✓
     * protected  ✓      ✓      ✓      ✗
     * default    ✓      ✓      ✗      ✗   （不写修饰符就是default，又叫包访问权限）
     * private    ✓      ✗      ✗      ✗
     * ============================================================
     */
    public static void demonstrateEncapsulation() {
        System.out.println("\n--- 1. 封装（Encapsulation）---");

        System.out.println("【创建Person对象，演示封装】");
        // 创建一个Person对象，使用构造方法初始化
        Person person = new Person("张三", 25);

        // 通过getter方法获取属性（不能直接访问private的name和age）
        System.out.println("姓名: " + person.getName());
        System.out.println("年龄: " + person.getAge());

        // 通过setter方法修改属性（setter中可以做数据校验）
        person.setAge(30);     // 合法修改
        System.out.println("修改后年龄: " + person.getAge());

        person.setAge(-5);     // 非法修改，setter会拦截
        System.out.println("尝试设为-5后年龄: " + person.getAge() + "（-5无效，年龄不变）");

        // 调用普通方法
        person.introduce();
    }

    /**
     * ============================================================
     * 演示继承特性
     *
     * 【继承（Inheritance）】
     * - 子类继承父类的非私有成员（属性和方法）
     * - 使用extends关键字
     * - Java是单继承，一个类只能有一个直接父类
     * - 所有类都直接或间接继承自Object类
     * - 子类构造方法第一行必须调用父类构造方法（super）
     *
     * 【this 和 super 关键字】
     * - this：代表当前对象的引用
     *   - this.成员变量：访问本类的成员变量
     *   - this.方法()：调用本类的方法
     *   - this()：调用本类的其他构造方法（必须在第一行）
     * - super：代表父类对象的引用
     *   - super.成员变量：访问父类的成员变量
     *   - super.方法()：调用父类的方法
     *   - super()：调用父类的构造方法（必须在第一行）
     * ============================================================
     */
    public static void demonstrateInheritance() {
        System.out.println("\n--- 2. 继承（Inheritance）---");

        System.out.println("【创建Circle对象，演示继承】");
        // 创建一个圆形对象
        Circle circle = new Circle("红色", 5.0);

        // 调用继承自父类的方法
        System.out.println("颜色: " + circle.getColor());
        System.out.println("面积: " + circle.calculateArea());
        System.out.println("周长: " + circle.calculatePerimeter());

        // 调用子类重写的方法
        circle.printInfo();

        System.out.println("\n【创建Rectangle对象】");
        Rectangle rectangle = new Rectangle("蓝色", 4.0, 6.0);
        rectangle.printInfo();

        System.out.println("\n【静态变量：形状总数】");
        // 静态变量属于类，通过类名访问
        System.out.println("总共创建了 " + Shape.getShapeCount() + " 个形状");
    }

    /**
     * ============================================================
     * 演示多态特性
     *
     * 【多态（Polymorphism）】
     * - 同一个行为在不同对象上有不同的表现形式
     * - 多态的三个必要条件：
     *   1. 继承（或接口实现）
     *   2. 方法重写
     *   3. 父类引用指向子类对象（向上转型）
     *
     * 【向上转型和向下转型】
     * - 向上转型（Upcasting）：子类引用 → 父类引用，自动完成，安全
     *   例如：Shape s = new Circle();
     * - 向下转型（Downcasting）：父类引用 → 子类引用，需要强制转换，可能失败
     *   例如：Circle c = (Circle) s;
     *   为了安全，转型前应该用instanceof判断类型
     *
     * 【动态绑定（Dynamic Binding）】
     * - 调用方法时，根据实际对象的类型来决定调用哪个方法
     * - 这是多态的底层实现机制
     * - 注意：只有方法有多态，属性没有多态（属性看引用类型）
     * ============================================================
     */
    public static void demonstratePolymorphism() {
        System.out.println("\n--- 3. 多态（Polymorphism）---");

        System.out.println("【多态数组：父类引用指向子类对象】");
        // 声明一个Shape类型的数组，但里面存的是不同子类的对象
        Shape[] shapes = new Shape[3];
        shapes[0] = new Circle("红色", 3.0);     // 向上转型：Circle → Shape
        shapes[1] = new Rectangle("蓝色", 4.0, 5.0);  // 向上转型：Rectangle → Shape
        shapes[2] = new Circle("绿色", 2.0);

        // 遍历数组，调用每个形状的printInfo方法
        // 虽然都是Shape类型，但实际调用的是各自子类重写后的方法
        System.out.println("\n遍历形状数组，展示多态效果：");
        for (Shape shape : shapes) {
            // 这就是多态：同一个方法调用，不同对象有不同的实现
            shape.printInfo();
            System.out.println();
        }

        System.out.println("【向下转型与instanceof判断】");
        // 向下转型：把父类引用转回子类引用
        // 必须先判断类型，否则可能抛出ClassCastException
        for (Shape shape : shapes) {
            if (shape instanceof Circle) {       // 判断shape是否是Circle类型
                Circle circle = (Circle) shape;  // 向下转型，强制转换
                System.out.println("这是一个圆，半径: " + circle.getRadius());
            } else if (shape instanceof Rectangle) {
                Rectangle rect = (Rectangle) shape;
                System.out.println("这是一个矩形，宽: " + rect.getWidth() + "，高: " + rect.getHeight());
            }
        }
    }

    /**
     * ============================================================
     * 演示接口的使用
     *
     * 【接口（Interface）】
     * - 接口定义了一组行为规范，类通过implements实现接口
     * - 一个类可以实现多个接口（Java是单继承、多实现）
     * - 接口和接口之间可以多继承
     * - Java 8+ 接口可以有默认方法和静态方法
     * ============================================================
     */
    public static void demonstrateInterface() {
        System.out.println("\n--- 4. 接口（Interface）---");

        System.out.println("【接口实现：Dog类实现Animal接口】");
        // 接口也可以实现多态：接口引用指向实现类对象
        Animal dog = new Dog("旺财");
        dog.eat();          // 调用实现类的方法
        dog.makeSound();    // 调用实现类的方法
        dog.sleep();        // 调用接口的默认方法（被重写了）
        dog.introduce();    // 调用接口的默认方法

        System.out.println("\n【接口静态方法】");
        // 接口的静态方法只能通过接口名调用，不能通过对象调用
        Animal.printAnimalTypes();

        System.out.println("\n【接口常量】");
        System.out.println("接口常量: " + Animal.TYPE_MAMMAL);

        System.out.println("\n【多实现：Bird类实现多个接口】");
        // Bird实现了Animal和Flyable两个接口
        Bird bird = new Bird("小鸟");
        bird.eat();
        bird.makeSound();
        bird.fly();         // Flyable接口的方法
    }

    /**
     * ============================================================
     * 演示static和final关键字
     *
     * 【static关键字】
     * - 修饰成员变量：静态变量，属于类，所有对象共享
     * - 修饰方法：静态方法，属于类，可直接通过类名调用
     * - 修饰代码块：静态代码块，类加载时执行一次
     * - 修饰内部类：静态内部类，不依赖外部类对象
     * - 注意：静态方法中不能直接访问非静态成员
     *
     * 【final关键字】
     * - 修饰类：final类，不能被继承
     * - 修饰方法：final方法，不能被重写
     * - 修饰变量：final变量，常量，只能赋值一次
     * - 修饰成员变量：必须在声明时或构造方法中赋值
     * - 修饰局部变量：只能赋值一次
     * - 修饰引用：引用不可变（指向的对象不能变，但对象的内容可以变）
     * ============================================================
     */
    public static void demonstrateStaticFinal() {
        System.out.println("\n--- 5. static 和 final 关键字 ---");

        System.out.println("【静态变量】");
        // 静态变量属于类，所有对象共享同一份
        Person p1 = new Person("张三", 20);
        Person p2 = new Person("李四", 30);
        System.out.println("创建的人数: " + Person.getPersonCount());

        System.out.println("\n【静态方法】");
        // 静态方法通过类名调用，不需要对象
        System.out.println("人类的物种名: " + Person.getSpecies());

        System.out.println("\n【final变量（常量）】");
        // final修饰的变量只能赋值一次
        final int MAX_AGE = 150;
        System.out.println("人类最大年龄常量: " + MAX_AGE);
        // MAX_AGE = 200;  // 编译错误：final变量不能重新赋值

        System.out.println("\n【final引用】");
        // final修饰的引用，指向的对象不能变（不能再指向其他对象）
        // 但对象的内容可以变（对象的属性可以修改）
        final Person finalPerson = new Person("王五", 25);
        finalPerson.setAge(26);  // 可以修改对象的属性
        System.out.println("final引用的对象内容可以修改: " + finalPerson.getAge());
        // finalPerson = new Person("赵六", 30);  // 编译错误：final引用不能重新指向新对象
    }

    /**
     * ============================================================
     * 演示内部类
     *
     * 【内部类（Inner Class）】
     * 定义在另一个类内部的类叫内部类
     *
     * 【内部类的四种类型】
     * 1. 成员内部类（Member Inner Class）：定义在类的成员位置
     *    - 依赖外部类对象，必须先创建外部类对象才能创建
     *    - 可以访问外部类的所有成员（包括private）
     * 2. 静态内部类（Static Inner Class）：用static修饰的成员内部类
     *    - 不依赖外部类对象，可以直接创建
     *    - 只能访问外部类的静态成员
     * 3. 局部内部类（Local Inner Class）：定义在方法内部
     *    - 只在当前方法内可见
     *    - 可以访问外部类的成员和方法中的final局部变量
     * 4. 匿名内部类（Anonymous Inner Class）：没有名字的内部类
     *    - 必须继承一个类或实现一个接口
     *    - 只能使用一次
     *    - 常用于事件监听、线程创建等场景
     * ============================================================
     */
    public static void demonstrateInnerClass() {
        System.out.println("\n--- 6. 内部类（Inner Class）---");

        // --- 成员内部类 ---
        System.out.println("【成员内部类】");
        // 成员内部类必须通过外部类对象来创建
        Outer outer = new Outer("外部类");
        Outer.MemberInner memberInner = outer.new MemberInner();
        memberInner.printInfo();

        // --- 静态内部类 ---
        System.out.println("\n【静态内部类】");
        // 静态内部类不依赖外部类对象，可以直接创建
        Outer.StaticInner staticInner = new Outer.StaticInner();
        staticInner.printInfo();

        // --- 匿名内部类 ---
        System.out.println("\n【匿名内部类】");
        // 匿名内部类：创建一个实现Animal接口的匿名类的对象
        // 语法：new 接口/父类() { 重写方法 }
        Animal cat = new Animal() {
            // 匿名内部类的成员变量
            private String name = "小猫";

            // 实现接口的抽象方法
            @Override
            public void eat() {
                System.out.println(name + "在吃鱼");
            }

            @Override
            public void makeSound() {
                System.out.println(name + "喵喵叫");
            }

            // 也可以重写默认方法
            @Override
            public void sleep() {
                System.out.println(name + "在打呼噜睡觉");
            }
        };

        cat.eat();
        cat.makeSound();
        cat.sleep();
        System.out.println("匿名内部类：不用定义类名，直接创建对象，适合只使用一次的情况");

        // --- 匿名内部类的常见用法：Runnable ---
        System.out.println("\n【匿名内部类常见用法：创建线程】");
        // 创建一个Runnable接口的匿名实现类
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                System.out.println("匿名内部类实现Runnable接口");
            }
        };
        runnable.run();  // 这里只是演示，实际会用Thread来启动
    }

    /**
     * ============================================================
     * 演示枚举类型
     *
     * 【枚举（enum）是什么】
     * - 枚举是一种特殊的类，用于表示一组固定的常量
     * - 用enum关键字定义
     * - 枚举实例必须在枚举类的最前面定义，用逗号分隔，分号结尾
     * - 枚举类的构造方法默认是private的
     * - 所有枚举类都默认继承自java.lang.Enum
     * - 枚举可以有成员变量、方法、构造方法
     * - 枚举天然是线程安全的单例
     *
     * 【枚举的常用方法】
     * - values()：返回所有枚举实例的数组
     * - valueOf(String name)：根据名字获取枚举实例
     * - ordinal()：返回枚举常量的序数（从0开始）
     * - name()：返回枚举常量的名字
     * ============================================================
     */
    public static void demonstrateEnum() {
        System.out.println("\n--- 7. 枚举（Enum）---");

        System.out.println("【枚举的基本使用】");
        // 使用枚举常量
        Season season = Season.SPRING;
        System.out.println("当前季节: " + season);
        System.out.println("季节名字: " + season.getName());
        System.out.println("季节描述: " + season.getDescription());

        // 枚举的ordinal方法（序数，从0开始）
        System.out.println("季节序数: " + season.ordinal());

        System.out.println("\n【遍历所有枚举值】");
        // values()方法获取所有枚举实例
        Season[] seasons = Season.values();
        for (Season s : seasons) {
            System.out.println(s.ordinal() + " - " + s.getName() + ": " + s.getDescription());
        }

        System.out.println("\n【valueOf方法：根据名称获取枚举】");
        // valueOf方法根据字符串获取枚举实例
        Season summer = Season.valueOf("SUMMER");
        System.out.println("valueOf(\"SUMMER\") = " + summer.getName());

        System.out.println("\n【枚举在switch中的使用】");
        // 枚举可以直接用于switch语句
        Season current = Season.AUTUMN;
        switch (current) {
            case SPRING:
                System.out.println("春暖花开，适合踏青");
                break;
            case SUMMER:
                System.out.println("夏日炎炎，适合游泳");
                break;
            case AUTUMN:
                System.out.println("秋高气爽，适合登山");
                break;
            case WINTER:
                System.out.println("白雪皑皑，适合滑雪");
                break;
        }

        System.out.println("\n【枚举实现接口】");
        // 枚举也可以实现接口
        Color color = Color.RED;
        System.out.println("颜色RGB值: " + color.getRgb());
        color.printColorInfo();
    }

    // ============================================================
    // 以下是演示用的内部类和辅助类
    // ============================================================

    /**
     * Person类：用于演示封装
     */
    static class Person {
        // 静态变量：统计创建的人数
        private static int personCount = 0;

        // 静态常量：物种名
        private static final String SPECIES = "智人";

        // 成员变量：用private修饰，实现封装
        private String name;
        private int age;

        // 构造方法
        public Person(String name, int age) {
            this.name = name;
            this.age = age;
            personCount++;  // 每创建一个人，计数器加1
        }

        // getter方法：获取name
        public String getName() {
            return name;
        }

        // setter方法：设置name
        public void setName(String name) {
            this.name = name;
        }

        // getter方法：获取age
        public int getAge() {
            return age;
        }

        // setter方法：设置age（带数据校验）
        public void setAge(int age) {
            if (age > 0 && age < 150) {  // 年龄必须在合理范围内
                this.age = age;
            } else {
                System.out.println("年龄不合法，必须在0-150之间");
            }
        }

        // 普通方法：自我介绍
        public void introduce() {
            System.out.println("大家好，我叫" + name + "，今年" + age + "岁。");
        }

        // 静态方法：获取人数
        public static int getPersonCount() {
            return personCount;
        }

        // 静态方法：获取物种名
        public static String getSpecies() {
            return SPECIES;
        }
    }

    /**
     * Dog类：实现Animal接口
     */
    static class Dog implements Animal {
        private String name;

        public Dog(String name) {
            this.name = name;
        }

        // 实现接口的抽象方法
        @Override
        public void eat() {
            System.out.println(name + "在啃骨头");
        }

        @Override
        public void makeSound() {
            System.out.println(name + "汪汪叫");
        }

        // 重写接口的默认方法（可选）
        @Override
        public void sleep() {
            System.out.println(name + "趴在地上睡觉");
        }

        public String getName() {
            return name;
        }
    }

    /**
     * Flyable接口：会飞的能力
     * 用于演示多实现
     */
    interface Flyable {
        void fly();
    }

    /**
     * Bird类：实现了Animal和Flyable两个接口
     * 演示一个类可以实现多个接口
     */
    static class Bird implements Animal, Flyable {
        private String name;

        public Bird(String name) {
            this.name = name;
        }

        @Override
        public void eat() {
            System.out.println(name + "在吃虫子");
        }

        @Override
        public void makeSound() {
            System.out.println(name + "叽叽喳喳叫");
        }

        @Override
        public void fly() {
            System.out.println(name + "在天空中飞翔");
        }
    }

    /**
     * Outer类：用于演示内部类
     */
    static class Outer {
        private String outerName;
        private static String staticOuterName = "静态外部类变量";

        public Outer(String name) {
            this.outerName = name;
        }

        /**
         * 成员内部类：定义在类的成员位置
         * 可以访问外部类的所有成员（包括private和静态的）
         */
        class MemberInner {
            private String innerName = "成员内部类";

            public void printInfo() {
                System.out.println("我是" + innerName);
                System.out.println("可以访问外部类的实例变量: " + outerName);
                System.out.println("可以访问外部类的静态变量: " + staticOuterName);
            }
        }

        /**
         * 静态内部类：用static修饰的成员内部类
         * 只能访问外部类的静态成员
         */
        static class StaticInner {
            private String innerName = "静态内部类";

            public void printInfo() {
                System.out.println("我是" + innerName);
                // System.out.println(outerName); // 编译错误：不能访问外部类的实例变量
                System.out.println("只能访问外部类的静态变量: " + staticOuterName);
            }
        }
    }

    /**
     * Season枚举：季节枚举
     * 演示枚举的定义和使用
     */
    enum Season {
        // 枚举实例（常量），必须在最前面
        // 每个实例调用构造方法，传入参数
        SPRING("春天", "万物复苏的季节"),
        SUMMER("夏天", "炎热的季节"),
        AUTUMN("秋天", "收获的季节"),
        WINTER("冬天", "寒冷的季节");  // 最后一个用分号结尾

        // 成员变量
        private final String name;
        private final String description;

        // 构造方法：默认就是private的，也只能是private
        // 枚举的构造方法用于创建枚举实例时初始化成员变量
        Season(String name, String description) {
            this.name = name;
            this.description = description;
        }

        // getter方法
        public String getName() {
            return name;
        }

        public String getDescription() {
            return description;
        }
    }

    /**
     * Color枚举：颜色枚举，实现接口
     * 演示枚举也可以实现接口
     */
    enum Color implements ColorInfo {
        RED("红色", "#FF0000"),
        GREEN("绿色", "#00FF00"),
        BLUE("蓝色", "#0000FF");

        private final String name;
        private final String rgb;

        Color(String name, String rgb) {
            this.name = name;
            this.rgb = rgb;
        }

        @Override
        public String getRgb() {
            return rgb;
        }

        @Override
        public void printColorInfo() {
            System.out.println(name + "的RGB值是: " + rgb);
        }

        public String getName() {
            return name;
        }
    }

    /**
     * ColorInfo接口：颜色信息接口
     * 用于演示枚举实现接口
     */
    interface ColorInfo {
        String getRgb();
        void printColorInfo();
    }
}
