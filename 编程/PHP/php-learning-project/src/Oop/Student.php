<?php
// ============================================================
// Student 类 - 继承示例
// ============================================================
// 演示继承（extends）、parent 关键字、方法重写、final 关键字
// ============================================================

namespace App\Oop;

/**
 * Student 类 - 学生类，继承自 Person 类
 * 
 * 演示以下 OOP 概念：
 * - 继承 extends
 * - parent 关键字（调用父类方法）
 * - 方法重写（Override）
 * - final 关键字（禁止继承/重写）
 */
class Student extends Person
{
    // 子类新增的属性
    public string $school;      // 学校
    public string $studentId;   // 学号
    private float $score;       // 成绩（私有，只有本类可访问）

    /**
     * 构造函数
     * 使用 parent::__construct() 调用父类构造函数
     * 
     * @param string $name 姓名
     * @param int $age 年龄
     * @param string $idCard 身份证号
     * @param string $school 学校
     * @param string $studentId 学号
     */
    public function __construct(
        string $name,
        int $age,
        string $idCard,
        string $school,
        string $studentId
    ) {
        // parent:: 用于调用父类的方法
        // 先调用父类构造函数初始化继承的属性
        parent::__construct($name, $age, $idCard);

        // 再初始化子类自己的属性
        $this->school = $school;
        $this->studentId = $studentId;
        $this->score = 0;

        echo "  [构造] Student 对象创建：{$this->name}（{$this->school}）\n";
    }

    /**
     * 析构函数 - 重写父类析构函数
     */
    public function __destruct()
    {
        echo "  [析构] Student 对象销毁：{$this->name}\n";
        // 也可以调用父类析构函数（可选）
        parent::__destruct();
    }

    /**
     * 方法重写（Override）- 覆盖父类的 say 方法
     * 子类可以重写父类的方法，实现不同的行为
     * 
     * @param string $message
     * @return string
     */
    public function say(string $message): string
    {
        // 也可以用 parent::say() 调用父类被重写的方法
        return "学生{$this->name}说：{$message}（我在{$this->school}上学）";
    }

    /**
     * 学习方法 - 子类新增的方法
     * 
     * @param string $subject 学习的科目
     * @return string
     */
    public function study(string $subject): string
    {
        return "{$this->name}正在学习{$subject}";
    }

    /**
     * 设置成绩
     * 
     * @param float $score
     */
    public function setScore(float $score): void
    {
        if ($score >= 0 && $score <= 100) {
            $this->score = $score;
        }
    }

    /**
     * 获取成绩
     * 
     * @return float
     */
    public function getScore(): float
    {
        return $this->score;
    }

    /**
     * 测试访问受保护的属性和方法
     * 子类可以访问父类的 protected 成员
     */
    public function testProtectedAccess(): string
    {
        // 可以访问父类的 protected 属性
        return "可以访问父类的 age（protected）：{$this->age}岁\n"
             . "可以调用父类的 protectedMethod()：" . $this->protectedMethod();
    }

    /**
     * final 方法 - 不能被子类重写
     * 使用 final 关键字修饰的方法，子类不能再重写
     */
    final public function getStudentInfo(): string
    {
        return "学号：{$this->studentId}，姓名：{$this->name}，学校：{$this->school}";
    }
}

/**
 * final 类 - 不能被继承
 * 使用 final 关键字修饰的类，不能有子类
 */
final class FinalClass
{
    public function test(): string
    {
        return '这是 final 类，不能被继承';
    }
}
