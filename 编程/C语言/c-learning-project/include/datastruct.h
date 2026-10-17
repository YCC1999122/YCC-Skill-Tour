/**
 * @file datastruct.h
 * @brief 数据结构模块头文件
 *
 * 包含：结构体、联合体、枚举、位域、链表、栈、队列
 */

#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* ========================================
 * 结构体、联合体、枚举演示
 * ======================================== */

/* 枚举：星期 */
typedef enum {
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekday;

/* 枚举：状态码 */
typedef enum {
    STATE_IDLE = 0,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
} SystemState;

/* 位域：寄存器/标志位的紧凑表示 */
typedef struct {
    unsigned int carry     : 1;   /* 进位标志 */
    unsigned int zero      : 1;   /* 零标志 */
    unsigned int negative  : 1;   /* 负标志 */
    unsigned int overflow  : 1;   /* 溢出标志 */
    unsigned int interrupt : 1;   /* 中断使能 */
    unsigned int reserved  : 27;  /* 保留位 */
} StatusRegister;

/* 日期结构体 */
typedef struct {
    int year;
    int month;
    int day;
} Date;

/* 学生结构体（复杂示例） */
typedef struct {
    char    name[50];
    int     age;
    char    gender;     /* 'M' / 'F' */
    float   score;
    Date    birthday;   /* 结构体嵌套 */
    int     *scores;    /* 动态数组指针 */
    int     score_count;
} Student;

/* 联合体：多种类型共享内存 */
typedef union {
    int     i_val;
    float   f_val;
    double  d_val;
    char    c_val;
    void    *p_val;
} Variant;

/* 带标签的联合体（Tagged Union）- 工程常用模式 */
typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_STRING } VariantType;
typedef struct {
    VariantType type;
    union {
        int    i;
        float  f;
        char  *s;
    } data;
} TaggedVariant;

/* 结构体演示函数 */
void demo_struct_union_enum(void);

/* 学生操作函数 */
Student* student_create(const char *name, int age, char gender, float score);
void     student_destroy(Student *s);
void     student_print(const Student *s);
void     student_add_score(Student *s, int score);

/* 日期工具函数 */
int  date_compare(const Date *d1, const Date *d2);
int  date_is_leap_year(int year);
int  date_days_in_month(int year, int month);

/* ========================================
 * 单链表
 * ======================================== */
typedef struct ListNode {
    int              data;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    size_t    size;
} LinkedList;

LinkedList*  list_create(void);
void         list_destroy(LinkedList *list);
int          list_push_front(LinkedList *list, int value);
int          list_push_back(LinkedList *list, int value);
int          list_pop_front(LinkedList *list, int *out_value);
int          list_insert(LinkedList *list, size_t index, int value);
int          list_remove(LinkedList *list, size_t index, int *out_value);
int          list_get(const LinkedList *list, size_t index, int *out_value);
size_t       list_size(const LinkedList *list);
bool         list_is_empty(const LinkedList *list);
void         list_print(const LinkedList *list);
void         list_reverse(LinkedList *list);

/* ========================================
 * 栈（链式实现）
 * ======================================== */
typedef struct StackNode {
    int              data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t     size;
} Stack;

Stack* stack_create(void);
void   stack_destroy(Stack *stack);
int    stack_push(Stack *stack, int value);
int    stack_pop(Stack *stack, int *out_value);
int    stack_peek(const Stack *stack, int *out_value);
size_t stack_size(const Stack *stack);
bool   stack_is_empty(const Stack *stack);
void   stack_print(const Stack *stack);

/* ========================================
 * 队列（链式实现）
 * ======================================== */
typedef struct QueueNode {
    int              data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    size_t     size;
} Queue;

Queue* queue_create(void);
void   queue_destroy(Queue *queue);
int    queue_enqueue(Queue *queue, int value);
int    queue_dequeue(Queue *queue, int *out_value);
int    queue_front(const Queue *queue, int *out_value);
size_t queue_size(const Queue *queue);
bool   queue_is_empty(const Queue *queue);
void   queue_print(const Queue *queue);

#endif /* DATASTRUCT_H */
