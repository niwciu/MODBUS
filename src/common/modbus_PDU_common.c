/**
 * @file modbus_PDU_common.c
 * @brief Implementation file for common Modbus Protocol Data Unit (PDU) operations.
 *
 * This file implements functions declared in modbus_PDU_common.h for handling
 * Modbus Protocol Data Unit (PDU) operations, specifically related to coils and discrete inputs.
 *
 * @author niwciu (niwciu@gmail.com)
 * @version 0.0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 */
#include "modbus_PDU_common.h"

/**
 * @brief Calculates the byte count required for a given quantity of coils or discrete inputs.
 *
 * This function calculates the number of bytes required to represent the specified quantity
 * of coils or discrete inputs. Each coil or input is represented by a single bit.
 *
 * @param coil_qty Number of coils or discrete inputs.
 * @return modbus_byte_count_t - Byte count needed to store the coils or discrete inputs.
 */
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

/**
 * @brief Clears the status byte in modbus msg buffer for coils or discrete inputs.
 *
 * This function clears (sets to 0) the status byte buffer used for storing the status
 * of coils or discrete inputs.
 *
 * @param buf Pointer to the part of msg buffer holding the status bytes.
 * @param qty Number of Coil or Discrete Inputs status bytes to clear.
 */
void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty)
{
    for (uint8_t i = 0; i < qty; i++)
    {
        *(buf + i) = 0;
    }
}
