#!/bin/bash
# ==============================================================================
# 文件名：main.sh
# 描述：Shell/Bash 学习项目 - 主菜单交互脚本
# 作者：Shell Learning Project
# 版本：1.0
# ==============================================================================

# 获取脚本所在目录（绝对路径）
# BASH_SOURCE[0] 是当前脚本的路径
# dirname 获取目录部分
# cd && pwd 取得绝对路径
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# -------------------- 颜色定义 --------------------
# 使用 ANSI 转义序列定义颜色，让菜单更美观
# 格式：\033[显示方式;前景色;背景色m
# 显示方式：0=默认, 1=高亮, 4=下划线, 7=反白
# 前景色：30=黑, 31=红, 32=绿, 33=黄, 34=蓝, 35=紫, 36=青, 37=白

RED='\033[0;31m'                      # 红色
GREEN='\033[0;32m'                    # 绿色
YELLOW='\033[0;33m'                   # 黄色
BLUE='\033[0;34m'                     # 蓝色
PURPLE='\033[0;35m'                   # 紫色
CYAN='\033[0;36m'                     # 青色
WHITE='\033[1;37m'                    # 白色（高亮）
NC='\033[0m'                          # 重置颜色（No Color）

# -------------------- 工具函数 --------------------

# 打印带颜色的标题
print_title() {
    echo ""
    echo -e "${CYAN}============================================================${NC}"
    echo -e "${CYAN}  $1${NC}"
    echo -e "${CYAN}============================================================${NC}"
    echo ""
}

# 打印分隔线
print_separator() {
    echo -e "${YELLOW}------------------------------------------------------------${NC}"
}

# 暂停等待用户按键
pause() {
    echo ""
    read -n 1 -s -p "按任意键继续..."   # -n 1 只读取一个字符，-s 不回显
    echo ""
}

# 运行脚本的通用函数
# 参数：$1=脚本路径
run_script() {
    local script_path="$1"

    if [ ! -f "$script_path" ]; then
        echo -e "${RED}错误：脚本文件不存在：$script_path${NC}"
        pause
        return 1
    fi

    print_title "正在运行：$(basename "$script_path")"
    echo -e "${GREEN}脚本路径：$script_path${NC}"
    print_separator
    echo ""

    # 运行脚本，传参给被调用脚本（用 $@ 不好用，这里传剩余参数）
    bash "$script_path" "$@"
    local exit_code=$?

    echo ""
    print_separator
    echo -e "${GREEN}脚本运行结束，退出码：$exit_code${NC}"
    pause
}

# -------------------- 各章节菜单函数 --------------------

# 第1章：基础语法
menu_basics() {
    while true; do
        clear
        print_title "第1章：基础语法"
        echo -e "${WHITE}  1.${NC} 01_hello_world.sh - Hello World 及基础概念"
        echo -e "${WHITE}  2.${NC} 02_variables.sh - 变量进阶（字符串、数组、数值计算）"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-2]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/01_basics/01_hello_world.sh"
                ;;
            2)
                run_script "$SCRIPT_DIR/01_basics/02_variables.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 第2章：流程控制
menu_flow_control() {
    while true; do
        clear
        print_title "第2章：流程控制"
        echo -e "${WHITE}  1.${NC} 01_conditionals.sh - 条件判断（if/case/test）"
        echo -e "${WHITE}  2.${NC} 02_loops.sh - 循环结构（for/while/until）"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-2]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/02_flow_control/01_conditionals.sh"
                ;;
            2)
                run_script "$SCRIPT_DIR/02_flow_control/02_loops.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 第3章：函数
menu_functions() {
    while true; do
        clear
        print_title "第3章：函数"
        echo -e "${WHITE}  1.${NC} 01_functions.sh - 函数定义、参数、返回值、递归、函数库"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-1]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/03_functions/01_functions.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 第4章：文本处理
menu_text_processing() {
    while true; do
        clear
        print_title "第4章：文本处理"
        echo -e "${WHITE}  1.${NC} 01_text_tools.sh - grep/sed/awk/cut/sort/uniq/find 等"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-1]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/04_text_processing/01_text_tools.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 第5章：高级技巧
menu_advanced() {
    while true; do
        clear
        print_title "第5章：高级技巧"
        echo -e "${WHITE}  1.${NC} 01_advanced.sh - 重定向、管道、信号、正则、调试等"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-1]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/05_advanced/01_advanced.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 第6章：综合案例
menu_examples() {
    while true; do
        clear
        print_title "第6章：综合案例"
        echo -e "${WHITE}  1.${NC} 01_utils.sh - 日志、备份、重命名、监控等实用脚本"
        echo ""
        echo -e "${WHITE}  0.${NC} 返回主菜单"
        echo ""
        read -p "请选择要运行的脚本 [0-1]：" choice

        case $choice in
            1)
                run_script "$SCRIPT_DIR/06_examples/01_utils.sh"
                ;;
            0)
                return 0
                ;;
            *)
                echo -e "${RED}无效选择，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# 运行全部脚本
run_all() {
    print_title "运行全部脚本"
    echo -e "${YELLOW}即将按顺序运行所有学习脚本...${NC}"
    echo -e "${YELLOW}每个脚本运行完毕后按任意键继续下一个${NC}"
    pause

    local scripts=(
        "$SCRIPT_DIR/01_basics/01_hello_world.sh"
        "$SCRIPT_DIR/01_basics/02_variables.sh"
        "$SCRIPT_DIR/02_flow_control/01_conditionals.sh"
        "$SCRIPT_DIR/02_flow_control/02_loops.sh"
        "$SCRIPT_DIR/03_functions/01_functions.sh"
        "$SCRIPT_DIR/04_text_processing/01_text_tools.sh"
        "$SCRIPT_DIR/05_advanced/01_advanced.sh"
        "$SCRIPT_DIR/06_examples/01_utils.sh"
    )

    local total=${#scripts[@]}
    local current=0

    for script in "${scripts[@]}"; do
        current=$((current + 1))
        echo ""
        echo -e "${GREEN}[$current/$total] 正在运行：$(basename "$script")${NC}"
        echo ""
        bash "$script"
        echo ""
        if [ $current -lt $total ]; then
            pause
        fi
    done

    print_title "全部脚本运行完成！"
    pause
}

# 关于信息
show_about() {
    print_title "关于本项目"
    echo -e "${GREEN}Shell/Bash 脚本学习项目${NC}"
    echo ""
    echo "项目目录：$SCRIPT_DIR"
    echo "当前Shell：$SHELL"
    echo "Bash版本：$BASH_VERSION"
    echo ""
    echo "本项目包含以下章节："
    echo "  1. 基础语法（01_basics/）"
    echo "  2. 流程控制（02_flow_control/）"
    echo "  3. 函数（03_functions/）"
    echo "  4. 文本处理（04_text_processing/）"
    echo "  5. 高级技巧（05_advanced/）"
    echo "  6. 综合案例（06_examples/）"
    echo ""
    echo "每个脚本都有详细的中文注释，建议逐行阅读学习。"
    echo "推荐学习顺序：按章节顺序依次学习。"
    pause
}

# -------------------- 主菜单 --------------------

main_menu() {
    while true; do
        clear

        # 显示主菜单标题
        echo -e "${GREEN}"
        echo "  ____  _          _ _   _                    _           "
        echo " / ___|| |__   ___| | | | |    ___   __ _  __| | ___ _ __ "
        echo " \\___ \\| '_ \\ / _ \\ | | | |   / _ \\ / _\` |/ _\` |/ _ \\ '__|"
        echo "  ___) | | | |  __/ | | | |__| (_) | (_| | (_| |  __/ |   "
        echo " |____/|_| |_|\\___|_|_| |_____\\___/ \\__,_|\\__,_|\\___|_|   "
        echo -e "${NC}"
        echo -e "${YELLOW}                    脚本学习项目 v1.0${NC}"
        echo ""

        echo -e "${WHITE}============================================================${NC}"
        echo -e "${WHITE}  主菜单${NC}"
        echo -e "${WHITE}============================================================${NC}"
        echo ""
        echo -e "${WHITE}  1.${NC} 第1章 - 基础语法"
        echo -e "${WHITE}  2.${NC} 第2章 - 流程控制"
        echo -e "${WHITE}  3.${NC} 第3章 - 函数"
        echo -e "${WHITE}  4.${NC} 第4章 - 文本处理"
        echo -e "${WHITE}  5.${NC} 第5章 - 高级技巧"
        echo -e "${WHITE}  6.${NC} 第6章 - 综合案例"
        echo ""
        echo -e "${WHITE}  a.${NC} 运行全部脚本（按顺序）"
        echo -e "${WHITE}  b.${NC} 关于本项目"
        echo -e "${WHITE}  q.${NC} 退出"
        echo ""
        echo -e "${WHITE}============================================================${NC}"
        echo ""

        read -p "请输入你的选择 [1-6 / a / b / q]：" choice

        case $choice in
            1)
                menu_basics
                ;;
            2)
                menu_flow_control
                ;;
            3)
                menu_functions
                ;;
            4)
                menu_text_processing
                ;;
            5)
                menu_advanced
                ;;
            6)
                menu_examples
                ;;
            a|A)
                run_all
                ;;
            b|B)
                show_about
                ;;
            q|Q|quit|exit)
                echo ""
                echo -e "${GREEN}感谢使用，再见！${NC}"
                echo ""
                exit 0
                ;;
            *)
                echo -e "${RED}无效选择：$choice，请重新输入${NC}"
                sleep 1
                ;;
        esac
    done
}

# -------------------- 程序入口 --------------------

# 检查是否在 Bash 环境中运行
if [ -z "$BASH_VERSION" ]; then
    echo "警告：当前不是 Bash 环境，某些功能可能无法正常工作"
    echo "建议使用 Bash 运行此脚本"
    sleep 2
fi

# 启动主菜单
main_menu
