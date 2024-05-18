/**
 * @file mock_modbus_data_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _MOCK_MODBUS_MASTER_DATA_INTERFACE_H_
#define _MOCK_MODBUS_MASTER_DATA_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "modbus_data.h"

    extern modbus_coil_disin_t mock_master_coil[COILS_QTY];
    extern modbus_coil_disin_t mock_master_dis_in[DISCRET_INPUT_QTY];
    extern modbus_reg_t mock_master_inreg[INPUT_REG_QTY];
    extern modbus_reg_t mock_master_hreg[HOLDING_REG_QTY];

    void mock_set_all_master_cails_to_off(void);
    void mock_set_all_master_din_to_off(void);
    void mock_reset_all_master_hreg_value(void);
    void mock_set_expected_master_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_coil_disin_t start_val);
    void mock_set_expected_master_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
    void mock_set_expected_master_hreg_alternately (modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val);
    void mock_set_expected_master_input_reg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val);
    
    void mock_register_master_coils_data(void);
    void mock_register_master_discrete_inputs_data(void);
    void mock_register_master_input_registers_data(void);
    void mock_register_master_holding_registers_data(void);

    void mock_clear_master_modbus_coils_data_teble(void);
    void mock_clear_master_modbus_discrete_inputs_data_teble(void);
    void mock_clear_master_modbus_input_register_data_teble(void);
    void mock_clear_master_modbus_holding_resgister_data_table(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MOCK_MODBUS_MASTER_DATA_INTERFACE_H_ */