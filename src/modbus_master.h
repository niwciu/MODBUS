
/**
 * @file modbus_master.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-27
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

    void register_modbus_master_error_cb(modbus_master_error_cb_t error_callback);
    modbus_master_req_ret_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr);
    modbus_master_req_ret_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr);
    modbus_master_req_ret_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr);
    modbus_master_req_ret_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr);

    modbus_master_req_ret_t modbus_master_write_single_coil(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_coil_disin_t coil_2_write);
    modbus_master_req_ret_t modbus_master_write_single_reg(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_reg_t reg_2_write);

    modbus_master_req_ret_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr);
    modbus_master_req_ret_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr);

    void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
    void check_modbus_master_manager(void); // zmiana nazwy na check_slave_respo albo master_parse_modbus_resp albo check_modbus_master_resp

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MASTER_H_ */