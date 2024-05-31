/**
 * @file mock_modbus_driver_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "modbus_type.h"

# pragma once

typedef enum{
    INIT_UNKNOWN,
    DRIVER_INITIALIZED,
}init_status_t;

typedef struct 
{
    baud_t baud_rate;
    parity_t parity;
    init_status_t init_status;
}driver_init_status_t ;

extern driver_init_status_t mock_USART;


