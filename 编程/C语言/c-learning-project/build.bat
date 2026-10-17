@echo off
REM ========================================
REM C 语言学习项目 - Windows 构建脚本
REM ========================================
REM 用法:
REM   build.bat          - 编译项目
REM   build.bat run      - 编译并运行全部演示
REM   build.bat clean    - 清理
REM ========================================

setlocal enabledelayedexpansion

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -Iinclude
set LDFLAGS=-lm
set BUILD_DIR=build
set SRC_DIR=src

if "%1"=="clean" goto clean
if "%1"=="run" goto run_build

REM ===== 编译 =====
echo === C语言学习项目 - 编译中 ===

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
if not exist %BUILD_DIR%\basics mkdir %BUILD_DIR%\basics
if not exist %BUILD_DIR%\pointers mkdir %BUILD_DIR%\pointers
if not exist %BUILD_DIR%\datastruct mkdir %BUILD_DIR%\datastruct
if not exist %BUILD_DIR%\patterns mkdir %BUILD_DIR%\patterns
if not exist %BUILD_DIR%\io mkdir %BUILD_DIR%\io

set OBJS=
for %%f in (%SRC_DIR%\*.c %SRC_DIR%\basics\*.c %SRC_DIR%\pointers\*.c %SRC_DIR%\datastruct\*.c %SRC_DIR%\patterns\*.c %SRC_DIR%\io\*.c) do (
    set "SRC=%%f"
    set "OBJ=!SRC:%SRC_DIR%=%BUILD_DIR%!"
    set "OBJ=!OBJ:.c=.o!"
    echo   CC %%~nxf
    %CC% %CFLAGS% -c "%%f" -o "!OBJ!"
    if errorlevel 1 (
        echo 编译失败！
        exit /b 1
    )
    set OBJS=!OBJS! "!OBJ!"
)

echo   LD  c-learning.exe
%CC% %OBJS% -o %BUILD_DIR%\c-learning.exe %LDFLAGS%
if errorlevel 1 (
    echo 链接失败！
    exit /b 1
)

echo.
echo 编译成功！可执行文件: %BUILD_DIR%\c-learning.exe
echo.
goto end

:run_build
call %0
if errorlevel 1 goto end
echo === 运行全部演示 ===
%BUILD_DIR%\c-learning.exe -a
goto end

:clean
echo 清理构建文件...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
echo 清理完成
goto end

:end
endlocal
