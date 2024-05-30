/**
 * @file modbus_slave_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MODBUS_SLAVE_PDU_H_
#define _MODBUS_SLAVE_PDU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_data.h"

    // Slave Functionality
    modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg);

    modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_PDU_H_ */