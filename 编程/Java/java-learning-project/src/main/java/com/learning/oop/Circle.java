/**
 * ============================================================
 * Circle 类：圆形类，继承自Shape抽象类
 *
 * 【继承（extends）是什么】
 * - 继承是一种"is-a"的关系，子类是父类的一种特殊类型
 * - 子类可以继承父类的成员变量和方法（非private的）
 * - 子类可以添加自己的成员变量和方法
 * - 子类可以重写（override）父类的方法，提供自己的实现
 * - Java是单继承的，一个类只能有一个直接父类，但可以多层继承
 * - 所有类都直接或间接继承自Object类
 *
 * 【继承的好处】
 * - 代码复用：子类可以直接使用父类的代码
 * - 代码扩展：子类可以在父类基础上增加新功能
 * - 多态基础：有了继承才有多态
 * ============================================================
 */
public class Circle extends Shape {

    /**
     * 成员变量：圆的半径
     * private访问修饰符：只有本类内部可以访问
     * 这是封装的体现：把数据隐藏起来，通过公共方法访问
     */
    private double radius;

    /**
     * 静态常量：圆周率
     * final修饰的变量叫常量，一旦赋值就不能修改
     * static final一起修饰的叫静态常量，通常全大写命名
     */
    public static final double PI = 3.141592653589793;

    /**
     * 构造方法：创建圆形对象
     *
     * @param color  圆形的颜色
     * @param radius 圆形的半径
     */
    public Circle(String color, double radius) {
        // super关键字：调用父类的构造方法
        // 子类构造方法的第一行必须先调用父类的构造方法（通过super）
        // 如果没有显式写super()，编译器会自动调用父类的无参构造方法
        // 这里父类只有有参构造方法，所以必须显式调用super(color)
        super(color);
        // this调用自己的成员变量
        this.radius = radius;
        System.out.println("Circle构造方法被调用，半径: " + radius);
    }

    /**
     * 重写（Override）父类的抽象方法：计算圆的面积
     *
     * 【方法重写（Override）是什么】
     * - 子类对父类的方法进行重新实现
     * - 要求：方法名、参数列表、返回值类型必须和父类一致
     * - 访问修饰符不能比父类更严格（可以更宽松）
     * - 抛出的异常不能比父类更多（可以更少）
     *
     * 【@Override注解】
     * - 标注在方法上，表示该方法是重写父类的方法
     * - 编译器会检查是否真的重写了父类方法，防止写错
     * - 不是必须的，但强烈建议加上，提高代码可读性和安全性
     *
     * @return 圆的面积，公式：π * r²
     */
    @Override
    public double calculateArea() {
        return PI * radius * radius;
    }

    /**
     * 重写父类的抽象方法：计算圆的周长
     *
     * @return 圆的周长，公式：2 * π * r
     */
    @Override
    public double calculatePerimeter() {
        return 2 * PI * radius;
    }

    /**
     * 子类自己的方法：获取半径
     * 这是子类特有的方法，父类中没有
     */
    public double getRadius() {
        return radius;
    }

    /**
     * 子类自己的方法：设置半径
     */
    public void setRadius(double radius) {
        if (radius > 0) {  // 数据校验，保证半径必须为正数
            this.radius = radius;
        } else {
            System.out.println("半径必须大于0！");
        }
    }

    /**
     * 重写父类的printInfo方法，增加半径信息
     * 使用super.printInfo()调用父类的printInfo方法
     * 这样既复用了父类的代码，又增加了子类特有的内容
     */
    @Override
    public void printInfo() {
        System.out.println("=== 圆形信息 ===");
        super.printInfo();  // 调用父类的printInfo方法
        System.out.println("圆的半径: " + radius);
    }
}
