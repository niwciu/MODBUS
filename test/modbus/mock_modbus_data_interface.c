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
#include "modbus_PDU.h"

modbus_r_coil_t mock_coil[MODBUS_MAX_COILS_READ_QTY]={0};
modbus_r_DisIn_t mock_dis_in[MODBUS_MAX_DISCRETE_INPUTS_READ_QTY]={0};
modbus_reg_t mock_hreg[MODBUS_MAX_REG_RW_QTY];
uint8_t mock_error_triger=0;

modbus_r_coil_t get_coil_state(modbus_adr_t adr)
{   
    if (mock_error_triger == 0)
        return mock_coil[adr];
    else
        return READ_COIL_ERROR;
}

void set_coil_state(modbus_adr_t adr, modbus_r_coil_t coil_state)
{
    mock_coil[adr]=coil_state;
}

modbus_r_DisIn_t get_din_state(modbus_adr_t adr)
{   
    if (mock_error_triger == 0)
        return mock_dis_in[adr];
    else
        return READ_DIS_IN_ERROR;
}


void mock_set_all_cails_to_off(void)
{
    for (int i=0; i<COILS_QTY; i++)
    {
        mock_coil[i]=READED_COIL_LOW;
    }
}

void mock_set_all_din_to_off(void)
{
    for (int i=0; i<DIS_IN_QTY; i++)
    {
        mock_dis_in[i]=READED_DIS_IN_LOW;
    }   
}

void mock_set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_r_coil_t current_state = READED_COIL_HIGH;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_coil[start_adr+i]=current_state;
        current_state=!current_state;
    }
}

void mock_set_expected_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_r_DisIn_t current_state = READED_DIS_IN_HIGH;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_dis_in[start_adr+i]=current_state;
        current_state=!current_state;
    }

}

void mock_set_expected_hreg_alternately (modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_reg_t reg_current_state= 0xA55A;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_hreg[start_adr+i]=reg_current_state;
        reg_current_state=!reg_current_state;
    }
}