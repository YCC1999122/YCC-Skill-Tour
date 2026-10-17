/**
 ******************************************************************************
 * @file    02_uart_it_receive.c
 * @brief   串口中断接收实验 - 环形缓冲区 + 命令解析
 *          使用USART1中断接收，配合环形缓冲区存储数据
 *          主循环中解析命令，支持LED控制、查询版本等命令
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
 * 支持的命令（在串口调试助手中输入，加换行发送）：
 *   "led on"     - 点亮LED
 *   "led off"    - 熄灭LED
 *   "led toggle" - 翻转LED
 *   "version"    - 显示版本信息
 *   "help"       - 显示帮助信息
 *   "status"     - 显示系统状态
 *
 * 学习重点：
 *   1. HAL_UART_Receive_IT() 中断接收的使用
 *   2. HAL_UART_RxCpltCallback() 接收完成回调函数
 *   3. 环形缓冲区（Ring Buffer）的原理和实现
 *   4. 串口命令解析的基本方法
 *   5. 中断与主循环之间的数据交互
 *   6. 字符串处理函数的使用（strcmp、strtok等）
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* USART1 引脚定义 */
#define UART_INSTANCE           USART1
#define UART_TX_GPIO_PORT       GPIOA
#define UART_TX_PIN             GPIO_PIN_9
#define UART_RX_GPIO_PORT       GPIOA
#define UART_RX_PIN             GPIO_PIN_10

/* LED引脚定义 */
#define LED_GPIO_PORT           GPIOC
#define LED_GPIO_PIN            GPIO_PIN_13

/* 环形缓冲区大小
 * 缓冲区大小一般选2的幂（如64、128、256、512），
 * 这样取模运算可以用位与运算优化（index & (SIZE-1)）。
 * 这里设置为128字节，足够存储几条命令。
 */
#define RING_BUFFER_SIZE        128

/* 命令行最大长度 */
#define CMD_MAX_LEN             64

/* 固件版本号 */
#define FIRMWARE_VERSION        "V1.0.0"

/* ================================================================
 * 全局变量区
 * ================================================================ */

/* UART句柄 */
UART_HandleTypeDef huart1;

/* ---------- 环形缓冲区 ----------
 *
 * 环形缓冲区（Ring Buffer / Circular Buffer）是一种常用的数据结构，
 * 特别适合于中断和主循环之间的数据传递。
 *
 * 原理：
 *   用一块固定大小的数组作为缓冲区，用两个指针（或索引）分别表示
 *   读位置（head）和写位置（tail）。
 *
 *   写入数据时，数据放到tail位置，tail向后移动。
 *   读取数据时，从head位置读取，head向后移动。
 *   当head或tail到达缓冲区末尾时，自动绕回到开头（环形）。
 *
 *   缓冲区空：head == tail
 *   缓冲区满：(tail + 1) % SIZE == head
 *   （为了区分空和满，一般浪费一个位置，即最多存 SIZE-1 个数据）
 *
 *         0   1   2   3   4   5   6   7
 *       ┌───┬───┬───┬───┬───┬───┬───┬───┐
 *       │ D │ D │ D │   │   │   │ D │ D │
 *       └───┴───┴───┴───┴───┴───┴───┴───┘
 *                 ↑               ↑
 *                head            tail
 *
 * 为什么要用环形缓冲区？
 *   1. 中断接收数据速度很快，而主循环处理可能比较慢，
 *      需要一个缓冲区来暂存数据，防止数据丢失。
 *   2. 中断中只做简单的写入操作，复杂的解析放到主循环，
 *      符合"中断快进快出"的原则。
 *   3. 环形缓冲区可以循环使用，不需要搬移数据，效率高。
 */
typedef struct {
  uint8_t buffer[RING_BUFFER_SIZE];  /* 缓冲区数组 */
  volatile uint16_t head;            /* 读指针（主循环读数据的位置） */
  volatile uint16_t tail;            /* 写指针（中断写数据的位置） */
} RingBuffer_t;

RingBuffer_t g_uart_rx_buffer;  /* 串口接收环形缓冲区 */

/* 当前接收的字节（用于HAL_UART_Receive_IT）
 * 每次中断接收1个字节，收到后存入环形缓冲区
 */
uint8_t g_rx_byte;

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void Error_Handler(void);
int fputc(int ch, FILE *f);

/* 环形缓冲区操作函数 */
void RingBuffer_Init(RingBuffer_t *rb);
uint16_t RingBuffer_Available(RingBuffer_t *rb);
uint8_t RingBuffer_Write(RingBuffer_t *rb, uint8_t data);
uint8_t RingBuffer_Read(RingBuffer_t *rb, uint8_t *data);

/* 命令处理函数 */
void Process_Command(char *cmd);
uint8_t Read_Line_From_RingBuffer(char *line_buf, uint16_t buf_size);

/**
 * @brief  主函数
 * @retval int
 *
 * 功能：
 *   1. 串口中断接收数据，存入环形缓冲区
 *   2. 主循环从环形缓冲区读取数据，解析命令
 *   3. 根据命令执行相应操作（控制LED、返回信息等）
 *
 * 整体架构：
 *   中断层（快）：接收字节 → 写入环形缓冲区 → 继续等待下一字节
 *   应用层（慢）：从缓冲区读取 → 组装成完整命令 → 解析执行
 */
int main(void)
{
  /* ========== 第1步：HAL库初始化 ========== */
  HAL_Init();

  /* ========== 第2步：系统时钟配置 ========== */
  SystemClock_Config();

  /* ========== 第3步：GPIO初始化 ========== */
  MX_GPIO_Init();

  /* ========== 第4步：USART1初始化 ========== */
  MX_USART1_UART_Init();

  /* ========== 第5步：初始化环形缓冲区 ========== */
  RingBuffer_Init(&g_uart_rx_buffer);

  /* ========== 第6步：启动串口中断接收 ==========
   * HAL_UART_Receive_IT(huart, pData, Size)
   *   - huart：UART句柄指针
   *   - pData：接收数据的缓冲区指针
   *   - Size：要接收的字节数
   *
   * 功能：启动中断接收，当接收到指定数量的字节后，
   *       触发接收完成中断，调用 HAL_UART_RxCpltCallback。
   *
   * 这里我们每次只接收1个字节（Size=1），
   * 因为串口数据是流式的，我们不知道对方会发多少数据。
   * 每收到1个字节，就存入环形缓冲区，然后重新启动接收。
   *
   * 注意：HAL_UART_Receive_IT 只是启动接收，函数立即返回，
   *       不会等待数据到来。数据到达时会触发中断。
   */
  if (HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1) != HAL_OK)
  {
    Error_Handler();
  }

  /* ========== 第7步：打印欢迎信息 ========== */
  printf("\r\n========================================\r\n");
  printf("  STM32 UART 中断接收实验\r\n");
  printf("  固件版本：%s\r\n", FIRMWARE_VERSION);
  printf("========================================\r\n");
  printf("  输入 'help' 查看帮助信息\r\n");
  printf("========================================\r\n\r\n");
  printf("> ");  /* 命令提示符 */

  /* 命令行缓冲区 */
  char cmd_buf[CMD_MAX_LEN];

  /* ========== 第8步：主循环 ========== */
  while (1)
  {
    /* 从环形缓冲区读取一行数据（遇到换行符或回车符为一行）
     * Read_Line_From_RingBuffer 返回1表示读到了完整的一行
     */
    if (Read_Line_From_RingBuffer(cmd_buf, CMD_MAX_LEN))
    {
      /* 回显输入的命令（方便用户看到自己输入了什么） */
      printf("%s\r\n", cmd_buf);

      /* 处理命令 */
      Process_Command(cmd_buf);

      /* 输出命令提示符 */
      printf("> ");
    }

    /* 主循环还可以做其他事情...
     * 串口接收完全由中断处理，不影响主循环
     */
  }
}

/* ================================================================
 * 中断回调函数
 * ================================================================ */

/**
 * @brief  串口接收完成回调函数
 * @param  huart: UART句柄指针
 * @note   当 HAL_UART_Receive_IT 设置的字节数全部接收完成时，
 *         HAL库会自动调用这个回调函数。
 *
 * 执行流程：
 *   1. USART收到1个字节，RXNE标志置位
 *   2. 触发USART1中断，CPU跳转到 USART1_IRQHandler
 *   3. USART1_IRQHandler 调用 HAL_UART_IRQHandler(&huart1)
 *   4. HAL_UART_IRQHandler 读取数据，清除标志
 *   5. 如果所有数据都接收完了，调用 HAL_UART_RxCpltCallback
 *
 * 注意：HAL_UART_Receive_IT 是"一次性"的，
 *       每次接收完成后需要重新调用才能继续接收。
 *       所以回调函数中必须再次调用 HAL_UART_Receive_IT。
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    /* 将收到的字节写入环形缓冲区 */
    RingBuffer_Write(&g_uart_rx_buffer, g_rx_byte);

    /* 重新启动中断接收，等待下一个字节
     * 这一步非常重要！HAL_UART_Receive_IT 只启动一次接收，
     * 接收完成后就停止了，必须重新调用才能继续接收。
     */
    HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
  }
}

/* ================================================================
 * 环形缓冲区操作函数
 * ================================================================ */

/**
 * @brief  初始化环形缓冲区
 * @param  rb: 环形缓冲区指针
 */
void RingBuffer_Init(RingBuffer_t *rb)
{
  rb->head = 0;
  rb->tail = 0;
  /* buffer数组不需要清零，因为读写都是按位置操作的 */
}

/**
 * @brief  获取环形缓冲区中可读的数据量
 * @param  rb: 环形缓冲区指针
 * @retval uint16_t: 可读的字节数
 *
 * 计算方法：
 *   如果 tail >= head：数据量 = tail - head
 *   如果 tail < head：说明tail绕回了开头，数据量 = tail + SIZE - head
 *
 * 统一公式：(tail - head + SIZE) % SIZE
 */
uint16_t RingBuffer_Available(RingBuffer_t *rb)
{
  return (rb->tail - rb->head + RING_BUFFER_SIZE) % RING_BUFFER_SIZE;
}

/**
 * @brief  向环形缓冲区写入一个字节
 * @param  rb: 环形缓冲区指针
 * @param  data: 要写入的数据
 * @retval uint8_t: 1=成功，0=缓冲区满
 *
 * 写入位置：tail
 * 写入后：tail = (tail + 1) % SIZE
 *
 * 缓冲区满的判断：
 *   如果写入后 tail 会追上 head，说明缓冲区满了，
 *   即 (tail + 1) % SIZE == head 时为满。
 *   （浪费一个位置来区分"空"和"满"）
 */
uint8_t RingBuffer_Write(RingBuffer_t *rb, uint8_t data)
{
  /* 计算下一个tail的位置 */
  uint16_t next_tail = (rb->tail + 1) % RING_BUFFER_SIZE;

  /* 如果下一个tail等于head，说明缓冲区满了 */
  if (next_tail == rb->head)
  {
    return 0;  /* 写入失败：缓冲区满 */
  }

  /* 写入数据 */
  rb->buffer[rb->tail] = data;

  /* 更新tail指针 */
  rb->tail = next_tail;

  return 1;  /* 写入成功 */
}

/**
 * @brief  从环形缓冲区读取一个字节
 * @param  rb: 环形缓冲区指针
 * @param  data: 存储读取数据的指针
 * @retval uint8_t: 1=成功，0=缓冲区空
 *
 * 读取位置：head
 * 读取后：head = (head + 1) % SIZE
 *
 * 缓冲区空的判断：head == tail
 */
uint8_t RingBuffer_Read(RingBuffer_t *rb, uint8_t *data)
{
  /* 如果head == tail，缓冲区为空 */
  if (rb->head == rb->tail)
  {
    return 0;  /* 读取失败：缓冲区空 */
  }

  /* 读取数据 */
  *data = rb->buffer[rb->head];

  /* 更新head指针 */
  rb->head = (rb->head + 1) % RING_BUFFER_SIZE;

  return 1;  /* 读取成功 */
}

/* ================================================================
 * 命令行处理函数
 * ================================================================ */

/**
 * @brief  从环形缓冲区读取一行数据
 * @param  line_buf: 存储行数据的缓冲区
 * @param  buf_size: 缓冲区大小
 * @retval uint8_t: 1=读到了完整的一行，0=还没读到完整行
 *
 * 功能：
 *   逐个字节从环形缓冲区读取，直到遇到换行符('\n')或回车符('\r')，
 *   或者缓冲区满了。遇到换行/回车时，在末尾添加字符串结束符'\0'。
 *
 * 注意：
 *   1. 回车('\r')和换行('\n')都作为行结束标志
 *   2. 连续的回车换行（\r\n）只算一行结束
 *   3. 超过缓冲区长度的数据会被截断
 */
uint8_t Read_Line_From_RingBuffer(char *line_buf, uint16_t buf_size)
{
  static uint16_t line_len = 0;  /* 当前已读取的行长度（静态变量，保持状态） */
  uint8_t byte;

  /* 循环读取，直到没有数据或读到行结束符 */
  while (RingBuffer_Read(&g_uart_rx_buffer, &byte))
  {
    /* 判断是否是行结束符（回车或换行） */
    if (byte == '\r' || byte == '\n')
    {
      /* 如果已经有数据了（不是空行），返回这一行 */
      if (line_len > 0)
      {
        line_buf[line_len] = '\0';  /* 添加字符串结束符 */
        line_len = 0;               /* 重置行长度，准备下一行 */
        return 1;                   /* 读到完整的一行 */
      }
      /* 如果是空行（只有回车换行），跳过继续读 */
      continue;
    }

    /* 不是行结束符，存入行缓冲区 */
    if (line_len < buf_size - 1)  /* -1 是为了留位置给 '\0' */
    {
      line_buf[line_len++] = (char)byte;
    }
    else
    {
      /* 缓冲区满了，强制结束（防止溢出） */
      line_buf[line_len] = '\0';
      line_len = 0;
      return 1;
    }
  }

  /* 缓冲区读完了，还没遇到行结束符 */
  return 0;
}

/**
 * @brief  命令处理函数
 * @param  cmd: 命令字符串
 * @note   解析命令并执行相应操作
 *
 * 支持的命令：
 *   help        - 显示帮助
 *   version     - 显示版本
 *   status      - 显示状态
 *   led on      - 点亮LED
 *   led off     - 熄灭LED
 *   led toggle  - 翻转LED
 *
 * 命令解析的基本思路：
 *   1. 提取第一个单词（命令关键字）
 *   2. 根据关键字进入不同的处理分支
 *   3. 提取后续单词（参数）
 *   4. 执行相应操作
 */
void Process_Command(char *cmd)
{
  /* 将命令转换为小写（不区分大小写）
   * 这样 "LED ON"、"Led On"、"led on" 都能识别
   */
  for (int i = 0; cmd[i]; i++)
  {
    cmd[i] = tolower((uint8_t)cmd[i]);
  }

  /* ---------- 帮助命令 ---------- */
  if (strcmp(cmd, "help") == 0)
  {
    printf("可用命令列表：\r\n");
    printf("  help        - 显示此帮助信息\r\n");
    printf("  version     - 显示固件版本\r\n");
    printf("  status      - 显示系统状态\r\n");
    printf("  led on      - 点亮LED\r\n");
    printf("  led off     - 熄灭LED\r\n");
    printf("  led toggle  - 翻转LED状态\r\n");
    return;
  }

  /* ---------- 版本命令 ---------- */
  if (strcmp(cmd, "version") == 0)
  {
    printf("固件版本：%s\r\n", FIRMWARE_VERSION);
    printf("芯片型号：STM32F103C8T6\r\n");
    printf("系统主频：72MHz\r\n");
    return;
  }

  /* ---------- 状态命令 ---------- */
  if (strcmp(cmd, "status") == 0)
  {
    GPIO_PinState led_state = HAL_GPIO_ReadPin(LED_GPIO_PORT, LED_GPIO_PIN);
    printf("系统状态：\r\n");
    printf("  LED状态：%s\r\n", (led_state == GPIO_PIN_RESET) ? "点亮" : "熄灭");
    printf("  系统运行时间：%lu ms\r\n", HAL_GetTick());
    printf("  缓冲区可用空间：%d 字节\r\n",
           RING_BUFFER_SIZE - 1 - RingBuffer_Available(&g_uart_rx_buffer));
    return;
  }

  /* ---------- LED命令（带参数） ----------
   * 使用 strncmp 比较前4个字符是否为 "led "
   * 如果是，说明是LED控制命令，后面跟着参数
   */
  if (strncmp(cmd, "led ", 4) == 0)
  {
    char *param = cmd + 4;  /* 参数部分：跳过 "led " */

    if (strcmp(param, "on") == 0)
    {
      /* 点亮LED（低电平有效） */
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
      printf("LED已点亮\r\n");
    }
    else if (strcmp(param, "off") == 0)
    {
      /* 熄灭LED */
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
      printf("LED已熄灭\r\n");
    }
    else if (strcmp(param, "toggle") == 0)
    {
      /* 翻转LED */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
      GPIO_PinState state = HAL_GPIO_ReadPin(LED_GPIO_PORT, LED_GPIO_PIN);
      printf("LED已翻转，当前状态：%s\r\n",
             (state == GPIO_PIN_RESET) ? "点亮" : "熄灭");
    }
    else
    {
      printf("未知的LED参数：%s\r\n", param);
      printf("用法：led on/off/toggle\r\n");
    }
    return;
  }

  /* ---------- 未知命令 ---------- */
  printf("未知命令：%s\r\n", cmd);
  printf("输入 'help' 查看可用命令\r\n");
}

/* ================================================================
 * 系统初始化函数
 * ================================================================ */

/**
 * @brief  printf重定向函数
 */
int fputc(int ch, FILE *f)
{
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
 * @note   配置PA9/PA10为串口引脚，PC13为LED输出
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  /* PA9 - USART1_TX - 复用推挽输出 */
  GPIO_InitStruct.Pin = UART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(UART_TX_GPIO_PORT, &GPIO_InitStruct);

  /* PA10 - USART1_RX - 浮空输入 */
  GPIO_InitStruct.Pin = UART_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(UART_RX_GPIO_PORT, &GPIO_InitStruct);

  /* PC13 - LED - 推挽输出 */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  USART1初始化函数
 * @note   9600波特率，8位数据，1位停止，无校验
 *         使能接收中断
 */
static void MX_USART1_UART_Init(void)
{
  /* 使能USART1时钟 */
  __HAL_RCC_USART1_CLK_ENABLE();

  /* 配置USART参数 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置NVIC中断
   * 注意：中断优先级不宜设太高，
   *       串口中断属于中低优先级的外设中断。
   */
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
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
 * 知识补充：为什么需要环形缓冲区
 * ================================================================
 *
 * 没有缓冲区的问题：
 *   假设主循环正在处理一个耗时的任务（比如刷新LCD需要50ms），
 *   这时串口连续发来数据。如果没有缓冲区：
 *     - 第一种方式（查询式接收）：主循环忙，没及时读数据，
 *       新数据覆盖旧数据，导致数据丢失（溢出错误 ORE）。
 *     - 第二种方式（中断式接收，但每收到一个字节就处理）：
 *       处理逻辑太复杂，中断执行时间太长，影响其他中断。
 *
 * 有了环形缓冲区：
 *   中断中只做最简单的操作：把数据写入缓冲区，立即返回。
 *   主循环有空的时候，从缓冲区读出数据慢慢处理。
 *   只要缓冲区不溢出，数据就不会丢。
 *
 * 生产者-消费者模型：
 *   这是一个经典的"生产者-消费者"模型：
 *     - 生产者：串口中断，往缓冲区里写数据（生产数据）
 *     - 消费者：主循环，从缓冲区里读数据处理（消费数据）
 *     - 缓冲区：生产者和消费者之间的缓冲
 *
 *   生产者和消费者的速度可以不一样：
 *     - 生产者速度快、消费者速度慢：缓冲区会被填满（需要足够大的缓冲区）
 *     - 生产者速度慢、消费者速度快：缓冲区经常是空的
 *
 * ================================================================
 * 知识补充：中断接收的几种方式
 * ================================================================
 *
 * 方式一：单字节中断接收（本示例使用的方式）
 *   HAL_UART_Receive_IT(&huart, &byte, 1);
 *   每收到1个字节就进一次中断，在回调函数中处理后重新启动接收。
 *
 *   优点：灵活，可以随时处理每个字节
 *   缺点：中断频繁，每个字节都进一次中断
 *   适用：大多数通用场景
 *
 * 方式二：固定长度中断接收
 *   HAL_UART_Receive_IT(&huart, buf, N);
 *   收到N个字节后才进一次中断回调。
 *
 *   优点：中断次数少，效率高
 *   缺点：必须知道要接收多少字节，不够灵活
 *   适用：通信协议固定、数据长度已知的场景
 *
 * 方式三：空闲中断 + 中断接收（高级用法）
 *   利用串口的"空闲中断"（IDLE）来判断一帧数据是否结束。
 *   当接收线上出现一个字节长度的空闲时，触发空闲中断。
 *   配合DMA或普通接收使用，可以实现不定长数据帧的接收。
 *
 *   优点：效率最高，一帧只进一次空闲中断
 *   缺点：配置稍复杂，需要处理空闲中断
 *   适用：高性能要求的串口通信
 *
 * 方式四：DMA接收
 *   由DMA控制器自动把串口数据搬到内存，完全不占用CPU。
 *   配合空闲中断，可以实现"零CPU占用"的串口接收。
 *
 *   优点：CPU效率最高
 *   缺点：配置最复杂，需要DMA资源
 *   适用：高速、大数据量通信
 *
 * 对于初学者，建议先掌握方式一（单字节中断接收 + 环形缓冲区），
 * 这是最基础也是最通用的方式。
 *
 * ================================================================
 * 知识补充：命令行界面（CLI）设计
 * ================================================================
 *
 * 什么是CLI？
 *   CLI（Command Line Interface，命令行界面）是一种通过输入
 *   文本命令来控制设备的人机交互方式。
 *   相对于图形界面（GUI），CLI更简洁、资源占用更少，
 *   非常适合嵌入式设备的调试和配置。
 *
 * CLI的基本组成：
 *   1. 命令提示符（Prompt）：如 "> " 或 "$ "，提示用户可以输入
 *   2. 命令关键字（Command）：如 "help"、"led"、"version"
 *   3. 参数（Arguments）：如 "led on" 中的 "on"
 *   4. 选项（Options）：如 "-h"、"--help"（可选）
 *   5. 回显（Echo）：把用户输入的字符显示出来
 *   6. 输出结果：命令执行后的反馈信息
 *
 * 命令解析的常用方法：
 *
 *   1. 整串比较（strcmp）：适合简单、命令数量少的情况
 *      本示例使用的就是这种方法。
 *
 *   2. 查表法：把命令和处理函数做成一张表，循环匹配
 *      优点：添加新命令很方便，只需要在表中加一项
 *      示例：
 *        typedef struct {
 *            char *cmd;
 *            void (*handler)(char *param);
 *        } CmdTable_t;
 *
 *        CmdTable_t cmd_table[] = {
 *            {"help",    cmd_help},
 *            {"version", cmd_version},
 *            {"led",     cmd_led},
 *            // ...
 *        };
 *
 *   3. 状态机解析：适合复杂的协议解析
 *      按字节处理，根据当前状态决定下一个状态。
 *
 * 设计建议：
 *   1. 命令尽量简短，容易记忆
 *   2. 帮助信息要完善
 *   3. 错误提示要清晰（告诉用户正确的用法）
 *   4. 命令不区分大小写，用户体验更好
 *   5. 支持退格键删除（本示例暂未实现，可作为扩展练习）
 *
 * ================================================================
 * 知识补充：volatile关键字的作用
 * ================================================================
 *
 * 什么是volatile？
 *   volatile是C语言的一个关键字，用来告诉编译器：
 *   这个变量的值可能会被意外地改变（不是由当前代码改变的），
 *   每次使用这个变量时都要从内存中重新读取，不要优化。
 *
 * 什么时候需要用volatile？
 *   1. 中断服务函数中修改的、主循环中也会访问的全局变量
 *   2. 多线程/多任务中共享的变量
 *   3. 内存映射的硬件寄存器（如GPIO的ODR、IDR等）
 *
 * 为什么需要volatile？（编译器优化的问题）
 *
 *   考虑以下代码：
 *
 *     int flag = 0;
 *
 *     void main() {
 *         while (flag == 0) {
 *             // 等待flag变为1
 *         }
 *         // 做其他事情
 *     }
 *
 *     void interrupt_handler() {
 *         flag = 1;
 *     }
 *
 *   编译器在优化时（比如-O2级别），可能会这样分析：
 *     "main函数里没有修改flag，while循环中flag永远是0，
 *      所以可以优化成一个死循环，不用每次都读flag的值。"
 *
 *   优化后的代码相当于：
 *
 *     void main() {
 *         while (1) {  // 永远循环！
 *         }
 *     }
 *
 *   这样即使中断把flag改成1，main函数也看不到！
 *
 * 加上volatile之后：
 *
 *     volatile int flag = 0;
 *
 *   编译器就知道："这个变量可能会被其他地方改变，
 *   我每次都要老老实实从内存里读，不能偷懒。"
 *
 * 本示例中的应用：
 *   环形缓冲区的head和tail指针都加了volatile修饰，
 *   因为：
 *     - head 只有主循环修改，中断中只读
 *     - tail 只有中断修改，主循环中只读
 *     - 它们都是在中断和主循环之间共享的变量
 *
 * 注意事项：
 *   1. volatile不能解决所有并发问题，它只是防止编译器优化
 *   2. 对于多字节变量（如uint32_t），在8/16位单片机上还要考虑读写原子性
 *   3. 不要滥用volatile，不需要的变量加了会降低性能
 *
 * ================================================================
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
