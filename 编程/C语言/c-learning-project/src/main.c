/**
 * @file main.c
 * @brief C语言学习项目 - 主程序入口
 *
 * 这是一个综合性的C语言学习项目，涵盖：
 *   - 基础语法（数据类型、运算符、控制流、函数）
 *   - 指针与数组（指针运算、多级指针、字符串、二维数组）
 *   - 数据结构（结构体、联合体、枚举、链表、栈、队列）
 *   - 内存管理（malloc/free、内存池、引用计数、泄漏检测）
 *   - 文件IO（文本/二进制读写、格式化输出）
 *   - 设计模式（OOP模拟、单例、观察者、策略、工厂、状态机）
 *
 * 项目采用模块化架构：
 *   include/  - 头文件（对外接口）
 *   src/      - 源文件（实现细节）
 *     basics/   - 基础语法模块
 *     pointers/ - 指针与字符串模块
 *     datastruct/ - 数据结构与内存模块
 *     io/       - 文件IO模块
 *     patterns/ - 设计模式模块
 *   build/    - 编译输出
 *   tests/    - 测试代码
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basics.h"
#include "pointers.h"
#include "datastruct.h"
#include "memory.h"
#include "file_io.h"
#include "patterns.h"

/* 菜单项结构 */
typedef struct {
    const char *title;
    void (*demo_func)(void);
} MenuItem;

/* ===== 各模块演示函数声明 ===== */
static void run_basics_demo(void);
static void run_pointers_demo(void);
static void run_datastruct_demo(void);
static void run_memory_demo(void);
static void run_fileio_demo(void);
static void run_patterns_demo(void);
static void run_all_demos(void);

/* ===== 打印分隔线 ===== */
static void print_separator(char ch, int len)
{
    for (int i = 0; i < len; i++) putchar(ch);
    putchar('\n');
}

/* ===== 打印标题 ===== */
static void print_title(const char *title)
{
    int len = (int)strlen(title) + 8;
    print_separator('=', len);
    printf("   %s\n", title);
    print_separator('=', len);
}

/* ========================================
 * 主菜单
 * ======================================== */
static void print_menu(void)
{
    printf("\n");
    print_title("C 语言学习项目 - 交互菜单");
    printf("  1. 基础语法演示（数据类型、运算符、控制流、函数）\n");
    printf("  2. 指针与字符串演示\n");
    printf("  3. 数据结构演示（结构体、链表、栈、队列）\n");
    printf("  4. 内存管理演示（内存池、引用计数、泄漏检测）\n");
    printf("  5. 文件IO演示\n");
    printf("  6. 设计模式演示（OOP、单例、观察者、策略、FSM）\n");
    printf("  7. 运行全部演示\n");
    printf("  0. 退出\n");
    printf("  请选择: ");
    fflush(stdout);
}

/* ========================================
 * 1. 基础语法演示
 * ======================================== */
static void run_basics_demo(void)
{
    print_title("模块一：基础语法");
    demo_basic_types();
    demo_operators();
    demo_control_flow();

    /* 函数演示 */
    printf("========== 4. 函数 ==========\n");
    printf("  递归: factorial(5) = %d\n", factorial(5));

    int add_result = apply_math_func(add, 10, 20);
    int mul_result = apply_math_func(multiply, 10, 20);
    printf("  函数指针: add(10,20)=%d, multiply(10,20)=%d\n",
           add_result, mul_result);

    int sum = sum_multiple(5, 1, 2, 3, 4, 5);
    printf("  可变参数: sum(1,2,3,4,5) = %d\n", sum);
    printf("\n");

    demo_storage_classes();
    demo_type_casting();
    demo_bitwise_operations();
}

/* ========================================
 * 2. 指针与字符串演示
 * ======================================== */
static void run_pointers_demo(void)
{
    print_title("模块二：指针与字符串");
    demo_pointer_basics();
    demo_pointer_arithmetic();
    demo_multi_level_pointers();
    demo_pointers_and_arrays();
    demo_string_operations();
    demo_2d_array();
    demo_function_pointer_table();
    demo_pointer_advanced();

    /* 动态数组操作演示 */
    printf("========== 动态数组操作 ==========\n");
    size_t n = 6;
    int *arr = create_dynamic_array(n);
    if (arr != NULL) {
        for (size_t i = 0; i < n; i++) arr[i] = (int)(i * 10 + 5);
        printf("  原始数组: ");
        print_array(arr, n);
        printf("\n");

        reverse_array(arr, n);
        printf("  反转后:   ");
        print_array(arr, n);
        printf("\n");

        printf("  最大值: %d\n", find_max(arr, n));
        free(arr);
    }
    printf("\n");
}

/* ========================================
 * 3. 数据结构演示
 * ======================================== */
static void run_datastruct_demo(void)
{
    print_title("模块三：数据结构");
    demo_struct_union_enum();

    /* 学生结构体演示 */
    printf("========== 学生管理示例 ==========\n");
    Student *s = student_create("张三", 20, 'M', 88.5f);
    if (s != NULL) {
        s->birthday.year = 2004;
        s->birthday.month = 3;
        s->birthday.day = 15;
        student_add_score(s, 85);
        student_add_score(s, 92);
        student_add_score(s, 78);
        student_print(s);
        student_destroy(s);
    }
    printf("\n");

    /* 链表演示 */
    printf("========== 单链表 ==========\n");
    LinkedList *list = list_create();
    if (list != NULL) {
        list_push_back(list, 10);
        list_push_back(list, 20);
        list_push_back(list, 30);
        list_push_front(list, 5);
        printf("  链表: ");
        list_print(list);
        printf(" (大小: %zu)\n", list_size(list));

        list_insert(list, 2, 25);
        printf("  插入后: ");
        list_print(list);
        printf("\n");

        list_reverse(list);
        printf("  反转后: ");
        list_print(list);
        printf("\n");

        int val;
        list_remove(list, 1, &val);
        printf("  删除索引1: 值=%d, 链表: ", val);
        list_print(list);
        printf("\n");

        list_destroy(list);
    }
    printf("\n");

    /* 栈演示 */
    printf("========== 栈 ==========\n");
    Stack *stack = stack_create();
    if (stack != NULL) {
        stack_push(stack, 10);
        stack_push(stack, 20);
        stack_push(stack, 30);
        printf("  压入 10, 20, 30 后: ");
        stack_print(stack);
        printf("\n");

        int top;
        stack_peek(stack, &top);
        printf("  栈顶: %d\n", top);

        stack_pop(stack, &top);
        printf("  弹出: %d, 剩余: ", top);
        stack_print(stack);
        printf("\n");

        stack_destroy(stack);
    }
    printf("\n");

    /* 队列演示 */
    printf("========== 队列 ==========\n");
    Queue *queue = queue_create();
    if (queue != NULL) {
        queue_enqueue(queue, 100);
        queue_enqueue(queue, 200);
        queue_enqueue(queue, 300);
        printf("  入队 100, 200, 300: ");
        queue_print(queue);
        printf("\n");

        int front;
        queue_dequeue(queue, &front);
        printf("  出队: %d, 剩余: ", front);
        queue_print(queue);
        printf("\n");

        queue_destroy(queue);
    }
    printf("\n");
}

/* ========================================
 * 4. 内存管理演示
 * ======================================== */
static void run_memory_demo(void)
{
    print_title("模块四：内存管理");
    demo_memory_basics();

    /* 内存池演示 */
    printf("========== 内存池演示 ==========\n");
    MemoryPool *pool = mempool_create(64, 10);  /* 10块，每块64字节 */
    if (pool != NULL) {
        mempool_print_status(pool);
        printf("\n");

        void *p1 = mempool_alloc(pool);
        void *p2 = mempool_alloc(pool);
        void *p3 = mempool_alloc(pool);
        printf("  分配3块后: ");
        mempool_print_status(pool);
        printf("\n");

        mempool_free(pool, p2);
        printf("  释放p2后:   ");
        mempool_print_status(pool);
        printf("\n");

        void *p4 = mempool_alloc(pool);
        printf("  再分配1块:  ");
        mempool_print_status(pool);
        printf("\n");

        mempool_destroy(pool);
    }
    printf("\n");

    /* 引用计数演示 */
    printf("========== 引用计数演示 ==========\n");
    RefCounted *rc = refcount_create(sizeof(int), NULL);
    if (rc != NULL) {
        int *data = (int*)refcount_data(rc);
        *data = 42;
        printf("  创建后: 引用计数=%zu, 数据=%d\n", refcount_count(rc), *data);

        RefCounted *rc2 = refcount_retain(rc);
        printf("  retain后: 引用计数=%zu\n", refcount_count(rc));

        refcount_release(rc2);
        printf("  release一次: 引用计数=%zu\n", refcount_count(rc));

        refcount_release(rc);  /* 计数归零，释放内存 */
        printf("  release第二次: 对象已销毁\n");
    }
    printf("\n");

    /* 内存泄漏检测演示 */
    printf("========== 内存泄漏检测 ==========\n");
    int *leak_test = (int*)MALLOC(100 * sizeof(int));
    printf("  分配了 100 个 int (使用 MALLOC 宏追踪)\n");
    /* 故意不释放以演示泄漏检测 */
    /* FREE(leak_test); */
    (void)leak_test;  /* 避免未使用警告 */

    int *normal_alloc = (int*)malloc(50 * sizeof(int));
    free(normal_alloc);
    printf("  普通 malloc/free: 不被追踪\n");

    debug_memory_leak_check();

    /* 修复：释放泄漏的内存 */
    printf("  修复: 释放泄漏内存...\n");
    FREE(leak_test);
    debug_memory_leak_check();
    printf("\n");
}

/* ========================================
 * 5. 文件IO演示
 * ======================================== */
static void run_fileio_demo(void)
{
    print_title("模块五：文件IO");
    demo_file_io();
}

/* ========================================
 * 6. 设计模式演示
 * ======================================== */
static void run_patterns_demo(void)
{
    print_title("模块六：设计模式与架构");
    demo_design_patterns();
}

/* ========================================
 * 运行全部演示
 * ======================================== */
static void run_all_demos(void)
{
    run_basics_demo();
    run_pointers_demo();
    run_datastruct_demo();
    run_memory_demo();
    run_fileio_demo();
    run_patterns_demo();
}

/* ========================================
 * 主函数
 * ======================================== */
int main(int argc, char *argv[])
{
    /* 命令行参数模式：带 -a 参数直接运行全部 */
    if (argc > 1 && strcmp(argv[1], "-a") == 0) {
        run_all_demos();
        printf("\n所有演示完成！\n");
        return 0;
    }

    /* 交互模式 */
    printf("\n");
    printf("  ╔══════════════════════════════════════════════╗\n");
    printf("  ║        C 语言综合学习项目 v1.0              ║\n");
    printf("  ║  涵盖语法 / 指针 / 数据结构 / 内存 / IO / 设计 ║\n");
    printf("  ╚══════════════════════════════════════════════╝\n");

    int choice;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            /* 清除错误输入 */
            while (getchar() != '\n');
            printf("  无效输入，请输入数字。\n");
            continue;
        }
        while (getchar() != '\n');  /* 清除剩余字符 */

        switch (choice) {
            case 1:  run_basics_demo();     break;
            case 2:  run_pointers_demo();   break;
            case 3:  run_datastruct_demo(); break;
            case 4:  run_memory_demo();     break;
            case 5:  run_fileio_demo();     break;
            case 6:  run_patterns_demo();   break;
            case 7:  run_all_demos();       break;
            case 0:
                printf("\n  感谢学习，再见！\n\n");
                return 0;
            default:
                printf("  无效选项，请重新选择。\n");
                break;
        }
    }

    return 0;
}
