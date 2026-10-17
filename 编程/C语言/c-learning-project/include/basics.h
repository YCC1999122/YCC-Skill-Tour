/**
 * @file basics.h
 * @brief C语言基础语法头文件
 *
 * 涵盖：数据类型、变量、运算符、控制流、函数声明
 */

#ifndef BASICS_H
#define BASICS_H

#include <stdio.h>

/* ===== 宏定义 ===== */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define TRUE  1
#define FALSE 0

/* ===== 函数返回码 ===== */
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_PARAM = -1,
    ERROR_OUT_OF_MEMORY = -2,
    ERROR_NOT_FOUND = -3
} StatusCode;

/* ===== 基础数据类型演示 ===== */
void demo_basic_types(void);

/* ===== 运算符演示 ===== */
void demo_operators(void);

/* ===== 控制流演示（if/else, switch, 循环） ===== */
void demo_control_flow(void);

/* ===== 函数演示（递归、内联、函数指针） ===== */
int factorial(int n);                    /* 递归函数 */
static inline int add_inline(int a, int b) { return a + b; } /* 内联函数 */
typedef int (*MathFunc)(int, int);       /* 函数指针类型 */
int apply_math_func(MathFunc func, int a, int b); /* 函数指针作为参数 */

/* ===== 可变参数函数 ===== */
int sum_multiple(int count, ...);

/* ===== 存储类演示（static, extern, register, volatile） ===== */
void demo_storage_classes(void);
int get_static_counter(void);  /* 静态变量计数 */

/* ===== 类型转换演示 ===== */
void demo_type_casting(void);

/* ===== 位运算演示 ===== */
void demo_bitwise_operations(void);

#endif /* BASICS_H */
