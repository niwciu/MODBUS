/**
 * @file mock_modbus_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "mock_master_app_data.h"

modbus_coil_disin_t mock_master_coil[COILS_QTY] = {0};
modbus_coil_disin_t mock_master_dis_in[DISCRET_INPUT_QTY] = {0};
modbus_reg_t mock_master_inreg[INPUT_REG_QTY] = {0};
modbus_reg_t mock_master_hreg[HOLDING_REG_QTY] = {0};

void mock_set_all_master_cails_to_off(void)
{
    for (modbus_adr_t i = 0; i < COILS_QTY; i++)
    {
        mock_master_coil[i] = 0;
    }
}

void mock_set_all_master_din_to_off(void)
{
    for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
    {
        mock_master_dis_in[i] = 0;
    }
}

void mock_reset_all_master_inreg_value(void)
{
    for (modbus_adr_t i = 0; i < INPUT_REG_QTY; i++)
    {
        mock_master_inreg[i] = 0;
    }
}
void mock_reset_all_master_hreg_value(void)
{
    for (modbus_adr_t i = 0; i < HOLDING_REG_QTY; i++)
    {
        mock_master_hreg[i] = 0;
    }
}

void mock_set_expected_master_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_coil_disin_t start_val)
{
    modbus_coil_disin_t current_state = start_val;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_master_coil[start_adr + i] = current_state;
        current_state = (!current_state);
    }
}

// void mock_set_expected_master_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
// {
//     modbus_coil_disin_t current_state = 1;
//     for (modbus_data_qty_t i = 0; i < qty; i++)
//     {
//         mock_master_dis_in[start_adr + i] = current_state;
//         current_state = (!current_state);
//     }
// }

void mock_set_expected_master_hreg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val)
{
    modbus_reg_t reg_current_state = init_val;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_master_hreg[start_adr + i] = reg_current_state;
        reg_current_state = ~reg_current_state;
    }
}

// void mock_set_expected_master_input_reg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val)
// {
//     modbus_reg_t reg_current_state = init_val;
//     for (modbus_data_qty_t i = 0; i < qty; i++)
//     {
//         mock_master_inreg[start_adr + i] = reg_current_state;
//         reg_current_state = ~reg_current_state;
//     }
// }

void mock_register_master_coils_data(void)
{
    for (modbus_adr_t i = 0; i < COILS_QTY; i++)
    {
        Master_Coils[i] = &mock_master_coil[i];
    }
}

void mock_register_master_discrete_inputs_data(void)
{
    for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
    {
        Master_Discrete_Inputs[i] = &mock_master_dis_in[i];
    }
}

void mock_register_master_input_registers_data(void)
{
    for (modbus_adr_t i = 0; i < INPUT_REG_QTY; i++)
    {
        Master_Input_Registers[i] = &mock_master_inreg[i];
    }
}

void mock_register_master_holding_registers_data(void)
{
    for (modbus_adr_t i = 0; i < HOLDING_REG_QTY; i++)
    {
        Master_Holding_Registers[i] = &mock_master_hreg[i];
    }
}

// void mock_clear_master_modbus_coils_data_teble(void)
// {
//     for (modbus_adr_t i = 0; i < COILS_QTY; i++)
//     {
//         Master_Coils[i] = NULL;
//     }
// }

// void mock_clear_master_modbus_discrete_inputs_data_teble(void)
// {
//     for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
//     {
//         Master_Discrete_Inputs[i] = NULL;
//     }
// }

// void mock_clear_master_modbus_input_register_data_teble(void)
// {
//     for (modbus_adr_t i = 0; i < INPUT_REG_QTY; i++)
//     {
//         Master_Input_Registers[i] = NULL;
//     }
// }

// void mock_clear_master_modbus_holding_resgister_data_table(void)
// {
//     for (modbus_adr_t i = 0; i < HOLDING_REG_QTY; i++)
//     {
//         Master_Holding_Registers[i] = NULL;
//     }
// }