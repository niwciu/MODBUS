/**
 * @file mock_slave_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-05-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "mock_slave_driver_interface.h"

#include "mem.h"

driver_subscr_cb_t mock_msg_tx_done_cb = NULL;
driver_subscr_cb_t mock_1_5_char_break_cb = NULL;
driver_subscr_cb_t mock_3_5_char_break_cb = NULL;
driver_subscr_cb_t mock_frame_error_cb = NULL;

driver_init_status_t mock_slave_USART = {0, NONE, INIT_UNKNOWN, IRQ_DISABLED, IRQ_DISABLED};
timer_state_t mock_1_5_char_timer = TIMER_INACTIVE;
timer_state_t mock_3_5_char_timer = TIMER_INACTIVE;

modbus_req_resp_t *rx_msg = NULL;

USART_Tx_status_t slave_USART_Tx_status = USART_IDLE;
modbus_buf_t mock_slave_tx_buffer[MODBUS_RTU_BUFFER_SIZE];
modbus_buf_t *mock_slave_tx_buffer_ptr;

static void slave_usart_init(baud_t baud, parity_t parity);
static void slave_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static const modbus_RTU_driver_struct_t slave_RTU_driver_interface = {
    slave_usart_init,
    slave_usart_send,
    slave_enable_usart_rx_interrupt,
    NULL,
    slave_t_1_5_char_expired_callback_subscribe,
    slave_msg_tx_done_callback_subscribe,
    slave_t_3_5_char_expired_callback_subscribe,
    slave_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void)
{
    return &slave_RTU_driver_interface;
}

static void slave_usart_init(baud_t baud, parity_t parity)
{
    mock_slave_USART.baud_rate = baud;
    mock_slave_USART.parity = parity;
    mock_slave_USART.init_status = DRIVER_INITIALIZED;
}
static void slave_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    for (uint8_t i = 0; i < msg_len; i++)
    {
        mock_slave_tx_buffer[i] = tx_msg[i];
    }
    mock_slave_tx_buffer_ptr = tx_msg;
    slave_USART_Tx_status = USART_SENDING_DATA;
}
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_msg = recv_buf;
    mock_slave_USART.Rx_IRQ = IRQ_ENABLED;
}

static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    mock_1_5_char_break_cb = callback;
}

static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback)
{
    mock_msg_tx_done_cb = callback;
}

static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    mock_3_5_char_break_cb = callback;
}
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback)
{
    mock_frame_error_cb = callback;
}

void mock_USART_RX_IRQ(void)
{
    if ((TIMER_FIRED == mock_1_5_char_timer) && (TIMER_FIRED == mock_3_5_char_timer))
    {
        // czyli odebrano msg i teraz przychodzi coś nowego
        // czy tutaj mam już bufor? gzie zarządzam pobieraniem nowego bufora?
    }
    if ((TIMER_FIRED == mock_1_5_char_timer) && (TIMER_COUNTING == mock_3_5_char_timer))
    {
        mock_frame_error_cb();
    }
    else
    {
        mock_1_5_char_timer = TIMER_COUNTING;
        mock_3_5_char_timer = TIMER_COUNTING;
    }
}
void mock_1_5_char_timer_IRQ(void)
{
    mock_1_5_char_timer = TIMER_FIRED;
    mock_1_5_char_break_cb();
}
void mock_3_5_char_timer_IRQ(void)
{
    mock_3_5_char_timer = TIMER_FIRED;
    mock_3_5_char_break_cb();
}
void mock_USART_Tx_Done_IRQ(void)
{
    mock_msg_tx_done_cb();
}