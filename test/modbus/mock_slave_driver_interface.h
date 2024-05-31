/**
 * @file mock_modbus_driver_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "modbus_type.h"

typedef enum{
    INIT_UNKNOWN,
    DRIVER_INITIALIZED,
}init_status_t;

typedef enum 
{
    TIMER_INACTIVE,
    TIMER_COUNTING,
    TIMER_FIRED,

}timer_state_t;

typedef enum 
{
    IRQ_DISABLED,
    IRQ_ENABLED,
}interrupt_state_t;

typedef struct 
{
    baud_t baud_rate;
    parity_t parity;
    init_status_t init_status;
    interrupt_state_t Rx_IRQ;
    interrupt_state_t Tx_IRQ;
}driver_init_status_t ;

extern driver_init_status_t mock_slave_USART;


