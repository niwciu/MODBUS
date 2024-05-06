/**
 * @file modbus_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_PDU.h"

void modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_qty_t len)
{
    send_buf[0] = READ_HOLDING_REGISTERS;

    send_buf[1] = (uint8_t)(adr >> 8);
    send_buf[2] = (uint8_t)(adr & 0xFF);
    
    send_buf[3] = (uint8_t)(len >> 8);
    send_buf[4] = (uint8_t)(len & 0xFF);
}