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
modbus_ret_t modbus_RTU_send(modbus_buf_t * buf, modbus_buf_size_t data_len, modbus_device_ID_t slave_ID);
modbus_ret_t modbus_RTU_recv(modbus_buf_t * buf, modbus_buf_size_t data_len, modbus_device_ID_t slave_ID);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_RTU_H_ */