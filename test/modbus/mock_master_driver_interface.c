/**
 * @file mock_master_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-05-31
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "mock_master_driver_interface.h"
#include "memory.h"

driver_subscr_cb_t mock_master_msg_tx_done_cb = NULL;
driver_subscr_cb_t mock_master_1_5_char_break_cb = NULL;
driver_subscr_cb_t mock_master_3_5_char_break_cb = NULL;
driver_subscr_cb_t mock_master_frame_error_cb = NULL;

modbus_buf_t mock_master_tx_buf[MODBUS_RTU_BUFFER_SIZE];
modbus_buf_t *mock_master_tx_buf_ptr = mock_master_tx_buf;
modbus_req_resp_t *mock_master_rx_msg_ptr = NULL;

driver_init_status_t mock_master_USART = {0, NONE, INIT_UNKNOWN, IRQ_DISABLED, IRQ_DISABLED};
USART_Tx_status_t master_USART_Tx_status = USART_IDLE;

static void master_usart_init(baud_t baud, parity_t parity);
static void master_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void master_disable_usart_rx_interrupt(void);
static void master_uasrt_subscribe_t_1_5_char_expired_cb(driver_subscr_cb_t callback);
static void master_uasrt_subscribe_msg_tx_done_callback(driver_subscr_cb_t callback);
static void master_uasrt_subscribe_t_3_5_char_expired_cb(driver_subscr_cb_t callback);
static void master_uasrt_subscribe_modbus_frame_error_cb(driver_subscr_cb_t callback);

static const modbus_RTU_driver_struct_t master_RTU_driver_interface = {
    master_usart_init,
    master_usart_send,
    master_enable_usart_rx_interrupt,
    master_disable_usart_rx_interrupt,
    master_uasrt_subscribe_t_1_5_char_expired_cb,
    master_uasrt_subscribe_msg_tx_done_callback,
    master_uasrt_subscribe_t_3_5_char_expired_cb,
    master_uasrt_subscribe_modbus_frame_error_cb,
};

const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void)
{
    return &master_RTU_driver_interface;
}

static void master_usart_init(baud_t baud, parity_t parity)
{
    mock_master_USART.baud_rate = baud;
    mock_master_USART.parity = parity;
    mock_master_USART.init_status = DRIVER_INITIALIZED;
}
static void master_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    (void)(msg_len);
    mock_master_tx_buf_ptr = tx_msg;
    master_USART_Tx_status = USART_SENDING_DATA;
    mock_master_USART.Tx_IRQ = IRQ_ENABLED;
}
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    mock_master_rx_msg_ptr = recv_buf;
    mock_master_USART.Rx_IRQ = IRQ_ENABLED;
}
static void master_disable_usart_rx_interrupt(void)
{
    mock_master_USART.Rx_IRQ = IRQ_DISABLED;
}
static void master_uasrt_subscribe_t_1_5_char_expired_cb(driver_subscr_cb_t callback)
{
    mock_master_1_5_char_break_cb = callback;
}
static void master_uasrt_subscribe_msg_tx_done_callback(driver_subscr_cb_t callback)
{
    mock_master_msg_tx_done_cb = callback;
}

static void master_uasrt_subscribe_t_3_5_char_expired_cb(driver_subscr_cb_t callback)
{
    mock_master_3_5_char_break_cb = callback;
}

static void master_uasrt_subscribe_modbus_frame_error_cb(driver_subscr_cb_t callback)
{
    mock_master_frame_error_cb = callback;
}

// mock functionality neede only for tests
void mock_USART_req_msg_sended_EVENT(void)
{
    // operations deone when USART dirver send last data from request msg
    mock_master_USART.Tx_IRQ = IRQ_DISABLED;
    mock_master_msg_tx_done_cb();
}

void mock_USART_T_1_5_timeout_EVENT(void)
{
    mock_master_1_5_char_break_cb();
}

void mock_USART_T_3_5_timeout_EVENT(void)
{
    mock_master_3_5_char_break_cb();
}

void mock_USART_frame_error_EVENT(void)
{
    mock_master_frame_error_cb();
}