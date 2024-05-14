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
    modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty);
    modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    modbus_ret_t modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val);
    modbus_ret_t modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_w_coil_t coil_state);
    modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, const modbus_reg_t *data_buf);
    modbus_ret_t modbus_master_write_multiple_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty ,const modbus_coil_reg_t *data_buf);

    // Slave Functionality
    void register_app_data_to_coils_table(modbus_adr_t coil_adr, modbus_coil_t *app_data_ptr);
    void register_app_data_to_discrete_inputs_table (modbus_adr_t din_adr, modbus_coil_t *app_data_ptr);
    void register_app_data_to_input_registers_table (modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr);
    void register_app_data_to_holding_registers_table (modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr);

    modbus_ret_t modbus_slave_read_coils(uint8_t *resp_buf, const uint8_t *req_buf);
    modbus_ret_t modbus_slave_read_discrete_inputs(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_read_holdin_reg(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_read_input_reg(uint8_t *resp_buf, const uint8_t *req_buf);

    void modbus_slave_write_single_coil(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_write_multiple_coils(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_write_single_reg(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_write_multiple_reg(uint8_t *resp_buf, const uint8_t *req_buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */