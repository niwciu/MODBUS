/**
 * @file modbus_RTU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024
 *
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
    /**
     * @brief
     *
     * @param buf
     * @param data_len - lenght of whole message including CRC. most probabbly need to be passed from USART Or readed from MSG
     * @param slave_ID
     * @return modbus_ret_t
     */
    modbus_ret_t modbus_RTU_recv(modbus_buf_t *buf, modbus_buf_size_t msg_len, modbus_device_ID_t slave_ID);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_RTU_H_ */