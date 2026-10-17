/**
 * ============================================================
 * Rectangle 类：矩形类，继承自Shape抽象类
 *
 * 这个类演示了另一个Shape的子类实现
 * 通过Circle和Rectangle两个不同的子类，我们可以看到多态的效果
 * ============================================================
 */
public class Rectangle extends Shape {

    /**
     * 成员变量：矩形的宽度和高度
     * 用private修饰，体现封装性
     */
    private double width;
    private double height;

    /**
     * 构造方法：创建矩形对象
     *
     * @param color  矩形的颜色
     * @param width  矩形的宽度
     * @param height 矩形的高度
     */
    public Rectangle(String color, double width, double height) {
        super(color);  // 调用父类构造方法，初始化颜色
        this.width = width;
        this.height = height;
        System.out.println("Rectangle构造方法被调用，宽: " + width + "，高: " + height);
    }

    /**
     * 重写抽象方法：计算矩形面积
     *
     * @return 矩形的面积，公式：宽 × 高
     */
    @Override
    public double calculateArea() {
        return width * height;
    }

    /**
     * 重写抽象方法：计算矩形周长
     *
     * @return 矩形的周长，公式：2 × (宽 + 高)
     */
    @Override
    public double calculatePerimeter() {
        return 2 * (width + height);
    }

    // ---- 以下是子类特有的getter/setter方法 ----

    public double getWidth() {
        return width;
    }

    public void setWidth(double width) {
        if (width > 0) {
            this.width = width;
        }
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        if (height > 0) {
            this.height = height;
        }
    }

    /**
     * 判断是否为正方形
     * 这是Rectangle特有的方法
     *
     * @return 如果宽高相等返回true，否则返回false
     */
    public boolean isSquare() {
        return Math.abs(width - height) < 0.0001;  // 浮点数比较不能直接用==
    }

    /**
     * 重写printInfo方法
     */
    @Override
    public void printInfo() {
        System.out.println("=== 矩形信息 ===");
        super.printInfo();  // 调用父类方法
        System.out.println("矩形宽度: " + width);
        System.out.println("矩形高度: " + height);
        System.out.println("是否为正方形: " + isSquare());
    }
}
