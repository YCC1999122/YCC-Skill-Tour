/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.advanced;

/**
 * ============================================================
 * 导入需要的类
 * ============================================================
 */
import java.io.*;
import java.lang.annotation.*;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.time.*;
import java.time.format.DateTimeFormatter;
import java.time.temporal.ChronoUnit;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * ============================================================
 * AdvancedDemo 类：Java高级特性演示
 *
 * 包含内容：
 * 1. 异常处理
 * 2. 注解
 * 3. 反射
 * 4. Lambda表达式与函数式接口
 * 5. Optional类
 * 6. 日期时间API（Java 8+）
 * 7. 序列化
 * ============================================================
 */
public class AdvancedDemo {

    public static void main(String[] args) {
        System.out.println("========== Java高级特性演示 ==========");

        demonstrateException();       // 演示异常处理
        demonstrateAnnotation();      // 演示注解
        demonstrateReflection();      // 演示反射
        demonstrateLambda();          // 演示Lambda表达式
        demonstrateOptional();        // 演示Optional类
        demonstrateDateTime();        // 演示日期时间API
        demonstrateSerialization();   // 演示序列化

        System.out.println("========== 高级特性演示结束 ==========\n");
    }

    /**
     * ============================================================
     * 演示异常处理
     *
     * 【异常是什么】
     * - 程序运行时出现的不正常情况（不是编译错误）
     * - Java用异常类来描述各种异常情况
     * - 所有异常都是Throwable的子类
     *
     * 【Throwable体系】
     * Throwable
     * ├── Error：严重错误，程序无法处理（如OOM、StackOverflowError）
     * └── Exception：异常，程序可以处理
     *     ├── RuntimeException：运行时异常，不强制处理（如NullPointerException）
     *     └── 受检异常（Checked Exception）：编译时必须处理（如IOException、SQLException）
     *
     * 【异常处理的五个关键字】
     * try：监控可能发生异常的代码
     * catch：捕获并处理异常
     * finally：无论是否发生异常都会执行（通常用于释放资源）
     * throw：手动抛出一个异常
     * throws：声明方法可能抛出的异常
     *
     * 【处理异常的两种方式】
     * 1. 捕获处理：try-catch-finally，自己处理
     * 2. 向上抛出：throws，交给调用者处理
     * ============================================================
     */
    public static void demonstrateException() {
        System.out.println("\n--- 1. 异常处理 ---");

        // --- try-catch 基本用法 ---
        System.out.println("【try-catch 基本用法】");
        try {
            // 可能发生异常的代码放在try块中
            int result = 10 / 0;  // 除以0，会抛出ArithmeticException
            System.out.println("这行不会执行");
        } catch (ArithmeticException e) {
            // catch块捕获并处理异常
            System.out.println("捕获到算术异常: " + e.getMessage());
            // e.printStackTrace();  // 打印异常堆栈信息（调试用）
        }

        // --- 多重catch ---
        System.out.println("\n【多重catch】");
        try {
            String str = null;
            System.out.println(str.length());  // 空指针异常
        } catch (NullPointerException e) {
            System.out.println("捕获到空指针异常: " + e.getMessage());
        } catch (Exception e) {
            // 多个catch时，从上到下匹配，子类异常要写在前面
            System.out.println("捕获到其他异常: " + e.getMessage());
        }

        // --- try-catch-finally ---
        System.out.println("\n【try-catch-finally】");
        System.out.println("finally块无论是否发生异常都会执行");
        FileReader reader = null;
        try {
            // 模拟读取文件（这里故意用不存在的文件）
            reader = new FileReader("nonexistent.txt");
        } catch (FileNotFoundException e) {
            System.out.println("文件不存在: " + e.getMessage());
        } finally {
            // finally通常用于释放资源（关闭流、释放锁等）
            System.out.println("finally块执行：释放资源");
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    System.out.println("关闭流失败");
                }
            }
        }

        // --- try-with-resources（Java 7+ 推荐用法）---
        System.out.println("\n【try-with-resources 自动关闭资源】");
        // 实现了AutoCloseable接口的资源可以放在try后面的括号里
        // 会自动调用close()方法，不需要手动写finally
        try (StringWriter writer = new StringWriter()) {
            writer.write("Hello");
            System.out.println("写入内容: " + writer.toString());
        } catch (IOException e) {
            System.out.println("IO异常: " + e.getMessage());
        }
        // 不需要写finally，资源会自动关闭

        // --- throw 和 throws ---
        System.out.println("\n【throw 手动抛出异常】");
        try {
            checkAge(15);  // 年龄不够，会抛出异常
        } catch (IllegalArgumentException e) {
            System.out.println("捕获异常: " + e.getMessage());
        }

        System.out.println("\n【throws 声明异常】");
        System.out.println("throws在方法声明处声明可能抛出的异常");
        System.out.println("受检异常必须处理（捕获或继续抛出）");
        System.out.println("运行时异常可以不声明，但建议声明");

        // --- 自定义异常 ---
        System.out.println("\n【自定义异常】");
        try {
            register("张三", 15);  // 年龄不够，抛出自定义异常
        } catch (AgeException e) {
            System.out.println("捕获自定义异常: " + e.getMessage());
            System.out.println("错误码: " + e.getErrorCode());
        }

        System.out.println("\n【异常处理最佳实践】");
        System.out.println("1. 不要捕获Exception或Throwable，要捕获具体异常");
        System.out.println("2. 不要吞掉异常（catch块空着），至少要打印日志");
        System.out.println("3. 尽量使用try-with-resources管理资源");
        System.out.println("4. 自定义异常要有业务含义，不要滥用");
        System.out.println("5. 异常信息要详细，方便定位问题");
    }

    /**
     * 检查年龄
     * 使用throw手动抛出异常
     *
     * @param age 年龄
     */
    public static void checkAge(int age) {
        if (age < 18) {
            // throw：手动抛出一个异常对象
            throw new IllegalArgumentException("年龄必须大于等于18岁，当前年龄: " + age);
        }
        System.out.println("年龄合法");
    }

    /**
     * 注册方法
     * throws声明该方法可能抛出AgeException
     * 调用者需要处理这个异常
     *
     * @param name 姓名
     * @param age  年龄
     * @throws AgeException 如果年龄不合法
     */
    public static void register(String name, int age) throws AgeException {
        if (age < 18) {
            // 抛出自定义异常
            throw new AgeException("注册失败，年龄不足18岁", 1001);
        }
        System.out.println(name + "注册成功");
    }

    /**
     * 自定义异常类：年龄异常
     * 继承自Exception（受检异常），也可以继承RuntimeException（运行时异常）
     */
    static class AgeException extends Exception {
        // 自定义错误码
        private final int errorCode;

        public AgeException(String message, int errorCode) {
            super(message);  // 调用父类构造方法
            this.errorCode = errorCode;
        }

        public int getErrorCode() {
            return errorCode;
        }
    }

    /**
     * ============================================================
     * 演示注解（Annotation）
     *
     * 【注解是什么】
     * - 注解是代码上的标记/元数据，提供额外信息
     * - 注解不会直接影响代码的执行，但可以被其他程序读取
     * - 注解可以用在类、方法、字段、参数等上面
     *
     * 【常见的内置注解】
     * @Override：标记方法是重写父类的
     * @Deprecated：标记已过时，不推荐使用
     * @SuppressWarnings：抑制编译器警告
     * @FunctionalInterface：标记函数式接口（Java 8+）
     *
     * 【元注解（Meta Annotation）】
     * 用来定义注解的注解
     * 1. @Target：注解可以用在什么地方（类、方法、字段等）
     * 2. @Retention：注解的生命周期（SOURCE/CLASS/RUNTIME）
     * 3. @Documented：注解是否包含在Javadoc中
     * 4. @Inherited：注解是否可以被继承
     * 5. @Repeatable：注解是否可重复（Java 8+）
     *
     * 【自定义注解的步骤】
     * 1. 用@interface定义注解
     * 2. 用元注解配置注解的适用范围和生命周期
     * 3. 定义注解的属性（方法形式，可有默认值）
     * ============================================================
     */
    public static void demonstrateAnnotation() {
        System.out.println("\n--- 2. 注解（Annotation）---");

        System.out.println("【常见内置注解】");
        System.out.println("@Override：标记重写方法");
        System.out.println("@Deprecated：标记已过时");
        System.out.println("@SuppressWarnings：抑制警告");
        System.out.println("@FunctionalInterface：函数式接口");

        // --- 使用自定义注解 ---
        System.out.println("\n【自定义注解的使用】");
        // 使用我们自定义的@MyAnnotation注解
        MyClass obj = new MyClass();
        System.out.println("类名: " + obj.getClass().getSimpleName());

        // --- 通过反射读取注解（在反射部分会详细讲） ---
        System.out.println("\n【通过反射读取注解信息】");
        Class<?> clazz = MyClass.class;

        // 读取类上的注解
        if (clazz.isAnnotationPresent(MyAnnotation.class)) {
            MyAnnotation annotation = clazz.getAnnotation(MyAnnotation.class);
            System.out.println("类上的注解：");
            System.out.println("  value = " + annotation.value());
            System.out.println("  description = " + annotation.description());
            System.out.println("  version = " + annotation.version());
        }

        // 读取方法上的注解
        try {
            Method method = clazz.getMethod("myMethod");
            if (method.isAnnotationPresent(MyAnnotation.class)) {
                MyAnnotation annotation = method.getAnnotation(MyAnnotation.class);
                System.out.println("方法上的注解：");
                System.out.println("  value = " + annotation.value());
                System.out.println("  description = " + annotation.description());
            }
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        System.out.println("\n【注解的应用场景】");
        System.out.println("1. 编译检查：@Override、@SuppressWarnings");
        System.out.println("2. 框架配置：Spring的@Component、@Autowired");
        System.out.println("3. 单元测试：JUnit的@Test");
        System.out.println("4. 代码生成：Lombok的@Data、@Getter");
        System.out.println("5. 文档生成：@Documented");
    }

    /**
     * 自定义注解：MyAnnotation
     *
     * @interface 用来定义注解
     */
    @Target({ElementType.TYPE, ElementType.METHOD, ElementType.FIELD})  // 可以用在类、方法、字段上
    @Retention(RetentionPolicy.RUNTIME)  // 运行时保留，可以通过反射读取
    @Documented  // 会生成到Javadoc中
    @interface MyAnnotation {
        // 注解的属性：看起来像方法
        // 使用时：@MyAnnotation(value="xxx", description="yyy")
        // 如果只有value属性，可以省略value=

        String value();  // value属性（名字特殊，只有它时可以省略名字）

        String description() default "默认描述";  // 带默认值的属性

        int version() default 1;  // 版本号，默认1
    }

    /**
     * 演示用的类：使用了自定义注解
     */
    @MyAnnotation(value = "我的类", description = "这是一个演示注解的类", version = 2)
    static class MyClass {

        @MyAnnotation("我的方法")  // 只有value属性，可以省略value=
        public void myMethod() {
            System.out.println("这是一个使用了注解的方法");
        }
    }

    /**
     * ============================================================
     * 演示反射（Reflection）
     *
     * 【反射是什么】
     * - 在运行时动态获取类的信息、操作对象的能力
     * - 正常情况：知道类名 → new 对象 → 调用方法
     * - 反射情况：运行时才知道类名 → 获取Class对象 → 操作属性和方法
     * - 反射是很多框架的基础（Spring、MyBatis等）
     *
     * 【Class类是什么】
     * - 类加载到内存后，JVM会为每个类创建一个Class对象
     * - Class对象包含了类的所有信息（属性、方法、构造方法等）
     * - Class对象是反射的入口
     *
     * 【获取Class对象的三种方式】
     * 1. 类名.class：编译期就可以获取，最常用
     * 2. 对象.getClass()：已有对象时使用
     * 3. Class.forName("全类名")：运行时动态加载，最灵活
     *
     * 【反射能做什么】
     * 1. 获取类的信息（类名、包、父类、接口等）
     * 2. 操作构造方法（创建对象）
     * 3. 操作方法（调用方法）
     * 4. 操作字段（读取/修改属性值）
     * 5. 读取注解
     *
     * 【反射的优缺点】
     * 优点：灵活，动态性强，是框架的基础
     * 缺点：性能较差，破坏封装（可以访问private成员），增加复杂度
     * ============================================================
     */
    public static void demonstrateReflection() {
        System.out.println("\n--- 3. 反射（Reflection）---");

        // --- 获取Class对象的三种方式 ---
        System.out.println("【获取Class对象的三种方式】");
        try {
            // 方式1：类名.class（最常用，最安全）
            Class<?> clazz1 = String.class;
            System.out.println("方式1 - String.class: " + clazz1.getName());

            // 方式2：对象.getClass()
            String str = "hello";
            Class<?> clazz2 = str.getClass();
            System.out.println("方式2 - 对象.getClass(): " + clazz2.getName());

            // 方式3：Class.forName("全类名")（最灵活，动态加载）
            Class<?> clazz3 = Class.forName("java.lang.String");
            System.out.println("方式3 - Class.forName(): " + clazz3.getName());

            // 三个都是同一个Class对象
            System.out.println("三个是否相同: " + (clazz1 == clazz2 && clazz2 == clazz3));
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        // --- 获取类的基本信息 ---
        System.out.println("\n【获取类的基本信息】");
        Class<Person> personClass = Person.class;
        System.out.println("类名: " + personClass.getName());
        System.out.println("简单类名: " + personClass.getSimpleName());
        System.out.println("包名: " + personClass.getPackage().getName());
        System.out.println("父类: " + personClass.getSuperclass().getSimpleName());
        System.out.println("是否是接口: " + personClass.isInterface());

        // --- 通过反射创建对象 ---
        System.out.println("\n【通过反射创建对象】");
        try {
            // 方式1：调用无参构造方法
            Person p1 = personClass.newInstance();  // Java 9已过时，推荐用下面的方式
            System.out.println("无参构造创建: " + p1.getName());

            // 方式2：调用有参构造方法（推荐）
            Constructor<Person> constructor = personClass.getConstructor(String.class, int.class);
            Person p2 = constructor.newInstance("李四", 25);
            System.out.println("有参构造创建: " + p2.getName() + ", " + p2.getAge() + "岁");
        } catch (Exception e) {
            e.printStackTrace();
        }

        // --- 通过反射操作方法 ---
        System.out.println("\n【通过反射调用方法】");
        try {
            Person person = new Person("张三", 20);

            // 获取public方法
            Method introduceMethod = personClass.getMethod("introduce");
            // 调用方法：第一个参数是对象，后面是方法参数
            introduceMethod.invoke(person);  // 相当于 person.introduce()

            // 获取有参数的方法
            Method setAgeMethod = personClass.getMethod("setAge", int.class);
            setAgeMethod.invoke(person, 30);  // 相当于 person.setAge(30)
            System.out.println("反射调用setAge后: " + person.getAge() + "岁");

            // 调用private方法（需要setAccessible(true)，破坏封装）
            Method privateMethod = personClass.getDeclaredMethod("privateMethod");
            privateMethod.setAccessible(true);  // 允许访问私有方法
            privateMethod.invoke(person);

        } catch (Exception e) {
            e.printStackTrace();
        }

        // --- 通过反射操作字段 ---
        System.out.println("\n【通过反射操作字段】");
        try {
            Person person = new Person("王五", 28);

            // 获取public字段（如果有的话）
            // getField只能获取public的，包括继承的
            // getDeclaredField可以获取本类所有的，包括private的，但不包括继承的

            // 获取private字段name
            Field nameField = personClass.getDeclaredField("name");
            nameField.setAccessible(true);  // 允许访问私有字段

            // 获取字段值
            String nameValue = (String) nameField.get(person);
            System.out.println("反射获取name: " + nameValue);

            // 修改字段值
            nameField.set(person, "赵六");
            System.out.println("反射修改name后: " + person.getName());

        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("\n【反射的应用场景】");
        System.out.println("1. 框架：Spring IOC（依赖注入）、MyBatis（ORM映射）");
        System.out.println("2. 动态代理：JDK动态代理");
        System.out.println("3. 注解处理：读取注解信息");
        System.out.println("4. JSON/XML序列化：反射读取对象属性");
        System.out.println("5. 单元测试：JUnit");
    }

    /**
     * Person类：用于反射演示
     */
    static class Person {
        // 私有字段
        private String name;
        private int age;

        // 无参构造方法
        public Person() {
            this.name = "默认";
            this.age = 0;
        }

        // 有参构造方法
        public Person(String name, int age) {
            this.name = name;
            this.age = age;
        }

        // public方法
        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        public void introduce() {
            System.out.println("大家好，我是" + name + "，今年" + age + "岁");
        }

        // private方法
        private void privateMethod() {
            System.out.println("这是一个private方法，被反射调用了");
        }
    }

    /**
     * ============================================================
     * 演示Lambda表达式与函数式接口
     *
     * 【Lambda表达式是什么】
     * - Java 8引入的新特性，支持函数式编程
     * - 本质是一个匿名函数（没有名字的方法）
     * - 可以把代码像数据一样传递
     * - 格式：(参数列表) -> { 方法体 }
     *
     * 【函数式接口（Functional Interface）】
     * - 只有一个抽象方法的接口（可以有多个默认方法、静态方法）
     * - Lambda表达式就是函数式接口的实现
     * - @FunctionalInterface注解可以用来标记（可选，用于编译检查）
     *
     * 【Java 8内置的四大函数式接口】
     * 1. Consumer<T>：消费型接口，有参数无返回值  void accept(T t)
     * 2. Supplier<T>：供给型接口，无参数有返回值  T get()
     * 3. Function<T, R>：函数型接口，有参数有返回值  R apply(T t)
     * 4. Predicate<T>：断言型接口，有参数返回boolean  boolean test(T t)
     *
     * 【方法引用（Method Reference）】
     * - Lambda表达式的简化写法
     * - 当Lambda体只有一个方法调用时，可以用方法引用
     * - 四种形式：
     *   1. 对象::实例方法
     *   2. 类::静态方法
     *   3. 类::实例方法
     *   4. 类::new（构造方法引用）
     * ============================================================
     */
    public static void demonstrateLambda() {
        System.out.println("\n--- 4. Lambda表达式与函数式接口 ---");

        System.out.println("【Lambda表达式语法】");
        System.out.println("格式：(参数列表) -> { 方法体 }");
        System.out.println("简化：");
        System.out.println("  - 参数类型可以省略（编译器可以推断）");
        System.out.println("  - 只有一个参数时，小括号可以省略");
        System.out.println("  - 方法体只有一条语句时，大括号可以省略");
        System.out.println("  - 方法体只有一条return语句时，return可以省略");

        // --- Runnable（无参无返回值） ---
        System.out.println("\n【Runnable：无参无返回值】");
        // 匿名内部类方式
        Runnable r1 = new Runnable() {
            @Override
            public void run() {
                System.out.println("匿名内部类方式");
            }
        };
        r1.run();

        // Lambda方式
        Runnable r2 = () -> System.out.println("Lambda方式");
        r2.run();

        // --- Consumer（消费型接口） ---
        System.out.println("\n【Consumer<T> 消费型接口：有参数无返回值】");
        Consumer<String> consumer = s -> System.out.println("消费: " + s);
        consumer.accept("Hello Lambda");

        // --- Supplier（供给型接口） ---
        System.out.println("\n【Supplier<T> 供给型接口：无参数有返回值】");
        Supplier<Integer> supplier = () -> (int) (Math.random() * 100);
        System.out.println("生成随机数: " + supplier.get());
        System.out.println("生成随机数: " + supplier.get());

        // --- Function（函数型接口） ---
        System.out.println("\n【Function<T, R> 函数型接口：有参数有返回值】");
        Function<String, Integer> function = s -> s.length();
        Integer length = function.apply("Hello");
        System.out.println("字符串长度: " + length);

        // --- Predicate（断言型接口） ---
        System.out.println("\n【Predicate<T> 断言型接口：返回boolean】");
        Predicate<Integer> predicate = n -> n > 0;
        System.out.println("5 > 0: " + predicate.test(5));
        System.out.println("-3 > 0: " + predicate.test(-3));

        // --- 自定义函数式接口 ---
        System.out.println("\n【自定义函数式接口】");
        // 使用自定义的Calculator接口
        Calculator add = (a, b) -> a + b;
        Calculator sub = (a, b) -> a - b;
        System.out.println("加法: 3 + 5 = " + add.calculate(3, 5));
        System.out.println("减法: 10 - 4 = " + sub.calculate(10, 4));

        // --- 方法引用 ---
        System.out.println("\n【方法引用 Method Reference】");
        // 对象::实例方法
        String str = "Hello";
        Supplier<Integer> lenSupplier = str::length;  // 等价于 () -> str.length()
        System.out.println("字符串长度: " + lenSupplier.get());

        // 类::静态方法
        Function<Integer, String> intToStr = String::valueOf;  // 等价于 n -> String.valueOf(n)
        System.out.println("int转String: " + intToStr.apply(123));

        // 类::new（构造方法引用）
        Supplier<Person> personSupplier = Person::new;  // 等价于 () -> new Person()
        Person p = personSupplier.get();
        System.out.println("构造方法引用创建: " + p.getName());

        System.out.println("\n【Lambda的应用场景】");
        System.out.println("1. Stream API（大量使用函数式接口）");
        System.out.println("2. 事件监听（GUI、Android等）");
        System.out.println("3. 线程/异步任务");
        System.out.println("4. 简化匿名内部类的写法");
    }

    /**
     * 自定义函数式接口：计算器
     *
     * @FunctionalInterface 注解用来标记函数式接口
     * 编译器会检查接口是否只有一个抽象方法
     */
    @FunctionalInterface
    interface Calculator {
        // 只有一个抽象方法
        int calculate(int a, int b);

        // 可以有默认方法
        default void printInfo() {
            System.out.println("这是一个计算器接口");
        }
    }

    /**
     * ============================================================
     * 演示Optional类
     *
     * 【Optional是什么】
     * - Java 8引入的容器类，用来优雅地处理null值
     * - Optional可以包含一个值，也可以为空
     * - 目的：避免NullPointerException（NPE），让代码更优雅
     *
     * 【创建Optional的方式】
     * 1. Optional.of(T)：创建非空的Optional（如果为null会抛异常）
     * 2. Optional.ofNullable(T)：创建可空的Optional（null也可以）
     * 3. Optional.empty()：创建空的Optional
     *
     * 【常用方法】
     * - isPresent()：判断是否有值
     * - ifPresent(Consumer)：如果有值，就执行消费操作
     * - get()：获取值（没有值会抛异常，不推荐）
     * - orElse(T)：如果有值返回值，否则返回默认值
     * - orElseGet(Supplier)：如果有值返回值，否则由Supplier提供默认值
     * - orElseThrow(Supplier)：如果有值返回值，否则抛出异常
     * - map(Function)：如果有值，对值做转换（返回新的Optional）
     * - flatMap(Function)：和map类似，但Function返回的是Optional
     * - filter(Predicate)：如果有值且满足条件，返回原值，否则返回空
     * ============================================================
     */
    public static void demonstrateOptional() {
        System.out.println("\n--- 5. Optional 类 ---");

        System.out.println("【创建Optional】");
        // of：值不能为null，否则抛NullPointerException
        Optional<String> opt1 = Optional.of("Hello");
        System.out.println("of创建: " + opt1);

        // ofNullable：值可以为null
        Optional<String> opt2 = Optional.ofNullable(null);
        System.out.println("ofNullable(null)创建: " + opt2);

        // empty：空的Optional
        Optional<String> opt3 = Optional.empty();
        System.out.println("empty创建: " + opt3);

        // --- 判断是否有值 ---
        System.out.println("\n【判断是否有值】");
        System.out.println("opt1.isPresent(): " + opt1.isPresent());
        System.out.println("opt2.isPresent(): " + opt2.isPresent());

        // --- 获取值 ---
        System.out.println("\n【获取值的方式】");
        // get()：不推荐，没有值会抛异常
        System.out.println("opt1.get(): " + opt1.get());

        // orElse：有值返回值，没值返回默认值
        System.out.println("opt2.orElse(\"默认值\"): " + opt2.orElse("默认值"));

        // orElseGet：有值返回值，没值由Supplier提供
        System.out.println("opt2.orElseGet(() -> \"计算默认值\"): "
                + opt2.orElseGet(() -> "计算默认值"));

        // orElseThrow：有值返回值，没值抛异常
        try {
            opt2.orElseThrow(() -> new RuntimeException("值不存在"));
        } catch (RuntimeException e) {
            System.out.println("orElseThrow捕获异常: " + e.getMessage());
        }

        // --- ifPresent ---
        System.out.println("\n【ifPresent：有值才执行】");
        opt1.ifPresent(s -> System.out.println("有值: " + s));
        opt2.ifPresent(s -> System.out.println("这行不会执行"));

        // --- map 和 flatMap（转换） ---
        System.out.println("\n【map：转换值】");
        // 如果有值，对值进行转换，返回新的Optional
        Optional<Integer> lenOpt = opt1.map(String::length);
        System.out.println("opt1.map(String::length): " + lenOpt.orElse(0));

        Optional<Integer> lenOpt2 = opt2.map(String::length);
        System.out.println("opt2.map(String::length): " + lenOpt2.orElse(0));

        // --- filter（过滤） ---
        System.out.println("\n【filter：过滤】");
        Optional<String> filtered = opt1.filter(s -> s.length() > 3);
        System.out.println("opt1.filter(s -> s.length()>3): " + filtered.isPresent());

        Optional<String> filtered2 = opt1.filter(s -> s.length() > 10);
        System.out.println("opt1.filter(s -> s.length()>10): " + filtered2.isPresent());

        // --- 链式调用（优雅处理嵌套null） ---
        System.out.println("\n【链式调用：优雅处理嵌套null】");
        // 传统写法需要层层判空
        // String city = person.getAddress().getCity();
        // 可能会NPE，需要写很多if判断

        // Optional链式调用
        Person person = new Person("张三", 25);
        String city = Optional.ofNullable(person)
                .map(Person::getName)  // 如果person不为空，获取name
                .map(String::toUpperCase)  // 转大写
                .orElse("未知");
        System.out.println("链式调用结果: " + city);

        System.out.println("\n【Optional的使用建议】");
        System.out.println("1. 不要在字段上使用Optional（序列化问题）");
        System.out.println("2. 不要在集合元素中使用Optional");
        System.out.println("3. 主要用于方法返回值，表示可能为null");
        System.out.println("4. 不要用Optional作为方法参数");
    }

    /**
     * ============================================================
     * 演示Java 8+日期时间API
     *
     * 【为什么用新的日期时间API】
     * - 旧的Date、Calendar有很多问题：
     *   - 线程不安全（SimpleDateFormat线程不安全）
     *   - 设计混乱（Date既是日期又是时间）
     *   - 月份从0开始（1月是0，容易出错）
     * - Java 8引入了全新的日期时间API（JSR 310）
     *   - 线程安全（不可变对象）
     *   - 设计清晰，API丰富
     *   - 在java.time包下
     *
     * 【核心类】
     * - LocalDate：日期（年-月-日）
     * - LocalTime：时间（时:分:秒）
     * - LocalDateTime：日期+时间
     * - Instant：时间戳（精确到纳秒）
     * - Duration：时间段（基于时间）
     * - Period：时间段（基于日期）
     * - ZoneId：时区
     * - ZonedDateTime：带时区的日期时间
     * - DateTimeFormatter：格式化/解析日期时间（线程安全）
     * ============================================================
     */
    public static void demonstrateDateTime() {
        System.out.println("\n--- 6. 日期时间API（Java 8+）---");

        // --- LocalDate：日期 ---
        System.out.println("【LocalDate 日期】");
        LocalDate today = LocalDate.now();  // 当前日期
        System.out.println("今天日期: " + today);
        System.out.println("年: " + today.getYear());
        System.out.println("月: " + today.getMonthValue());  // 月份从1开始（不像旧的Date从0开始）
        System.out.println("日: " + today.getDayOfMonth());
        System.out.println("星期: " + today.getDayOfWeek());
        System.out.println("这一年的第几天: " + today.getDayOfYear());

        // 指定日期
        LocalDate birthday = LocalDate.of(2000, 1, 1);
        System.out.println("指定日期: " + birthday);

        // 日期运算
        System.out.println("加10天: " + today.plusDays(10));
        System.out.println("减2周: " + today.minusWeeks(2));
        System.out.println("加3个月: " + today.plusMonths(3));
        System.out.println("加1年: " + today.plusYears(1));

        // --- LocalTime：时间 ---
        System.out.println("\n【LocalTime 时间】");
        LocalTime now = LocalTime.now();
        System.out.println("当前时间: " + now);
        System.out.println("时: " + now.getHour());
        System.out.println("分: " + now.getMinute());
        System.out.println("秒: " + now.getSecond());
        System.out.println("纳秒: " + now.getNano());

        LocalTime time = LocalTime.of(14, 30, 45);
        System.out.println("指定时间: " + time);

        // --- LocalDateTime：日期+时间 ---
        System.out.println("\n【LocalDateTime 日期时间】");
        LocalDateTime dateTime = LocalDateTime.now();
        System.out.println("当前日期时间: " + dateTime);

        LocalDateTime specificDT = LocalDateTime.of(2025, 6, 1, 12, 0, 0);
        System.out.println("指定日期时间: " + specificDT);

        // --- DateTimeFormatter：格式化与解析 ---
        System.out.println("\n【DateTimeFormatter 格式化与解析】");
        // 格式化：日期 → 字符串
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formatted = dateTime.format(formatter);
        System.out.println("格式化后: " + formatted);

        // 解析：字符串 → 日期
        String dateStr = "2025-06-01 12:00:00";
        LocalDateTime parsed = LocalDateTime.parse(dateStr, formatter);
        System.out.println("解析后: " + parsed);

        // --- Duration 和 Period ---
        System.out.println("\n【Duration 和 Period（时间段）】");
        // Duration：基于时间的时间段（秒、纳秒）
        LocalTime start = LocalTime.of(9, 0);
        LocalTime end = LocalTime.of(17, 30);
        Duration duration = Duration.between(start, end);
        System.out.println("工作时长: " + duration.toHours() + "小时" + duration.toMinutesPart() + "分钟");

        // Period：基于日期的时间段（年、月、日）
        LocalDate startDate = LocalDate.of(2020, 1, 1);
        LocalDate endDate = LocalDate.of(2025, 6, 1);
        Period period = Period.between(startDate, endDate);
        System.out.println("相差: " + period.getYears() + "年" + period.getMonths() + "月" + period.getDays() + "日");

        // --- ChronoUnit 计算 ---
        System.out.println("\n【ChronoUnit 计算时间差】");
        long daysBetween = ChronoUnit.DAYS.between(startDate, endDate);
        System.out.println("相差天数: " + daysBetween + "天");

        long monthsBetween = ChronoUnit.MONTHS.between(startDate, endDate);
        System.out.println("相差月数: " + monthsBetween + "个月");

        // --- Instant：时间戳 ---
        System.out.println("\n【Instant 时间戳】");
        Instant instant = Instant.now();
        System.out.println("当前时间戳: " + instant);
        System.out.println("秒数: " + instant.getEpochSecond());
        System.out.println("毫秒数: " + instant.toEpochMilli());

        System.out.println("\n【新API vs 旧API】");
        System.out.println("新API是不可变对象，线程安全");
        System.out.println("DateTimeFormatter是线程安全的（SimpleDateFormat不是）");
        System.out.println("月份从1开始，更符合直觉");
        System.out.println("API更丰富、更易用");
    }

    /**
     * ============================================================
     * 演示序列化
     *
     * 【什么是序列化/反序列化】
     * - 序列化：将Java对象转换为字节流的过程（对象 → 字节）
     * - 反序列化：将字节流恢复为Java对象的过程（字节 → 对象）
     * - 目的：对象可以保存到文件、数据库，或在网络中传输
     *
     * 【如何实现序列化】
     * - 类实现Serializable接口（标记接口，没有方法）
     * - 使用ObjectOutputStream.writeObject()序列化
     * - 使用ObjectInputStream.readObject()反序列化
     *
     * 【serialVersionUID】
     * - 序列化版本号，用于验证序列化和反序列化的类是否一致
     * - 如果不显示指定，编译器会自动生成一个（根据类的结构）
     * - 类结构改变（加字段、改方法等）会导致serialVersionUID变化
     * - 反序列化时，如果serialVersionUID不一致，会抛出InvalidClassException
     * - 建议：显示指定serialVersionUID，避免类修改后反序列化失败
     *
     * 【transient关键字】
     * - 修饰成员变量，被修饰的变量不会被序列化
     * - 反序列化后，该变量的值为默认值（null、0等）
     * - 用途：敏感信息（密码）、不需要序列化的字段、不能序列化的字段
     *
     * 【注意事项】
     * - 静态变量不会被序列化（静态变量属于类，不属于对象）
     * - 父类如果没有实现Serializable，父类的字段不会被序列化
     * - 序列化是深拷贝（对象引用的对象也会被序列化，前提是也实现了Serializable）
     * ============================================================
     */
    public static void demonstrateSerialization() {
        System.out.println("\n--- 7. 序列化（Serialization）---");

        System.out.println("【序列化与反序列化演示】");

        String filePath = "c:/temp/student.ser";  // 序列化文件路径

        // 确保目录存在
        File dir = new File("c:/temp");
        if (!dir.exists()) {
            dir.mkdirs();
        }

        // --- 序列化 ---
        System.out.println("序列化：将对象写入文件");
        Student student = new Student("张三", 20, "123456");
        System.out.println("序列化前: " + student);

        try (ObjectOutputStream oos = new ObjectOutputStream(
                new FileOutputStream(filePath))) {
            oos.writeObject(student);  // 写入对象
            System.out.println("序列化成功");
        } catch (IOException e) {
            System.out.println("序列化失败: " + e.getMessage());
        }

        // --- 反序列化 ---
        System.out.println("\n反序列化：从文件读取对象");
        try (ObjectInputStream ois = new ObjectInputStream(
                new FileInputStream(filePath))) {
            Student readStudent = (Student) ois.readObject();  // 读取对象
            System.out.println("反序列化后: " + readStudent);
            System.out.println("注意：password字段被transient修饰，反序列化后为null");
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("反序列化失败: " + e.getMessage());
        }

        System.out.println("\n【序列化要点】");
        System.out.println("1. 类必须实现Serializable接口");
        System.out.println("2. 建议显式指定serialVersionUID");
        System.out.println("3. transient修饰的字段不会被序列化");
        System.out.println("4. 静态变量不会被序列化");
        System.out.println("5. 引用的对象也必须实现Serializable");
        System.out.println("6. 序列化是深拷贝");

        // 清理临时文件
        File file = new File(filePath);
        if (file.exists()) {
            file.delete();
        }
    }

    /**
     * 学生类：用于序列化演示
     * 实现Serializable接口（标记接口，没有方法需要实现）
     */
    static class Student implements Serializable {
        // 序列化版本号，建议显式指定
        private static final long serialVersionUID = 1L;

        private String name;    // 姓名
        private int age;        // 年龄

        /**
         * transient关键字：表示该字段不参与序列化
         * 反序列化后，password的值为null（引用类型默认值）
         * 用途：敏感信息、不需要持久化的字段
         */
        private transient String password;  // 密码（不序列化）

        public Student(String name, int age, String password) {
            this.name = name;
            this.age = age;
            this.password = password;
        }

        public String getName() {
            return name;
        }

        public int getAge() {
            return age;
        }

        public String getPassword() {
            return password;
        }

        @Override
        public String toString() {
            return "Student{name='" + name + "', age=" + age + ", password='" + password + "'}";
        }
    }
}
