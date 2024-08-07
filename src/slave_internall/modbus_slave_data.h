/**
 * @file modbus_data.h
 * @brief Header file for Modbus data handling.
 *
 * This header file defines structures and functions related to Modbus data handling,
 * including registers, coils, and discrete inputs for both slave and master devices.
 *
 * @date 2024-05-09
 * @author niwciu (niwciu@gmail.com)
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_DATA_H_
#define _MODBUS_DATA_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"
#ifndef UNIT_TEST
#include "modbus_config.h"
#else
#include "modbus_unit_test_config.h"
#endif

/**
 * Number of holding registers as determined by the application or protocol limits.
 */
#if MAIN_APP_HOLDING_REG_QTY < MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define HOLDING_REG_QTY MAIN_APP_HOLDING_REG_QTY
#else
#define HOLDING_REG_QTY MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#endif

/**
 * Number of input registers as determined by the application or protocol limits.
 */
#if MAIN_APP_INPUT_REG_QTY < MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define INPUT_REG_QTY (MAIN_APP_INPUT_REG_QTY)
#else
#define INPUT_REG_QTY (MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY)
#endif

/**
 * Number of coils as determined by the application or protocol limits.
 */
#if MAIN_APP_COILS_QTY < MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define COILS_QTY (MAIN_APP_COILS_QTY)
#else
#define COILS_QTY (MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY)
#endif

/**
 * Number of discrete inputs as determined by the application or protocol limits.
 */
#if MAIN_APP_DISCRET_INPUTS_QTY < MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define DISCRET_INPUT_QTY (MAIN_APP_DISCRET_INPUTS_QTY)
#else
#define DISCRET_INPUT_QTY (MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY)
#endif

    extern modbus_reg_t *Slave_Holding_Registers[HOLDING_REG_QTY]; /* * @brief Array of pointers to holding registers for the Modbus slave.*/
    extern modbus_reg_t *Slave_Input_Registers[INPUT_REG_QTY];
    extern modbus_coil_disin_t *Slave_Coils[COILS_QTY];
    extern modbus_coil_disin_t *Slave_Discrete_Inputs[DISCRET_INPUT_QTY];

    void register_app_data_to_modbus_coils_din_table(modbus_coil_disin_t **coil_din_table, modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr);
    void register_app_data_to_modbus_reg_table(modbus_reg_t **reg_table, modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr);

    modbus_ret_t get_coil_din_state(modbus_coil_disin_t **coils_din_tab_ptr, modbus_adr_t coil_din_adr);
    modbus_ret_t set_coil_state(modbus_coil_disin_t **coils_tab_ptr, modbus_adr_t coil_adr, modbus_coil_disin_t coil_state);

    modbus_ret_t get_register_state(modbus_reg_t **reg_tab_ptr, modbus_adr_t reg_adr);
    modbus_ret_t set_register_value(modbus_reg_t **hreg_tab_ptr, modbus_adr_t holding_reg_adr, modbus_reg_t hreg_val);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DATA_H_ */