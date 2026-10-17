# ============================================================================
# 文件名: CMakeLists.txt
# 描述: CMake构建配置文件
# 作者: C++学习项目
# 说明: 使用CMake构建C++学习项目，支持跨平台
# ============================================================================

# CMake最低版本要求
# 3.10是比较新的稳定版本，支持大多数C++17特性
cmake_minimum_required(VERSION 3.10)

# 项目名称
# CXX表示这是C++项目
project(cpp_learning_project CXX)

# ============================================================================
# C++标准设置
# ============================================================================

# 设置C++标准为C++17
# C++17包含了结构化绑定、if constexpr、inline变量等特性
set(CMAKE_CXX_STANDARD 17)

# 要求编译器必须支持指定的C++标准，不能降级
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器特有的扩展，保证代码的可移植性
# 比如GNU的一些扩展在MSVC上可能不支持
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# 编译选项设置
# ============================================================================

# 根据不同的编译器设置不同的编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC 和 Clang 的编译选项
    # -Wall: 开启大部分警告
    # -Wextra: 开启额外警告
    # -Wpedantic: 严格遵循C++标准
    # -O2: 优化等级2（发布版本使用）
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    # MSVC (Visual Studio) 的编译选项
    # /W4: 警告等级4
    # /permissive-: 禁用语言扩展，更严格的标准符合性
    add_compile_options(/W4 /permissive-)
endif()

# ============================================================================
# 源文件收集
# ============================================================================

# 头文件目录
# 使用 include_directories 后，源文件中可以直接 #include "xxx.h"
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集所有源文件
# file(GLOB ...) 用于匹配文件
# 注意: 实际项目中不推荐用GLOB，因为新增文件后CMake不会自动感知
# 这里为了演示项目简单，使用GLOB方式
file(GLOB_RECURSE SOURCES
    "src/*.cpp"    # src目录下所有.cpp文件
)

# ============================================================================
# 可执行文件
# ============================================================================

# 添加可执行文件目标
# 第一个参数是可执行文件名，后面是所有源文件
add_executable(cpp_learning ${SOURCES})

# ============================================================================
# 输出目录设置（可选）
# ============================================================================

# 设置可执行文件输出目录为项目根目录下的 bin
# set_target_properties(cpp_learning PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
# )

# ============================================================================
# 使用说明
# ============================================================================
# Linux/macOS:
#   mkdir build && cd build
#   cmake ..
#   make
#   ./cpp_learning
#
# Windows (Visual Studio):
#   mkdir build && cd build
#   cmake ..
#   cmake --build . --config Release
#   Release\cpp_learning.exe
#
# Windows (MinGW):
#   mkdir build && cd build
#   cmake -G "MinGW Makefiles" ..
#   mingw32-make
#   cpp_learning.exe
# ============================================================================
