/**
 * @file basics.c
 * @brief C语言基础语法实现
 *
 * 包含：数据类型、运算符、控制流、函数、存储类、类型转换、位运算
 */

#include "basics.h"
#include <stdarg.h>     /* 可变参数 */
#include <limits.h>     /* 类型极限值 */
#include <float.h>      /* 浮点数极限值 */
#include <stdbool.h>    /* C99 bool类型 */

/* ========================================
 * 1. 基础数据类型演示
 * ======================================== */
void demo_basic_types(void)
{
    printf("========== 1. 基础数据类型 ==========\n");

    /* 字符型 */
    char c = 'A';
    unsigned char uc = 255;
    printf("  char:        '%c' (size=%zu byte, value=%d)\n", c, sizeof(char), c);
    printf("  unsigned char: %u (size=%zu byte)\n", uc, sizeof(unsigned char));

    /* 整型 */
    short s = 32767;
    int i = 2147483647;
    long l = 9223372036854775807L;
    long long ll = 9223372036854775807LL;
    unsigned int ui = 4294967295U;

    printf("  short:       %hd (size=%zu bytes, max=%d)\n", s, sizeof(short), SHRT_MAX);
    printf("  int:         %d (size=%zu bytes, max=%d)\n", i, sizeof(int), INT_MAX);
    printf("  long:        %ld (size=%zu bytes)\n", l, sizeof(long));
    printf("  long long:   %lld (size=%zu bytes)\n", ll, sizeof(long long));
    printf("  unsigned int: %u (size=%zu bytes, max=%u)\n", ui, sizeof(unsigned int), UINT_MAX);

    /* 浮点型 */
    float f = 3.14159f;
    double d = 2.718281828459045;
    long double ld = 1.4142135623730950488L;
    printf("  float:       %.6f (size=%zu bytes)\n", f, sizeof(float));
    printf("  double:      %.15f (size=%zu bytes)\n", d, sizeof(double));
    printf("  long double: %.18Lf (size=%zu bytes)\n", ld, sizeof(long double));

    /* C99 bool */
    bool b = true;
    printf("  bool:        %s (size=%zu byte)\n", b ? "true" : "false", sizeof(bool));

    /* 枚举类型 */
    enum Color { RED, GREEN, BLUE = 10, YELLOW };
    enum Color color = GREEN;
    printf("  enum Color:  RED=%d, GREEN=%d, BLUE=%d, YELLOW=%d\n",
           RED, GREEN, BLUE, YELLOW);
    printf("  color=GREEN: %d\n", color);

    /* const 修饰符 */
    const int CONST_VAL = 100;
    printf("  const int:   %d (不可修改)\n", CONST_VAL);

    printf("\n");
}

/* ========================================
 * 2. 运算符演示
 * ======================================== */
void demo_operators(void)
{
    printf("========== 2. 运算符 ==========\n");

    int a = 10, b = 3;

    /* 算术运算符 */
    printf("  算术: a=%d, b=%d\n", a, b);
    printf("    a + b = %d\n", a + b);
    printf("    a - b = %d\n", a - b);
    printf("    a * b = %d\n", a * b);
    printf("    a / b = %d\n", a / b);       /* 整数除法 */
    printf("    a %% b = %d\n", a % b);      /* 取模 */

    /* 自增自减 */
    int x = 5;
    printf("  自增自减: x=%d\n", x);
    printf("    x++ = %d (后x=%d)\n", x++, x);
    printf("    ++x = %d (后x=%d)\n", ++x, x);
    printf("    x-- = %d (后x=%d)\n", x--, x);
    printf("    --x = %d (后x=%d)\n", --x, x);

    /* 关系运算符 */
    printf("  关系: %d > %d = %d, %d == %d = %d\n",
           a, b, a > b, a, b, a == b);

    /* 逻辑运算符 */
    int t = 1, f = 0;
    printf("  逻辑: t=%d, f=%d, t&&f=%d, t||f=%d, !t=%d\n",
           t, f, t && f, t || f, !t);

    /* 赋值运算符 */
    int n = 10;
    n += 5;  printf("  n += 5 -> %d\n", n);
    n -= 3;  printf("  n -= 3 -> %d\n", n);
    n *= 2;  printf("  n *= 2 -> %d\n", n);
    n /= 4;  printf("  n /= 4 -> %d\n", n);
    n %= 5;  printf("  n %%= 5 -> %d\n", n);

    /* 三目运算符 */
    int max_val = (a > b) ? a : b;
    printf("  三目: max(%d,%d) = %d\n", a, b, max_val);

    /* sizeof 运算符 */
    printf("  sizeof(int)=%zu, sizeof(double)=%zu\n", sizeof(int), sizeof(double));

    printf("\n");
}

/* ========================================
 * 3. 控制流演示
 * ======================================== */
void demo_control_flow(void)
{
    printf("========== 3. 控制流 ==========\n");

    /* if-else */
    int score = 85;
    printf("  if-else (score=%d): ", score);
    if (score >= 90) {
        printf("优秀\n");
    } else if (score >= 80) {
        printf("良好\n");
    } else if (score >= 60) {
        printf("及格\n");
    } else {
        printf("不及格\n");
    }

    /* switch-case */
    int day = 3;
    printf("  switch (day=%d): ", day);
    switch (day) {
        case 1:  printf("周一\n"); break;
        case 2:  printf("周二\n"); break;
        case 3:  printf("周三\n"); break;
        case 4:  printf("周四\n"); break;
        case 5:  printf("周五\n"); break;
        default: printf("周末\n"); break;
    }

    /* for 循环 */
    printf("  for 循环(1~5): ");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    /* while 循环 */
    printf("  while 循环(5~1): ");
    int w = 5;
    while (w > 0) {
        printf("%d ", w);
        w--;
    }
    printf("\n");

    /* do-while 循环 */
    printf("  do-while 循环: ");
    int dw = 0;
    do {
        printf("%d ", dw);
        dw++;
    } while (dw < 3);
    printf("\n");

    /* break & continue */
    printf("  break/continue: ");
    for (int i = 0; i < 10; i++) {
        if (i == 7) break;       /* 跳出循环 */
        if (i % 2 == 0) continue; /* 跳过偶数 */
        printf("%d ", i);
    }
    printf("\n");

    /* goto (谨慎使用，常用于错误处理) */
    printf("  goto 错误处理示例: ");
    int *p = NULL;
    if (p == NULL) {
        goto error;
    }
    printf("正常\n");
    return;
error:
    printf("空指针错误（goto跳转）\n");

    printf("\n");
}

/* ========================================
 * 4. 函数演示
 * ======================================== */

/**
 * @brief 阶乘函数（递归实现）
 * @param n 非负整数
 * @return n的阶乘，n<0时返回-1
 */
int factorial(int n)
{
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);  /* 递归调用 */
}

/**
 * @brief 应用函数指针
 * @param func 函数指针，指向接收两个int返回int的函数
 * @param a 第一个参数
 * @param b 第二个参数
 * @return 函数执行结果
 */
int apply_math_func(MathFunc func, int a, int b)
{
    return func(a, b);
}

/* 辅助函数：加法 */
static int add(int a, int b) { return a + b; }
/* 辅助函数：乘法 */
static int multiply(int a, int b) { return a * b; }

/**
 * @brief 可变参数函数 - 求和
 * @param count 参数个数
 * @param ... 可变数量的int参数
 * @return 总和
 */
int sum_multiple(int count, ...)
{
    va_list ap;
    int sum = 0;
    va_start(ap, count);
    for (int i = 0; i < count; i++) {
        sum += va_arg(ap, int);
    }
    va_end(ap);
    return sum;
}

/* ========================================
 * 5. 存储类演示
 * ======================================== */

/* 静态全局变量 - 仅本文件可见 */
static int s_file_static = 100;

/* 全局变量 - 其他文件可通过extern访问 */
int g_external_var = 200;

/**
 * @brief 获取静态计数器值（每次调用自增）
 * @return 当前计数值
 *
 * 静态局部变量：函数调用结束后不释放，保留上次的值
 */
int get_static_counter(void)
{
    static int counter = 0;  /* 静态局部变量，只初始化一次 */
    counter++;
    return counter;
}

void demo_storage_classes(void)
{
    printf("========== 5. 存储类 ==========\n");

    /* auto - 自动存储（默认，可省略） */
    auto int auto_var = 10;
    printf("  auto:         %d (函数内自动分配/释放)\n", auto_var);

    /* static - 静态局部变量 */
    printf("  static counter: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", get_static_counter());
    }
    printf("(每次调用自增，保留状态)\n");

    /* static 全局变量 */
    printf("  static全局:   %d (仅本文件可见)\n", s_file_static);

    /* extern - 外部变量声明 */
    extern int g_external_var;
    printf("  extern:       %d (跨文件共享)\n", g_external_var);

    /* register - 寄存器变量（建议编译器放寄存器，现代编译器自动优化） */
    register int reg_var = 0;
    for (int i = 0; i < 100; i++) {
        reg_var += i;
    }
    printf("  register:     %d (建议存寄存器)\n", reg_var);

    /* volatile - 易变变量（告诉编译器不要优化，每次从内存读取）
       常用于硬件寄存器、多线程共享变量、中断服务程序 */
    volatile int vol_var = 0;
    printf("  volatile:     %d (禁止优化)\n", vol_var);

    printf("\n");
}

/* ========================================
 * 6. 类型转换演示
 * ======================================== */
void demo_type_casting(void)
{
    printf("========== 6. 类型转换 ==========\n");

    /* 隐式转换 */
    int i = 10;
    double d = i;  /* int -> double，自动提升 */
    printf("  隐式转换: int %d -> double %f\n", i, d);

    /* 显式转换 (强制类型转换) */
    double pi = 3.14159;
    int int_pi = (int)pi;
    printf("  显式转换: double %.5f -> int %d\n", pi, int_pi);

    /* 整数提升 */
    char c = 'A';
    int result = c + 1;  /* char 提升为 int 再运算 */
    printf("  整数提升: char '%c' + 1 = int %d ('%c')\n", c, result, result);

    /* 指针类型转换 */
    int num = 0x12345678;
    unsigned char *byte_ptr = (unsigned char *)&num;
    printf("  指针转换: 0x%08X 的字节: ", num);
    for (size_t k = 0; k < sizeof(int); k++) {
        printf("%02X ", byte_ptr[k]);
    }
    printf("(%s端序)\n", byte_ptr[0] == 0x78 ? "小" : "大");

    printf("\n");
}

/* ========================================
 * 7. 位运算演示
 * ======================================== */
void demo_bitwise_operations(void)
{
    printf("========== 7. 位运算 ==========\n");

    unsigned char a = 0b11001010;  /* 202 */
    unsigned char b = 0b10101100;  /* 172 */

    printf("  a = 11001010 (202)\n");
    printf("  b = 10101100 (172)\n");
    printf("  a & b  = %03d (按位与: %08b)\n", a & b, a & b);
    printf("  a | b  = %03d (按位或: %08b)\n", a | b, a | b);
    printf("  a ^ b  = %03d (按位异或: %08b)\n", a ^ b, a ^ b);
    printf("  ~a     = %03d (按位取反: %08b)\n", (unsigned char)~a, (unsigned char)~a);
    printf("  a << 2 = %03d (左移2位: %08b)\n", a << 2, a << 2);
    printf("  a >> 2 = %03d (右移2位: %08b)\n", a >> 2, a >> 2);

    /* 常用位操作技巧 */
    unsigned int flags = 0;
    printf("\n  位操作技巧:\n");
    flags |= (1 << 2);   /* 设置第2位 */
    printf("    设置第2位:    %08X\n", flags);
    flags &= ~(1 << 2);  /* 清除第2位 */
    printf("    清除第2位:    %08X\n", flags);
    flags ^= (1 << 3);   /* 翻转第3位 */
    printf("    翻转第3位:    %08X\n", flags);
    int bit = (flags >> 3) & 1;  /* 读取第3位 */
    printf("    读取第3位:    %d\n", bit);

    printf("\n");
}
