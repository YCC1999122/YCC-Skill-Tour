/**
 ******************************************************************************
 * @file    02_timer_pwm.c
 * @brief   定时器PWM输出实验 - TIM3 CH1 呼吸灯效果
 *          使用TIM3通道1（PA6引脚）输出PWM波形，通过调节占空比
 *          实现LED呼吸灯效果（渐亮渐灭）
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐
 *   │  STM32F103   │
 *   │  PA6  ────┬──┤ LED阳极（长脚）
 *   │  (TIM3_CH1)│  │
 *   │           220Ω│ （限流电阻，PWM驱动用较小阻值）
 *   │            │  │
 *   │  GND  ────┴──┤ LED阴极（短脚）
 *   └──────────────┘
 *
 *   注意：PA6是TIM3通道1的复用输出引脚（AFIO复用功能）
 *         需要将PA6配置为复用推挽输出模式
 *
 * PWM原理简介：
 *   PWM（Pulse Width Modulation，脉冲宽度调制）
 *   通过改变高电平时间（占空比）来控制平均电压，从而控制LED亮度。
 *
 *   占空比 = 高电平时间 / 周期 × 100%
 *
 *   100%占空比：████████████████  最亮
 *    75%占空比：████████████░░░░  较亮
 *    50%占空比：████████░░░░░░░░  中等亮度
 *    25%占空比：████░░░░░░░░░░░░  较暗
 *     0%占空比：░░░░░░░░░░░░░░░░  熄灭
 *
 * 学习重点：
 *   1. PWM（脉冲宽度调制）的基本原理
 *   2. 通用定时器TIM3的PWM输出模式配置
 *   3. GPIO复用功能（AFIO）的配置方法
 *   4. HAL_TIM_PWM_Init() 和 HAL_TIM_PWM_Start()
 *   5. __HAL_TIM_SET_COMPARE() 宏修改占空比
 *   6. 呼吸灯效果的实现（渐变占空比）
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* PWM输出引脚：PA6 对应 TIM3 CH1 */
#define PWM_TIM_INSTANCE        TIM3
#define PWM_CHANNEL             TIM_CHANNEL_1
#define PWM_GPIO_PORT           GPIOA
#define PWM_GPIO_PIN            GPIO_PIN_6

/* PWM参数计算
 *
 * PWM频率公式：
 *   f_pwm = f_timer_clk / ((PSC + 1) × (ARR + 1))
 *
 * PWM占空比：
 *   duty = CCR / (ARR + 1) × 100%
 *   CCR 是捕获/比较寄存器的值
 *
 * 参数选择：
 *   目标PWM频率：1kHz（人眼看不到闪烁，适合LED呼吸灯）
 *   定时器时钟：72MHz
 *
 *   方案：PSC = 72 - 1 = 71
 *         计数时钟 = 72MHz / 72 = 1MHz = 1μs/计
 *
 *         ARR = 1000 - 1 = 999
 *         PWM频率 = 1MHz / 1000 = 1kHz  ✓
 *
 *         占空比分辨率：1/1000 = 0.1%（1000级亮度）
 *         CCR范围：0 ~ 999
 *           CCR = 0   → 占空比 0%  （全灭）
 *           CCR = 500 → 占空比 50% （半亮）
 *           CCR = 999 → 占空比 ~100%（最亮，注意是999/1000=99.9%）
 */
#define TIM3_PRESCALER          71      /* 预分频值：72MHz / 72 = 1MHz */
#define TIM3_PERIOD             999     /* 自动重装载值：1MHz / 1000 = 1kHz PWM */
#define PWM_MAX_DUTY            999     /* 最大占空比对应的CCR值 */

/* 呼吸灯步进延时（毫秒）
 * 数值越大，呼吸速度越慢
 * 总呼吸周期 = 2 × PWM_MAX_DUTY × BREATH_STEP_MS
 *           = 2 × 1000 × 1 = 2000ms = 2秒
 */
#define BREATH_STEP_MS          1

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* TIM3句柄 */
TIM_HandleTypeDef htim3;

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_PWM_Init(void);
void Error_Handler(void);

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：使用TIM3的PWM输出实现LED呼吸灯效果
 *
 * 呼吸灯原理：
 *   1. 占空比从0逐渐增加到100%  → LED渐亮
 *   2. 占空比从100%逐渐减少到0  → LED渐灭
 *   3. 循环往复 → 呼吸效果
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ========== */
  HAL_Init();

  /* ========== 第2步：系统时钟配置（72MHz） ========== */
  SystemClock_Config();

  /* ========== 第3步：GPIO初始化 ========== */
  MX_GPIO_Init();

  /* ========== 第4步：TIM3 PWM模式初始化 ========== */
  MX_TIM3_PWM_Init();

  /* ========== 第5步：启动PWM输出 ==========
   * HAL_TIM_PWM_Start() 启动PWM输出
   * 参数1：定时器句柄指针
   * 参数2：通道号（TIM_CHANNEL_1 ~ TIM_CHANNEL_4）
   *
   * 启动后，PA6引脚就会持续输出PWM波形。
   * 我们可以随时通过修改CCR寄存器来改变占空比。
   */
  if (HAL_TIM_PWM_Start(&htim3, PWM_CHANNEL) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 第6步：主循环 - 呼吸灯效果 ========== */
  while (1)
  {
    /* ---------- 渐亮过程：占空比从0增加到最大 ---------- */
    for (uint16_t duty = 0; duty <= PWM_MAX_DUTY; duty++)
    {
      /* 修改比较寄存器CCR的值，改变占空比
       *
       * __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)
       *   - __HANDLE__：定时器句柄指针
       *   - __CHANNEL__：通道号
       *   - __COMPARE__：比较值（CCR值）
       *
       * 这是一个宏定义，展开后相当于：
       *   (__HANDLE__)->Instance->CCR1 = (__COMPARE__)
       *
       * 注意：修改CCR的值是立即生效的吗？
       *   取决于是否启用了预装载（OCxPE位）。
       *   如果启用了预装载，新的CCR值会在下次更新事件时才生效；
       *   如果没有启用，立即生效。
       *   HAL库默认配置中，PWM模式下预装载是使能的（OCxPE = 1）。
       */
      __HAL_TIM_SET_COMPARE(&htim3, PWM_CHANNEL, duty);

      /* 延时一段时间，让肉眼能看到渐变效果 */
      HAL_Delay(BREATH_STEP_MS);
    }

    /* ---------- 渐灭过程：占空比从最大减少到0 ---------- */
    for (uint16_t duty = PWM_MAX_DUTY; duty > 0; duty--)
    {
      __HAL_TIM_SET_COMPARE(&htim3, PWM_CHANNEL, duty);
      HAL_Delay(BREATH_STEP_MS);
    }

    /* 循环往复，实现呼吸效果 */
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
 * @note   配置PA6为复用推挽输出模式（TIM3_CH1）
 *
 * 复用功能（AFIO）说明：
 *   STM32的很多引脚除了普通GPIO功能外，还可以配置为外设功能（复用功能）。
 *   例如PA6可以配置为：
 *     - 普通GPIO输入/输出
 *     - TIM3_CH1（定时器3通道1）
 *     - ADC_IN6（ADC通道6）
 *
 *   当引脚用作外设功能时，需要配置为"复用模式"：
 *     - GPIO_MODE_AF_PP  复用推挽输出（用于PWM、串口TX等）
 *     - GPIO_MODE_AF_OD  复用开漏输出（用于I2C等）
 *
 *   复用功能的引脚映射关系可以在数据手册的"引脚定义"章节查到。
 *   部分引脚有重映射（Remap）功能，可以通过AFIO寄存器重新映射到其他引脚。
 *
 *   本示例使用默认映射：TIM3_CH1 → PA6（不需要重映射）
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能GPIOA端口时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* 使能AFIO时钟（复用功能IO时钟）
   * 注意：使用复用功能时，必须使能AFIO时钟！
   * 这是新手常犯的错误之一。
   */
  __HAL_RCC_AFIO_CLK_ENABLE();

  /* ---------- 配置PA6为复用推挽输出 ----------
   *
   * Mode = GPIO_MODE_AF_PP：复用推挽输出模式
   *   引脚由外设（TIM3）控制，而不是由GPIO的ODR寄存器控制。
   *   推挽输出可以输出强高低电平，适合PWM输出。
   *
   * 注意：
   *   1. 复用模式下，引脚的输出状态由外设决定，HAL_GPIO_WritePin无效
   *   2. 必须使能AFIO时钟
   *   3. 如果有重映射需求，需要调用 HAL_GPIO_AFIO_REMAP 或直接操作寄存器
   */
  GPIO_InitStruct.Pin = PWM_GPIO_PIN;            /* PA6 */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        /* 复用推挽输出模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;            /* 不需要上下拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  /* 高速（PWM频率较高） */

  HAL_GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  TIM3 PWM模式初始化函数
 * @note   配置TIM3通道1为PWM输出模式
 *
 * PWM工作原理（PWM模式1，向上计数）：
 *
 *   CNT计数器:  0 ─────────────────────────────────▶ ARR
 *               │                                   │
 *   CCR比较值:  │───────────●                       │
 *               │           │                       │
 *   PWM输出:    │███████████│░░░░░░░░░░░░░░░░░░░░░░░│
 *               │  高电平   │        低电平          │
 *               └───────────┴───────────────────────┘
 *               ↑           ↑
 *             CNT=0      CNT=CCR1
 *             输出置1    输出清0
 *
 *   PWM模式1（向上计数）：
 *     当 CNT < CCR 时，输出高电平
 *     当 CNT >= CCR 时，输出低电平
 *     占空比 = CCR / (ARR + 1) × 100%
 *
 *   PWM模式2（向上计数）：
 *     当 CNT < CCR 时，输出低电平
 *     当 CNT >= CCR 时，输出高电平
 *     占空比 = (ARR + 1 - CCR) / (ARR + 1) × 100%
 *
 *   简单理解：模式1是CCR越大占空比越高，模式2相反。
 *
 * 通用定时器TIM3的通道：
 *   CH1 → PA6（默认）/ PB4（部分重映射）/ PC6（完全重映射）
 *   CH2 → PA7（默认）/ PB5（部分重映射）/ PC7（完全重映射）
 *   CH3 → PB0（默认）/ PC8（完全重映射）
 *   CH4 → PB1（默认）/ PC9（完全重映射）
 *
 *   重映射需要配置AFIO_REMAP寄存器。
 */
static void MX_TIM3_PWM_Init(void)
{
  /* PWM输出配置结构体
   * 用于配置PWM模式、脉冲宽度等参数
   */
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* ========== 第1步：使能TIM3时钟 ==========
   * TIM3挂在APB1总线上，定时器时钟 = 72MHz
   */
  __HAL_RCC_TIM3_CLK_ENABLE();

  /* ========== 第2步：配置定时器时基（PSC、ARR等） ========== */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = TIM3_PRESCALER;         /* 预分频：72MHz / 72 = 1MHz */
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 向上计数 */
  htim3.Init.Period = TIM3_PERIOD;               /* ARR = 999，PWM频率 = 1kHz */
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 第3步：配置PWM输出通道 ==========
   *
   * TIM_OC_InitTypeDef 结构体成员说明：
   *
   *   OCMode：输出比较模式
   *     TIM_OCMODE_TIMING   - 定时模式（仅产生中断，不影响输出引脚）
   *     TIM_OCMODE_ACTIVE   - 匹配时输出有效电平
   *     TIM_OCMODE_INACTIVE - 匹配时输出无效电平
   *     TIM_OCMODE_TOGGLE   - 匹配时翻转输出
   *     TIM_OCMODE_PWM1     - PWM模式1（向上计数时，CNT<CCR输出高）
   *     TIM_OCMODE_PWM2     - PWM模式2（向上计数时，CNT<CCR输出低）
   *     TIM_OCMODE_FORCED_ACTIVE   - 强制输出有效电平
   *     TIM_OCMODE_FORCED_INACTIVE - 强制输出无效电平
   *
   *   Pulse：脉冲宽度，即CCR寄存器的初始值
   *     对应初始占空比，范围 0 ~ ARR
   *
   *   OCPolarity：输出极性
   *     TIM_OCPOLARITY_HIGH - 高电平有效（PWM模式1下，占空比=高电平比例）
   *     TIM_OCPOLARITY_LOW  - 低电平有效（输出反向）
   *
   *   OCFastMode：输出比较快速使能
   *     用于PWM模式下快速响应，一般保持默认关闭
   *
   *   OCIdleState：空闲状态（MOE=0时的输出状态）
   *     主要用于高级定时器带死区的互补输出，通用定时器一般不用
   */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;        /* PWM模式1 */
  sConfigOC.Pulse = 0;                       /* 初始占空比：0（全灭） */
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; /* 高电平有效 */
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;  /* 关闭快速模式 */

  /* 配置通道1的PWM输出
   * HAL_TIM_PWM_ConfigChannel() 配置指定通道的PWM参数
   */
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, PWM_CHANNEL) != HAL_OK)
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
    /* 这里用死循环表示错误，实际项目可添加报警 */
  }
}

/* ================================================================
 * 知识补充：PWM的常见应用场景
 * ================================================================
 *
 * 1. LED调光 / 呼吸灯
 *    通过改变占空比控制LED的平均电流，从而改变亮度。
 *    频率一般选1kHz以上，人眼就看不到闪烁了。
 *
 * 2. 电机调速
 *    直流电机的转速与平均电压成正比，通过PWM可以方便地调速。
 *    频率一般选10kHz~20kHz（避开人耳听觉范围，减少噪声）。
 *
 * 3. 舵机控制
 *    舵机通过PWM脉冲宽度来控制角度。
 *    典型值：周期20ms（50Hz），脉冲宽度0.5ms~2.5ms对应0°~180°。
 *
 * 4. 蜂鸣器播放音乐
 *    通过改变PWM频率来产生不同音高的声音。
 *    同时可以通过占空比控制音量。
 *
 * 5. DAC（数模转换）
 *    PWM + RC低通滤波器可以实现简易的DAC功能。
 *    占空比对应输出电压的平均值。
 *
 * 6. 开关电源
 *    降压/升压变换器通过PWM控制开关管的导通时间来稳定输出电压。
 *
 * ================================================================
 * 知识补充：PWM频率和分辨率的权衡
 * ================================================================
 *
 * PWM的分辨率（即占空比有多少级）取决于ARR的值：
 *   分辨率 = ARR + 1 级
 *
 * PWM频率公式：
 *   f_pwm = f_timer / ((PSC + 1) × (ARR + 1))
 *
 * 从公式可以看出：
 *   - ARR越大，分辨率越高，但PWM频率越低
 *   - ARR越小，PWM频率越高，但分辨率越低
 *
 * 这是一个权衡关系，需要根据应用场景选择合适的参数。
 *
 * 示例（定时器时钟72MHz）：
 *
 *   场景          PSC     ARR      PWM频率     分辨率
 *   LED呼吸灯     71      999      1kHz        1000级
 *   电机调速      0       719      100kHz      720级
 *   舵机控制      719     1999     50Hz        2000级
 *   蜂鸣器        0       1000     72kHz       1000级
 *
 * 经验法则：
 *   - LED调光：频率1kHz以上，分辨率256级以上就够用了
 *   - 电机调速：频率10kHz以上（避免人耳噪声），分辨率适中
 *   - 高分辨率DAC替代：尽量提高分辨率，频率够用就行
 *
 * ================================================================
 * 知识补充：定时器的8种输出比较模式
 * ================================================================
 *
 * 通用定时器的每个通道都支持8种输出比较模式：
 *
 * 1. 冻结模式（Frozen）
 *    比较匹配时，输出不变。用于基准定时。
 *
 * 2. 匹配时置有效电平（Active on match）
 *    CNT=CCR时，输出变高。
 *
 * 3. 匹配时置无效电平（Inactive on match）
 *    CNT=CCR时，输出变低。
 *
 * 4. 匹配时翻转（Toggle）
 *    CNT=CCR时，输出翻转。可用于产生方波。
 *
 * 5. 强制无效电平（Force inactive）
 *    强制输出低电平，与比较器无关。
 *
 * 6. 强制有效电平（Force active）
 *    强制输出高电平，与比较器无关。
 *
 * 7. PWM模式1（PWM mode 1）
 *    向上计数：CNT<CCR时有效，CNT>=CCR时无效。
 *    向下计数：相反。
 *    这是最常用的PWM模式。
 *
 * 8. PWM模式2（PWM mode 2）
 *    与模式1相反。向上计数：CNT<CCR时无效，CNT>=CCR时有效。
 *
 * ================================================================
 * 知识补充：高级定时器的PWM互补输出和死区
 * ================================================================
 *
 * 高级定时器（TIM1、TIM8）相比通用定时器，多了以下功能：
 *
 * 1. 互补输出（Complementary outputs）
 *    每个通道有两个输出：OCx和OCxN，波形互补。
 *    用于H桥、三相逆变器等需要互补驱动的场合。
 *
 * 2. 死区时间插入（Dead-time insertion）
 *    为了防止上下桥臂直通短路，需要在两个互补信号之间插入死区。
 *    死区时间可以通过BDTR寄存器编程。
 *
 * 3. 刹车输入（Brake input）
 *    紧急情况下可以立即关闭所有输出，保护电路。
 *
 * 4. 重复计数器（Repetition counter）
 *   可以设置每N个更新周期才产生一次更新事件。
 *    用于三相电机的6步换相等。
 *
 * 本实验使用的是通用定时器TIM3，不涉及上述高级功能。
 * 如果后续学习电机驱动，可以继续研究TIM1高级定时器。
 *
 * ================================================================
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
