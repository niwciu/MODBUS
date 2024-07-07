/**
 * @file mock_master_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-07-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_master_data_interface.h"

modbus_coil_disin_t mock_master_coil_data[100] = {0};
 modbus_coil_disin_t mock_master_dis_in[100] = {0};
 modbus_reg_t mock_master_holding_reg[100] = {0};
 modbus_reg_t mock_master_inreg[100] = {0};

void update_master_coils(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t coil_val)
{
    (void)(slave_adr);
    mock_master_coil_data[data_adr] = coil_val;
}

void update_master_disin(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    (void)(slave_adr);
    mock_master_dis_in[data_adr] = disin_val;
}

void update_master_hreg(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val)
{
    (void)(slave_adr);
    mock_master_holding_reg[data_adr] = hreg_val;
}
void update_master_inreg(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{
    (void)(slave_adr);
    mock_master_inreg[data_adr] = inreg_val;
}