/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

/**
 * ============================================================
 * PatternsDemo 类：设计模式演示入口
 *
 * 本模块包含以下设计模式：
 * 1. 单例模式（Singleton）：饿汉/懒汉/双重检查/枚举/静态内部类
 * 2. 工厂模式（Factory）：简单工厂/工厂方法/抽象工厂
 * 3. 观察者模式（Observer）：发布-订阅
 * 4. 策略模式（Strategy）：算法封装与切换
 * 5. 代理模式（Proxy）：静态代理/JDK动态代理
 * 6. 建造者模式（Builder）：复杂对象构建
 * ============================================================
 */
public class PatternsDemo {

    /**
     * main方法：程序入口
     */
    public static void main(String[] args) {
        System.out.println("========== Java设计模式演示 ==========");

        System.out.println("\n========== 单例模式 ==========");
        SingletonPattern.demonstrate();

        System.out.println("\n========== 工厂模式 ==========");
        FactoryPattern.demonstrate();

        System.out.println("\n========== 观察者模式 ==========");
        ObserverPattern.demonstrate();

        System.out.println("\n========== 策略模式 ==========");
        StrategyPattern.demonstrate();

        System.out.println("\n========== 代理模式 ==========");
        ProxyPattern.demonstrate();

        System.out.println("\n========== 建造者模式 ==========");
        BuilderPattern.demonstrate();

        System.out.println("\n========== 设计模式演示结束 ==========\n");
    }
}
