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

typedef enum
{
    USART_IDLE,
    USART_SENDING_DATA
} USART_Rx_status_t;

typedef struct 
{
    baud_t baud_rate;
    parity_t parity;
    init_status_t init_status;
    interrupt_state_t Rx_IRQ;
    interrupt_state_t Tx_IRQ;
}driver_init_status_t ;

extern driver_init_status_t mock_slave_USART;
extern driver_subscr_cb_t mock_msg_tx_done_cb;
extern driver_subscr_cb_t mock_1_5_char_break_cb;
extern driver_subscr_cb_t mock_3_5_char_break_cb;
extern driver_subscr_cb_t mock_frame_error_cb;
extern USART_Rx_status_t USART_Tx_status;

void mock_USART_RX_IRQ(void);
void mock_1_5_char_timer_IRQ (void);
void mock_3_5_char_timer_IRQ (void);
void mock_USART_Tx_Done_IRQ(void);



