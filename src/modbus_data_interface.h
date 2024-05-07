/**
 * @file modbus_data_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_DATA_INTERFACE_H_
#define _MODBUS_DATA_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "modbus_type.h"
#include "stdbool.h"

typedef bool modbus_r_coil_t;

modbus_r_coil_t get_coil_state(modbus_adr_t adr);
void set_coil_state(modbus_adr_t adr, modbus_r_coil_t coil_state);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DATA_INTERFACE_H_ */