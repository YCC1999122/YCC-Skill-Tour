#!/bin/bash
# ==============================================================================
# 文件名：01_advanced.sh
# 描述：Shell/Bash 高级技巧 - 重定向、管道、子shell、信号、正则、调试等
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 01_advanced.sh ===================="
echo ""

# -------------------- 1. 输入输出重定向 --------------------
echo "========== 1. 输入输出重定向 =========="

# Linux 中每个进程都有三个标准文件描述符：
# 0 - 标准输入（stdin）  ：键盘输入
# 1 - 标准输出（stdout） ：屏幕输出
# 2 - 标准错误（stderr） ：错误信息输出
# 重定向就是改变这些描述符的默认来源/去向

echo "--- 1.1 输出重定向 > 和 >> ---"

# > ：覆盖写入（如果文件不存在则创建，存在则清空后写入）
# >>：追加写入（如果文件不存在则创建，存在则在末尾追加）

echo "创建文件并写入内容（>）："
echo "第一行内容" > redirect_test.txt
echo "第二行内容" >> redirect_test.txt   # 追加
cat redirect_test.txt

echo ""
echo "> 会覆盖原有内容："
echo "新内容覆盖了" > redirect_test.txt
cat redirect_test.txt

echo ""
echo "--- 1.2 输入重定向 < ---"

# < ：从文件读取输入，而不是从键盘

echo "用 < 从文件读取："
cat < redirect_test.txt

# 统计文件行数（两种方式等价）
echo "wc -l < redirect_test.txt："
wc -l < redirect_test.txt

echo ""
echo "--- 1.3 标准错误重定向 2> ---"
# 2> ：重定向标准错误（文件描述符2）

echo "错误输出重定向到文件："
ls /nonexistent_dir_12345 2> error.log
echo "错误内容："
cat error.log
rm -f error.log

echo ""
echo "--- 1.4 同时重定向 stdout 和 stderr ---"

# 方式一：2>&1（把 stderr 重定向到 stdout 的去向）
# 顺序很重要：> file 2>&1 正确，2>&1 > file 错误
echo "方式一：> file 2>&1"
ls /etc /nonexistent_dir_12345 > all_output.log 2>&1
echo "（输出已保存到文件，此处不显示）"
rm -f all_output.log

# 方式二：&> （Bash 4+ 支持，直接重定向 stdout 和 stderr）
echo "方式二：&> file（Bash 4+）"
ls /etc /nonexistent_dir_12345 &> all_output2.log
rm -f all_output2.log

echo ""
echo "--- 1.5 丢弃输出 /dev/null ---"
# /dev/null 是一个特殊的设备文件，写入的内容都会被丢弃
# 常用于屏蔽不需要的输出

echo "丢弃正常输出："
ls /etc > /dev/null                    # 正常输出丢弃，错误仍显示
echo "（正常输出已丢弃）"

echo "丢弃所有输出："
ls /etc /nonexistent_xyz > /dev/null 2>&1
echo "（所有输出都已丢弃）"

echo ""
echo "--- 1.6 Here Document（here 文档）---"
# << 标记 ... 标记
# 将多行文本作为输入传给命令
# 标记可以是任意字符串，通常用 EOF（End Of File）

echo "Here Document 示例："
cat << 'MENDOF'
====================
  这里是 Here Document
  可以包含多行文本
  单引号包裹标记可以防止变量解析
====================
MENDOF

# 如果标记不加引号，内部的变量和命令会被解析
name="张三"
echo ""
echo "带变量解析的 Here Document："
cat << EOF
你好，$name！
当前日期：$(date +%Y-%m-%d)
EOF

echo ""
echo "--- 1.7 Here String ---"
# <<< 字符串
# 将单行字符串作为输入传给命令（比 echo | 更高效）

echo "Here String 示例："
cat <<< "这是一行文本通过 here string 传递"

# 常用于给需要从 stdin 读取的命令传参
echo "用 bc 计算（here string）："
bc <<< "5.5 * 2"

# 清理
rm -f redirect_test.txt

echo ""
echo "========== 2. 管道 pipe =========="

# -------------------- 2. 管道 pipe --------------------
# 管道符 | 用于将前一个命令的 stdout 作为后一个命令的 stdin
# 语法：命令1 | 命令2 | 命令3 ...
# 管道中的每个命令都在独立的子 shell 中运行

echo "--- 2.1 基本管道 ---"

echo "统计 .sh 文件数量："
ls *.sh 2>/dev/null | wc -l

echo "排序并去重："
echo -e "banana\napple\ncherry\napple\nbanana" | sort | uniq

echo ""
echo "--- 2.2 管道链（多个管道）---"

echo "取日志中 ERROR 行的第3列（时间）："
# 模拟日志
echo -e "2024-01-15 10:23:45 [ERROR] 错误1\n2024-01-15 10:24:01 [ERROR] 错误2" | \
    grep "ERROR" | \
    cut -d' ' -f2

echo ""
echo "--- 2.3 tee 分流 ---"
# tee 命令把 stdin 的内容同时输出到 stdout 和文件
# 相当于管道的分流器

echo "tee 同时输出到屏幕和文件："
echo "hello tee" | tee tee_test.txt
echo "文件内容：$(cat tee_test.txt)"
rm -f tee_test.txt

echo "tee -a 追加模式："
echo "第一行" | tee tee_test2.txt
echo "第二行" | tee -a tee_test2.txt
cat tee_test2.txt
rm -f tee_test2.txt

echo ""
echo "========== 3. 子shell与父shell =========="

# -------------------- 3. 子shell与父shell --------------------
# 子 shell 是从当前 shell 派生出来的新 shell 进程
# 子 shell 会继承父 shell 的环境变量、当前目录等
# 但子 shell 中对变量的修改不会影响父 shell
#
# 触发子 shell 的情况：
# 1. 管道中的每个命令
# 2. () 括号中的命令组
# 3. 执行外部脚本或命令
# 4. 后台运行的命令 &

echo "--- 3.1 () 创建子 shell ---"
# (命令列表) 会在子 shell 中执行

outer_var="父shell的值"

(
    # 这是在子 shell 中
    inner_var="子shell的值"
    outer_var="子shell修改后的值"
    echo "子shell内：outer_var = $outer_var"
    echo "子shell内：inner_var = $inner_var"
)

echo "父shell中：outer_var = $outer_var"
echo "父shell中：inner_var = ${inner_var:-不存在}"

echo ""
echo "--- 3.2 子 shell 不影响父 shell 的目录 ---"

current_dir=$(pwd)
echo "当前目录：$current_dir"

(
    cd /tmp 2>/dev/null || cd /
    echo "子shell中切换目录：$(pwd)"
)

echo "回到父shell，目录仍然是：$(pwd)"

echo ""
echo "--- 3.3 {} 当前 shell 执行 ---"
# { 命令列表; } 在当前 shell 中执行（不创建子shell）
# 注意：左括号后要有空格，右括号前要有分号或换行

{
    test_var="当前shell的值"
    echo "在 {} 中定义变量"
}

echo "在 {} 外可以访问：test_var = $test_var"

echo ""
echo "--- 3.4 管道中的子 shell 问题 ---"
# 这是一个经典的坑：管道右边的命令在子 shell 中运行
# 所以在 while read 中修改的变量，循环外面看不到

count=0
echo -e "line1\nline2\nline3" | while read line; do
    count=$((count + 1))
done
echo "管道方式，循环外 count = $count（子shell中修改不影响外部）"

# 解决方法：用进程替换 <()
count=0
while read line; do
    count=$((count + 1))
done < <(echo -e "line1\nline2\nline3")
echo "进程替换方式，循环外 count = $count"

echo ""
echo "========== 4. 信号 trap =========="

# -------------------- 4. 信号 trap --------------------
# trap 命令用于捕获信号，并执行指定的命令
# 常见信号：
# INT (2) ：中断信号，Ctrl+C 触发
# TERM (15)：终止信号，kill 默认发送
# EXIT ：脚本退出时触发（无论正常还是异常退出）
# HUP (1) ：挂起信号
# QUIT (3)：退出信号，Ctrl+\ 触发

echo "--- 4.1 trap 基本用法 ---"
# 语法：trap '命令' 信号

# 设置一个清理函数
cleanup() {
    echo ""
    echo "收到退出信号，正在清理..."
    rm -f temp_*.txt 2>/dev/null
    echo "清理完成"
}

# 捕获 EXIT 信号（脚本退出时执行）
trap cleanup EXIT

echo "已设置 trap，脚本退出时会执行清理"
echo "创建临时文件 temp_test.txt..."
echo "临时内容" > temp_test.txt

echo ""
echo "--- 4.2 捕获 Ctrl+C ---"
# INT 信号是 Ctrl+C 触发的

# 定义一个专门处理 INT 的函数
handle_int() {
    echo ""
    echo "你按下了 Ctrl+C！"
    echo "确认退出吗？(y/n)"
    # 这里简化处理，直接退出
    exit 1
}

# 注意：本脚本是自动演示，不会真的等待 Ctrl+C
# 实际使用时取消下面注释即可
# trap handle_int INT

echo "（INT 信号捕获已注释，本脚本自动运行）"

echo ""
echo "--- 4.3 查看信号列表 ---"
echo "使用 trap -l 可以查看所有信号："
trap -l | head -3

echo ""
echo "========== 5. 进程替换 <() >() =========="

# -------------------- 5. 进程替换 <() >() --------------------
# 进程替换是 Bash 的高级特性（不兼容 POSIX sh）
# <(命令) ：将命令的输出当作一个文件（可读）
# >(命令) ：将命令的输入当作一个文件（可写）
# 本质是创建一个命名管道（fifo）或 /dev/fd/N 文件

echo "--- 5.1 <() 输入进程替换 ---"
# 常用于需要文件参数的地方，用命令输出来替代

echo "比较两个命令的输出："
# diff 通常用来比较两个文件，用 <() 可以直接比较命令输出
diff <(echo -e "a\nb\nc") <(echo -e "a\nb\nd") || echo "（有差异是正常的）"

echo ""
echo "while read 中使用（避免子 shell）："
count=0
while read line; do
    count=$((count + 1))
done < <(echo -e "第一行\n第二行\n第三行")
echo "读取了 $count 行"

echo ""
echo "--- 5.2 >() 输出进程替换 ---"
# 把输出重定向到另一个命令的输入

echo "输出同时经过两个过滤器："
echo -e "apple\nbanana\ncherry" | tee >(tr 'a-z' 'A-Z' > upper.txt) >(sort -r > reversed.txt) > /dev/null
echo "大写版本：$(cat upper.txt | tr '\n' ' ')"
echo "逆序版本：$(cat reversed.txt | tr '\n' ' ')"
rm -f upper.txt reversed.txt

echo ""
echo "========== 6. 正则表达式基础 =========="

# -------------------- 6. 正则表达式基础 --------------------
# 正则表达式是文本匹配的强大工具
# Bash 中常见的使用场景：grep, sed, awk, [[ =~ ]]

echo "--- 6.1 基本元字符 ---"

test_str="Hello World 123! email@example.com"

echo "测试字符串：$test_str"
echo ""

echo "常用元字符："
echo "  .    匹配任意单个字符（除换行符）"
echo "  *    匹配前面的字符0次或多次"
echo "  +    匹配前面的字符1次或多次（扩展正则）"
echo "  ?    匹配前面的字符0次或1次（扩展正则）"
echo "  ^    匹配行开头"
echo "  $    匹配行结尾"
echo "  []   匹配括号内的任意一个字符"
echo "  [^]  匹配不在括号内的任意字符"
echo "  ()   分组（扩展正则）"
echo "  |    或（扩展正则）"
echo "  \d   数字（Perl正则，等价于[0-9]）"
echo "  \w   单词字符（字母数字下划线）"
echo "  \s   空白字符"

echo ""
echo "--- 6.2 [[ =~ ]] 正则匹配 ---"
# Bash 中可以用 [[ 字符串 =~ 正则 ]] 进行正则匹配
# 匹配成功返回0（真），失败返回1（假）

email="test123@example.com"
echo "邮箱验证：$email"
if [[ $email =~ ^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$ ]]; then
    echo "  是有效的邮箱格式"
else
    echo "  不是有效的邮箱格式"
fi

phone="13812345678"
echo "手机号验证：$phone"
if [[ $phone =~ ^1[3-9][0-9]{9}$ ]]; then
    echo "  是有效的手机号格式"
else
    echo "  不是有效的手机号格式"
fi

echo ""
echo "--- 6.3 捕获组 ---"
# [[ =~ ]] 的匹配结果会保存在 BASH_REMATCH 数组中
# BASH_REMATCH[0] 是整个匹配的内容
# BASH_REMATCH[1] 是第一个捕获组，以此类推

str="今天是2024年01月15日，天气不错"
echo "字符串：$str"
if [[ $str =~ ([0-9]{4})年([0-9]{2})月([0-9]{2})日 ]]; then
    echo "  完整匹配：${BASH_REMATCH[0]}"
    echo "  年：${BASH_REMATCH[1]}"
    echo "  月：${BASH_REMATCH[2]}"
    echo "  日：${BASH_REMATCH[3]}"
fi

echo ""
echo "========== 7. 脚本调试 =========="

# -------------------- 7. 脚本调试 --------------------
# Bash 提供了几种调试手段：set 选项

echo "--- 7.1 set -x 跟踪执行 ---"
# set -x 开启调试模式，每条命令执行前都会打印出来
# set +x 关闭调试模式

echo "开启 set -x 调试（只演示3行）："
set -x
var1=10
var2=20
result=$((var1 + var2))
set +x
echo "调试结束，result = $result"

echo ""
echo "--- 7.2 set -e 遇到错误立即退出 ---"
# set -e 让脚本在任何命令返回非0时立即退出
# 注意：不是所有非0都是错误，有时需要临时关闭

echo "set -e 的作用（本脚本不启用，只是演示）"
echo "启用后：只要有一条命令失败，脚本就立即退出"
echo "用法：set -e  或  set -o errexit"

echo ""
echo "--- 7.3 set -u 未定义变量报错 ---"
# set -u 使用未定义的变量时报错（而不是当作空）
# 有助于发现拼写错误的变量名

echo "set -u 的作用（本脚本不启用）"
echo "启用后：使用未定义的变量会报错并退出"
echo "用法：set -u  或  set -o nounset"

echo ""
echo "--- 7.4 组合使用 ---"
echo "脚本开头常用组合："
echo "  set -euo pipefail"
echo "  -e: 错误即退出"
echo "  -u: 未定义变量报错"
echo "  -o pipefail: 管道中任何命令失败，整体返回失败"

echo ""
echo "--- 7.5 bash -x 调试脚本 ---"
echo "也可以在运行脚本时指定调试："
echo "  bash -x script.sh"
echo "  bash -v script.sh   （同时打印原始行）"
echo "  bash -n script.sh   （只检查语法，不执行）"

echo ""
echo "========== 8. getopts 命令行参数解析 =========="

# -------------------- 8. getopts 命令行参数解析 --------------------
# getopts 是 Bash 内置的命令行参数解析工具
# 语法：getopts 选项字符串 变量名
# 选项字符串定义了支持的选项：
#   - 单个字母表示该选项不需要参数
#   - 字母后跟 : 表示该选项需要参数
#   - 开头的 : 表示静默错误模式

echo "--- 8.1 getopts 基本用法 ---"

# 定义一个演示函数（模拟接收命令行参数）
parse_args() {
    local verbose=0
    local output=""
    local count=0

    # 选项：h（帮助）、v（详细）、o:（输出文件，需参数）、n:（数量，需参数）
    while getopts "hvo:n:" opt; do
        case $opt in
            h)
                echo "用法：命令 [-h] [-v] [-o 文件] [-n 数量]"
                return 0
                ;;
            v)
                verbose=1
                echo "详细模式已开启"
                ;;
            o)
                output=$OPTARG              # OPTARG 保存选项的参数
                echo "输出文件：$output"
                ;;
            n)
                count=$OPTARG
                echo "数量：$count"
                ;;
            \?)
                echo "无效选项：-$OPTARG"
                return 1
                ;;
        esac
    done

    # OPTIND 指向第一个非选项参数的位置
    shift $((OPTIND - 1))
    echo "剩余参数：$@"
}

echo "调用 parse_args -v -o output.txt -n 10 file1 file2："
parse_args -v -o output.txt -n 10 file1 file2

echo ""
echo "调用 parse_args -h："
parse_args -h

echo ""
echo "--- 8.2 长选项（getopt 外部命令）---"
echo "getopts 只支持短选项（-h -v 等）"
echo "如果需要长选项（--help --version 等），可以使用外部命令 getopt"
echo "或者自己用 case 解析 \$1"

echo ""
echo "========== 9. 错误处理与退出码 =========="

# -------------------- 9. 错误处理与退出码 --------------------
# 退出码（Exit Code）：
# 0  - 成功
# 1  - 通用错误
# 2  - 误用命令（如参数错误）
# 126 - 无法执行（权限不够）
# 127 - 命令未找到
# 128 + N - 被信号 N 终止
# 130 - 被 Ctrl+C 终止（128+2）

echo "--- 9.1 查看退出码 \$? ---"

ls /tmp > /dev/null 2>&1
echo "ls /tmp 的退出码：$?"

ls /nonexistent_dir_12345 > /dev/null 2>&1
echo "ls 不存在目录的退出码：$?"

echo ""
echo "--- 9.2 exit 命令 ---"
# exit N 可以指定退出码
echo "exit 0  正常退出"
echo "exit 1  错误退出"
echo "exit 2  用法错误"

echo ""
echo "--- 9.3 错误处理模式 ---"

# 模式一：检查每个命令的返回值
echo "模式一：检查返回值"
if ls /nonexistent_xyz > /dev/null 2>&1; then
    echo "命令成功"
else
    echo "命令失败（这是预期的）"
fi

echo ""
echo "模式二：命令失败时输出错误并退出"
# 常用写法：command || { echo "error"; exit 1; }
# 注意：大括号内的最后一条命令要以分号或换行结束

# 示例（注释掉，因为真的会退出）：
# ls /nonexistent_xyz || { echo "错误：命令执行失败"; exit 1; }

echo ""
echo "模式三：自定义错误函数"

error_exit() {
    echo "错误：$1" >&2               # 错误信息输出到 stderr
    exit 1
}

# 使用示例（注释掉）：
# ls /nonexistent_xyz || error_exit "文件不存在"

echo ""
echo "--- 9.4 输出到 stderr ---"
# 错误信息应该输出到 stderr（文件描述符2）
# 这样用户可以用 2> 单独重定向错误信息

echo "这是正常输出（stdout）"
echo "这是错误信息（stderr）" >&2

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 06_examples/01_utils.sh 了解综合案例"
