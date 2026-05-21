/**
 * @file modbus_driver_config.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus slave driver hardware configuration for ATmega328P.
 *        USART0: PD1=TX, PD0=RX, PD2=DE (manual GPIO)
 *        Timer1: 16-bit, prescaler 8, used for 1.5T and 3.5T detection
 * @version 1.0.0
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
#define SLAVE_USART_DE_HW_CONTROLL OFF

/** USART0 clock frequency — set to match F_CPU */
#define MODBUS_SLAVE_USART_CLK_FREQ 16000000UL

/** RS-485 DE pin: PD2 (manual GPIO direction control) */
#define MODBUS_SLAVE_USART_DE_DDR  DDRD
#define MODBUS_SLAVE_USART_DE_PORT PORTD
#define MODBUS_SLAVE_USART_DE_PIN  PD2

/**
 * Timer1 (16-bit) configuration — prescaler 8 from 16 MHz → 0.5 µs resolution
 * Start mask: set CS11 (prescaler /8)
 * Stop mask:  clear CS12|CS11|CS10
 */
#define MODBUS_SLAVE_TIMER_CLK_FREQ    16000000UL
#define MODBUS_SLAVE_TIMER_PRESCALER   8
#define MODBUS_SLAVE_TIMER_START_MASK  ((1 << CS11))
#define MODBUS_SLAVE_TIMER_STOP_MASK   ((1 << CS12) | (1 << CS11) | (1 << CS10))
