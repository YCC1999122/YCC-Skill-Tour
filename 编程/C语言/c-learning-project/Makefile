# ========================================
# C 语言学习项目 - Makefile
# ========================================
# 支持 GCC / Clang / MinGW
# 用法:
#   make          - 编译项目
#   make run      - 编译并运行
#   make clean    - 清理构建文件
#   make debug    - 调试模式编译
# ========================================

# 编译器设置
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -lm

# 目录
SRC_DIR     = src
BUILD_DIR   = build
INCLUDE_DIR = include

# 源文件（递归查找所有 .c 文件）
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/basics/*.c) \
       $(wildcard $(SRC_DIR)/pointers/*.c) \
       $(wildcard $(SRC_DIR)/datastruct/*.c) \
       $(wildcard $(SRC_DIR)/patterns/*.c) \
       $(wildcard $(SRC_DIR)/io/*.c)

# 目标文件
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# 可执行文件
TARGET = $(BUILD_DIR)/c-learning

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	@echo "  LD      $@"
	@mkdir -p $(dir $@)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "  编译完成！可执行文件: $@"

# 编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "  CC      $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# 运行
run: $(TARGET)
	@echo "  运行中..."
	@./$(TARGET) -a

# 交互模式运行
run-interactive: $(TARGET)
	@./$(TARGET)

# 调试模式
debug: CFLAGS += -g -O0 -DDEBUG
debug: clean all

# 优化模式
release: CFLAGS += -O2 -DNDEBUG
release: clean all
	@echo "  Release 版本编译完成"

# 静态分析
analyze: CFLAGS += --analyze
analyze: clean all

# 代码统计
count:
	@echo "  代码行数统计:"
	@find . -name "*.c" -o -name "*.h" | xargs wc -l

# 清理
clean:
	@echo "  清理构建文件..."
	@rm -rf $(BUILD_DIR)
	@echo "  清理完成"

# 伪目标
.PHONY: all run run-interactive debug release analyze count clean
