/**
 ******************************************************************************
 * @file    01_spi_w25qxx.c
 * @brief   SPI通信实验 - W25Qxx Flash读写
 *          使用HAL库的SPI接口，实现对W25Q系列Flash的读写擦除操作
 *          基于STM32F103C8T6最小系统板（SPI1 PA5/SCK PA6/MISO PA7/MOSI）
 ******************************************************************************
 *
 * 硬件连接：
 *   ┌──────────────┐                    ┌──────────────┐
 *   │  STM32F103   │                    │  W25Qxx      │
 *   │              │                    │  (SPI Flash) │
 *   │  PA5  SCK ───┼────────────────────┼── CLK        │
 *   │              │                    │              │
 *   │  PA6 MISO ───┼────────────────────┼── DO (MISO)  │
 *   │              │                    │              │
 *   │  PA7 MOSI ───┼────────────────────┼── DI (MOSI)  │
 *   │              │                    │              │
 *   │  PA4   CS ───┼────────────────────┼── /CS        │
 *   │              │                    │              │
 *   │  3.3V  ──────┼────────────────────┼── VCC        │
 *   │              │                    │  /HOLD=3.3V  │
 *   │              │                    │  /WP=3.3V    │
 *   │  GND   ──────┴────────────────────┴── GND        │
 *   └──────────────┘                    └──────────────┘
 *
 *   注意：
 *     1. W25Qxx是3.3V器件，不能直接接5V
 *     2. /HOLD（暂停）和/WP（写保护）引脚建议接3.3V（禁用）
 *     3. CS引脚用GPIO模拟（软件片选），不使用SPI硬件NSS
 *     4. SPI1的SCK/MISO/MOSI在PA5/PA6/PA7（默认映射）
 *
 *   W25Q系列Flash容量对照：
 *     W25Q64 = 64Mbit = 8MB
 *     W25Q128 = 128Mbit = 16MB
 *     W25Q256 = 256Mbit = 32MB
 *
 * 学习重点：
 *   1. SPI总线的工作原理（SCK、MOSI、MISO、CS四线）
 *   2. SPI的四种工作模式（模式0/1/2/3，CPOL和CPHA）
 *   3. HAL_SPI_TransmitReceive() 全双工收发函数
 *   4. W25Qxx的指令集（读ID、读数据、页编程、扇区擦除）
 *   5. 软件片选控制（GPIO模拟CS）
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

/* ================================================================
 * 宏定义区
 * ================================================================ */

/* SPI外设定义（SPI1） */
#define FLASH_SPI               SPI1

/* 片选引脚（PA4，软件控制）
 * SPI片选是低电平有效，使用GPIO模拟比硬件NSS更灵活
 */
#define FLASH_CS_PORT           GPIOA
#define FLASH_CS_PIN            GPIO_PIN_4

/* LED引脚（PC13，状态指示） */
#define LED_GPIO_PORT           GPIOC
#define LED_GPIO_PIN            GPIO_PIN_13

/* W25Qxx指令集（常用指令）
 * SPI Flash的所有操作都是通过发送指令码开始的
 */
#define W25X_WriteEnable        0x06    /* 写使能 */
#define W25X_WriteDisable       0x04    /* 写禁止 */
#define W25X_ReadStatusReg      0x05    /* 读状态寄存器 */
#define W25X_WriteStatusReg     0x01    /* 写状态寄存器 */
#define W25X_ReadData           0x03    /* 读数据 */
#define W25X_FastReadData       0x0B    /* 快速读数据 */
#define W25X_PageProgram        0x02    /* 页编程（写入） */
#define W25X_BlockErase32K      0x52    /* 32K块擦除 */
#define W25X_BlockErase64K      0xD8    /* 64K块擦除 */
#define W25X_SectorErase        0x20    /* 扇区擦除（4KB） */
#define W25X_ChipErase          0xC7    /* 全片擦除 */
#define W25X_PowerDown          0xB9    /* 掉电模式 */
#define W25X_ReleasePowerDown   0xAB    /* 释放掉电/读设备ID */
#define W25X_DeviceID           0xAB    /* 读设备ID */
#define W25X_ManufactDeviceID   0x90    /* 读厂商ID + 设备ID */
#define W25X_JedecDeviceID      0x9F    /* 读JEDEC ID（厂商+内存类型+容量） */

/* W25Qxx状态寄存器标志位 */
#define W25X_FSR_BUSY           0x01    /* 忙标志位（正在擦除或编程） */
#define W25X_FSR_WEL            0x02    /* 写使能锁存位 */

/* W25Qxx参数（以W25Q64为例，8MB） */
#define W25Q64_PAGE_SIZE        256     /* 页大小：256字节 */
#define W25Q64_SECTOR_SIZE      4096    /* 扇区大小：4KB */
#define W25Q64_BLOCK_SIZE       65536   /* 块大小：64KB */
#define W25Q64_TOTAL_SIZE       0x800000 /* 总容量：8MB = 8*1024*1024 */

/* SPI超时时间 */
#define SPI_TIMEOUT             1000    /* 1秒超时 */

/* ================================================================
 * 函数声明
 * ================================================================ */
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void Error_Handler(void);

/* W25Qxx操作函数声明 */
void W25Q_CS_Enable(void);
void W25Q_CS_Disable(void);
uint32_t W25Q_ReadJEDECID(void);
HAL_StatusTypeDef W25Q_WaitForReady(uint32_t timeout);
HAL_StatusTypeDef W25Q_WriteEnable(void);
HAL_StatusTypeDef W25Q_SectorErase(uint32_t addr);
HAL_StatusTypeDef W25Q_PageProgram(uint32_t addr, uint8_t *pdata, uint16_t size);
HAL_StatusTypeDef W25Q_ReadData(uint32_t addr, uint8_t *pdata, uint32_t size);

/* SPI句柄全局变量 */
SPI_HandleTypeDef hspi1;

/**
 * @brief  主函数：程序入口
 * @retval int
 *
 * 实验流程：
 *   1. 系统初始化（HAL、时钟、GPIO、SPI）
 *   2. 读取W25Qxx的JEDEC ID，验证芯片是否正常
 *   3. 擦除指定扇区
 *   4. 向Flash写入测试数据
 *   5. 从Flash读取数据并校验
 *   6. LED闪烁表示测试结果
 */
int main(void)
{
  /* ========== 第1步：系统初始化 ========== */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* ========== 第2步：读取JEDEC ID，验证芯片 ========== */
  uint32_t jedec_id = W25Q_ReadJEDECID();
  uint8_t test_passed = 1;

  /* W25Q64的JEDEC ID应该是：
   *   厂商ID = 0xEF（Winbond）
   *   内存类型 = 0x40
   *   容量 = 0x17（代表64Mbit）
   * 如果读出来是0xFFFFFF，说明通信失败（SPI没通）
   */
  if ((jedec_id == 0) || (jedec_id == 0xFFFFFF))
  {
    test_passed = 0;
  }

  /* ========== 第3步：Flash读写测试 ========== */
  if (test_passed)
  {
    uint8_t write_buf[W25Q64_PAGE_SIZE];  /* 写入缓冲区（1页=256字节） */
    uint8_t read_buf[W25Q64_PAGE_SIZE];   /* 读取缓冲区 */
    uint32_t test_addr = 0x000000;        /* 测试地址（扇区0起始） */

    /* 填充测试数据（0x00 ~ 0xFF递增） */
    for (uint16_t i = 0; i < W25Q64_PAGE_SIZE; i++)
    {
      write_buf[i] = (uint8_t)i;
    }

    /* ---------- 擦除扇区 ----------
     * Flash写入前必须先擦除（擦除后全是0xFF）
     * 最小擦除单位是1个扇区（4KB）
     */
    if (W25Q_SectorErase(test_addr) != HAL_OK)
    {
      test_passed = 0;
    }
    else
    {
      /* ---------- 写入数据（页编程） ----------
       * 每次最多写1页（256字节）
       * 写入前需要先发送写使能指令
       */
      if (W25Q_PageProgram(test_addr, write_buf, W25Q64_PAGE_SIZE) != HAL_OK)
      {
        test_passed = 0;
      }
      else
      {
        /* ---------- 读取数据 ---------- */
        if (W25Q_ReadData(test_addr, read_buf, W25Q64_PAGE_SIZE) != HAL_OK)
        {
          test_passed = 0;
        }
        else
        {
          /* 逐字节比较 */
          for (uint16_t i = 0; i < W25Q64_PAGE_SIZE; i++)
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
  }

  /* ========== 第4步：主循环 - LED指示测试结果 ========== */
  while (1)
  {
    if (test_passed)
    {
      /* 测试通过：LED慢闪 */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
      HAL_Delay(500);
    }
    else
    {
      /* 测试失败：LED快闪 */
      HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
      HAL_Delay(100);
    }
  }
}

/* ================================================================
 * W25Qxx底层操作函数
 * ================================================================ */

/**
 * @brief  拉低CS引脚（选中芯片）
 * @note   SPI片选是低电平有效
 *         每次SPI通信前必须拉低CS，通信结束后拉高
 */
void W25Q_CS_Enable(void)
{
  HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET);
}

/**
 * @brief  拉高CS引脚（释放芯片）
 */
void W25Q_CS_Disable(void)
{
  HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief  读取JEDEC ID
 * @retval 32位JEDEC ID（格式：0x00 + 厂商ID + 内存类型 + 容量）
 *
 * JEDEC ID指令时序：
 *   主机发送：0x9F（指令码）
 *   从机回复：厂商ID(1字节) + 内存类型(1字节) + 容量(1字节)
 *
 * 例如W25Q64返回：0xEF 0x40 0x17
 *   - 0xEF = Winbond（华邦）
 *   - 0x40 = SPI Flash类型
 *   - 0x17 = 64Mbit（容量编码：0x10=1M, 0x11=2M, ... 0x17=64M, 0x18=128M）
 */
uint32_t W25Q_ReadJEDECID(void)
{
  uint8_t tx_buf[4] = {W25X_JedecDeviceID, 0, 0, 0};  /* 发送：指令 + 3个dummy */
  uint8_t rx_buf[4] = {0};                              /* 接收缓冲区 */

  /* 拉低CS，开始通信 */
  W25Q_CS_Enable();

  /* 全双工收发：发4字节，收4字节
   * 第1字节发指令时，收到的是无效数据
   * 第2~4字节发dummy时，收到的是ID数据
   */
  HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, 4, SPI_TIMEOUT);

  /* 拉高CS，结束通信 */
  W25Q_CS_Disable();

  /* 组合成32位ID（rx_buf[1]=厂商, rx_buf[2]=类型, rx_buf[3]=容量） */
  return ((uint32_t)rx_buf[1] << 16) | ((uint32_t)rx_buf[2] << 8) | rx_buf[3];
}

/**
 * @brief  等待Flash就绪（忙等待）
 * @param  timeout: 超时时间（毫秒）
 * @retval HAL状态
 *
 * Flash在执行擦除和编程操作时，状态寄存器的BUSY位会置1。
 * 这个函数循环读取状态寄存器，直到BUSY位清零。
 *
 * 状态寄存器格式：
 *   Bit 7   6    5    4    3    2    1    0
 *   ┌────┬────┬────┬────┬────┬────┬────┬────┐
 *   │SRP │TB  │ BP2│ BP1│ BP0│ WEL│─── │BUSY│
 *   └────┴────┴────┴────┴────┴────┴────┴────┘
 *   BUSY=1表示正在执行操作，BUSY=0表示空闲
 */
HAL_StatusTypeDef W25Q_WaitForReady(uint32_t timeout)
{
  uint8_t status = 0;
  uint32_t tick_start = HAL_GetTick();

  do
  {
    /* 读状态寄存器指令：0x05 */
    W25Q_CS_Enable();
    HAL_SPI_Transmit(&hspi1, (uint8_t[]){W25X_ReadStatusReg}, 1, SPI_TIMEOUT);
    HAL_SPI_Receive(&hspi1, &status, 1, SPI_TIMEOUT);
    W25Q_CS_Disable();

    /* 检查超时 */
    if ((HAL_GetTick() - tick_start) > timeout)
    {
      return HAL_TIMEOUT;
    }

    /* 小延时，避免连续读太频繁 */
    HAL_Delay(1);

  } while (status & W25X_FSR_BUSY);  /* BUSY位为1就继续等 */

  return HAL_OK;
}

/**
 * @brief  写使能
 * @retval HAL状态
 *
 * 在执行任何写操作（页编程、扇区擦除、写状态寄存器）之前，
 * 必须先发送写使能指令（0x06），让状态寄存器的WEL位置1。
 *
 * 为什么要有写使能？
 *   这是一种保护机制，防止误操作导致数据丢失。
 *   每次写操作完成后，WEL位会自动清零。
 */
HAL_StatusTypeDef W25Q_WriteEnable(void)
{
  uint8_t cmd = W25X_WriteEnable;

  W25Q_CS_Enable();
  HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, &cmd, 1, SPI_TIMEOUT);
  W25Q_CS_Disable();

  return status;
}

/**
 * @brief  扇区擦除（4KB）
 * @param  addr: 扇区地址（扇区内任意地址都可以，会自动对齐）
 * @retval HAL状态
 *
 * 扇区擦除指令时序：
 *   1. 发送写使能指令（0x06）
 *   2. 拉低CS
 *   3. 发送扇区擦除指令（0x20）
 *   4. 发送24位地址（A23~A0）
 *   5. 拉高CS
 *   6. 等待擦除完成（典型45ms，最大400ms）
 *
 * 注意：
 *   - 擦除后整个扇区的数据都变成0xFF
 *   - Flash只能把1写成0（编程），不能把0写成1（必须擦除）
 *   - 所以写入前必须先擦除
 */
HAL_StatusTypeDef W25Q_SectorErase(uint32_t addr)
{
  HAL_StatusTypeDef status;

  /* 第1步：写使能 */
  status = W25Q_WriteEnable();
  if (status != HAL_OK) return status;

  /* 第2步：发送擦除指令和地址 */
  uint8_t tx_buf[4];
  tx_buf[0] = W25X_SectorErase;           /* 指令码 */
  tx_buf[1] = (addr >> 16) & 0xFF;        /* 地址高8位（A23~A16） */
  tx_buf[2] = (addr >> 8) & 0xFF;         /* 地址中8位（A15~A8） */
  tx_buf[3] = addr & 0xFF;                /* 地址低8位（A7~A0） */

  W25Q_CS_Enable();
  status = HAL_SPI_Transmit(&hspi1, tx_buf, 4, SPI_TIMEOUT);
  W25Q_CS_Disable();

  if (status != HAL_OK) return status;

  /* 第3步：等待擦除完成（超时设为1秒，足够大） */
  return W25Q_WaitForReady(1000);
}

/**
 * @brief  页编程（写入数据）
 * @param  addr: 起始地址
 * @param  pdata: 数据指针
 * @param  size: 数据长度（最大256字节，且不能跨页边界）
 * @retval HAL状态
 *
 * 页编程指令时序：
 *   1. 发送写使能指令（0x06）
 *   2. 拉低CS
 *   3. 发送页编程指令（0x02）
 *   4. 发送24位地址
 *   5. 发送最多256字节数据（1页）
 *   6. 拉高CS
 *   7. 等待编程完成（典型0.7ms，最大3ms）
 *
 * 注意：
 *   - 每次最多写1页（256字节）
 *   - 如果写入数据跨页边界，会回卷到本页开头，覆盖前面的数据
 *   - 写入前必须确保目标地址已经被擦除（全0xFF）
 */
HAL_StatusTypeDef W25Q_PageProgram(uint32_t addr, uint8_t *pdata, uint16_t size)
{
  HAL_StatusTypeDef status;

  /* 参数检查 */
  if (size > W25Q64_PAGE_SIZE) return HAL_ERROR;

  /* 第1步：写使能 */
  status = W25Q_WriteEnable();
  if (status != HAL_OK) return status;

  /* 第2步：发送指令+地址+数据 */
  W25Q_CS_Enable();

  /* 先发指令码 */
  uint8_t cmd = W25X_PageProgram;
  status = HAL_SPI_Transmit(&hspi1, &cmd, 1, SPI_TIMEOUT);
  if (status != HAL_OK) goto end;

  /* 再发24位地址 */
  uint8_t addr_buf[3];
  addr_buf[0] = (addr >> 16) & 0xFF;
  addr_buf[1] = (addr >> 8) & 0xFF;
  addr_buf[2] = addr & 0xFF;
  status = HAL_SPI_Transmit(&hspi1, addr_buf, 3, SPI_TIMEOUT);
  if (status != HAL_OK) goto end;

  /* 最后发数据 */
  status = HAL_SPI_Transmit(&hspi1, pdata, size, SPI_TIMEOUT);

end:
  W25Q_CS_Disable();

  if (status != HAL_OK) return status;

  /* 第3步：等待编程完成 */
  return W25Q_WaitForReady(100);
}

/**
 * @brief  读取数据
 * @param  addr: 起始地址
 * @param  pdata: 数据缓冲区指针
 * @param  size: 读取长度（字节数）
 * @retval HAL状态
 *
 * 读数据指令时序：
 *   1. 拉低CS
 *   2. 发送读数据指令（0x03）
 *   3. 发送24位地址
 *   4. 连续读取数据（地址自动递增，想读多少读多少）
 *   5. 拉高CS
 *
 * 注意：
 *   - 读操作没有页限制，可以连续读取整个芯片
 *   - 普通读取（0x03）最高频率约33MHz
 *   - 快速读取（0x0B）需要发一个dummy字节，但速度更快
 */
HAL_StatusTypeDef W25Q_ReadData(uint32_t addr, uint8_t *pdata, uint32_t size)
{
  HAL_StatusTypeDef status;

  W25Q_CS_Enable();

  /* 先发指令码 */
  uint8_t cmd = W25X_ReadData;
  status = HAL_SPI_Transmit(&hspi1, &cmd, 1, SPI_TIMEOUT);
  if (status != HAL_OK) goto end;

  /* 再发24位地址 */
  uint8_t addr_buf[3];
  addr_buf[0] = (addr >> 16) & 0xFF;
  addr_buf[1] = (addr >> 8) & 0xFF;
  addr_buf[2] = addr & 0xFF;
  status = HAL_SPI_Transmit(&hspi1, addr_buf, 3, SPI_TIMEOUT);
  if (status != HAL_OK) goto end;

  /* 最后接收数据 */
  status = HAL_SPI_Receive(&hspi1, pdata, size, SPI_TIMEOUT);

end:
  W25Q_CS_Disable();
  return status;
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
 * @note   PA4为输出（CS片选），PC13为输出（LED）
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能GPIOA和GPIOC时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* ---------- 配置PA4为推挽输出（Flash CS片选） ---------- */
  GPIO_InitStruct.Pin = FLASH_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(FLASH_CS_PORT, &GPIO_InitStruct);

  /* CS初始状态：高电平（未选中） */
  HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET);

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
 * @brief  SPI1初始化函数
 * @note   配置SPI1为主机模式，全双工，8位数据，模式0
 *         SCK -> PA5, MISO -> PA6, MOSI -> PA7
 *
 * SPI的四种工作模式（CPOL和CPHA组合）：
 *
 *   模式  CPOL  CPHA  空闲时SCK电平  采样边沿
 *   ─────────────────────────────────────────
 *    0     0     0        低电平       上升沿
 *    1     0     1        低电平       下降沿
 *    2     1     0        高电平       下降沿
 *    3     1     1        高电平       上升沿
 *
 *   CPOL（Clock Polarity）：时钟极性
 *     - CPOL=0：空闲时SCK为低电平
 *     - CPOL=1：空闲时SCK为高电平
 *
 *   CPHA（Clock Phase）：时钟相位
 *     - CPHA=0：在SCK的第1个边沿（奇数边沿）采样数据
 *     - CPHA=1：在SCK的第2个边沿（偶数边沿）采样数据
 *
 * 常用的Flash芯片（W25Qxx）支持模式0和模式3：
 *   - 模式0：CPOL=0, CPHA=0（空闲低电平，上升沿采样）
 *   - 模式3：CPOL=1, CPHA=1（空闲高电平，上升沿采样）
 *   两种模式数据采样都是在上升沿，区别只是空闲电平。
 *   本示例使用模式0（最常用）。
 *
 * SPI频率计算：
 *   SPI1挂在APB2总线上（PCLK2 = 72MHz）
 *   通过波特率预分频器分频得到SCK频率：
 *     SPI_BAUDRATEPRESCALER_2   → 72/2 = 36MHz
 *     SPI_BAUDRATEPRESCALER_4   → 72/4 = 18MHz
 *     SPI_BAUDRATEPRESCALER_8   → 72/8 = 9MHz
 *     SPI_BAUDRATEPRESCALER_16  → 72/16 = 4.5MHz
 *     SPI_BAUDRATEPRESCALER_32  → 72/32 = 2.25MHz
 *     SPI_BAUDRATEPRESCALER_64  → 72/64 = 1.125MHz
 *     SPI_BAUDRATEPRESCALER_128 → 72/128 = 562.5kHz
 *     SPI_BAUDRATEPRESCALER_256 → 72/256 = 281.25kHz
 *
 *   W25Qxx的普通读模式最高支持约33MHz，
 *   这里设置为9MHz（8分频），比较保守，稳定可靠。
 */
static void MX_SPI1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ---------- 第1步：使能时钟 ----------
   * SPI1挂在APB2总线上
   * GPIOA挂在APB2总线上
   */
  __HAL_RCC_SPI1_CLK_ENABLE();     /* 使能SPI1时钟 */
  __HAL_RCC_GPIOA_CLK_ENABLE();    /* 使能GPIOA时钟 */

  /* ---------- 第2步：配置SPI引脚（复用推挽） ----------
   * PA5 -> SCK  （串行时钟）
   * PA6 -> MISO （主入从出）
   * PA7 -> MOSI （主出从入）
   *
   * 这三个引脚配置为复用推挽输出模式（AF_PP）
   * 因为是SPI主机，SCK和MOSI由主机输出，MISO是输入
   * 配置成AF_PP后，HAL库会自动处理方向
   */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;          /* 复用推挽输出 */
  GPIO_InitStruct.Pull = GPIO_NOPULL;              /* 不需要上下拉 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;    /* 高速（SPI需要高速） */

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ---------- 第3步：配置SPI外设 ----------
   * SPI初始化结构体参数说明：
   *
   * Mode：SPI模式（主机/从机）
   *   SPI_MODE_MASTER = 主机模式
   *   SPI_MODE_SLAVE  = 从机模式
   *
   * Direction：通信方向
   *   SPI_DIRECTION_2LINES        = 双线全双工（MOSI+MISO）
   *   SPI_DIRECTION_2LINES_RXONLY = 双线只读（MOSI不用）
   *   SPI_DIRECTION_1LINE         = 单线半双工
   *
   * DataSize：数据宽度
   *   SPI_DATASIZE_8BIT  = 8位数据
   *   SPI_DATASIZE_16BIT = 16位数据
   *
   * CLKPolarity：时钟极性（CPOL）
   *   SPI_POLARITY_LOW  = 空闲时SCK为低电平（CPOL=0）
   *   SPI_POLARITY_HIGH = 空闲时SCK为高电平（CPOL=1）
   *
   * CLKPhase：时钟相位（CPHA）
   *   SPI_PHASE_1EDGE = 第1个边沿采样（CPHA=0）
   *   SPI_PHASE_2EDGE = 第2个边沿采样（CPHA=1）
   *
   * NSS：片选信号控制方式
   *   SPI_NSS_SOFT = 软件控制（用GPIO模拟CS）
   *   SPI_NSS_HARD = 硬件控制（使用SPI的NSS引脚）
   *   一般都用软件控制，更灵活
   *
   * BaudRatePrescaler：波特率预分频
   *   决定SPI时钟频率 = PCLK2 / 预分频系数
   *
   * FirstBit：数据传输位序
   *   SPI_FIRSTBIT_MSB = 高位在前（MSB First）
   *   SPI_FIRSTBIT_LSB = 低位在前（LSB First）
   *   大多数SPI器件都是MSB First
   *
   * TIMode：TI模式（禁用，SPI标准模式）
   *
   * CRCCalculation：CRC校验（禁用，一般不需要）
   */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;                  /* 主机模式 */
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;        /* 双线全双工 */
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;            /* 8位数据宽度 */
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;          /* CPOL=0，空闲低电平 */
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;              /* CPHA=0，第1边沿采样 */
  hspi1.Init.NSS = SPI_NSS_SOFT;                      /* 软件片选 */
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; /* 8分频 = 72/8 = 9MHz */
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;             /* 高位在前 */
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;             /* 禁用TI模式 */
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; /* 禁用CRC */
  hspi1.Init.CRCPolynomial = 10;                       /* CRC多项式（禁用时无效） */

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
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
 * 知识补充：SPI通信协议详解
 * ================================================================
 *
 * 一、SPI总线简介
 *   SPI（Serial Peripheral Interface）是摩托罗拉公司发明的串行外设接口。
 *   特点：
 *     - 四根线：SCK（时钟）+ MOSI（主出从入）+ MISO（主入从出）+ CS（片选）
 *     - 全双工通信（收发同时进行）
 *     - 同步通信（时钟同步）
 *     - 只有一个主机，可以有多个从机（每个从机一个CS）
 *     - 没有应答机制，没有地址概念（靠CS选设备）
 *     - 速度比I2C快很多（可达几十MHz）
 *
 * 二、SPI vs I2C 对比
 *
 *   特性          SPI              I2C
 *   ─────────────────────────────────────────
 *   信号线        4根（+CS）       2根
 *   通信方式      全双工           半双工
 *   速度          快（几十MHz）    慢（最高400kHz/3.4MHz）
 *   设备寻址      CS片选           7/10位地址
 *   多主机        不支持           支持
 *   应答机制      无               有ACK/NACK
 *   上拉电阻      不需要           需要
 *   引脚配置      推挽输出         开漏输出
 *   适用场景      高速、短距离     低速、多设备、长距离
 *
 * 三、SPI数据传输过程
 *
 *   主机SCK:  ──┐   ┌───┐   ┌───┐   ┌───┐
 *              └───┘   └───┘   └───┘   └──
 *
 *   主机MOSI:  ────┐  D7  │  D6  │  D5  │───
 *                  └───────┴───────┴───────┘
 *
 *   从机MISO:  ────┐  D7  │  D6  │  D5  │───
 *                  └───────┴───────┴───────┘
 *
 *   特点：
 *   1. 主机输出SCK时钟信号，控制传输节奏
 *   2. 每个SCK周期，主机和从机各移位1位
 *   3. 发送和接收是同时进行的（全双工）
 *   4. CS拉低期间传输有效
 *
 * 四、W25Qxx Flash重要概念
 *
 * 1. 为什么需要先擦除再写入？
 *    Flash存储单元的物理特性决定了：
 *      - 擦除操作：把所有位变成1（0xFF）
 *      - 编程操作：把1变成0（只能写0，不能写1）
 *    所以要写某个位置，必须确保它先被擦除（全1状态）。
 *    如果目标地址已经是0xFF，就可以直接写。
 *
 * 2. 擦除单位和写入单位
 *    - 擦除：最小单位是扇区（4KB），也可以按块（32K/64K）或整片擦除
 *    - 写入：最小单位是1字节，最大单位是1页（256字节）
 *    - 读取：任意字节数，没有限制
 *
 * 3. Flash寿命
 *    - 擦写次数：约10万次（SLC NAND Flash）
 *    - 数据保存：约20年
 *    - 注意：频繁擦写的区域要做磨损均衡（Wear Leveling）
 *
 * 五、常见问题
 *
 * 1. 读取JEDEC ID全是0xFF？
 *    - 检查CS引脚是否正确拉低
 *    - 检查SPI模式是否正确（CPOL/CPHA）
 *    - 检查引脚接线（MOSI接DI，MISO接DO，不要搞反）
 *    - 检查电源和地
 *
 * 2. 写入后读出来不对？
 *    - 有没有先擦除？（写入前必须擦除）
 *    - 有没有等写操作完成？（检查BUSY位）
 *    - 有没有跨页写入？（页编程不能超过256字节，不能跨页边界）
 *
 * 3. HAL_SPI_TransmitReceive的作用？
 *    SPI是全双工的，发数据的同时也在收数据。
 *    HAL_SPI_TransmitReceive同时处理发送和接收。
 *    如果只需要发（不需要收），用HAL_SPI_Transmit。
 *    如果只需要收（不需要发），用HAL_SPI_Receive（内部会发dummy数据）。
 *
 * ================================================================ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
