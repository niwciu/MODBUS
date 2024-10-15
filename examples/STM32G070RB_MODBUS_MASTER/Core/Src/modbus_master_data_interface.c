/**
 * @file modbus_master_data_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_master_data_interface.h"
#include "main.h"

extern modbus_coil_disin_t readed_coil;
extern modbus_reg_t readed_hreg;
extern modbus_coil_disin_t prev_coil_val;
extern modbus_reg_t prev_hreg_val;

void modbus_master_coil_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
{
    if( SLAVE_ADDRES_EXAMPLE_NODE == slave_adr)
    {
        if(READ_COIL_ADR == data_adr)
        {
            readed_coil= disin_val;
        }
    }
}

void modbus_master_hreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{
    if (SLAVE_ADDRES_EXAMPLE_NODE == slave_adr)
    {
        if (READ_HREG_ADR == data_adr)
        {
            readed_hreg = inreg_val;
        }
    }
}

// void modbus_master_coil_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t coil_qty)
// {

    
// }
void modbus_master_coil_exception(const modbus_read_data_t *resp_data)
{
    prev_coil_val = 0;
}
// void modbus_master_disin_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_coil_disin_t disin_val)
// {

// }
// void modbus_master_disin_exception(const modbus_read_data_t *resp_data)
// {

// }

// void modbus_master_hreg_write(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_data_qty_t reg_qty)
// {
    
// }
void modbus_master_hreg_exception(const modbus_read_data_t *resp_data)
{
    prev_hreg_val = 0;
}
void modbus_master_inreg_read(modbus_device_ID_t slave_adr, modbus_adr_t data_adr, modbus_reg_t inreg_val)
{

}
// void modbus_master_inreg_exception(const modbus_read_data_t *resp_data)
// {

// }
void modbus_master_data_timeout_error(const modbus_master_error_report_t *timeout_error_rep)
{
    prev_coil_val = 0;
    prev_hreg_val = 666;
}
void modbus_master_communication_error(const modbus_master_error_report_t *communication_error_rep)
{
    prev_coil_val = 0;
    prev_hreg_val = 777;
}