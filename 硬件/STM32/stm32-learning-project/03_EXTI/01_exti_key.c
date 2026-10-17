/**
 ******************************************************************************
 * @file    01_exti_key.c
 * @brief   外部中断实验 - 按键触发外部中断
 *          使用PA0引脚的外部中断线EXTI0，下降沿触发
 *          中断触发后翻转LED状态，并实现软件消抖
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐
 *   │  STM32F103   │
 *   │  PA0  ────┬──┤ 按键一端
 *   │           │  │
 *   │          10kΩ│ （上拉电阻，使用内部上拉则不需要外部）
 *   │           │  │
 *   │  3.3V ────┘  │
 *   │              │
 *   │  GND  ───────┤ 按键另一端
 *   └──────────────┘
 *
 *   按键按下时：PA0 = 低电平（GND）→ 下降沿 → 触发外部中断
 *   按键松开时：PA0 = 高电平（3.3V）→ 上升沿
 *
 *   本示例使用STM32内部上拉电阻，无需外接上拉电阻。
 *   LED使用PC13引脚（最小系统板自带）。
 *
 * EXTI引脚映射关系：
 *   GPIO_PIN_0  → EXTI0 （PA0/PB0/PC0...都共用EXTI0线）
 *   GPIO_PIN_1  → EXTI1
 *   GPIO_PIN_2  → EXTI2
 *   GPIO_PIN_3  → EXTI3
 *   GPIO_PIN_4  → EXTI4
 *   GPIO_PIN_5~9 → EXTI9_5（共用一个中断向量）
 *   GPIO_PIN_10~15 → EXTI15_10（共用一个中断向量）
 *
 *   注意：同一编号的引脚（如PA0、PB0、PC0）只能有一个使用EXTI0！
 *   通过AFIO_EXTICR寄存器选择哪个端口的引脚连接到EXTI线。
 *
 * 学习重点：
 *   1. EXTI（外部中断/事件控制器）的工作原理
 *   2. GPIO外部中断的配置步骤（GPIO + EXTI + NVIC）
 *   3. 上升沿/下降沿/双边沿触发的配置
 *   4. HAL_GPIO_EXTI_Callback() 中断回调函数
 *   5. 外部中断中的按键消抖处理
 *   6. 中断服务函数的编写规范
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* 按键引脚定义（PA0 + EXTI0） */
#define KEY_GPIO_PORT       GPIOA
#define KEY_GPIO_PIN        GPIO_PIN_0
#define KEY_IRQn            EXTI0_IRQn
#define KEY_IRQ_HANDLER     EXTI0_IRQHandler

/* LED引脚定义（PC13） */
#define LED_GPIO_PORT       GPIOC
#define LED_GPIO_PIN        GPIO_PIN_13

/* 按键消抖时间（单位：毫秒）
 * 外部中断的消抖不能用 HAL_Delay()（阻塞式延时不能在中断中调用），
 * 所以采用"时间差判断"的方式：
 *   记录上次触发中断的时间，如果两次中断间隔小于消抖时间，就忽略。
 */
#define KEY_DEBOUNCE_MS     20

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* 记录上次有效中断的时刻（用于消抖） */
uint32_t g_last_exti_time = 0;

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：按键按下触发外部中断，翻转LED状态
 *
 * 外部中断 vs 查询方式：
 *   查询方式（02_key_input.c）：主循环中不断读取引脚电平，CPU占用率高
 *   中断方式（本程序）：只有按键按下时才响应，平时CPU可以做其他事
 *
 *   中断方式的优点：
 *     1. 响应速度快（纳秒级响应）
 *     2. CPU效率高（不需要轮询）
 *     3. 可以唤醒低功耗模式
 *
 *   中断方式的缺点：
 *     1. 编程复杂度高（要考虑中断优先级、重入、共享资源等问题）
 *     2. 机械按键需要特殊的消抖处理
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ========== */
  HAL_Init();

  /* ========== 第2步：系统时钟配置（72MHz） ========== */
  SystemClock_Config();

  /* ========== 第3步：GPIO + EXTI初始化 ==========
   * 外部中断需要同时配置：
   *   1. GPIO为输入模式（带上拉/下拉）
   *   2. EXTI线的触发方式（上升沿/下降沿/双边沿）
   *   3. NVIC中断优先级和使能
   *
   * HAL库把GPIO和EXTI的配置合并到了一个函数中，
   * 只需要设置 Mode 为中断模式即可。
   */
  MX_GPIO_Init();

  /* ========== 第4步：主循环 ==========
   * 主循环中不需要轮询按键！
   * 按键按下会自动触发中断，在中断回调函数中处理。
   *
   * 主循环可以执行其他任务，比如：
   *   - 数据处理
   *   - 显示刷新
   *   - 通信协议处理
   *   - 进入低功耗模式等待中断唤醒
   */
  while (1)
  {
    /* 主循环可以处理其他事务
     * 按键响应完全由中断完成
     */

    /* 示例：这里可以放其他代码 */
  }
}

/**
 * @brief  GPIO外部中断回调函数
 * @param  GPIO_Pin: 触发中断的引脚号
 * @note   这是HAL库提供的外部中断回调函数（弱定义），
 *         所有GPIO外部中断都会调用这个函数，
 *         通过 GPIO_Pin 参数区分是哪个引脚触发的。
 *
 * 中断服务函数调用链：
 *   1. 按键按下 → PA0产生下降沿 → EXTI0检测到下降沿
 *   2. EXTI向NVIC发送中断请求
 *   3. CPU跳转到 EXTI0_IRQHandler()
 *   4. EXTI0_IRQHandler() 调用 HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0)
 *   5. HAL_GPIO_EXTI_IRQHandler() 清除中断标志
 *   6. 调用 HAL_GPIO_EXTI_Callback(GPIO_PIN_0)
 *   7. 在回调函数中执行用户代码
 *
 * 注意事项（非常重要！）：
 *   1. 中断回调函数在中断上下文中执行，不能执行耗时操作！
 *   2. 不能在中断中调用 HAL_Delay()（基于SysTick，可能死锁）
 *   3. 不能在中断中执行printf等耗时的IO操作
 *   4. 中断中访问全局变量需要注意 volatile 和 原子操作
 *   5. 中断处理函数要尽量短，一般只做：
 *      - 设置标志位
 *      - 读写少量数据
 *      - 快速的硬件操作
 *
 * 按键消抖策略（中断方式）：
 *   机械按键按下和松开时都会产生抖动，可能导致多次进入中断。
 *   中断中不能用 HAL_Delay() 延时消抖，所以用"时间差法"：
 *     - 记录每次中断的时间戳
 *     - 如果距离上次有效中断的时间小于消抖时间，忽略本次中断
 *     - 只有超过消抖时间的中断才认为是有效的按键动作
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* 判断是哪个引脚触发的中断 */
  if (GPIO_Pin == KEY_GPIO_PIN)
  {
    /* ---------- 消抖处理 ----------
     * 原理：记录上次有效中断的时间，如果两次中断间隔太短，
     *       说明是抖动产生的，直接忽略。
     *
     * HAL_GetTick() 返回SysTick的计数值（毫秒），
     * 这个函数读取的是一个全局变量，执行很快，可以在中断中调用。
     */
    uint32_t current_time = HAL_GetTick();

    if ((current_time - g_last_exti_time) < KEY_DEBOUNCE_MS)
    {
      /* 两次中断间隔小于消抖时间，认为是抖动，直接返回 */
      return;
    }

    /* 更新上次有效中断时间 */
    g_last_exti_time = current_time;

    /* ---------- 有效按键动作：翻转LED ---------- */
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
 * @note   配置PA0为外部中断下降沿触发模式，PC13为推挽输出
 *
 * GPIO外部中断配置步骤（HAL库方式）：
 *   1. 使能GPIO端口时钟 和 AFIO时钟
 *   2. 配置GPIO引脚为中断模式（GPIO_MODE_IT_xxx）
 *      同时设置上下拉电阻
 *   3. 调用 HAL_GPIO_Init()，这个函数内部会自动配置EXTI
 *   4. 配置NVIC中断优先级和使能
 *
 * HAL库的GPIO中断模式：
 *   GPIO_MODE_IT_RISING         - 上升沿触发
 *   GPIO_MODE_IT_FALLING        - 下降沿触发
 *   GPIO_MODE_IT_RISING_FALLING - 双边沿触发
 *
 * 为什么需要AFIO时钟？
 *   EXTI线和GPIO引脚的连接是通过AFIO（复用功能IO）的
 *   EXTICR寄存器来选择的。例如EXTI0可以选择PA0、PB0、PC0等。
 *   所以使用外部中断时必须使能AFIO时钟。
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ========== 第1步：使能时钟 ==========
   * GPIOA（按键）、GPIOC（LED）、AFIO（外部中断需要）
   */
  __HAL_RCC_GPIOA_CLK_ENABLE();   /* 按键GPIO时钟 */
  __HAL_RCC_GPIOC_CLK_ENABLE();   /* LED GPIO时钟 */
  __HAL_RCC_AFIO_CLK_ENABLE();    /* AFIO时钟（外部中断必须！） */

  /* ========== 第2步：配置PA0为外部中断下降沿触发 ==========
   *
   * Mode = GPIO_MODE_IT_FALLING：外部中断-下降沿触发
   *   当引脚电平从高变低（下降沿）时，触发EXTI中断。
   *
   *   可选的中断模式：
   *     GPIO_MODE_IT_RISING         - 上升沿触发（低→高）
   *     GPIO_MODE_IT_FALLING        - 下降沿触发（高→低）
   *     GPIO_MODE_IT_RISING_FALLING - 双边沿触发（两个边沿都触发）
   *
   * Pull = GPIO_PULLUP：上拉输入
   *   按键另一端接GND，按下时引脚变低。
   *   上拉电阻确保按键松开时引脚为高电平。
   *
   * 注意：当 Mode 设置为 IT_xxx 时，
   *       HAL_GPIO_Init() 内部会自动完成以下配置：
   *         1. 配置GPIO为输入模式
   *         2. 配置AFIO EXTICR寄存器，将GPIO引脚映射到EXTI线
   *         3. 配置EXTI的触发方式（上升沿/下降沿）
   *         4. 使能EXTI线（取消屏蔽）
   *       但是NVIC的配置需要手动完成！
   */
  GPIO_InitStruct.Pin = KEY_GPIO_PIN;              /* PA0 */
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;     /* 外部中断-下降沿触发 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;              /* 上拉电阻 */

  HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

  /* ========== 第3步：配置NVIC中断 ==========
   * 这一步很重要！HAL_GPIO_Init() 只配置了EXTI，
   * 但中断能否被CPU响应，还取决于NVIC的设置。
   *
   * NVIC配置两个要点：
   *   1. 设置中断优先级（抢占优先级 + 子优先级）
   *   2. 使能中断线
   *
   * STM32F103的外部中断线：
   *   EXTI0     → EXTI0_IRQn      （独立）
   *   EXTI1     → EXTI1_IRQn      （独立）
   *   EXTI2     → EXTI2_IRQn      （独立）
   *   EXTI3     → EXTI3_IRQn      （独立）
   *   EXTI4     → EXTI4_IRQn      （独立）
   *   EXTI5~9   → EXTI9_5_IRQn    （共用一个中断向量）
   *   EXTI10~15 → EXTI15_10_IRQn  （共用一个中断向量）
   *
   *   共用中断向量的意思是：EXTI5到EXTI9都触发同一个中断服务函数，
   *   在函数内部需要通过读取中断标志位来判断是哪条线触发的。
   *   HAL库已经帮我们处理好了，回调函数会传入GPIO_Pin参数。
   */
  HAL_NVIC_SetPriority(KEY_IRQn, 2, 0);   /* 设置抢占优先级为2，子优先级为0 */
  HAL_NVIC_EnableIRQ(KEY_IRQn);            /* 使能EXTI0中断 */

  /* ========== 第4步：配置PC13为推挽输出（LED） ========== */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

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

/* ================================================================
 * 知识补充：EXTI（外部中断/事件控制器）详解
 * ================================================================
 *
 * EXTI是什么？
 *   EXTI（External interrupt/event controller）是STM32的一个外设，
 *   负责管理所有GPIO引脚的外部中断和事件。
 *
 * EXTI的主要特性：
 *   1. 每个EXTI线对应一个GPIO引脚编号（Pin0~Pin15）
 *   2. 共19个EXTI线（EXTI0~EXTI18）
 *      - EXTI0~EXTI15：对应GPIO的Pin0~Pin15
 *      - EXTI16：PVD输出（可编程电压检测器）
 *      - EXTI17：RTC闹钟事件
 *      - EXTI18：USB唤醒事件
 *   3. 每条EXTI线可以独立配置触发方式
 *      - 上升沿触发
 *      - 下降沿触发
 *      - 双边沿触发
 *   4. 每条EXTI线可以独立屏蔽/使能
 *   5. 支持产生中断（Interrupt）和事件（Event）两种
 *      - 中断：会让CPU跳转到中断服务函数执行
 *      - 事件：不会中断CPU，但可以触发其他硬件（如DMA、ADC等）
 *
 * EXTI和GPIO的关系：
 *   EXTI线0~15对应GPIO的Pin0~Pin15。
 *   但是每条EXTI线同一时间只能连接到一个端口的对应引脚。
 *
 *   例如EXTI0：可以选择 PA0 或 PB0 或 PC0 ... 但只能选一个！
 *
 *   选择哪个端口由 AFIO_EXTICR 寄存器控制。
 *   HAL库的 HAL_GPIO_Init() 会自动设置这个寄存器。
 *
 *   这意味着：
 *     - 你不能同时使用 PA0 和 PB0 作为外部中断（它们共用EXTI0）
 *     - 但你可以同时使用 PA0 和 PA1 作为外部中断（它们用不同的EXTI线）
 *
 * ================================================================
 * 知识补充：中断和事件的区别
 * ================================================================
 *
 * 中断（Interrupt）：
 *   - 会打断CPU当前的执行，跳转到中断服务函数
 *   - 需要CPU参与处理
 *   - 有中断向量表，每个中断有对应的服务函数
 *   - 可以设置优先级，高优先级可以打断低优先级
 *
 * 事件（Event）：
 *   - 不会打断CPU执行
 *   - 是一个硬件信号，可以触发其他硬件模块
 *   - 例如：EXTI事件可以触发ADC开始转换、触发DMA传输等
 *   - 不需要CPU参与，完全由硬件自动完成
 *
 * 类比：
 *   中断就像有人按你家门铃，你需要停下手中的事去开门。
 *   事件就像你设置的闹钟，闹钟响了但你可以选择不理它，
 *   但闹钟可能会自动打开咖啡机（触发其他硬件动作）。
 *
 * 实际应用中，我们大多使用"中断"模式，
 * "事件"模式主要用于一些特殊的低功耗或硬件联动场景。
 *
 * ================================================================
 * 知识补充：NVIC和中断优先级
 * ================================================================
 *
 * NVIC（Nested Vectored Interrupt Controller）是嵌套向量中断控制器，
 * 它是Cortex-M内核的一部分，负责管理所有中断。
 *
 * 中断优先级分组：
 *   STM32F103有4位优先级配置位，可以分为：
 *
 *   分组方式      抢占优先级位数  子优先级位数  抢占优先级范围  子优先级范围
 *   NVIC_PRIORITYGROUP_0    0位         4位         0级          0~15级
 *   NVIC_PRIORITYGROUP_1    1位         3位         0~1级        0~7级
 *   NVIC_PRIORITYGROUP_2    2位         2位         0~3级        0~3级
 *   NVIC_PRIORITYGROUP_3    3位         1位         0~7级        0~1级
 *   NVIC_PRIORITYGROUP_4    4位         0位         0~15级       0级
 *
 *   HAL_Init() 默认设置为 NVIC_PRIORITYGROUP_4（4位抢占优先级）。
 *   这也是最常用的分组方式。
 *
 * 抢占优先级 vs 子优先级：
 *   - 抢占优先级：高抢占优先级的中断可以打断低抢占优先级的中断（中断嵌套）
 *   - 子优先级：当两个中断的抢占优先级相同时，子优先级高的先响应
 *                子优先级不能实现嵌套，只是决定同时发生时谁先执行
 *
 * 优先级数值：
 *   数值越小，优先级越高！
 *   0是最高优先级，15是最低优先级。
 *
 * 注意事项：
 *   1. 不要让所有中断都用最高优先级，否则就没有优先级之分了
 *   2. 合理规划优先级：紧急的（如电机控制、故障保护）设高优先级，
 *      不紧急的（如按键、串口打印）设低优先级
 *   3. 中断优先级分组一般在系统初始化时设置一次，之后不建议修改
 *
 * ================================================================
 * 知识补充：中断服务函数的编写原则
 * ================================================================
 *
 * 1. 快进快出（最重要！）
 *    中断服务函数要尽量短，执行时间尽量少。
 *    复杂的逻辑放到主循环中处理，中断只设置标志位。
 *
 *    好的做法：
 *      void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
 *          if (GPIO_Pin == KEY_PIN) {
 *              g_key_pressed = 1;  // 只设置标志位
 *          }
 *      }
 *      // 主循环中处理：if (g_key_pressed) { ... g_key_pressed = 0; }
 *
 *    不好的做法：
 *      void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
 *          按键消抖延时10ms     // 不能用HAL_Delay！
 *          处理按键逻辑
 *          刷新LCD显示         // 耗时操作！
 *          printf("按键按下\n"); // 超级慢！
 *      }
 *
 * 2. 共享资源保护
 *    中断和主循环都访问的变量，要加 volatile 关键字，
 *    防止编译器优化导致数据不一致。
 *    如果是多字节变量（如uint32_t），还要考虑原子操作的问题。
 *
 * 3. 不可重入函数
 *    中断中调用的函数要确保是可重入的（不依赖全局状态、不使用静态变量）。
 *    标准库的 printf、malloc 等一般是不可重入的，不要在中断中调用。
 *
 * 4. 中断中不要调用阻塞式函数
 *    HAL_Delay() 基于SysTick，如果当前中断优先级比SysTick高，
 *    SysTick无法执行，HAL_Delay()会永远等下去（死锁）。
 *
 * ================================================================
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
