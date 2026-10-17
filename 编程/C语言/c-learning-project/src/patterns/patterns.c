/**
 * @file patterns.c
 * @brief 设计模式与架构实现
 */

#include "patterns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ========================================
 * 1. 面向对象模拟 - 基类 Shape
 * ======================================== */

void shape_init(Shape *self, int x, int y)
{
    if (self == NULL) return;
    self->vtable = NULL;  /* 基类无虚表，由派生类设置 */
    self->x = x;
    self->y = y;
}

double shape_area(const Shape *self)
{
    if (self == NULL || self->vtable == NULL) return 0.0;
    return self->vtable->area(self);
}

double shape_perimeter(const Shape *self)
{
    if (self == NULL || self->vtable == NULL) return 0.0;
    return self->vtable->perimeter(self);
}

void shape_print(const Shape *self)
{
    if (self == NULL || self->vtable == NULL) return;
    self->vtable->print(self);
}

void shape_destroy(Shape *self)
{
    if (self == NULL || self->vtable == NULL) return;
    self->vtable->destroy(self);
}

/* ========================================
 * 派生类 Rectangle
 * ======================================== */

static double rectangle_area_impl(const Shape *self)
{
    const Rectangle *r = (const Rectangle*)self;  /* 向下转型 */
    return r->width * r->height;
}

static double rectangle_perimeter_impl(const Shape *self)
{
    const Rectangle *r = (const Rectangle*)self;
    return 2 * (r->width + r->height);
}

static void rectangle_print_impl(const Shape *self)
{
    const Rectangle *r = (const Rectangle*)self;
    printf("矩形 [位置=(%d,%d), 宽=%.1f, 高=%.1f, 面积=%.2f, 周长=%.2f]",
           r->base.x, r->base.y, r->width, r->height,
           rectangle_area_impl(self), rectangle_perimeter_impl(self));
}

static void rectangle_destroy_impl(Shape *self)
{
    Rectangle *r = (Rectangle*)self;
    free(r);
}

/* Rectangle的虚表 */
static const ShapeVTable rectangle_vtable = {
    .area      = rectangle_area_impl,
    .perimeter = rectangle_perimeter_impl,
    .print     = rectangle_print_impl,
    .destroy   = rectangle_destroy_impl
};

Rectangle* rectangle_create(int x, int y, double w, double h)
{
    Rectangle *r = (Rectangle*)malloc(sizeof(Rectangle));
    if (r == NULL) return NULL;
    shape_init(&r->base, x, y);
    r->base.vtable = &rectangle_vtable;  /* 设置虚表指针 */
    r->width = w;
    r->height = h;
    return r;
}

void rectangle_destroy(Rectangle *self)
{
    shape_destroy(&self->base);
}

/* ========================================
 * 派生类 Circle
 * ======================================== */

static double circle_area_impl(const Shape *self)
{
    const Circle *c = (const Circle*)self;
    return M_PI * c->radius * c->radius;
}

static double circle_perimeter_impl(const Shape *self)
{
    const Circle *c = (const Circle*)self;
    return 2 * M_PI * c->radius;
}

static void circle_print_impl(const Shape *self)
{
    const Circle *c = (const Circle*)self;
    printf("圆形 [位置=(%d,%d), 半径=%.1f, 面积=%.2f, 周长=%.2f]",
           c->base.x, c->base.y, c->radius,
           circle_area_impl(self), circle_perimeter_impl(self));
}

static void circle_destroy_impl(Shape *self)
{
    Circle *c = (Circle*)self;
    free(c);
}

static const ShapeVTable circle_vtable = {
    .area      = circle_area_impl,
    .perimeter = circle_perimeter_impl,
    .print     = circle_print_impl,
    .destroy   = circle_destroy_impl
};

Circle* circle_create(int x, int y, double radius)
{
    Circle *c = (Circle*)malloc(sizeof(Circle));
    if (c == NULL) return NULL;
    shape_init(&c->base, x, y);
    c->base.vtable = &circle_vtable;
    c->radius = radius;
    return c;
}

void circle_destroy(Circle *self)
{
    shape_destroy(&self->base);
}

/* 多态示例函数 */
void print_shape_info(const Shape *shape)
{
    shape_print(shape);
    printf("\n");
}

/* ========================================
 * 2. 单例模式
 * ======================================== */

AppConfig* app_config_instance(void)
{
    /* 静态局部变量 - 只初始化一次（C11及以后线程安全）*/
    static AppConfig instance = {
        .instance_id = 1,
        .app_name = "C-Learning-App",
        .version_major = 1,
        .version_minor = 0
    };
    return &instance;
}

void app_config_set_name(AppConfig *cfg, const char *name)
{
    if (cfg == NULL || name == NULL) return;
    strncpy(cfg->app_name, name, sizeof(cfg->app_name) - 1);
    cfg->app_name[sizeof(cfg->app_name) - 1] = '\0';
}

/* ========================================
 * 3. 观察者模式
 * ======================================== */

void subject_init(Subject *sub)
{
    if (sub == NULL) return;
    sub->count = 0;
}

int subject_attach(Subject *sub, ObserverCallback cb, void *ctx)
{
    if (sub == NULL || cb == NULL) return -1;
    if (sub->count >= MAX_OBSERVERS) return -1;
    sub->observers[sub->count].callback = cb;
    sub->observers[sub->count].context = ctx;
    sub->count++;
    return 0;
}

void subject_detach(Subject *sub, ObserverCallback cb)
{
    if (sub == NULL || cb == NULL) return;
    for (int i = 0; i < sub->count; i++) {
        if (sub->observers[i].callback == cb) {
            /* 用最后一个元素填补空位 */
            sub->observers[i] = sub->observers[sub->count - 1];
            sub->count--;
            return;
        }
    }
}

void subject_notify(Subject *sub, const char *event, void *data)
{
    if (sub == NULL) return;
    for (int i = 0; i < sub->count; i++) {
        sub->observers[i].callback(sub->observers[i].context, event, data);
    }
}

/* ========================================
 * 4. 策略模式 - 排序算法
 * ======================================== */

/* 冒泡排序 */
int sort_bubble(int *arr, size_t size)
{
    if (arr == NULL || size < 2) return 0;
    for (size_t i = 0; i < size - 1; i++) {
        int swapped = 0;
        for (size_t j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;  /* 优化：无交换则已有序 */
    }
    return 0;
}

/* 选择排序 */
int sort_selection(int *arr, size_t size)
{
    if (arr == NULL || size < 2) return 0;
    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
    return 0;
}

/* 插入排序 */
int sort_insertion(int *arr, size_t size)
{
    if (arr == NULL || size < 2) return 0;
    for (size_t i = 1; i < size; i++) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
    return 0;
}

void sorter_init(Sorter *sorter, SortStrategy strategy, const char *name)
{
    if (sorter == NULL) return;
    sorter->strategy = strategy;
    sorter->name = name;
}

void sorter_set_strategy(Sorter *sorter, SortStrategy strategy, const char *name)
{
    if (sorter == NULL) return;
    sorter->strategy = strategy;
    sorter->name = name;
}

int sorter_sort(Sorter *sorter, int *arr, size_t size)
{
    if (sorter == NULL || sorter->strategy == NULL) return -1;
    return sorter->strategy(arr, size);
}

/* ========================================
 * 5. 简单工厂模式
 * ======================================== */

Shape* shape_factory_create(ShapeType type, int x, int y, double p1, double p2)
{
    switch (type) {
        case SHAPE_TYPE_RECTANGLE:
            return &rectangle_create(x, y, p1, p2)->base;
        case SHAPE_TYPE_CIRCLE:
            return &circle_create(x, y, p1)->base;
        default:
            return NULL;
    }
}

/* ========================================
 * 6. 有限状态机（FSM）
 * ======================================== */

void fsm_init(StateMachine *fsm)
{
    if (fsm == NULL) return;
    fsm->current_state = FSM_STATE_OFF;
    fsm->transition_count = 0;
}

int fsm_handle_event(StateMachine *fsm, FsmEvent event)
{
    if (fsm == NULL) return -1;

    FsmState old_state = fsm->current_state;

    switch (fsm->current_state) {
        case FSM_STATE_OFF:
            if (event == FSM_EVENT_POWER_ON) {
                fsm->current_state = FSM_STATE_ON;
            }
            break;

        case FSM_STATE_ON:
            if (event == FSM_EVENT_POWER_OFF) {
                fsm->current_state = FSM_STATE_OFF;
            } else if (event == FSM_EVENT_STANDBY) {
                fsm->current_state = FSM_STATE_STANDBY;
            } else if (event == FSM_EVENT_ERROR_OCCURRED) {
                fsm->current_state = FSM_STATE_ERROR;
            }
            break;

        case FSM_STATE_STANDBY:
            if (event == FSM_EVENT_WAKE_UP) {
                fsm->current_state = FSM_STATE_ON;
            } else if (event == FSM_EVENT_POWER_OFF) {
                fsm->current_state = FSM_STATE_OFF;
            }
            break;

        case FSM_STATE_ERROR:
            if (event == FSM_EVENT_RESET) {
                fsm->current_state = FSM_STATE_OFF;
            }
            break;
    }

    if (fsm->current_state != old_state) {
        fsm->transition_count++;
        return 1;  /* 状态已改变 */
    }
    return 0;  /* 状态未改变 */
}

const char* fsm_state_name(FsmState state)
{
    switch (state) {
        case FSM_STATE_OFF:     return "OFF";
        case FSM_STATE_ON:      return "ON";
        case FSM_STATE_STANDBY: return "STANDBY";
        case FSM_STATE_ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

const char* fsm_event_name(FsmEvent event)
{
    switch (event) {
        case FSM_EVENT_POWER_ON:      return "POWER_ON";
        case FSM_EVENT_POWER_OFF:     return "POWER_OFF";
        case FSM_EVENT_STANDBY:       return "STANDBY";
        case FSM_EVENT_WAKE_UP:       return "WAKE_UP";
        case FSM_EVENT_ERROR_OCCURRED: return "ERROR";
        case FSM_EVENT_RESET:         return "RESET";
        default:                      return "UNKNOWN";
    }
}

/* ========================================
 * 综合演示
 * ======================================== */

/* 观察者回调示例 */
static void on_event_log(void *ctx, const char *event, void *data)
{
    (void)ctx;
    printf("    [日志观察者] 事件: %s, 数据: %s\n", event, (char*)data);
}

static void on_event_count(void *ctx, const char *event, void *data)
{
    (void)data;
    int *count = (int*)ctx;
    (*count)++;
    printf("    [计数观察者] 事件 '%s' 是第 %d 个事件\n", event, *count);
}

void demo_design_patterns(void)
{
    printf("========== 设计模式与架构 ==========\n");

    /* ---- 1. OOP + 多态 ---- */
    printf("\n  --- OOP与多态 ---\n");
    Shape *shapes[3];
    shapes[0] = &rectangle_create(0, 0, 5.0, 3.0)->base;
    shapes[1] = &circle_create(10, 10, 4.0)->base;
    shapes[2] = &rectangle_create(20, 5, 2.0, 8.0)->base;

    for (int i = 0; i < 3; i++) {
        printf("  形状 %d: ", i + 1);
        print_shape_info(shapes[i]);
    }
    /* 释放 */
    for (int i = 0; i < 3; i++) {
        shape_destroy(shapes[i]);
    }

    /* ---- 2. 单例模式 ---- */
    printf("\n  --- 单例模式 ---\n");
    AppConfig *cfg1 = app_config_instance();
    AppConfig *cfg2 = app_config_instance();
    printf("  两个指针相同: %s (%p == %p)\n",
           cfg1 == cfg2 ? "是" : "否", (void*)cfg1, (void*)cfg2);
    printf("  应用名: %s, 版本: %d.%d\n",
           cfg1->app_name, cfg1->version_major, cfg1->version_minor);

    /* ---- 3. 观察者模式 ---- */
    printf("\n  --- 观察者模式 ---\n");
    Subject subj;
    subject_init(&subj);
    int event_count = 0;
    subject_attach(&subj, on_event_log, NULL);
    subject_attach(&subj, on_event_count, &event_count);

    printf("  发送 'data_update' 事件:\n");
    subject_notify(&subj, "data_update", "温度=25度");
    printf("  发送 'user_login' 事件:\n");
    subject_notify(&subj, "user_login", "用户=Alice");

    /* ---- 4. 策略模式 ---- */
    printf("\n  --- 策略模式（排序）---\n");
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int arr3[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = 7;

    Sorter sorter;
    sorter_init(&sorter, sort_bubble, "冒泡排序");
    sorter_sort(&sorter, arr1, n);
    printf("  %s: ", sorter.name);
    for (size_t i = 0; i < n; i++) printf("%d ", arr1[i]);
    printf("\n");

    sorter_set_strategy(&sorter, sort_selection, "选择排序");
    sorter_sort(&sorter, arr2, n);
    printf("  %s: ", sorter.name);
    for (size_t i = 0; i < n; i++) printf("%d ", arr2[i]);
    printf("\n");

    sorter_set_strategy(&sorter, sort_insertion, "插入排序");
    sorter_sort(&sorter, arr3, n);
    printf("  %s: ", sorter.name);
    for (size_t i = 0; i < n; i++) printf("%d ", arr3[i]);
    printf("\n");

    /* ---- 5. 简单工厂 ---- */
    printf("\n  --- 简单工厂 ---\n");
    Shape *s = shape_factory_create(SHAPE_TYPE_CIRCLE, 5, 5, 3.0, 0);
    printf("  工厂创建: ");
    print_shape_info(s);
    shape_destroy(s);

    /* ---- 6. 状态机 ---- */
    printf("\n  --- 有限状态机 ---\n");
    StateMachine fsm;
    fsm_init(&fsm);
    printf("  初始状态: %s\n", fsm_state_name(fsm.current_state));

    FsmEvent events[] = {
        FSM_EVENT_POWER_ON,
        FSM_EVENT_STANDBY,
        FSM_EVENT_WAKE_UP,
        FSM_EVENT_ERROR_OCCURRED,
        FSM_EVENT_POWER_OFF,   /* 错误状态下无效 */
        FSM_EVENT_RESET,
        FSM_EVENT_POWER_ON
    };
    for (size_t i = 0; i < sizeof(events)/sizeof(events[0]); i++) {
        int changed = fsm_handle_event(&fsm, events[i]);
        printf("  事件 %-12s -> %s  %s\n",
               fsm_event_name(events[i]),
               fsm_state_name(fsm.current_state),
               changed ? "(状态变更)" : "(无变化)");
    }
    printf("  总状态转换次数: %d\n", fsm.transition_count);

    printf("\n");
}
