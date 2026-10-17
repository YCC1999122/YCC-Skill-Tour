# STM32 学习项目

## 项目简介

本项目是一套基于 STM32F103C8T6 的完整学习教程，从 GPIO 基础操作到综合应用实例，循序渐进地涵盖 STM32 开发的核心知识点。

本项目的核心特点：

- **目标芯片**：STM32F103C8T6，采用 ARM Cortex-M3 内核，主频 72MHz，外部 8MHz 晶振（HSE）
- **开发方式**：基于 HAL 库开发，配合 STM32CubeMX 图形化配置工具
- **代码风格**：每个 `.c` 文件都配有极其详细的逐行中文注释，适合零基础入门到进阶
- **完整工程框架**：每个示例文件都包含完整的 `main` 函数、`SystemClock_Config` 时钟配置函数、`Error_Handler` 错误处理函数，可以直接作为独立工程的参考模板
- **学习路径**：按照 GPIO → 定时器 → 外部中断 → 串口通信 → I2C → SPI → ADC → 综合实例 的顺序，由浅入深地掌握嵌入式开发

## 目录结构

```
stm32-learning-project/
├── 01_GPIO/                     # 第1章：GPIO操作
│   ├── 01_led_blink.c           # LED闪烁（推挽输出、HAL_GPIO_TogglePin、HAL_Delay）
│   └── 02_key_input.c           # 按键输入（上拉输入、GPIO读取、消抖）
├── 02_TIM/                      # 第2章：定时器
│   ├── 01_timer_basic.c         # 基本定时器（TIM6、定时中断、Period/Prescaler计算、1秒LED闪烁）
│   └── 02_timer_pwm.c           # PWM输出（TIM3 CH1 PA6、呼吸灯、占空比调节、AFIO复用）
├── 03_EXTI/                     # 第3章：外部中断
│   └── 01_exti_key.c            # 按键外部中断（EXTI配置、下降沿触发、HAL_GPIO_EXTI_Callback回调、时间差消抖）
├── 04_UART/                     # 第4章：串口通信
│   ├── 01_uart_send.c           # 串口发送（USART1 PA9/PA10、9600波特率、HAL_UART_Transmit、printf重定向）
│   └── 02_uart_it_receive.c     # 串口中断接收（HAL_UART_Receive_IT、环形缓冲区、命令行解析CLI）
├── 05_I2C/                      # 第5章：I2C通信
│   └── 01_i2c_eeprom.c          # I2C读写EEPROM（I2C1 PB6/PB7、24C02、HAL_I2C_Mem_Write/Read、页写入）
├── 06_SPI/                      # 第6章：SPI通信
│   └── 01_spi_w25qxx.c          # SPI读写Flash（SPI1 PA5/PA6/PA7、W25Qxx、HAL_SPI_TransmitReceive、4种模式）
├── 07_ADC/                      # 第7章：ADC采集
│   └── 01_adc_single.c          # ADC单通道采集（ADC1 PA0、单次转换、HAL_ADC_Start/GetValue、电压换算、DMA补充）
├── 08_Examples/                 # 第8章：综合实例
│   └── 01_digital_clock.c       # 数字时钟（RTC实时时钟+LCD1602显示+按键调整、BCD码、状态机菜单）
├── Core/                         # 核心文件
│   ├── main_template.c          # 主函数模板（系统时钟配置72MHz、GPIO初始化框架、HAL库回调机制）
│   └── stm32f1xx_hal_conf.h     # HAL库配置文件
└── README_STM32.md              # 本文件
```

## 学习路径建议

本教程按照由浅入深的顺序划分为 8 个阶段，建议按顺序学习，每个阶段都建立在前一阶段的基础之上。

### 第一阶段：GPIO操作（01_GPIO）

GPIO 是 STM32 入门的第一步，掌握 GPIO 就掌握了最基础的外设控制能力。

- **01_led_blink.c** — LED闪烁
  - GPIO 的 8 种工作模式（推挽输出、开漏输出、上拉输入、下拉输入、浮空输入、复用推挽、复用开漏、模拟输入）
  - 推挽输出与开漏输出的区别及适用场景
  - 时钟使能（`__HAL_RCC_GPIOx_CLK_ENABLE`）的必要性
  - `HAL_GPIO_WritePin` / `HAL_GPIO_TogglePin` 的使用
  - `HAL_Delay` 阻塞延时函数的原理与局限

- **02_key_input.c** — 按键输入
  - 上拉输入模式的配置
  - `HAL_GPIO_ReadPin` 读取引脚电平
  - 软件消抖原理（延时消抖、状态机消抖）
  - 按键按下与释放事件的检测

### 第二阶段：定时器（02_TIM）

定时器是 STM32 最核心的外设之一，掌握定时器就掌握了精确的时间控制和 PWM 输出能力。

- **01_timer_basic.c** — 基本定时器
  - TIM6 基本定时器的特点（只有定时中断功能，无 GPIO 输出）
  - 定时时间的计算公式：`T = (Period + 1) × (Prescaler + 1) / Freq`
  - HAL 库定时器中断的回调机制（`HAL_TIM_PeriodElapsedCallback`）
  - 实现 1 秒 LED 闪烁的完整流程

- **02_timer_pwm.c** — PWM输出
  - TIM3 通用定时器的 PWM 模式配置
  - PWM 频率与占空比的计算方法
  - AFIO 复用功能时钟使能的重要性（`__HAL_RCC_AFIO_CLK_ENABLE`）
  - 引脚复用映射（PA6 → TIM3_CH1）
  - `HAL_TIM_PWM_Start` 启动 PWM 输出
  - 呼吸灯实现（占空比渐变循环）

### 第三阶段：外部中断（03_EXTI）

外部中断让 STM32 能够响应外部事件，是实现低功耗和实时响应的关键。

- **01_exti_key.c** — 按键外部中断
  - EXTI 线与 GPIO 的映射关系（EXTI0 对应 PA0/PB0/PC0...）
  - 下降沿触发与上升沿触发的选择
  - NVIC 中断优先级配置（抢占优先级与响应优先级）
  - `HAL_GPIO_EXTI_Callback` 回调函数的使用（弱定义函数重写）
  - 中断服务函数中基于时间差的消抖方法（利用 `HAL_GetTick`）

### 第四阶段：串口通信（04_UART）

串口通信是嵌入式开发中最常用的通信方式，也是调试的重要手段。

- **01_uart_send.c** — 串口发送
  - USART1 的配置（波特率、数据位、停止位、校验位）
  - 波特率计算原理（`Baud = Fclk / (16 × USARTDIV)`）
  - `HAL_UART_Transmit` 阻塞发送
  - `printf` 重定向到串口（重写 `fputc` 函数）
  - Keil 中勾选 Use MicroLIB 的必要性

- **02_uart_it_receive.c** — 串口中断接收
  - `HAL_UART_Receive_IT` 非阻塞接收的原理
  - 接收完成回调函数 `HAL_UART_RxCpltCallback`
  - 环形缓冲区（Ring Buffer）的设计与实现
  - 命令行解析（CLI）的简单实现思路
  - 变量 `volatile` 关键字在中断与主循环共享变量中的重要性

### 第五阶段：I2C通信（05_I2C）

I2C 是常见的两线制串行通信协议，广泛用于连接 EEPROM、传感器、OLED 屏幕等外设。

- **01_i2c_eeprom.c** — I2C读写EEPROM
  - I2C 时序（起始、停止、应答、非应答）
  - I2C 设备地址（7位地址 + 读写位）
  - 24C02 EEPROM 的存储结构（256 字节，页大小 8 字节）
  - `HAL_I2C_Mem_Write` / `HAL_I2C_Mem_Read` 的使用
  - 页写入跨页处理（页写入不能跨页，否则回卷覆盖）
  - 写入后需要等待 `5ms` 的写周期

### 第六阶段：SPI通信（06_SPI）

SPI 是高速全双工串行通信协议，常用于连接 Flash、SD 卡、LCD 屏幕等高速外设。

- **01_spi_w25qxx.c** — SPI读写Flash
  - SPI 的 4 种模式（CPOL/CPHA 组合）
  - 全双工通信原理（主机与从机同时发送和接收）
  - W25Qxx Flash 的指令集（JEDEC ID、读数据、页编程、扇区擦除）
  - `HAL_SPI_TransmitReceive` 全双工收发
  - Flash 写入前必须先擦除的特性
  - 等待 Flash 繁忙位（Status Register 的 WIP 位）

### 第七阶段：ADC采集（07_ADC）

ADC 是连接模拟世界与数字世界的桥梁，掌握 ADC 就能采集传感器信号。

- **01_adc_single.c** — ADC单通道采集
  - ADC 框图与转换原理（逐次逼近型 SAR）
  - 单次转换模式的配置
  - `HAL_ADC_Start` / `HAL_ADC_GetValue` 的使用
  - 电压换算公式：`V = ADC_Value × 3.3V / 4096`
  - ADC 校准（`HAL_ADCEx_Calibration_Start`）
  - DMA 方式采集的补充说明（连续高效采集）
  - 精度提升方法（多次采样取平均）

### 第八阶段：综合实例（08_Examples）

综合实例将前面学到的知识融合运用，培养完整的项目开发能力。

- **01_digital_clock.c** — 数字时钟
  - RTC 实时时钟的配置与使用
  - BCD 码与十进制的相互转换
  - LCD1602 的 4 位数据线驱动方式
  - 状态机菜单设计（设置时、分、秒）
  - 按键调整时间的逻辑
  - 多模块协同工作的工程组织

## 开发环境

### 1. STM32CubeMX（图形化配置工具，强烈推荐）

- **用途**：通过图形化界面配置引脚、时钟、外设，自动生成 HAL 库初始化代码
- **下载地址**：https://www.st.com/en/development-tools/stm32cubemx.html
- **版本**：免费，持续更新
- **优势**：
  - 可视化引脚分配，避免冲突
  - 时钟树图形化配置，直观清晰
  - 一键生成工程，支持 Keil、CubeIDE、Makefile 等多种工具链
  - 学习本项目时建议用 CubeMX 打开对照配置，加深理解

### 2. Keil MDK-ARM（主要IDE）

- **用途**：STM32 主流开发 IDE，编译、下载、调试一体化
- **下载地址**：https://www.keil.com/download/
- **版本**：MDK-ARM，可免费申请 MDK Community 版（限制 Flash 大小到 32KB）
- **特点**：
  - 行业标准 IDE，资料最丰富
  - 调试功能强大（断点、变量监视、寄存器查看、逻辑分析仪）
  - 支持 ST-Link、J-Link 等下载器
  - 推荐申请社区版：https://www2.keil.com/mdk5/install

### 3. STM32CubeIDE（免费官方IDE）

- **用途**：ST 官方免费 IDE，基于 Eclipse 深度定制
- **下载地址**：https://www.st.com/en/development-tools/stm32cubeide.html
- **特点**：
  - 完全免费，无代码大小限制
  - 内置 CubeMX，配置与编码一体化
  - 跨平台支持 Windows、Linux、macOS
  - 适合不想使用 Keil 的开发者

### 4. 下载器与硬件工具

| 工具 | 价格 | 说明 |
|------|------|------|
| ST-Link V2 下载器 | 约 15-30 元 | SWD 接口下载调试，必备工具，注意购买正版或品质可靠的版本 |
| CH340 USB 转串口 | 约 5-15 元 | 串口通信调试、烧录 HEX 文件、查看 printf 输出 |
| USB-TTL 模块 | 约 5-15 元 | 连接 USART 进行串口通信调试 |
| 逻辑分析仪 | 约 50-150 元 | 分析 I2C、SPI、UART 时序，调试通信协议的利器 |

### 5. 推荐开发板

| 开发板 | 芯片 | 价格 | 说明 |
|--------|------|------|------|
| STM32F103C8T6 最小系统板 | STM32F103C8T6 | 约 10-30 元 | 本项目目标板，性价比高，资料丰富，入门首选 |
| STM32F103ZET6 开发板 | STM32F103ZET6 | 约 80-200 元 | 大 Flash/RAM、丰富外设，适合进阶 |
| 正点原子精英版 | STM32F103ZET6 | 约 200-300 元 | 配套丰富例程与教程，资料齐全 |
| 野火指南者 | STM32F103ZET6 | 约 200-300 元 | 配套详细教程与视频，适合系统学习 |

## STM32基础知识

### STM32F103C8T6 参数一览

| 参数 | 说明 |
|------|------|
| 内核 | ARM Cortex-M3 |
| 最高主频 | 72MHz |
| Flash 容量 | 64KB |
| SRAM 容量 | 20KB |
| GPIO 引脚数 | 37 个（LQFP48 封装） |
| 定时器 | 1 个高级定时器（TIM1） + 3 个通用定时器（TIM2/3/4） + 2 个基本定时器（TIM6/7） |
| ADC | 3 个 ADC（12 位逐次逼近型），最多 10 个外部通道 |
| I2C | 2 个（I2C1、I2C2） |
| SPI | 2 个（SPI1、SPI2） |
| USART | 3 个（USART1、USART2、USART3） |
| USB | 1 个（USB 2.0 全速） |
| CAN | 1 个（CAN 总线） |
| 外部晶振 | 8MHz（HSE） |
| 内部晶振 | 8MHz（HSI，精度约 1%） |
| 供电电压 | 2.0V - 3.6V |
| 典型封装 | LQFP48 |

### 时钟树

STM32F103 的时钟系统较为复杂，理解时钟树是配置外设的前提。

```
                    HSE 外部晶振 8MHz
                          │
                          ▼
                    ┌──────────┐
                    │  PLL 输入  │  可选 HSE / HSI /2
                    │  分频选择  │
                    └──────────┘
                          │
                          ▼
                    ┌──────────┐
                    │  PLL 倍频  │  ×9
                    └──────────┘
                          │
                          ▼
                    72MHz (PLLCLK)
                          │
                ┌─────────┴─────────┐
                ▼                   ▼
          SYSCLK 72MHz          (其他)
                │
                ▼
          ┌──────────┐
          │  AHB 分频  │  /1
          └──────────┘
                │
                ▼
          HCLK 72MHz ──────► CPU 内核、DMA、AHB 总线
                │
        ┌───────┴───────┐
        ▼               ▼
   ┌──────────┐   ┌──────────┐
   │ APB1 分频 │   │ APB2 分频 │
   │   /2     │   │   /1     │
   └──────────┘   └──────────┘
        │               │
        ▼               ▼
   APB1 36MHz      APB2 72MHz
   (定时器×2=72MHz) (定时器×1=72MHz)
```

**关键说明：**

- HSE 8MHz 经 PLL 倍频 9 倍得到 72MHz 的 SYSCLK
- HCLK（AHB 总线时钟）= 72MHz，供 CPU 与 AHB 总线外设使用
- APB1 总线时钟（PCLK1）= 36MHz，挂载 USART2/3、I2C1/2、SPI2、TIM2/3/4/6/7、USB 等
- APB2 总线时钟（PCLK2）= 72MHz，挂载 USART1、SPI1、ADC1/2/3、TIM1、GPIO 等
- 当 APB1 分频不为 1 时，挂载在 APB1 上的定时器时钟会自动 ×2（即 72MHz）
- 当 APB2 分频不为 1 时，挂载在 APB2 上的定时器时钟会自动 ×2

### HAL库 vs 标准库 vs 寄存器

STM32 开发有三种主流方式，各有优缺点：

| 对比项 | HAL 库 | 标准库（SPL） | 寄存器 |
|--------|--------|---------------|--------|
| 抽象层级 | 高 | 中 | 低 |
| 代码量 | 大（库函数封装） | 中 | 小（直接操作寄存器） |
| 学习难度 | 低 | 中 | 高 |
| 可移植性 | 强（跨芯片系列） | 弱（仅限同系列） | 弱 |
| 执行效率 | 中 | 中 | 高 |
| 与 CubeMX 配合 | 原生支持 | 不支持 | 不支持 |
| 资料丰富度 | 新资料多，越来越丰富 | 老资料多，新芯片停更 | 需查手册 |
| 维护状态 | ST 主推，持续更新 | 已停更，不推荐新项目 | 永久 |
| 推荐场景 | 新项目、初学者、需快速开发 | 维护老项目 | 性能极致优化场合 |

**HAL 库的核心机制：**

- **回调机制**：HAL 库大量使用回调函数，外设事件发生时自动调用用户重写的回调函数（如 `HAL_GPIO_EXTI_Callback`、`HAL_UART_RxCpltCallback`）
- **弱定义函数**：回调函数使用 `__weak` 关键字声明，用户在自己的代码中重新定义同名函数即可覆盖默认实现，无需修改库代码
- **句柄结构体**：每个外设都有一个 `_HandleTypeDef` 句柄结构体，包含配置参数、状态、错误码等信息，贯穿 HAL 库的所有操作

### 存储器映射

STM32F103 的存储器映射遵循 ARM Cortex-M3 的统一编址方案：

| 地址范围 | 区域 | 说明 |
|----------|------|------|
| `0x00000000 - 0x07FFFFFF` | 代码区 | Flash 镜像，启动后从此处执行 |
| `0x08000000 - 0x0800FFFF` | Flash | 主 Flash 区域（C8T6 为 64KB，即 `0x08000000 - 0x0800FFFF`） |
| `0x1FFFF000 - 0x1FFFF7FF` | 系统存储区 | 出厂 bootloader，支持串口下载 |
| `0x1FFFF800 - 0x1FFFF80F` | 选项字节 | 配置读写保护、看门狗等 |
| `0x20000000 - 0x20004FFF` | SRAM | 片内 SRAM（C8T6 为 20KB） |
| `0x40000000 - 0x400077FF` | 外设区（APB1） | APB1 总线外设寄存器 |
| `0x40010000 - 0x40013FFF` | 外设区（APB2） | APB2 总线外设寄存器 |
| `0x40018000 - 0x40023FFF` | 外设区（AHB） | AHB 总线外设寄存器（DMA、RCC、GPIO 等） |
| `0xE0000000 - 0xE00FFFFF` | 系统区 | Cortex-M3 内部外设（NVIC、SysTick、SCB 等） |

## 学习方法建议

1. **先看 main_template.c 理解框架**：`Core/main_template.c` 是所有示例的基础模板，先理解它就能掌握每个示例的整体结构（main → SystemClock_Config → MX_*_Init → while 循环）

2. **逐行阅读注释**：每个 `.c` 文件都有极其详细的逐行中文注释，从寄存器位定义到 HAL 库函数用法都有解释，建议逐行精读，理解每一行代码的作用

3. **用 STM32CubeMX 对照配置**：在 CubeMX 中新建工程，按照示例中的引脚和外设配置进行操作，对照生成的代码与示例代码，加深对配置流程的理解

4. **配合示波器/逻辑分析仪**：对于 I2C、SPI、UART 等通信协议，使用逻辑分析仪观察实际时序波形，将抽象的协议时序与代码对应起来，效果显著

5. **循序渐进，不要跳步**：严格按照 GPIO → 定时器 → 中断 → 串口 → I2C → SPI → ADC → 综合实例 的顺序学习，每个阶段都建立在前一阶段的基础上

6. **动手修改实验**：在理解示例代码后，尝试修改参数（如波特率、PWM 频率、ADC 通道），观察现象变化，验证自己的理解是否正确

7. **遇到问题先查手册**：养成查阅《参考手册》（RM0008）和 HAL 库用户手册（UM1850）的习惯，这是最权威的资料

## 注意事项

### 1. 必须先使能时钟

STM32 的所有外设（GPIO、USART、TIM、I2C、SPI、ADC 等）默认时钟是关闭的，使用前必须先使能对应时钟，否则配置寄存器无效。

```c
// GPIOA 时钟使能（必须在使用 GPIOA 之前调用）
__HAL_RCC_GPIOA_CLK_ENABLE();

// USART1 时钟使能
__HAL_RCC_USART1_CLK_ENABLE();

// TIM3 时钟使能
__HAL_RCC_TIM3_CLK_ENABLE();
```

### 2. HAL_Delay 不能在中断中使用

`HAL_Delay` 函数依赖 SysTick 中断更新计数器，在中断服务函数中调用 `HAL_Delay` 会导致死锁（等待 SysTick 中断更新计数，但 SysTick 优先级可能低于当前中断，永远无法执行）。

```c
// 错误示范：在中断回调中调用 HAL_Delay
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 危险！如果当前中断优先级高于 SysTick，会死锁
    HAL_Delay(20);  // 不要这样做
}

// 正确做法：使用 HAL_GetTick() 做时间差消抖
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t last_tick = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_tick < 50) return;  // 50ms 消抖
    last_tick = now;
    // 业务逻辑
}
```

### 3. volatile 关键字的重要性

当中断服务函数和主循环共享同一个变量时，该变量必须用 `volatile` 修饰，否则编译器优化可能导致主循环读不到中断中的修改值。

```c
// 正确：volatile 修饰，防止编译器优化
volatile uint8_t rx_flag = 0;
volatile uint8_t rx_data;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    rx_flag = 1;  // 中断中写入
}

// 主循环
while (1)
{
    if (rx_flag)  // 没有 volatile，这里可能永远读不到更新
    {
        // 处理数据
        rx_flag = 0;
    }
}
```

### 4. AFIO 时钟在复用功能时必须使能

当使用 GPIO 的复用功能（如 USART、I2C、SPI、TIM 的 PWM 输出、EXTI 等）时，必须先使能 AFIO 时钟，否则复用功能配置寄存器无法写入。

```c
// 使用任何复用功能前都应使能 AFIO 时钟
__HAL_RCC_AFIO_CLK_ENABLE();

// 重映射引脚时尤其需要（如将 USART1 从 PA9/PA10 重映射到 PB6/PB7）
__HAL_RCC_AFIO_CLK_ENABLE();
__HAL_AFIO_REMAP_USART1(ENABLE);
```

### 5. Flash 等待周期

STM32F103 的 Flash 读取速度跟不上 CPU 高频运行，需要在 `FLASH_ACR` 中设置等待周期：

| SYSCLK 频率 | 推荐等待周期 |
|-------------|-------------|
| 0 - 24MHz | 0 个等待周期 |
| 24 - 48MHz | 1 个等待周期 |
| 48 - 72MHz | 2 个等待周期 |

当主频超过 48MHz 时必须配置 2 个等待周期，否则读取 Flash 会出错，导致程序跑飞。CubeMX 配置时钟时会自动处理，但手写代码时需要注意。

### 6. printf 重定向需要勾选 Use MicroLIB

在 Keil 中使用 `printf` 重定向到串口时，需要做两件事：

```c
// 1. 重写 fputc 函数
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
```

```
2. 在 Keil 工程选项中勾选 Use MicroLIB
   Project → Options for Target → Target → Code Generation → 勾选 Use MicroLIB
```

不勾选 MicroLIB 时，标准库的 `printf` 会引入半主机模式（Semi-Hosting），导致程序卡死在 `BKPT` 指令处。MicroLIB 是精简版 C 库，避免了这个问题。

## HAL库函数速查

下表列出本项目涉及的常用 HAL 库函数，便于快速查阅：

| 函数名 | 所属外设 | 功能说明 |
|--------|----------|----------|
| `HAL_Init` | HAL 核心 | 初始化 HAL 库、配置 SysTick、初始化 NVIC 分组 |
| `HAL_Delay` | HAL 核心 | 阻塞延时，单位毫秒（不可在中断中调用） |
| `HAL_GetTick` | HAL 核心 | 获取当前系统滴答数（毫秒级，可用于时间差消抖） |
| `HAL_GPIO_WritePin` | GPIO | 设置引脚电平（GPIO_PIN_SET / GPIO_PIN_RESET） |
| `HAL_GPIO_TogglePin` | GPIO | 翻转引脚电平 |
| `HAL_GPIO_ReadPin` | GPIO | 读取引脚输入电平 |
| `HAL_GPIO_EXTI_Callback` | GPIO/EXTI | 外部中断回调函数（弱定义，需用户重写） |
| `HAL_TIM_Base_Start_IT` | TIM | 启动基本定时器中断 |
| `HAL_TIM_PWM_Start` | TIM | 启动 PWM 输出 |
| `HAL_TIM_PeriodElapsedCallback` | TIM | 定时器周期溢出回调函数（弱定义） |
| `HAL_UART_Transmit` | UART | 阻塞方式发送数据 |
| `HAL_UART_Receive_IT` | UART | 中断方式接收数据（非阻塞） |
| `HAL_UART_RxCpltCallback` | UART | 串口接收完成回调函数（弱定义） |
| `HAL_I2C_Mem_Write` | I2C | 向 I2C 设备的指定寄存器地址写入数据 |
| `HAL_I2C_Mem_Read` | I2C | 从 I2C 设备的指定寄存器地址读取数据 |
| `HAL_I2C_Master_Transmit` | I2C | I2C 主机发送（不带寄存器地址） |
| `HAL_I2C_Master_Receive` | I2C | I2C 主机接收（不带寄存器地址） |
| `HAL_SPI_TransmitReceive` | SPI | SPI 全双工收发数据 |
| `HAL_SPI_Transmit` | SPI | SPI 仅发送 |
| `HAL_SPI_Receive` | SPI | SPI 仅接收 |
| `HAL_ADC_Start` | ADC | 启动 ADC 转换 |
| `HAL_ADC_PollForConversion` | ADC | 轮询等待转换完成 |
| `HAL_ADC_GetValue` | ADC | 读取 ADC 转换结果 |
| `HAL_ADCEx_Calibration_Start` | ADC | 启动 ADC 校准 |
| `__HAL_RCC_GPIOx_CLK_ENABLE` | RCC | 使能 GPIOx 时钟（x = A/B/C/D） |
| `__HAL_RCC_AFIO_CLK_ENABLE` | RCC | 使能 AFIO 时钟（复用功能必需） |
| `__HAL_RCC_USARTx_CLK_ENABLE` | RCC | 使能 USARTx 时钟 |
| `__HAL_RCC_TIMx_CLK_ENABLE` | RCC | 使能 TIMx 时钟 |
| `__HAL_RCC_I2Cx_CLK_ENABLE` | RCC | 使能 I2Cx 时钟 |
| `__HAL_RCC_SPIx_CLK_ENABLE` | RCC | 使能 SPIx 时钟 |
| `__HAL_RCC_ADCx_CLK_ENABLE` | RCC | 使能 ADCx 时钟 |

## 学习资源推荐

### 推荐书籍

| 书名 | 作者/出版社 | 说明 |
|------|-------------|------|
| 《Cortex-M3权威指南》 | Joseph Yiu | 深入讲解 Cortex-M3 内核架构，进阶必读 |
| 《STM32库开发实战指南》 | 刘火良、杨森（野火） | 基于 HAL 库，配有丰富实例 |
| 《STM32F103中文参考手册》 | ST 官方 | RM0008 中文版，寄存器级别权威资料 |
| 《STM32 HAL库开发实战指南》 | 野火 | 专注 HAL 库开发，适合入门 |
| 《STM32 自学笔记》 | 张洋 | 入门级，讲解通俗易懂 |

### 在线教程

- **正点原子 STM32 教程**：http://www.openedv.com/ — 配套视频与文档，资料丰富
- **野火 STM32 教程**：https://doc.embedfire.com/ — 在线文档详尽，配有硬件平台
- **ST 官方中文社区**：https://shequ.stmicroelectronics.cn/ — 官方技术支持与中文资料
- **STM32CubeMX 教程**：ST 官网视频教程，学习图形化配置
- **安富莱 STM32 教程**：https://www.armbbs.cn/ — 经典教程，适合系统学习

### 官方手册

- **STM32F103 参考手册（RM0008）**：https://www.st.com/resource/en/reference_manual/cd00171190.pdf
- **STM32F103 数据手册**：https://www.st.com/resource/en/datasheet/stm32f103c8.pdf
- **STM32 HAL 库用户手册（UM1850）**：https://www.st.com/resource/en/user_manual/dm00154093.pdf
- **Cortex-M3 技术参考手册**：ARM 官方文档

### 在线社区

- **STMicro 官方社区**：https://community.st.com/
- **阿莫电子论坛**：https://www.amobbs.com/
- **野火电子论坛**：https://www.firebbs.cn/
- **正点原子论坛**：http://www.openedv.com/forum/

---

祝学习愉快，从点灯到点亮整个嵌入式世界！
