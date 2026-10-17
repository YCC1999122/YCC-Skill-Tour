<?php
// ============================================================
// MagicClass - 魔术方法演示
// ============================================================
// 魔术方法是 PHP 中以 __ 开头的特殊方法
// 它们在特定情况下会被自动调用
// ============================================================

namespace App\Oop;

/**
 * MagicClass - 魔术方法演示类
 * 
 * 常用魔术方法：
 * - __get / __set       访问不存在的属性时调用
 * - __isset / __unset   isset() / unset() 不存在的属性时调用
 * - __call / __callStatic  调用不存在的方法时调用
 * - __toString          把对象当作字符串使用时调用
 * - __invoke            把对象当作函数调用时调用
 * - __clone             克隆对象时调用
 * - __construct         构造函数
 * - __destruct          析构函数
 */
class MagicClass
{
    // 存储动态属性的数组
    private array $data = [];

    // 公开属性
    public string $publicProp = '公开属性';

    // ============================================================
    // __set() - 在给不可访问的属性赋值时自动调用
    // ============================================================
    /**
     * @param string $name 属性名
     * @param mixed $value 属性值
     */
    public function __set(string $name, mixed $value): void
    {
        echo "  [__set] 设置属性 '{$name}' = '{$value}'\n";
        $this->data[$name] = $value;
    }

    // ============================================================
    // __get() - 读取不可访问的属性值时自动调用
    // ============================================================
    /**
     * @param string $name 属性名
     * @return mixed
     */
    public function __get(string $name): mixed
    {
        echo "  [__get] 读取属性 '{$name}'\n";
        return $this->data[$name] ?? null;
    }

    // ============================================================
    // __isset() - 对不可访问的属性调用 isset() 或 empty() 时自动调用
    // ============================================================
    /**
     * @param string $name 属性名
     * @return bool
     */
    public function __isset(string $name): bool
    {
        echo "  [__isset] 检查属性 '{$name}' 是否存在\n";
        return isset($this->data[$name]);
    }

    // ============================================================
    // __unset() - 对不可访问的属性调用 unset() 时自动调用
    // ============================================================
    /**
     * @param string $name 属性名
     */
    public function __unset(string $name): void
    {
        echo "  [__unset] 销毁属性 '{$name}'\n";
        unset($this->data[$name]);
    }

    // ============================================================
    // __call() - 调用不可访问的普通方法时自动调用
    // ============================================================
    /**
     * @param string $name 方法名
     * @param array $arguments 参数数组
     * @return mixed
     */
    public function __call(string $name, array $arguments): mixed
    {
        echo "  [__call] 调用不存在的方法 '{$name}'，参数：" . implode(', ', $arguments) . "\n";
        return "调用的方法 {$name} 不存在";
    }

    // ============================================================
    // __callStatic() - 调用不可访问的静态方法时自动调用
    // ============================================================
    /**
     * @param string $name 方法名
     * @param array $arguments 参数数组
     * @return mixed
     */
    public static function __callStatic(string $name, array $arguments): mixed
    {
        echo "  [__callStatic] 调用不存在的静态方法 '{$name}'\n";
        return "调用的静态方法 {$name} 不存在";
    }

    // ============================================================
    // __toString() - 把对象当作字符串使用时自动调用
    // ============================================================
    /**
     * @return string
     */
    public function __toString(): string
    {
        return "MagicClass 对象（包含 " . count($this->data) . " 个动态属性）";
    }

    // ============================================================
    // __invoke() - 把对象当作函数调用时自动调用
    // ============================================================
    /**
     * @param mixed ...$args
     * @return string
     */
    public function __invoke(mixed ...$args): string
    {
        echo "  [__invoke] 对象被当作函数调用，参数：" . implode(', ', $args) . "\n";
        return '对象被调用了！';
    }

    // ============================================================
    // __clone() - 克隆对象时自动调用
    // ============================================================
    public function __clone(): void
    {
        echo "  [__clone] 对象被克隆了\n";
        // 可以在这里进行深拷贝等操作
        // $this->data 是数组，默认是浅拷贝，这里需要重新赋值
        // 如果有对象属性，需要手动克隆
    }
}
