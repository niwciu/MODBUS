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
#define COILS_QTY 2000
#define DIS_IN_QTY 2000

#include "modbus_type.h"
#include "modbus_data_interface.h"

    extern modbus_r_coil_t mock_coil[COILS_QTY];

    void mock_set_all_cails_to_off(void);
    void mock_set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
    void mock_set_expected_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
    // void mock_set_expected_coil_state(modbus_adr_t adr, modbus_coil_t coil_state);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MOCK_MODBUS_DATA_INTERFACE_H_ */