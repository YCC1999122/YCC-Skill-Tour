/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

/**
 * ============================================================
 * 代理模式（Proxy Pattern）
 *
 * 【定义】
 * 为其他对象提供一个代理，以控制对这个对象的访问
 * 代理对象在客户端和目标对象之间起到中介作用
 *
 * 【为什么使用代理】
 * - 控制访问：限制某些客户端的访问权限
 * - 增强功能：在不修改目标对象的前提下增加功能
 * - 远程代理：访问远程对象（如RMI）
 * - 虚拟代理：延迟加载（大对象按需创建）
 * - 保护代理：权限控制
 * - 智能引用：访问对象时做额外操作（如计数、日志）
 *
 * 【代理模式的分类】
 * 1. 静态代理：代理类在编译期就存在，需要手动编写
 *    - 优点：简单，容易理解
 *    - 缺点：每个接口都要写代理类，代码冗余；接口变化时代理类也要改
 *
 * 2. 动态代理：代理类在运行时动态生成，不需要手动编写
 *    - JDK动态代理：基于接口，利用反射生成代理类（目标类必须实现接口）
 *    - CGLIB动态代理：基于继承，生成目标类的子类（不需要接口）
 *
 * 【JDK动态代理 vs CGLIB】
 * - JDK动态代理：目标类必须实现接口，基于接口生成代理类
 * - CGLIB：目标类不需要接口，通过继承生成子类
 * - Spring AOP中两种都支持，默认用JDK动态代理，没有接口时用CGLIB
 *
 * 【组成角色】
 * 1. 抽象主题（Subject）：定义真实主题和代理的共同接口
 * 2. 真实主题（Real Subject）：被代理的对象，执行业务逻辑
 * 3. 代理（Proxy）：持有真实主题的引用，控制对真实主题的访问
 * ============================================================
 */
public class ProxyPattern {

    /**
     * 抽象主题接口：用户服务
     * 定义真实主题和代理的共同接口
     */
    interface UserService {
        /**
         * 添加用户
         */
        void addUser(String name);

        /**
         * 删除用户
         */
        void deleteUser(String name);
    }

    /**
     * 真实主题：用户服务实现类
     * 执行业务逻辑的真正对象
     */
    static class UserServiceImpl implements UserService {
        @Override
        public void addUser(String name) {
            System.out.println("【真实对象】添加用户: " + name);
        }

        @Override
        public void deleteUser(String name) {
            System.out.println("【真实对象】删除用户: " + name);
        }
    }

    // ============================================================
    // 1. 静态代理
    // ============================================================

    /**
     * 静态代理类：用户服务代理
     * 手动编写的代理类，实现了和真实对象相同的接口
     * 在调用真实对象前后可以增加额外逻辑（日志、权限检查等）
     */
    static class UserServiceProxy implements UserService {
        // 持有真实对象的引用
        private final UserService realService;

        public UserServiceProxy(UserService realService) {
            this.realService = realService;
        }

        @Override
        public void addUser(String name) {
            // 前置增强：在调用真实方法前增加逻辑
            System.out.println("【静态代理】添加用户前：记录日志，检查权限");

            // 调用真实对象的方法
            realService.addUser(name);

            // 后置增强：在调用真实方法后增加逻辑
            System.out.println("【静态代理】添加用户后：提交事务，记录结果");
        }

        @Override
        public void deleteUser(String name) {
            System.out.println("【静态代理】删除用户前：检查权限");

            if ("admin".equals(name)) {
                System.out.println("【静态代理】不能删除admin用户，操作被拒绝");
                return;  // 控制访问：可以决定是否调用真实方法
            }

            realService.deleteUser(name);
            System.out.println("【静态代理】删除用户后：记录日志");
        }
    }

    // ============================================================
    // 2. JDK动态代理
    // ============================================================

    /**
     * 动态代理的调用处理器
     * 实现InvocationHandler接口，重写invoke方法
     * 代理对象的所有方法调用都会被转发到invoke方法
     */
    static class LogInvocationHandler implements InvocationHandler {
        // 被代理的真实对象
        private final Object target;

        public LogInvocationHandler(Object target) {
            this.target = target;
        }

        /**
         * 代理对象的方法调用都会进入这个方法
         *
         * @param proxy  代理对象本身（一般不用）
         * @param method 被调用的方法
         * @param args   方法参数
         * @return 方法返回值
         */
        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            // 前置增强
            String methodName = method.getName();
            System.out.println("【JDK动态代理】方法 " + methodName + " 开始执行");
            long start = System.currentTimeMillis();

            // 调用真实对象的方法（反射调用）
            Object result = method.invoke(target, args);

            // 后置增强
            long end = System.currentTimeMillis();
            System.out.println("【JDK动态代理】方法 " + methodName + " 执行结束，耗时: " + (end - start) + "ms");

            return result;
        }
    }

    /**
     * 创建JDK动态代理的工具方法
     *
     * @param target 真实对象
     * @return 代理对象
     */
    public static Object createJdkProxy(Object target) {
        return Proxy.newProxyInstance(
                target.getClass().getClassLoader(),      // 类加载器
                target.getClass().getInterfaces(),       // 真实对象实现的接口（代理类要实现这些接口）
                new LogInvocationHandler(target)         // 调用处理器
        );
    }

    // ============================================================
    // 演示方法
    // ============================================================

    /**
     * 演示代理模式
     */
    public static void demonstrate() {
        // --- 静态代理 ---
        System.out.println("【1. 静态代理】");
        // 创建真实对象
        UserService realService = new UserServiceImpl();
        // 创建代理对象（包装真实对象）
        UserService staticProxy = new UserServiceProxy(realService);

        // 通过代理对象调用方法
        staticProxy.addUser("张三");
        System.out.println();
        staticProxy.deleteUser("admin");
        System.out.println();
        staticProxy.deleteUser("李四");

        // --- JDK动态代理 ---
        System.out.println("\n【2. JDK动态代理】");
        // 创建动态代理对象
        UserService dynamicProxy = (UserService) createJdkProxy(new UserServiceImpl());

        // 调用方法
        dynamicProxy.addUser("王五");
        System.out.println();
        dynamicProxy.deleteUser("赵六");

        System.out.println("\n【代理模式总结】");
        System.out.println("静态代理：手动编写代理类，编译时生成，简单但不够灵活");
        System.out.println("JDK动态代理：运行时生成代理类，基于接口，灵活强大");
        System.out.println("CGLIB动态代理：基于继承，不需要接口");
        System.out.println("应用场景：AOP（面向切面编程）、事务管理、日志、权限控制等");
        System.out.println("Spring AOP就是基于动态代理实现的");
    }
}
