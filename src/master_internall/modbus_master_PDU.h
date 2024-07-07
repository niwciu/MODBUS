/**
 * @file modbus_master_PDU.h
 * @brief Header file defining Modbus master protocol data unit (PDU) functionality.
 *
 * This header file provides functions for generating Modbus master protocol data unit (PDU) requests
 * for various Modbus operations including reading holding registers, input registers, discrete inputs,
 * and coils, as well as writing single and multiple registers and coils. It also includes a function
 * for processing responses received from a Modbus slave.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-30
 * @version 1.0.0
 *
 * @copyright Copyright (c) 2024
 */
#ifndef _MODBUS_PDU_H_
#define _MODBUS_PDU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    #include "modbus_type.h"

    // Master Functionality
    modbus_ret_t modbus_master_read_holding_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_read_input_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_read_coils_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);

    modbus_ret_t modbus_master_write_single_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_write_single_coil_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_write_multiple_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
    modbus_ret_t modbus_master_write_multiple_coils_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);

    modbus_ret_t modbus_master_read_slave_resp(modbus_msg_t *modbus_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */