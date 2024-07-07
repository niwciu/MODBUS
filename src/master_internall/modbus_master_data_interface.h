/**
 * @file modbus_master_data_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @version 0.0.1
 * @date 2024-07-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "modbus_public_type.h"

void update_master_coils(modbus_device_ID_t slave_adr,modbus_adr_t data_adr, modbus_coil_disin_t coil_val);
void update_master_disin(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val);
void update_master_hreg(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val);
void update_master_inreg(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val);