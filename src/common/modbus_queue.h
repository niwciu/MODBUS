/**
 * @file modbus_manager.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _MODBUS_MANAGER_H_
#define _MODBUS_MANAGER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "modbus_type.h"
// #include "modbus_public_type.h"


void modbus_queue_init(modbus_queue_t *q);
void modbus_queue_push(modbus_queue_t *q, modbus_msg_t *data);
modbus_msg_t* modbus_queue_pop(modbus_queue_t *q);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MANAGER_H_ */