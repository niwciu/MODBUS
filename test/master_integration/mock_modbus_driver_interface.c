/**
 * @file mock_modbus_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "modbus_driver_interface.h"
#include "mock_modbus_driver_interface.h"




typedef struct
{
    modbus_buf_t *cur_byte_ptr;
    modbus_buf_t *last_byte_ptr;
} tx_buf_t;
// tx_buf_t tx_buf;

// modbus_buf_t *rx_data = NULL;
// rx_cb_t rx_callback = NULL;


driver_init_status_t mock_USART = {0,NONE,INIT_UNKNOWN};
const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void);

static void usart_init(baud_t baud, parity_t parity);
static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void enable_usart_rx_interrupt(modbus_buf_t *recv_buf);
static void disable_usart_rx_interrupt(void);
static void uasrt_subscribe_rx_callback(rx_cb_t callback);



static const struct modbus_RTU_driver_struct RTU_driver_interface = {
    usart_init,
    usart_send,
    enable_usart_rx_interrupt,
    disable_usart_rx_interrupt,
    uasrt_subscribe_rx_callback,
};

const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void)
{
    return &RTU_driver_interface;
}

static void usart_init(baud_t baud, parity_t parity)
{
    mock_USART.baud_rate=baud;
    mock_USART.parity=parity;
    mock_USART.init_status=DRIVER_INITIALIZED;
}
static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len)
{

}
static void enable_usart_rx_interrupt(modbus_buf_t *recv_buf)
{

}
static void disable_usart_rx_interrupt(void)
{

}
static void uasrt_subscribe_rx_callback(rx_cb_t callback)
{

}