/**
 * @file mock_master_slave_common.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @version 0.0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

typedef enum
{
    USART_IDLE,
    USART_SENDING_DATA
} USART_Tx_status_t;

typedef enum
{
    INIT_UNKNOWN,
    DRIVER_INITIALIZED,
} init_status_t;

typedef enum
{
    IRQ_DISABLED,
    IRQ_ENABLED,
} interrupt_state_t;

typedef struct
{
    baud_t baud_rate;
    parity_t parity;
    init_status_t init_status;
    interrupt_state_t Rx_IRQ;
    interrupt_state_t Tx_IRQ;
} driver_init_status_t;