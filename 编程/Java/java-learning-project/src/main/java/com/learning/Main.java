/**
 * ============================================================
 * 包声明：com.learning 是项目的根包
 * ============================================================
 */
package com.learning;

/**
 * ============================================================
 * 导入各模块的演示类
 * 主菜单通过调用各模块的main方法来运行不同的演示
 * ============================================================
 */
import com.learning.basics.BasicsDemo;
import com.learning.oop.OopDemo;
import com.learning.collections.CollectionsDemo;
import com.learning.concurrency.ConcurrencyDemo;
import com.learning.advanced.AdvancedDemo;
import com.learning.patterns.PatternsDemo;

import java.util.Scanner;

/**
 * ============================================================
 * Main 类：Java学习项目的主程序入口
 *
 * 【功能说明】
 * 提供一个交互式菜单，用户可以选择运行不同模块的演示
 * 包括：基础语法、面向对象、集合框架、并发编程、高级特性、设计模式
 *
 * 【使用方式】
 * 运行本类的main方法，按照菜单提示输入数字选择要演示的模块
 * 输入0退出程序
 *
 * 【Scanner使用注意事项】
 * - Scanner是Java 5引入的扫描器类，可以从控制台读取输入
 * - nextInt()读取整数，但不会读取换行符
 * - nextLine()读取整行（包括换行符）
 * - 如果nextInt()后面接nextLine()，需要注意处理换行符问题
 * ============================================================
 */
public class Main {

    /**
     * main方法：程序入口
     * JVM从这里开始执行
     *
     * @param args 命令行参数（本项目未使用）
     */
    public static void main(String[] args) {
        // 创建Scanner对象，用于从控制台读取用户输入
        // System.in 表示标准输入流（键盘）
        Scanner scanner = new Scanner(System.in);

        // 主循环：不断显示菜单，等待用户选择，直到用户选择退出
        while (true) {
            // 打印主菜单
            printMenu();

            // 读取用户输入的选项
            // hasNextInt()判断下一个输入是否是整数（防止用户输入非数字导致异常）
            if (!scanner.hasNextInt()) {
                // 如果输入不是数字，读取并丢弃这一行
                String invalid = scanner.next();
                System.out.println("\n无效的输入: " + invalid + "，请输入数字！");
                continue;  // 跳过本次循环，重新显示菜单
            }

            int choice = scanner.nextInt();  // 读取用户选择的数字

            // 使用switch-case根据用户选择执行不同模块
            switch (choice) {
                case 1:
                    // 基础语法模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：基础语法演示");
                    System.out.println("=".repeat(60));
                    BasicsDemo.main(new String[]{});  // 调用BasicsDemo的main方法
                    break;

                case 2:
                    // 面向对象模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：面向对象编程演示");
                    System.out.println("=".repeat(60));
                    OopDemo.main(new String[]{});
                    break;

                case 3:
                    // 集合框架模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：集合框架演示");
                    System.out.println("=".repeat(60));
                    CollectionsDemo.main(new String[]{});
                    break;

                case 4:
                    // 并发编程模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：并发编程演示");
                    System.out.println("=".repeat(60));
                    ConcurrencyDemo.main(new String[]{});
                    break;

                case 5:
                    // 高级特性模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：高级特性演示");
                    System.out.println("=".repeat(60));
                    AdvancedDemo.main(new String[]{});
                    break;

                case 6:
                    // 设计模式模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：设计模式演示");
                    System.out.println("=".repeat(60));
                    PatternsDemo.main(new String[]{});
                    break;

                case 7:
                    // 运行所有模块
                    System.out.println("\n" + "=".repeat(60));
                    System.out.println("正在运行：全部演示（按顺序执行所有模块）");
                    System.out.println("=".repeat(60));
                    BasicsDemo.main(new String[]{});
                    OopDemo.main(new String[]{});
                    CollectionsDemo.main(new String[]{});
                    ConcurrencyDemo.main(new String[]{});
                    AdvancedDemo.main(new String[]{});
                    PatternsDemo.main(new String[]{});
                    System.out.println("\n所有模块演示完毕！");
                    break;

                case 0:
                    // 退出程序
                    System.out.println("\n感谢使用Java学习项目，再见！");
                    scanner.close();  // 关闭Scanner
                    System.exit(0);   // 正常退出程序
                    break;

                default:
                    // 输入的数字不在选项范围内
                    System.out.println("\n无效的选项: " + choice + "，请输入0-7之间的数字！");
                    break;
            }

            // 每个模块演示完后暂停一下，等待用户按回车继续
            System.out.println("\n按回车键返回主菜单...");
            try {
                // 先消费掉nextInt()留下的换行符
                scanner.nextLine();
                // 再等待用户输入一行（按回车）
                scanner.nextLine();
            } catch (Exception e) {
                // 忽略异常
            }
        }
    }

    /**
     * 打印主菜单
     * 将菜单显示逻辑抽取为一个方法，使main方法更简洁
     */
    private static void printMenu() {
        System.out.println("\n" + "=".repeat(60));
        System.out.println("          Java 学习项目 - 交互式演示菜单");
        System.out.println("=".repeat(60));
        System.out.println("  1. 基础语法（数据类型、运算符、控制流、方法、数组、String）");
        System.out.println("  2. 面向对象（封装、继承、多态、接口、内部类、枚举）");
        System.out.println("  3. 集合框架（List、Set、Map、迭代器、泛型、Stream）");
        System.out.println("  4. 并发编程（线程、同步、锁、线程池、原子类、死锁）");
        System.out.println("  5. 高级特性（异常、注解、反射、Lambda、日期、序列化）");
        System.out.println("  6. 设计模式（单例、工厂、观察者、策略、代理、建造者）");
        System.out.println("  7. 运行全部演示");
        System.out.println("  0. 退出程序");
        System.out.println("=".repeat(60));
        System.out.print("请输入选项（0-7）: ");
    }
}
