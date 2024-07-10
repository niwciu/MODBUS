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

void modbus_master_coil_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t coil_val);
void modbus_master_coil_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t coil_qty);
void modbus_master_coil_exception(const modbus_read_data_t *resp_data);

void modbus_master_disin_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val);
void modbus_master_disin_exception(const modbus_read_data_t *resp_data);

void modbus_master_hreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val);
void modbus_master_hreg_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t reg_qty);
void modbus_master_hreg_exception(const modbus_read_data_t *resp_data);

void modbus_master_inreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val);
void modbus_master_inreg_exception(const modbus_read_data_t *resp_data);

void modbus_master_data_timeout_error(const modbus_master_error_report_t *timeout_error_rep);