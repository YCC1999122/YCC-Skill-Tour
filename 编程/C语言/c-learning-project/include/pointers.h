/**
 * @file pointers.h
 * @brief 指针、数组、字符串模块头文件
 */

#ifndef POINTERS_H
#define POINTERS_H

#include <stddef.h>   /* size_t */

/* ===== 指针基础 ===== */
void demo_pointer_basics(void);

/* ===== 指针运算 ===== */
void demo_pointer_arithmetic(void);

/* ===== 多级指针 ===== */
void demo_multi_level_pointers(void);

/* ===== 指针与数组 ===== */
void demo_pointers_and_arrays(void);

/* ===== 动态数组操作 ===== */
int* create_dynamic_array(size_t size);
void  print_array(const int *arr, size_t size);
void  reverse_array(int *arr, size_t size);
int   find_max(const int *arr, size_t size);

/* ===== 字符串操作（自实现） ===== */
size_t my_strlen(const char *str);
char*  my_strcpy(char *dest, const char *src);
char*  my_strcat(char *dest, const char *src);
int    my_strcmp(const char *s1, const char *s2);
char*  my_strchr(const char *str, char c);
void   demo_string_operations(void);

/* ===== 二维数组与指针 ===== */
void demo_2d_array(void);

/* ===== 函数指针数组（回调表） ===== */
void demo_function_pointer_table(void);

/* ===== 指针进阶：void指针、const指针 ===== */
void demo_pointer_advanced(void);

#endif /* POINTERS_H */
