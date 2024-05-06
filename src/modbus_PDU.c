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

static void write_u16_to_buf(uint8_t *buf, uint16_t data);
static void read_reg_request(uint8_t *send_buf, uint8_t req_code, modbus_adr_t adr, modbus_data_qty_t len);

static void write_u16_to_buf(uint8_t *buf, uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);
    buf[1] = (uint8_t)(data & 0xFF);
}

static void read_reg_request(uint8_t *send_buf, uint8_t req_code, modbus_adr_t adr, modbus_data_qty_t len)
{
    send_buf[0] = req_code;

    write_u16_to_buf(send_buf + 1, adr);
    write_u16_to_buf(send_buf + 3, len);
}

void modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len)
{
    read_reg_request(send_buf, READ_HOLDING_REGISTERS, adr, len);
}

void modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len)
{
    read_reg_request(send_buf, READ_INPUT_REGISTERS, adr, len);
}
void modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len)
{
    if (len <= MODBUS_MAX_DISCRETE_INPUTS_READ_QTY)
    {
        read_reg_request(send_buf, READ_DISCRETE_INPUTS, adr, len);
    }
    else
    {
        //ToDo obsługa bledu zbyt duzej ilosci inputsów do odczytu
    }
}

void modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len)
{
    if (len <= MODBUS_MAX_COILS_READ_QTY)
    {
        read_reg_request(send_buf, READ_COILS, adr, len);
    }
    else
    {
        //ToDo obsługa bledu zbyt duzej ilosci inputsów do odczytu
    }
}

void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val)
{
    read_reg_request(send_buf, WRITE_SINGLE_REGISTER, adr, val);
}

void modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty,
                                      modbus_reg_t *data_buf)
{
    send_buf[0] = WRITE_MULTIPLE_REGISTER;

    write_u16_to_buf(send_buf + 1, adr);
    write_u16_to_buf(send_buf + 3, reg_qty);

    send_buf[5] = reg_qty * 2;

    for (modbus_data_qty_t i = 0; i < reg_qty; i++)
    {
        write_u16_to_buf(send_buf + (6 + (i * 2)), data_buf[i]);
    }
}
