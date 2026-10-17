/**
 ******************************************************************************
 * @file    01_adc_single.c
 * @brief   ADC单通道采集实验
 *          使用HAL库的ADC接口，实现单通道单次转换模式的电压采集
 *          基于STM32F103C8T6最小系统板（ADC1 CH0 PA0）
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐
 *   │  STM32F103   │
 *   │              │
 *   │  PA0  IN0 ───┼─────────────── 模拟输入电压 (0 ~ 3.3V)
 *   │              │
 *   │              │          ┌───────────────┐
 *   │              │          │  分压电路      │
 *   │              │  (可选)   │  VIN ── R1 ──┼── PA0
 *   │              │          │               │
 *   │              │          │              R2
 *   │              │          │               │
 *   │  GND  ───────┴──────────┴───────────────┴── GND
 *   └──────────────┘
 *
 *   注意：
 *     1. ADC输入电压范围：0 ~ VDDA（通常等于3.3V）
 *     2. 输入电压不能超过VDDA，也不能低于0V，否则会损坏芯片
 *     3. 如果测量高于3.3V的电压，需要用电阻分压
 *     4. STM32F103C8T6的ADC参考电压=VDDA=3.3V
 *     5. 建议在ADC输入端加一个104电容到GND滤波
 *
 *   STM32F103C8T6的ADC通道与引脚对应：
 *     ADC1_IN0  = PA0    ADC1_IN8  = PB0
 *     ADC1_IN1  = PA1    ADC1_IN9  = PB1
 *     ADC1_IN2  = PA2    ADC1_IN10 = PC0
 *     ADC1_IN3  = PA3    ADC1_IN11 = PC1
 *     ADC1_IN4  = PA4    ADC1_IN12 = PC2
 *     ADC1_IN5  = PA5    ADC1_IN13 = PC3
 *     ADC1_IN6  = PA6    ADC1_IN14 = PC4
 *     ADC1_IN7  = PA7    ADC1_IN15 = PC5
 *     注意：ADC1和ADC2的通道引脚是共享的
 *
 * 学习重点：
 *   1. ADC的工作原理（逐次逼近型SAR ADC）
 *   2. 单通道单次转换模式的配置方法
 *   3. HAL_ADC_Start() / HAL_ADC_GetValue() 函数使用
 *   4. ADC采样值到实际电压的换算公式
 *   5. DMA方式ADC采集的原理（补充说明）
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* ADC外设定义（ADC1） */
#define ADC_HANDLE              hadc1

/* ADC通道（通道0 = PA0） */
#define ADC_CHANNEL             ADC_CHANNEL_0

/* ADC参考电压（单位：mV）
 * STM32F103的ADC参考电压等于VDDA，一般是3.3V
 * 这个值需要根据实际硬件校准
 */
#define ADC_VREF_MV             3300.0f   /* 3.3V = 3300mV */

/* ADC分辨率
 * STM32F103的ADC是12位的，最大值为 2^12 - 1 = 4095
 */
#define ADC_MAX_VALUE           4095.0f

/* LED引脚（PC13，状态指示） */
#define LED_GPIO_PORT           GPIOC
#define LED_GPIO_PIN            GPIO_PIN_13

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* ADC句柄 */
ADC_HandleTypeDef hadc1;

/* 采集结果存储 */
uint16_t g_adc_value = 0;        /* ADC原始采样值（0~4095） */
float g_voltage_mv = 0;          /* 换算后的电压值（mV） */

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
void Error_Handler(void);

/* ADC操作函数声明 */
uint16_t ADC_GetSingleValue(void);
float ADC_ValueToVoltage(uint16_t adc_value);

/**
 * @brief  主函数：程序入口
 * @retval int
 *
 * 实验流程：
 *   1. 系统初始化
 *   2. 主循环中周期性采集ADC
 *   3. 将采样值换算为实际电压
 *   4. 用LED闪烁表示电压等级（越高闪越快）
 *      - 电压 < 1V：  慢闪（1秒一次）
 *      - 1V ~ 2V：    中速（500ms一次）
 *      - 2V ~ 3V：    快速（200ms一次）
 *      - 电压 > 3V：  极快（100ms一次）
 */
int main(void)
{
  /* ========== 第1步：系统初始化 ========== */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();

  /* ========== 第2步：主循环 ========== */
  while (1)
  {
    /* ---------- 采集一次ADC ---------- */
    g_adc_value = ADC_GetSingleValue();

    /* ---------- 换算为电压 ---------- */
    g_voltage_mv = ADC_ValueToVoltage(g_adc_value);

    /* ---------- 根据电压等级控制LED闪烁速度 ----------
     * 这里用不同的延时来模拟不同的闪烁频率
     * 实际项目中可以用定时器实现更精确的闪烁
     */
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);

    if (g_voltage_mv < 1000)       /* < 1V */
    {
      HAL_Delay(500);   /* 慢闪：周期1秒 */
    }
    else if (g_voltage_mv < 2000)  /* 1V ~ 2V */
    {
      HAL_Delay(250);   /* 中速：周期500ms */
    }
    else if (g_voltage_mv < 3000)  /* 2V ~ 3V */
    {
      HAL_Delay(100);   /* 快速：周期200ms */
    }
    else                            /* > 3V */
    {
      HAL_Delay(50);    /* 极快：周期100ms */
    }
  }
}

/* ================================================================
 * ADC操作函数
 * ================================================================ */

/**
 * @brief  获取一次ADC单通道转换结果
 * @retval 16位ADC采样值（0~4095）
 *
 * 单次转换流程：
 *   1. 启动ADC转换（HAL_ADC_Start）
 *   2. 等待转换完成（HAL_ADC_PollForConversion）
 *   3. 读取转换结果（HAL_ADC_GetValue）
 *
 * 注意：
 *   - 这是阻塞式采集，等待期间CPU不能做其他事
 *   - 适合低频、简单的采集场景
 *   - 高频采集建议使用DMA方式
 */
uint16_t ADC_GetSingleValue(void)
{
  /* ---------- 第1步：启动ADC转换 ----------
   * HAL_ADC_Start() 启动ADC的规则组转换。
   * 启动后ADC开始对配置好的通道进行采样和转换。
   */
  if (HAL_ADC_Start(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /* ---------- 第2步：等待转换完成 ----------
   * HAL_ADC_PollForConversion() 轮询等待转换完成。
   *
   * 参数：
   *   hadc    - ADC句柄
   *   Timeout - 超时时间（毫秒）
   *
   * ADC转换时间 = 采样时间 + 12.5个ADC时钟周期
   * 假设ADC时钟=12MHz，采样时间=55.5周期：
   *   总转换时间 = (55.5 + 12.5) / 12MHz ≈ 5.67us
   * 非常快，所以10ms超时绰绰有余。
   */
  if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
  {
    Error_Handler();
  }

  /* ---------- 第3步：读取转换结果 ----------
   * HAL_ADC_GetValue() 读取ADC转换结果寄存器（DR）的值。
   * 返回值是16位的，对于12位ADC，范围是0 ~ 4095。
   */
  return HAL_ADC_GetValue(&hadc1);
}

/**
 * @brief  ADC采样值转换为实际电压（mV）
 * @param  adc_value: ADC原始采样值（0~4095）
 * @retval 实际电压值，单位mV
 *
 * 换算公式：
 *   V = (ADC_Value / 4095) × V_REF
 *
 *   其中：
 *     V         = 实际输入电压
 *     ADC_Value = ADC采样值（0~4095）
 *     V_REF     = 参考电压（3.3V）
 *
 * 原理：
 *   ADC是逐次逼近型，把模拟电压量化成数字值。
 *   12位ADC有4096个量化等级（0~4095）。
 *   当输入电压=0V时，ADC值=0
 *   当输入电压=V_REF时，ADC值=4095
 *   输入电压和ADC值成正比关系。
 *
 * 注意：
 *   这个公式假设参考电压精确等于3.3V。
 *   实际中参考电压可能有误差（±1%或更大），
 *   精确测量需要校准参考电压。
 */
float ADC_ValueToVoltage(uint16_t adc_value)
{
  return ((float)adc_value / ADC_MAX_VALUE) * ADC_VREF_MV;
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
 * @note   PA0为模拟输入（ADC），PC13为输出（LED）
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能GPIOA和GPIOC时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* ---------- 配置PA0为模拟模式（ADC输入） ----------
   * ADC采集时，引脚必须配置为模拟输入模式（GPIO_MODE_ANALOG）。
   *
   * 为什么要模拟模式？
   *   - 数字输入电路（施密特触发器）会对模拟信号产生干扰
   *   - 模拟模式下，数字部分断开，减少干扰
   *   - 同时也可以省电（数字电路不工作）
   *
   * 模拟模式下：
   *   - 上下拉电阻无效
   *   - 输出速度无效
   *   - 引脚完全由模拟外设（ADC）控制
   */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;     /* 模拟模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;          /* 模拟模式下无效 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ---------- 配置PC13为推挽输出（LED） ---------- */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始熄灭 */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  ADC1初始化函数
 * @note   配置ADC1为单通道单次转换模式，12位分辨率
 *
 * STM32F103 ADC简介：
 *   - 12位分辨率（2^12 = 4096级）
 *   - 最多18个通道（16个外部通道 + 2个内部通道）
 *   - 支持单次转换、连续转换、扫描转换等模式
 *   - 支持DMA传输
 *   - 输入范围：0 ~ VDDA（3.3V）
 *
 * ADC时钟配置：
 *   ADC时钟来自APB2（PCLK2 = 72MHz），经过ADC预分频器分频。
 *   ADC最大时钟频率是14MHz（F1系列）。
 *   可选的分频系数：2/4/6/8
 *     72/2 = 36MHz（超过14MHz，不行）
 *     72/4 = 18MHz（还是超了）
 *     72/6 = 12MHz（可以，常用）
 *     72/8 = 9MHz（也可以，更稳定）
 *   本示例用6分频 = 12MHz
 *
 * 采样时间选择：
 *   ADC每次转换需要先对输入电容充电（采样），然后再转换。
 *   采样时间越长，精度越高，但速度越慢。
 *   可选采样周期：1.5/7.5/13.5/28.5/41.5/55.5/71.5/239.5 个ADC时钟周期
 *
 *   总转换时间 = 采样时间 + 12.5个周期（12位固定转换时间）
 *   例如：55.5 + 12.5 = 68个周期 = 68/12MHz ≈ 5.67us
 *
 *   输入阻抗越高，需要的采样时间越长。
 *   一般应用选55.5个周期比较折中。
 */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /* ---------- 第1步：使能ADC时钟 ----------
   * ADC1挂在APB2总线上
   */
  __HAL_RCC_ADC1_CLK_ENABLE();

  /* ---------- 第2步：配置ADC基本参数 ----------
   * ADC初始化结构体参数说明：
   *
   * DataAlign：数据对齐方式
   *   ADC_DATAALIGN_RIGHT = 右对齐（数据存在低12位）
   *   ADC_DATAALIGN_LEFT  = 左对齐（数据存在高12位）
   *   一般用右对齐，读取后直接用，不需要移位
   *
   * ScanConvMode：扫描模式
   *   DISABLE = 单通道模式（只转换一个通道）
   *   ENABLE  = 扫描模式（依次转换多个通道）
   *   本示例只有一个通道，所以禁用扫描
   *
   * ContinuousConvMode：连续转换模式
   *   DISABLE = 单次转换（转一次就停）
   *   ENABLE  = 连续转换（转完一次自动开始下一次）
   *   本示例用单次转换模式（每次手动启动）
   *
   * NbrOfConversion：规则组转换通道数
   *   单通道模式下设为1
   *
   * DiscontinuousConvMode：间断模式（禁用）
   *
   * ExternalTrigConv：外部触发源
   *   不需要外部触发，用软件启动，设为软件触发
   */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;          /* 禁用扫描（单通道） */
  hadc1.Init.ContinuousConvMode = DISABLE;             /* 单次转换模式 */
  hadc1.Init.DiscontinuousConvMode = DISABLE;          /* 禁用语断模式 */
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;    /* 软件触发启动 */
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;          /* 数据右对齐 */
  hadc1.Init.NbrOfConversion = 1;                      /* 1个转换通道 */

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /* ---------- 第3步：配置ADC通道 ----------
   * 配置规则组的第1个通道（也是唯一一个）
   *
   * Channel：ADC通道号
   *   ADC_CHANNEL_0 ~ ADC_CHANNEL_17
   *   通道0~15是外部通道，通道16是内部温度传感器，通道17是内部参考电压
   *
   * Rank：规则组中的序号（第几个转换）
   *   单通道模式下设为1（第一个，也是唯一一个）
   *
   * SamplingTime：采样时间
   *   ADC_SAMPLETIME_1CYCLES_5     = 1.5个周期
   *   ADC_SAMPLETIME_7CYCLES_5     = 7.5个周期
   *   ADC_SAMPLETIME_13CYCLES_5    = 13.5个周期
   *   ADC_SAMPLETIME_28CYCLES_5    = 28.5个周期
   *   ADC_SAMPLETIME_41CYCLES_5    = 41.5个周期
   *   ADC_SAMPLETIME_55CYCLES_5    = 55.5个周期
   *   ADC_SAMPLETIME_71CYCLES_5    = 71.5个周期
   *   ADC_SAMPLETIME_239CYCLES_5   = 239.5个周期
   *
   *   选55.5个周期，兼顾精度和速度
   */
  sConfig.Channel = ADC_CHANNEL_0;                     /* 通道0（PA0） */
  sConfig.Rank = ADC_REGULAR_RANK_1;                   /* 规则组第1位 */
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;    /* 采样时间55.5周期 */

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* ---------- 第4步：ADC校准（非常重要！） ----------
   * STM32F103的ADC出厂时有一定误差，需要校准。
   * 校准会消除内部电路的偏移误差，提高精度。
   *
   * 注意：
   *   - 校准必须在ADC上电后、开始转换前进行
   *   - 校准过程大约需要几十个ADC时钟周期
   *   - 校准值保存在ADC_DR中，掉电后丢失，每次上电都要重新校准
   */
  HAL_ADCEx_Calibration_Start(&hadc1);
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
 * 知识补充：ADC深入理解
 * ================================================================
 *
 * 一、ADC基本原理
 *
 * ADC（Analog-to-Digital Converter，模数转换器）
 * 把连续变化的模拟电压转换成离散的数字值。
 *
 * STM32F103使用的是SAR型ADC（逐次逼近型）：
 *   1. 采样：对输入电压进行采样，保持在内部电容上
 *   2. 比较：用内部DAC产生一个电压，和输入电压比较
 *   3. 逐次逼近：从最高位到最低位，一位一位地确定结果
 *   4. 输出：最终得到N位的数字量
 *
 * 二、重要概念
 *
 * 1. 分辨率（Resolution）
 *    ADC能分辨的最小电压变化量。
 *    12位ADC有4096个等级（2^12）。
 *    参考电压3.3V时：
 *      1 LSB = 3.3V / 4096 ≈ 0.806mV
 *    即输入电压变化0.8mV，ADC值变化1。
 *
 * 2. 精度（Accuracy）
 *    ADC测量值和真实值的偏差。
 *    注意：分辨率高不代表精度高！
 *    精度受参考电压误差、偏移误差、增益误差、非线性等影响。
 *    STM32F103的ADC总误差一般在±2~5 LSB左右。
 *
 * 3. 采样率（Sampling Rate）
 *    每秒能进行多少次转换。
 *    STM32F103 ADC最高采样率约1MHz（12位、1.5周期采样）。
 *
 * 4. 奈奎斯特定理
 *    采样频率必须大于信号最高频率的2倍，才能完整还原信号。
 *    例如：要采样1kHz的信号，采样率至少要2kHz。
 *
 * 三、ADC的工作模式
 *
 * 1. 单次转换模式（本示例使用）
 *    - 启动后转换一次，转换完就停止
 *    - 每次都要手动启动
 *    - 适合低频、按需采集的场景
 *
 * 2. 连续转换模式
 *    - 启动后一直不停，转换完一次自动开始下一次
 *    - 适合需要连续监测的场景
 *    - 一般配合DMA使用
 *
 * 3. 扫描模式
 *    - 依次转换多个通道
 *    - 可以一次启动，自动转换多个通道
 *    - 适合多通道采集
 *
 * 4. 间断模式
 *    - 扫描模式下，每次只转换几个通道
 *    - 可以分批完成所有通道的转换
 *
 * 四、DMA方式ADC采集（补充说明）
 *
 * 为什么要用DMA？
 *   用CPU轮询的方式采集ADC，效率很低：
 *     - 每次采集都要CPU参与
 *     - 采集期间CPU不能干别的
 *     - 高频采集时CPU占用率很高
 *
 * DMA（直接存储器访问）的好处：
 *   - ADC转换完成后，DMA自动把数据搬到内存
 *   - 整个过程不需要CPU干预
 *   - CPU可以同时做其他事情
 *   - 适合高速、连续的数据采集
 *
 * DMA方式ADC采集的基本流程：
 *   1. 配置DMA：外设地址=ADC_DR，内存地址=缓冲区，方向=外设到内存
 *   2. 配置ADC：连续转换模式 + DMA使能
 *   3. 启动ADC和DMA
 *   4. ADC不断转换，DMA不断搬运数据到内存
 *   5. 缓冲区满了触发DMA中断，通知CPU处理
 *
 * 常用HAL函数：
 *   HAL_ADC_Start_DMA()    // 启动ADC + DMA
 *   HAL_ADC_Stop_DMA()     // 停止ADC + DMA
 *   HAL_ADC_ConvCpltCallback()  // 转换完成回调函数
 *
 * 五、提高ADC精度的方法
 *
 * 1. 硬件方面：
 *    - VDDA和VSSA单独走线，和数字电源分开
 *    - 在VREF+引脚加滤波电容（104 + 10uF）
 *    - 模拟输入引脚加RC滤波
 *    - PCB布线时模拟地和数字地单点连接
 *
 * 2. 软件方面：
 *    - 多次采样取平均值（滑动滤波）
 *    - 去掉最大值和最小值再平均（防脉冲干扰）
 *    - 校准偏移误差和增益误差
 *    - 使用内部参考电压（VREFINT）校准
 *
 * 3. 采样时间：
 *    - 输入信号源阻抗高时，要加长采样时间
 *    - 一般来说，采样时间越长，精度越高
 *
 * 六、常见问题
 *
 * 1. ADC值跳变很大，不稳定？
 *    - 检查参考电压是否稳定
 *    - 检查输入信号是否有噪声（加电容滤波）
 *    - 检查电源纹波是否太大
 *    - 可以多次采样取平均
 *
 * 2. ADC读数偏低或偏高？
 *    - 检查参考电压是否准确（3.3V是否真的是3.3V）
 *    - 做ADC校准（HAL_ADCEx_Calibration_Start）
 *    - 检查分压电阻阻值是否正确
 *
 * 3. 采样速度上不去？
 *    - 减少采样时间（注意保证精度）
 *    - 使用DMA方式（减少CPU开销）
 *    - 使用连续转换模式
 *
 * ================================================================ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
