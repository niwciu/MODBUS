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

#include "modbus_driver_interface.h"
#include "mock_slave_driver_interface.h"

#include "mem.h"



typedef struct
{
    modbus_buf_t *cur_byte_ptr;
    modbus_buf_t *last_byte_ptr;
} tx_buf_t;
// tx_buf_t tx_buf;

// modbus_buf_t *rx_data = NULL;
// rx_cb_t rx_callback = NULL;

// modbus_buf_t slave_rx_buf[MODBUS_RTU_BUFFER_SIZE];

driver_init_status_t mock_slave_USART = {0,NONE,INIT_UNKNOWN,IRQ_DISABLED,IRQ_DISABLED};
timer_state_t mock_1_5char_timer;
timer_state_t mock_2char_timer;
modbus_msg_t *rx_buf=NULL;

static void slave_usart_init(baud_t baud, parity_t parity);
static void slave_usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void slave_enable_usart_rx_interrupt(modbus_buf_t *recv_buf);
// static void disable_usart_rx_interrupt(void);
static void slave_uasrt_subscribe_rx_callback(rx_cb_t callback);



static const modbus_RTU_driver_struct_t slave_RTU_driver_interface = {
    slave_usart_init,
    slave_usart_send,
    slave_enable_usart_rx_interrupt,
    NULL,
    slave_uasrt_subscribe_rx_callback,
};

const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void)
{
    return &slave_RTU_driver_interface;
}

static void slave_usart_init(baud_t baud, parity_t parity)
{
    mock_slave_USART.baud_rate=baud;
    mock_slave_USART.parity=parity;
    mock_slave_USART.init_status=DRIVER_INITIALIZED;
}
static void slave_usart_send(modbus_buf_t *tx_msg, uint8_t msg_len)
{
    // memcpy(slave_rx_buf,tx_msg,msg_len);
    //ToDo możena wykorzystać zrobionego slave do odbierania danych i gnerowania ramki zwrotnej
}
static void slave_enable_usart_rx_interrupt(modbus_buf_t *recv_buf)
{
    rx_buf=recv_buf;
    mock_slave_USART.Rx_IRQ=IRQ_ENABLED;
}
// static void disable_usart_rx_interrupt(void)
// {

// }
static void slave_uasrt_subscribe_rx_callback(rx_cb_t callback)
{

}