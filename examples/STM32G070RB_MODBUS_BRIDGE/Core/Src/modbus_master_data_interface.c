/**
 * @file modbus_master_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus master data callbacks for the bridge example.
 *        Data read from the downstream slave is written directly into the arrays
 *        registered in the upstream slave's data tables (bridge_coil_data / bridge_hreg_data).
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "modbus_master_data_interface.h"
#include "main.h"

extern modbus_coil_disin_t bridge_coil_data[];
extern modbus_reg_t        bridge_hreg_data[];

void modbus_master_coil_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    if (DOWNSTREAM_SLAVE_ID == slave_adr && data_adr < BRIDGE_DATA_QTY)
        bridge_coil_data[data_adr] = disin_val;
}

void modbus_master_hreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val)
{
    if (DOWNSTREAM_SLAVE_ID == slave_adr && data_adr < BRIDGE_DATA_QTY)
        bridge_hreg_data[data_adr] = hreg_val;
}

void modbus_master_coil_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t coil_qty)
{
    (void)(slave_adr);
    (void)(data_adr);
    (void)(coil_qty);
}

void modbus_master_coil_exception(const modbus_read_data_t *resp_data)
{
    (void)(*resp_data);
}

void modbus_master_disin_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    (void)(slave_adr);
    (void)(data_adr);
    (void)(disin_val);
}

void modbus_master_disin_exception(const modbus_read_data_t *resp_data)
{
    (void)(*resp_data);
}

void modbus_master_hreg_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t reg_qty)
{
    (void)(slave_adr);
    (void)(data_adr);
    (void)(reg_qty);
}

void modbus_master_hreg_exception(const modbus_read_data_t *resp_data)
{
    (void)(*resp_data);
}

void modbus_master_inreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{
    (void)(slave_adr);
    (void)(data_adr);
    (void)(inreg_val);
}

void modbus_master_inreg_exception(const modbus_read_data_t *resp_data)
{
    (void)(*resp_data);
}

void modbus_master_data_timeout_error(const modbus_master_error_report_t *timeout_error_rep)
{
    (void)(*timeout_error_rep);
}

void modbus_master_communication_error(const modbus_master_error_report_t *communication_error_rep)
{
    (void)(*communication_error_rep);
}
