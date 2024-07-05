/**
 * @file modbus_slave_data.c
 * @brief Implementation of Modbus data handling functions.
 *
 * This file contains the implementation of functions for registering application data
 * to Modbus tables (coils, discrete inputs, holding registers, input registers) and
 * for accessing and setting the states/values of these data items.
 *
 * @date 2024-05-07
 */
#include "modbus_slave_data.h"
#include <stdio.h>

modbus_reg_t *Slave_Holding_Registers[HOLDING_REG_QTY];        /**< Array of pointers to holding registers for the Modbus slave. */
modbus_reg_t *Slave_Input_Registers[INPUT_REG_QTY];            /**< Array of pointers to input registers for the Modbus slave. */
modbus_coil_disin_t *Slave_Coils[COILS_QTY];                   /**< Array of pointers to coils for the Modbus slave. */
modbus_coil_disin_t *Slave_Discrete_Inputs[DISCRET_INPUT_QTY]; /**< Array of pointers to discrete inputs for the Modbus slave. */

/**
 * @brief Registers application data to the Modbus coils or discrete inputs table.
 *
 * This function registers application data to the specified table of coils or discrete inputs
 * at the given address.
 *
 * @param coil_din_table Pointer to the table of coils or discrete inputs.
 * @param coil_adr Address within the table to register the application data.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_coils_din_table(modbus_coil_disin_t **coil_din_table, modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    coil_din_table[coil_adr] = app_data_ptr;
}

/**
 * @brief Registers application data to the Modbus holding or input registers table.
 *
 * This function registers application data to the specified table of holding or input registers
 * at the given address.
 *
 * @param reg_table Pointer to the table of holding or input registers.
 * @param reg_adr Address within the table to register the application data.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_reg_table(modbus_reg_t **reg_table, modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    reg_table[reg_adr] = app_data_ptr;
}

/**
 * @brief Gets the state of a coil or discrete input at the specified address.
 *
 * This function retrieves the state of a coil or discrete input from the specified table
 * at the given address.
 *
 * @param coils_din_tab_ptr Pointer to the table of coils or discrete inputs.
 * @param coil_din_adr Address of the coil or discrete input to query.
 * @return modbus_ret_t The state of the coil or discrete input.
 */
modbus_ret_t get_coil_din_state(modbus_coil_disin_t **coils_din_tab_ptr, modbus_adr_t coil_din_adr)
{
    if (NULL != (coils_din_tab_ptr[coil_din_adr]))
    {
        return (modbus_ret_t)(*coils_din_tab_ptr[coil_din_adr]);
    }
    else
        return RET_ERROR;
}

/**
 * @brief Sets the state of a coil at the specified address.
 *
 * This function sets the state of a coil in the specified table at the given address.
 *
 * @param coils_tab_ptr Pointer to the table of coils.
 * @param coil_adr Address of the coil to set.
 * @param coil_state State to set for the coil (ON or OFF).
 * @return modbus_ret_t Returns RET_OK if successful, otherwise RET_ERROR.
 */
modbus_ret_t set_coil_state(modbus_coil_disin_t **coils_tab_ptr, modbus_adr_t coil_adr, modbus_coil_disin_t coil_state)
{
    if (NULL != coils_tab_ptr[coil_adr])
    {
        *coils_tab_ptr[coil_adr] = coil_state;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

/**
 * @brief Gets the value of a holding or input register at the specified address.
 *
 * This function retrieves the value of a holding or input register from the specified
 * table at the given address.
 *
 * @param reg_tab_ptr Pointer to the table of holding or input registers.
 * @param reg_adr Address of the register to query.
 * @return modbus_ret_t The value of the register.
 */
modbus_ret_t get_register_state(modbus_reg_t **reg_tab_ptr, modbus_adr_t reg_adr)
{
    if (NULL != (reg_tab_ptr[reg_adr]))
    {
        return (modbus_ret_t)(*reg_tab_ptr[reg_adr]);
    }
    else
    {
        return RET_ERROR;
    }
}

/**
 * @brief Sets the value of a holding register at the specified address.
 *
 * This function sets the value of a holding register in the specified table at the
 * given address.
 *
 * @param hreg_tab_ptr Pointer to the table of holding registers.
 * @param holding_reg_adr Address of the holding register to set.
 * @param hreg_val Value to set for the holding register.
 * @return modbus_ret_t Returns RET_OK if successful, otherwise RET_ERROR.
 * @retval - RET_OK - set requested value succesfull
 * @retval - RET_ERROR - no data registered in holding register tab
 */
modbus_ret_t set_register_value(modbus_reg_t **hreg_tab_ptr, modbus_adr_t holding_reg_adr, modbus_reg_t hreg_val)
{
    if (NULL != (hreg_tab_ptr[holding_reg_adr]))
    {
        *hreg_tab_ptr[holding_reg_adr] = hreg_val;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}
