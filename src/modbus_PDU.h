/**
 * @file modbus_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-06
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
#include "modbus_public_type.h"

    // Master and Slave Functionality
    void register_app_data_to_modbus_coils_din_table(modbus_coil_disin_t **modbus_data_type, modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_reg_table(modbus_reg_t **reg_table, modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);

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
#endif /* _MODBUS_PDU_H_ */