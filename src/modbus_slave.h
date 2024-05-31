/**
 * @file modbus_slave.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MODBUS_MASTER_H_
#define _MODBUS_MASTER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_public_type.h"

void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
void check_modbus_request(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MASTER_H_ */