/**
 * @file mock_modbus_data_interface.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "modbus_data.h"

extern modbus_coil_disin_t mock_slave_coil[COILS_QTY];
extern modbus_coil_disin_t mock_slave_dis_in[DISCRET_INPUT_QTY];
extern modbus_reg_t mock_slave_inreg[INPUT_REG_QTY];
extern modbus_reg_t mock_slave_hreg[HOLDING_REG_QTY];

void mock_set_all_slave_cails_to_off(void);
void mock_set_all_slave_din_to_off(void);
void mock_reset_all_slave_hreg_value(void);
void mock_reset_all_slave_inreg_value(void);
void mock_set_expected_slave_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_coil_disin_t start_val);
void mock_set_expected_slave_disc_in_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);
void mock_set_expected_slave_hreg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val);
void mock_set_expected_slave_input_reg_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty, modbus_reg_t init_val);

void mock_register_slave_coils_data(void);
void mock_register_slave_discrete_inputs_data(void);
void mock_register_slave_input_registers_data(void);
void mock_register_slave_holding_registers_data(void);

void mock_clear_modbus_slave_coils_data_table(void);
void mock_clear_modbus_slave_discrete_inputs_data_table(void);
void mock_clear_modbus_slave_input_register_data_table(void);
void mock_clear_modbus_slave_holding_resgister_data_table(void);
