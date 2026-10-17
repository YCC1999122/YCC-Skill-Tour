/**
 ******************************************************************************
 * @file    02_key_input.c
 * @brief   GPIO输入实验 - 按键输入检测
 *          实现按键消抖、单击、长按短按检测
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐
 *   │  STM32F103   │
 *   │  PA0  ────┬──┤ 按键一端
 *   │           │  │
 *   │          10kΩ│ （上拉电阻，也可以使用内部上拉）
 *   │           │  │
 *   │  3.3V ────┘  │
 *   │              │
 *   │  GND  ───────┤ 按键另一端
 *   └──────────────┘
 *
 *   按键按下时：PA0 = 低电平（GND）
 *   按键松开时：PA0 = 高电平（3.3V，上拉电阻作用）
 *
 *   本示例使用STM32内部上拉电阻，无需外接上拉电阻。
 *
 * 学习重点：
 *   1. GPIO输入模式的配置（上拉/下拉/浮空）
 *   2. HAL_GPIO_ReadPin() 读引脚电平
 *   3. 机械按键的抖动问题及软件消抖方法
 *   4. 长按和短按的检测原理
 *
 * 按键抖动说明：
 *   机械按键按下和松开时，由于金属弹片的弹性，会产生几毫秒到十几毫秒的抖动。
 *   如果不做消抖处理，一次按下可能会被检测成多次。
 *
 *   按键按下时的电平变化：
 *     理想波形：___________````````````````
 *     实际波形：_/\_/\_/\__`````````````````
 *               ← 抖动期 → ← 稳定期 →
 *               约5~20ms
 *
 *   软件消抖方法：检测到电平变化后，延时一段时间再检测一次，
 *   如果两次结果一致，才认为是有效的按键动作。
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================
 * 用宏定义来管理按键和LED的引脚，方便修改和移植。
 * 以后换了引脚，只需要改这里的宏定义即可。
 * ================================================================ */

/* 按键引脚定义（PA0） */
#define KEY_GPIO_PORT       GPIOA           /* 按键所在端口 */
#define KEY_GPIO_PIN        GPIO_PIN_0      /* 按键所在引脚 */
#define KEY_PRESSED_LEVEL   GPIO_PIN_RESET  /* 按键按下时的电平（低电平有效） */

/* LED引脚定义（PC13） */
#define LED_GPIO_PORT       GPIOC           /* LED所在端口 */
#define LED_GPIO_PIN        GPIO_PIN_13     /* LED所在引脚 */

/* 按键消抖时间（单位：毫秒）
 * 一般机械按键的抖动时间在5~20ms之间，设置20ms比较保险。
 * 设置太长会影响响应速度，设置太短可能消抖不彻底。
 */
#define KEY_DEBOUNCE_MS     20

/* 长按判定时间（单位：毫秒）
 * 按键按下持续超过这个时间，判定为长按
 */
#define KEY_LONG_PRESS_MS   1000

/* ================================================================
 * 全局变量区
 * ================================================================
 * 用于记录按键状态和时间
 * ================================================================ */

/* 按键状态枚举
 * 用枚举类型比用0/1更清晰，代码可读性更好
 */
typedef enum {
  KEY_RELEASED = 0,    /* 按键松开 */
  KEY_PRESSED  = 1,    /* 按键按下 */
  KEY_LONG_PRESS = 2   /* 按键长按 */
} KeyState_t;

KeyState_t g_key_state = KEY_RELEASED;   /* 当前按键状态 */
uint32_t g_key_press_time = 0;           /* 按键按下的时刻（用于计时） */
uint8_t  g_long_press_triggered = 0;     /* 长按是否已经触发过（防止重复触发） */

/* 函数声明 */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

KeyState_t Key_Scan(void);    /* 按键扫描函数声明 */

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：按下按键切换LED状态，长按LED闪烁
 */
int main(void)
{
  /* HAL库初始化 */
  HAL_Init();

  /* 系统时钟配置（72MHz） */
  SystemClock_Config();

  /* GPIO初始化（PA0输入，PC13输出） */
  MX_GPIO_Init();

  /* 主循环 */
  while (1)
  {
    /* 扫描按键状态
     * 建议在主循环中周期性调用按键扫描函数，扫描周期最好固定
     */
    KeyState_t key_state = Key_Scan();

    /* 根据按键状态执行相应动作 */
    if (key_state == KEY_PRESSED)
    {
      /* 短按：翻转LED状态 */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    }
    else if (key_state == KEY_LONG_PRESS)
    {
      /* 长按：LED快速闪烁3次（示例效果） */
      for (uint8_t i = 0; i < 6; i++)
      {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
        HAL_Delay(100);
      }
    }

    /* 主循环延时，也可以不加
     * 实际项目中主循环可能有很多任务，扫描间隔自然就有了
     */
    HAL_Delay(10);
  }
}

/**
 * @brief  按键扫描函数
 * @retval KeyState_t - 按键状态（KEY_RELEASED / KEY_PRESSED / KEY_LONG_PRESS）
 *
 * 本函数实现了完整的按键处理：
 *   1. 消抖处理（软件延时消抖）
 *   2. 按下检测（下降沿检测，只在按下瞬间返回一次KEY_PRESSED）
 *   3. 长按检测（按下超过设定时间返回KEY_LONG_PRESS）
 *   4. 松开检测
 *
 * 工作原理（状态机思想）：
 *   - 每次调用都读取一次按键电平
 *   - 结合上一次的状态判断当前状态变化
 *   - 只有在状态稳定（消抖后）才更新状态
 *
 * 注意：此函数需要周期性调用（建议每10~20ms调用一次），
 *       调用频率会影响消抖效果和响应速度。
 */
KeyState_t Key_Scan(void)
{
  /* 静态变量：函数退出后值会保留，下次进入继续使用
   * 这是实现状态保持的关键
   */
  static uint8_t last_key_level = 1;  /* 上一次读取的按键电平（初始为高电平，即松开状态） */
  static uint8_t stable_key_level = 1;/* 稳定后的按键电平 */
  static uint32_t last_change_time = 0; /* 上次电平变化的时刻 */

  KeyState_t ret = KEY_RELEASED;  /* 返回值，默认无事件 */

  /* ---------- 第1步：读取当前按键电平 ----------
   * HAL_GPIO_ReadPin() 用于读取GPIO引脚的电平状态
   * 返回值：
   *   GPIO_PIN_SET   (1) = 高电平
   *   GPIO_PIN_RESET (0) = 低电平
   *
   * 读取的是IDR寄存器（输入数据寄存器），只读寄存器。
   */
  uint8_t current_level = HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PIN);

  /* ---------- 第2步：检测电平变化并消抖 ----------
   * 如果当前电平和上一次读取的不一样，说明有变化（可能是抖动也可能是真实按键）
   */
  if (current_level != last_key_level)
  {
    /* 记录变化时刻和当前电平 */
    last_change_time = HAL_GetTick();  /* HAL_GetTick()获取当前SysTick计数值（毫秒） */
    last_key_level = current_level;
  }
  /* 如果电平和上次一样，并且距离上次变化已经超过消抖时间，
   * 说明电平已经稳定了，是有效的状态变化
   */
  else if (current_level != stable_key_level)
  {
    if ((HAL_GetTick() - last_change_time) >= KEY_DEBOUNCE_MS)
    {
      /* 更新稳定电平 */
      stable_key_level = current_level;

      /* ---------- 第3步：判断是按下还是松开 ---------- */
      if (stable_key_level == KEY_PRESSED_LEVEL)
      {
        /* 按键刚按下（下降沿） */
        g_key_state = KEY_PRESSED;
        g_key_press_time = HAL_GetTick();  /* 记录按下时刻 */
        g_long_press_triggered = 0;        /* 清除长按触发标志 */
        ret = KEY_PRESSED;                 /* 返回按键按下事件 */
      }
      else
      {
        /* 按键刚松开（上升沿） */
        g_key_state = KEY_RELEASED;
      }
    }
  }

  /* ---------- 第4步：检测长按 ----------
   * 如果按键处于按下状态，并且还没有触发过长按，
   * 检查按下时间是否超过了长按阈值
   */
  if ((g_key_state == KEY_PRESSED) && (g_long_press_triggered == 0))
  {
    if ((HAL_GetTick() - g_key_press_time) >= KEY_LONG_PRESS_MS)
    {
      g_long_press_triggered = 1;  /* 标记长按已触发 */
      ret = KEY_LONG_PRESS;        /* 返回长按事件 */
    }
  }

  return ret;
}

/**
 * @brief  系统时钟配置（72MHz）
 * @note   详细注释请参考 01_led_blink.c
 */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
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
}

/**
 * @brief  GPIO初始化函数
 * @note   配置PA0为输入模式（带上拉），PC13为输出模式
 *
 * GPIO输入模式详解：
 *   1. GPIO_MODE_INPUT + GPIO_NOPULL：浮空输入
 *      - 引脚电平完全由外部电路决定，没有内部上下拉
 *      - 缺点：引脚悬空时电平不确定，可能受到干扰
 *      - 适用：外部已经有上下拉电阻的情况
 *
 *   2. GPIO_MODE_INPUT + GPIO_PULLUP：上拉输入
 *      - 引脚内部连接了一个上拉电阻（约40kΩ）到VDD
 *      - 引脚悬空时默认为高电平
 *      - 适用：按键一端接GND的情况（本示例使用这种）
 *
 *   3. GPIO_MODE_INPUT + GPIO_PULLDOWN：下拉输入
 *      - 引脚内部连接了一个下拉电阻到GND
 *      - 引脚悬空时默认为低电平
 *      - 适用：按键一端接VCC的情况
 *
 * 为什么要用上拉输入？
 *   因为按键的另一端接GND，按下时引脚接地（低电平）。
 *   如果没有上拉电阻，松开按键时引脚是浮空的，电平不确定，
 *   可能会因为电磁干扰而乱跳，导致误触发。
 *   上拉电阻确保了按键松开时引脚稳定在高电平。
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ---------- 使能时钟 ----------
   * GPIOA和GPIOC都在APB2总线上
   */
  __HAL_RCC_GPIOA_CLK_ENABLE();   /* 使能GPIOA时钟（按键用） */
  __HAL_RCC_GPIOC_CLK_ENABLE();   /* 使能GPIOC时钟（LED用） */

  /* ---------- 配置PA0为上拉输入（按键） ---------- */
  GPIO_InitStruct.Pin = KEY_GPIO_PIN;        /* 按键引脚：PA0 */
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    /* 输入模式 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;        /* 上拉电阻（确保悬空时为高电平） */
  /* 注意：输入模式下Speed参数无效，可以不设置 */

  HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  /* ---------- 配置PC13为推挽输出（LED） ---------- */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;              /* LED引脚：PC13 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      /* 推挽输出模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;              /* 输出模式不需要上下拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     /* 低速输出 */

  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始状态：熄灭（高电平） */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
