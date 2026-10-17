/**
 ******************************************************************************
 * @file    01_timer_basic.c
 * @brief   基本定时器实验 - TIM6定时中断实现1秒LED闪烁
 *          使用TIM6基本定时器，配置定时中断，每1秒触发一次中断
 *          在中断回调函数中翻转LED电平，实现非阻塞式LED闪烁
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐
 *   │  STM32F103   │
 *   │  PC13 ────┬──┤ LED阳极（长脚）
 *   │            │  │
 *   │           1kΩ │ （限流电阻）
 *   │            │  │
 *   │  GND  ────┴──┤ LED阴极（短脚）
 *   └──────────────┘
 *   注意：大部分最小系统板的LED是低电平点亮（共阳极接法），
 *         即PC13输出低电平时LED亮，高电平时灭。
 *
 * 定时器时钟说明：
 *   TIM6/TIM7 挂在 APB1 总线上
 *   APB1预分频 = 2，PCLK1 = 36MHz
 *   但定时器时钟 = PCLK1 × 2 = 72MHz（当APB预分频不为1时，定时器时钟加倍）
 *   所以 TIM6 的工作时钟是 72MHz
 *
 * 学习重点：
 *   1. 基本定时器TIM6/TIM7的特点和寄存器结构
 *   2. HAL_TIM_Base_Init() 定时器时基初始化
 *   3. Period（自动重装载值ARR）和 Prescaler（预分频值PSC）的计算
 *   4. HAL_TIM_Base_Start_IT() 启动定时器中断
 *   5. HAL_TIM_PeriodElapsedCallback() 定时器更新中断回调函数
 *   6. 定时器中断的NVIC配置
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* LED引脚定义 */
#define LED_GPIO_PORT       GPIOC
#define LED_GPIO_PIN        GPIO_PIN_13

/* 定时器参数计算
 *
 * 定时器溢出频率公式：
 *   f_overflow = f_timer_clk / ((PSC + 1) × (ARR + 1))
 *
 * 其中：
 *   f_timer_clk = 定时器工作时钟 = 72MHz（TIM6在APB1上，APB1预分频为2时定时器时钟加倍）
 *   PSC = 预分频器值（Prescaler），16位，范围 0 ~ 65535
 *   ARR = 自动重装载值（Period），16位，范围 0 ~ 65535
 *
 * 目标：定时时间 = 1秒，即溢出频率 = 1Hz
 *
 * 计算方法：
 *   我们需要 (PSC + 1) × (ARR + 1) = 72MHz / 1Hz = 72,000,000
 *
 *   由于 PSC 和 ARR 都是16位寄存器（最大值65535），
 *   72,000,000 超过了 16位的范围，所以需要两个寄存器配合。
 *
 *   方案：让 PSC+1 = 7200，ARR+1 = 10000
 *         7200 × 10000 = 72,000,000  ✓
 *
 *   即：PSC = 7199，ARR = 9999
 *
 *   验证：72MHz / (7200 × 10000) = 72,000,000 / 72,000,000 = 1Hz = 1秒  ✓
 */
#define TIM6_PRESCALER       7199    /* 预分频值：72MHz / (7199+1) = 10kHz */
#define TIM6_PERIOD          9999    /* 自动重装载值：10kHz / (9999+1) = 1Hz = 1秒 */

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* TIM6句柄
 * HAL库使用"句柄"（Handle）来管理外设实例。
 * 句柄是一个结构体，包含了外设的配置参数、状态信息等。
 * 所有HAL库的定时器函数都需要传入句柄的指针。
 */
TIM_HandleTypeDef htim6;

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
void Error_Handler(void);

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：使用TIM6定时器中断实现1秒LED闪烁
 *
 * 与01_led_blink.c的区别：
 *   - 01_led_blink.c 使用 HAL_Delay() 阻塞式延时，CPU在延时期间什么都做不了
 *   - 本程序 使用定时器中断，CPU可以在主循环中执行其他任务
 *     LED翻转由定时器中断自动完成，不占用CPU时间
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ========== */
  HAL_Init();

  /* ========== 第2步：系统时钟配置（72MHz） ========== */
  SystemClock_Config();

  /* ========== 第3步：GPIO初始化 ========== */
  MX_GPIO_Init();

  /* ========== 第4步：TIM6定时器初始化 ========== */
  MX_TIM6_Init();

  /* ========== 第5步：启动定时器中断 ==========
   * HAL_TIM_Base_Start_IT() 启动定时器并使能更新中断（Update Interrupt）
   * 当计数器计数到ARR值时，会产生更新中断，触发中断服务函数
   *
   * 注意区分三种启动方式：
   *   HAL_TIM_Base_Start()       - 仅启动定时器，不产生中断（需要用查询方式）
   *   HAL_TIM_Base_Start_IT()    - 启动定时器并使能更新中断
   *   HAL_TIM_Base_Start_DMA()   - 启动定时器并使能DMA请求
   */
  if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 第6步：主循环 ==========
   * 注意：这里没有 HAL_Delay() 阻塞延时！
   * LED的翻转完全由定时器中断处理，主循环可以执行其他任务。
   *
   * 这就是定时器中断的优势：非阻塞、精确计时、不占用CPU主循环。
   */
  while (1)
  {
    /* ------------------------------------------------
     * 主循环中可以执行其他任务，例如：
     *   - 按键扫描
     *   - 数据处理
     *   - 其他外设操作
     *
     * LED闪烁由TIM6中断自动完成，不需要在这里处理
     * ------------------------------------------------ */

    /* 示例：这里可以放其他代码，不会影响LED闪烁 */
    /* 为了演示效果，主循环保持空转 */
  }
}

/**
 * @brief  定时器更新中断回调函数
 * @param  htim: 定时器句柄指针
 * @note   这是HAL库的"弱定义"回调函数，用户重写后会覆盖默认实现。
 *         当定时器发生更新事件（计数器溢出）时，HAL库的中断服务函数
 *         会自动调用这个回调函数。
 *
 * 执行流程：
 *   1. TIM6计数器从0开始向上计数
 *   2. 计数到ARR(9999)时，产生更新事件，触发中断
 *   3. CPU跳转到 TIM6_IRQHandler() 中断服务函数
 *   4. TIM6_IRQHandler() 调用 HAL_TIM_IRQHandler(&htim6)
 *   5. HAL_TIM_IRQHandler() 清除中断标志，然后调用 HAL_TIM_PeriodElapsedCallback()
 *   6. 在回调函数中执行用户代码（翻转LED）
 *   7. 计数器自动从0重新开始计数
 *
 * 注意事项：
 *   1. 回调函数在中断上下文中执行，不能执行耗时操作！
 *   2. 不要在回调函数中调用 HAL_Delay()，可能导致死锁
 *   3. 多个定时器共用同一个回调函数，需要通过 htim->Instance 判断是哪个定时器
 *   4. 中断处理要尽量短，一般只设置标志位，具体逻辑放到主循环中处理
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* 判断是哪个定时器触发的中断
   * 因为所有定时器的更新中断都会调用这个回调函数，
   * 所以需要判断是哪个定时器，避免误操作。
   */
  if (htim->Instance == TIM6)
  {
    /* 翻转LED引脚电平
     * 每1秒进入一次这个回调，所以LED每1秒翻转一次
     * 即LED亮0.5秒、灭0.5秒，闪烁周期为2秒
     */
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
  }
}

/**
 * @brief  系统时钟配置函数（72MHz）
 * @note   详细注释请参考 01_GPIO/01_led_blink.c
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
 * @note   配置PC13为推挽输出模式，用于LED指示
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能GPIOC端口时钟 */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* 配置PC13为推挽输出（LED） */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始状态：熄灭（高电平） */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  TIM6基本定时器初始化函数
 * @note   配置TIM6为1秒定时中断
 *
 * 基本定时器TIM6/TIM7的特点：
 *   1. 16位向上计数器（只能从0数到ARR，不能向下计数）
 *   2. 16位可编程预分频器（PSC）
 *   3. 没有输入/输出通道，不能做PWM、输入捕获等
 *   4. 主要用于：定时中断、DAC触发、时基基准
 *   5. TIM6和TIM7是完全独立的，功能相同
 *
 * 定时器工作原理（向上计数模式）：
 *
 *   计数器CNT:  0 → 1 → 2 → ... → ARR → 0 → 1 → ...
 *                                                    ↑
 *                                               更新事件（UEV）
 *                                               产生中断/DMA请求
 *                                               CNT清零重新开始
 *
 *   每来一个计数时钟，CNT加1
 *   当CNT达到ARR时，产生更新事件（Update Event），CNT自动清零
 *   如果使能了更新中断，就会触发中断
 *
 * HAL库定时器配置步骤：
 *   1. 使能定时器时钟（__HAL_RCC_TIMx_CLK_ENABLE）
 *   2. 定义TIM_HandleTypeDef句柄
 *   3. 配置时基初始化结构体（TIM_Base_InitTypeDef）
 *   4. 调用HAL_TIM_Base_Init()初始化
 *   5. 配置NVIC中断优先级
 *   6. 调用HAL_TIM_Base_Start_IT()启动定时器中断
 */
static void MX_TIM6_Init(void)
{
  /* ========== 第1步：使能TIM6时钟 ==========
   * TIM6挂在APB1总线上
   * __HAL_RCC_TIM6_CLK_ENABLE() 使能TIM6的外设时钟
   *
   * 注意：APB1上的定时器有 TIM2, TIM3, TIM4, TIM5, TIM6, TIM7
   *       APB2上的定时器有 TIM1, TIM8（高级定时器）
   */
  __HAL_RCC_TIM6_CLK_ENABLE();

  /* ========== 第2步：配置定时器时基 ==========
   * htim6.Instance - 选择哪个定时器（TIM6）
   * htim6.Init     - 时基初始化参数结构体
   *
   * TIM_Base_InitTypeDef 结构体成员说明：
   *
   *   Prescaler：预分频器值（PSC）
   *     定时器时钟 = 输入时钟 / (PSC + 1)
   *     PSC是16位的，范围 0 ~ 65535
   *     为什么要+1？因为预分频器是从0开始计数的，
   *     PSC=0 表示不分频（每1个时钟脉冲计数一次）
   *     PSC=1 表示2分频（每2个时钟脉冲计数一次）
   *
   *   CounterMode：计数模式
   *     TIM_COUNTERMODE_UP     - 向上计数（从0到ARR）
   *     TIM_COUNTERMODE_DOWN   - 向下计数（从ARR到0）
   *     TIM_COUNTERMODE_CENTERALIGNED1/2/3 - 中心对齐模式
   *     注意：基本定时器TIM6/TIM7只支持向上计数模式！
   *
   *   Period：自动重装载值（ARR）
   *     计数器计数到这个值后溢出，产生更新事件
   *     ARR是16位的，范围 0 ~ 65535
   *     实际计数个数是 ARR + 1（因为从0开始）
   *
   *   ClockDivision：时钟分频因子
   *     用于设置数字滤波器的采样时钟频率
   *     TIM_CLOCKDIVISION_DIV1 - 不分频
   *     TIM_CLOCKDIVISION_DIV2 - 2分频
   *     TIM_CLOCKDIVISION_DIV4 - 4分频
   *     基本定时器没有输入捕获，这个参数无效
   *
   *   AutoReloadPreload：自动重装载预装载
   *     TIM_AUTORELOAD_PRELOAD_DISABLE - ARR寄存器立即生效
   *     TIM_AUTORELOAD_PRELOAD_ENABLE  - ARR寄存器在更新事件时生效
   *     基本定时器一般用默认值即可
   */
  htim6.Instance = TIM6;                         /* 选择TIM6定时器 */
  htim6.Init.Prescaler = TIM6_PRESCALER;         /* 预分频值：7199 */
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 向上计数模式（基本定时器只能向上） */
  htim6.Init.Period = TIM6_PERIOD;               /* 自动重装载值：9999 */
  htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* 时钟分频：不分频 */
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* ARR立即生效 */

  /* ========== 第3步：调用HAL_TIM_Base_Init初始化 ==========
   * HAL_TIM_Base_Init() 会根据传入的参数配置定时器的寄存器。
   * 它内部会调用 HAL_TIM_Base_MspInit() 来完成底层初始化（时钟使能、GPIO、NVIC等）。
   *
   * 注意：HAL库有"MspInit"机制，外设初始化分为两层：
   *   - 上层：HAL_xxx_Init() - 配置外设的功能寄存器
   *   - 底层：HAL_xxx_MspInit() - 配置时钟、GPIO、NVIC等硬件相关
   *
   * 我们可以重写 HAL_TIM_Base_MspInit() 来做底层初始化，
   * 也可以直接在 MX_TIM6_Init() 中手动配置（本文件采用手动方式，更直观）。
   */
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 第4步：配置NVIC中断 ==========
   * 定时器要产生中断，还需要配置NVIC（嵌套向量中断控制器）。
   * NVIC负责管理所有中断的优先级和使能。
   *
   * HAL_NVIC_SetPriority() - 设置中断优先级
   *   参数1：中断号（TIM6_IRQn）
   *   参数2：抢占优先级（0-15，数值越小优先级越高）
   *   参数3：子优先级（0-15）
   *
   * HAL_NVIC_EnableIRQ() - 使能中断线
   *   参数：中断号
   *
   * 注意：必须在 NVIC 中使能中断，定时器的中断请求才能到达CPU。
   */
  HAL_NVIC_SetPriority(TIM6_IRQn, 0, 0);  /* 设置TIM6中断优先级为最高 */
  HAL_NVIC_EnableIRQ(TIM6_IRQn);          /* 使能TIM6中断 */
}

/**
 * @brief  错误处理函数
 */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    /* LED快速闪烁表示错误 */
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    HAL_Delay(100);
  }
}

/* ================================================================
 * 知识补充：STM32定时器家族
 * ================================================================
 *
 * STM32F103系列共有8个定时器，分为四类：
 *
 * 1. 高级控制定时器（TIM1, TIM8）
 *    - 16位，支持向上/向下/中心对齐计数
 *    - 4个独立通道：输入捕获、输出比较、PWM生成、单脉冲模式
 *    - 带死区时间控制的互补输出（用于电机驱动、开关电源）
 *    - 挂在APB2上，时钟72MHz
 *
 * 2. 通用定时器（TIM2, TIM3, TIM4, TIM5）
 *    - 16位，支持向上/向下/中心对齐计数
 *    - 4个独立通道：输入捕获、输出比较、PWM生成
 *    - 编码器接口、霍尔传感器接口
 *    - 挂在APB1上，定时器时钟72MHz
 *
 * 3. 基本定时器（TIM6, TIM7）
 *    - 16位，只能向上计数
 *    - 没有输入/输出通道
 *    - 主要用途：定时中断、DAC触发信号
 *    - 挂在APB1上，定时器时钟72MHz
 *
 * 4. 看门狗定时器（IWDG, WWDG）
 *    - 独立看门狗和窗口看门狗
 *    - 用于程序跑飞时自动复位
 *
 * ================================================================
 * 知识补充：定时器时钟频率的计算（重要！容易搞错）
 * ================================================================
 *
 * STM32的定时器时钟不是简单地等于APB总线时钟，而是有一个规则：
 *
 *   如果 APB 预分频系数 = 1，则定时器时钟 = APB时钟
 *   如果 APB 预分频系数 > 1，则定时器时钟 = APB时钟 × 2
 *
 * 为什么是这样？
 *   因为定时器需要工作在较高的频率下，
 *   当APB总线被分频降低速度时，定时器仍然保持较高的时钟。
 *
 * 具体到我们的配置（72MHz系统时钟）：
 *
 *   APB2预分频 = 1 → PCLK2 = 72MHz
 *     → APB2上的定时器（TIM1, TIM8）时钟 = 72MHz × 1 = 72MHz
 *
 *   APB1预分频 = 2 → PCLK1 = 36MHz
 *     → APB1上的定时器（TIM2~7）时钟 = 36MHz × 2 = 72MHz
 *
 * 所以：所有定时器的时钟都是72MHz！
 * 这是一个常见的面试题，也是新手容易出错的地方。
 *
 * ================================================================
 * 知识补充：中断服务函数的完整调用链
 * ================================================================
 *
 * 当定时器溢出时，完整的调用过程是：
 *
 *   1. 硬件层面：
 *      TIM6的CNT计数器达到ARR值
 *      → 硬件置位 TIM6->SR 寄存器的 UIF 位（更新中断标志）
 *      → 中断信号送到 NVIC
 *      → NVIC判断优先级，决定是否立即响应
 *
 *   2. 启动文件（startup_stm32f103xb.s）：
 *      → CPU跳转到中断向量表中 TIM6_IRQHandler 的地址
 *      → 执行 TIM6_IRQHandler 函数
 *
 *   3. 用户需要实现 TIM6_IRQHandler：
 *      → 调用 HAL_TIM_IRQHandler(&htim6)
 *
 *   4. HAL库的 HAL_TIM_IRQHandler：
 *      → 检查是哪种中断（更新/捕获/比较等）
 *      → 清除中断标志位
 *      → 调用对应的回调函数
 *      → 更新中断的回调是 HAL_TIM_PeriodElapsedCallback
 *
 *   5. 用户的回调函数：
 *      → 在 HAL_TIM_PeriodElapsedCallback 中编写业务逻辑
 *
 * 注意：在STM32CubeMX生成的代码中，TIM6_IRQHandler已经在stm32f1xx_it.c中实现了。
 * 如果是自己写工程，需要自己实现中断服务函数并调用HAL_TIM_IRQHandler。
 *
 * ================================================================
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
