/**
 * @file mock_modbus_data_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _MOCK_MODBUS_DATA_INTERFACE_H_
#define _MOCK_MODBUS_DATA_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "modbus_type.h"

    void mock_set_all_cails_to_off(void);
    void mock_set_all_din_to_off(void);
    void mock_set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
    void mock_set_expected_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
    void mock_set_expected_hreg_alternately (modbus_adr_t start_adr, modbus_data_qty_t qty);
    // void mock_set_expected_coil_state(modbus_adr_t adr, modbus_coil_t coil_state);
    void mock_register_coils_data(void);
    void mock_register_discrete_inputs_data(void);
    void mock_register_coils_data(void);
    void mock_register_input_registers_data(void);
    void mock_register_holding_registers_data(void);

    void mock_clear_modbus_coils_data_teble(void);
    void mock_clear_modbus_discrete_inputs_data_teble(void);
    void mock_clear_modbus_input_register_data_teble(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MOCK_MODBUS_DATA_INTERFACE_H_ */