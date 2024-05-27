/**
 * @file modbus_master.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_PDU.h"
#include <stdio.h>

void register_app_data_to_modbus_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_din_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    
}

modbus_ret_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    return 0;
}
modbus_ret_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return 0;
}
modbus_ret_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    return 0;
}
modbus_ret_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return 0;
}
modbus_ret_t modbus_master_write_single_reg(modbus_adr_t adr)
{
    return 0;
}
modbus_ret_t modbus_master_write_single_coil(modbus_adr_t adr)
{
    return 0;
}
modbus_ret_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return 0;
}
modbus_ret_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return 0;
}