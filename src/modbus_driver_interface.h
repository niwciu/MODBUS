/**
 * @file modbus_driver_interface.h
 * @brief Defines the interface for a Modbus RTU driver.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-24
 * @copyright Copyright (c) 2024
 *
 * This header file defines the interface structure and function pointers
 * required for implementing a Modbus RTU driver. It includes functions
 * for initializing the driver, sending messages, enabling and disabling
 * reception, and subscribing to various callback events related to Modbus
 * communication.
 *
 * The `modbus_RTU_driver_struct_t` structure encapsulates function pointers
 * for initializing the driver (`init`), sending messages (`send`), enabling
 * reception (`enable_rcev`), disabling reception (`disable_rcev`), and subscribing
 * to specific callback events such as timeout expiration (`subscribe_t_1_5_char_expired_cb`),
 * message transmission completion (`subscribe_msg_tx_done_cb`), timeout error handling
 * (`subscribe_t_3_5_char_expired_cb`), and Modbus frame error detection (`subscribe_modbus_frame_error_cb`).
 *
 * @note Implement these functions according to the requirements of your Modbus RTU
 *       master or slave application. Ensure proper initialization and configuration
 *       of the driver functions for reliable Modbus communication.
 */
#ifndef _MODBUS_DRIVER_INTERFACE_H_
#define _MODBUS_DRIVER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"
#include <stdint.h>

    typedef void (*init_func_ptr_t)(baud_t baud, parity_t parity);
    typedef void (*send_func_ptr_t)(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
    typedef void (*enable_rx_func_ptr_t)(modbus_req_resp_t *recv_buf);
    typedef void (*driver_func_ptr_t)(void);

    typedef void (*driver_subscr_cb_t)(void);
    typedef void (*subscribed_func_ptr_t)(driver_subscr_cb_t callback);

    typedef struct
    {
        init_func_ptr_t init;
        send_func_ptr_t send;
        enable_rx_func_ptr_t enable_rcev;
        driver_func_ptr_t disable_rcev;
        subscribed_func_ptr_t subscribe_t_1_5_char_expired_cb;
        subscribed_func_ptr_t subscribe_msg_tx_done_cb;
        subscribed_func_ptr_t subscribe_t_3_5_char_expired_cb;
        subscribed_func_ptr_t subscribe_modbus_frame_error_cb;
    } modbus_RTU_driver_struct_t;

    const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void);
    const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DRIVER_INTERFACE_H_ */