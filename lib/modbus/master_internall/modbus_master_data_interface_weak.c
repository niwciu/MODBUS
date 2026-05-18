/**
 * @file modbus_master_data_interface_weak.c
 * @author 
 * @brief Provides default weak implementations for Modbus master data interface callbacks.
 * 
 * This file contains empty (stub) implementations of the Modbus master data interface functions. 
 * All functions are marked with the `weak` attribute, which allows the application developer 
 * to override them with custom implementations without modifying the library itself.
 * 
 * By default, these functions do nothing and simply cast unused parameters to `(void)` 
 * to avoid compiler warnings.
 * 
 * Typical usage:
 * - The application can override any of these functions by providing its own non-weak 
 *   implementation with the same signature.
 * - If no custom implementation is provided, the weak default (empty) function will be used.
 * 
 * @version 1.0.0
 * @date 2025-08-26
 * 
 * @copyright Copyright (c) 2025
 */

#include "modbus_master_data_interface.h"

#define MODBUS_WEAK __attribute__((weak))

MODBUS_WEAK void modbus_master_coil_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t coil_val)
{
    (void)slave_adr;
    (void)data_adr;
    (void)coil_val;
}

MODBUS_WEAK void modbus_master_coil_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t coil_qty)
{
    (void)slave_adr;
    (void)data_adr;
    (void)coil_qty;
}

MODBUS_WEAK void modbus_master_coil_exception(const modbus_read_data_t *resp_data)
{
    (void)resp_data;
}

MODBUS_WEAK void modbus_master_disin_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    (void)slave_adr;
    (void)data_adr;
    (void)disin_val;
}

MODBUS_WEAK void modbus_master_disin_exception(const modbus_read_data_t *resp_data)
{
    (void)resp_data;
}

MODBUS_WEAK void modbus_master_hreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val)
{
    (void)slave_adr;
    (void)data_adr;
    (void)hreg_val;
}

MODBUS_WEAK void modbus_master_hreg_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t reg_qty)
{
    (void)slave_adr;
    (void)data_adr;
    (void)reg_qty;
}

MODBUS_WEAK void modbus_master_hreg_exception(const modbus_read_data_t *resp_data)
{
    (void)resp_data;
}

MODBUS_WEAK void modbus_master_inreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{
    (void)slave_adr;
    (void)data_adr;
    (void)inreg_val;
}

MODBUS_WEAK void modbus_master_inreg_exception(const modbus_read_data_t *resp_data)
{
    (void)resp_data;
}

MODBUS_WEAK void modbus_master_data_timeout_error(const modbus_master_error_report_t *timeout_error_rep)
{
    (void)timeout_error_rep;
}

MODBUS_WEAK void modbus_master_communication_error(const modbus_master_error_report_t *communication_error_rep)
{
    (void)communication_error_rep;
}
