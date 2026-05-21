/**
 * @file modbus_master_driver_config.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Header file with defines that speed up modbus master driver configuration
 *        specially when porting driver to different USARTs and Timers.
 *        ATmega328P — USART0 variant.
 *        USART0: PD1=TX, PD0=RX, PD2=DE (manual GPIO, no HW control in USART)
 *        Timer1: 16-bit, CTC mode, prescaler 8 → 0.5 µs/tick at 16 MHz
 * @version 0.0.1
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <avr/io.h>

#define OFF 0
#define ON  1

/** ATmega328P does not have hardware DE control in USART */
#define MASTER_USART_DE_HW_CONTROLL OFF

/** USART0 clock frequency — set to match F_CPU */
#define MODBUS_MASTER_USART_CLK_FREQ 16000000UL

/** RS-485 DE pin: PD2 (manual GPIO direction control) */
#define MODBUS_MASTER_USART_DE_DDR  DDRD
#define MODBUS_MASTER_USART_DE_PORT PORTD
#define MODBUS_MASTER_USART_DE_PIN  PD2

/**
 * Timer1 (16-bit) configuration — prescaler 8 from 16 MHz → 0.5 µs resolution
 * Start mask: set CS11 (prescaler /8)
 * Stop mask:  clear CS12|CS11|CS10
 */
#define MODBUS_MASTER_TIMER_CLK_FREQ   16000000UL
#define MODBUS_MASTER_TIMER_PRESCALER  8
#define MODBUS_MASTER_TIMER_START_MASK ((1 << CS11))
#define MODBUS_MASTER_TIMER_STOP_MASK  ((1 << CS12) | (1 << CS11) | (1 << CS10))
