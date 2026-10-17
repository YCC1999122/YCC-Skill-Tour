#!/bin/bash
# ==============================================================================
# 文件名：02_loops.sh
# 描述：Shell/Bash 流程控制 - 循环结构 for/while/until
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 02_loops.sh ===================="
echo ""

# -------------------- 1. for 循环 --------------------
echo "========== 1. for 循环 =========="

# --- 1.1 for-in 列表循环 ---
echo "--- 1.1 for-in 列表循环 ---"
# 语法：
# for 变量 in 值1 值2 值3 ...; do
#     命令列表
# done
# 每次循环，变量依次取列表中的一个值

echo "遍历字符串列表："
for fruit in 苹果 香蕉 橙子 葡萄; do
    echo "  水果：$fruit"
done

echo ""
echo "遍历数字序列："
# 可以用 {起始..结束..步长} 生成数字序列（Bash 4+ 支持步长）
for num in {1..5}; do
    echo "  数字：$num"
done

echo ""
echo "带步长的数字序列（步长2）："
for num in {1..10..2}; do
    echo "  数字：$num"
done

echo ""
echo "逆序序列："
for num in {5..1}; do
    echo "  数字：$num"
done

echo ""
echo "遍历命令输出结果："
# 用命令替换获取列表
for file in $(ls *.sh 2>/dev/null); do
    echo "  Shell 文件：$file"
done

echo ""

# --- 1.2 C 语言风格 for 循环 ---
echo "--- 1.2 C 语言风格 for 循环 ---"
# 语法：
# for (( 初始化; 条件; 步进 )); do
#     命令列表
# done
# 这种写法和 C 语言的 for 循环完全一样
# 注意：是双括号 (( ))

echo "从1加到5："
sum=0
for (( i=1; i<=5; i++ )); do
    echo "  i = $i, sum = $((sum + i))"
    sum=$((sum + i))
done
echo "总和：$sum"

echo ""
echo "倒序输出："
for (( i=5; i>=1; i-- )); do
    echo "  i = $i"
done

echo ""
echo "嵌套循环（九九乘法表）："
for (( i=1; i<=9; i++ )); do
    line=""
    for (( j=1; j<=i; j++ )); do
        product=$((i * j))
        line="${line}${j}x${i}=${product}  "
    done
    echo "  $line"
done

echo ""

# --- 1.3 遍历文件（目录）---
echo "--- 1.3 遍历文件和目录 ---"

echo "遍历当前目录下的 .sh 文件："
for file in ./*.sh; do
    if [ -f "$file" ]; then
        echo "  文件：$file"
    fi
done

echo ""
echo "递归遍历目录（用 find）："
# 注意：遍历文件名包含空格的文件时，for-in 可能有问题
# 推荐用 while read 配合 find（见管道与循环一节）
count=0
for f in $(find . -maxdepth 2 -name "*.sh" -type f 2>/dev/null | head -5); do
    echo "  找到：$f"
    count=$((count + 1))
done
echo "共找到 $count 个 .sh 文件（只显示前5个）"

echo ""
echo "========== 2. while 循环 =========="

# -------------------- 2. while 循环 --------------------
# 语法：
# while 条件命令; do
#     命令列表
# done
# 当条件命令返回 0（成功）时，继续循环
# 当条件命令返回非 0（失败）时，退出循环

echo "--- 2.1 while 基本用法 ---"

echo "从1数到5："
i=1                                  # 初始化计数器
while [ $i -le 5 ]; do               # 条件：i <= 5
    echo "  i = $i"
    i=$((i + 1))                     # 计数器自增
done

echo ""
echo "倒序输出："
i=5
while [ $i -ge 1 ]; do
    echo "  i = $i"
    i=$((i - 1))
done

echo ""
echo "while 读取文件内容："
# 创建一个测试文件
cat > test_file.txt << 'EOF'
第一行内容
第二行内容
第三行内容
第四行内容
EOF

# while read 逐行读取文件
# read 每次读取一行，赋值给变量
while read line; do
    echo "  读取：$line"
done < test_file.txt                  # 重定向文件输入到 while 循环

rm -f test_file.txt                   # 清理测试文件

echo ""
echo "--- 2.2 while 无限循环（死循环）---"
# 有几种写法可以创建死循环：
# while true; do ... done
# while :; do ... done     （: 是空命令，永远返回成功）
# while [ 1 ]; do ... done

echo "死循环示例（使用 break 跳出）："
i=1
while true; do
    echo "  第 $i 次循环"
    if [ $i -ge 3 ]; then
        echo "  达到3次，跳出循环"
        break
    fi
    i=$((i + 1))
done

echo ""
echo "========== 3. until 循环 =========="

# -------------------- 3. until 循环 --------------------
# 语法：
# until 条件命令; do
#     命令列表
# done
# until 和 while 相反：
# - while 是条件为真时循环
# - until 是条件为假时循环（直到条件为真时停止）

echo "--- until 基本用法 ---"

echo "从1数到5："
i=1
until [ $i -gt 5 ]; do               # 直到 i > 5 时停止
    echo "  i = $i"
    i=$((i + 1))
done

echo ""
echo "until 也可以写死循环："
i=1
until false; do                      # 条件永远为假，所以永远循环
    echo "  第 $i 次"
    if [ $i -ge 3 ]; then
        break
    fi
    i=$((i + 1))
done

echo ""
echo "========== 4. break / continue / exit =========="

# -------------------- 4. break / continue / exit --------------------

echo "--- 4.1 break 跳出循环 ---"
# break 用于立即终止当前循环，跳出循环体
# break N 可以跳出 N 层循环（N 为数字）

echo "遇到3就跳出："
for i in 1 2 3 4 5; do
    if [ $i -eq 3 ]; then
        echo "  遇到 $i，跳出循环"
        break
    fi
    echo "  i = $i"
done

echo ""
echo "break 跳出多层循环："
for i in 1 2 3; do
    for j in 1 2 3; do
        echo "  i=$i, j=$j"
        if [ $i -eq 2 -a $j -eq 2 ]; then
            echo "  跳出2层循环"
            break 2                   # 跳出2层循环
        fi
    done
done

echo ""
echo "--- 4.2 continue 跳过本次循环 ---"
# continue 用于跳过本次循环的剩余部分，直接进入下一次循环
# continue N 可以跳过 N 层循环的当前迭代

echo "跳过偶数："
for i in {1..10}; do
    if [ $((i % 2)) -eq 0 ]; then
        continue                      # 跳过偶数
    fi
    echo "  奇数：$i"
done

echo ""
echo "--- 4.3 exit 退出脚本 ---"
# exit 用于立即退出整个脚本
# exit N 指定退出码（0 表示成功，非0表示失败）
# 注意：exit 退出的是整个脚本/进程，不只是循环

echo "exit 演示（本脚本不会真的退出，只是演示语法）"
echo "用法：exit 0   # 正常退出"
echo "用法：exit 1   # 错误退出"

echo ""
echo "========== 5. 循环重定向 =========="

# -------------------- 5. 循环重定向 --------------------
# 整个循环的输入输出都可以重定向

echo "--- 5.1 循环输出重定向 ---"
# 把整个循环的输出写入文件

echo "将循环输出写入文件..."
for i in {1..5}; do
    echo "这是第 $i 行"
done > loop_output.txt                # 整个循环的输出重定向到文件

echo "文件内容："
cat loop_output.txt
rm -f loop_output.txt                 # 清理

echo ""
echo "--- 5.2 循环输入重定向 ---"
# 从文件读取内容到循环

# 创建测试文件
cat > test_data.txt << 'EOF'
张三 25 北京
李四 30 上海
王五 28 广州
EOF

echo "从文件读取并解析："
while read name age city; do           # read 可以一次读多个变量
    echo "  姓名：$name，年龄：$age，城市：$city"
done < test_data.txt                   # 输入重定向

rm -f test_data.txt                    # 清理

echo ""
echo "--- 5.3 循环管道 ---"
# 循环的输出可以通过管道传给其他命令

echo "排序输出："
for fruit in banana apple cherry date; do
    echo "$fruit"
done | sort                            # 输出通过管道传给 sort 排序

echo ""
echo "========== 6. 管道与循环 =========="

# -------------------- 6. 管道与循环 --------------------
# 管道连接命令时，每个命令都在子 shell 中运行
# 这意味着在管道的循环中修改的变量，在循环外部是不可见的！

echo "--- 6.1 管道中的子 shell 问题 ---"

count=0
echo "a" "b" "c" | tr ' ' '\n' | while read line; do
    count=$((count + 1))
    echo "  循环内 count = $count"
done
echo "循环外 count = $count（子shell中修改不影响外部！）"

echo ""
echo "--- 6.2 解决方案一：使用进程替换 ---"
# 使用 <() 进程替换，避免子 shell 问题（Bash 特有）

count=0
while read line; do
    count=$((count + 1))
    echo "  循环内 count = $count"
done < <(echo -e "a\nb\nc")           # 进程替换
echo "循环外 count = $count（进程替换方式）"

echo ""
echo "--- 6.3 解决方案二：把结果写入临时文件 ---"

count=0
echo -e "a\nb\nc" > temp_lines.txt
while read line; do
    count=$((count + 1))
    echo "  循环内 count = $count"
done < temp_lines.txt
echo "循环外 count = $count（临时文件方式）"
rm -f temp_lines.txt

echo ""
echo "--- 6.4 正确遍历文件名（含空格）---"
# for-in 遍历文件名时，如果文件名包含空格会出问题
# 推荐用 find ... | while read 的方式

# 创建测试文件（含空格）
mkdir -p test_dir
touch "test_dir/file 1.txt" "test_dir/file 2.txt" "test_dir/file3.txt"

echo "用 while read 遍历文件（安全处理空格）："
find test_dir -type f -name "*.txt" | while read filepath; do
    echo "  找到文件：$filepath"
done

rm -rf test_dir                        # 清理

echo ""
echo "========== 7. 死循环与跳出 =========="

# -------------------- 7. 死循环与跳出 --------------------

echo "--- 7.1 几种死循环写法 ---"
echo "写法一：while true"
echo "写法二：while :"
echo "写法三：while [ 1 ]"
echo "写法四：for (( ; ; ))"

echo ""
echo "--- 7.2 死循环的实际应用 ---"
# 死循环常用于：
# - 服务监控脚本（不断检查服务状态）
# - 交互式菜单（等待用户输入，选择退出才结束）
# - 后台守护进程

echo "模拟服务监控（运行5次后退出）："
count=0
while true; do
    count=$((count + 1))
    echo "  [$count] 检查服务状态... 正常运行"
    sleep 0.1                         # 模拟检查间隔（0.1秒）
    if [ $count -ge 5 ]; then
        echo "  监控结束"
        break
    fi
done

echo ""
echo "--- 7.3 使用标志变量控制循环 ---"
# 除了用 break，也可以用标志变量控制循环结束

running=true
count=0

while $running; do
    count=$((count + 1))
    echo "  运行中... 第 $count 次"
    if [ $count -ge 3 ]; then
        running=false                 # 设置标志为 false，循环结束
        echo "  设置 running=false，循环将结束"
    fi
done

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 03_functions/01_functions.sh 了解函数"
