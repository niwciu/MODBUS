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

#define MODBUS_PDU_FRAME_LEN 256U

    
    // Master Functionality
    modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty);
    modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val);
    void modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_w_coil_t coil_state);
    modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, const modbus_reg_t *data_buf);

    // Slave Functionality

    void register_app_data_to_coil_table(modbus_adr_t coil_adr, modbus_coil_t *app_data_ptr);
    void modbus_slave_read_coils(uint8_t *resp_buf, const uint8_t *req_buf);
    void modbus_slave_read_discrete_inputs(uint8_t *resp_buf, const uint8_t *req_buf);
    // void modbus_slave_read_holdin_reg(uint8_t *resp_buf, const uint8_t *req_buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */