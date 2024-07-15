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

#include "modbus_driver_interface.h"
#include "mock_master_slave_common.h"

typedef enum
{
    TIMER_INACTIVE,
    TIMER_COUNTING,
    TIMER_FIRED,

} timer_state_t;

extern driver_init_status_t mock_slave_USART;
extern driver_subscr_cb_t mock_msg_tx_done_cb;
extern driver_subscr_cb_t mock_1_5_char_break_cb;
extern driver_subscr_cb_t mock_3_5_char_break_cb;
extern driver_subscr_cb_t mock_frame_error_cb;
extern USART_Tx_status_t slave_USART_Tx_status;

void mock_USART_RX_IRQ(void);
void mock_1_5_char_timer_IRQ(void);
void mock_3_5_char_timer_IRQ(void);
void mock_USART_Tx_Done_IRQ(void);
