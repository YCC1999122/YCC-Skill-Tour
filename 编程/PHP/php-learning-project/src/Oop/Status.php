<?php
// ============================================================
// Status 枚举 - Enum 示例（PHP 8.1+）
// ============================================================
// 枚举（Enum）是 PHP 8.1 引入的新特性
// 用于定义一组固定的可能值
// ============================================================

namespace App\Oop;

/**
 * Status 枚举 - 用户状态枚举
 * 
 * 枚举特点：
 * - 使用 enum 关键字定义
 * - 枚举的 case 是固定的可选值
 * - 可以有方法
 * - 可以实现接口
 * - 分为普通枚举和回退枚举（Backed Enum）
 */
enum Status
{
    // 枚举的各个可选值（case）
    case Active;     // 激活状态
    case Inactive;   // 未激活状态
    case Suspended;  // 暂停状态
    case Deleted;    // 已删除状态

    /**
     * 获取状态的中文名称
     * 枚举可以有方法
     * 
     * @return string
     */
    public function label(): string
    {
        // $this 指向当前枚举值
        return match ($this) {
            self::Active    => '激活',
            self::Inactive  => '未激活',
            self::Suspended => '已暂停',
            self::Deleted   => '已删除'
        };
    }

    /**
     * 检查状态是否为活跃状态
     * 
     * @return bool
     */
    public function isActive(): bool
    {
        return $this === self::Active;
    }
}

/**
 * UserRole 回退枚举 - 带值的枚举
 * 
 * 回退枚举（Backed Enum）：
 * - 每个 case 都有一个对应的值（string 或 int）
 * - 必须指定类型：enum UserRole: string
 * - 可以通过 value 属性获取值
 * - 可以通过 from() / tryFrom() 方法从值创建枚举
 */
enum UserRole: string
{
    // 每个 case 对应一个字符串值
    case Admin    = 'admin';      // 管理员
    case Editor   = 'editor';     // 编辑
    case Author   = 'author';     // 作者
    case Viewer   = 'viewer';     // 访客

    /**
     * 获取角色的权限列表
     * 
     * @return array
     */
    public function permissions(): array
    {
        return match ($this) {
            self::Admin  => ['read', 'write', 'delete', 'manage'],
            self::Editor => ['read', 'write', 'delete'],
            self::Author => ['read', 'write'],
            self::Viewer => ['read']
        };
    }

    /**
     * 检查是否有某个权限
     * 
     * @param string $permission
     * @return bool
     */
    public function hasPermission(string $permission): bool
    {
        return in_array($permission, $this->permissions());
    }
}
