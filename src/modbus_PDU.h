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
// #include "modbus_type.h" // zaciągnięte z data.h



    
    // Master Functionality
    void register_app_data_to_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_master_discrete_inputs_table (modbus_adr_t din_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_master_input_registers_table (modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr);
    void register_app_data_to_master_holding_registers_table (modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr);

    modbus_ret_t modbus_master_read_holding_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty);
    modbus_ret_t modbus_master_read_input_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    modbus_ret_t modbus_master_read_coils_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    modbus_ret_t modbus_master_write_single_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_reg_t val);
    modbus_ret_t modbus_master_write_single_coil_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_w_coil_t coil_state);
    modbus_ret_t modbus_master_write_multiple_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, const modbus_reg_t *data_buf);
    modbus_ret_t modbus_master_write_multiple_coils_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty ,const modbus_coil_reg_t *data_buf);

    modbus_ret_t modbus_master_read_coils_resp(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_master_read_input_reg_resp (modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    
    // Slave Functionality
    void register_app_data_to_slave_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_slave_discrete_inputs_table (modbus_adr_t din_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_slave_input_registers_table (modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr);
    void register_app_data_to_slave_holding_registers_table (modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr);

    modbus_ret_t modbus_slave_read_coils(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_read_discrete_inputs(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_read_holdin_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_read_input_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);

    modbus_ret_t modbus_slave_write_single_coil(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_write_multiple_coils(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_write_single_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);
    modbus_ret_t modbus_slave_write_multiple_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */