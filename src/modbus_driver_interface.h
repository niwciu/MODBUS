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

    typedef void (*init_func_ptr_t)(baud_t baud, parity_t parity);
    typedef void (*send_func_ptr_t)(modbus_buf_t *tx_msg, uint8_t msg_len);
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
    }modbus_RTU_driver_struct_t;

    const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void);
    const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DRIVER_INTERFACE_H_ */