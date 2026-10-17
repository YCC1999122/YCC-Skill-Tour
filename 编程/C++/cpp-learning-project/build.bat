@echo off
REM ============================================================================
REM 文件名: build.bat
REM 描述: Windows平台构建脚本
REM 作者: C++学习项目
REM 说明: 使用MSVC (cl.exe) 或 MinGW (g++) 编译C++学习项目
REM      自动检测可用的编译器并选择合适的编译方式
REM ============================================================================

setlocal enabledelayedexpansion

REM 设置项目根目录（脚本所在目录）
set PROJECT_DIR=%~dp0
set SRC_DIR=%PROJECT_DIR%src
set INCLUDE_DIR=%PROJECT_DIR%include
set TARGET=cpp_learning.exe
set BUILD_DIR=%PROJECT_DIR%build

echo ============================================================
echo    C++学习项目 - Windows构建脚本
echo ============================================================
echo.

REM ============================================================================
REM 检测编译器
REM ============================================================================

REM 先检测是否有 MSVC (cl.exe)
where cl >nul 2>nul
if %errorlevel%==0 (
    set COMPILER=msvc
    echo 检测到编译器: MSVC (cl.exe)
    goto :build_msvc
)

REM 再检测是否有 MinGW (g++)
where g++ >nul 2>nul
if %errorlevel%==0 (
    set COMPILER=mingw
    echo 检测到编译器: MinGW (g++)
    goto :build_mingw
)

REM 如果都没找到，提示用户
echo.
echo 错误: 未找到C++编译器！
echo.
echo 请安装以下任一编译器:
echo   1. Visual Studio (Community版本免费)
echo      安装后在"Developer Command Prompt for VS"中运行此脚本
echo   2. MinGW-w64 (TDM-GCC, MinGW-w64等)
echo      安装后将bin目录添加到PATH环境变量
echo.
pause
exit /b 1

REM ============================================================================
REM 使用 MSVC 编译
REM ============================================================================
:build_msvc
echo.
echo 开始使用 MSVC 编译...
echo.

REM 创建构建目录
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM 收集所有源文件
set SRCS=
for /r "%SRC_DIR%" %%f in (*.cpp) do (
    set SRCS=!SRCS! "%%f"
)

REM 编译选项
REM /EHsc: 启用C++异常处理
REM /std:c++17: 使用C++17标准
REM /W4: 警告等级4
REM /O2: 优化等级2
REM /I: 头文件搜索路径
REM /Fo: 目标文件输出目录
set CXXFLAGS=/EHsc /std:c++17 /W4 /O2 /I "%INCLUDE_DIR%"

echo 编译选项: %CXXFLAGS%
echo.

REM 编译并链接
REM /Fe 指定输出文件名
cl %CXXFLAGS% %SRCS% /Fe:"%PROJECT_DIR%%TARGET%" /Fo"%BUILD_DIR%\\"

if %errorlevel%==0 (
    echo.
    echo ============================================================
    echo    编译成功！
    echo    可执行文件: %PROJECT_DIR%%TARGET%
    echo    运行方式: %TARGET%
    echo ============================================================
) else (
    echo.
    echo 编译失败，请检查错误信息。
)

goto :end

REM ============================================================================
REM 使用 MinGW 编译
REM ============================================================================
:build_mingw
echo.
echo 开始使用 MinGW (g++) 编译...
echo.

REM 创建构建目录
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM 收集所有源文件
set SRCS=
for /r "%SRC_DIR%" %%f in (*.cpp) do (
    set SRCS=!SRCS! "%%f"
)

REM 编译选项
REM -std=c++17: C++17标准
REM -Wall -Wextra -Wpedantic: 警告选项
REM -O2: 优化等级2
REM -I: 头文件搜索路径
set CXXFLAGS=-std=c++17 -Wall -Wextra -Wpedantic -O2 -I "%INCLUDE_DIR%"

echo 编译选项: %CXXFLAGS%
echo.

REM 编译并链接
g++ %CXXFLAGS% %SRCS% -o "%PROJECT_DIR%%TARGET%"

if %errorlevel%==0 (
    echo.
    echo ============================================================
    echo    编译成功！
    echo    可执行文件: %PROJECT_DIR%%TARGET%
    echo    运行方式: %TARGET%
    echo ============================================================
) else (
    echo.
    echo 编译失败，请检查错误信息。
)

goto :end

REM ============================================================================
REM 结束
REM ============================================================================
:end
echo.
pause
endlocal
