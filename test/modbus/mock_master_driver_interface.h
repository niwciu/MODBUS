/**
 * @file mock_modbus_driver_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_driver_interface.h"
#include "mock_master_slave_common.h"

#pragma once

typedef enum
{
    INIT_UNKNOWN,
    DRIVER_INITIALIZED,
} init_status_t;

typedef struct
{
    baud_t baud_rate;
    parity_t parity;
    init_status_t init_status;
} driver_init_status_t;

extern driver_init_status_t mock_master_USART;

extern driver_subscr_cb_t mock_master_msg_tx_done_cb;
// extern driver_subscr_cb_t mock_1_5_char_break_cb;
// extern driver_subscr_cb_t mock_3_5_char_break_cb;
// extern driver_subscr_cb_t mock_frame_error_cb;
extern modbus_buf_t *mock_master_tx_buf_ptr;
extern USART_Tx_status_t master_USART_Tx_status;
