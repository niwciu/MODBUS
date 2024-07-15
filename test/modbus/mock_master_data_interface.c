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
#include "mock_master_data_interface.h"
#include "modbus_config.h"
#include <memory.h>

modbus_coil_disin_t mock_master_coil_data[MOCK_MASTER_COILS_TABLE_SIZE] = {0};
modbus_coil_disin_t mock_master_dis_in[MOCK_MASTER_DIS_IN_TABLE_SIZE] = {0};
modbus_reg_t mock_master_holding_reg[MOCK_MASTER_IN_REG_TABLE_SIZE] = {0};
modbus_reg_t mock_master_inreg[MOCK_MASTER_H_REG_TABLE_SIZE] = {0};

modbus_read_data_t exception_data;
modbus_master_error_report_t modbus_master_error_rep;
typedef struct
{
    modbus_adr_t data_adr;
    modbus_data_qty_t coil_qty;
} confirmation_struct_t;
confirmation_struct_t mock_colis_write_confirmation;
confirmation_struct_t mock_reg_write_confirmation;

void modbus_master_coil_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t coil_val)
{
    (void)(slave_adr);
    mock_master_coil_data[data_adr] = coil_val;
}
void modbus_master_coil_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t coil_qty)
{
    (void)(slave_adr);
    mock_colis_write_confirmation.data_adr = data_adr;
    mock_colis_write_confirmation.coil_qty = coil_qty;
}
void modbus_master_inreg_exception(const modbus_read_data_t *resp_data)
{
    memset(&exception_data, 0, sizeof(modbus_read_data_t));
    exception_data = *resp_data;
}

void modbus_master_disin_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    (void)(slave_adr);
    mock_master_dis_in[data_adr] = disin_val;
}
void modbus_master_coil_exception(const modbus_read_data_t *resp_data)
{
    memset(&exception_data, 0, sizeof(modbus_read_data_t));
    exception_data = *resp_data;
}

void modbus_master_hreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t hreg_val)
{
    (void)(slave_adr);
    mock_master_holding_reg[data_adr] = hreg_val;
}
void modbus_master_hreg_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t hreg_qty)
{
    (void)(slave_adr);
    mock_reg_write_confirmation.data_adr = data_adr;
    mock_reg_write_confirmation.coil_qty = hreg_qty;
}
void modbus_master_disin_exception(const modbus_read_data_t *resp_data)
{
    memset(&exception_data, 0, sizeof(modbus_read_data_t));
    exception_data = *resp_data;
}

void modbus_master_inreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{
    (void)(slave_adr);
    mock_master_inreg[data_adr] = inreg_val;
}
void modbus_master_hreg_exception(const modbus_read_data_t *resp_data)
{
    memset(&exception_data, 0, sizeof(modbus_read_data_t));
    exception_data = *resp_data;
}

void modbus_master_data_timeout_error(const modbus_master_error_report_t *timeout_error_rep)
{
    memset(&modbus_master_error_rep, 0, sizeof(modbus_master_error_report_t));
    modbus_master_error_rep = *timeout_error_rep;
}

void mock_clear_modbus_master_coil_data(void)
{
    for (uint16_t i = 0; i < (sizeof(mock_master_coil_data) / sizeof(mock_master_coil_data[0])); i++)
    {
        mock_master_coil_data[i] = 0;
    }
}

void modbus_master_communication_error(const modbus_master_error_report_t *communication_error_rep)
{
    memset(&modbus_master_error_rep, 0, sizeof(modbus_master_error_report_t));
    modbus_master_error_rep = *communication_error_rep;
}