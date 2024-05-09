/**
 * @file mock_modbus_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "mock_app_data.h"
#include "modbus_PDU.h"
#include "modbus_data.h"
#include <stdio.h>

modbus_coil_t mock_coil[COILS_QTY] = {0};
modbus_disin_t mock_dis_in[DISCRET_INPUT_QTY] = {0};
modbus_reg_t mock_inreg[INPUT_REG_QTY] = {0};
modbus_reg_t mock_hreg[HOLDING_REG_QTY] = {0};
uint8_t mock_error_triger = 0;

void mock_set_all_cails_to_off(void)
{
    for (modbus_adr_t i = 0; i < COILS_QTY; i++)
    {
        mock_coil[i] = 0;
    }
}

void mock_set_all_din_to_off(void)
{
    for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
    {
        mock_dis_in[i] = 0;
    }
}

void mock_set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_coil_t current_state = 1;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_coil[start_adr + i] = current_state;
        current_state = (!current_state);
    }
}

void mock_set_expected_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_disin_t current_state = 1;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_dis_in[start_adr + i] = current_state;
        current_state = (!current_state);
    }
}

void mock_set_expected_hreg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val)
{
    modbus_reg_t reg_current_state = init_val;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_hreg[start_adr + i] = reg_current_state;
        reg_current_state = ~reg_current_state;
    }
}

void mock_set_expected_input_reg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val)
{
    modbus_reg_t reg_current_state = init_val;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_inreg[start_adr + i] = reg_current_state;
        reg_current_state = ~reg_current_state;
    }
}

void mock_register_coils_data(void)
{
    for (modbus_adr_t i = 0; i < COILS_QTY; i++)
    {
        Coils[i] = &mock_coil[i];
    }
}

void mock_register_discrete_inputs_data(void)
{
    for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
    {
        Discrete_Inputs[i] = &mock_dis_in[i];
    }
}

void mock_register_input_registers_data(void)
{
    for (modbus_adr_t i = 0; i < INPUT_REG_QTY; i++)
    {
        Input_Registers[i] = &mock_inreg[i];
    }
}

void mock_register_holding_registers_data(void)
{
    for (modbus_adr_t i = 0; i < HOLDING_REG_QTY; i++)
    {
        Holding_Registers[i] = &mock_hreg[i];
    }
}

void mock_clear_modbus_coils_data_teble(void)
{
    for (modbus_adr_t i = 0; i < COILS_QTY; i++)
    {
        Coils[i] = NULL;
    }
}

void mock_clear_modbus_discrete_inputs_data_teble(void)
{
    for (modbus_adr_t i = 0; i < DISCRET_INPUT_QTY; i++)
    {
        Discrete_Inputs[i] = NULL;
    }
}

void mock_clear_modbus_input_register_data_teble(void)
{
    for (modbus_adr_t i = 0; i < INPUT_REG_QTY; i++)
    {
        Input_Registers[i] = NULL;
    }
}