/**
 * @file modbus_master_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MODBUS_PDU_H_
#define _MODBUS_PDU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_data.h"
// #include "modbus_public_type.h"

    // Master Functionality
    modbus_ret_t modbus_master_read_holding_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t hreg_qty);
    modbus_ret_t modbus_master_read_input_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    modbus_ret_t modbus_master_read_coils_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    modbus_ret_t modbus_master_write_single_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr);
    modbus_ret_t modbus_master_write_single_coil_req(modbus_msg_t *modbus_msg, modbus_adr_t adr);
    modbus_ret_t modbus_master_write_multiple_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_write_multiple_coils_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    modbus_ret_t modbus_master_read_coils_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_read_discrete_inputs_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_read_input_reg_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_read_holding_reg_resp(const modbus_msg_t *modbus_msg);

    modbus_ret_t modbus_master_write_single_coil_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_write_single_reg_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_write_multiple_coils_resp(const modbus_msg_t *modbus_msg);
    modbus_ret_t modbus_master_write_multiple_reg_resp(const modbus_msg_t *modbus_msg);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */