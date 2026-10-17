/**
 ******************************************************************************
 * @file    01_led_blink.c
 * @brief   GPIO输出实验 - LED闪烁
 *          使用HAL库控制GPIO引脚输出高低电平，实现LED闪烁效果
 *          基于STM32F103C8T6最小系统板（PC13引脚连接LED）
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
 * 学习重点：
 *   1. GPIO推挽输出模式的配置方法
 *   2. HAL_GPIO_WritePin() 写引脚电平
 *   3. HAL_GPIO_TogglePin() 翻转引脚电平
 *   4. HAL_Delay() 毫秒级延时函数
 *
 ******************************************************************************
 */

/* 包含HAL库头文件，所有HAL库函数都在这里声明 */
#include "stm32f1xx_hal.h"

/* ================================================================
 * 函数声明
 * ================================================================
 * 按照C语言规范，函数在使用前需要先声明（或定义）。
 * 这里声明了三个函数，它们的实现在文件后面。
 * ================================================================ */
static void SystemClock_Config(void);   /* 系统时钟配置函数声明 */
static void MX_GPIO_Init(void);         /* GPIO初始化函数声明 */
void Error_Handler(void);               /* 错误处理函数声明 */

/**
 * @brief  主函数：程序入口
 * @retval int
 *
 * 主函数执行流程：
 *   1. HAL库初始化
 *   2. 系统时钟配置（72MHz）
 *   3. GPIO初始化（PC13为输出模式）
 *   4. 主循环：翻转LED电平 + 延时
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ==========
   * HAL_Init() 是HAL库的初始化函数，必须最先调用。
   * 它主要做了以下工作：
   *   1. 设置SysTick定时器为1ms中断（作为HAL库的时基）
   *   2. 设置NVIC优先级分组为4位抢占优先级
   *   3. 初始化底层硬件接口
   */
  HAL_Init();

  /* ========== 第2步：配置系统时钟 ==========
   * 将系统时钟配置为72MHz（外部8MHz晶振 + PLL ×9）。
   * 具体实现见文件末尾的 SystemClock_Config() 函数。
   */
  SystemClock_Config();

  /* ========== 第3步：初始化GPIO ==========
   * 配置PC13引脚为推挽输出模式，用于控制LED。
   * 具体实现见文件末尾的 MX_GPIO_Init() 函数。
   */
  MX_GPIO_Init();

  /* ========== 第4步：主循环 ==========
   * 嵌入式程序永远运行在死循环中。
   * 这里实现LED每隔500毫秒翻转一次，即闪烁周期为1秒。
   */
  while (1)
  {
    /* ---------- 方法一：使用 HAL_GPIO_TogglePin 翻转引脚 ----------
     * HAL_GPIO_TogglePin() 函数会自动翻转指定引脚的电平状态：
     *   如果当前是高电平，就变成低电平
     *   如果当前是低电平，就变成高电平
     *
     * 参数1：GPIO端口基地址（GPIOC表示PORTC端口）
     * 参数2：引脚编号（GPIO_PIN_13表示第13号引脚）
     *
     * 这是实现LED闪烁最简洁的方式。
     */
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  /* 翻转PC13引脚电平 */

    /* ---------- HAL_Delay() 延时函数 ----------
     * HAL_Delay() 是HAL库提供的毫秒级延时函数，基于SysTick定时器实现。
     * 参数：延时的毫秒数。
     *
     * 工作原理：
     *   SysTick定时器每1ms产生一次中断，全局变量uwTick加1。
     *   HAL_Delay(ms) 会记录当前uwTick的值，然后不断循环判断
     *   uwTick的增量是否达到了指定的毫秒数。
     *
     * 注意事项：
     *   1. HAL_Delay() 是阻塞式延时，延时期间CPU什么都做不了
     *   2. 在中断服务函数中不要调用HAL_Delay()，可能导致死锁
     *   3. 最低延时精度是1ms
     */
    HAL_Delay(500);  /* 延时500毫秒 */

    /* ========== 方法二（备选）：使用 HAL_GPIO_WritePin 分别控制 ==========
     * 下面的代码是另一种实现方式，分别设置高电平和低电平：
     *
     *   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // 低电平，LED亮
     *   HAL_Delay(500);                                         // 亮500ms
     *   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // 高电平，LED灭
     *   HAL_Delay(500);                                         // 灭500ms
     *
     * 效果和TogglePin一样，但TogglePin更简洁。
     * 如果需要精确控制亮灭时间不一样，就用WritePin。
     */
  }
}

/**
 * @brief  系统时钟配置函数
 * @note   使用外部8MHz晶振，通过PLL倍频到72MHz作为系统时钟
 *
 * 详细原理：
 *   1. 外部晶振HSE = 8MHz
 *   2. PLL（锁相环）将HSE倍频9倍：8MHz × 9 = 72MHz
 *   3. PLL输出作为系统时钟SYSCLK
 *   4. AHB总线不分频：HCLK = 72MHz（CPU主频）
 *   5. APB1总线2分频：PCLK1 = 36MHz（低速外设总线）
 *   6. APB2总线不分频：PCLK2 = 72MHz（高速外设总线）
 *
 * 为什么要这么配置？
 *   - STM32F103的最高主频是72MHz，这是性能最优的配置
 *   - APB1最高只能到36MHz，所以必须2分频
 *   - APB2最高可以到72MHz，所以不分频
 */
static void SystemClock_Config(void)
{
  /* 定义振荡器初始化结构体，并清零初始化 */
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* 定义系统时钟初始化结构体，并清零初始化 */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* ---------- 配置振荡器 ---------- */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  /* 选择配置HSE（外部高速晶振） */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    /* 开启HSE */
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;     /* HSE到PLL不分频（直通） */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                /* 开启PLL */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        /* PLL时钟源为HSE */
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                /* PLL倍频系数：9倍 */

  /* 调用HAL库函数配置振荡器，失败则进入错误处理 */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* ---------- 配置系统时钟和总线时钟 ---------- */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |          /* 配置HCLK */
                                RCC_CLOCKTYPE_SYSCLK |        /* 配置SYSCLK */
                                RCC_CLOCKTYPE_PCLK1 |         /* 配置PCLK1 */
                                RCC_CLOCKTYPE_PCLK2;          /* 配置PCLK2 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   /* 系统时钟源：PLL输出 */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          /* AHB预分频：1分频 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           /* APB1预分频：2分频 */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           /* APB2预分频：1分频 */

  /* 配置系统时钟，同时设置Flash等待周期为2（72MHz必须设置2个等待周期） */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  GPIO初始化函数
 * @note   配置PC13为推挽输出模式，用于控制LED
 *
 * GPIO配置步骤（记忆口诀：先开时钟，再配引脚）：
 *   1. 使能GPIO端口时钟（__HAL_RCC_GPIOx_CLK_ENABLE）
 *   2. 定义GPIO_InitTypeDef结构体
 *   3. 设置结构体参数（引脚号、模式、上下拉、速度）
 *   4. 调用HAL_GPIO_Init()初始化
 *
 * GPIO工作模式详解：
 *   - GPIO_MODE_OUTPUT_PP：推挽输出模式
 *     推挽输出可以输出强高电平和强低电平，驱动能力强。
 *     内部有两个MOS管：PMOS负责输出高电平，NMOS负责输出低电平。
 *     适用于LED、继电器等需要直接驱动的器件。
 *
 *   - GPIO_MODE_OUTPUT_OD：开漏输出模式
 *     开漏输出只能输出低电平（NMOS导通），高电平需要外部上拉电阻。
 *     适用于I2C总线等需要"线与"逻辑的场合。
 *
 * GPIO输出速度：
 *   - GPIO_SPEED_FREQ_LOW  ：低速（2MHz），适用于LED、按键等低速场景
 *   - GPIO_SPEED_FREQ_MEDIUM：中速（25MHz）
 *   - GPIO_SPEED_FREQ_HIGH ：高速（50MHz），适用于SPI等高速通信
 *   速度越高，输出边沿越陡，但电磁干扰（EMI）也越大。
 *   一般应用选低速即可，既省电又减少干扰。
 */
static void MX_GPIO_Init(void)
{
  /* 定义GPIO初始化结构体变量，初始化为0
   * 这是一个好习惯，避免结构体中未初始化的成员造成不可预期的行为
   */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ---------- 第1步：使能GPIOC端口时钟 ----------
   * 重要！STM32的所有外设都需要先使能时钟才能工作。
   * 如果忘记使能时钟，操作寄存器将没有任何反应。
   *
   * __HAL_RCC_GPIOC_CLK_ENABLE() 是一个宏定义，展开后就是：
   *   RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
   * 即把APB2外设使能寄存器中的IOPCEN位置1，使能GPIOC的时钟。
   *
   * 各GPIO端口挂在哪条总线上：
   *   - GPIOA、GPIOB、GPIOC、GPIOD、GPIOE → APB2总线（高速）
   *   - 注意：F1系列的GPIO都在APB2上，F4系列不同
   */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* ---------- 第2步：配置PC13引脚 ----------
   * Pin：选择要配置的引脚，可以用 | 同时配置多个引脚
   *      例如：GPIO_PIN_0 | GPIO_PIN_1 表示同时配置Pin0和Pin1
   *
   * Mode：GPIO工作模式
   *   GPIO_MODE_OUTPUT_PP = 推挽输出
   *   GPIO_MODE_OUTPUT_OD = 开漏输出
   *   GPIO_MODE_INPUT     = 输入模式
   *   GPIO_MODE_AF_PP     = 复用推挽
   *   GPIO_MODE_AF_OD     = 复用开漏
   *   GPIO_MODE_ANALOG    = 模拟模式
   *
   * Pull：上下拉电阻配置
   *   GPIO_NOPULL  = 不上拉不下拉
   *   GPIO_PULLUP  = 上拉（输出模式下一般不需要）
   *   GPIO_PULLDOWN= 下拉（输出模式下一般不需要）
   *
   * Speed：输出速度
   *   GPIO_SPEED_FREQ_LOW    = 低速（约2MHz）
   *   GPIO_SPEED_FREQ_MEDIUM = 中速（约25MHz）
   *   GPIO_SPEED_FREQ_HIGH   = 高速（约50MHz）
   */
  GPIO_InitStruct.Pin = GPIO_PIN_13;              /* 配置第13号引脚 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     /* 推挽输出模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;             /* 不需要上下拉电阻 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    /* 低速输出（LED不需要高速） */

  /* ---------- 第3步：调用初始化函数 ----------
   * HAL_GPIO_Init() 函数会根据传入的结构体参数，
   * 自动配置GPIO的CRL（配置低寄存器）或CRH（配置高寄存器）。
   *
   * Pin0-Pin7 由 CRL 寄存器控制（每引脚4位）
   * Pin8-Pin15 由 CRH 寄存器控制（每引脚4位）
   *
   * PC13属于Pin13，所以配置的是GPIOC->CRH寄存器的第20-23位。
   */
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* ---------- 第4步：设置初始电平 ----------
   * 上电后让LED默认熄灭（高电平）。
   *
   * HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState)
   *   - GPIOx：端口（GPIOA/GPIOB/GPIOC...）
   *   - GPIO_Pin：引脚号（GPIO_PIN_0 ~ GPIO_PIN_15）
   *   - PinState：引脚状态
   *       GPIO_PIN_SET   = 高电平（1）
   *       GPIO_PIN_RESET = 低电平（0）
   *
   * 这个函数实际上操作的是 BSRR 寄存器（位设置/清除寄存器）。
   * 写BSRR是原子操作，不会被中断打断，比直接读改写ODR更安全。
   */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  /* PC13输出高电平，LED熄灭 */
}

/**
 * @brief  错误处理函数
 * @note   当初始化失败时调用，程序进入死循环
 *         实际项目中可以添加LED报警、串口打印等
 */
void Error_Handler(void)
{
  /* 关闭总中断 */
  __disable_irq();

  /* 死循环，程序停留在这里表示出错了
   * 调试时可以在这里设置断点，查看调用栈找到错误位置
   */
  while (1)
  {
    /* LED快速闪烁表示错误 */
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(100);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
