/**
 ******************************************************************************
 * @file    01_uart_send.c
 * @brief   串口发送实验 - USART1发送字符串和格式化输出
 *          使用USART1（PA9 TX / PA10 RX），波特率9600
 *          实现字符串发送、数组发送、printf重定向
 *          基于STM32F103C8T6最小系统板
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐         ┌──────────────┐
 *   │  STM32F103   │         │ USB转TTL模块  │
 *   │              │         │              │
 *   │  PA9  (TX) ──┼─────────┼── RX         │
 *   │  PA10 (RX) ──┼─────────┼── TX         │
 *   │              │         │              │
 *   │  GND  ───────┼─────────┼── GND        │
 *   │  3.3V ───────┼─────────┼── 3.3V       │
 *   └──────────────┘         └──────────────┘
 *
 *   注意：TX接RX，RX接TX（交叉连接）！
 *        一定要共地（GND接GND），否则电平参考不同会通信失败。
 *        建议使用3.3V供电的USB转TTL模块，避免5V烧坏STM32。
 *
 *   常见USB转TTL芯片：CH340、CP2102、PL2303、FT232
 *
 * 串口通信参数（本示例）：
 *   波特率：9600
 *   数据位：8位
 *   停止位：1位
 *   校验位：无
 *   流控制：无
 *   即：9600-8-N-1（最常用的配置）
 *
 * 学习重点：
 *   1. USART/UART串口通信的基本原理
 *   2. HAL_UART_Init() 串口初始化
 *   3. HAL_UART_Transmit() 阻塞式发送
 *   4. printf重定向（重写fputc函数）
 *   5. 波特率的概念和计算
 *   6. 串口调试助手的使用
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"
#include <stdio.h>    /* 为了使用 printf */
#include <string.h>   /* 为了使用 strlen */

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* USART1 引脚定义 */
#define UART_INSTANCE           USART1
#define UART_TX_GPIO_PORT       GPIOA
#define UART_TX_PIN             GPIO_PIN_9
#define UART_RX_GPIO_PORT       GPIOA
#define UART_RX_PIN             GPIO_PIN_10

/* LED引脚定义（PC13） */
#define LED_GPIO_PORT       GPIOC
#define LED_GPIO_PIN        GPIO_PIN_13

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* UART句柄 */
UART_HandleTypeDef huart1;

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void Error_Handler(void);

/* printf重定向需要的函数声明 */
int fputc(int ch, FILE *f);

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：串口发送各种格式的数据，实现printf格式化输出
 *
 * 演示内容：
 *   1. 发送字符串
 *   2. 发送数字和变量（printf格式化）
 *   3. 发送数组数据
 *   4. 实现简易的日志输出功能
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ========== */
  HAL_Init();

  /* ========== 第2步：系统时钟配置（72MHz） ========== */
  SystemClock_Config();

  /* ========== 第3步：GPIO初始化 ========== */
  MX_GPIO_Init();

  /* ========== 第4步：USART1初始化 ========== */
  MX_USART1_UART_Init();

  /* ========== 第5步：发送启动信息 ==========
   * 程序启动时通过串口打印欢迎信息，
   * 方便我们知道程序已经正常运行。
   */

  /* 方法一：直接发送字符串（使用HAL库函数）
   * HAL_UART_Transmit(huart, pData, Size, Timeout)
   *   - huart：UART句柄指针
   *   - pData：要发送的数据缓冲区指针
   *   - Size：要发送的字节数
   *   - Timeout：超时时间（毫秒），发送时间超过这个值就放弃
   *
   * 返回值：
   *   HAL_OK      - 发送成功
   *   HAL_ERROR   - 发送出错
   *   HAL_BUSY    - 串口忙
   *   HAL_TIMEOUT - 发送超时
   *
   * 注意：HAL_UART_Transmit() 是阻塞式发送函数，
   *       发送完成（或超时）之前函数不会返回。
   */
  char welcome_msg[] = "\r\n========== STM32 UART 发送实验 ==========\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)welcome_msg, strlen(welcome_msg), 100);

  /* 方法二：使用 printf 格式化输出（需要重定向fputc）
   * printf 是C标准库函数，功能强大，可以输出各种格式。
   * 默认情况下 printf 输出到控制台（屏幕），
   * 在嵌入式中我们需要把它重定向到串口。
   *
   * 重定向方法：重写 fputc 函数（见文件末尾）。
   *
   * printf 的格式控制符：
   *   %d  - 十进制整数
   *   %x  - 十六进制整数（小写）
   *   %X  - 十六进制整数（大写）
   *   %f  - 浮点数
   *   %c  - 单个字符
   *   %s  - 字符串
   *   %u  - 无符号十进制整数
   *   %ld - 长整数
   *   %%  - 输出一个%号
   *
   * 注意：
   *   1. 使用 printf 需要包含 <stdio.h>
   *   2. 有些编译器需要勾选 "Use MicroLIB" 选项才能使用 printf
   *   3. printf 是阻塞式的，而且比较慢，不要在中断中调用
   *   4. 字符串结尾一定要有 '\0' 结束符
   */
  printf("系统启动成功！\r\n");
  printf("芯片型号：STM32F103C8T6\r\n");
  printf("系统主频：%d MHz\r\n", 72);
  printf("串口配置：9600-8-N-1\r\n");
  printf("\r\n");

  /* 计数变量 */
  uint32_t counter = 0;

  /* ========== 第6步：主循环 ========== */
  while (1)
  {
    /* 计数器自增 */
    counter++;

    /* ---- 演示1：输出变量值 ---- */
    printf("[%lu] 计数器值：%lu (0x%08lX)\r\n", counter, counter, counter);

    /* ---- 演示2：输出浮点数 ----
     * 注意：默认情况下STM32的printf可能不支持浮点数，
     * 需要在工程设置中勾选 "Use float with printf from newlib-nano"
     * 如果不支持，可以用整数模拟：
     *   例如 3.14 可以输出为 "3.14"（整数部分+小数部分分开输出）
     */
    float temperature = 25.5f + counter * 0.1f;
    printf("    模拟温度：%.1f ℃\r\n", temperature);

    /* ---- 演示3：发送二进制数组 ----
     * 假设我们有一个传感器数据包，包含4个字节
     * 用 HAL_UART_Transmit 直接发送原始数据
     */
    uint8_t sensor_data[4] = {0xAA, 0x01, (uint8_t)(counter & 0xFF), 0x55};
    printf("    原始数据：");
    for (int i = 0; i < 4; i++)
    {
      printf("%02X ", sensor_data[i]);
    }
    printf("\r\n");

    printf("\r\n");  /* 空行分隔 */

    /* 翻转LED，表示程序运行正常 */
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);

    /* 延时1秒 */
    HAL_Delay(1000);
  }
}

/**
 * @brief  printf重定向函数
 * @param  ch: 要输出的字符
 * @param  f: 文件指针（标准输出）
 * @retval int: 输出的字符
 * @note   重写C标准库的fputc函数，将printf的输出重定向到USART1。
 *         这样就可以像在PC上一样使用printf函数了。
 *
 * 原理：
 *   printf() 函数内部会调用 fputc() 来输出每一个字符。
 *   fputc() 是一个弱定义的函数，我们重写它之后，
 *   编译器就会使用我们写的这个版本，而不是标准库的版本。
 *
 * 注意事项：
 *   1. 函数名和参数必须严格一致，否则不会被调用
 *   2. 这里使用 HAL_UART_Transmit() 发送单个字符
 *   3. 因为printf可能输出很多字符，所以这个函数会被频繁调用
 *   4. 发送失败时也直接返回，避免printf卡住
 */
int fputc(int ch, FILE *f)
{
  /* 注意：参数 ch 是 int 类型，但实际是一个字符（ASCII码）
   * 需要强制转换为 uint8_t 传给 HAL_UART_Transmit
   */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);

  return ch;
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
 * @note   配置PA9为复用推挽（USART1_TX），PA10为浮空输入（USART1_RX）
 *         PC13为推挽输出（LED）
 *
 * USART引脚配置说明：
 *
 * TX引脚（发送）：
 *   - 模式：GPIO_MODE_AF_PP（复用推挽输出）
 *   - 引脚由USART外设控制，输出串口波形
 *   - 需要高速模式（串口波特率较高时）
 *
 * RX引脚（接收）：
 *   - 模式：GPIO_MODE_INPUT（输入模式）
 *   - 或者：GPIO_MODE_AF_INPUT（复用输入，F1系列用浮空输入即可）
 *   - 上下拉：一般浮空输入，或者根据需要上拉/下拉
 *   - F1系列中，RX引脚配置为普通输入模式即可，不需要复用模式
 *
 * STM32F103的USART引脚：
 *   USART1：TX=PA9， RX=PA10 （APB2总线，时钟72MHz）
 *   USART2：TX=PA2， RX=PA3   （APB1总线，时钟36MHz）
 *   USART3：TX=PB10，RX=PB11  （APB1总线，时钟36MHz）
 *
 *   部分引脚支持重映射（通过AFIO寄存器）。
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();   /* GPIOA时钟（PA9/PA10用） */
  __HAL_RCC_GPIOC_CLK_ENABLE();   /* GPIOC时钟（LED用） */
  __HAL_RCC_AFIO_CLK_ENABLE();    /* AFIO时钟（复用功能需要） */

  /* ---------- 配置PA9（USART1_TX）为复用推挽输出 ---------- */
  GPIO_InitStruct.Pin = UART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       /* 复用推挽输出 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
  HAL_GPIO_Init(UART_TX_GPIO_PORT, &GPIO_InitStruct);

  /* ---------- 配置PA10（USART1_RX）为浮空输入 ----------
   * 注意：F1系列的RX引脚不需要配置为复用模式，
   *      普通的输入模式就可以。
   *      浮空输入即可，也可以根据需要配置为上拉。
   */
  GPIO_InitStruct.Pin = UART_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;       /* 输入模式 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;           /* 浮空（无上拉下拉） */
  HAL_GPIO_Init(UART_RX_GPIO_PORT, &GPIO_InitStruct);

  /* ---------- 配置PC13为推挽输出（LED） ---------- */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始状态：熄灭 */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  USART1初始化函数
 * @note   配置USART1为9600波特率，8位数据，1位停止，无校验
 *
 * USART vs UART：
 *   UART（Universal Asynchronous Receiver/Transmitter）：
 *     通用异步收发器，只有异步通信。
 *
 *   USART（Universal Synchronous/Asynchronous Receiver/Transmitter）：
 *     通用同步/异步收发器，支持同步和异步两种模式。
 *     同步模式需要额外的时钟线（SCLK）。
 *
 *   STM32F103有3个USART（USART1/2/3）和2个UART（UART4/5）。
 *   我们平时说的"串口"一般指异步模式（UART模式）。
 *
 * 异步串口通信协议：
 *
 *   空闲  起始位  数据位（LSB先传）     停止位  空闲
 *   ────┐       ┌─┬─┬─┬─┬─┬─┬─┬─┐        ┌────
 *       │       │ │ │ │ │ │ │ │ │        │
 *       └───────┘ └─┘ └─┘ └─┘ └────────┘
 *
 *       ← 1位 → ←──── 8位 ────→ ← 1位 →
 *
 *   - 空闲状态：数据线保持高电平
 *   - 起始位：1位低电平，表示一帧数据的开始
 *   - 数据位：5~9位数据，低位在前（LSB first）
 *   - 校验位：可选（奇校验/偶校验/无校验）
 *   - 停止位：1位或2位高电平，表示一帧数据的结束
 *
 * 波特率（Baud Rate）：
 *   表示每秒传输的二进制位数，单位是bps（bit per second）。
 *   常用波特率：9600, 19200, 38400, 57600, 115200, 230400...
 *
 *   9600波特率 = 每秒传输9600位
 *   每帧数据 = 1起始 + 8数据 + 1停止 = 10位
 *   每秒传输字节数 ≈ 9600 / 10 = 960 字节/秒
 *
 * 波特率计算（STM32 USART）：
 *   USARTDIV = fCK / (16 × BaudRate)
 *
 *   其中：
 *     fCK = USART的输入时钟
 *       - USART1在APB2上：fCK = PCLK2 = 72MHz
 *       - USART2/3在APB1上：fCK = PCLK1 = 36MHz
 *
 *   以USART1、9600波特率为例：
 *     USARTDIV = 72,000,000 / (16 × 9600)
 *              = 72,000,000 / 153,600
 *              = 468.75
 *
 *   BRR寄存器的格式：
 *     - 高12位：USARTDIV的整数部分（468）
 *     - 低4位： USARTDIV的小数部分 × 16（0.75 × 16 = 12 = 0xC）
 *
 *   所以 BRR = 0x468C
 *
 *   HAL库的 HAL_UART_Init() 会自动计算BRR的值，
 *   我们只需要告诉它波特率即可。
 */
static void MX_USART1_UART_Init(void)
{
  /* ========== 第1步：使能USART1时钟 ==========
   * USART1挂在APB2总线上
   */
  __HAL_RCC_USART1_CLK_ENABLE();

  /* ========== 第2步：配置USART参数 ==========
   *
   * UART_InitTypeDef 结构体成员说明：
   *
   *   BaudRate：波特率
   *     常用值：9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
   *     通信双方波特率必须一致，否则无法正常通信
   *
   *   WordLength：字长（数据位长度）
   *     UART_WORDLENGTH_8B - 8位数据（最常用）
   *     UART_WORDLENGTH_9B - 9位数据（可以用作8位+校验位）
   *
   *   StopBits：停止位
   *     UART_STOPBITS_1   - 1位停止位（最常用）
   *     UART_STOPBITS_0_5 - 0.5位停止位
   *     UART_STOPBITS_2   - 2位停止位
   *     UART_STOPBITS_1_5 - 1.5位停止位
   *
   *   Parity：校验位
   *     UART_PARITY_NONE  - 无校验（最常用）
   *     UART_PARITY_EVEN  - 偶校验
   *     UART_PARITY_ODD   - 奇校验
   *     注意：如果使能了校验位，数据位会自动减1
   *           （例如8位数据+偶校验 实际上是 WordLength=9B + Parity=EVEN）
   *
   *   Mode：工作模式
   *     UART_MODE_TX    - 仅发送
   *     UART_MODE_RX    - 仅接收
   *     UART_MODE_TX_RX - 收发都有（最常用）
   *
   *   HwFlowCtl：硬件流控制
   *     UART_HWCONTROL_NONE    - 无流控制（最常用）
   *     UART_HWCONTROL_RTS     - RTS流控制
   *     UART_HWCONTROL_CTS     - CTS流控制
   *     UART_HWCONTROL_RTS_CTS - RTS+CTS流控制
   *     硬件流控制需要额外的RTS/CTS引脚，用于防止数据溢出
   *
   *   OverSampling：过采样
   *     UART_OVERSAMPLING_16 - 16倍过采样（默认，精度高）
   *     UART_OVERSAMPLING_8  - 8倍过采样（可以达到更高波特率）
   */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;                  /* 波特率：9600 */
  huart1.Init.WordLength = UART_WORDLENGTH_8B;  /* 数据位：8位 */
  huart1.Init.StopBits = UART_STOPBITS_1;       /* 停止位：1位 */
  huart1.Init.Parity = UART_PARITY_NONE;        /* 校验位：无 */
  huart1.Init.Mode = UART_MODE_TX_RX;           /* 模式：收发都有 */
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  /* 硬件流控：无 */
  huart1.Init.OverSampling = UART_OVERSAMPLING_16; /* 16倍过采样 */

  /* 调用HAL库函数初始化UART */
  if (HAL_UART_Init(&huart1) != HAL_OK)
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
 * 知识补充：串口通信基础
 * ================================================================
 *
 * 什么是串口通信？
 *   串口通信是一种串行通信方式，数据一位一位地按顺序传输。
 *   只需要两根线（TX和RX）就可以实现双向通信。
 *
 * 串行 vs 并行：
 *   串行：数据按位依次传输，只需要1~2根数据线
 *         优点：线少，成本低，适合长距离
 *         缺点：速度相对较慢
 *
 *   并行：数据多位同时传输，需要多根数据线（如8位、16位）
 *         优点：速度快
 *         缺点：线多，成本高，容易受到干扰，不适合长距离
 *
 * 同步 vs 异步：
 *   同步通信：有单独的时钟线，收发双方根据时钟信号同步
 *             例子：SPI、I2S
 *
 *   异步通信：没有时钟线，收发双方约定好波特率，通过起始位和停止位同步
 *             例子：UART、RS232、RS485
 *
 * RS232 vs TTL：
 *   这是两个容易混淆的概念：
 *
 *   TTL电平串口：
 *     - 高电平：3.3V 或 5V
 *     - 低电平：0V
 *     - 距离短（一般几米以内）
 *     - STM32的串口就是TTL电平的
 *
 *   RS232电平串口：
 *     - 高电平（逻辑0）：+3V ~ +15V
 *     - 低电平（逻辑1）：-3V ~ -15V
 *     - 距离较远（可达几十米）
 *     - 需要MAX232等电平转换芯片
 *
 *   注意：电平不同，不能直接连接！
 *   STM32的串口是TTL电平，要连接电脑的RS232口需要MAX232芯片。
 *   现在常用USB转TTL模块，直接插USB口，另一端是TTL电平。
 *
 * ================================================================
 * 知识补充：HAL库UART的三种工作方式
 * ================================================================
 *
 * HAL库的UART支持三种工作方式：
 *
 * 1. 阻塞式（Polling/Blocking）
 *    函数：HAL_UART_Transmit(), HAL_UART_Receive()
 *    特点：函数一直等到发送/接收完成（或超时）才返回
 *    优点：编程简单，逻辑清晰
 *    缺点：CPU被阻塞，期间不能做其他事
 *    适用：简单的应用、对实时性要求不高的场合
 *
 * 2. 中断式（Interrupt）
 *    函数：HAL_UART_Transmit_IT(), HAL_UART_Receive_IT()
 *    特点：函数立即返回，发送/接收完成后触发中断，在回调函数中处理
 *    优点：不阻塞CPU，效率高
 *    缺点：编程稍复杂，需要处理中断回调
 *    适用：大多数应用场景
 *
 * 3. DMA式
 *    函数：HAL_UART_Transmit_DMA(), HAL_UART_Receive_DMA()
 *    特点：由DMA控制器搬运数据，CPU完全不参与
 *    优点：CPU效率最高，适合大量数据传输
 *    缺点：需要配置DMA，资源占用多
 *    适用：高速、大量数据传输的场合
 *
 * 本文件演示的是第1种（阻塞式），下一个文件演示第2种（中断式）。
 *
 * ================================================================
 * 知识补充：常见的串口调试问题及解决方法
 * ================================================================
 *
 * 问题1：完全收不到数据
 *   可能原因：
 *   - TX和RX接反了（TX应该接对方的RX）
 *   - GND没接（没有共地）
 *   - 波特率设置不一致
 *   - 串口选错了（COM口号不对）
 *   - USB转TTL驱动没装
 *   - 串口初始化失败（检查时钟是否使能）
 *
 * 问题2：收到乱码
 *   可能原因：
 *   - 波特率不对（最常见！）
 *   - 数据位/停止位/校验位设置不一致
 *   - 晶振不准（外部晶振与代码配置不一致）
 *   - 干扰导致误码
 *
 * 问题3：丢数据
 *   可能原因：
 *   - 发送太快，接收端处理不过来
 *   - 缓冲区太小
 *   - 没有使用中断/DMA，查询方式太慢
 *
 * 问题4：printf不输出
 *   可能原因：
 *   - 没有重写fputc函数
 *   - 没有包含stdio.h
 *   - 没有勾选Use MicroLIB（Keil中）
 *   - 串口初始化失败
 *
 * 调试技巧：
 *   1. 用示波器或逻辑分析仪看TX引脚波形
 *   2. 先做回环测试（TX接RX，自己发自己收）
 *   3. 发送固定的0x55（01010101），方便测量波特率
 *   4. 检查硬件连接，特别是GND
 *
 * ================================================================
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
