/**
 * @file mock_modbus_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "mock_modbus_data_interface.h"

modbus_r_coil_t mock_coil[COILS_QTY]={0};

modbus_r_coil_t get_coil_state(modbus_adr_t adr)
{
    return mock_coil[adr];
}

void set_coil_state(modbus_adr_t adr, modbus_r_coil_t coil_state)
{
    mock_coil[adr]=coil_state;
}

// void mock_set_expected_coil_state(modbus_adr_t adr, modbus_coil_t coil_state)
// {
//     set_coil_state(adr,coil_state);
// }

void mock_set_all_cails_to_off(void)
{
    for (int i=0; i<COILS_QTY; i++)
    {
        mock_coil[i]=0;
    }
}