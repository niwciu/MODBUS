/**
 * @file modbus_master.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @version 0.0.1
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
#include "modbus_data.h"
// #include "modbus_public_type.h"

    void register_app_data_to_modbus_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_master_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_master_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);
    void register_app_data_to_modbus_master_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);
    
    // modbus_master_error_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty);
    // modbus_master_error_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    // modbus_master_error_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_master_error_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty,modbus_device_ID_t slave_ID);
    
    
    

    // modbus_master_error_t modbus_master_write_single_reg(modbus_adr_t adr);
    // modbus_master_error_t modbus_master_write_single_coil(modbus_adr_t adr);
    // modbus_master_error_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty);
    // modbus_master_error_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty);

    void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
    void check_modbus_master_manager(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MASTER_H_ */