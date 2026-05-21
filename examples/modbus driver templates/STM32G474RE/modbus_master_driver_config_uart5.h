/**
 * @file modbus_master_driver_config_uart5.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Header file with defines that speed up modbus master driver configuration
 *        specially when porting driver to different USARTs and Timers.
 *        STM32G474RE — UART5 variant.
 *        UART5: PC12=TX (AF5), PD2=RX (AF5), PB6=DE (AF5)
 *        TIM7: APB1, IRQ shared with DAC
 *        DMA1_Channel5 via DMAMUX1_Channel4, UART5_TX resource=32
 *
 *        NOTE: UART5 TX (PC12, GPIOC) and RX (PD2, GPIOD) are on DIFFERENT GPIO ports.
 *        The standard driver template uses MODBUS_MASTER_USART_RX_TX_PORT for both TX and
 *        RX AFR/MODER configuration. For UART5 the USART_GPIO_init() function in
 *        modbus_master_driver_interface.c must be extended to configure GPIOD separately
 *        for the RX pin. Additional defines below mark the split-port configuration.
 *        NOTE: Verify DMAMUX request IDs for UART4/UART5 in RM0440 Table 91.
 *        NOTE: Verify DE pin AF assignment in the device datasheet.

 * @version 0.0.1
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include "stm32g474xx.h"

#define OFF 0
#define ON 1
#define LOW 0
#define HIGH 1

#define MASTER_USE_DMA ON
#define MASTER_USART_DE_HW_CONTROLL ON

/** modbus USART configuration*/
#define MODBUS_MASTER_USART (UART5)
#define MODBUS_MASTER_USART_CLK APB1ENR1
#define MODBUS_MASTER_USART_CLK_EN (RCC_APB1ENR1_UART5EN)
#define MODBUS_MASTER_USART_CLK_FREQ 64000000UL
#define MODBUS_MASTER_USART_IRQN UART5_IRQn
#define MODBUS_MASTER_USART_IRQHandler UART5_IRQHandler
#define MODBUS_MASTER_USART_IRQ_PRIORITY 10

#if MASTER_USE_DMA == ON
#define MODBUS_MASTER_DMA DMA1
#define MODBUS_MASTER_DMA_IRQN DMA1_Channel5_IRQn
#define MODBUS_MASTER_DMA_IRQHandler DMA1_Channel5_IRQHandler
#define MODBUS_MASTER_DMA_chanel DMA1_Channel5
#define MODBUS_MASTER_DMA_ISR_TCIF DMA_ISR_TCIF5
#define MODBUS_MASTER_DMA_ISR_TEIF DMA_ISR_TEIF5

#define USART1_TX_RESOURCE 25
#define USART2_TX_RESOURCE 26
#define USART3_TX_RESOURCE 28
#define UART4_TX_RESOURCE  30  // verify in RM0440 Table 91
#define UART5_TX_RESOURCE  32  // verify in RM0440 Table 91
#define MODBUS_MASTER_USART_TX_RESOURCE UART5_TX_RESOURCE

#define MODBUS_MASTER_DMAMUX_Chanel (DMAMUX1_Channel4)

/** DMA clock on AHB1 (G474 differs from G070 AHBENR) */
#define MODBUS_MASTER_DMA_CLK_REG AHB1ENR
#define MODBUS_MASTER_DMA_CLK_EN  RCC_AHB1ENR_DMA1EN
#endif

/** GPIO ports — TX: PC12 (GPIOC), RX: PD2 (GPIOD) — SPLIT PORT, see file header NOTE
 *  MODBUS_MASTER_USART_RX_TX_PORT is set to GPIOC (TX). Driver modification required for RX. */
#define MODBUS_MASTER_USART_RX_TX_PORT (GPIOC)  // TX port; RX (PD2) needs separate handling
#define MODBUS_MASTER_USART_RX_PORT    (GPIOD)  // extra define for driver modification
#define MODBUS_MASTER_USART_DE_PORT    (GPIOB)

/** TX mode only via MODBUS_MASTER_USART_RX_TX_PORT (PC12); configure GPIOD MODE2 for RX separately */
#define MODBUS_MASTER_USART_TX_MODE     (GPIO_MODER_MODE12_1) // af mode — PC12
#define MODBUS_MASTER_USART_TX_MODE_Msk (GPIO_MODER_MODE12)
#define MODBUS_MASTER_USART_RX_MODE     (GPIO_MODER_MODE2_1)  // af mode — PD2 (on GPIOD)
#define MODBUS_MASTER_USART_RX_MODE_Msk (GPIO_MODER_MODE2)
#if MASTER_USART_DE_HW_CONTROLL == ON
#define MODBUS_MASTER_USART_DE_MODE (GPIO_MODER_MODE6_1) // af mode — PB6
#else
#define MODBUS_MASTER_USART_DE_MODE (GPIO_MODER_MODE6_0) // general purpose output mode
#endif
#define MODBUS_MASTER_USART_DE_MODE_Msk (GPIO_MODER_MODE6)

/** AF register masks */
#define MODBUS_MASTER_USART_TX_AF_Msk   (GPIO_AFRH_AFSEL12)  // PC12 in AFRH
#define MODBUS_MASTER_USART_RX_AF_Msk   (GPIO_AFRL_AFSEL2)   // PD2 in AFRL (GPIOD)
#if MASTER_USART_DE_HW_CONTROLL == ON
#define MODBUS_MASTER_USART_DE_AF_Msk   (GPIO_AFRL_AFSEL6)   // PB6 in AFRL
#endif

/** AF5 values */
#define MODBUS_MASTER_USART_TX_AF (GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_2) // AF5 on PC12
#define MODBUS_MASTER_USART_RX_AF (GPIO_AFRL_AFSEL2_0  | GPIO_AFRL_AFSEL2_2)  // AF5 on PD2
#if MASTER_USART_DE_HW_CONTROLL == ON
#define MODBUS_MASTER_USART_DE_AF (GPIO_AFRL_AFSEL6_0  | GPIO_AFRL_AFSEL6_2)  // AF5 on PB6
#endif

/** PC12 in AFRH; RX PD2 in AFRL (different port — handled in driver) */
#define MODBUS_MASTER_USART_RX_TX_AF_REG (HIGH) // for TX (PC12) only
#if MASTER_USART_DE_HW_CONTROLL == ON
#define MODBUS_MASTER_USART_DE_AF_REG (LOW)
#endif

// additionaly defines for manual DE controll
#if MASTER_USART_DE_HW_CONTROLL == OFF
#define MODBUS_MASTER_USART_DE_PUPDR        GPIO_PUPDR_PUPD6_0
#define MODBUS_MASTER_USART_DE_PUPDR_Msk    GPIO_PUPDR_PUPD6_Msk

#define MODBUS_MASTER_USART_DE_RESET_PIN	GPIO_BSRR_BR6
#define MODBUS_MASTER_USART_DE_SET_PIN		GPIO_BSRR_BS6
#endif

/** GPIO clock enable — G474 uses AHB2 for GPIO */
#define MODBUS_MASTER_USART_GPIO_RX_TX_CLK_EN (RCC_AHB2ENR_GPIOCEN) // TX clock
#define MODBUS_MASTER_USART_GPIO_RX_CLK_EN    (RCC_AHB2ENR_GPIODEN)  // RX clock (driver mod needed)
#define MODBUS_MASTER_USART_GPIO_DE_CLK_EN    (RCC_AHB2ENR_GPIOBEN)
#define MODBUS_MASTER_GPIO_CLK_REG AHB2ENR


/** modbus Timer configuration — TIM7 APB1, IRQ shared with DAC */
#define MODBUS_MASTER_TIMER (TIM7)
#define MODBUS_MASTER_TIMER_APBENR (RCC->APB1ENR1)
#define MODBUS_MASTER_TIMER_CLK_EN (RCC_APB1ENR1_TIM7EN)
#define MODBUS_MASTER_TIMER_CLK_FREQ 64000000UL
#define MODBUS_MASTER_TIMER_IRQN TIM7_DAC_IRQn
#define MODBUS_MASTER_TIMER_IRQHandler TIM7_DAC_IRQHandler
#define MODBUS_MASTER_TIMER_IRQ_PRIORITY 10
