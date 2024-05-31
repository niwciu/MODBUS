/**
 * @file modbus_slave.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MODBUS_SLAVE_H_
#define _MODBUS_SLAVE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_public_type.h"

void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity, modbus_device_ID_t slave_ID);
void check_modbus_request(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_H_ */