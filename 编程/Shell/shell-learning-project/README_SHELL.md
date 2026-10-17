# Shell/Bash 脚本学习项目

一个完整的 Shell/Bash 脚本学习项目，包含从基础到高级的所有核心知识点，每个脚本都有极其详细的中文注释。

## 项目结构

```
shell-learning-project/
├── main.sh                 # 主菜单脚本（交互运行各脚本）
├── README_SHELL.md         # 学习说明（本文件）
├── 01_basics/              # 第1章：基础语法
│   ├── 01_hello_world.sh   # Hello World、变量、引号、输入输出、特殊变量
│   └── 02_variables.sh     # 字符串操作、数组、关联数组、数值计算
├── 02_flow_control/        # 第2章：流程控制
│   ├── 01_conditionals.sh  # if/elif/else、case、test、逻辑运算
│   └── 02_loops.sh         # for、while、until、break/continue
├── 03_functions/           # 第3章：函数
│   └── 01_functions.sh     # 函数定义、参数、返回值、递归、函数库
├── 04_text_processing/     # 第4章：文本处理
│   └── 01_text_tools.sh    # grep、sed、awk、cut、sort、find、xargs
├── 05_advanced/            # 第5章：高级技巧
│   └── 01_advanced.sh      # 重定向、管道、子shell、信号、正则、调试
└── 06_examples/            # 第6章：综合案例
    └── 01_utils.sh         # 日志、备份、重命名、监控等实用脚本
```

## 快速开始

### 方式一：使用主菜单（推荐）

```bash
# 进入项目目录
cd shell-learning-project

# 运行主菜单
bash main.sh
```

主菜单提供交互式界面，可以选择运行任意章节的脚本。

### 方式二：直接运行单个脚本

```bash
# 运行指定脚本
bash 01_basics/01_hello_world.sh

# 也可以给脚本加执行权限后直接运行
chmod +x 01_basics/01_hello_world.sh
./01_basics/01_hello_world.sh
```

## 运行环境说明

### 支持的环境

本项目的脚本基于 Bash 编写，支持以下运行环境：

| 环境 | 说明 |
|------|------|
| **Linux** | 原生支持，推荐使用 Ubuntu、CentOS 等发行版 |
| **macOS** | 自带 Bash（版本较旧，建议升级到 Bash 4+） |
| **Git Bash** | Windows 上安装 Git 后自带，推荐 Windows 用户使用 |
| **WSL** | Windows Subsystem for Linux，完整的 Linux 环境 |
| **Cygwin** | Windows 上的类 Unix 环境 |

### 环境要求

- Bash 版本 4.0 及以上（关联数组等特性需要）
- 基础命令工具：grep、sed、awk、find、sort 等
- 部分示例需要：bc（高精度计算）

### 检查 Bash 版本

```bash
bash --version
echo $BASH_VERSION
```

### Windows 用户建议

**方案一：Git Bash（最简单）**

1. 下载安装 Git for Windows：https://git-scm.com/download/win
2. 安装完成后，右键选择 "Git Bash Here" 打开终端
3. 进入项目目录，运行 `bash main.sh`

**方案二：WSL（推荐进阶用户）**

1. 以管理员身份打开 PowerShell，运行：
   ```powershell
   wsl --install
   ```
2. 重启电脑，设置 Linux 用户名和密码
3. 在 WSL 终端中进入项目目录运行脚本

**方案三：Cygwin**

1. 下载安装 Cygwin：https://www.cygwin.com/
2. 安装时选择 bash、grep、sed、awk 等包
3. 使用 Cygwin 终端运行脚本

## 学习路径建议

### 初学者（按顺序学习）

1. **第1章：基础语法**
   - `01_hello_world.sh` - 了解脚本结构、变量、输入输出
   - `02_variables.sh` - 掌握字符串操作、数组、数值计算

2. **第2章：流程控制**
   - `01_conditionals.sh` - 条件判断 if/case/test
   - `02_loops.sh` - 循环 for/while/until

3. **第3章：函数**
   - `01_functions.sh` - 函数定义、参数传递、返回值

4. **第4章：文本处理**
   - `01_text_tools.sh` - grep/sed/awk 三剑客及其他工具

5. **第5章：高级技巧**
   - `01_advanced.sh` - 重定向、管道、信号、正则、调试

6. **第6章：综合案例**
   - `01_utils.sh` - 实际应用场景的脚本案例

### 有基础者

直接跳到感兴趣的章节，重点看：
- 文本处理（第4章）：grep/sed/awk 是 Shell 编程的核心
- 高级技巧（第5章）：深入理解 Shell 运行机制
- 综合案例（第6章）：学习实际项目中的写法

## 学习方法

1. **逐行阅读注释**：每个脚本都有详细的中文注释，解释每个语法点
2. **动手运行**：运行脚本看实际输出，加深理解
3. **修改实验**：尝试修改脚本中的代码，观察运行结果的变化
4. **做笔记**：记录重要的知识点和容易出错的地方
5. **实际应用**：找一些实际问题，尝试用 Shell 脚本来解决

## 注意事项

### 常见坑点

1. **等号两边不能有空格**
   ```bash
   # 错误
   name = "张三"
   # 正确
   name="张三"
   ```

2. **[ ] 两边必须有空格**
   ```bash
   # 错误
   if [$a -eq $b]; then
   # 正确
   if [ $a -eq $b ]; then
   ```

3. **变量引用最好加双引号**
   ```bash
   # 当变量包含空格时，不加引号会出问题
   name="Zhang San"
   echo $name      # 可能出问题
   echo "$name"    # 安全
   ```

4. **命令替换推荐用 $()**
   ```bash
   # 旧写法（不推荐）
   date_now=`date`
   # 新写法（推荐）
   date_now=$(date)
   ```

5. **管道中的变量在循环外不可见**
   ```bash
   # 管道右边的命令在子 shell 中运行
   count=0
   cat file | while read line; do
       count=$((count + 1))
   done
   echo $count   # 还是 0！
   ```

### 编码问题

- 脚本文件请使用 UTF-8 编码
- Windows 用户注意：不要用记事本编辑脚本，可能会添加 BOM 头
- 推荐使用 VS Code、Notepad++ 等专业编辑器

### 换行符问题

- Linux/macOS 使用 LF（\n）换行
- Windows 使用 CRLF（\r\n）换行
- 如果脚本在 Windows 上编辑过，可能需要转换换行符：
  ```bash
  # 安装 dos2unix 工具
  dos2unix script.sh
  # 或者用 sed
  sed -i 's/\r$//' script.sh
  ```

### 权限问题

- 脚本需要执行权限才能直接运行：`chmod +x script.sh`
- 也可以用 `bash script.sh` 的方式运行（不需要执行权限）

## 推荐资源

### 在线教程
- Bash 指南（英文）：https://mywiki.wooledge.org/BashGuide
- Linuxize Shell 教程：https://linuxize.com/category/bash/
- Shell 脚本教程（菜鸟教程）：https://www.runoob.com/linux/linux-shell.html

### 参考手册
- Bash 官方手册：https://www.gnu.org/software/bash/manual/
- man 手册：`man bash`、`man grep`、`man sed`、`man awk`

### 练习网站
- LeetCode Shell 题目：https://leetcode.cn/problemset/shell/
- Exercism Bash Track：https://exercism.org/tracks/bash

## 版本信息

- 版本：1.0
- 最后更新：2024
- 适用 Bash 版本：4.0+

---

祝你学习愉快！有问题多查 man 手册，多动手实践。
