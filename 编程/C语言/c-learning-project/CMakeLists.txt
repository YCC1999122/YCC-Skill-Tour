# ========================================
# C 语言学习项目 - CMake 构建脚本
# ========================================
# 用法:
#   mkdir build && cd build
#   cmake ..
#   cmake --build .
# ========================================

cmake_minimum_required(VERSION 3.10)
project(c-learning C)

# C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# 头文件目录
include_directories(include)

# 源文件
set(SOURCES
    src/main.c
    src/basics/basics.c
    src/pointers/pointers.c
    src/datastruct/datastruct.c
    src/datastruct/memory.c
    src/patterns/patterns.c
    src/io/file_io.c
)

# 可执行文件
add_executable(c-learning ${SOURCES})

# 数学库（Unix系统需要）
if(UNIX)
    target_link_libraries(c-learning m)
endif()

# 可执行文件输出目录
set_target_properties(c-learning PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
