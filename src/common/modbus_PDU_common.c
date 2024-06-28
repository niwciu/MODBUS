/**
 * @file modbus_PDU_common.c
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @version 0.0.1
 * @date 2024-06-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "modbus_PDU_common.h"
#include "modbus_data.h"

modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty)
{
    modbus_byte_count_t byte_count;

    byte_count = (coil_qty / 8);
    if (coil_qty % 8)
    {
        byte_count++;
    }
    return byte_count;
}

void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty)
{
    for (uint8_t i = 0; i < qty; i++)
    {
        *(buf + i) = 0;
    }
}

modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab)
{
    modbus_ret_t status = RET_OK;
    for (modbus_data_qty_t i = 0; i < coil_din_qty; i++)
    {
        if (0 != (*(data_state_ptr + (i / 8)) & (1 << (i % 8))))
        {
            status = set_coil_state(data_tab, (start_adr + i), 1);
        }
        else
        {
            status = set_coil_state(data_tab, (start_adr + i), 0);
        }
        if (status == RET_ERROR)
            break;
    }
    return status;
}