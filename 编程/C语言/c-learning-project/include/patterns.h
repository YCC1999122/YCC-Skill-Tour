/**
 * @file patterns.h
 * @brief 设计模式与架构头文件
 *
 * 包含：面向对象模拟、单例模式、观察者模式、策略模式、工厂模式、
 *       状态机、回调机制、模块化架构示例
 */

#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdbool.h>
#include <stddef.h>

/* ========================================
 * 面向对象模拟：基类与继承
 * ======================================== */

/* 基类：Shape（形状）*/
typedef struct Shape Shape;
typedef struct ShapeVTable ShapeVTable;

/* 虚函数表 - 模拟C++的vtable */
struct ShapeVTable {
    double (*area)(const Shape *self);       /* 计算面积 */
    double (*perimeter)(const Shape *self);  /* 计算周长 */
    void   (*print)(const Shape *self);      /* 打印信息 */
    void   (*destroy)(Shape *self);          /* 析构函数 */
};

/* 基类结构体 */
struct Shape {
    const ShapeVTable *vtable;  /* 虚表指针 */
    int x;  /* x坐标 */
    int y;  /* y坐标 */
};

/* 基类方法 */
void   shape_init(Shape *self, int x, int y);
double shape_area(const Shape *self);
double shape_perimeter(const Shape *self);
void   shape_print(const Shape *self);
void   shape_destroy(Shape *self);

/* 派生类：Rectangle（矩形）*/
typedef struct {
    Shape  base;    /* 基类子对象（放在第一个成员，实现指针转换）*/
    double width;
    double height;
} Rectangle;

Rectangle* rectangle_create(int x, int y, double w, double h);
void       rectangle_destroy(Rectangle *self);

/* 派生类：Circle（圆形）*/
typedef struct {
    Shape   base;
    double  radius;
} Circle;

Circle* circle_create(int x, int y, double radius);
void    circle_destroy(Circle *self);

/* 多态示例 */
void print_shape_info(const Shape *shape);

/* ========================================
 * 单例模式
 * ======================================== */
typedef struct {
    int    instance_id;
    char   app_name[64];
    int    version_major;
    int    version_minor;
} AppConfig;

AppConfig* app_config_instance(void);  /* 获取单例 */
void       app_config_set_name(AppConfig *cfg, const char *name);

/* ========================================
 * 观察者模式
 * ======================================== */
#define MAX_OBSERVERS 10

typedef void (*ObserverCallback)(void *context, const char *event, void *data);

typedef struct {
    ObserverCallback callback;
    void            *context;
} Observer;

typedef struct {
    Observer observers[MAX_OBSERVERS];
    int      count;
} Subject;

void subject_init(Subject *sub);
int  subject_attach(Subject *sub, ObserverCallback cb, void *ctx);
void subject_detach(Subject *sub, ObserverCallback cb);
void subject_notify(Subject *sub, const char *event, void *data);

/* ========================================
 * 策略模式
 * ======================================== */
typedef int (*SortStrategy)(int *arr, size_t size);

typedef struct {
    SortStrategy strategy;
    const char  *name;
} Sorter;

/* 具体策略 */
int sort_bubble(int *arr, size_t size);
int sort_selection(int *arr, size_t size);
int sort_insertion(int *arr, size_t size);

/* 策略上下文 */
void    sorter_init(Sorter *sorter, SortStrategy strategy, const char *name);
void    sorter_set_strategy(Sorter *sorter, SortStrategy strategy, const char *name);
int     sorter_sort(Sorter *sorter, int *arr, size_t size);

/* ========================================
 * 简单工厂模式
 * ======================================== */
typedef enum {
    SHAPE_TYPE_RECTANGLE,
    SHAPE_TYPE_CIRCLE
} ShapeType;

Shape* shape_factory_create(ShapeType type, int x, int y, double p1, double p2);

/* ========================================
 * 有限状态机（FSM）
 * ======================================== */
typedef enum {
    FSM_STATE_OFF,
    FSM_STATE_ON,
    FSM_STATE_STANDBY,
    FSM_STATE_ERROR
} FsmState;

typedef enum {
    FSM_EVENT_POWER_ON,
    FSM_EVENT_POWER_OFF,
    FSM_EVENT_STANDBY,
    FSM_EVENT_WAKE_UP,
    FSM_EVENT_ERROR_OCCURRED,
    FSM_EVENT_RESET
} FsmEvent;

typedef struct {
    FsmState current_state;
    int      transition_count;
} StateMachine;

void  fsm_init(StateMachine *fsm);
int   fsm_handle_event(StateMachine *fsm, FsmEvent event);
const char* fsm_state_name(FsmState state);
const char* fsm_event_name(FsmEvent event);

/* ========================================
 * 模块演示入口
 * ======================================== */
void demo_design_patterns(void);

#endif /* PATTERNS_H */
