/**
 * ============================================================
 * Shape 抽象类：形状的抽象基类
 *
 * 【抽象类（abstract class）是什么】
 * - 用abstract关键字修饰的类叫抽象类
 * - 抽象类不能被实例化（不能new出来），只能被继承
 * - 抽象类中可以有抽象方法，也可以有普通方法、成员变量、构造方法
 * - 抽象类是用来被继承的，体现了"模板设计模式"的思想
 *
 * 【抽象方法（abstract method）是什么】
 * - 用abstract关键字修饰，没有方法体（直接用分号结束）
 * - 抽象方法必须在抽象类中（或接口中）
 * - 子类继承抽象类后，必须实现所有抽象方法，否则子类也必须是抽象类
 *
 * 【为什么需要抽象类】
 * - 当父类的某些方法无法确定具体实现，需要由子类来实现时
 * - 抽象类可以定义子类必须实现的方法，起到规范/契约的作用
 * - 同时可以提供一些通用的实现，供所有子类复用
 * ============================================================
 */
public abstract class Shape {

    /**
     * 成员变量：形状的颜色
     * protected访问修饰符：同一个包内和子类可以访问
     * 这里用protected是为了让子类可以直接访问，同时封装了外部访问
     */
    protected String color;

    /**
     * 静态变量：形状计数
     * static修饰的变量属于类本身，所有对象共享同一个变量
     * 每创建一个形状对象，计数器就加1
     */
    private static int shapeCount = 0;

    /**
     * 构造方法：创建Shape对象时初始化颜色
     * 构造方法的特点：
     * - 方法名和类名完全相同
     * - 没有返回值类型（连void都不能写）
     * - 创建对象时自动调用，用于初始化对象
     * - 如果不写任何构造方法，编译器会自动提供一个无参构造方法
     * - 如果写了有参构造方法，编译器就不会再提供无参构造方法了
     *
     * @param color 形状的颜色
     */
    public Shape(String color) {
        // this关键字：代表当前对象的引用
        // this.color 表示当前对象的color成员变量
        // 这里用于区分成员变量和局部变量（参数名和成员变量名相同时）
        this.color = color;
        shapeCount++;  // 每创建一个形状，静态计数器加1
        System.out.println("Shape构造方法被调用，当前形状总数: " + shapeCount);
    }

    /**
     * 抽象方法：计算面积
     * 只有方法声明，没有方法体（用分号结束）
     * 子类必须实现这个方法，提供具体的面积计算逻辑
     * 因为不同形状的面积计算公式不同，所以父类无法给出具体实现
     *
     * @return 形状的面积
     */
    public abstract double calculateArea();

    /**
     * 抽象方法：计算周长
     * 同样，不同形状周长计算公式不同，所以定义为抽象方法
     *
     * @return 形状的周长
     */
    public abstract double calculatePerimeter();

    /**
     * 普通方法（非抽象方法）：获取颜色
     * 抽象类中可以有普通方法，这些方法有具体实现
     * 子类可以直接继承使用，也可以重写
     *
     * @return 形状的颜色
     */
    public String getColor() {
        return color;
    }

    /**
     * 普通方法：设置颜色
     */
    public void setColor(String color) {
        this.color = color;
    }

    /**
     * 普通方法：打印形状信息
     * 这是一个通用方法，所有子类都可以直接使用
     * 方法中调用了抽象方法calculateArea()，这体现了模板方法模式的思想
     */
    public void printInfo() {
        System.out.println("形状颜色: " + color);
        System.out.println("形状面积: " + calculateArea());
        System.out.println("形状周长: " + calculatePerimeter());
    }

    /**
     * 静态方法：获取形状总数
     * static方法属于类本身，不依赖于对象
     * 可以直接通过类名调用，如 Shape.getShapeCount()
     * 静态方法中不能直接访问非静态成员（因为没有对象）
     *
     * @return 已创建的形状总数
     */
    public static int getShapeCount() {
        return shapeCount;
    }
}
