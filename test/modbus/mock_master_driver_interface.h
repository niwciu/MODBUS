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
#define MASTER_COILS_TABLE_SIZE 10

extern modbus_coil_disin_t mock_master_coils[MASTER_COILS_TABLE_SIZE];

extern driver_init_status_t mock_master_USART;

extern driver_subscr_cb_t mock_master_msg_tx_done_cb;
extern driver_subscr_cb_t mock_master_1_5_char_break_cb;
extern driver_subscr_cb_t mock_master_3_5_char_break_cb;
extern driver_subscr_cb_t mock_master_frame_error_cb;
extern modbus_buf_t *mock_master_tx_buf_ptr;
extern modbus_req_resp_t *mock_master_rx_msg_ptr;
extern USART_Tx_status_t master_USART_Tx_status;

void mock_USART_req_msg_sended_EVENT(void);
void mock_USART_T_1_5_timeout_EVENT(void);
void mock_USART_T_3_5_timeout_EVENT(void);
void mock_USART_frame_error_EVENT(void);
