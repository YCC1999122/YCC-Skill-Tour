#!/bin/bash
# ==============================================================================
# 文件名：01_utils.sh
# 描述：Shell/Bash 综合案例 - 日志函数、备份、重命名、服务检查、磁盘监控等
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

echo "==================== 01_utils.sh ===================="
echo ""

# 获取脚本所在目录
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# -------------------- 1. 日志函数 --------------------
echo "========== 1. 日志函数 =========="

# --- 日志函数定义 ---
# 在实际项目中，统一的日志函数非常重要
# 支持不同日志级别，输出带时间戳和级别的日志信息

# 日志级别常量
LOG_LEVEL_DEBUG=0
LOG_LEVEL_INFO=1
LOG_LEVEL_WARN=2
LOG_LEVEL_ERROR=3

# 当前日志级别（可以通过环境变量设置）
CURRENT_LOG_LEVEL=${LOG_LEVEL_DEBUG}

# 通用日志输出函数
# 参数：$1=级别名称, $2=日志内容
log_print() {
    local level_name=$1
    shift
    local message="$*"
    local timestamp=$(date "+%Y-%m-%d %H:%M:%S")
    echo "[$timestamp] [$level_name] $message"
}

# DEBUG 级别日志
log_debug() {
    if [ $CURRENT_LOG_LEVEL -le $LOG_LEVEL_DEBUG ]; then
        log_print "DEBUG" "$@"
    fi
}

# INFO 级别日志
log_info() {
    if [ $CURRENT_LOG_LEVEL -le $LOG_LEVEL_INFO ]; then
        log_print "INFO" "$@"
    fi
}

# WARN 级别日志（输出到 stderr）
log_warn() {
    if [ $CURRENT_LOG_LEVEL -le $LOG_LEVEL_WARN ]; then
        log_print "WARN" "$@" >&2
    fi
}

# ERROR 级别日志（输出到 stderr）
log_error() {
    if [ $CURRENT_LOG_LEVEL -le $LOG_LEVEL_ERROR ]; then
        log_print "ERROR" "$@" >&2
    fi
}

# --- 使用示例 ---
echo "--- 日志函数演示 ---"

log_debug "这是一条 DEBUG 级别的调试信息"
log_info "程序启动成功"
log_info "正在处理数据..."
log_warn "配置项缺失，使用默认值"
log_error "连接数据库失败"
log_info "程序执行完毕"

echo ""
echo "--- 修改日志级别后（只显示 WARN 及以上）---"
CURRENT_LOG_LEVEL=$LOG_LEVEL_WARN
log_debug "这条 DEBUG 不会显示"
log_info "这条 INFO 不会显示"
log_warn "这条 WARN 会显示"
log_error "这条 ERROR 会显示"

# 恢复日志级别
CURRENT_LOG_LEVEL=$LOG_LEVEL_DEBUG

echo ""
echo "--- 日志写入文件 ---"
log_file="app.log"
log_info "第一条日志" >> "$log_file"
log_info "第二条日志" >> "$log_file"
log_error "一条错误日志" >> "$log_file"
echo "日志文件内容："
cat "$log_file"
rm -f "$log_file"

echo ""
echo "========== 2. 备份脚本 =========="

# -------------------- 2. 备份脚本 --------------------
# 一个完整的备份脚本应该包含：
# - 源目录和目标目录配置
# - 日期命名
# - 压缩打包
# - 日志记录
# - 旧备份清理

echo "--- 备份函数定义 ---"

# 备份函数
# 参数：$1=源目录, $2=备份存放目录
backup_dir() {
    local src_dir="$1"
    local backup_dir="$2"

    # 参数校验
    if [ -z "$src_dir" ] || [ -z "$backup_dir" ]; then
        log_error "用法：backup_dir 源目录 备份目录"
        return 1
    fi

    if [ ! -d "$src_dir" ]; then
        log_error "源目录不存在：$src_dir"
        return 1
    fi

    # 创建备份目录
    mkdir -p "$backup_dir"

    # 生成备份文件名（带时间戳）
    local timestamp=$(date "+%Y%m%d_%H%M%S")
    local dir_name=$(basename "$src_dir")
    local backup_file="${backup_dir}/${dir_name}_${timestamp}.tar.gz"

    log_info "开始备份：$src_dir -> $backup_file"

    # 执行备份（tar 打包压缩）
    # -c 创建, -z gzip压缩, -f 指定文件, -C 切换目录
    if tar -czf "$backup_file" -C "$(dirname "$src_dir")" "$(basename "$src_dir")"; then
        log_info "备份成功：$backup_file"
        local size=$(du -h "$backup_file" | cut -f1)
        log_info "备份大小：$size"
        echo "$backup_file"                     # 返回备份文件路径
        return 0
    else
        log_error "备份失败"
        return 1
    fi
}

# 清理旧备份
# 参数：$1=备份目录, $2=保留天数
cleanup_old_backups() {
    local backup_dir="$1"
    local keep_days="$2"

    if [ -z "$backup_dir" ] || [ -z "$keep_days" ]; then
        log_error "用法：cleanup_old_backups 备份目录 保留天数"
        return 1
    fi

    log_info "清理 $keep_days 天前的备份文件..."

    # find -mtime +N 查找 N 天前修改的文件
    local old_files=$(find "$backup_dir" -name "*.tar.gz" -mtime +$keep_days -type f 2>/dev/null)
    local count=$(echo "$old_files" | grep -c . 2>/dev/null || echo 0)

    if [ "$count" -gt 0 ] && [ -n "$old_files" ]; then
        echo "$old_files" | while read file; do
            log_info "删除旧备份：$file"
            rm -f "$file"
        done
        log_info "共删除 $count 个旧备份"
    else
        log_info "没有需要清理的旧备份"
    fi

    return 0
}

echo ""
echo "--- 备份演示 ---"

# 创建测试源目录
src_test="./backup_source"
backup_test_dir="./backup_dest"
mkdir -p "$src_test/subdir1"
echo "文件1内容" > "$src_test/file1.txt"
echo "文件2内容" > "$src_test/file2.txt"
echo "子目录文件" > "$src_test/subdir1/file3.txt"

log_info "创建测试源目录：$src_test"

# 执行备份
backup_path=$(backup_dir "$src_test" "$backup_test_dir")
if [ $? -eq 0 ]; then
    log_info "备份文件已生成：$backup_path"

    echo ""
    echo "备份文件内容："
    tar -tzf "$backup_path"                       # 列出压缩包内容
fi

# 清理测试文件
rm -rf "$src_test" "$backup_test_dir"

echo ""
echo "========== 3. 文件批量重命名 =========="

# -------------------- 3. 文件批量重命名 --------------------

echo "--- 批量添加前缀 ---"

# 创建测试文件
rename_dir="./rename_test"
mkdir -p "$rename_dir"
touch "$rename_dir/a.txt" "$rename_dir/b.txt" "$rename_dir/c.txt"
touch "$rename_dir/photo1.jpg" "$rename_dir/photo2.jpg"

echo "重命名前："
ls "$rename_dir"

echo ""
echo "给所有 .txt 文件添加前缀 \"new_\"："
for file in "$rename_dir"/*.txt; do
    if [ -f "$file" ]; then
        dir=$(dirname "$file")
        filename=$(basename "$file")
        newname="${dir}/new_${filename}"
        mv "$file" "$newname"
        echo "  $filename -> new_$filename"
    fi
done

echo ""
echo "重命名后："
ls "$rename_dir"

echo ""
echo "--- 批量修改扩展名 ---"

echo "把 .txt 改成 .md："
for file in "$rename_dir"/*.txt; do
    if [ -f "$file" ]; then
        newname="${file%.txt}.md"              # 用变量替换修改扩展名
        mv "$file" "$newname"
        echo "  $(basename "$file") -> $(basename "$newname")"
    fi
done

echo ""
echo "修改后："
ls "$rename_dir"

echo ""
echo "--- 按序号重命名 ---"

echo "按序号重命名 jpg 文件："
i=1
for file in "$rename_dir"/*.jpg; do
    if [ -f "$file" ]; then
        dir=$(dirname "$file")
        ext="${file##*.}"                      # 获取扩展名
        newname=$(printf "%s/image_%03d.%s" "$dir" "$i" "$ext")
        mv "$file" "$newname"
        echo "  $(basename "$file") -> image_$(printf "%03d" $i).$ext"
        i=$((i + 1))
    fi
done

echo ""
echo "最终结果："
ls "$rename_dir"

# 清理
rm -rf "$rename_dir"

echo ""
echo "========== 4. 服务状态检查 =========="

# -------------------- 4. 服务状态检查 --------------------
# 检查系统服务是否在运行
# 支持多种方式检查进程

echo "--- 进程检查函数 ---"

# 检查进程是否在运行（通过进程名）
# 参数：$1=进程名
is_process_running() {
    local process_name="$1"

    # pgrep 按名称查找进程，比 ps | grep 更可靠
    if pgrep -x "$process_name" > /dev/null 2>&1; then
        return 0  # 运行中
    else
        return 1  # 未运行
    fi
}

# 检查端口是否被占用
# 参数：$1=端口号
is_port_listening() {
    local port="$1"

    # 用 netstat 或 ss 检查
    if command -v ss > /dev/null 2>&1; then
        ss -tlnp 2>/dev/null | grep -q ":$port "
    elif command -v netstat > /dev/null 2>&1; then
        netstat -tlnp 2>/dev/null | grep -q ":$port "
    else
        # 用 lsof 试试
        lsof -i ":$port" > /dev/null 2>&1
    fi

    return $?
}

# 检查 HTTP 服务是否可用
# 参数：$1=URL
is_http_alive() {
    local url="$1"
    local timeout=${2:-5}

    # 用 curl 检查 HTTP 状态码
    if command -v curl > /dev/null 2>&1; then
        local status=$(curl -s -o /dev/null -w "%{http_code}" --connect-timeout "$timeout" "$url" 2>/dev/null)
        if [ "$status" = "200" ] || [ "$status" = "301" ] || [ "$status" = "302" ]; then
            return 0
        fi
    fi

    return 1
}

echo "服务检查演示："
echo ""

# 检查一些常见进程
for proc in bash sh; do
    if is_process_running "$proc"; then
        echo "  [运行中] $proc"
    else
        echo "  [未运行] $proc"
    fi
done

echo ""
echo "检查常见端口："
for port in 22 80 443 3306 8080; do
    if is_port_listening "$port"; then
        echo "  [监听中] 端口 $port"
    else
        echo "  [未监听] 端口 $port"
    fi
done

echo ""
echo "========== 5. 磁盘空间监控告警 =========="

# -------------------- 5. 磁盘空间监控告警 --------------------
# 监控磁盘使用情况，超过阈值发出告警

echo "--- 磁盘空间检查 ---"

# 检查磁盘空间
# 参数：$1=挂载点, $2=告警阈值(百分比)
check_disk_space() {
    local mount_point="$1"
    local threshold="$2"

    # df 获取磁盘使用情况
    # -P 使用 POSIX 格式（输出更稳定）
    # -h 人类可读格式
    local usage_percent=$(df -P "$mount_point" 2>/dev/null | awk 'NR==2 {print $5}' | tr -d '%')

    if [ -z "$usage_percent" ]; then
        log_error "无法获取磁盘使用情况：$mount_point"
        return 1
    fi

    local used=$(df -Ph "$mount_point" 2>/dev/null | awk 'NR==2 {print $3}')
    local total=$(df -Ph "$mount_point" 2>/dev/null | awk 'NR==2 {print $2}')
    local available=$(df -Ph "$mount_point" 2>/dev/null | awk 'NR==2 {print $4}')

    log_info "磁盘 $mount_point：已用 $used / $total（$usage_percent%），可用 $available"

    if [ "$usage_percent" -ge "$threshold" ]; then
        log_warn "告警：磁盘使用率 $usage_percent% 超过阈值 $threshold%"
        return 2  # 2 表示超过阈值
    else
        log_info "磁盘空间正常"
        return 0
    fi
}

# 检查系统所有磁盘分区
check_all_disks() {
    local threshold=${1:-80}

    log_info "开始检查所有磁盘分区，阈值：${threshold}%"
    echo ""

    # df -P 获取所有挂载点，跳过 tmpfs 和其它虚拟文件系统
    df -P 2>/dev/null | grep -vE 'tmpfs|devtmpfs|Filesystem' | while read line; do
        local mount_point=$(echo "$line" | awk '{print $6}')
        check_disk_space "$mount_point" "$threshold"
        echo ""
    done
}

echo "检查当前磁盘："
check_disk_space "/" 90                  # 阈值设高一点，避免误报

echo ""
echo "（检查所有磁盘分区的函数已定义：check_all_disks）"

echo ""
echo "--- 内存使用检查 ---"

check_memory() {
    if [ -f /proc/meminfo ]; then
        # Linux 系统
        local total=$(grep MemTotal /proc/meminfo | awk '{print $2}')
        local free=$(grep MemFree /proc/meminfo | awk '{print $2}')
        local available=$(grep MemAvailable /proc/meminfo | awk '{print $2}')

        local total_mb=$((total / 1024))
        local available_mb=$((available / 1024))
        local used_mb=$(((total - available) / 1024))
        local usage=$(( (total - available) * 100 / total ))

        log_info "内存：已用 ${used_mb}MB / ${total_mb}MB（${usage}%），可用 ${available_mb}MB"
    else
        # macOS 或其它系统
        log_warn "无法读取内存信息（非Linux系统）"
    fi
}

check_memory 2>/dev/null || echo "（内存检查跳过，非Linux环境）"

echo ""
echo "========== 6. 批量文件处理 =========="

# -------------------- 6. 批量文件处理 --------------------

echo "--- 批量查找替换 ---"

# 创建测试文件
batch_dir="./batch_process"
mkdir -p "$batch_dir"
cat > "$batch_dir/file1.txt" << 'EOF'
这是文件1，包含 PLACEHOLDER 占位符
还有另一个 PLACEHOLDER
EOF
cat > "$batch_dir/file2.txt" << 'EOF'
文件2也有 PLACEHOLDER
EOF
cat > "$batch_dir/file3.txt" << 'EOF'
文件3没有占位符
EOF

echo "批量替换前（统计 PLACEHOLDER 出现次数）："
grep -c "PLACEHOLDER" "$batch_dir"/*.txt

echo ""
echo "执行批量替换：将 PLACEHOLDER 替换为 ACTUAL_VALUE"
find "$batch_dir" -name "*.txt" -type f -print0 | while IFS= read -r -d '' file; do
    # 使用 sed 原地替换
    sed -i 's/PLACEHOLDER/ACTUAL_VALUE/g' "$file"
    echo "  已处理：$file"
done

echo ""
echo "批量替换后："
grep "ACTUAL_VALUE" "$batch_dir"/*.txt

echo ""
echo "--- 批量统计文件行数 ---"

echo "统计每个 .txt 文件的行数："
total_lines=0
for file in "$batch_dir"/*.txt; do
    if [ -f "$file" ]; then
        lines=$(wc -l < "$file")
        echo "  $(basename "$file")：$lines 行"
        total_lines=$((total_lines + lines))
    fi
done
echo "  ------------"
echo "  总计：$total_lines 行"

echo ""
echo "--- 批量转换文件编码（示例说明）---"
echo "iconv -f GBK -t UTF-8 input.txt > output.txt   # GBK 转 UTF-8"
echo "批量转换可以用 find + iconv 配合循环实现"

# 清理
rm -rf "$batch_dir"

echo ""
echo "--- 批量压缩图片（示例说明）---"
echo "使用 ImageMagick 的 convert 命令可以批量压缩图片"
echo "for img in *.jpg; do"
echo "    convert \"\$img\" -quality 80 \"compressed/\$img\""
echo "done"

echo ""
echo "==================== 脚本结束 ===================="
echo "恭喜你完成了所有基础学习！"
echo "建议多动手实践，在实际项目中运用这些知识"
