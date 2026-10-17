#!/bin/bash
# ==============================================================================
# 文件名：01_text_tools.sh
# 描述：Shell/Bash 文本处理 - grep/sed/awk/cut/sort/uniq/wc/tr/find/xargs
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 01_text_tools.sh ===================="
echo ""

# 创建一个通用的测试文件
cat > test_text.txt << 'EOF'
张三 25 男 北京 程序员
李四 30 男 上海 设计师
王五 28 女 广州 测试工程师
赵六 35 男 深圳 产品经理
钱七 22 女 杭州 前端开发
孙八 27 男 成都 后端开发
周九 32 女 武汉 数据分析
吴十 29 男 南京 运维工程师
郑十一 26 女 西安 程序员
王十二 31 男 苏州 设计师
EOF

cat > test_log.txt << 'EOF'
2024-01-15 10:23:45 [INFO] 系统启动成功
2024-01-15 10:23:46 [INFO] 加载配置文件
2024-01-15 10:23:47 [WARN] 配置项缺失，使用默认值
2024-01-15 10:24:01 [ERROR] 数据库连接失败
2024-01-15 10:24:02 [ERROR] 重试连接数据库
2024-01-15 10:24:05 [INFO] 数据库连接成功
2024-01-15 10:25:00 [INFO] 用户登录成功：张三
2024-01-15 10:25:30 [WARN] 密码即将过期：李四
2024-01-15 10:26:00 [ERROR] 文件上传失败：disk full
2024-01-15 10:27:00 [INFO] 系统运行正常
EOF

echo "测试文件已创建：test_text.txt, test_log.txt"
echo ""

# -------------------- 1. grep 文本搜索 --------------------
echo "========== 1. grep 文本搜索 =========="

echo "--- 1.1 基本用法 ---"
# grep 模式 文件名
# 在文件中搜索包含指定模式的行

echo "搜索包含 \"程序员\" 的行："
grep "程序员" test_text.txt

echo ""
echo "--- 1.2 忽略大小写 -i ---"
echo "搜索 \"error\"（忽略大小写）："
grep -i "error" test_log.txt

echo ""
echo "--- 1.3 显示行号 -n ---"
echo "显示行号搜索 ERROR："
grep -n "ERROR" test_log.txt

echo ""
echo "--- 1.4 反向匹配 -v ---"
echo "显示不包含 INFO 的行："
grep -v "INFO" test_log.txt

echo ""
echo "--- 1.5 计数 -c ---"
echo "统计 ERROR 出现的行数："
grep -c "ERROR" test_log.txt

echo ""
echo "--- 1.6 只显示匹配的部分 -o ---"
echo "只提取日期（2024-xx-xx）："
grep -o "2024-[0-9][0-9]-[0-9][0-9]" test_log.txt | head -3

echo ""
echo "--- 1.7 正则表达式 ---"
# grep 默认使用基本正则（BRE）
# -E 使用扩展正则（ERE），等价于 egrep
# -P 使用 Perl 正则（PCRE），不是所有版本都支持

echo "使用扩展正则搜索 ERROR 或 WARN："
grep -E "ERROR|WARN" test_log.txt

echo ""
echo "搜索以 2024 开头的行："
grep "^2024" test_log.txt | head -3

echo "搜索以 成功 结尾的行："
grep "成功$" test_log.txt

echo ""
echo "--- 1.8 上下文显示 -A -B -C ---"
echo "显示 ERROR 行及其后2行（-A）："
grep -A 2 "数据库连接失败" test_log.txt

echo ""
echo "显示 ERROR 行及其前1行（-B）："
grep -B 1 "文件上传失败" test_log.txt

echo ""
echo "显示 ERROR 行及其前后各1行（-C）："
grep -C 1 "数据库连接失败" test_log.txt

echo ""
echo "--- 1.9 递归搜索 -r ---"
echo "递归搜索当前目录下所有文件中的 \"echo\"："
grep -r "echo" --include="*.sh" . | head -5

echo ""
echo "========== 2. sed 流编辑器 =========="

# -------------------- 2. sed 流编辑器 --------------------
# sed 是流式编辑器，按行处理文本
# 基本语法：sed '命令' 文件名
# 常用命令：s 替换, d 删除, i 插入, a 追加, p 打印

echo "--- 2.1 替换 s ---"
# 格式：sed 's/旧内容/新内容/' 文件名
# 默认只替换每行的第一个匹配

echo "将 \"男\" 替换为 \"M\"："
sed 's/男/M/' test_text.txt

echo ""
echo "--- 2.2 全局替换 g ---"
# 加 g 标志替换所有匹配

echo "将所有 \"工程师\" 替换为 \"Engineer\"："
sed 's/工程师/Engineer/g' test_text.txt

echo ""
echo "--- 2.3 指定行替换 ---"
echo "只替换第3行："
sed '3s/男/M/' test_text.txt

echo "替换第2到5行："
sed '2,5s/男/M/' test_text.txt

echo ""
echo "--- 2.4 删除行 d ---"
echo "删除第1行："
sed '1d' test_text.txt

echo "删除第2到4行："
sed '2,4d' test_text.txt

echo "删除包含 \"设计师\" 的行："
sed '/设计师/d' test_text.txt

echo "删除空行："
sed '/^$/d' test_text.txt

echo ""
echo "--- 2.5 插入 i 和追加 a ---"
echo "在第1行之前插入标题行："
sed '1i\姓名 年龄 性别 城市 职业' test_text.txt

echo "在最后一行追加："
sed '$a\--- 数据结束 ---' test_text.txt

echo ""
echo "--- 2.6 打印 p ---"
# -n 选项禁止默认输出，只打印匹配的行
echo "只打印第3到5行："
sed -n '3,5p' test_text.txt

echo "只打印包含 \"女\" 的行："
sed -n '/女/p' test_text.txt

echo ""
echo "--- 2.7 原地修改文件 -i ---"
# sed -i 可以直接修改原文件（危险，慎用）
# sed -i.bak 会先备份原文件为 .bak

echo "创建测试副本并修改："
cp test_text.txt test_text_backup.txt
sed -i 's/程序员/程序猿/g' test_text_backup.txt
grep "程序猿" test_text_backup.txt
rm -f test_text_backup.txt

echo ""
echo "--- 2.8 多个命令 -e ---"
echo "同时执行多个替换："
sed -e 's/男/M/g' -e 's/女/F/g' test_text.txt

echo ""
echo "========== 3. awk 文本处理 =========="

# -------------------- 3. awk 文本处理 --------------------
# awk 是强大的文本处理语言，特别适合处理结构化文本
# 基本语法：awk '模式 {动作}' 文件名
# awk 自动把每行按分隔符分成多个字段：$1, $2, $3...
# $0 表示整行内容

echo "--- 3.1 基本字段操作 ---"
# 默认按空格或制表符分隔

echo "打印第1列（姓名）和第4列（城市）："
awk '{print $1, $4}' test_text.txt

echo ""
echo "--- 3.2 自定义分隔符 -F ---"
# -F 指定字段分隔符

echo "用冒号分隔 /etc/passwd（示例，显示前3行）："
awk -F: '{print $1, $3, $6}' /etc/passwd 2>/dev/null | head -3 || echo "（无 /etc/passwd，跳过）"

echo ""
echo "--- 3.3 内置变量 ---"
# NF：当前行的字段数（Number of Fields）
# NR：当前行号（Number of Records）
# FS：字段分隔符（Field Separator）
# OFS：输出字段分隔符（Output Field Separator）

echo "显示行号和字段数："
awk '{print "第"NR"行："$1", 字段数="NF}' test_text.txt

echo ""
echo "设置输出分隔符："
awk -v OFS=" | " '{print $1, $2, $5}' test_text.txt

echo ""
echo "--- 3.4 模式匹配 ---"
# awk 可以用模式来筛选行

echo "打印年龄大于30的行："
awk '$2 > 30 {print $1, $2, $5}' test_text.txt

echo ""
echo "打印性别为女的行："
awk '$3 == "女" {print $1, $4}' test_text.txt

echo ""
echo "正则匹配：职业包含 \"开发\" 的人："
awk '$5 ~ /开发/ {print $1, $5}' test_text.txt

echo ""
echo "--- 3.5 BEGIN 和 END 块 ---"
# BEGIN：在处理文件之前执行一次
# END：在处理完所有行之后执行一次

echo "统计所有年龄的总和："
awk '
BEGIN { sum = 0 }
{ sum += $2 }
END { print "总年龄：" sum "，平均年龄：" sum/NR }
' test_text.txt

echo ""
echo "带表头的输出："
awk '
BEGIN {
    printf "%-10s %-5s %-5s %-10s\n", "姓名", "年龄", "性别", "职业"
    print "-----------------------------------"
}
{
    printf "%-10s %-5s %-5s %-10s\n", $1, $2, $3, $5
}
' test_text.txt

echo ""
echo "--- 3.6 awk 数组 ---"
# awk 的数组是关联数组（键值对）

echo "按性别统计人数："
awk '
{ count[$3]++ }
END {
    for (gender in count) {
        print gender ": " count[gender] " 人"
    }
}
' test_text.txt

echo ""
echo "按城市统计人数："
awk '
{ cities[$4]++ }
END {
    for (city in cities) {
        print city ": " cities[city] " 人"
    }
}
' test_text.txt

echo ""
echo "--- 3.7 awk 函数 ---"
# awk 内置了很多函数
# 字符串函数：length, substr, index, split, gsub 等
# 数学函数：sin, cos, sqrt, int 等

echo "计算姓名长度："
awk '{print $1 " 的名字长度：" length($1)}' test_text.txt

echo ""
echo "提取子串（城市的前2个字符）："
awk '{print $1 " 来自 " substr($4, 1, 2)}' test_text.txt

echo ""
echo "========== 4. cut 截取列 =========="

# -------------------- 4. cut 截取列 --------------------
# cut 用于从文本中截取指定的列或字符
# 选项：
# -c 按字符位置截取
# -f 按字段截取（需配合 -d 指定分隔符）
# -d 指定字段分隔符

echo "--- 4.1 按字符截取 -c ---"
echo "截取每行前6个字符："
cut -c 1-6 test_text.txt

echo "截取每行第3到第8个字符："
cut -c 3-8 test_text.txt

echo ""
echo "--- 4.2 按字段截取 -f -d ---"
echo "截取第1列和第4列（空格分隔）："
cut -d' ' -f1,4 test_text.txt

echo "截取第1到3列："
cut -d' ' -f1-3 test_text.txt

echo ""
echo "========== 5. sort 排序 / uniq 去重 =========="

# -------------------- 5. sort 排序 / uniq 去重 --------------------

echo "--- 5.1 sort 基本排序 ---"
echo "按姓名排序："
sort test_text.txt

echo ""
echo "--- 5.2 按列排序 -k ---"
echo "按年龄（第2列）排序："
sort -k2 -n test_text.txt           # -n 按数值排序

echo "按年龄降序排序："
sort -k2 -nr test_text.txt          # -r 逆序

echo ""
echo "--- 5.3 按指定分隔符排序 -t ---"
echo "按城市（第4列）排序："
sort -k4 test_text.txt

echo ""
echo "--- 5.4 uniq 去重 ---"
# uniq 只能去除相邻的重复行，所以通常配合 sort 使用

cat > test_dup.txt << 'EOF'
苹果
香蕉
苹果
橙子
香蕉
香蕉
葡萄
EOF

echo "原始数据："
cat test_dup.txt

echo "去重（需先排序）："
sort test_dup.txt | uniq

echo "显示重复次数："
sort test_dup.txt | uniq -c

echo "只显示重复的行："
sort test_dup.txt | uniq -d

rm -f test_dup.txt

echo ""
echo "========== 6. wc 统计 / head / tail =========="

# -------------------- 6. wc 统计 / head / tail --------------------

echo "--- 6.1 wc 统计 ---"
# wc 用于统计行数、单词数、字符数
# -l 行数（line）
# -w 单词数（word）
# -c 字符数（character）

echo "统计 test_text.txt："
wc -l test_text.txt                 # 行数
wc -w test_text.txt                 # 单词数
wc -c test_text.txt                 # 字符数

echo ""
echo "统计日志中的 ERROR 数量："
grep "ERROR" test_log.txt | wc -l

echo ""
echo "--- 6.2 head 显示开头 ---"
# head 显示文件的前几行，默认10行

echo "显示前3行："
head -3 test_text.txt

echo ""
echo "--- 6.3 tail 显示末尾 ---"
# tail 显示文件的最后几行，默认10行
# -f 实时跟踪文件新增内容（常用于监控日志）

echo "显示最后3行："
tail -3 test_text.txt

echo "显示从第5行开始的所有行："
tail -n +5 test_text.txt

echo ""
echo "========== 7. tr 字符转换 =========="

# -------------------- 7. tr 字符转换 --------------------
# tr 用于字符的替换、删除、压缩
# tr 从标准输入读取，写入标准输出
# 语法：tr 源字符集 目标字符集

echo "--- 7.1 字符替换 ---"
echo "小写转大写："
echo "hello world" | tr 'a-z' 'A-Z'

echo "大写转小写："
echo "HELLO WORLD" | tr 'A-Z' 'a-z'

echo ""
echo "--- 7.2 删除字符 -d ---"
echo "删除所有数字："
echo "abc123def456" | tr -d '0-9'

echo "删除空格："
echo "a b c d e" | tr -d ' '

echo ""
echo "--- 7.3 压缩重复字符 -s ---"
echo "压缩连续空格："
echo "a   b    c  d" | tr -s ' '

echo "压缩空行："
cat -A test_text.txt 2>/dev/null | tr -s '\n' | head -3

echo ""
echo "--- 7.4 字符集替换 ---"
echo "ROT13 加密："
echo "hello" | tr 'a-zA-Z' 'n-za-mN-ZA-M'

echo ""
echo "========== 8. find 文件查找 =========="

# -------------------- 8. find 文件查找 --------------------
# find 用于在目录树中查找文件
# 语法：find 路径 选项 动作

echo "--- 8.1 按名称查找 -name ---"
echo "查找当前目录下的 .sh 文件："
find . -maxdepth 2 -name "*.sh" -type f | head -10

echo ""
echo "--- 8.2 按类型查找 -type ---"
# f：普通文件，d：目录，l：符号链接

echo "查找目录："
find . -maxdepth 2 -type d | head -10

echo ""
echo "--- 8.3 按大小查找 -size ---"
# +n 大于n，-n 小于n，n 等于n
# 单位：c 字节, k KB, M MB, G GB

echo "查找大于 1KB 的文件："
find . -maxdepth 2 -size +1k -type f 2>/dev/null | head -10

echo ""
echo "--- 8.4 按时间查找 ---"
# -mtime 按修改时间（天）
# -atime 按访问时间（天）
# -mmin 按修改时间（分钟）

echo "查找 7 天内修改过的文件："
find . -maxdepth 2 -mtime -7 -type f 2>/dev/null | head -10

echo ""
echo "--- 8.5 组合条件 -a -o ! ---"
echo "查找 .sh 文件且大小大于 500 字节："
find . -maxdepth 2 -name "*.sh" -size +500c -type f 2>/dev/null | head -5

echo ""
echo "--- 8.6 执行命令 -exec ---"
# -exec 对找到的每个文件执行命令
# {} 代表找到的文件名，\; 表示命令结束

echo "对找到的 .sh 文件执行 ls -l："
find . -maxdepth 1 -name "*.sh" -type f -exec ls -l {} \; 2>/dev/null | head -5

echo ""
echo "========== 9. xargs 参数传递 =========="

# -------------------- 9. xargs 参数传递 --------------------
# xargs 从标准输入读取数据，转换成命令行参数传递给其他命令
# 常用于配合 find、grep 等命令批量处理

echo "--- 9.1 基本用法 ---"
echo "用 xargs 输出："
echo "a b c d e" | xargs echo "参数："

echo ""
echo "--- 9.2 配合 find 使用 ---"
echo "查找 .sh 文件并用 ls -l 显示（xargs 方式）："
find . -maxdepth 1 -name "*.sh" -type f | xargs ls -l 2>/dev/null | head -5

echo ""
echo "--- 9.3 控制参数数量 -n ---"
echo "每次传2个参数："
echo "1 2 3 4 5 6" | xargs -n 2 echo "批次："

echo ""
echo "--- 9.4 处理含空格的文件名 -0 ---"
# find -print0 和 xargs -0 配合使用，可以正确处理含空格的文件名
echo "find -print0 + xargs -0（安全处理空格）："
find . -maxdepth 1 -name "*.sh" -type f -print0 | xargs -0 ls -l 2>/dev/null | head -3

echo ""
echo "--- 9.5 交互式确认 -p ---"
# xargs -p 会在执行每个命令前询问确认
echo "xargs -p 会询问确认（本脚本不演示交互）"

# 清理测试文件
rm -f test_text.txt test_log.txt

echo ""
echo "==================== 脚本结束 ===================="
echo "接下来可以学习 05_advanced/01_advanced.sh 了解高级技巧"
