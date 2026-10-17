/**
 ******************************************************************************
 * @file    01_i2c_eeprom.c
 * @brief   I2C通信实验 - 24C02 EEPROM读写
 *          使用HAL库的I2C接口，实现对24C02 EEPROM的字节读写和页写入
 *          基于STM32F103C8T6最小系统板（I2C1 PB6/SCL PB7/SDA）
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐                    ┌──────────────┐
 *   │  STM32F103   │                    │   24C02      │
 *   │              │                    │  (EEPROM)    │
 *   │  PB6  SCL ───┼────────────────────┼── SCL        │
 *   │              │                    │              │
 *   │  PB7  SDA ───┼────────────────────┼── SDA        │
 *   │              │                    │              │
 *   │  3.3V  ──────┼───┬────────────────┼── VCC        │
 *   │              │   │ 4.7kΩ          │              │
 *   │              │   ├──────── SCL ───┤              │
 *   │              │   │                │              │
 *   │              │   └──────── SDA ───┤              │
 *   │  GND   ──────┴────────────────────┴── GND        │
 *   └──────────────┘                    └──────────────┘
 *
 *   注意：
 *     1. I2C总线需要上拉电阻（一般4.7kΩ），SCL和SDA各一个
 *     2. 24C02的A0/A1/A2引脚决定设备地址，本示例假设全部接GND
 *     3. 24C02的WP引脚（写保护）接GND，允许写入
 *
 *   24C02设备地址计算：
 *     7位地址格式：1 0 1 0 A2 A1 A0 R/W
 *     A2=A1=A0=0 时，写地址 = 0xA0，读地址 = 0xA1
 *
 * 学习重点：
 *   1. I2C总线的工作原理（SCL时钟、SDA数据、起始/停止条件）
 *   2. HAL_I2C_Mem_Write() / HAL_I2C_Mem_Read() 存储器读写函数
 *   3. I2C时序分析（起始位、应答、数据传输、停止位）
 *   4. 24C02 EEPROM的页写入特性（Page Write）
 *   5. I2C设备地址的计算方法
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* I2C外设定义（I2C1） */
#define EEPROM_I2C              I2C1

/* 24C02设备地址
 * I2C地址是7位，HAL库中使用8位格式（左移一位，最低位为R/W位）
 * 24C02的地址前缀固定为 1010，加上A2/A1/A0三位
 * 本示例A2=A1=A0=0，所以：
 *   写地址 = 10100000 = 0xA0
 *   读地址 = 10100001 = 0xA1
 * HAL库函数内部会自动处理R/W位，所以传0xA0即可
 */
#define EEPROM_ADDR             0xA0

/* 24C02参数 */
#define EEPROM_PAGE_SIZE        8       /* 24C02每页8字节 */
#define EEPROM_TOTAL_SIZE       256     /* 24C02总容量256字节 */
#define EEPROM_WRITE_TIMEOUT    100     /* 写入超时时间（毫秒） */
#define EEPROM_READ_TIMEOUT     100     /* 读取超时时间（毫秒） */

/* 写入周期等待时间
 * EEPROM写入需要时间（典型值5ms，最大10ms）
 * 写操作完成后需要等待一段时间才能进行下一次操作
 */
#define EEPROM_WRITE_DELAY      10      /* 写入后等待时间（毫秒） */

/* 测试用的存储地址 */
#define TEST_ADDR               0x00    /* 测试起始地址 */

/* LED引脚（PC13，用于状态指示） */
#define LED_GPIO_PORT           GPIOC
#define LED_GPIO_PIN            GPIO_PIN_13

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void Error_Handler(void);

/* EEPROM操作函数声明 */
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t addr, uint8_t data);
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t addr, uint8_t *pdata);
HAL_StatusTypeDef EEPROM_WritePage(uint16_t addr, uint8_t *pdata, uint16_t size);
HAL_StatusTypeDef EEPROM_ReadBytes(uint16_t addr, uint8_t *pdata, uint16_t size);

/**
 * @brief  主函数：程序入口
 * @retval int
 *
 * 实验流程：
 *   1. 系统初始化（HAL、时钟、GPIO、I2C）
 *   2. 向EEPROM指定地址写入测试数据
 *   3. 从相同地址读取数据
 *   4. 比较写入和读取的数据是否一致
 *   5. LED闪烁表示测试结果（正常慢闪，失败快闪）
 */
int main(void)
{
  /* ========== 第1步：系统初始化 ========== */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  /* ========== 第2步：EEPROM读写测试 ========== */

  /* 测试数据 */
  uint8_t write_data = 0x5A;    /* 要写入的数据 */
  uint8_t read_data = 0;        /* 读取的数据缓存 */
  uint8_t test_passed = 1;      /* 测试通过标志 */

  /* ---------- 测试1：单字节写入和读取 ---------- */

  /* 向地址0x00写入一个字节 */
  if (EEPROM_WriteByte(TEST_ADDR, write_data) != HAL_OK)
  {
    /* 写入失败 */
    test_passed = 0;
  }
  else
  {
    /* 等待写入完成（EEPROM内部编程时间） */
    HAL_Delay(EEPROM_WRITE_DELAY);

    /* 从地址0x00读取一个字节 */
    if (EEPROM_ReadByte(TEST_ADDR, &read_data) != HAL_OK)
    {
      test_passed = 0;
    }
    else if (read_data != write_data)
    {
      /* 数据不一致 */
      test_passed = 0;
    }
  }

  /* ---------- 测试2：多字节（页）写入和读取 ---------- */
  if (test_passed)
  {
    uint8_t write_buf[EEPROM_PAGE_SIZE] = {0x01, 0x02, 0x03, 0x04,
                                            0x05, 0x06, 0x07, 0x08};
    uint8_t read_buf[EEPROM_PAGE_SIZE] = {0};

    /* 页写入：从地址0x10开始写入一页（8字节） */
    if (EEPROM_WritePage(0x10, write_buf, EEPROM_PAGE_SIZE) != HAL_OK)
    {
      test_passed = 0;
    }
    else
    {
      HAL_Delay(EEPROM_WRITE_DELAY);

      /* 连续读取8字节 */
      if (EEPROM_ReadBytes(0x10, read_buf, EEPROM_PAGE_SIZE) != HAL_OK)
      {
        test_passed = 0;
      }
      else
      {
        /* 逐字节比较 */
        for (uint8_t i = 0; i < EEPROM_PAGE_SIZE; i++)
        {
          if (read_buf[i] != write_buf[i])
          {
            test_passed = 0;
            break;
          }
        }
      }
    }
  }

  /* ========== 第3步：主循环 - LED指示测试结果 ========== */
  while (1)
  {
    if (test_passed)
    {
      /* 测试通过：LED慢闪（每秒1次） */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
      HAL_Delay(500);
    }
    else
    {
      /* 测试失败：LED快闪（每秒10次） */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
      HAL_Delay(100);
    }
  }
}

/* ================================================================
 * EEPROM操作函数
 * ================================================================ */

/**
 * @brief  EEPROM单字节写入
 * @param  addr: 写入地址（0~255 for 24C02）
 * @param  data: 要写入的数据
 * @retval HAL状态
 *
 * 字节写入时序：
 *   起始 → 设备地址(写) → 应答 → 存储地址 → 应答 → 数据 → 应答 → 停止
 *
 * 注意：写入后EEPROM需要内部编程时间（典型5ms），
 *       在此期间不响应新的写操作。
 */
HAL_StatusTypeDef EEPROM_WriteByte(uint16_t addr, uint8_t data)
{
  /* HAL_I2C_Mem_Write：I2C存储器写函数
   *
   * 参数说明：
   *   hi2c      - I2C句柄指针
   *   DevAddress- 设备地址（8位格式，含R/W位，HAL会自动处理）
   *   MemAddress- 存储器内部地址
   *   MemAddSize- 存储器地址大小（8位或16位）
   *               I2C_MEMADD_SIZE_8BIT  = 8位地址（24C02用）
   *               I2C_MEMADD_SIZE_16BIT = 16位地址（24C64以上用）
   *   pData     - 数据缓冲区指针
   *   Size      - 数据长度（字节数）
   *   Timeout   - 超时时间（毫秒）
   */
  return HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR,
                           addr, I2C_MEMADD_SIZE_8BIT,
                           &data, 1,
                           EEPROM_WRITE_TIMEOUT);
}

/**
 * @brief  EEPROM单字节读取
 * @param  addr: 读取地址（0~255 for 24C02）
 * @param  pdata: 读取数据的存放指针
 * @retval HAL状态
 *
 * 随机读取时序：
 *   起始 → 设备地址(写) → 应答 → 存储地址 → 应答 →
 *   重复起始 → 设备地址(读) → 应答 → 数据(从机输出) → 非应答 → 停止
 *
 * 这是一个"写-读"复合操作：先写地址，再读数据。
 * HAL_I2C_Mem_Read内部会自动处理这个过程。
 */
HAL_StatusTypeDef EEPROM_ReadByte(uint16_t addr, uint8_t *pdata)
{
  return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR,
                          addr, I2C_MEMADD_SIZE_8BIT,
                          pdata, 1,
                          EEPROM_READ_TIMEOUT);
}

/**
 * @brief  EEPROM页写入
 * @param  addr: 起始地址（必须页对齐！）
 * @param  pdata: 数据缓冲区指针
 * @param  size: 写入字节数（不能超过页大小！）
 * @retval HAL状态
 *
 * 页写入原理：
 *   24C02内部有一个8字节的页缓存（Page Buffer）。
 *   在一次写操作中，可以连续写入最多8字节（一页），
 *   然后EEPROM会一次性把这8字节写入存储阵列。
 *
 * 页写入的好处：
 *   - 比单字节写入效率高很多
 *   - 8字节只需要一次5ms的编程时间
 *   - 而单字节写8次需要8×5ms=40ms
 *
 * 注意事项：
 *   1. 页写入不能跨页边界！
 *      例如：从地址0x07开始写2字节，第2字节会回卷到0x00
 *      因为地址的低3位（页内地址）溢出后会回卷
 *   2. 24C02的页大小是8字节，24C04是16字节，24C08/16是32字节
 */
HAL_StatusTypeDef EEPROM_WritePage(uint16_t addr, uint8_t *pdata, uint16_t size)
{
  /* 参数检查：写入大小不能超过页大小 */
  if (size > EEPROM_PAGE_SIZE)
  {
    return HAL_ERROR;
  }

  return HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR,
                           addr, I2C_MEMADD_SIZE_8BIT,
                           pdata, size,
                           EEPROM_WRITE_TIMEOUT);
}

/**
 * @brief  EEPROM连续读取
 * @param  addr: 起始地址
 * @param  pdata: 数据缓冲区指针
 * @param  size: 读取字节数
 * @retval HAL状态
 *
 * 连续读取原理：
 *   读取第一个字节后，主机发送应答信号（ACK），
 *   EEPROM就会自动将地址加1，继续输出下一个地址的数据。
 *   直到主机发送非应答信号（NACK）和停止条件。
 *
 * 读取没有页限制，可以一次读取整个芯片的内容（256字节）。
 */
HAL_StatusTypeDef EEPROM_ReadBytes(uint16_t addr, uint8_t *pdata, uint16_t size)
{
  return HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR,
                          addr, I2C_MEMADD_SIZE_8BIT,
                          pdata, size,
                          EEPROM_READ_TIMEOUT);
}

/* ================================================================
 * 系统初始化函数
 * ================================================================ */

/**
 * @brief  系统时钟配置函数（72MHz）
 * @note   外部8MHz晶振 + PLL ×9 = 72MHz
 *         详细注释请参考 01_GPIO/01_led_blink.c
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
 * @note   配置PC13为推挽输出（LED状态指示）
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

  /* LED初始熄灭 */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

/**
 * @brief  I2C1初始化函数
 * @note   配置I2C1为主机模式，标准速度100kHz
 *         SCL -> PB6，SDA -> PB7
 *
 * I2C初始化步骤：
 *   1. 使能I2C时钟和GPIO时钟
 *   2. 配置GPIO为复用开漏模式（I2C必须开漏！）
 *   3. 配置I2C工作参数（模式、时钟、地址等）
 *
 * I2C引脚复用：
 *   STM32F103的I2C1引脚：
 *     - 默认映射：SCL=PB6，SDA=PB7
 *     - 重映射：  SCL=PB8，SDA=PB9（需要开启AFIO时钟并重映射）
 *   本示例使用默认映射。
 *
 * 为什么I2C引脚必须配置为开漏输出？
 *   I2C总线是"线与"逻辑：
 *   - 任何一个设备拉低SDA/SCL，总线就是低电平
 *   - 只有所有设备都释放（高阻），上拉电阻才把总线拉到高电平
 *   推挽输出会导致总线冲突（一个拉低一个拉高=短路）
 *   开漏输出只能拉低，高电平靠上拉电阻，天然支持线与
 */
static void MX_I2C1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ---------- 第1步：使能时钟 ----------
   * I2C1挂在APB1总线上
   * GPIOB挂在APB2总线上
   */
  __HAL_RCC_I2C1_CLK_ENABLE();     /* 使能I2C1时钟 */
  __HAL_RCC_GPIOB_CLK_ENABLE();    /* 使能GPIOB时钟（I2C引脚用） */

  /* ---------- 第2步：配置GPIO引脚（复用开漏模式） ----------
   * PB6 -> SCL（时钟线）
   * PB7 -> SDA（数据线）
   *
   * 模式：GPIO_MODE_AF_OD = 复用开漏输出
   *   - 复用：引脚控制权交给I2C外设
   *   - 开漏：I2C总线要求开漏输出（线与逻辑）
   *
   * 速度：GPIO_SPEED_FREQ_HIGH = 高速
   *   I2C通信速率虽然不高（标准模式100kHz），
   *   但开漏输出的上升沿较慢，设置高速可以改善边沿。
   */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;  /* PB6和PB7同时配置 */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;         /* 复用开漏模式 */
  GPIO_InitStruct.Pull = GPIO_PULLUP;             /* 内部上拉（可以替代外部上拉电阻，但建议还是用外部的） */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   /* 高速 */

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ---------- 第3步：配置I2C外设 ----------
   * I2C初始化结构体说明：
   *
   * ClockSpeed：I2C通信速率
   *   - 标准模式：100000 (100kHz)
   *   - 快速模式：400000 (400kHz)
   *   24C02支持标准模式（100kHz），所以设为100000
   *
   * DutyCycle：快速模式下的占空比
   *   - I2C_DUTYCYCLE_2    = 1:2（标准模式默认）
   *   - I2C_DUTYCYCLE_16_9 = 16:9（快速模式可选）
   *   标准模式下这个参数不生效
   *
   * OwnAddress1：自己的设备地址（主机模式下一般用不到）
   *   I2C是多主机总线，每个设备都有地址。
   *   即使作为主机，也要设置自己的地址（万一被别的主机寻址）。
   *   这里随便设一个不冲突的地址即可，比如0x00。
   *
   * AddressingMode：地址模式
   *   - I2C_ADDRESSINGMODE_7BIT  = 7位地址模式（常用）
   *   - I2C_ADDRESSINGMODE_10BIT = 10位地址模式
   *   24C02使用7位地址，所以选7BIT
   *
   * DualAddressMode：双地址模式（禁用）
   *
   * GeneralCallMode：广播呼叫模式（禁用）
   *
   * NoStretchMode：时钟拉伸（禁用）
   *   时钟拉伸是从机拉低SCL让主机等待的机制。
   *   主机模式下一般不需要。
   */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;              /* 100kHz标准模式 */
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;      /* 占空比1:2 */
  hi2c1.Init.OwnAddress1 = 0x00;               /* 自身地址（主机模式随意） */
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  /* 7位地址模式 */
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; /* 禁用双地址 */
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* 禁用广播 */
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* 禁用时钟拉伸（允许拉伸） */

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
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
 * 知识补充：I2C通信协议详解
 * ================================================================
 *
 * 一、I2C总线简介
 *   I2C（Inter-Integrated Circuit）是飞利浦公司发明的两线式串行总线。
 *   特点：
 *     - 只需要两根线：SCL（时钟线）+ SDA（数据线）
 *     - 支持多主机（Multi-master）
 *     - 每个设备有唯一的7位（或10位）地址
 *     - 半双工通信（同一时间只能一个方向传输）
 *     - 开漏输出 + 上拉电阻（线与逻辑）
 *
 * 二、I2C基本时序
 *
 *   SCL:  ──┐   ┌───┐   ┌───┐   ┌───┐   ┌───┐   ┌───
 *          └───┘   └───┘   └───┘   └───┘   └───┘
 *
 *   SDA:  ──────┐  D7  │  D6  │  ...  │  D0  │  ACK  ───
 *              └───────┴───────┴───────┴───────┘
 *              ↑                                    ↑
 *            起始位                               应答位
 *
 * 1. 起始条件（START）：
 *    SCL为高电平时，SDA由高变低
 *    表示一次数据传输的开始
 *
 * 2. 停止条件（STOP）：
 *    SCL为高电平时，SDA由低变高
 *    表示一次数据传输的结束
 *
 * 3. 数据传输：
 *    - SCL为高电平时，SDA必须保持稳定（数据有效）
 *    - SCL为低电平时，SDA才能变化
 *    - 每个字节8位，高位在前（MSB first）
 *
 * 4. 应答（ACK/NACK）：
 *    - 每传输完一个字节（8位），第9个时钟周期是应答位
 *    - ACK：接收方拉低SDA（低电平表示应答）
 *    - NACK：接收方释放SDA（高电平表示非应答）
 *    - 主机接收最后一个字节时，应该发NACK告诉从机"可以结束了"
 *
 * 三、I2C读写流程
 *
 * 写操作（主机写从机）：
 *   START → 设备地址(W) → ACK → 寄存器地址 → ACK → 数据 → ACK → ... → STOP
 *
 * 读操作（主机读从机，随机读）：
 *   START → 设备地址(W) → ACK → 寄存器地址 → ACK →
 *   RESTART → 设备地址(R) → ACK → 数据 → ACK → ... → 最后数据 → NACK → STOP
 *
 * 四、24C02 EEPROM知识点
 *
 * 1. 容量：2K bit = 256 Byte
 * 2. 地址：8位地址（0x00 ~ 0xFF）
 * 3. 页大小：8字节（Page Write模式）
 * 4. 写周期时间：典型5ms，最大10ms
 *    （写操作后EEPROM需要时间把数据从缓存写到存储阵列）
 * 5. 擦写次数：约100万次
 * 6. 数据保存时间：约100年
 *
 * 五、常见问题与调试技巧
 *
 * 1. I2C通信失败，检查什么？
 *    - 上拉电阻是否焊接？阻值是否合适（4.7kΩ常用）
 *    - 设备地址是否正确？（注意R/W位和HAL库的格式）
 *    - 引脚是否接反？SCL接SCL，SDA接SDA
 *    - 电源电压是否一致？（3.3V系统接3.3V器件）
 *
 * 2. 如何用逻辑分析仪调试I2C？
 *    - 抓取SCL和SDA波形
 *    - 检查起始位、地址、ACK、数据、停止位
 *    - 看ACK有没有回来（从机是否响应）
 *    - 如果连ACK都没有，基本是硬件问题
 *
 * 3. HAL_I2C_Mem_Write和HAL_I2C_Transmit的区别？
 *    - HAL_I2C_Transmit：普通I2C发送，只发数据
 *    - HAL_I2C_Mem_Write：存储器专用，先发内部地址再发数据
 *    对于EEPROM这种有内部地址的器件，用Mem函数更方便。
 *
 * ================================================================ */

/* I2C句柄全局变量
 * 注意：在实际工程中，这个变量应该定义在main.c或专门的i2c.c中。
 * 本示例为了文件独立运行，在这里定义。
 */
I2C_HandleTypeDef hi2c1;

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
