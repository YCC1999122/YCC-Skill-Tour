#!/bin/bash
# ==============================================================================
# 文件名：02_variables.sh
# 描述：Shell/Bash 变量进阶 - 字符串操作、数组、数值计算等
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 02_variables.sh ===================="
echo ""

# -------------------- 1. 变量命名规则 --------------------
echo "========== 1. 变量命名规则 =========="

# 正确的变量命名示例
valid_name1="合法变量1"           # 字母开头，后跟字母数字下划线
_valid_name="合法变量2"           # 下划线开头
VALID_NAME="合法变量3"            # 全大写（通常用于环境变量）
name123="合法变量4"               # 字母开头，后跟数字
echo "合法变量名示例：$valid_name1, $_valid_name, $VALID_NAME, $name123"

# 错误的变量命名（以下代码如果取消注释会报错）：
# 123name="错误"                  # 不能以数字开头
# my-name="错误"                  # 不能包含减号
# my name="错误"                  # 不能包含空格
# $name="错误"                    # 定义时不能加 $

# 变量名区分大小写
name="小写name"
Name="大写Name"
NAME="全大写NAME"
echo "变量区分大小写：name=$name, Name=$Name, NAME=$NAME"

echo ""
echo "========== 2. 字符串操作 =========="

# -------------------- 2. 字符串操作 --------------------

str="Hello, Shell Script World!"
echo "原始字符串：$str"
echo ""

# --- 2.1 字符串拼接 ---
echo "--- 2.1 字符串拼接 ---"
first_name="张"
last_name="三"
# Bash 中字符串拼接非常简单，直接把变量写在一起即可
full_name=$first_name$last_name
echo "直接拼接：$full_name"

# 也可以用双引号拼接（更清晰）
full_name2="${first_name}${last_name}"
echo "双引号拼接：$full_name2"

# 拼接中间可以加其他字符
greeting="你好，"$full_name"！"
echo "带字符拼接：$greeting"

# 多个变量和文字混合拼接
str1="我爱"
str2="Shell"
str3="编程"
result="${str1}${str2}${str3}"
echo "多变量拼接：$result"

echo ""

# --- 2.2 字符串长度 ---
echo "--- 2.2 字符串长度 ---"
# 格式：${#变量名}
echo "字符串 \"$str\" 的长度：${#str}"
echo "空字符串的长度：${#empty_var}"     # 空变量长度为 0

echo ""

# --- 2.3 字符串截取 ---
echo "--- 2.3 字符串截取 ---"
# 格式1：${变量名:起始位置}
#        从起始位置开始截取到末尾，位置从 0 开始计数
echo "从第0位开始截取：${str:0}"         # 截取整个字符串
echo "从第7位开始截取：${str:7}"         # 从第7个字符开始（Shell Script World!）

# 格式2：${变量名:起始位置:长度}
#        从起始位置开始截取指定长度的字符
echo "从第0位截取5个字符：${str:0:5}"    # Hello
echo "从第7位截取5个字符：${str:7:5}"    # Shell

# 起始位置可以是负数（表示从右边开始数）
# 注意：冒号后面要有空格或者用括号，否则会被误认为是 ${var:-default} 语法
echo "从右边第6位开始截取：${str: -6}"    # World!（注意冒号后有空格）
echo "从右边第6位截取3个：${str: -6:3}"   # Wor

echo ""

# --- 2.4 字符串替换 ---
echo "--- 2.4 字符串替换 ---"
# 格式1：${变量名/旧字符串/新字符串}
#        只替换第一个匹配的字符串
str2="apple banana apple orange"
echo "原始：$str2"
echo "替换第一个 apple 为 cherry：${str2/apple/cherry}"

# 格式2：${变量名//旧字符串/新字符串}
#        替换所有匹配的字符串（两个斜杠）
echo "替换所有 apple 为 cherry：${str2//apple/cherry}"

# 格式3：${变量名/#旧字符串/新字符串}
#        只替换开头匹配的字符串
echo "开头替换 apple 为 fruit：${str2/#apple/fruit}"

# 格式4：${变量名/%旧字符串/新字符串}
#        只替换结尾匹配的字符串
echo "结尾替换 orange 为 fruit：${str2/%orange/fruit}"

echo ""

# --- 2.5 字符串删除 ---
echo "--- 2.5 字符串删除 ---"
path="/home/user/documents/report.txt"
echo "原始路径：$path"

# 删除开头匹配（最短匹配）：${变量名#模式}
# 从左向右删除第一个匹配的最短子串
echo "删除开头第一个 / 及其前面（空）：${path#/}"
echo "删除开头到第一个 /：${path#*/}"      # 删除到第一个 /

# 删除开头匹配（最长匹配）：${变量名##模式}
# 从左向右删除最后一个匹配的子串（贪婪匹配）
echo "删除开头到最后一个 /：${path##*/}"    # 只保留文件名

# 删除结尾匹配（最短匹配）：${变量名%模式}
# 从右向左删除第一个匹配的最短子串
echo "删除最后一个 . 及其后面：${path%.*}"  # 删除扩展名
echo "删除结尾到第一个 /：${path%/*}"       # 删除文件名，保留目录

# 删除结尾匹配（最长匹配）：${变量名%%模式}
# 从右向左删除最后一个匹配的子串（贪婪匹配）
echo "删除结尾到第一个 .（贪婪）：${path%%.*}"

echo ""

# --- 2.6 变量默认值 ---
echo "--- 2.6 变量默认值 ---"
# ${变量名:-默认值}：如果变量为空或未定义，返回默认值，变量本身不变
echo "未定义变量的默认值：${undefined_var:-默认值}"
echo "变量仍然未定义：${undefined_var:-还是默认值}"

# ${变量名:=默认值}：如果变量为空或未定义，返回默认值并赋值给变量
echo "未定义变量赋值：${undefined2:=自动赋值}"
echo "变量现在有值了：$undefined2"

# ${变量名:?错误信息}：如果变量为空或未定义，打印错误信息并退出脚本
# 常用于必须参数的检查
# echo "${must_var:?必须设置 must_var 变量}"  # 取消注释会报错并退出

# ${变量名:+替换值}：如果变量已定义且非空，返回替换值；否则返回空
defined_var="有值"
echo "变量有值时返回：${defined_var:+替换成功}"
echo "变量无值时返回：${undefined3:+替换成功}"

echo ""
echo "========== 3. 数组定义与访问 =========="

# -------------------- 3. 数组定义与访问 --------------------
# Bash 支持普通数组（索引数组）和关联数组（键值对）

# --- 3.1 普通数组定义 ---
echo "--- 3.1 数组定义 ---"

# 方式一：逐个赋值
fruits[0]="苹果"
fruits[1]="香蕉"
fruits[2]="橙子"
echo "数组定义方式一：索引0=${fruits[0]}, 索引1=${fruits[1]}"

# 方式二：一次性赋值（用空格分隔）
colors=("红色" "绿色" "蓝色" "黄色")
echo "数组定义方式二：${colors[0]}, ${colors[1]}, ${colors[2]}"

# 方式三：带索引赋值
declare -a nums=([2]=二十 [0]=十 [1]=十五)
echo "带索引数组：${nums[0]}, ${nums[1]}, ${nums[2]}"

# --- 3.2 数组元素访问 ---
echo ""
echo "--- 3.2 数组元素访问 ---"
echo "colors 数组第1个元素：${colors[0]}"     # 索引从0开始
echo "colors 数组第2个元素：${colors[1]}"
echo "colors 数组最后一个元素：${colors[-1]}"  # 负数索引，从末尾开始数（Bash 4+支持）

# --- 3.3 获取数组所有元素 ---
echo ""
echo "--- 3.3 获取数组所有元素 ---"
# ${数组名[@]} 或 ${数组名[*]} 可以获取所有元素
echo "所有元素(@)：${colors[@]}"
echo "所有元素(*)：${colors[*]}"
# 和特殊变量类似，@ 和 * 的区别在加双引号时才体现

# --- 3.4 数组长度（元素个数）---
echo ""
echo "--- 3.4 数组长度 ---"
# 格式：${#数组名[@]} 或 ${#数组名[*]}
echo "colors 数组元素个数：${#colors[@]}"
echo "fruits 数组元素个数：${#fruits[*]}"

# --- 3.5 数组遍历 ---
echo ""
echo "--- 3.5 数组遍历 ---"

# 方法一：使用 for 循环遍历所有元素（推荐）
echo "方法一：for + \${colors[@]}"
for color in "${colors[@]}"; do
    echo "  颜色：$color"
done

# 方法二：通过索引遍历
echo "方法二：通过索引遍历"
for i in "${!colors[@]}"; do       # ${!colors[@]} 获取所有索引
    echo "  索引 $i：${colors[$i]}"
done

# 方法三：C 语言风格的 for 循环
echo "方法三：C风格 for 循环"
for (( i=0; i<${#colors[@]}; i++ )); do
    echo "  第 $i 个：${colors[$i]}"
done

# --- 3.6 数组切片（截取部分元素）---
echo ""
echo "--- 3.6 数组切片 ---"
# 格式：${数组名[@]:起始索引:长度}
echo "从索引1开始取2个元素：${colors[@]:1:2}"
echo "从索引2开始到末尾：${colors[@]:2}"

# --- 3.7 数组添加元素 ---
echo ""
echo "--- 3.7 数组添加元素 ---"
# 方法一：直接追加
colors+=("紫色")                    # 在末尾添加元素
echo "添加紫色后：${colors[@]}"

# 方法二：重新赋值（合并数组）
more_colors=("青色" "品红")
all_colors=("${colors[@]}" "${more_colors[@]}")
echo "合并两个数组：${all_colors[@]}"

# --- 3.8 删除数组元素 ---
echo ""
echo "--- 3.8 删除数组元素 ---"
# unset 数组名[索引] 删除指定元素
unset 'colors[1]'                   # 删除索引为1的元素（绿色）
echo "删除索引1后：${colors[@]}"
echo "数组长度变为：${#colors[@]}"
# 注意：删除元素后索引不会重新排列，会留下空洞

# 删除整个数组
# unset colors

echo ""
echo "========== 4. 关联数组 =========="

# -------------------- 4. 关联数组（declare -A）--------------------
# 关联数组也叫字典或哈希表，使用字符串作为键
# 注意：关联数组必须先用 declare -A 声明，这是 Bash 4.0 及以上才支持的特性

echo "--- 4.1 关联数组定义 ---"
# 首先声明关联数组
declare -A person

# 方式一：逐个赋值
person["name"]="张三"
person["age"]=25
person["city"]="北京"

# 方式二：一次性赋值
declare -A fruit_prices=(
    ["苹果"]=5.5
    ["香蕉"]=3.2
    ["橙子"]=4.8
)

echo "person[name] = ${person[name]}"
echo "person[age] = ${person[age]}"
echo "person[city] = ${person[city]}"

echo ""
echo "--- 4.2 关联数组遍历 ---"

# 遍历所有值
echo "遍历所有值："
for value in "${person[@]}"; do
    echo "  值：$value"
done

# 遍历所有键
echo "遍历所有键："
for key in "${!person[@]}"; do      # ${!数组名[@]} 获取所有键
    echo "  键：$key，值：${person[$key]}"
done

echo ""
echo "水果价格表："
for fruit in "${!fruit_prices[@]}"; do
    echo "  $fruit：${fruit_prices[$fruit]} 元/斤"
done

# 检查键是否存在（间接判断：值是否为空）
echo ""
if [ -n "${fruit_prices[苹果]+exists}" ]; then
    echo "苹果存在于价格表中"
fi
if [ -z "${fruit_prices[葡萄]+exists}" ]; then
    echo "葡萄不存在于价格表中"
fi

echo ""
echo "========== 5. 数值计算 =========="

# -------------------- 5. 数值计算 --------------------
# Bash 本身不擅长数值计算，但提供了几种方式

# --- 5.1 expr 命令 ---
echo "--- 5.1 expr 命令 ---"
# expr 是外部命令，用于计算表达式
# 注意：运算符两边必须有空格
result=$(expr 2 + 3)
echo "2 + 3 = $result"

result=$(expr 10 - 4)
echo "10 - 4 = $result"

result=$(expr 3 \* 4)               # 乘号需要转义，否则会被当作通配符
echo "3 * 4 = $result"

result=$(expr 10 / 3)
echo "10 / 3 = $result"             # 整数除法，结果为 3

result=$(expr 10 % 3)
echo "10 % 3 = $result"             # 取余

# expr 也可以用于字符串（不常用）
echo ""

# --- 5.2 $(( )) 算术扩展 ---
echo "--- 5.2 \$(( )) 算术扩展（推荐）---"
# $(( )) 是 Bash 内置的算术运算，速度快，推荐使用
# 运算符两边可以有空格，也可以没有
# 不需要转义乘号

a=10
b=3
echo "$a + $b = $((a + b))"
echo "$a - $b = $((a - b))"
echo "$a * $b = $((a * b))"
echo "$a / $b = $((a / b))"         # 整数除法
echo "$a % $b = $((a % b))"         # 取余
echo "$a ** $b = $((a ** b))"       # 幂运算（a 的 b 次方）

# 支持自增自减
c=5
echo "c = $c"
echo "c++ = $((c++)) (后置，先返回后自增)"
echo "现在 c = $c"
echo "++c = $((++c)) (前置，先自增后返回)"
echo "现在 c = $c"

# 支持复合赋值
d=10
((d += 5))                           # 等价于 d = d + 5
echo "d += 5 后 d = $d"

((d *= 2))
echo "d *= 2 后 d = $d"

# 支持位运算
e=6                                  # 二进制 0110
f=3                                  # 二进制 0011
echo "6 & 3 = $((e & f))"           # 按位与 = 2 (0010)
echo "6 | 3 = $((e | f))"           # 按位或 = 7 (0111)
echo "6 ^ 3 = $((e ^ f))"           # 按位异或 = 5 (0101)
echo "~6 = $((~e))"                 # 按位取反

echo ""

# --- 5.3 let 命令 ---
echo "--- 5.3 let 命令 ---"
# let 也是内置命令，用于算术运算
# 特点：变量名前不需要加 $，表达式整体不需要加引号（除非有空格）
let g=2+3
echo "let g=2+3 → g = $g"

let "h = 10 - 4"                    # 有空格时需要加引号
echo "let \"h = 10 - 4\" → h = $h"

let i++                             # 自增
echo "let i++ → i = $i"

let "j = 5 * 3, k = j + 2"          # 支持多个表达式
echo "let 多表达式 → j = $j, k = $k"

echo ""

# --- 5.4 bc 命令（高精度计算）---
echo "--- 5.4 bc 命令（支持小数）---"
# Bash 的算术运算只支持整数，小数计算需要用 bc 等外部工具
# bc 是一个高精度计算器语言

# 基本用法：echo "表达式" | bc
echo "3.5 + 2.1 = $(echo "3.5 + 2.1" | bc)"
echo "10 / 3 = $(echo "10 / 3" | bc)"                    # 默认整数除法
echo "10 / 3（保留2位小数）= $(echo "scale=2; 10 / 3" | bc)"  # scale 设置小数位数
echo "2 的 10 次方 = $(echo "2^10" | bc)"
echo "平方根 16 = $(echo "sqrt(16)" | bc)"

# 也可以用 here string 方式
echo "使用 here string：$(bc <<< "5.5 * 2")"

echo ""
echo "========== 6. 只读变量 readonly =========="

# -------------------- 6. 只读变量 readonly --------------------
# 使用 readonly 或 declare -r 声明的变量不能被修改或删除

readonly PI=3.14159                 # 声明只读变量
echo "只读变量 PI = $PI"

# 尝试修改会报错（取消注释测试）
# PI=3.14                           # 会报错：PI: readonly variable

# 也可以用 declare -r
declare -r MAX_SIZE=1024
echo "只读变量 MAX_SIZE = $MAX_SIZE"

echo ""
echo "========== 7. 删除变量 unset =========="

# -------------------- 7. 删除变量 unset --------------------
# 使用 unset 命令可以删除变量（只读变量除外）

temp_var="临时变量"
echo "删除前：temp_var = $temp_var"

unset temp_var                      # 删除变量
echo "删除后：temp_var = ${temp_var:-变量已删除}"

# 也可以删除数组
temp_arr=("a" "b" "c")
echo "数组删除前长度：${#temp_arr[@]}"
unset temp_arr
echo "数组删除后长度：${#temp_arr[@]:-0}"

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 02_flow_control/01_conditionals.sh 了解流程控制"
