/**
 * @file datastruct.c
 * @brief 数据结构模块实现
 *
 * 包含：结构体、联合体、枚举、位域、单链表、栈、队列
 */

#include "datastruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================
 * 结构体、联合体、枚举演示
 * ======================================== */

void demo_struct_union_enum(void)
{
    printf("========== 结构体、联合体、枚举 ==========\n");

    /* ---- 枚举 ---- */
    Weekday today = FRIDAY;
    printf("  枚举 Weekday: 周一=%d, 周五=%d, 周日=%d\n",
           MONDAY, FRIDAY, SUNDAY);
    printf("  today = %d (%s)\n", today,
           today == FRIDAY ? "星期五" : "其他");

    SystemState state = STATE_RUNNING;
    printf("  系统状态: %d (运行中)\n", state);

    /* ---- 位域 ---- */
    StatusRegister sr = {0};
    sr.carry = 1;
    sr.zero = 0;
    sr.negative = 1;
    printf("  状态寄存器大小: %zu 字节\n", sizeof(StatusRegister));
    printf("  标志位: C=%d Z=%d N=%d O=%d I=%d\n",
           sr.carry, sr.zero, sr.negative, sr.overflow, sr.interrupt);

    /* ---- 结构体 ---- */
    Date d1 = {2024, 6, 15};
    printf("  日期: %d-%02d-%02d\n", d1.year, d1.month, d1.day);

    /* 结构体指定初始化（C99） */
    Date d2 = {.year = 2025, .month = 12, .day = 25};
    printf("  指定初始化: %d-%02d-%02d\n", d2.year, d2.month, d2.day);

    /* 结构体比较大小 */
    printf("  日期比较: d1 %s d2\n",
           date_compare(&d1, &d2) < 0 ? "<" : ">");

    /* 闰年测试 */
    printf("  2024年是闰年: %s\n", date_is_leap_year(2024) ? "是" : "否");
    printf("  2025年2月天数: %d\n", date_days_in_month(2025, 2));

    /* ---- 联合体 ---- */
    Variant v;
    v.i_val = 42;
    printf("  联合体大小: %zu 字节 (共用内存)\n", sizeof(Variant));
    printf("  存int: %d\n", v.i_val);
    v.f_val = 3.14f;
    printf("  存float后读float: %.2f (int值被覆盖)\n", v.f_val);

    /* 用联合体判断字节序 */
    union {
        short s;
        char  c[sizeof(short)];
    } endian_test;
    endian_test.s = 0x0102;
    printf("  字节序: %s端\n",
           endian_test.c[0] == 1 ? "大" : "小");

    /* ---- Tagged Union ---- */
    TaggedVariant tv;
    tv.type = TYPE_INT;
    tv.data.i = 100;
    printf("  TaggedUnion (int): %d\n", tv.data.i);

    tv.type = TYPE_FLOAT;
    tv.data.f = 99.5f;
    printf("  TaggedUnion (float): %.1f\n", tv.data.f);

    tv.type = TYPE_STRING;
    tv.data.s = "Hello";
    printf("  TaggedUnion (string): %s\n", tv.data.s);

    /* ---- 结构体对齐 ---- */
    struct AlignTest1 { char a; int b; char c; };
    struct AlignTest2 { char a; char c; int b; };
    printf("  结构体对齐(顺序影响大小):\n");
    printf("    {char,int,char} = %zu 字节\n", sizeof(struct AlignTest1));
    printf("    {char,char,int} = %zu 字节 (优化排列)\n", sizeof(struct AlignTest2));

    printf("\n");
}

/* ========================================
 * 学生操作函数
 * ======================================== */

Student* student_create(const char *name, int age, char gender, float score)
{
    Student *s = (Student*)malloc(sizeof(Student));
    if (s == NULL) return NULL;

    strncpy(s->name, name ? name : "", sizeof(s->name) - 1);
    s->name[sizeof(s->name) - 1] = '\0';
    s->age = age;
    s->gender = gender;
    s->score = score;
    s->birthday.year = 2000;
    s->birthday.month = 1;
    s->birthday.day = 1;
    s->scores = NULL;
    s->score_count = 0;
    return s;
}

void student_destroy(Student *s)
{
    if (s != NULL) {
        free(s->scores);
        free(s);
    }
}

void student_print(const Student *s)
{
    if (s == NULL) {
        printf("(空学生)\n");
        return;
    }
    printf("学生: %s | 年龄: %d | 性别: %c | 成绩: %.1f\n",
           s->name, s->age, s->gender, s->score);
    printf("  生日: %d-%02d-%02d\n",
           s->birthday.year, s->birthday.month, s->birthday.day);
    if (s->score_count > 0 && s->scores != NULL) {
        printf("  成绩列表: ");
        for (int i = 0; i < s->score_count; i++) {
            printf("%d ", s->scores[i]);
        }
        printf("\n");
    }
}

void student_add_score(Student *s, int score)
{
    if (s == NULL) return;
    int *new_scores = (int*)realloc(s->scores, (s->score_count + 1) * sizeof(int));
    if (new_scores == NULL) return;
    s->scores = new_scores;
    s->scores[s->score_count++] = score;
}

/* ========================================
 * 日期工具函数
 * ======================================== */

int date_compare(const Date *d1, const Date *d2)
{
    if (d1->year != d2->year)   return d1->year - d2->year;
    if (d1->month != d2->month) return d1->month - d2->month;
    return d1->day - d2->day;
}

int date_is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int date_days_in_month(int year, int month)
{
    static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && date_is_leap_year(year)) return 29;
    return days[month - 1];
}

/* ========================================
 * 单链表实现
 * ======================================== */

LinkedList* list_create(void)
{
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list != NULL) {
        list->head = NULL;
        list->size = 0;
    }
    return list;
}

void list_destroy(LinkedList *list)
{
    if (list == NULL) return;
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int list_push_front(LinkedList *list, int value)
{
    if (list == NULL) return -1;
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) return -1;
    node->data = value;
    node->next = list->head;
    list->head = node;
    list->size++;
    return 0;
}

int list_push_back(LinkedList *list, int value)
{
    if (list == NULL) return -1;
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) return -1;
    node->data = value;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        ListNode *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
    return 0;
}

int list_pop_front(LinkedList *list, int *out_value)
{
    if (list == NULL || list->head == NULL) return -1;
    ListNode *old_head = list->head;
    if (out_value != NULL) *out_value = old_head->data;
    list->head = old_head->next;
    free(old_head);
    list->size--;
    return 0;
}

int list_insert(LinkedList *list, size_t index, int value)
{
    if (list == NULL || index > list->size) return -1;
    if (index == 0) return list_push_front(list, value);

    ListNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) return -1;
    node->data = value;
    node->next = prev->next;
    prev->next = node;
    list->size++;
    return 0;
}

int list_remove(LinkedList *list, size_t index, int *out_value)
{
    if (list == NULL || index >= list->size) return -1;
    if (index == 0) return list_pop_front(list, out_value);

    ListNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    ListNode *target = prev->next;
    if (out_value != NULL) *out_value = target->data;
    prev->next = target->next;
    free(target);
    list->size--;
    return 0;
}

int list_get(const LinkedList *list, size_t index, int *out_value)
{
    if (list == NULL || index >= list->size) return -1;
    ListNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    if (out_value != NULL) *out_value = current->data;
    return 0;
}

size_t list_size(const LinkedList *list)
{
    return list ? list->size : 0;
}

bool list_is_empty(const LinkedList *list)
{
    return list == NULL || list->size == 0;
}

void list_print(const LinkedList *list)
{
    if (list == NULL || list->head == NULL) {
        printf("(空链表)");
        return;
    }
    printf("[");
    ListNode *current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) printf(" -> ");
        current = current->next;
    }
    printf("]");
}

void list_reverse(LinkedList *list)
{
    if (list == NULL || list->size < 2) return;
    ListNode *prev = NULL;
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head = prev;
}

/* ========================================
 * 栈实现
 * ======================================== */

Stack* stack_create(void)
{
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack != NULL) {
        stack->top = NULL;
        stack->size = 0;
    }
    return stack;
}

void stack_destroy(Stack *stack)
{
    if (stack == NULL) return;
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    free(stack);
}

int stack_push(Stack *stack, int value)
{
    if (stack == NULL) return -1;
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    if (node == NULL) return -1;
    node->data = value;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    return 0;
}

int stack_pop(Stack *stack, int *out_value)
{
    if (stack == NULL || stack->top == NULL) return -1;
    StackNode *old_top = stack->top;
    if (out_value != NULL) *out_value = old_top->data;
    stack->top = old_top->next;
    free(old_top);
    stack->size--;
    return 0;
}

int stack_peek(const Stack *stack, int *out_value)
{
    if (stack == NULL || stack->top == NULL) return -1;
    if (out_value != NULL) *out_value = stack->top->data;
    return 0;
}

size_t stack_size(const Stack *stack)
{
    return stack ? stack->size : 0;
}

bool stack_is_empty(const Stack *stack)
{
    return stack == NULL || stack->size == 0;
}

void stack_print(const Stack *stack)
{
    if (stack == NULL || stack->top == NULL) {
        printf("(空栈)");
        return;
    }
    printf("顶 [");
    StackNode *current = stack->top;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) printf(", ");
        current = current->next;
    }
    printf("] 底");
}

/* ========================================
 * 队列实现
 * ======================================== */

Queue* queue_create(void)
{
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue != NULL) {
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
    }
    return queue;
}

void queue_destroy(Queue *queue)
{
    if (queue == NULL) return;
    QueueNode *current = queue->front;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

int queue_enqueue(Queue *queue, int value)
{
    if (queue == NULL) return -1;
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if (node == NULL) return -1;
    node->data = value;
    node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return 0;
}

int queue_dequeue(Queue *queue, int *out_value)
{
    if (queue == NULL || queue->front == NULL) return -1;
    QueueNode *old_front = queue->front;
    if (out_value != NULL) *out_value = old_front->data;
    queue->front = old_front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(old_front);
    queue->size--;
    return 0;
}

int queue_front(const Queue *queue, int *out_value)
{
    if (queue == NULL || queue->front == NULL) return -1;
    if (out_value != NULL) *out_value = queue->front->data;
    return 0;
}

size_t queue_size(const Queue *queue)
{
    return queue ? queue->size : 0;
}

bool queue_is_empty(const Queue *queue)
{
    return queue == NULL || queue->size == 0;
}

void queue_print(const Queue *queue)
{
    if (queue == NULL || queue->front == NULL) {
        printf("(空队列)");
        return;
    }
    printf("前 [");
    QueueNode *current = queue->front;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) printf(" <- ");
        current = current->next;
    }
    printf("] 后");
}
