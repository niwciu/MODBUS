/**
 * @file modbus_data.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_data.h"
#include <stdio.h>

modbus_reg_t *Slave_Holding_Registers[HOLDING_REG_QTY] = {NULL};
modbus_reg_t *Slave_Input_Registers[INPUT_REG_QTY] = {NULL};
modbus_coil_disin_t *Slave_Coils[COILS_QTY] = {NULL};
modbus_coil_disin_t *Slave_Discrete_Inputs[DISCRET_INPUT_QTY] = {NULL};

modbus_reg_t *Master_Holding_Registers[HOLDING_REG_QTY] = {NULL};
modbus_reg_t *Master_Input_Registers[INPUT_REG_QTY] = {NULL};
modbus_coil_disin_t *Master_Coils[COILS_QTY] = {NULL};
modbus_coil_disin_t *Master_Discrete_Inputs[DISCRET_INPUT_QTY] = {NULL};

void register_app_data_to_modbus_coils_din_table(modbus_coil_disin_t **coil_din_table, modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    coil_din_table[coil_adr] = app_data_ptr;
}
void register_app_data_to_modbus_reg_table(modbus_reg_t **reg_table, modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    reg_table[reg_adr] = app_data_ptr;
}


modbus_coil_disin_t get_coil_state(modbus_coil_disin_t **coils_tab_ptr, modbus_adr_t coil_adr)
{
    if (NULL != (coils_tab_ptr[coil_adr]))
    {
        return *(coils_tab_ptr[coil_adr]);
    }
    else
        return 0;
}

modbus_ret_t set_coil_state(modbus_coil_disin_t **coils_tab_ptr, modbus_adr_t coil_adr, modbus_coil_disin_t coil_state)
{
    if (NULL != (coils_tab_ptr[coil_adr]))
    {
        *coils_tab_ptr[coil_adr] = coil_state;
        return RET_OK;
    }
    else
        return RET_ERROR;
}

modbus_coil_disin_t get_discrete_input_state(modbus_coil_disin_t **disin_tab_ptr, modbus_adr_t din_adr)
{
    if (NULL != (disin_tab_ptr[din_adr]))
    {
        return *disin_tab_ptr[din_adr];
    }
    else
        return 0;
}

modbus_reg_t get_input_register_state(modbus_reg_t **inreg_tab_ptr, modbus_adr_t input_reg_adr)
{
    if (NULL != (inreg_tab_ptr[input_reg_adr]))
    {
        return *inreg_tab_ptr[input_reg_adr];
    }
    else
        return 0;
}

modbus_reg_t get_holding_register_value(modbus_reg_t **hreg_tab_ptr, modbus_adr_t holding_reg_adr)
{
    if (NULL != (hreg_tab_ptr[holding_reg_adr]))
    {
        return *hreg_tab_ptr[holding_reg_adr];
    }
    else
        return 0;
}

modbus_ret_t set_register_value(modbus_reg_t **hreg_tab_ptr, modbus_adr_t holding_reg_adr, modbus_reg_t hreg_val)
{
    if (NULL != (hreg_tab_ptr[holding_reg_adr]))
    {
        *hreg_tab_ptr[holding_reg_adr] = hreg_val;
        return RET_OK;
    }
    else
        return RET_ERROR;
}
