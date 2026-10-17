/**
 ******************************************************************************
 * @file    stm32f1xx_hal_conf.h
 * @brief   STM32F1xx HAL库配置文件
 *          本文件用于配置HAL库的各个模块是否启用，以及一些关键参数。
 *          适用于 STM32F103C8T6 最小系统板（蓝桥杯常用）
 ******************************************************************************
 * @attention
 *
 * 本文件是STM32 HAL库的核心配置头文件，通过宏定义来启用或禁用各个外设模块。
 * 学习时请仔细阅读每个宏的含义，理解为什么需要这样配置。
 *
 ******************************************************************************
 */

/* 防止头文件被重复包含的保护机制 */
#ifndef __STM32F1xx_HAL_CONF_H
#define __STM32F1xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* ================================================================
 * 第一部分：HAL库模块使能配置
 * ================================================================
 * 下面的宏定义决定了哪些外设模块的HAL库驱动会被编译进工程。
 * 1 表示启用该模块，0 表示禁用。
 * 启用的模块越多，编译后的代码体积越大。
 * ================================================================ */

/* ---------- 核心模块（必须启用） ---------- */
#define HAL_MODULE_ENABLED              /* 启用HAL库核心模块 */
#define HAL_CORTEX_MODULE_ENABLED       /* 启用Cortex-M3内核相关函数（NVIC、SysTick等） */
#define HAL_RCC_MODULE_ENABLED          /* 启用复位与时钟控制模块（配置系统时钟必须） */
#define HAL_FLASH_MODULE_ENABLED        /* 启用Flash操作模块（设置Flash等待周期等） */
#define HAL_GPIO_MODULE_ENABLED         /* 启用GPIO模块（最基础的IO口操作） */
#define HAL_PWR_MODULE_ENABLED          /* 启用电源管理模块 */

/* ---------- 常用外设模块（按需启用） ---------- */
#define HAL_TIM_MODULE_ENABLED          /* 启用定时器模块（TIM1-TIM4等） */
#define HAL_UART_MODULE_ENABLED         /* 启用串口模块（USART1/2/3、UART4/5） */
#define HAL_ADC_MODULE_ENABLED          /* 启用模数转换模块（ADC1/ADC2） */
#define HAL_I2C_MODULE_ENABLED          /* 启用I2C通信模块（I2C1/I2C2） */
#define HAL_SPI_MODULE_ENABLED          /* 启用SPI通信模块（SPI1/SPI2） */
#define HAL_DMA_MODULE_ENABLED          /* 启用DMA控制器模块 */
#define HAL_EXTI_MODULE_ENABLED         /* 启用外部中断模块 */

/* ---------- 不常用外设模块（默认禁用，需要时再开启） ---------- */
#undef  HAL_CAN_MODULE_ENABLED          /* 禁用CAN通信模块 */
#undef  HAL_USB_MODULE_ENABLED          /* 禁用USB模块 */
#undef  HAL_SD_MODULE_ENABLED           /* 禁用SD卡模块 */
#undef  HAL_DAC_MODULE_ENABLED          /* 禁用数模转换模块（F103C8T6没有DAC） */
#undef  HAL_RTC_MODULE_ENABLED          /* 禁用实时时钟模块 */
#undef  HAL_WWDG_MODULE_ENABLED         /* 禁用窗口看门狗 */
#undef  HAL_IWDG_MODULE_ENABLED         /* 禁用独立看门狗 */

/* ================================================================
 * 第二部分：HAL库系统时基配置
 * ================================================================
 * SysTick是Cortex-M内核自带的系统滴答定时器，HAL库默认用它作为时基。
 * HAL_TICK_FREQ_1KHZ 表示每1ms产生一次中断，即HAL_Delay()的基本单位是1ms。
 * ================================================================ */

/* 选择SysTick作为HAL时基来源（这是最常用的配置） */
#define  USE_HAL_TIM_REGISTER_CALLBACKS  0U  /* 不使用定时器回调注册机制 */
#define  USE_HAL_UART_REGISTER_CALLBACKS 0U  /* 不使用串口回调注册机制 */
#define  USE_HAL_ADC_REGISTER_CALLBACKS  0U  /* 不使用ADC回调注册机制 */
#define  USE_HAL_I2C_REGISTER_CALLBACKS  0U  /* 不使用I2C回调注册机制 */
#define  USE_HAL_SPI_REGISTER_CALLBACKS  0U  /* 不使用SPI回调注册机制 */

/* ================================================================
 * 第三部分：HSE（高速外部时钟）值配置
 * ================================================================
 * STM32F103C8T6最小系统板通常使用8MHz的外部晶振（HSE）。
 * 如果你使用的开发板晶振频率不同，需要修改这个值。
 * 系统时钟通过PLL将HSE倍频到72MHz。
 * ================================================================ */

#if !defined  (HSE_VALUE)
  /* 外部高速晶振频率，单位：Hz。最小系统板常用8MHz。 */
  #define HSE_VALUE    8000000U
#endif /* HSE_VALUE */

/* HSE启动超时时间，单位：ms。如果晶振损坏或不起振，超时后会报错。 */
#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    100U
#endif /* HSE_STARTUP_TIMEOUT */

/* ================================================================
 * 第四部分：HSI（高速内部时钟）值配置
 * ================================================================
 * STM32内部自带8MHz的RC振荡器（HSI），精度不如外部晶振，
 * 但在没有外部晶振的情况下也能工作。一般不建议用HSI做串口通信，
 * 因为频率误差可能导致波特率不准。
 * ================================================================ */

#if !defined  (HSI_VALUE)
  /* 内部高速RC振荡器频率，固定为8MHz（出厂校准值） */
  #define HSI_VALUE    8000000U
#endif /* HSI_VALUE */

/* ================================================================
 * 第五部分：包含各个模块的头文件
 * ================================================================
 * 根据上面的使能配置，自动包含对应的HAL库头文件。
 * 这样做的好处是：禁用的模块不会被编译，减小代码体积。
 * ================================================================ */

#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32f1xx_hal_cortex.h"   /* Cortex-M3内核函数（NVIC、SysTick等） */
#endif /* HAL_CORTEX_MODULE_ENABLED */

#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32f1xx_hal_rcc.h"      /* 复位与时钟控制 */
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32f1xx_hal_gpio.h"     /* GPIO输入输出 */
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32f1xx_hal_dma.h"      /* DMA控制器 */
#endif /* HAL_DMA_MODULE_ENABLED */

#ifdef HAL_EXTI_MODULE_ENABLED
  #include "stm32f1xx_hal_exti.h"     /* 外部中断 */
#endif /* HAL_EXTI_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32f1xx_hal_flash.h"    /* Flash操作 */
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_PWR_MODULE_ENABLED
  #include "stm32f1xx_hal_pwr.h"      /* 电源管理 */
#endif /* HAL_PWR_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
  #include "stm32f1xx_hal_tim.h"      /* 定时器 */
#endif /* HAL_TIM_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
  #include "stm32f1xx_hal_uart.h"     /* 串口通信 */
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
  #include "stm32f1xx_hal_adc.h"      /* 模数转换 */
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
  #include "stm32f1xx_hal_i2c.h"      /* I2C通信 */
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
  #include "stm32f1xx_hal_spi.h"      /* SPI通信 */
#endif /* HAL_SPI_MODULE_ENABLED */

/* ================================================================
 * 第六部分：断言（Assert）配置
 * ================================================================
 * assert_param用于函数参数检查，调试时非常有用，可以快速发现传入的非法参数。
 * 正式发布版本可以注释掉USE_FULL_ASSERT以减小代码体积。
 * ================================================================ */

/* 取消下面一行的注释，开启参数断言检查（调试阶段建议开启） */
/* #define USE_FULL_ASSERT    1 */

#ifdef  USE_FULL_ASSERT
  /**
   * @brief  assert_param宏定义
   * @param  expr: 表达式，如果为假（0），则触发断言
   * @note   断言失败时会打印出错的文件名和行号，方便调试定位问题。
   *         实际项目中可以在这里添加LED闪烁、串口打印等错误指示。
   */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);  /* 断言失败处理函数声明 */
#else
  #define assert_param(expr) ((void)0U)  /* 不启用断言时，assert_param为空操作 */
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
