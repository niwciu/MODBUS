/**
 * @file modbus_driver_config.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Header file with defines that speed up modbus driver configuration specially when porting driver to different USARTs and Timers

 * @version 0.0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "stm32g070xx.h"

#define RX_DATA_BUF_SIZE 256
#define LOW 0
#define HIGH 1

/** modbus USART configuration*/
#define MODBUS_USART (USART2)
#define MODBUS_USART_CLK APBENR1
#define MODBUS_USART_CLK_EN (RCC_APBENR1_USART2EN)
#define MODBUS_USART_CLK_FREQ 64000000UL
#define MODBUS_USART_IRQN USART2_IRQn
#define MODBUS_USART_IRQHandler USART2_IRQHandler
#define MODBUS_USART_IRQ_PRIORITY 10

#define MODBUS_USART_PORT (GPIOA)

#define MODBUS_USART_RX_MODE (GPIO_MODER_MODE3_1)
#define MODBUS_USART_TX_MODE (GPIO_MODER_MODE2_1)
#define MODBUS_USART_DE_MODE (GPIO_MODER_MODE1_1)

#define MODBUS_USART_RX_MODE_Msk (GPIO_MODER_MODE3)
#define MODBUS_USART_TX_MODE_Msk (GPIO_MODER_MODE2)
#define MODBUS_USART_DE_MODE_Msk (GPIO_MODER_MODE1)

#define MODBUS_USART_RX_AF_Msk (GPIO_AFRL_AFSEL3)
#define MODBUS_USART_TX_AF_Msk (GPIO_AFRL_AFSEL2)
#define MODBUS_USART_DE_AF_Msk (GPIO_AFRL_AFSEL1)

#define MODBUS_USART_RX_AF (GPIO_AFRL_AFSEL3_0)
#define MODBUS_USART_TX_AF (GPIO_AFRL_AFSEL2_0)
#define MODBUS_USART_DE_AF (GPIO_AFRL_AFSEL1_0)

#define MODBUS_USART_AF_REG (LOW)

#define MODBUS_USART_GPIO_CLK_EN (RCC_IOPENR_GPIOAEN)

/** modbus Timer configuration*/
#define MODBUS_TIMER (TIM7)
#define MODBUS_TIMER_CLK_EN (RCC_APBENR1_TIM7EN)
#define MODBUS_TIMER_CLK_FREQ 64000000UL
#define MODBUS_TIMER_IRQN TIM7_IRQn
#define MODBUS_TIMER_IRQHandler TIM7_LPTIM2_IRQHandler
#define MODBUS_TIMER_IRQ_PRIORITY 10