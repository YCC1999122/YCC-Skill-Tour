<?php
// ============================================================
// PHP 数组与数据结构学习 - Arrays 类
// ============================================================
// 本文件包含 PHP 数组操作的所有核心知识点
// 数组是 PHP 中最强大、最常用的数据结构之一
// ============================================================

// 声明命名空间：App\Arrays
namespace App\Arrays;

/**
 * Arrays 类 - PHP 数组与数据结构演示
 * 
 * 包含以下知识点：
 * - 数组定义（索引/关联/多维）
 * - 数组操作（增删改查/合并/拆分/过滤/映射/归约）
 * - 数组遍历
 * - 常用数组函数
 * - 数组排序
 * - 栈与队列
 * - array_map / array_filter / array_reduce
 */
class Arrays
{
    // ============================================================
    // 1. 数组定义
    // ============================================================

    /**
     * 演示数组的三种定义方式
     * 
     * PHP 数组类型：
     * - 索引数组：键是数字，从 0 开始
     * - 关联数组：键是字符串
     * - 多维数组：数组的元素也是数组
     */
    public function arrayDefinition(): void
    {
        echo "=== 数组定义 ===\n";

        // ---------- 索引数组 ----------
        // 索引数组的键是整数，默认从 0 开始自动递增
        echo "索引数组：\n";

        // 方式一：使用 array() 函数（旧语法）
        $fruits1 = array('苹果', '香蕉', '橘子');

        // 方式二：使用 [] 语法（PHP 5.4+，推荐）
        $fruits2 = ['苹果', '香蕉', '橘子'];

        echo "  \$fruits = ['苹果', '香蕉', '橘子'];\n";
        echo "  \$fruits[0] = {$fruits2[0]}（第一个元素）\n";
        echo "  \$fruits[1] = {$fruits2[1]}（第二个元素）\n";
        echo "  \$fruits[2] = {$fruits2[2]}（第三个元素）\n";

        // 手动指定索引
        $customKeys = [2 => 'a', 5 => 'b', 10 => 'c'];
        echo "\n手动指定索引：\n";
        echo "  \$customKeys = [2 => 'a', 5 => 'b', 10 => 'c'];\n";
        echo "  \$customKeys[5] = {$customKeys[5]}\n";

        // ---------- 关联数组 ----------
        // 关联数组的键是字符串，使用 => 操作符定义键值对
        echo "\n关联数组：\n";
        $user = [
            'name'  => '张三',      // 键为 'name'，值为 '张三'
            'age'   => 25,          // 键为 'age'，值为 25
            'email' => 'zhangsan@example.com',
            'city'  => '北京'
        ];

        echo "  \$user = ['name' => '张三', 'age' => 25, ...];\n";
        echo "  \$user['name'] = {$user['name']}\n";
        echo "  \$user['age'] = {$user['age']}\n";
        echo "  \$user['email'] = {$user['email']}\n";

        // ---------- 多维数组 ----------
        // 数组的元素也是数组，形成二维、三维等多维数组
        echo "\n多维数组（二维）：\n";
        $users = [
            ['id' => 1, 'name' => '张三', 'age' => 25],
            ['id' => 2, 'name' => '李四', 'age' => 30],
            ['id' => 3, 'name' => '王五', 'age' => 28]
        ];

        echo "  \$users 是一个二维数组\n";
        echo "  \$users[0]['name'] = {$users[0]['name']}（第一个用户的名字）\n";
        echo "  \$users[1]['age'] = {$users[1]['age']}（第二个用户的年龄）\n";

        // 三维数组示例
        $matrix = [
            [[1, 2], [3, 4]],
            [[5, 6], [7, 8]]
        ];
        echo "  三维数组 \$matrix[0][1][0] = {$matrix[0][1][0]}\n";

        echo "\n";
    }

    // ============================================================
    // 2. 数组操作（增删改查）
    // ============================================================

    /**
     * 演示数组的增删改查基本操作
     */
    public function arrayCrud(): void
    {
        echo "=== 数组增删改查 ===\n";

        $fruits = ['苹果', '香蕉', '橘子'];
        echo "初始数组：[" . implode(', ', $fruits) . "]\n";

        // ---------- 增加元素 ----------
        echo "\n增加元素：\n";

        // 方式一：在末尾添加
        $fruits[] = '葡萄';
        echo "  \$fruits[] = '葡萄' → [" . implode(', ', $fruits) . "]\n";

        // 方式二：使用 array_push() 函数
        array_push($fruits, '西瓜', '芒果');  // 可以一次添加多个
        echo "  array_push(\$fruits, '西瓜', '芒果') → [" . implode(', ', $fruits) . "]\n";

        // 在开头添加：array_unshift()
        $fruits2 = ['苹果', '香蕉'];
        array_unshift($fruits2, '草莓');
        echo "  array_unshift(\$arr, '草莓') → [" . implode(', ', $fruits2) . "]\n";

        // ---------- 删除元素 ----------
        echo "\n删除元素：\n";

        // 删除最后一个元素：array_pop()
        $last = array_pop($fruits);
        echo "  array_pop() 删除最后一个 '{$last}' → [" . implode(', ', $fruits) . "]\n";

        // 删除第一个元素：array_shift()
        $first = array_shift($fruits);
        echo "  array_shift() 删除第一个 '{$first}' → [" . implode(', ', $fruits) . "]\n";

        // 删除指定元素（使用 unset）
        $arr = ['a', 'b', 'c', 'd'];
        unset($arr[1]);  // 删除索引为 1 的元素
        echo "  unset(\$arr[1]) 删除索引1 → [";
        foreach ($arr as $key => $val) {
            echo "{$key}:{$val} ";
        }
        echo "]（注意：索引不会重新排序）\n";

        // ---------- 修改元素 ----------
        echo "\n修改元素：\n";
        $fruits3 = ['苹果', '香蕉', '橘子'];
        $fruits3[1] = '草莓';
        echo "  \$fruits[1] = '草莓' → [" . implode(', ', $fruits3) . "]\n";

        // ---------- 查询元素 ----------
        echo "\n查询元素：\n";
        echo "  \$fruits3[0] = {$fruits3[0]}（通过索引访问）\n";
        echo "  isset(\$fruits3[5]): " . (isset($fruits3[5]) ? '存在' : '不存在') . "\n";

        // array_key_exists() 检查键是否存在（即使值为 null 也返回 true）
        $arr2 = ['a' => 1, 'b' => null];
        echo "  array_key_exists('b', \$arr): " . (array_key_exists('b', $arr2) ? '存在' : '不存在') . "（键存在，值为null）\n";

        echo "\n";
    }

    // ============================================================
    // 3. 数组遍历
    // ============================================================

    /**
     * 演示数组的各种遍历方式
     */
    public function arrayTraversal(): void
    {
        echo "=== 数组遍历 ===\n";

        $fruits = ['苹果', '香蕉', '橘子', '葡萄'];

        // ---------- for 循环遍历（仅适用于索引数组）----------
        echo "for 循环遍历索引数组：\n  ";
        for ($i = 0; $i < count($fruits); $i++) {
            echo "[{$i}]{$fruits[$i]} ";
        }
        echo "\n";

        // ---------- foreach 遍历（最常用）----------
        echo "\nforeach 遍历值：\n";
        foreach ($fruits as $fruit) {
            echo "  {$fruit}\n";
        }

        // foreach 同时获取键和值
        echo "\nforeach 遍历键值对：\n";
        $user = ['name' => '张三', 'age' => 25, 'city' => '北京'];
        foreach ($user as $key => $value) {
            echo "  {$key}: {$value}\n";
        }

        // ---------- list() 解构赋值 ----------
        echo "\nlist() 解构赋值：\n";
        $person = ['张三', 25, '北京'];
        list($name, $age, $city) = $person;  // 将数组元素依次赋值给变量
        echo "  list(\$name, \$age, \$city) = \$person → \$name={$name}, \$age={$age}, \$city={$city}\n";

        // PHP 7.1+ 支持 [] 语法解构
        [$n, $a, $c] = $person;
        echo "  [\$n, \$a, \$c] = \$person → \$n={$n}, \$a={$a}, \$c={$c}\n";

        // 关联数组解构（PHP 7.1+）
        $userData = ['username' => 'admin', 'password' => '123456'];
        ['username' => $u, 'password' => $p] = $userData;
        echo "  关联数组解构: username={$u}, password={$p}\n";

        // ---------- each() 函数（已废弃，不推荐使用）----------
        echo "\neach() 函数（PHP 7.2 起废弃，PHP 8.0 移除）：已不推荐使用\n";

        // ---------- 数组指针函数 ----------
        echo "\n数组指针函数：\n";
        $arr = ['a', 'b', 'c', 'd'];
        echo "  current() = " . current($arr) . "（当前元素）\n";
        echo "  key() = " . key($arr) . "（当前键）\n";
        next($arr);  // 指针向后移动
        echo "  next() 后 current() = " . current($arr) . "\n";
        prev($arr);  // 指针向前移动
        echo "  prev() 后 current() = " . current($arr) . "\n";
        end($arr);   // 指针移到最后
        echo "  end() 后 current() = " . current($arr) . "\n";
        reset($arr); // 指针重置到开头
        echo "  reset() 后 current() = " . current($arr) . "\n";

        echo "\n";
    }

    // ============================================================
    // 4. 常用数组函数
    // ============================================================

    /**
     * 演示常用的数组函数
     */
    public function commonFunctions(): void
    {
        echo "=== 常用数组函数 ===\n";

        // ---------- count() - 数组长度 ----------
        echo "count() / sizeof()：\n";
        $arr = [1, 2, 3, 4, 5];
        echo "  count([1,2,3,4,5]) = " . count($arr) . "\n";
        echo "  sizeof() 是 count() 的别名\n";

        // ---------- in_array() - 检查值是否存在 ----------
        echo "\nin_array() - 检查值是否存在：\n";
        $fruits = ['苹果', '香蕉', '橘子'];
        echo "  in_array('香蕉', \$fruits): " . (in_array('香蕉', $fruits) ? '存在' : '不存在') . "\n";
        echo "  in_array('葡萄', \$fruits): " . (in_array('葡萄', $fruits) ? '存在' : '不存在') . "\n";
        echo "  第三个参数 true 表示严格比较（类型也必须相同）\n";

        // ---------- array_search() - 搜索值，返回对应的键 ----------
        echo "\narray_search() - 搜索值返回键：\n";
        $key = array_search('橘子', $fruits);
        echo "  array_search('橘子', \$fruits) = {$key}\n";

        // ---------- array_keys() - 获取所有键 ----------
        echo "\narray_keys() - 获取所有键：\n";
        $user = ['name' => '张三', 'age' => 25, 'city' => '北京'];
        $keys = array_keys($user);
        echo "  array_keys(\$user) = [" . implode(', ', $keys) . "]\n";

        // ---------- array_values() - 获取所有值 ----------
        echo "\narray_values() - 获取所有值：\n";
        $values = array_values($user);
        echo "  array_values(\$user) = [" . implode(', ', $values) . "]\n";

        // ---------- array_unique() - 去重 ----------
        echo "\narray_unique() - 去重：\n";
        $duplicate = [1, 2, 2, 3, 3, 3, 4];
        $unique = array_values(array_unique($duplicate));
        echo "  array_unique([1,2,2,3,3,3,4]) = [" . implode(', ', $unique) . "]\n";

        // ---------- array_reverse() - 反转数组 ----------
        echo "\narray_reverse() - 反转数组：\n";
        $reversed = array_reverse($arr);
        echo "  array_reverse([1,2,3,4,5]) = [" . implode(', ', $reversed) . "]\n";

        // ---------- array_merge() - 合并数组 ----------
        echo "\narray_merge() - 合并数组：\n";
        $arr1 = ['a', 'b'];
        $arr2 = ['c', 'd'];
        $merged = array_merge($arr1, $arr2);
        echo "  array_merge(['a','b'], ['c','d']) = [" . implode(', ', $merged) . "]\n";

        // 关联数组合并，后面的会覆盖前面的相同键
        $arr3 = ['a' => 1, 'b' => 2];
        $arr4 = ['b' => 3, 'c' => 4];
        $merged2 = array_merge($arr3, $arr4);
        echo "  关联数组合并: b 被覆盖 → b={$merged2['b']}\n";

        // ---------- array_slice() - 截取数组 ----------
        echo "\narray_slice() - 截取数组：\n";
        $sliced = array_slice($arr, 1, 3);  // 从索引1开始，取3个元素
        echo "  array_slice([1,2,3,4,5], 1, 3) = [" . implode(', ', $sliced) . "]\n";

        // ---------- array_splice() - 移除并替换数组元素 ----------
        echo "\narray_splice() - 移除并替换元素：\n";
        $arr5 = [1, 2, 3, 4, 5];
        $removed = array_splice($arr5, 2, 2, ['x', 'y']);  // 从索引2开始移除2个，替换为 x, y
        echo "  移除的元素: [" . implode(', ', $removed) . "]\n";
        echo "  替换后的数组: [" . implode(', ', $arr5) . "]\n";

        // ---------- array_flip() - 交换键和值 ----------
        echo "\narray_flip() - 交换键值：\n";
        $flip = array_flip($user);
        echo "  交换后: '张三' => 'name', '25' => 'age', '北京' => 'city'\n";

        echo "\n";
    }

    // ============================================================
    // 5. 数组排序详解
    // ============================================================

    /**
     * 演示数组的各种排序函数
     */
    public function arraySorting(): void
    {
        echo "=== 数组排序 ===\n";

        // ---------- sort() - 升序排序（索引数组）----------
        echo "sort() - 升序排序：\n";
        $nums = [5, 2, 8, 1, 9];
        sort($nums);  // 注意：sort() 直接修改原数组，不返回新数组
        echo "  sort([5,2,8,1,9]) → [" . implode(', ', $nums) . "]\n";

        // ---------- rsort() - 降序排序 ----------
        echo "\nrsort() - 降序排序：\n";
        $nums2 = [5, 2, 8, 1, 9];
        rsort($nums2);
        echo "  rsort([5,2,8,1,9]) → [" . implode(', ', $nums2) . "]\n";

        // ---------- asort() - 根据值升序排序（保持关联关系）----------
        echo "\nasort() - 按值升序（保持键值关联）：\n";
        $ages = ['张三' => 25, '李四' => 30, '王五' => 20];
        asort($ages);
        echo "  asort 后：\n";
        foreach ($ages as $name => $age) {
            echo "    {$name}: {$age}\n";
        }

        // ---------- arsort() - 根据值降序排序 ----------
        echo "\narsort() - 按值降序（保持键值关联）：\n";
        $ages2 = ['张三' => 25, '李四' => 30, '王五' => 20];
        arsort($ages2);
        echo "  arsort 后：\n";
        foreach ($ages2 as $name => $age) {
            echo "    {$name}: {$age}\n";
        }

        // ---------- ksort() - 根据键升序排序 ----------
        echo "\nksort() - 按键升序排序：\n";
        $data = ['c' => 3, 'a' => 1, 'b' => 2];
        ksort($data);
        echo "  ksort 后：\n";
        foreach ($data as $key => $val) {
            echo "    {$key}: {$val}\n";
        }

        // ---------- krsort() - 根据键降序排序 ----------
        echo "\nkrsort() - 按键降序排序：\n";
        $data2 = ['c' => 3, 'a' => 1, 'b' => 2];
        krsort($data2);
        echo "  krsort 后：\n";
        foreach ($data2 as $key => $val) {
            echo "    {$key}: {$val}\n";
        }

        // ---------- usort() - 自定义排序 ----------
        echo "\nusort() - 自定义排序函数：\n";
        $people = [
            ['name' => '张三', 'age' => 25],
            ['name' => '李四', 'age' => 30],
            ['name' => '王五', 'age' => 20]
        ];

        // 按年龄排序
        usort($people, function ($a, $b) {
            return $a['age'] <=> $b['age'];  // 太空船运算符
        });

        echo "  按年龄升序排序后：\n";
        foreach ($people as $p) {
            echo "    {$p['name']}: {$p['age']}岁\n";
        }

        // ---------- 多维数组排序 ----------
        echo "\narray_multisort() - 多维数组排序：\n";
        echo "  可同时对多个数组或多维数组进行排序\n";

        echo "\n";
    }

    // ============================================================
    // 6. 栈与队列
    // ============================================================

    /**
     * 演示用数组实现栈和队列
     * 
     * 栈（Stack）：后进先出（LIFO）
     *   - array_push() 在末尾压入
     *   - array_pop() 从末尾弹出
     * 
     * 队列（Queue）：先进先出（FIFO）
     *   - array_push() 在末尾入队
     *   - array_shift() 从开头出队
     */
    public function stackAndQueue(): void
    {
        echo "=== 栈与队列 ===\n";

        // ---------- 栈（Stack）----------
        echo "栈（后进先出 LIFO）：\n";
        $stack = [];

        // 压栈（push）：向栈顶添加元素
        array_push($stack, '元素1');
        array_push($stack, '元素2');
        array_push($stack, '元素3');
        echo "  压栈后：[" . implode(', ', $stack) . "]\n";

        // 弹栈（pop）：从栈顶移除元素
        $pop1 = array_pop($stack);
        echo "  弹出: {$pop1} → [" . implode(', ', $stack) . "]\n";

        $pop2 = array_pop($stack);
        echo "  弹出: {$pop2} → [" . implode(', ', $stack) . "]\n";

        // ---------- 队列（Queue）----------
        echo "\n队列（先进先出 FIFO）：\n";
        $queue = [];

        // 入队（enqueue）：向队尾添加元素
        array_push($queue, '任务1');
        array_push($queue, '任务2');
        array_push($queue, '任务3');
        echo "  入队后：[" . implode(', ', $queue) . "]\n";

        // 出队（dequeue）：从队首移除元素
        $dequeue1 = array_shift($queue);
        echo "  出队: {$dequeue1} → [" . implode(', ', $queue) . "]\n";

        $dequeue2 = array_shift($queue);
        echo "  出队: {$dequeue2} → [" . implode(', ', $queue) . "]\n";

        // ---------- 双端队列（Deque）----------
        echo "\n双端队列（两端都可操作）：\n";
        $deque = ['B', 'C'];

        // 两端都可以添加
        array_unshift($deque, 'A');  // 开头添加
        array_push($deque, 'D');     // 末尾添加
        echo "  两端添加后：[" . implode(', ', $deque) . "]\n";

        // 两端都可以删除
        $front = array_shift($deque);  // 开头删除
        $back = array_pop($deque);     // 末尾删除
        echo "  两端删除：弹出 {$front} 和 {$back} → [" . implode(', ', $deque) . "]\n";

        echo "\n";
    }

    // ============================================================
    // 7. array_map / array_filter / array_reduce
    // ============================================================

    /**
     * 演示高阶数组函数：映射、过滤、归约
     * 
     * 这些是函数式编程风格的数组操作
     */
    public function higherOrderFunctions(): void
    {
        echo "=== 高阶数组函数 ===\n";
        $numbers = [1, 2, 3, 4, 5];

        // ---------- array_map() - 映射 ----------
        // 将数组的每个元素通过回调函数处理，返回新数组
        echo "array_map() - 映射（每个元素应用回调函数）：\n";

        // 每个数乘以 2
        $doubled = array_map(function ($n) {
            return $n * 2;
        }, $numbers);
        echo "  每个数乘2: [" . implode(', ', $doubled) . "]\n";

        // 使用箭头函数（PHP 7.4+）
        $squared = array_map(fn($n) => $n * $n, $numbers);
        echo "  每个数平方: [" . implode(', ', $squared) . "]\n";

        // 多个数组一起处理
        $arr1 = [1, 2, 3];
        $arr2 = [10, 20, 30];
        $sumArr = array_map(fn($a, $b) => $a + $b, $arr1, $arr2);
        echo "  两个数组对应相加: [" . implode(', ', $sumArr) . "]\n";

        // ---------- array_filter() - 过滤 ----------
        // 过滤数组元素，返回满足条件的元素组成的新数组
        echo "\narray_filter() - 过滤（保留满足条件的元素）：\n";

        // 保留偶数
        $evens = array_filter($numbers, function ($n) {
            return $n % 2 === 0;
        });
        echo "  保留偶数: [" . implode(', ', array_values($evens)) . "]\n";

        // 使用箭头函数保留大于3的数
        $greaterThan3 = array_filter($numbers, fn($n) => $n > 3);
        echo "  大于3的数: [" . implode(', ', array_values($greaterThan3)) . "]\n";

        // 不带回调函数：过滤掉所有 "假" 值
        $mixed = [0, 1, '', 'hello', null, false, true, []];
        $filtered = array_filter($mixed);
        echo "  过滤假值后剩余 " . count($filtered) . " 个真值元素\n";

        // ---------- array_reduce() - 归约 ----------
        // 将数组通过回调函数迭代处理，最终得到一个值
        echo "\narray_reduce() - 归约（迭代数组得到一个值）：\n";

        // 求和
        $sum = array_reduce($numbers, function ($carry, $item) {
            // $carry 是累计值，$item 是当前元素
            return $carry + $item;
        }, 0);  // 第三个参数是初始值
        echo "  求和: {$sum}（初始值0）\n";

        // 求乘积
        $product = array_reduce($numbers, fn($carry, $item) => $carry * $item, 1);
        echo "  乘积: {$product}（初始值1）\n";

        // 将数组元素拼接成字符串
        $str = array_reduce($numbers, fn($carry, $item) => $carry . '-' . $item, '');
        echo "  拼接字符串: '{$str}'（初始值空字符串）\n";

        // ---------- 组合使用 ----------
        echo "\n组合使用示例（求所有偶数的平方和）：\n";
        $result = array_reduce(
            array_map(
                fn($n) => $n * $n,
                array_filter($numbers, fn($n) => $n % 2 === 0)
            ),
            fn($carry, $item) => $carry + $item,
            0
        );
        echo "  [1,2,3,4,5] 中偶数的平方和 = {$result}（2²+4²=4+16=20）\n";

        echo "\n";
    }

    // ============================================================
    // 运行所有演示
    // ============================================================

    /**
     * 运行所有数组操作演示
     */
    public function runAll(): void
    {
        $this->arrayDefinition();
        $this->arrayCrud();
        $this->arrayTraversal();
        $this->commonFunctions();
        $this->arraySorting();
        $this->stackAndQueue();
        $this->higherOrderFunctions();
    }
}
