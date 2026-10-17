/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

import java.util.ArrayList;
import java.util.List;

/**
 * ============================================================
 * 观察者模式（Observer Pattern）
 *
 * 【定义】
 * 定义对象间的一对多依赖关系，当一个对象的状态发生改变时，
 * 所有依赖它的对象都会得到通知并自动更新
 *
 * 【别名】
 * - 发布-订阅模式（Publish-Subscribe）
 * - 源-监听器模式（Source-Listener）
 *
 * 【组成角色】
 * 1. 主题（Subject）：被观察的对象，维护观察者列表
 *    - 可以添加/删除观察者
 *    - 状态变化时通知所有观察者
 * 2. 观察者（Observer）：观察主题的对象
 *    - 定义更新方法，主题变化时被调用
 * 3. 具体主题（Concrete Subject）：主题的具体实现
 * 4. 具体观察者（Concrete Observer）：观察者的具体实现
 *
 * 【优点】
 * - 主题和观察者之间松耦合
 * - 支持广播通信（主题通知所有观察者）
 * - 符合开闭原则：新增观察者不需要修改主题
 *
 * 【缺点】
 * - 观察者太多时，通知所有观察者耗时
 * - 如果观察者和主题有循环依赖，可能导致死循环
 * - 观察者不知道主题是怎么变的，只知道变了
 *
 * 【JDK中的应用】
 * - java.util.Observable 和 java.util.Observer（已过时）
 * - 事件监听机制（Swing、Android的点击事件等）
 * - Spring的事件机制（ApplicationEvent / ApplicationListener）
 * ============================================================
 */
public class ObserverPattern {

    /**
     * 抽象观察者接口：所有观察者都要实现这个接口
     * 定义了更新方法，主题变化时调用这个方法通知观察者
     */
    interface Observer {
        /**
         * 更新方法
         *
         * @param message 主题传递的消息
         */
        void update(String message);
    }

    /**
     * 抽象主题接口：被观察的对象
     * 定义了添加、删除、通知观察者的方法
     */
    interface Subject {
        /**
         * 注册（添加）观察者
         */
        void registerObserver(Observer observer);

        /**
         * 移除观察者
         */
        void removeObserver(Observer observer);

        /**
         * 通知所有观察者
         */
        void notifyObservers();
    }

    /**
     * 具体主题：新闻发布者
     * 维护订阅者列表，发布新闻时通知所有订阅者
     */
    static class NewsPublisher implements Subject {
        // 观察者列表（订阅者列表）
        private final List<Observer> observers = new ArrayList<>();

        // 最新的新闻内容（主题的状态）
        private String latestNews;

        @Override
        public void registerObserver(Observer observer) {
            observers.add(observer);
            System.out.println("新增了一个订阅者");
        }

        @Override
        public void removeObserver(Observer observer) {
            observers.remove(observer);
            System.out.println("移除了一个订阅者");
        }

        @Override
        public void notifyObservers() {
            // 遍历所有观察者，调用update方法通知
            for (Observer observer : observers) {
                observer.update(latestNews);
            }
        }

        /**
         * 发布新闻
         * 当新闻更新时，通知所有订阅者
         *
         * @param news 新闻内容
         */
        public void publishNews(String news) {
            this.latestNews = news;
            System.out.println("\n新闻发布者发布了新闻: " + news);
            notifyObservers();  // 通知所有观察者
        }
    }

    /**
     * 具体观察者：手机APP订阅者
     */
    static class PhoneAppObserver implements Observer {
        private String appName;

        public PhoneAppObserver(String appName) {
            this.appName = appName;
        }

        @Override
        public void update(String message) {
            System.out.println(appName + " 收到推送: " + message);
        }
    }

    /**
     * 具体观察者：邮箱订阅者
     */
    static class EmailObserver implements Observer {
        private String email;

        public EmailObserver(String email) {
            this.email = email;
        }

        @Override
        public void update(String message) {
            System.out.println("邮箱 " + email + " 收到邮件: " + message);
        }
    }

    /**
     * 具体观察者：微信公众号订阅者
     */
    static class WeChatObserver implements Observer {
        private String wechatId;

        public WeChatObserver(String wechatId) {
            this.wechatId = wechatId;
        }

        @Override
        public void update(String message) {
            System.out.println("微信 " + wechatId + " 收到公众号消息: " + message);
        }
    }

    /**
     * 演示观察者模式
     */
    public static void demonstrate() {
        System.out.println("【观察者模式演示】");

        // 创建主题（新闻发布者）
        NewsPublisher publisher = new NewsPublisher();

        // 创建观察者（订阅者）
        Observer phoneObserver = new PhoneAppObserver("今日头条APP");
        Observer emailObserver = new EmailObserver("user@example.com");
        Observer wechatObserver = new WeChatObserver("小明");

        // 注册观察者
        publisher.registerObserver(phoneObserver);
        publisher.registerObserver(emailObserver);
        publisher.registerObserver(wechatObserver);

        // 发布第一条新闻，所有订阅者都会收到通知
        publisher.publishNews("Java 21发布了！");

        // 移除一个观察者
        publisher.removeObserver(emailObserver);

        // 发布第二条新闻，剩下的订阅者收到通知
        publisher.publishNews("Spring Boot 3.0正式发布！");

        System.out.println("\n【观察者模式总结】");
        System.out.println("一对多关系：一个主题，多个观察者");
        System.out.println("松耦合：主题不知道观察者的具体实现");
        System.out.println("广播通知：主题状态变化时自动通知所有观察者");
        System.out.println("应用场景：事件监听、消息推送、MVC模式等");
    }
}
