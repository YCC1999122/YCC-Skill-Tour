<?php
// ============================================================
// UserInterface 接口 - 接口示例
// ============================================================
// 演示接口（interface）的定义和使用
// 接口定义了类应该实现的方法规范，但不包含具体实现
// ============================================================

namespace App\Oop;

/**
 * UserInterface 接口 - 用户接口
 * 
 * 接口特点：
 * - 使用 interface 关键字定义
 * - 接口中的方法都是公开的（public）
 * - 接口中的方法都是抽象的（没有方法体）
 * - 类可以实现（implements）多个接口
 * - 接口可以继承（extends）其他接口
 * - 接口中可以定义常量
 */
interface UserInterface
{
    // 接口常量（和类常量类似）
    public const USER_TYPE = '普通用户';

    /**
     * 获取用户名
     * 实现类必须实现这个方法
     * 
     * @return string
     */
    public function getUsername(): string;

    /**
     * 获取用户邮箱
     * 
     * @return string
     */
    public function getEmail(): string;

    /**
     * 验证密码
     * 
     * @param string $password 要验证的密码
     * @return bool
     */
    public function verifyPassword(string $password): bool;
}

/**
 * AdminInterface 接口 - 管理员接口
 * 演示接口继承接口
 */
interface AdminInterface extends UserInterface
{
    /**
     * 获取管理员权限等级
     * 
     * @return int
     */
    public function getPermissionLevel(): int;
}

/**
 * LoggerAwareInterface 接口 - 日志接口
 * 演示一个类实现多个接口
 */
interface LoggerAwareInterface
{
    /**
     * 设置日志
     * 
     * @param string $message
     */
    public function setLogger(string $message): void;
}
