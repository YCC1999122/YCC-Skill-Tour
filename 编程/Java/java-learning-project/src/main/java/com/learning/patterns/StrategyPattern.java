/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

import java.util.Arrays;

/**
 * ============================================================
 * 策略模式（Strategy Pattern）
 *
 * 【定义】
 * 定义一系列算法，把它们一个个封装起来，并且使它们可以互相替换
 * 策略模式让算法的变化独立于使用算法的客户
 *
 * 【组成角色】
 * 1. 策略接口（Strategy）：定义所有算法的公共接口
 * 2. 具体策略（Concrete Strategy）：实现策略接口，封装具体算法
 * 3. 上下文（Context）：持有策略对象的引用，客户端通过它与策略交互
 *
 * 【优点】
 * - 算法可以自由切换
 * - 避免使用多重条件判断（if-else / switch）
 * - 扩展性好：新增算法只需新增策略类，符合开闭原则
 * - 算法封装性好，安全性高
 *
 * 【缺点】
 * - 策略类会增多
 * - 所有策略类都需要对外暴露（客户端需要知道所有策略）
 *
 * 【应用场景】
 * - 一个系统有多个类，区别只在于行为/算法不同
 * - 需要动态切换算法
 * - 需要屏蔽算法的具体实现细节
 * - 多重条件判断（if-else太多）的重构
 *
 * 【JDK中的应用】
 * - java.util.Comparator（比较器，sort方法的第二个参数）
 * - ThreadPoolExecutor的拒绝策略（RejectedExecutionHandler）
 * ============================================================
 */
public class StrategyPattern {

    /**
     * 策略接口：排序策略
     * 定义排序算法的统一接口
     */
    interface SortStrategy {
        /**
         * 排序方法
         *
         * @param arr 待排序的数组
         */
        void sort(int[] arr);

        /**
         * 获取策略名称
         */
        String getName();
    }

    /**
     * 具体策略：冒泡排序
     */
    static class BubbleSort implements SortStrategy {
        @Override
        public void sort(int[] arr) {
            int n = arr.length;
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (arr[j] > arr[j + 1]) {
                        // 交换
                        int temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        }

        @Override
        public String getName() {
            return "冒泡排序";
        }
    }

    /**
     * 具体策略：选择排序
     */
    static class SelectionSort implements SortStrategy {
        @Override
        public void sort(int[] arr) {
            int n = arr.length;
            for (int i = 0; i < n - 1; i++) {
                int minIndex = i;
                for (int j = i + 1; j < n; j++) {
                    if (arr[j] < arr[minIndex]) {
                        minIndex = j;
                    }
                }
                // 交换
                int temp = arr[minIndex];
                arr[minIndex] = arr[i];
                arr[i] = temp;
            }
        }

        @Override
        public String getName() {
            return "选择排序";
        }
    }

    /**
     * 具体策略：插入排序
     */
    static class InsertionSort implements SortStrategy {
        @Override
        public void sort(int[] arr) {
            int n = arr.length;
            for (int i = 1; i < n; i++) {
                int key = arr[i];
                int j = i - 1;
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
        }

        @Override
        public String getName() {
            return "插入排序";
        }
    }

    /**
     * 上下文（Context）：排序器
     * 持有排序策略的引用，客户端通过它来使用不同的排序策略
     */
    static class Sorter {
        // 当前使用的排序策略
        private SortStrategy strategy;

        /**
         * 构造方法：传入初始策略
         */
        public Sorter(SortStrategy strategy) {
            this.strategy = strategy;
        }

        /**
         * 设置排序策略（可以运行时动态切换）
         */
        public void setStrategy(SortStrategy strategy) {
            this.strategy = strategy;
            System.out.println("切换策略为: " + strategy.getName());
        }

        /**
         * 执行排序
         * 调用策略的排序方法（具体算法由策略决定）
         */
        public void sortArray(int[] arr) {
            System.out.println("使用" + strategy.getName() + "进行排序");
            strategy.sort(arr);
        }
    }

    /**
     * 演示策略模式
     */
    public static void demonstrate() {
        System.out.println("【策略模式演示：排序算法】");

        // 准备待排序数组
        int[] arr1 = {5, 2, 8, 1, 9, 3};
        int[] arr2 = {5, 2, 8, 1, 9, 3};
        int[] arr3 = {5, 2, 8, 1, 9, 3};

        // 创建上下文（排序器），初始使用冒泡排序
        Sorter sorter = new Sorter(new BubbleSort());

        // 使用冒泡排序
        System.out.print("排序前: " + Arrays.toString(arr1));
        sorter.sortArray(arr1);
        System.out.println("排序后: " + Arrays.toString(arr1));

        // 动态切换策略为选择排序
        sorter.setStrategy(new SelectionSort());
        System.out.print("排序前: " + Arrays.toString(arr2));
        sorter.sortArray(arr2);
        System.out.println("排序后: " + Arrays.toString(arr2));

        // 动态切换策略为插入排序
        sorter.setStrategy(new InsertionSort());
        System.out.print("排序前: " + Arrays.toString(arr3));
        sorter.sortArray(arr3);
        System.out.println("排序后: " + Arrays.toString(arr3));

        System.out.println("\n【策略模式总结】");
        System.out.println("策略模式封装了一系列可互换的算法");
        System.out.println("客户端可以动态切换算法，而不需要修改代码");
        System.out.println("避免了大量if-else，提高了扩展性");
        System.out.println("JDK中的Comparator就是策略模式的典型应用");
    }
}
