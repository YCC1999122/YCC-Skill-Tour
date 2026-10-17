<?php
// ============================================================
// LoggerTrait - Trait 代码复用示例
// ============================================================
// Trait 是 PHP 5.4+ 引入的代码复用机制
// 用于解决 PHP 单继承的限制，实现横向代码复用
// ============================================================

namespace App\Oop;

/**
 * LoggerTrait - 日志 Trait
 * 
 * Trait 特点：
 * - 使用 trait 关键字定义
 * - 不能直接实例化
 * - 类通过 use 关键字引入 Trait
 * - 一个类可以使用多个 Trait
 * - Trait 可以有属性、方法、抽象方法、静态方法
 * - Trait 优先级：当前类方法 > Trait 方法 > 父类方法
 */
trait LoggerTrait
{
    // Trait 可以有属性
    private array $logs = [];

    /**
     * 记录日志
     * 
     * @param string $message 日志消息
     * @param string $level 日志级别
     */
    public function log(string $message, string $level = 'info'): void
    {
        // 记录日志时间和内容
        $timestamp = date('Y-m-d H:i:s');
        $this->logs[] = "[{$timestamp}] [{$level}] {$message}";
    }

    /**
     * 获取所有日志
     * 
     * @return array
     */
    public function getLogs(): array
    {
        return $this->logs;
    }

    /**
     * 打印所有日志
     */
    public function printLogs(): void
    {
        foreach ($this->logs as $log) {
            echo "  {$log}\n";
        }
    }

    /**
     * 清空日志
     */
    public function clearLogs(): void
    {
        $this->logs = [];
    }
}

/**
 * TimestampTrait - 时间戳 Trait
 * 演示多个 Trait 组合使用
 */
trait TimestampTrait
{
    // 创建时间
    protected \DateTime $createdAt;

    /**
     * 设置创建时间为当前时间
     */
    public function setCreatedAt(): void
    {
        $this->createdAt = new \DateTime();
    }

    /**
     * 获取格式化的创建时间
     * 
     * @return string
     */
    public function getCreatedAt(): string
    {
        return $this->createdAt->format('Y-m-d H:i:s');
    }
}
