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

modbus_reg_t *Holding_Registers[HOLDING_REG_QTY] = {NULL};
modbus_reg_t *Input_Registers[INPUT_REG_QTY] = {NULL};
modbus_coil_t *Coils[COILS_QTY] = {NULL};
modbus_disin_t *Discrete_Inputs[DISCRET_INPUT_QTY] = {NULL};


modbus_coil_t get_coil_state(modbus_adr_t adr)
{
    if(NULL != (Coils[adr]))
    {
        return *Coils[adr];
    }
    else return 0;
}

void set_coil_state(modbus_adr_t coil_adr, modbus_coil_t coil_state)
{

}

modbus_disin_t get_discrete_input_state(modbus_adr_t din_adr)
{
    if(NULL != (Discrete_Inputs[din_adr]))
    {
        return *Discrete_Inputs[din_adr];
    }
    else return 0;
}

modbus_reg_t get_input_register_state (modbus_adr_t input_reg_adr)
{
    if(NULL != (Input_Registers[input_reg_adr]))
    {
        return *Input_Registers[input_reg_adr];
    }
    else return 0;

}
