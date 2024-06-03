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

#include "mem.h"



typedef struct
{
    modbus_buf_t *cur_byte_ptr;
    modbus_buf_t *last_byte_ptr;
} tx_buf_t;
// tx_buf_t tx_buf;

// modbus_buf_t *rx_data = NULL;
// rx_cb_t rx_callback = NULL;

modbus_buf_t slave_rx_buf[MODBUS_RTU_BUFFER_SIZE];

driver_init_status_t mock_USART = {0,NONE,INIT_UNKNOWN};

static void master_usart_init(baud_t baud, parity_t parity);
static void master_usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void master_disable_usart_rx_interrupt(void);
static void master_enable_silence_timer(void);
static void master_uasrt_subscribe_msg_rx_done_callback(driver_subscr_cb_t callback);
static void master_uasrt_subscribe_msg_tx_done_callback(driver_subscr_cb_t callback);



static const modbus_RTU_driver_struct_t master_RTU_driver_interface = {
    master_usart_init,
    master_usart_send,
    master_enable_usart_rx_interrupt,
    master_disable_usart_rx_interrupt,
    master_enable_silence_timer,
    master_uasrt_subscribe_msg_rx_done_callback,
    master_uasrt_subscribe_msg_tx_done_callback,
    NULL,
    NULL,
};

const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void)
{
    return &master_RTU_driver_interface;
}

static void master_usart_init(baud_t baud, parity_t parity)
{
    mock_USART.baud_rate=baud;
    mock_USART.parity=parity;
    mock_USART.init_status=DRIVER_INITIALIZED;
}
static void master_usart_send(modbus_buf_t *tx_msg, uint8_t msg_len)
{
    memcpy(slave_rx_buf,tx_msg,msg_len);
    //ToDo możena wykorzystać zrobionego slave do odbierania danych i gnerowania ramki zwrotnej
}
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{

}
static void master_disable_usart_rx_interrupt(void)
{

}
static void master_enable_silence_timer(void)
{
    
}
static void master_uasrt_subscribe_msg_rx_done_callback(driver_subscr_cb_t callback)
{

}
static void master_uasrt_subscribe_msg_tx_done_callback(driver_subscr_cb_t callback)
{

}