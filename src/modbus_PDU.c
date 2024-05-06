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

static void write_u16_to_buf(uint8_t *buf,uint16_t data);

void modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_qty_t len)
{
    send_buf[0] = READ_HOLDING_REGISTERS;

    write_u16_to_buf(send_buf+1,adr);
    write_u16_to_buf(send_buf+3,len);
}

static void write_u16_to_buf(uint8_t *buf,uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);
    buf[1] = (uint8_t)(data & 0xFF);
}