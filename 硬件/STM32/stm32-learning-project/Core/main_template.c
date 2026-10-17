/**
 ******************************************************************************
 * @file    main_template.c
 * @brief   STM32F103C8T6 主函数模板（基于HAL库）
 *          适用于蓝桥杯/最小系统板开发
 *          包含系统时钟配置、GPIO初始化、主循环等基础框架
 ******************************************************************************
 * @attention
 *
 * 本文件是STM32 HAL库工程的标准主函数模板，所有例程都基于此框架。
 * 学习时请重点理解：
 *   1. 系统时钟配置（SystemClock_Config）
 *   2. HAL库初始化流程（HAL_Init）
 *   3. 外设初始化函数（MX_XXX_Init）
 *   4. 主循环（while(1)）的编写方式
 *
 * 硬件说明（STM32F103C8T6最小系统板）：
 *   - 外部晶振：8MHz（HSE）
 *   - 系统主频：72MHz（通过PLL倍频）
 *   - LED引脚：PC13（低电平点亮，大部分最小系统板的设计）
 *   - 按键引脚：PA0（需外接上拉或下拉，根据实际硬件）
 *
 ******************************************************************************
 */

/* ================================================================
 * 头文件包含区
 * ================================================================
 * stm32f1xx_hal.h 是HAL库的总头文件，包含了所有HAL库函数的声明。
 * 它会根据 stm32f1xx_hal_conf.h 中的配置自动包含各个模块的头文件。
 * ================================================================ */
#include "stm32f1xx_hal.h"

/* ================================================================
 * 私有函数声明区
 * ================================================================
 * 这些函数在本文件中定义并使用，static关键字表示它们只能在本文件内访问。
 * 按照HAL库的命名规范，初始化函数以 MX_ 开头。
 * ================================================================ */
static void SystemClock_Config(void);    /* 系统时钟配置函数 */
static void MX_GPIO_Init(void);          /* GPIO初始化函数 */
void Error_Handler(void);                /* 错误处理函数 */

/**
 * @brief  主函数：程序的入口点
 * @retval int（嵌入式程序一般不会返回，因为是死循环）
 *
 * 主函数的执行流程：
 *   1. 复位后从启动文件（startup_stm32f103xb.s）跳转到main()
 *   2. 调用HAL_Init()初始化HAL库
 *   3. 配置系统时钟到72MHz
 *   4. 初始化所有外设（GPIO、定时器、串口等）
 *   5. 进入while(1)主循环，不断执行用户代码
 */
int main(void)
{
  /* ---------- 第1步：HAL库初始化 ----------
   * HAL_Init() 是HAL库的入口函数，它会做以下几件事：
   *   1. 配置 SysTick 为1ms中断（作为HAL时基）
   *   2. 设置 NVIC 优先级分组为 4 位抢占优先级，0 位子优先级
   *   3. 初始化一些底层硬件
   * 注意：HAL_Init() 必须在所有其他HAL函数之前调用！
   */
  HAL_Init();

  /* ---------- 第2步：配置系统时钟 ----------
   * SystemClock_Config() 将系统时钟配置为72MHz。
   * 时钟是单片机的"心跳"，所有外设的工作频率都来源于系统时钟。
   * 必须在初始化外设之前配置好系统时钟！
   */
  SystemClock_Config();

  /* ---------- 第3步：初始化外设 ----------
   * 按照需求初始化各个外设模块。
   * 这里只初始化了GPIO作为示例，实际项目中按需添加。
   */
  MX_GPIO_Init();

  /* ---------- 第4步：主循环（死循环） ----------
   * 嵌入式程序永远不会退出，所以主循环是一个无限循环。
   * 所有的业务逻辑都在这里面执行。
   */
  while (1)
  {
    /* ================================================
     * 用户代码区：在这里编写你的应用逻辑
     * ================================================
     *
     * 示例：LED闪烁（PC13引脚的LED）
     * HAL_GPIO_TogglePin() 用于翻转GPIO引脚的电平状态
     * HAL_Delay() 是毫秒级延时函数，基于SysTick实现
     *
     * HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
     * HAL_Delay(500);
     *
     * ================================================ */
  }
}

/**
 * @brief  系统时钟配置函数
 * @note   将系统时钟配置为 72MHz（基于外部8MHz晶振）
 *
 * 时钟树说明（STM32F103的时钟系统）：
 * ┌─────────┐     ┌──────┐     ┌──────────┐     ┌─────┐
 * │ HSE 8MHz │───▶│ PLL  │───▶│ SYSCLK   │───▶│ HCLK │───▶ AHB总线 = 72MHz
 * └─────────┘  ×9 └──────┘  /1 └──────────┘  /1 └─────┘
 *                                                │
 *                                                ├─▶ APB1预分频器 /2 ──▶ PCLK1 = 36MHz
 *                                                │                       (TIM2-7时钟 ×2 = 72MHz)
 *                                                └─▶ APB2预分频器 /1 ──▶ PCLK2 = 72MHz
 *                                                                        (TIM1/8时钟 ×1 = 72MHz)
 *
 * 重要知识点：
 *   - SYSCLK：系统时钟，最高72MHz
 *   - HCLK：AHB总线时钟，CPU主频
 *   - PCLK1：APB1低速总线时钟，最高36MHz
 *   - PCLK2：APB2高速总线时钟，最高72MHz
 *   - 定时器时钟：如果APB预分频系数为1，则定时器时钟等于APB时钟；
 *                 否则为APB时钟的2倍（这是一个容易混淆的点！）
 */
static void SystemClock_Config(void)
{
  /* RCC_OscInitTypeDef：振荡器初始化结构体
   * 用于配置内部/外部振荡器、PLL等时钟源
   */
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* RCC_ClkInitTypeDef：系统时钟初始化结构体
   * 用于配置系统时钟源、AHB/APB预分频器等
   */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* ========== 配置振荡器 ========== */

  /* 配置PLL时钟源为HSE（外部高速晶振） */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  /* 选择要配置的振荡器类型：HSE */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    /* 开启HSE外部晶振 */
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;     /* HSE到PLL的预分频：不分频（直接8MHz输入PLL） */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                /* 开启PLL（锁相环，用于倍频） */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        /* PLL的时钟源选择HSE */
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                /* PLL倍频系数：×9（8MHz × 9 = 72MHz） */

  /* 调用HAL库函数初始化振荡器
   * 如果初始化失败（例如HSE晶振不起振），会调用Error_Handler()
   */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 配置系统时钟和总线时钟 ========== */

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |       /* 配置HCLK（AHB总线时钟） */
                                RCC_CLOCKTYPE_SYSCLK |     /* 配置SYSCLK（系统时钟） */
                                RCC_CLOCKTYPE_PCLK1 |      /* 配置PCLK1（APB1低速总线时钟） */
                                RCC_CLOCKTYPE_PCLK2;       /* 配置PCLK2（APB2高速总线时钟） */

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 系统时钟源选择PLL输出（72MHz） */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* AHB预分频：不分频，HCLK = 72MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         /* APB1预分频：2分频，PCLK1 = 36MHz（APB1最高36MHz） */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         /* APB2预分频：不分频，PCLK2 = 72MHz */

  /* 调用HAL库函数配置系统时钟
   * 参数 FLASH_LATENCY_2 表示Flash等待周期为2个CPU周期
   * 当SYSCLK > 48MHz时，必须设置至少2个等待周期，否则读取Flash会出错
   */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  GPIO初始化函数
 * @note   配置LED引脚（PC13）为推挽输出模式
 *
 * GPIO的8种工作模式：
 *   输入模式：
 *     1. GPIO_MODE_INPUT         - 浮空输入（复位默认状态）
 *     2. GPIO_MODE_IT_RISING     - 外部中断上升沿触发
 *     3. GPIO_MODE_IT_FALLING    - 外部中断下降沿触发
 *     4. GPIO_MODE_IT_RISING_FALLING - 外部中断双边沿触发
 *   输出模式：
 *     5. GPIO_MODE_OUTPUT_PP     - 推挽输出（可输出高低电平，驱动能力强）
 *     6. GPIO_MODE_OUTPUT_OD     - 开漏输出（只能输出低电平，高电平需上拉电阻）
 *   复用模式：
 *     7. GPIO_MODE_AF_PP         - 复用推挽输出（外设控制引脚，如串口TX）
 *     8. GPIO_MODE_AF_OD         - 复用开漏输出（如I2C的SDA/SCL）
 *   模拟模式：
 *     9. GPIO_MODE_ANALOG        - 模拟模式（ADC采集时使用）
 */
static void MX_GPIO_Init(void)
{
  /* GPIO_InitTypeDef：GPIO初始化结构体
   * 包含了GPIO引脚的所有配置参数
   */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ---------- 第1步：使能GPIO端口时钟 ----------
   * STM32的所有外设都需要先使能时钟才能工作，否则读写寄存器无效。
   * 这是新手最容易犯的错误之一：忘记使能时钟！
   * __HAL_RCC_GPIOC_CLK_ENABLE() 是一个宏，用于使能GPIOC端口的时钟。
   */
  __HAL_RCC_GPIOC_CLK_ENABLE();   /* 使能GPIOC端口时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();   /* 使能GPIOA端口时钟（备用） */

  /* ---------- 第2步：配置PC13为推挽输出（LED） ----------
   * PC13连接最小系统板上的LED（低电平点亮）
   */
  GPIO_InitStruct.Pin = GPIO_PIN_13;              /* 选择要配置的引脚：Pin13 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     /* 设置为推挽输出模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;             /* 不上拉不下拉（推挽输出不需要上下拉） */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;    /* 输出速度：低速（LED不需要高速） */

  /* 调用HAL库函数初始化GPIO引脚
   * 参数1：GPIO端口（GPIOC）
   * 参数2：初始化结构体指针
   */
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* ---------- 第3步：设置LED初始状态 ----------
   * 上电后LED默认熄灭（高电平）
   * HAL_GPIO_WritePin() 用于设置GPIO引脚的电平状态
   *   GPIO_PIN_SET   - 高电平
   *   GPIO_PIN_RESET - 低电平
   */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  /* PC13输出高电平，LED熄灭 */
}

/**
 * @brief  错误处理函数
 * @note   当初始化或运行时出现严重错误时调用此函数。
 *         实际项目中可以在这里添加：
 *           - LED闪烁指示错误
 *           - 串口打印错误信息
 *           - 系统复位
 *         这里使用简单的死循环来停止程序，方便调试时发现问题。
 */
void Error_Handler(void)
{
  /* 关闭所有中断，防止中断干扰 */
  __disable_irq();

  /* 死循环：程序停在这里，表示出现了错误
   * 调试时可以在这里打断点，查看调用堆栈找到出错的位置
   */
  while (1)
  {
    /* 可以添加LED快速闪烁来指示错误状态 */
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(100);
  }
}

/* ================================================================
 * 中断回调函数（弱定义，可被用户重写）
 * ================================================================
 * HAL库使用回调机制处理中断事件。
 * 下面这些函数是"弱定义"的，也就是说如果你在其他地方重写了同名函数，
 * 编译器会使用你写的那个，而不是这里的默认实现。
 * ================================================================ */

/**
 * @brief  SysTick中断回调函数
 * @note   每1ms被调用一次（由SysTick中断服务函数调用）
 *         可以在这里实现毫秒级的定时任务。
 *         注意：此函数在中断上下文中执行，不能执行耗时操作！
 */
/*
void HAL_SYSTICK_Callback(void)
{
  // 用户可以在这里添加1ms周期的任务
}
*/

#ifdef USE_FULL_ASSERT
/**
 * @brief  断言失败处理函数
 * @param  file: 出错的文件名
 * @param  line: 出错的行号
 * @note   当USE_FULL_ASSERT宏被定义时，参数检查失败会调用此函数。
 *         调试时可以在这里打印文件名和行号，快速定位问题。
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 可以使用串口打印错误信息 */
  /* printf("Wrong parameters value: file %s on line %d\r\n", file, line); */

  /* 死循环，方便调试 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
