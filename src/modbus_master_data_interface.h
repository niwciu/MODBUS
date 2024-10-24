/**
 * @file modbus_master_data_interface.h
 * @brief Defines functions for Modbus master data operations and error handling.
 * @author niwciu (niwciu@gmail.com)
 * @version 0.0.1
 * @date 2024-07-07
 * @copyright Copyright (c) 2024
 *
 * This header file declares functions for Modbus master data operations and error handling.
 * It includes functions for reading and writing coils (discrete outputs), reading and writing
 * holding registers (Holding Registers), reading discrete inputs, and handling exceptions for
 * coil and register reads. Additionally, it provides functions to handle timeouts and
 * communication errors specific to Modbus master operations.
 *
 * The functions are designed to interact with a Modbus slave device identified by its address
 * (`slave_adr`) and specific data addresses (`data_adr`). They utilize types defined in
 * `modbus_public_type.h` for consistency and clarity in data handling.
 *
 * @note Implement these functions according to the Modbus protocol specifications and the
 *       requirements of your Modbus master application. Handle errors and exceptions
 *       appropriately to ensure reliable communication with Modbus slave devices.
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
void modbus_master_communication_error(const modbus_master_error_report_t *communication_error_rep);