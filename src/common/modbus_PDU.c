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
#include "modbus_PDU.h"

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

