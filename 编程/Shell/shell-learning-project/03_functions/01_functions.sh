#!/bin/bash
# ==============================================================================
# 文件名：01_functions.sh
# 描述：Shell/Bash 函数 - 定义、调用、参数、返回值、递归、函数库
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 01_functions.sh ===================="
echo ""

# -------------------- 1. 函数定义（两种语法）--------------------
echo "========== 1. 函数定义（两种语法）=========="

# --- 语法一：function 关键字 ---
# function 函数名() {
#     函数体
# }
function hello1() {
    echo "你好，这是用 function 关键字定义的函数"
}

# --- 语法二：省略 function 关键字（推荐，更兼容）---
# 函数名() {
#     函数体
# }
hello2() {
    echo "你好，这是省略 function 关键字的写法"
}

# --- 语法三：函数名后面没有括号（不推荐）---
# function 函数名 {
#     函数体
# }
function hello3 {
    echo "你好，这是没有括号的写法"
}

echo "--- 函数调用 ---"
# 函数调用直接写函数名即可，不需要加括号
hello1
hello2
hello3

echo ""
echo "========== 2. 函数调用 =========="

# -------------------- 2. 函数调用 --------------------

# 简单的函数
greet() {
    echo "Hello, World!"
}

echo "第一次调用："
greet

echo "第二次调用："
greet

# 函数可以在脚本的任何位置定义，但必须在调用之前定义
# （函数定义和变量类似，都是顺序执行的）

echo ""
echo "========== 3. 函数参数 =========="

# -------------------- 3. 函数参数 --------------------
# Bash 函数的参数和脚本参数使用相同的特殊变量：
# $1, $2, $3 ... ：第1、2、3...个参数
# $# ：参数个数
# $@ ：所有参数（每个独立）
# $* ：所有参数（合并为一个字符串）
# $0 ：注意！函数内的 $0 仍然是脚本名，不是函数名

echo "--- 3.1 位置参数 ---"

# 定义一个带参数的函数
say_hello() {
    echo "你好，$1！"                 # $1 是第一个参数
    echo "你今年 $2 岁"               # $2 是第二个参数
}

echo "调用 say_hello 张三 25："
say_hello "张三" 25

echo ""
echo "调用 say_hello 李四 30："
say_hello "李四" 30

echo ""
echo "--- 3.2 参数个数 \$# ---"

show_args() {
    echo "参数个数：$#"
    echo "第一个参数：$1"
    echo "第二个参数：${2:-无}"        # 如果没有第二个参数，显示"无"
    echo "所有参数：$@"
}

echo "调用 show_args a b c："
show_args a b c

echo ""
echo "调用 show_args 只有一个参数："
show_args "只有一个"

echo ""
echo "--- 3.3 遍历所有参数 ---"

print_all_args() {
    echo "用 for 遍历 \$@："
    for arg in "$@"; do
        echo "  参数：$arg"
    done
}

echo "调用 print_all_args 苹果 香蕉 橙子："
print_all_args "苹果" "香蕉" "橙子"

echo ""
echo "--- 3.4 传递数组参数 ---"
# 注意：不能直接传递数组，需要把数组元素展开传递

print_array() {
    echo "函数收到的参数列表："
    for item in "$@"; do
        echo "  $item"
    done
}

my_array=("红色" "绿色" "蓝色")
echo "传递数组参数："
print_array "${my_array[@]}"          # 把数组展开为多个参数传递

echo ""
echo "========== 4. return 返回值 =========="

# -------------------- 4. return 返回值 --------------------
# Bash 函数的 return 语句用于返回退出状态码（0-255的整数）
# 这和其他语言的 return 不同！Bash 的 return 只能返回数字
# 返回字符串需要用 echo + 命令替换（见下一节）

echo "--- 4.1 return 返回状态码 ---"

# 判断数字是否为偶数
is_even() {
    if [ $(( $1 % 2 )) -eq 0 ]; then
        return 0                      # 0 表示真（成功）
    else
        return 1                      # 非0表示假（失败）
    fi
}

echo "判断 4 是否为偶数："
if is_even 4; then                    # 函数的返回值可以直接作为 if 的条件
    echo "  是偶数"
else
    echo "  是奇数"
fi

echo "判断 5 是否为偶数："
if is_even 5; then
    echo "  是偶数"
else
    echo "  是奇数"
fi

echo ""
echo "--- 4.2 获取返回值 \$? ---"

add_numbers() {
    sum=$(( $1 + $2 ))
    return $sum                       # 返回计算结果（注意：只能是0-255）
}

add_numbers 10 20
echo "10 + 20 = $?"                   # $? 获取上一个命令/函数的返回值

echo ""
echo "--- 4.3 return 的限制 ---"
# return 只能返回 0-255 的整数，超过会溢出（对256取模）
echo "返回值只能是 0-255，超过会取模："
add_numbers 200 100                   # 200+100=300，300%256=44
echo "200 + 100 的返回值 = $?"
echo "（300 对 256 取模 = $((300 % 256))）"

echo ""
echo "========== 5. 函数返回字符串 =========="

# -------------------- 5. 函数返回字符串（echo + 命令替换）--------------------
# Bash 函数不能直接返回字符串，但可以通过以下方式实现：
# 1. 在函数中用 echo 输出结果
# 2. 调用时用 $() 捕获输出
# 这是 Bash 中返回字符串的标准做法

echo "--- 5.1 使用 echo 返回字符串 ---"

get_greeting() {
    local hour=$(date +%H)            # 获取当前小时
    if [ $hour -lt 12 ]; then
        echo "早上好"
    elif [ $hour -lt 18 ]; then
        echo "下午好"
    else
        echo "晚上好"
    fi
}

# 用命令替换捕获函数的输出
greeting=$(get_greeting)
echo "问候语：$greeting"

echo ""
echo "--- 5.2 返回计算结果的字符串形式 ---"

get_factorial() {
    local n=$1
    local result=1
    for (( i=1; i<=n; i++ )); do
        result=$((result * i))
    done
    echo $result                       # 用 echo 输出结果
}

# 调用函数并获取结果
fact_5=$(get_factorial 5)
echo "5 的阶乘 = $fact_5"

fact_10=$(get_factorial 10)
echo "10 的阶乘 = $fact_10"

echo ""
echo "--- 5.3 返回多个值 ---"
# 可以输出多个值，用分隔符分隔，调用者再解析

get_user_info() {
    local name="张三"
    local age=25
    local city="北京"
    echo "$name,$age,$city"            # 用逗号分隔多个返回值
}

# 调用并解析
info=$(get_user_info)
name=$(echo "$info" | cut -d',' -f1)
age=$(echo "$info" | cut -d',' -f2)
city=$(echo "$info" | cut -d',' -f3)
echo "姓名：$name，年龄：$age，城市：$city"

echo ""
echo "========== 6. 局部变量 local =========="

# -------------------- 6. 局部变量 local --------------------
# 函数内的变量默认是全局变量，函数外部也能访问
# 使用 local 关键字可以定义局部变量，只在函数内部有效
# 注意：local 只能在函数内部使用

echo "--- 6.1 全局变量的问题 ---"

global_var="初始值"

modify_global() {
    global_var="被函数修改了"          # 这是全局变量
}

echo "调用函数前：global_var = $global_var"
modify_global
echo "调用函数后：global_var = $global_var"

echo ""
echo "--- 6.2 使用 local 定义局部变量 ---"

test_local() {
    local local_var="我是局部变量"      # 局部变量，函数外不可见
    echo "函数内：local_var = $local_var"
}

test_local
echo "函数外：local_var = ${local_var:-不存在}"

echo ""
echo "--- 6.3 局部变量遮蔽全局变量 ---"

my_var="全局值"

test_shadow() {
    local my_var="局部值"              # 局部变量和全局变量同名
    echo "函数内：my_var = $my_var"    # 函数内优先使用局部变量
}

echo "调用前：my_var = $my_var"
test_shadow
echo "调用后：my_var = $my_var"        # 全局变量不受影响

echo ""
echo "========== 7. 递归函数 =========="

# -------------------- 7. 递归函数 --------------------
# 递归函数就是在函数内部调用自己
# 必须有终止条件，否则会无限递归导致栈溢出

echo "--- 7.1 阶乘递归 ---"

factorial() {
    local n=$1
    if [ $n -le 1 ]; then
        echo 1                         # 终止条件：n<=1 时返回 1
    else
        local prev=$(factorial $((n - 1)))  # 递归调用
        echo $((n * prev))             # n * (n-1)!
    fi
}

echo "5! = $(factorial 5)"
echo "10! = $(factorial 10)"

echo ""
echo "--- 7.2 斐波那契数列 ---"

fibonacci() {
    local n=$1
    if [ $n -eq 0 ]; then
        echo 0
    elif [ $n -eq 1 ]; then
        echo 1
    else
        local a=$(fibonacci $((n - 1)))
        local b=$(fibonacci $((n - 2)))
        echo $((a + b))
    fi
}

echo "斐波那契数列前10项："
for i in {0..9}; do
    echo "  fib($i) = $(fibonacci $i)"
done

echo ""
echo "--- 递归的注意事项 ---"
echo "1. 必须有明确的终止条件"
echo "2. 每次递归都要向终止条件靠近"
echo "3. 递归深度过大会导致栈溢出"
echo "4. Bash 递归效率不高，复杂计算建议用其他语言"

echo ""
echo "========== 8. 函数库 source / . =========="

# -------------------- 8. 函数库 source / . --------------------
# 可以把常用的函数定义在单独的文件中（函数库）
# 然后用 source 或 . 命令加载到当前脚本中
# source 和 . 是等价的，. 是 source 的简写

echo "--- 8.1 创建函数库文件 ---"

# 创建一个简单的函数库文件
cat > math_lib.sh << 'LIBEOF'
#!/bin/bash
# 这是一个数学函数库

# 加法
add() {
    echo $(( $1 + $2 ))
}

# 减法
subtract() {
    echo $(( $1 - $2 ))
}

# 乘法
multiply() {
    echo $(( $1 * $2 ))
}

# 除法（整数）
divide() {
    if [ $2 -eq 0 ]; then
        echo "错误：除数不能为0" >&2
        return 1
    fi
    echo $(( $1 / $2 ))
}
LIBEOF

echo "已创建函数库文件：math_lib.sh"

echo ""
echo "--- 8.2 使用 source 加载函数库 ---"

# 使用 source 加载函数库（两种写法等价）
source math_lib.sh                    # 写法一
# . math_lib.sh                      # 写法二（点命令）

echo "调用 add 3 5 = $(add 3 5)"
echo "调用 subtract 10 4 = $(subtract 10 4)"
echo "调用 multiply 6 7 = $(multiply 6 7)"
echo "调用 divide 20 4 = $(divide 20 4)"

echo ""
echo "--- 8.3 检查函数是否存在 ---"

# 用 type 命令检查函数是否已定义
if type add &>/dev/null; then
    echo "add 函数已定义"
fi

if type nonexistent_func &>/dev/null; then
    echo "nonexistent_func 已定义"
else
    echo "nonexistent_func 未定义"
fi

# 清理函数库文件
rm -f math_lib.sh

echo ""
echo "========== 9. 函数的其他技巧 =========="

# -------------------- 9. 函数的其他技巧 --------------------

echo "--- 9.1 函数的前置/后置操作 ---"
# 可以在函数前后做一些统一操作，比如日志记录

# 封装一个带日志的函数调用
log_and_run() {
    local func_name=$1
    shift                              # 移除第一个参数（函数名）
    echo "[LOG] 开始执行 $func_name，参数：$@"
    $func_name "$@"                    # 调用函数
    local ret=$?
    echo "[LOG] $func_name 执行完毕，返回值：$ret"
    return $ret
}

demo_func() {
    echo "  这是 demo_func 的输出，参数：$@"
    return 0
}

log_and_run demo_func 参数1 参数2

echo ""
echo "--- 9.2 函数作为参数传递 ---"
# Bash 中函数名可以作为参数传递（用字符串形式）

apply_operation() {
    local op=$1                        # 操作名（函数名）
    local a=$2
    local b=$3
    $op $a $b                          # 调用对应的函数
}

# 定义几个操作函数
op_add() { echo $(( $1 + $2 )); }
op_sub() { echo $(( $1 - $2 )); }
op_mul() { echo $(( $1 * $2 )); }

echo "10 op_add 5 = $(apply_operation op_add 10 5)"
echo "10 op_sub 5 = $(apply_operation op_sub 10 5)"
echo "10 op_mul 5 = $(apply_operation op_mul 10 5)"

echo ""
echo "--- 9.3 删除函数 ---"
# 使用 unset -f 可以删除函数定义

temp_func() {
    echo "临时函数"
}

echo "删除前："
type temp_func 2>/dev/null && echo "  函数存在" || echo "  函数不存在"

unset -f temp_func                    # 删除函数

echo "删除后："
type temp_func 2>/dev/null && echo "  函数存在" || echo "  函数不存在"

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 04_text_processing/01_text_tools.sh 了解文本处理"
