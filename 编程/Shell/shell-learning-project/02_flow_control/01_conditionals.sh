#!/bin/bash
# ==============================================================================
# 文件名：01_conditionals.sh
# 描述：Shell/Bash 流程控制 - 条件判断 if/case/test
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 01_conditionals.sh ===================="
echo ""

# -------------------- 1. if/then/elif/else/fi --------------------
echo "========== 1. if/then/elif/else/fi 基本语法 =========="

# if 语句的基本结构：
# if 条件命令; then
#     命令列表
# elif 条件命令; then
#     命令列表
# else
#     命令列表
# fi
#
# 注意：
# 1. if 后面跟的是命令，命令执行成功（退出码0）则条件成立
# 2. then 可以和 if 写在同一行（用分号分隔），也可以另起一行
# 3. fi 是 if 的结束标记（if 反过来拼）

# 简单的 if 示例
age=18
echo "年龄 = $age"

if [ $age -ge 18 ]; then             # if 关键字 + 条件表达式 + then
    echo "你已经成年了"               # 条件成立时执行
fi

echo ""

# if-else 示例
score=75
echo "分数 = $score"

if [ $score -ge 60 ]; then
    echo "恭喜，考试及格了！"
else
    echo "很遗憾，考试不及格。"
fi

echo ""

# if-elif-else 示例
score=85
echo "分数 = $score"

if [ $score -ge 90 ]; then
    echo "优秀！"
elif [ $score -ge 80 ]; then          # elif 相当于 else if
    echo "良好！"
elif [ $score -ge 60 ]; then
    echo "及格！"
else
    echo "不及格！"
fi

echo ""

# 嵌套 if 示例
echo "--- 嵌套 if 示例 ---"
is_student=true
age=20

if [ "$is_student" = true ]; then
    echo "你是学生"
    if [ $age -ge 18 ]; then
        echo "你是成年学生"
    else
        echo "你是未成年学生"
    fi
else
    echo "你不是学生"
fi

echo ""
echo "========== 2. test 命令与 [ ] 表达式 =========="

# -------------------- 2. test 命令与 [ ] 表达式 --------------------
# test 命令用于检查条件，返回真（0）或假（非0）
# [ ] 是 test 的另一种写法，功能完全相同
# 注意：[ ] 内部两边必须有空格！这是最常见的错误之一

echo "--- test 命令 ---"
# test 命令的两种等价写法
test 5 -gt 3
echo "test 5 -gt 3 的退出码：$?"      # 0 表示真

test 5 -lt 3
echo "test 5 -lt 3 的退出码：$?"      # 1 表示假

echo ""
echo "--- [ ] 表达式 ---"
# [ ] 是 test 的语法糖，用法完全一样
# [ 是一个命令（和 test 一样），] 是它的最后一个参数
# 所以 [ 和 ] 两边都必须有空格！

if [ 5 -gt 3 ]; then
    echo "5 大于 3"
fi

# 常见错误写法（取消注释会报错）：
# if [5 -gt 3]; then       # 错误：[ 后面没有空格
# if [ 5 -gt 3]; then      # 错误：] 前面没有空格
# if [5 -gt 3 ]; then      # 错误：[ 后面没有空格

echo ""
echo "--- [[ ]] 扩展测试（Bash 特有）---"
# [[ ]] 是 Bash 提供的增强版测试结构，比 [ ] 更强大
# 支持：
# - 字符串模式匹配（=~ 正则匹配）
# - 逻辑运算符更直观（&& || 代替 -a -o）
# - 不需要对变量加引号防止单词分割
# 注意：[[ ]] 是 Bash 特有，不兼容 POSIX sh

str1="hello"
str2="world"

if [[ $str1 < $str2 ]]; then          # 字符串比较，不需要转义 <
    echo "$str1 < $str2（按字典序）"
fi

# 正则匹配（=~ 操作符）
email="test@example.com"
if [[ $email =~ ^[a-zA-Z0-9]+@[a-zA-Z0-9]+\.[a-zA-Z]+$ ]]; then
    echo "$email 是一个有效的邮箱格式"
fi

echo ""
echo "========== 3. 字符串比较 =========="

# -------------------- 3. 字符串比较 --------------------
# 字符串比较运算符：
# =  或 == ：等于（== 是 Bash 扩展，[ ] 中推荐用 =）
# !=       ：不等于
# <        ：小于（按字典序）
# >        ：大于（按字典序）
# -z       ：长度为零（zero）
# -n       ：长度不为零（non-zero）

echo "--- 字符串相等比较 ---"
str_a="hello"
str_b="hello"
str_c="world"

if [ "$str_a" = "$str_b" ]; then      # 使用 = 比较相等
    echo "$str_a = $str_b"
fi

if [ "$str_a" != "$str_c" ]; then     # 使用 != 比较不等
    echo "$str_a != $str_c"
fi

# [[ ]] 中可以用 ==
if [[ $str_a == $str_b ]]; then
    echo "[[ ]] 中使用 ==：相等"
fi

echo ""
echo "--- 字符串大小比较（字典序）---"
# 在 [ ] 中，< 和 > 需要转义（否则会被当作重定向符号）
if [ "$str_a" \< "$str_c" ]; then
    echo "$str_a < $str_c（字典序）"
fi

# 在 [[ ]] 中，不需要转义
if [[ $str_a < $str_c ]]; then
    echo "[[ ]] 中 $str_a < $str_c"
fi

echo ""
echo "--- -z 和 -n 判断 ---"
empty_str=""
non_empty_str="hello"

if [ -z "$empty_str" ]; then          # -z：字符串长度为 0
    echo "empty_str 是空字符串"
fi

if [ -n "$non_empty_str" ]; then      # -n：字符串长度不为 0
    echo "non_empty_str 不是空字符串"
fi

# 也可以直接判断（不推荐，不清晰）
if [ "$non_empty_str" ]; then         # 非空即为真
    echo "直接判断：non_empty_str 非空"
fi

echo ""
echo "========== 4. 数值比较 =========="

# -------------------- 4. 数值比较 --------------------
# 数值比较运算符（用于整数）：
# -eq ：等于（equal）
# -ne ：不等于（not equal）
# -gt ：大于（greater than）
# -lt ：小于（less than）
# -ge ：大于等于（greater than or equal）
# -le ：小于等于（less than or equal）
#
# 注意：这些是 test/[ ] 中的运算符，不要和 > < 混淆
# > < 是字符串比较，-gt -lt 是数值比较

echo "--- 数值比较示例 ---"
num1=10
num2=20
num3=10

echo "num1=$num1, num2=$num2, num3=$num3"

if [ $num1 -eq $num3 ]; then
    echo "num1 -eq num3：$num1 等于 $num3"
fi

if [ $num1 -ne $num2 ]; then
    echo "num1 -ne num2：$num1 不等于 $num2"
fi

if [ $num2 -gt $num1 ]; then
    echo "num2 -gt num1：$num2 大于 $num1"
fi

if [ $num1 -lt $num2 ]; then
    echo "num1 -lt num2：$num1 小于 $num2"
fi

if [ $num1 -ge $num3 ]; then
    echo "num1 -ge num3：$num1 大于等于 $num3"
fi

if [ $num1 -le $num2 ]; then
    echo "num1 -le num2：$num1 小于等于 $num2"
fi

echo ""
echo "--- (( )) 算术判断 ---"
# (( )) 算术表达式也可以用于条件判断
# 表达式结果非零则为真，为零则为假
# 这种写法更接近 C 语言风格

if (( num1 < num2 )); then
    echo "(( num1 < num2 ))：$num1 < $num2"
fi

if (( num1 == num3 )); then
    echo "(( num1 == num3 ))：$num1 == $num3"
fi

if (( num1 + num2 > 25 )); then
    echo "(( num1 + num2 > 25 ))：$num1 + $num2 = $((num1+num2)) > 25"
fi

echo ""
echo "========== 5. 文件测试 =========="

# -------------------- 5. 文件测试 --------------------
# test/[ ] 提供了丰富的文件测试运算符
# 常用文件测试运算符：
# -e 文件 ：文件存在（exist）
# -f 文件 ：文件存在且是普通文件（file）
# -d 文件 ：文件存在且是目录（directory）
# -r 文件 ：文件存在且可读（readable）
# -w 文件 ：文件存在且可写（writable）
# -x 文件 ：文件存在且可执行（executable）
# -s 文件 ：文件存在且大小不为0（size）
# -L 文件 ：文件存在且是符号链接（link）
# -O 文件 ：文件存在且属主是当前用户（owner）
# -G 文件 ：文件存在且属组是当前用户组（group）
# 文件1 -nt 文件2 ：文件1比文件2新（newer than）
# 文件1 -ot 文件2 ：文件1比文件2旧（older than）

# 创建测试用的文件和目录
test_dir="./test_dir"
test_file="./test_dir/test.txt"
mkdir -p "$test_dir"
echo "测试内容" > "$test_file"

echo "--- 文件测试示例 ---"
echo "测试目录：$test_dir"
echo "测试文件：$test_file"
echo ""

# -e 检查文件是否存在
if [ -e "$test_file" ]; then
    echo "-e：$test_file 存在"
fi

# -f 检查是否是普通文件
if [ -f "$test_file" ]; then
    echo "-f：$test_file 是普通文件"
fi

# -d 检查是否是目录
if [ -d "$test_dir" ]; then
    echo "-d：$test_dir 是目录"
fi

# -r 检查是否可读
if [ -r "$test_file" ]; then
    echo "-r：$test_file 可读"
fi

# -w 检查是否可写
if [ -w "$test_file" ]; then
    echo "-w：$test_file 可写"
fi

# -x 检查是否可执行
if [ -x "$test_file" ]; then
    echo "-x：$test_file 可执行"
else
    echo "-x：$test_file 不可执行"
fi

# -s 检查文件大小是否不为0
if [ -s "$test_file" ]; then
    echo "-s：$test_file 大小不为0"
fi

# 新建一个空文件测试
empty_file="$test_dir/empty.txt"
touch "$empty_file"
if [ -s "$empty_file" ]; then
    echo "-s：$empty_file 大小不为0"
else
    echo "-s：$empty_file 是空文件"
fi

# 清理测试文件
rm -rf "$test_dir"

echo ""
echo "========== 6. 逻辑运算符 =========="

# -------------------- 6. 逻辑与/或/非 --------------------

echo "--- [ ] 中的逻辑运算符 ---"
# 在 [ ] 或 test 中：
# -a ：逻辑与（and），两边都为真则结果为真
# -o ：逻辑或（or），有一边为真则结果为真
# !  ：逻辑非（not），取反

age=25
name="张三"

# -a 逻辑与
if [ $age -gt 18 -a $age -lt 30 ]; then
    echo "-a：年龄在 18 到 30 之间"
fi

# -o 逻辑或
if [ "$name" = "张三" -o "$name" = "李四" ]; then
    echo "-o：名字是张三或李四"
fi

# ! 逻辑非
if [ ! -d "/nonexistent_dir_12345" ]; then
    echo "!：目录不存在"
fi

echo ""
echo "--- [[ ]] 中的逻辑运算符 ---"
# 在 [[ ]] 中可以使用更直观的 && 和 ||

if [[ $age -gt 18 && $age -lt 30 ]]; then
    echo "&&：年龄在 18 到 30 之间"
fi

if [[ $name == "张三" || $name == "李四" ]]; then
    echo "||：名字是张三或李四"
fi

if [[ ! -d "/nonexistent_dir_12345" ]]; then
    echo "!：目录不存在"
fi

echo ""
echo "--- 多个 [ ] 用 && || 连接 ---"
# 也可以用多个 [ ] 配合 shell 的 && || 运算符
# 这种方式更清晰，也更安全（每个测试独立）

if [ $age -gt 18 ] && [ $age -lt 30 ]; then
    echo "多个 [ ] 用 && 连接：年龄在 18 到 30 之间"
fi

if [ "$name" = "张三" ] || [ "$name" = "李四" ]; then
    echo "多个 [ ] 用 || 连接：名字是张三或李四"
fi

echo ""
echo "========== 7. case/esac 模式匹配 =========="

# -------------------- 7. case/esac 模式匹配 --------------------
# case 语句用于多分支选择，相当于其他语言的 switch/case
# 语法：
# case 变量 in
#     模式1)
#         命令列表
#         ;;
#     模式2)
#         命令列表
#         ;;
#     *)
#         默认命令列表
#         ;;
# esac
#
# 特点：
# - 支持通配符模式匹配（* ? [ ]）
# - 每个分支以 ;; 结束（相当于 break）
# - 可以用 | 表示或（多个模式共用一个分支）
# - *) 是默认分支（可选）
# - esac 是 case 的结束标记（case 反过来拼）

echo "--- case 基本用法 ---"
day="周一"

case "$day" in
    "周一")
        echo "今天是星期一，新的一周开始了！"
        ;;
    "周二"|"周三"|"周四")
        echo "今天是工作日，努力工作！"
        ;;
    "周五")
        echo "今天是星期五，周末就要到了！"
        ;;
    "周六"|"周日")
        echo "今天是周末，好好休息！"
        ;;
    *)
        echo "无效的日期：$day"
        ;;
esac

echo ""
echo "--- case 通配符模式 ---"
# case 支持 shell 通配符：
# *  匹配任意字符（任意长度，包括空）
# ?  匹配任意单个字符
# [] 匹配括号内的任意一个字符

filename="script.sh"

case "$filename" in
    *.sh)
        echo "$filename 是 Shell 脚本文件"
        ;;
    *.txt)
        echo "$filename 是文本文件"
        ;;
    *.log)
        echo "$filename 是日志文件"
        ;;
    *)
        echo "$filename 是其他类型的文件"
        ;;
esac

echo ""
echo "--- case 字符范围匹配 ---"
grade="B"

case "$grade" in
    [A])
        echo "优秀！"
        ;;
    [B-C])
        echo "良好！"
        ;;
    [D])
        echo "及格"
        ;;
    [F])
        echo "不及格"
        ;;
    *)
        echo "无效的等级"
        ;;
esac

echo ""
echo "--- 用 case 判断命令行参数 ---"
# case 常用于处理命令行参数
action="start"

case "$action" in
    start)
        echo "启动服务..."
        ;;
    stop)
        echo "停止服务..."
        ;;
    restart)
        echo "重启服务..."
        ;;
    status)
        echo "查看服务状态..."
        ;;
    *)
        echo "用法：$0 {start|stop|restart|status}"
        exit 1
        ;;
esac

echo ""
echo "========== 8. 短路求值技巧 =========="

# -------------------- 8. 短路求值技巧 --------------------
# 在 Bash 中，&& 和 || 可以用来简化简单的条件判断
# 这叫做短路求值（short-circuit evaluation）
#
# 命令1 && 命令2 ：如果命令1成功（退出码0），才执行命令2
# 命令1 || 命令2 ：如果命令1失败（退出码非0），才执行命令2

echo "--- && 短路与 ---"
# 只有前面的命令成功，才会执行后面的命令
# 相当于：if 命令1; then 命令2; fi

# 示例：如果目录不存在就创建
test_dir="./temp_test_dir"
[ -d "$test_dir" ] || mkdir "$test_dir"   # 目录不存在则创建
echo "目录 $test_dir 已创建"

# 示例：如果文件存在就删除
[ -f "$test_dir/test.txt" ] && rm "$test_dir/test.txt"
echo "如果文件存在则已删除"

# 清理
rmdir "$test_dir"

echo ""
echo "--- || 短路或 ---"
# 只有前面的命令失败，才会执行后面的命令
# 相当于：if ! 命令1; then 命令2; fi

# 示例：变量不存在时使用默认值
# （这种用法更多用 ${var:-default}，这里演示短路求值）
undefined_var=""
echo "${undefined_var:-默认值}"           # 更推荐这种写法

# 示例：命令失败时输出错误信息
ls /nonexistent_file_12345 2>/dev/null || echo "文件不存在"

echo ""
echo "--- && 和 || 组合使用 ---"
# 可以组合使用，实现简单的 if-then-else
# 命令1 && 命令2 || 命令3
# 注意：这种写法有坑！如果命令2失败，命令3也会执行
# 所以只有在命令2一定成功的情况下才安全

# 安全的用法（命令2是 echo，肯定成功）
age=20
[ $age -ge 18 ] && echo "成年" || echo "未成年"

# 等价的 if 写法更安全：
if [ $age -ge 18 ]; then
    echo "成年"
else
    echo "未成年"
fi

# 注意：不要在不确定命令2是否成功时使用 && || 组合代替 if-else
# 例如下面的写法可能有问题：
# [ -f file ] && rm file || echo "删除失败"
# 如果 rm 命令失败（比如权限不够），也会执行 echo "删除失败"
# 这可能不是你想要的结果

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 02_loops.sh 了解循环结构"
