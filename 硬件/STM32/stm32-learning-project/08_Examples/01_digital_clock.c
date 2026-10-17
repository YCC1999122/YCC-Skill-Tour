/**
 ******************************************************************************
 * @file    01_digital_clock.c
 * @brief   综合实例：数字时钟
 *          使用RTC实时时钟 + LCD1602显示 + 按键调整时间
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *
 * ┌──────────────┐                  ┌──────────────────┐
 * │  STM32F103   │                  │   LCD1602        │
 * │              │                  │  (1602字符液晶)  │
 * │  PB0  ───────┼──────────────────┤  RS              │
 * │  PB1  ───────┼──────────────────┤  RW              │
 * │  PB2  ───────┼──────────────────┤  E  (使能)       │
 * │              │                  │                  │
 * │  PB4  ───────┼──────────────────┤  D4              │
 * │  PB5  ───────┼──────────────────┤  D5              │
 * │  PB6  ───────┼──────────────────┤  D6              │
 * │  PB7  ───────┼──────────────────┤  D7              │
 * │              │                  │                  │
 * │  PA0  ───────┼─────┬────────────┤  K1 (设置键)     │
 * │  PA1  ───────┼─────┤────────────┤  K2 (加键)       │
 * │  PA2  ───────┼─────┤────────────┤  K3 (减键)       │
 * │              │     │            │                  │
 * │  3.3V ───────┼──10kΩ×3─────────┤  VCC             │
 * │              │                  │  A  (背光+)      │
 * │              │              220Ω│                  │
 * │              │  ┌───────────────┤  K (背光-)       │
 * │              │  │               │                  │
 * │  GND  ───────┴──┴───────────────┴── VSS / V0 / D0-D3
 * └──────────────┘                  └──────────────────┘
 *
 *   RTC晶振：
 *     PC14 -> 32.768kHz晶振 -> PC15
 *     （STM32F103的RTC外部晶振，最小系统板一般已焊接）
 *
 *   注意：
 *     1. LCD1602使用4位数据总线模式（只用D4~D7，节省IO口）
 *     2. D0~D3可以悬空不接
 *     3. V0接GND时对比度最高，可接10k电位器调节
 *     4. 按键一端接GPIO，另一端接GND（低电平有效，使用内部上拉）
 *     5. LCD背光A接VCC，K接GND（可串联电阻限流）
 *
 *   三个按键功能：
 *     K1 (PA0)：设置键 - 切换设置项（时→分→秒→退出）
 *     K2 (PA1)：加键   - 当前设置项数值+1
 *     K3 (PA2)：减键   - 当前设置项数值-1
 *
 * 学习重点：
 *   1. RTC实时时钟的配置和使用（BCD码格式）
 *   2. LCD1602的驱动方法（4位模式、指令/数据写入）
 *   3. BCD码与十进制的转换
 *   4. 按键状态机的设计（菜单式调整）
 *   5. 综合项目的模块化编程思想
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* ---------- LCD1602引脚定义（PB口） ---------- */
#define LCD_RS_PORT             GPIOB
#define LCD_RS_PIN              GPIO_PIN_0
#define LCD_RW_PORT             GPIOB
#define LCD_RW_PIN              GPIO_PIN_1
#define LCD_EN_PORT             GPIOB
#define LCD_EN_PIN              GPIO_PIN_2

/* 数据口（4位模式：D4~D7 -> PB4~PB7） */
#define LCD_DATA_PORT           GPIOB
#define LCD_D4_PIN              GPIO_PIN_4
#define LCD_D5_PIN              GPIO_PIN_5
#define LCD_D6_PIN              GPIO_PIN_6
#define LCD_D7_PIN              GPIO_PIN_7

/* ---------- LCD1602指令定义 ---------- */
#define LCD_CMD_CLEAR           0x01    /* 清屏 */
#define LCD_CMD_HOME            0x02    /* 光标归位 */
#define LCD_CMD_ENTRY_MODE      0x06    /* 输入模式：光标右移，画面不动 */
#define LCD_CMD_DISPLAY_ON      0x0C    /* 显示开，光标关，闪烁关 */
#define LCD_CMD_DISPLAY_OFF     0x08    /* 显示关 */
#define LCD_CMD_FUNC_SET        0x28    /* 功能设置：4位数据，2行显示，5x8点阵 */
#define LCD_CMD_CGRAM_SET       0x40    /* 设置CGRAM地址 */
#define LCD_CMD_DDRAM_SET       0x80    /* 设置DDRAM地址 */

/* ---------- 按键定义 ---------- */
#define KEY_PORT                GPIOA
#define KEY_SET_PIN             GPIO_PIN_0    /* 设置键 */
#define KEY_ADD_PIN             GPIO_PIN_1    /* 加键 */
#define KEY_SUB_PIN             GPIO_PIN_2    /* 减键 */

#define KEY_DEBOUNCE_MS         20      /* 消抖时间 */
#define KEY_LONG_PRESS_MS       800     /* 长按触发时间 */
#define KEY_REPEAT_MS           150     /* 长按连发间隔 */

/* ---------- LED引脚 ---------- */
#define LED_GPIO_PORT           GPIOC
#define LED_GPIO_PIN            GPIO_PIN_13

/* ================================================================
 * 类型定义和全局变量
 * ================================================================ */

/* 时间结构体（BCD格式，和RTC寄存器格式一致） */
typedef struct {
  uint8_t hours;     /* 时（BCD码，00~23） */
  uint8_t minutes;   /* 分（BCD码，00~59） */
  uint8_t seconds;   /* 秒（BCD码，00~59） */
} Time_TypeDef;

/* 设置项枚举 */
typedef enum {
  SET_ITEM_NONE = 0,   /* 正常显示模式 */
  SET_ITEM_HOURS,      /* 设置小时 */
  SET_ITEM_MINUTES,    /* 设置分钟 */
  SET_ITEM_SECONDS,    /* 设置秒 */
  SET_ITEM_MAX         /* 设置项数量 */
} SetItem_t;

/* 按键状态枚举 */
typedef enum {
  KEY_RELEASED = 0,
  KEY_PRESSED,
  KEY_LONG_PRESS,
  KEY_REPEAT
} KeyState_t;

/* 全局变量 */
RTC_HandleTypeDef hrtc;                /* RTC句柄 */
Time_TypeDef g_current_time;           /* 当前时间 */
SetItem_t g_set_item = SET_ITEM_NONE;  /* 当前设置项 */
uint8_t g_blink_flag = 0;              /* 设置项闪烁标志 */
uint32_t g_blink_tick = 0;             /* 闪烁计时 */

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
void Error_Handler(void);

/* LCD1602函数声明 */
void LCD_Init(void);
void LCD_WriteCmd(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_ShowString(uint8_t row, uint8_t col, char *str);
void LCD_ShowNum(uint8_t row, uint8_t col, uint8_t num, uint8_t len);

/* 按键函数声明 */
KeyState_t Key_Scan(GPIO_TypeDef *port, uint16_t pin, uint8_t active_level);

/* 时间相关函数声明 */
void RTC_GetTime(Time_TypeDef *time);
void RTC_SetTime(Time_TypeDef *time);
uint8_t BCD_To_Dec(uint8_t bcd);
uint8_t Dec_To_BCD(uint8_t dec);

/* 应用函数 */
void Clock_Display(void);
void Clock_SetMode_Process(void);

/**
 * @brief  主函数：程序入口
 * @retval int
 *
 * 数字时钟功能：
 *   1. RTC实时时钟计时（掉电后由备份电池继续走时）
 *   2. LCD1602显示时间（格式：HH:MM:SS）
 *   3. 三个按键调整时间（设置、加、减）
 *   4. 设置模式下，被调整的数字闪烁
 *   5. LED每秒闪烁一次（秒指示）
 */
int main(void)
{
  /* ========== 第1步：系统初始化 ========== */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_RTC_Init();
  LCD_Init();

  /* 显示欢迎信息 */
  LCD_ShowString(0, 0, "  Digital Clock ");
  LCD_ShowString(1, 0, "   STM32F103   ");
  HAL_Delay(1000);

  /* 清屏，准备显示时间 */
  LCD_WriteCmd(LCD_CMD_CLEAR);
  HAL_Delay(2);

  /* 显示固定字符（冒号） */
  LCD_ShowString(0, 2, ":");
  LCD_ShowString(0, 5, ":");
  LCD_ShowString(1, 0, "  2025-01-01   ");

  uint32_t last_second = 0;  /* 上一次更新显示的秒数 */

  /* ========== 第2步：主循环 ========== */
  while (1)
  {
    /* ---------- 读取RTC时间 ---------- */
    RTC_GetTime(&g_current_time);

    /* ---------- 判断是否进入设置模式 ----------
     * 设置键按下，进入/切换设置模式
     */
    if (Key_Scan(KEY_PORT, KEY_SET_PIN, GPIO_PIN_RESET) == KEY_PRESSED)
    {
      /* 切换到下一个设置项 */
      g_set_item++;
      if (g_set_item >= SET_ITEM_MAX)
      {
        g_set_item = SET_ITEM_NONE;  /* 退出设置模式，保存时间到RTC */
        RTC_SetTime(&g_current_time);
      }
      g_blink_flag = 0;
      g_blink_tick = HAL_GetTick();
    }

    /* ---------- 设置模式处理 ---------- */
    if (g_set_item != SET_ITEM_NONE)
    {
      Clock_SetMode_Process();
    }

    /* ---------- 显示更新 ----------
     * 正常模式：每秒更新一次
     * 设置模式：实时更新（闪烁效果）
     */
    if ((g_set_item != SET_ITEM_NONE) ||
        (BCD_To_Dec(g_current_time.seconds) != last_second))
    {
      last_second = BCD_To_Dec(g_current_time.seconds);
      Clock_Display();

      /* LED随秒闪烁（正常模式下） */
      if (g_set_item == SET_ITEM_NONE)
      {
        if ((last_second % 2) == 0)
          HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
        else
          HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
      }
    }

    /* 小延时，降低CPU占用 */
    HAL_Delay(10);
  }
}

/* ================================================================
 * LCD1602驱动函数（4位模式）
 * ================================================================ */

/**
 * @brief  LCD1602写指令
 * @param  cmd: 指令码
 *
 * 4位模式下，一个8位指令分两次发送：
 *   先发高4位（D7~D4），再发低4位（D3~D0）
 *
 * 写指令操作：
 *   RS=0（指令）, RW=0（写）, E=1→0（下降沿锁存）
 */
void LCD_WriteCmd(uint8_t cmd)
{
  /* RS = 0（指令模式） */
  HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
  /* RW = 0（写模式） */
  HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);

  /* ---------- 发送高4位 ---------- */
  /* 先清空数据位 */
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, (cmd >> 4) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (cmd >> 5) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (cmd >> 6) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (cmd >> 7) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);

  /* 产生E脉冲（高电平脉冲，下降沿锁存） */
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);  /* 至少140ns，用1ms绰绰有余 */
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);

  /* ---------- 发送低4位 ---------- */
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, (cmd >> 0) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (cmd >> 1) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (cmd >> 2) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (cmd >> 3) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
}

/**
 * @brief  LCD1602写数据（显示一个字符）
 * @param  data: 字符的ASCII码
 *
 * 写数据和写指令的区别：
 *   写数据时 RS=1（数据模式）
 *   写指令时 RS=0（指令模式）
 */
void LCD_WriteData(uint8_t data)
{
  /* RS = 1（数据模式） */
  HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);
  /* RW = 0（写模式） */
  HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);

  /* 高4位 */
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, (data >> 4) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (data >> 5) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (data >> 6) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (data >> 7) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);

  /* 低4位 */
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, (data >> 0) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, (data >> 1) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, (data >> 2) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, (data >> 3) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
}

/**
 * @brief  LCD1602初始化（4位模式）
 *
 * LCD1602上电初始化流程（4位模式的特殊初始化序列）：
 *   1. 延时15ms以上（等待电源稳定）
 *   2. 发送0x30（8位模式指令）
 *   3. 延时4.1ms以上
 *   4. 再发0x30
 *   5. 延时100us以上
 *   6. 再发0x30
 *   7. 延时100us以上
 *   8. 发送0x20（切换到4位模式）
 *   9. 之后就可以按4位模式正常操作了
 *
 * 为什么要这么麻烦？
 *   因为LCD上电默认是8位模式，
 *   而我们用的是4位接线（D4~D7），
 *   所以需要一个特殊的序列来切换到4位模式。
 */
void LCD_Init(void)
{
  /* 上电延时，等待LCD内部电路稳定 */
  HAL_Delay(20);

  /* 初始化序列（3次0x30，然后0x20）
   * 注意：这些是特殊的8位指令，用4位接口发送时
   * 只需要发送高4位，不需要发低4位
   * 因为此时LCD还在8位模式，它只看D7~D4
   */

  /* 第1次：0x30 */
  HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);  /* 至少4.1ms */

  /* 第2次：0x30 */
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);  /* 至少100us */

  /* 第3次：0x30 */
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);

  /* 切换到4位模式：0x20（只发高4位=0x2） */
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);

  /* 现在已经是4位模式了，正常发指令 */
  LCD_WriteCmd(LCD_CMD_FUNC_SET);      /* 功能设置：4位，2行，5x8 */
  LCD_WriteCmd(LCD_CMD_DISPLAY_OFF);   /* 显示关 */
  LCD_WriteCmd(LCD_CMD_CLEAR);         /* 清屏 */
  LCD_WriteCmd(LCD_CMD_ENTRY_MODE);    /* 输入模式：右移 */
  LCD_WriteCmd(LCD_CMD_DISPLAY_ON);    /* 显示开，光标关 */
}

/**
 * @brief  设置光标位置
 * @param  row: 行号（0=第一行，1=第二行）
 * @param  col: 列号（0~15）
 *
 * DDRAM地址：
 *   第一行：0x00 ~ 0x0F（共16个字符位置）
 *   第二行：0x40 ~ 0x4F
 *
 * 设置地址指令 = 0x80 | 地址
 */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
  uint8_t addr;

  if (row == 0)
    addr = 0x00 + col;    /* 第一行起始地址0x00 */
  else
    addr = 0x40 + col;    /* 第二行起始地址0x40 */

  LCD_WriteCmd(LCD_CMD_DDRAM_SET | addr);
}

/**
 * @brief  在指定位置显示字符串
 */
void LCD_ShowString(uint8_t row, uint8_t col, char *str)
{
  LCD_SetCursor(row, col);
  while (*str)
  {
    LCD_WriteData(*str++);
  }
}

/**
 * @brief  在指定位置显示数字（BCD格式，显示2位十进制）
 */
void LCD_ShowNum(uint8_t row, uint8_t col, uint8_t num, uint8_t len)
{
  LCD_SetCursor(row, col);
  for (int8_t i = len - 1; i >= 0; i--)
  {
    uint8_t digit = (num >> (i * 4)) & 0x0F;  /* BCD码的每一位 */
    LCD_WriteData('0' + digit);
  }
}

/* ================================================================
 * 按键驱动函数
 * ================================================================ */

/**
 * @brief  单个按键扫描（含消抖、长按、连发）
 * @param  port: GPIO端口
 * @param  pin:  引脚号
 * @param  active_level: 有效电平（GPIO_PIN_SET 或 GPIO_PIN_RESET）
 * @retval 按键状态
 *
 * 支持的功能：
 *   - 软件消抖
 *   - 按下检测（返回一次KEY_PRESSED）
 *   - 长按检测（超过设定时间返回KEY_LONG_PRESS）
 *   - 长按连发（长按后每隔一段时间返回一次KEY_REPEAT）
 */
KeyState_t Key_Scan(GPIO_TypeDef *port, uint16_t pin, uint8_t active_level)
{
  static uint8_t last_level[3] = {1, 1, 1};   /* 上一次电平（按引脚索引存储会更好，这里简化） */
  static uint8_t stable_level[3] = {1, 1, 1}; /* 稳定电平 */
  static uint32_t change_time[3] = {0};       /* 变化时刻 */
  static uint8_t long_press_flag[3] = {0};    /* 长按触发标志 */
  static uint32_t repeat_time[3] = {0};       /* 连发计时 */

  /* 用引脚号作为索引（简化处理，假设不同引脚不会混淆） */
  uint8_t idx = (pin == KEY_SET_PIN) ? 0 : (pin == KEY_ADD_PIN ? 1 : 2);

  KeyState_t ret = KEY_RELEASED;
  uint8_t current_level = HAL_GPIO_ReadPin(port, pin);

  /* 电平变化检测 + 消抖 */
  if (current_level != last_level[idx])
  {
    change_time[idx] = HAL_GetTick();
    last_level[idx] = current_level;
  }
  else if (current_level != stable_level[idx])
  {
    if ((HAL_GetTick() - change_time[idx]) >= KEY_DEBOUNCE_MS)
    {
      stable_level[idx] = current_level;

      if (stable_level[idx] == active_level)
      {
        /* 按键按下 */
        long_press_flag[idx] = 0;
        repeat_time[idx] = HAL_GetTick();
        ret = KEY_PRESSED;
      }
    }
  }

  /* 长按检测（按键处于按下状态时） */
  if (stable_level[idx] == active_level)
  {
    if (!long_press_flag[idx])
    {
      if ((HAL_GetTick() - change_time[idx]) >= KEY_LONG_PRESS_MS)
      {
        long_press_flag[idx] = 1;
        repeat_time[idx] = HAL_GetTick();
        ret = KEY_LONG_PRESS;
      }
    }
    else
    {
      /* 连发 */
      if ((HAL_GetTick() - repeat_time[idx]) >= KEY_REPEAT_MS)
      {
        repeat_time[idx] = HAL_GetTick();
        ret = KEY_REPEAT;
      }
    }
  }

  return ret;
}

/* ================================================================
 * RTC和时间相关函数
 * ================================================================ */

/**
 * @brief  BCD码转十进制
 * @param  bcd: BCD码（0x00 ~ 0x99）
 * @retval 十进制数（0 ~ 99）
 *
 * BCD码（Binary-Coded Decimal）：
 *   用4位二进制数表示1位十进制数。
 *   一个字节（8位）可以表示2位十进制数。
 *
 * 例如：
 *   十进制 25 = BCD 0x25 = 二进制 0010 0101
 *   高4位表示十位（2），低4位表示个位（5）
 *
 * 转换公式：
 *   十进制 = (BCD >> 4) * 10 + (BCD & 0x0F)
 */
uint8_t BCD_To_Dec(uint8_t bcd)
{
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
 * @brief  十进制转BCD码
 * @param  dec: 十进制数（0 ~ 99）
 * @retval BCD码
 *
 * 转换公式：
 *   BCD = (十进制 / 10) << 4 | (十进制 % 10)
 */
uint8_t Dec_To_BCD(uint8_t dec)
{
  return ((dec / 10) << 4) | (dec % 10);
}

/**
 * @brief  从RTC读取当前时间
 * @param  time: 时间结构体指针（输出）
 *
 * HAL库RTC时间读取注意事项：
 *   - STM32F1的HAL库RTC比较特殊，RTC计数的是秒数
 *   - 不是像F4/F7那样直接有时分秒寄存器
 *   - 需要把秒数换算成天、时、分、秒
 *   - 这里使用简化的方式：用RTC的计数器表示当天的秒数
 *
 * 注意：这是简化实现，实际项目中可以用RTC日历功能
 *       或者用秒数+基准时间计算
 */
void RTC_GetTime(Time_TypeDef *time)
{
  /* 读取RTC计数器值（秒数） */
  uint32_t total_seconds = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);

  /* 这里简化处理：使用HAL_RTC_GetTime函数
   * F1系列HAL库的RTC实际上是32位计数器
   * 我们把它配置为每秒加1，表示当天的秒数
   */
  RTC_TimeTypeDef sTime;
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

  time->hours = sTime.Hours;
  time->minutes = sTime.Minutes;
  time->seconds = sTime.Seconds;
}

/**
 * @brief  设置RTC时间
 * @param  time: 时间结构体指针（输入）
 */
void RTC_SetTime(Time_TypeDef *time)
{
  RTC_TimeTypeDef sTime = {0};

  sTime.Hours = time->hours;
  sTime.Minutes = time->minutes;
  sTime.Seconds = time->seconds;

  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
}

/* ================================================================
 * 应用层函数（时钟显示和设置逻辑）
 * ================================================================ */

/**
 * @brief  显示时间
 *
 * 显示格式（第一行）：
 *   HH:MM:SS
 *   0123456789...
 *
 * 设置模式下，当前设置项闪烁
 */
void Clock_Display(void)
{
  /* 时（位置0-1） */
  if (g_set_item == SET_ITEM_HOURS && g_blink_flag)
  {
    LCD_ShowString(0, 0, "  ");  /* 闪烁时显示空格 */
  }
  else
  {
    LCD_ShowNum(0, 0, g_current_time.hours, 2);
  }

  /* 分（位置3-4） */
  if (g_set_item == SET_ITEM_MINUTES && g_blink_flag)
  {
    LCD_ShowString(0, 3, "  ");
  }
  else
  {
    LCD_ShowNum(0, 3, g_current_time.minutes, 2);
  }

  /* 秒（位置6-7） */
  if (g_set_item == SET_ITEM_SECONDS && g_blink_flag)
  {
    LCD_ShowString(0, 6, "  ");
  }
  else
  {
    LCD_ShowNum(0, 6, g_current_time.seconds, 2);
  }
}

/**
 * @brief  设置模式处理
 *
 * 处理加减按键，调整当前设置项的数值
 * 同时处理闪烁效果
 */
void Clock_SetMode_Process(void)
{
  KeyState_t add_state = Key_Scan(KEY_PORT, KEY_ADD_PIN, GPIO_PIN_RESET);
  KeyState_t sub_state = Key_Scan(KEY_PORT, KEY_SUB_PIN, GPIO_PIN_RESET);

  /* ---------- 闪烁控制（500ms闪烁一次） ---------- */
  if ((HAL_GetTick() - g_blink_tick) >= 500)
  {
    g_blink_tick = HAL_GetTick();
    g_blink_flag = !g_blink_flag;
  }

  /* ---------- 加键处理 ----------
   * 按下或连发时，数值+1
   */
  if (add_state == KEY_PRESSED || add_state == KEY_REPEAT)
  {
    uint8_t dec_val;

    switch (g_set_item)
    {
      case SET_ITEM_HOURS:
        dec_val = BCD_To_Dec(g_current_time.hours);
        dec_val = (dec_val + 1) % 24;   /* 0~23循环 */
        g_current_time.hours = Dec_To_BCD(dec_val);
        break;

      case SET_ITEM_MINUTES:
        dec_val = BCD_To_Dec(g_current_time.minutes);
        dec_val = (dec_val + 1) % 60;   /* 0~59循环 */
        g_current_time.minutes = Dec_To_BCD(dec_val);
        break;

      case SET_ITEM_SECONDS:
        dec_val = BCD_To_Dec(g_current_time.seconds);
        dec_val = (dec_val + 1) % 60;   /* 0~59循环 */
        g_current_time.seconds = Dec_To_BCD(dec_val);
        break;

      default:
        break;
    }
  }

  /* ---------- 减键处理 ---------- */
  if (sub_state == KEY_PRESSED || sub_state == KEY_REPEAT)
  {
    uint8_t dec_val;

    switch (g_set_item)
    {
      case SET_ITEM_HOURS:
        dec_val = BCD_To_Dec(g_current_time.hours);
        dec_val = (dec_val + 23) % 24;   /* -1 + 24，避免负数 */
        g_current_time.hours = Dec_To_BCD(dec_val);
        break;

      case SET_ITEM_MINUTES:
        dec_val = BCD_To_Dec(g_current_time.minutes);
        dec_val = (dec_val + 59) % 60;
        g_current_time.minutes = Dec_To_BCD(dec_val);
        break;

      case SET_ITEM_SECONDS:
        dec_val = BCD_To_Dec(g_current_time.seconds);
        dec_val = (dec_val + 59) % 60;
        g_current_time.seconds = Dec_To_BCD(dec_val);
        break;

      default:
        break;
    }
  }
}

/* ================================================================
 * 系统初始化函数
 * ================================================================ */

/**
 * @brief  系统时钟配置函数（72MHz）
 */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* 配置主振荡器和PLL（72MHz） */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;            /* 开启外部低速晶振（32.768kHz，给RTC用） */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置RTC时钟源（LSE = 外部低速晶振 32.768kHz） */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  GPIO初始化函数
 * @note   LCD引脚（PB0~PB2, PB4~PB7）+ 按键（PA0~PA2）+ LED（PC13）
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();   /* AFIO时钟（PB3/PB4是JTAG引脚，需要关闭） */

  /* 关闭JTAG-DP，释放PB3/PB4引脚
   * 因为PB4是LCD的D4，默认是JTAG的NJTRST
   * 所以要禁用JTAG，只用SWD调试
   */
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /* ---------- LCD引脚配置（推挽输出） ----------
   * PB0=RS, PB1=RW, PB2=E, PB4=D4, PB5=D5, PB6=D6, PB7=D7
   */
  GPIO_InitStruct.Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN |
                        LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* LCD引脚初始低电平 */
  HAL_GPIO_WritePin(GPIOB, LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN |
                    LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN, GPIO_PIN_RESET);

  /* ---------- 按键引脚配置（上拉输入） ----------
   * PA0=设置键, PA1=加键, PA2=减键
   * 按键一端接GPIO，一端接GND，按下为低电平
   */
  GPIO_InitStruct.Pin = KEY_SET_PIN | KEY_ADD_PIN | KEY_SUB_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;    /* 内部上拉，按键松开时为高电平 */
  HAL_GPIO_Init(KEY_PORT, &GPIO_InitStruct);

  /* ---------- LED引脚（PC13） ---------- */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始熄灭 */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  RTC初始化函数
 * @note   配置RTC使用外部32.768kHz晶振，BCD格式时间
 *
 * RTC（Real-Time Clock，实时时钟）特点：
 *   - 独立的时钟源（外部32.768kHz晶振或内部LSI）
 *   - 主电源掉电后，可以由VBAT引脚的备份电池继续供电走时
 *   - 用于记录真实时间，不受系统复位影响
 *
 * RTC时钟源选择：
 *   - LSE（外部低速晶振）：32.768kHz，精度高，推荐使用
 *   - LSI（内部低速RC）：约40kHz，精度差，省成本
 *   - HSE/128（外部高速晶振分频）：8MHz/128=62.5kHz
 *
 * STM32F1的RTC本质上是一个32位的自动重装载计数器，
 * 每来一个RTC时钟周期，计数器加1。
 * 配置预分频器为32767时，每秒加1。
 */
static void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};

  /* 使能RTC时钟 */
  __HAL_RCC_RTC_ENABLE();

  /* 配置RTC */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;  /* 自动计算预分频，产生1秒时基 */
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;     /* 无输出 */

  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* 设置初始时间（12:00:00）
   * 实际产品中，可以检查备份寄存器判断是否首次上电，
   * 如果是首次上电才设置初始时间，否则保留原来的时间
   */
  sTime.Hours = 0x12;     /* 12点（BCD码） */
  sTime.Minutes = 0x00;   /* 0分 */
  sTime.Seconds = 0x00;   /* 0秒 */

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  错误处理函数
 */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    HAL_Delay(100);
  }
}

/* ================================================================
 * 知识补充：综合项目设计与RTC深入理解
 * ================================================================
 *
 * 一、RTC实时时钟
 *
 * 1. RTC是什么？
 *    RTC（Real-Time Clock）是一个独立的定时器，
 *    即使主电源掉电，也能通过备份电池继续运行，
 *    用于记录年、月、日、时、分、秒等真实时间。
 *
 * 2. STM32F1的RTC特点
 *    - F1系列的RTC比较简单，本质是32位计数器
 *    - 没有直接的年月日时分秒寄存器（F4以后才有日历功能）
 *    - 需要软件把秒数换算成年月日
 *    - 有20个备份寄存器（BKP），掉电后数据不丢失
 *
 * 3. RTC时钟源
 *    - LSE：外部32.768kHz晶振（精度最高，推荐）
 *    - LSI：内部约40kHz RC振荡器（精度低，不用外接晶振）
 *    - HSE/128：外部高速晶振128分频
 *
 * 4. 为什么用32.768kHz？
 *    32768 = 2^15
 *    经过15级2分频后正好得到1Hz（1秒）的信号，
 *    非常适合做实时时钟的时基。
 *
 * 二、LCD1602字符液晶
 *
 * 1. LCD1602基本参数
 *    - 2行，每行16个字符
 *    - 每个字符是5x8点阵
 *    - 并行接口（8位或4位数据总线）
 *    - 内置字符集ROM（ASCII码等）
 *    - 可自定义8个CGRAM字符
 *
 * 2. 4位模式 vs 8位模式
 *    - 8位模式：D0~D7全部连接，传输快，用IO多
 *    - 4位模式：只用D4~D7，分两次传一个字节，节省IO
 *    - 一般都用4位模式，因为LCD速度不快，4位足够用
 *
 * 3. 常用指令
 *    清屏：       0x01
 *    光标归位：    0x02
 *    输入模式：    0x06（右移，画面不动）
 *    显示开关：    0x08 + 显示位 + 光标位 + 闪烁位
 *    功能设置：    0x20 + 数据位数 + 行数 + 字体
 *    设置DDRAM：   0x80 + 地址
 *
 * 三、BCD码
 *
 * 1. 什么是BCD码？
 *    BCD = Binary-Coded Decimal（二进制编码的十进制）
 *    用4位二进制表示1位十进制数。
 *
 * 2. 为什么RTC用BCD码？
 *    - 早期的RTC芯片（如DS1302）使用BCD码存储时间
 *    - STM32的RTC兼容这种格式
 *    - 直接显示方便，不需要做十进制转换
 *    - 适合用于数码管或LCD显示
 *
 * 3. BCD码运算注意事项
 *    BCD码不能直接做加减运算！
 *    例如：0x09 + 0x01 = 0x0A（错误，BCD没有0x0A）
 *    正确结果应该是 0x10
 *    所以运算前要转成十进制，运算后再转回BCD。
 *
 * 四、综合项目设计思想
 *
 * 1. 分层设计
 *    - 硬件驱动层：LCD驱动、RTC驱动、按键驱动
 *    - 应用逻辑层：时钟显示、设置模式、状态管理
 *    - 主循环：调度各模块执行
 *
 * 2. 模块化编程
 *    每个功能封装成独立的函数，
 *    函数名清晰地表达功能，
 *    便于调试、维护和复用。
 *
 * 3. 状态机思想
 *    设置模式就是一个简单的状态机：
 *    正常显示 → 设置时 → 设置分 → 设置秒 → 正常显示
 *    每个状态有不同的行为（响应不同的按键）。
 *
 * 4. 非阻塞设计
 *    - 主循环不使用阻塞式延时
 *    - 用HAL_GetTick()记录时间点来定时
 *    - 按键扫描、显示刷新、闪烁控制都是非阻塞的
 *    - CPU可以同时处理多件事情
 *
 * 五、扩展功能建议
 *
 * 1. 增加日期显示（年、月、日）
 * 2. 增加闹钟功能
 * 3. 增加温度显示（接DS18B20）
 * 4. 掉电保存设置（用BKP备份寄存器）
 * 5. 增加自动亮度调节（光敏电阻+PWM调光）
 * 6. 使用I2C接口的LCD1602（PCF8574模块，只需要2根线）
 *
 * ================================================================ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
