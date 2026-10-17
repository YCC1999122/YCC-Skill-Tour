/**
 * @file pointers.c
 * @brief 指针、数组、字符串模块实现
 */

#include "pointers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================
 * 1. 指针基础
 * ======================================== */
void demo_pointer_basics(void)
{
    printf("========== 指针基础 ==========\n");

    int var = 42;
    int *ptr = &var;  /* 指针指向变量地址 */

    printf("  var 的值:   %d\n", var);
    printf("  var 的地址: %p\n", (void*)&var);
    printf("  ptr 的值:   %p (指向var)\n", (void*)ptr);
    printf("  *ptr 解引用: %d\n", *ptr);

    /* 通过指针修改变量 */
    *ptr = 100;
    printf("  *ptr = 100 后, var = %d\n", var);

    /* 空指针 */
    int *null_ptr = NULL;
    printf("  空指针: %p (解引用会导致段错误)\n", (void*)null_ptr);

    /* 野指针（未初始化）- 永远不要使用！ */
    /* int *wild_ptr;  危险！ */

    printf("\n");
}

/* ========================================
 * 2. 指针运算
 * ======================================== */
void demo_pointer_arithmetic(void)
{
    printf("========== 指针运算 ==========\n");

    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;  /* 指向数组首元素 */

    printf("  数组: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("  p  = %p -> %d\n", (void*)p, *p);
    p++;  /* 前进一个int大小 */
    printf("  p++ = %p -> %d\n", (void*)p, *p);
    p += 2;
    printf("  p+=2 = %p -> %d\n", (void*)p, *p);
    p--;
    printf("  p-- = %p -> %d\n", (void*)p, *p);

    /* 指针相减 */
    int *start = arr;
    int *end = &arr[4];
    printf("  end - start = %td (元素个数差)\n", end - start);

    /* 指针比较 */
    printf("  end > start = %d\n", end > start);

    printf("\n");
}

/* ========================================
 * 3. 多级指针
 * ======================================== */
void demo_multi_level_pointers(void)
{
    printf("========== 多级指针 ==========\n");

    int x = 123;
    int *p1 = &x;     /* 一级指针 */
    int **p2 = &p1;   /* 二级指针 */
    int ***p3 = &p2;  /* 三级指针 */

    printf("  x     = %d\n", x);
    printf("  *p1   = %d\n", *p1);
    printf("  **p2  = %d\n", **p2);
    printf("  ***p3 = %d\n", ***p3);

    /* 通过三级指针修改x */
    ***p3 = 999;
    printf("  ***p3 = 999 后, x = %d\n", x);

    printf("\n");
}

/* ========================================
 * 4. 指针与数组
 * ======================================== */
void demo_pointers_and_arrays(void)
{
    printf("========== 指针与数组 ==========\n");

    int arr[] = {1, 2, 3, 4, 5};
    int n = 5;

    /* 数组名是首元素地址（常量指针） */
    printf("  arr    = %p\n", (void*)arr);
    printf("  &arr[0] = %p\n", (void*)&arr[0]);

    /* 三种遍历方式 */
    printf("  下标遍历:   ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("  指针偏移:   ");
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    printf("  指针移动:   ");
    for (int *p = arr; p < arr + n; p++) {
        printf("%d ", *p);
    }
    printf("\n");

    /* 数组指针 vs 指针数组 */
    int (*arr_ptr)[5] = &arr;  /* 数组指针：指向含5个int的数组 */
    int *ptr_arr[3];            /* 指针数组：元素是指针的数组 */

    int a = 10, b = 20, c = 30;
    ptr_arr[0] = &a;
    ptr_arr[1] = &b;
    ptr_arr[2] = &c;

    printf("  数组指针 *arr_ptr[0] = %d\n", (*arr_ptr)[0]);
    printf("  指针数组: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", *ptr_arr[i]);
    }
    printf("\n");

    printf("\n");
}

/* ========================================
 * 5. 动态数组操作
 * ======================================== */

/**
 * @brief 创建动态数组并初始化为0
 * @param size 数组大小
 * @return 数组指针，失败返回NULL
 */
int* create_dynamic_array(size_t size)
{
    if (size == 0) return NULL;
    int *arr = (int*)malloc(size * sizeof(int));
    if (arr != NULL) {
        memset(arr, 0, size * sizeof(int));
    }
    return arr;
}

/**
 * @brief 打印数组
 */
void print_array(const int *arr, size_t size)
{
    if (arr == NULL || size == 0) {
        printf("(空数组)");
        return;
    }
    printf("[");
    for (size_t i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

/**
 * @brief 反转数组（原地）
 */
void reverse_array(int *arr, size_t size)
{
    if (arr == NULL || size < 2) return;
    size_t left = 0;
    size_t right = size - 1;
    while (left < right) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

/**
 * @brief 查找数组最大值
 */
int find_max(const int *arr, size_t size)
{
    if (arr == NULL || size == 0) return 0;
    int max_val = arr[0];
    for (size_t i = 1; i < size; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

/* ========================================
 * 6. 字符串操作（自实现）
 * ======================================== */

size_t my_strlen(const char *str)
{
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') p++;
    return p - str;
}

char* my_strcpy(char *dest, const char *src)
{
    if (dest == NULL || src == NULL) return NULL;
    char *p = dest;
    while ((*p++ = *src++) != '\0')
        ;
    return dest;
}

char* my_strcat(char *dest, const char *src)
{
    if (dest == NULL || src == NULL) return NULL;
    char *p = dest;
    while (*p != '\0') p++;  /* 找到dest末尾 */
    while ((*p++ = *src++) != '\0')
        ;
    return dest;
}

int my_strcmp(const char *s1, const char *s2)
{
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

char* my_strchr(const char *str, char c)
{
    if (str == NULL) return NULL;
    while (*str != '\0') {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

void demo_string_operations(void)
{
    printf("========== 字符串操作 ==========\n");

    char str1[100] = "Hello, ";
    char str2[] = "World!";

    printf("  my_strlen(\"%s\") = %zu\n", str2, my_strlen(str2));

    my_strcat(str1, str2);
    printf("  my_strcat: %s\n", str1);

    char copy[50];
    my_strcpy(copy, str1);
    printf("  my_strcpy: %s\n", copy);

    printf("  my_strcmp(\"abc\", \"abd\") = %d\n", my_strcmp("abc", "abd"));
    printf("  my_strcmp(\"abc\", \"abc\") = %d\n", my_strcmp("abc", "abc"));

    char *found = my_strchr(str1, 'W');
    printf("  my_strchr('W'): %s\n", found ? found : "未找到");

    /* 字符串数组 */
    const char *names[] = {"Alice", "Bob", "Charlie", "David"};
    printf("  字符串数组: ");
    for (int i = 0; i < 4; i++) {
        printf("%s ", names[i]);
    }
    printf("\n");

    printf("\n");
}

/* ========================================
 * 7. 二维数组与指针
 * ======================================== */
void demo_2d_array(void)
{
    printf("========== 二维数组与指针 ==========\n");

    int matrix[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12}
    };

    printf("  矩阵:\n");
    for (int i = 0; i < 3; i++) {
        printf("    ");
        for (int j = 0; j < 4; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    /* 二维数组名是指向第一行的指针 */
    int (*row_ptr)[4] = matrix;  /* 数组指针，指向含4个int的行 */
    printf("  matrix = %p (首行地址)\n", (void*)matrix);
    printf("  *matrix = %p (首元素地址)\n", (void*)*matrix);
    printf("  **matrix = %d (首元素值)\n", **matrix);

    /* 使用指针遍历 */
    printf("  指针遍历: ");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", *(*(matrix + i) + j));
        }
    }
    printf("\n");

    /* 动态二维数组（指针数组方式） */
    int **dyn_matrix = (int**)malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        dyn_matrix[i] = (int*)malloc(4 * sizeof(int));
        for (int j = 0; j < 4; j++) {
            dyn_matrix[i][j] = (i + 1) * 10 + j;
        }
    }
    printf("  动态二维矩阵[1][2] = %d\n", dyn_matrix[1][2]);

    /* 释放动态二维数组 */
    for (int i = 0; i < 3; i++) {
        free(dyn_matrix[i]);
    }
    free(dyn_matrix);

    printf("\n");
}

/* ========================================
 * 8. 函数指针数组（回调表）
 * ======================================== */

static int op_add(int a, int b)      { return a + b; }
static int op_subtract(int a, int b) { return a - b; }
static int op_multiply(int a, int b) { return a * b; }
static int op_divide(int a, int b)   { return b != 0 ? a / b : 0; }

typedef enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV } Operation;

void demo_function_pointer_table(void)
{
    printf("========== 函数指针数组（调度表）==========\n");

    /* 函数指针数组 - 调度表模式（工程常用） */
    int (*op_table[])(int, int) = {
        [OP_ADD] = op_add,
        [OP_SUB] = op_subtract,
        [OP_MUL] = op_multiply,
        [OP_DIV] = op_divide
    };

    int a = 20, b = 5;
    printf("  a = %d, b = %d\n", a, b);
    printf("  加法: %d\n", op_table[OP_ADD](a, b));
    printf("  减法: %d\n", op_table[OP_SUB](a, b));
    printf("  乘法: %d\n", op_table[OP_MUL](a, b));
    printf("  除法: %d\n", op_table[OP_DIV](a, b));

    printf("  调度表大小: %zu 个操作\n",
           sizeof(op_table) / sizeof(op_table[0]));

    printf("\n");
}

/* ========================================
 * 9. 指针进阶：void指针、const指针
 * ======================================== */

/* 通用交换函数 - 使用void指针 */
static void generic_swap(void *a, void *b, size_t size)
{
    unsigned char *p1 = (unsigned char*)a;
    unsigned char *p2 = (unsigned char*)b;
    while (size--) {
        unsigned char temp = *p1;
        *p1++ = *p2;
        *p2++ = temp;
    }
}

void demo_pointer_advanced(void)
{
    printf("========== 指针进阶 ==========\n");

    /* const 指针的三种形式 */
    int x = 10, y = 20;

    /* 指向常量的指针：不能通过指针修改值，但指针可改 */
    const int *ptr_to_const = &x;
    printf("  const int*: *ptr = %d\n", *ptr_to_const);
    ptr_to_const = &y;  /* OK: 指针可以改 */
    printf("  指向y: *ptr = %d\n", *ptr_to_const);
    /* *ptr_to_const = 30;  错误：不能通过指针修改值 */

    /* 常量指针：指针本身不能改，但可以通过指针修改值 */
    int *const const_ptr = &x;
    *const_ptr = 15;  /* OK: 可以修改值 */
    printf("  int* const: x = %d (通过指针修改)\n", x);
    /* const_ptr = &y;  错误：指针不能改 */

    /* 指向常量的常量指针：都不能改 */
    const int *const const_ptr_to_const = &x;
    printf("  const int* const: %d (只读)\n", *const_ptr_to_const);

    /* void 指针 - 通用指针 */
    int i_val = 42;
    double d_val = 3.14;
    void *vptr;

    vptr = &i_val;
    printf("  void* 指向int: %d\n", *(int*)vptr);
    vptr = &d_val;
    printf("  void* 指向double: %.2f\n", *(double*)vptr);

    /* 通用交换函数演示 */
    int a = 100, b = 200;
    printf("  交换前: a=%d, b=%d\n", a, b);
    generic_swap(&a, &b, sizeof(int));
    printf("  交换后: a=%d, b=%d\n", a, b);

    double d1 = 1.5, d2 = 2.5;
    printf("  double交换前: %.1f, %.1f\n", d1, d2);
    generic_swap(&d1, &d2, sizeof(double));
    printf("  double交换后: %.1f, %.1f\n", d1, d2);

    printf("\n");
}
