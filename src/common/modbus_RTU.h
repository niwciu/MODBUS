/**
 * @file modbus_RTU.h
 * @brief Header file for Modbus RTU communication protocol implementation.
 *
 * This header file defines functions for sending and receiving Modbus messages using the
 * RTU (Remote Terminal Unit) communication protocol. It includes function declarations
 * for sending and receiving Modbus RTU messages.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-14
 * @version 0.0.1
 *
 * @copyright Copyright (c) 2024
 */
#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"
#include "modbus_public_type.h"

    modbus_ret_t modbus_RTU_send(modbus_buf_t *buf, modbus_buf_size_t *msg_len, modbus_device_ID_t slave_ID);
    modbus_ret_t modbus_RTU_recv(modbus_buf_t *buf, modbus_buf_size_t msg_len, modbus_device_ID_t slave_ID);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_RTU_H_ */