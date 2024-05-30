/**
 * @file modbus_driver_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_DRIVER_INTERFACE_H_
#define _MODBUS_DRIVER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"
#include <stdint.h>
    typedef enum
    {
        NONE,
        ODD,
        EVEN
    } parity_t;


    typedef void (*driver_init_t)(uint32_t Baud, parity_t parity);
    typedef void (*driver_send_t)(modbus_buf_t *tx_msg, uint8_t msg_len);
    typedef void (*driver_enable_rx_t)(modbus_buf_t *recv_buf);
    typedef void (*driver_disable_rx_t)(void);

    typedef void (*rx_cb_t)(modbus_buf_t *resp, uint8_t msg_len);
    typedef void (*driver_subscribe_rx_cb_t)(rx_cb_t callback);

    struct modbus_RTU_driver_struct
    {
        driver_init_t init;
        driver_send_t send;
        driver_enable_rx_t enable_rcev;
        driver_disable_rx_t disable_rcev;
        driver_subscribe_rx_cb_t subscribe_rx_cb;
    };

    const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DRIVER_INTERFACE_H_ */