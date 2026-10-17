/**
 * ============================================================
 * 包声明
 * ============================================================
 */
package com.learning.patterns;

/**
 * ============================================================
 * 工厂模式（Factory Pattern）
 *
 * 【定义】
 * 定义创建对象的接口，让子类决定实例化哪个类
 * 工厂方法使一个类的实例化延迟到其子类
 *
 * 【为什么用工厂模式】
 * - 解耦：将对象的创建和使用分离
 * - 减少重复代码：创建逻辑集中管理
 * - 提高可维护性：修改创建逻辑只需改工厂类
 * - 符合开闭原则：新增产品时只需增加对应工厂类
 *
 * 【工厂模式的三种形式】
 * 1. 简单工厂（Simple Factory）：不是23种设计模式之一，但是工厂模式的基础
 *    - 一个工厂类根据参数创建不同产品
 *    - 缺点：新增产品需要修改工厂类，违反开闭原则
 *
 * 2. 工厂方法（Factory Method）：23种设计模式之一
 *    - 定义工厂接口，每个产品对应一个具体工厂
 *    - 新增产品只需新增工厂类，符合开闭原则
 *    - 缺点：类的数量会增多
 *
 * 3. 抽象工厂（Abstract Factory）：23种设计模式之一
 *    - 工厂可以创建一族（多个相关/相互依赖的）产品
 *    - 适用于有多个产品族的场景
 *    - 缺点：新增产品族容易，新增产品等级结构困难（开闭原则的倾斜）
 * ============================================================
 */
public class FactoryPattern {

    // ============================================================
    // 产品接口和实现类（公共）
    // ============================================================

    /**
     * 产品接口：手机
     */
    interface Phone {
        void call();  // 打电话
        String getName();
    }

    /**
     * 具体产品：苹果手机
     */
    static class IPhone implements Phone {
        @Override
        public void call() {
            System.out.println("使用iPhone打电话");
        }

        @Override
        public String getName() {
            return "iPhone";
        }
    }

    /**
     * 具体产品：华为手机
     */
    static class HuaweiPhone implements Phone {
        @Override
        public void call() {
            System.out.println("使用华为手机打电话");
        }

        @Override
        public String getName() {
            return "华为手机";
        }
    }

    /**
     * 具体产品：小米手机
     */
    static class XiaomiPhone implements Phone {
        @Override
        public void call() {
            System.out.println("使用小米手机打电话");
        }

        @Override
        public String getName() {
            return "小米手机";
        }
    }

    // ============================================================
    // 1. 简单工厂模式
    // ============================================================

    /**
     * 简单工厂（Simple Factory）
     *
     * 【特点】
     * - 一个工厂类，根据参数创建不同的产品
     * - 也叫静态工厂（通常工厂方法是静态的）
     *
     * 【缺点】
     * - 新增产品需要修改工厂类，违反开闭原则
     * - 工厂类职责过重，出问题影响所有产品
     */
    static class SimplePhoneFactory {
        /**
         * 根据类型创建手机
         *
         * @param type 手机类型
         * @return 手机对象
         */
        public static Phone createPhone(String type) {
            if ("apple".equalsIgnoreCase(type)) {
                return new IPhone();
            } else if ("huawei".equalsIgnoreCase(type)) {
                return new HuaweiPhone();
            } else if ("xiaomi".equalsIgnoreCase(type)) {
                return new XiaomiPhone();
            } else {
                throw new IllegalArgumentException("不支持的手机类型: " + type);
            }
        }
    }

    // ============================================================
    // 2. 工厂方法模式（Factory Method）
    // ============================================================

    /**
     * 抽象工厂接口：手机工厂
     * 定义创建产品的方法
     */
    interface PhoneFactory {
        Phone createPhone();  // 工厂方法：创建手机
    }

    /**
     * 具体工厂：苹果手机工厂
     * 专门生产苹果手机
     */
    static class AppleFactory implements PhoneFactory {
        @Override
        public Phone createPhone() {
            return new IPhone();
        }
    }

    /**
     * 具体工厂：华为手机工厂
     * 专门生产华为手机
     */
    static class HuaweiFactory implements PhoneFactory {
        @Override
        public Phone createPhone() {
            return new HuaweiPhone();
        }
    }

    /**
     * 具体工厂：小米手机工厂
     * 专门生产小米手机
     */
    static class XiaomiFactory implements PhoneFactory {
        @Override
        public Phone createPhone() {
            return new XiaomiPhone();
        }
    }

    // ============================================================
    // 3. 抽象工厂模式（Abstract Factory）
    // ============================================================

    /**
     * 另一个产品等级结构：笔记本电脑
     */
    interface Laptop {
        void work();
        String getName();
    }

    static class MacBook implements Laptop {
        @Override
        public void work() {
            System.out.println("使用MacBook办公");
        }

        @Override
        public String getName() {
            return "MacBook";
        }
    }

    static class MateBook implements Laptop {
        @Override
        public void work() {
            System.out.println("使用MateBook办公");
        }

        @Override
        public String getName() {
            return "MateBook";
        }
    }

    /**
     * 抽象工厂接口：电子产品工厂
     * 可以创建一族产品（手机 + 笔记本）
     *
     * 【抽象工厂 vs 工厂方法】
     * - 工厂方法：一个工厂只创建一种产品
     * - 抽象工厂：一个工厂可以创建一族（多种）相关产品
     */
    interface ElectronicsFactory {
        Phone createPhone();    // 创建手机
        Laptop createLaptop();  // 创建笔记本
    }

    /**
     * 具体工厂：苹果工厂
     * 生产苹果系列产品（iPhone + MacBook）
     */
    static class AppleElectronicsFactory implements ElectronicsFactory {
        @Override
        public Phone createPhone() {
            return new IPhone();
        }

        @Override
        public Laptop createLaptop() {
            return new MacBook();
        }
    }

    /**
     * 具体工厂：华为工厂
     * 生产华为系列产品（华为手机 + MateBook）
     */
    static class HuaweiElectronicsFactory implements ElectronicsFactory {
        @Override
        public Phone createPhone() {
            return new HuaweiPhone();
        }

        @Override
        public Laptop createLaptop() {
            return new MateBook();
        }
    }

    // ============================================================
    // 演示方法
    // ============================================================

    /**
     * 演示所有工厂模式
     */
    public static void demonstrate() {
        // --- 简单工厂 ---
        System.out.println("【1. 简单工厂模式】");
        Phone phone1 = SimplePhoneFactory.createPhone("apple");
        Phone phone2 = SimplePhoneFactory.createPhone("huawei");
        phone1.call();
        phone2.call();
        System.out.println("缺点：新增手机需要修改工厂类，违反开闭原则");

        // --- 工厂方法 ---
        System.out.println("\n【2. 工厂方法模式】");
        PhoneFactory appleFactory = new AppleFactory();
        PhoneFactory huaweiFactory = new HuaweiFactory();

        Phone iphone = appleFactory.createPhone();
        Phone huawei = huaweiFactory.createPhone();

        iphone.call();
        huawei.call();
        System.out.println("优点：新增手机只需新增工厂类，符合开闭原则");
        System.out.println("缺点：类的数量会变多");

        // --- 抽象工厂 ---
        System.out.println("\n【3. 抽象工厂模式】");
        ElectronicsFactory appleElectronics = new AppleElectronicsFactory();
        ElectronicsFactory huaweiElectronics = new HuaweiElectronicsFactory();

        System.out.println("苹果系列产品：");
        appleElectronics.createPhone().call();
        appleElectronics.createLaptop().work();

        System.out.println("华为系列产品：");
        huaweiElectronics.createPhone().call();
        huaweiElectronics.createLaptop().work();

        System.out.println("抽象工厂：一个工厂可以创建一族相关产品");
        System.out.println("开闭原则的倾斜：新增产品族容易，新增产品等级困难");
    }
}
