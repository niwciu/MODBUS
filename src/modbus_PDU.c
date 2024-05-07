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
static void read_reg_request(uint8_t *send_buf, uint8_t req_code, modbus_adr_t adr, modbus_data_t len);

static void write_u16_to_buf(uint8_t *buf, uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);
    buf[1] = (uint8_t)(data & 0xFF);
}

static void read_reg_request(uint8_t *send_buf, uint8_t req_code, modbus_adr_t adr, modbus_data_t len)
{
    send_buf[0] = req_code;

    write_u16_to_buf(send_buf + 1, adr);
    write_u16_to_buf(send_buf + 3, len);
}

modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    if ((hreg_qty <= MODBUS_MAX_REG_RW_QTY) && (hreg_qty>0))
    {
        read_reg_request(send_buf, READ_HOLDING_REGISTERS, adr, (modbus_data_t)hreg_qty);
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    if (reg_qty <= MODBUS_MAX_REG_RW_QTY)
    {
        read_reg_request(send_buf, READ_INPUT_REGISTERS, adr, (modbus_data_t)reg_qty);
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    if (discrete_input_qty <= MODBUS_MAX_DISCRETE_INPUTS_READ_QTY)
    {
        read_reg_request(send_buf, READ_DISCRETE_INPUTS, adr, (modbus_data_t)discrete_input_qty);
        return RET_OK;
    }
    else
    {
        // ToDo obsługa bledu zbyt duzej ilosci inputsów do odczytu
        return RET_ERROR;
    }
}

modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    if (coils_qty <= MODBUS_MAX_COILS_READ_QTY)
    {
        read_reg_request(send_buf, READ_COILS, adr, (modbus_data_t)coils_qty);
        return RET_OK;
    }
    else
    {
        // ToDo obsługa bledu zbyt duzej ilosci inputsów do odczytu
        return RET_ERROR;
    }
}

void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val)
{
    read_reg_request(send_buf, WRITE_SINGLE_REGISTER, adr, (modbus_data_t)val);
}

void modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_coil_t coil_state)
{
    read_reg_request(send_buf, WRITE_SINGLE_REGISTER, adr, (modbus_data_t)coil_state);
}

modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_reg_t *data_buf)
{
    if (reg_qty > MODBUS_MAX_REG_RW_QTY)
    {
        return RET_ERROR;
    }
    else
    {
        send_buf[0] = WRITE_MULTIPLE_REGISTER;

        write_u16_to_buf(send_buf + 1, adr);
        write_u16_to_buf(send_buf + 3, reg_qty);

        send_buf[5] = reg_qty * 2;

        for (modbus_data_qty_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(send_buf + (6 + (i * 2)), data_buf[i]);
        }
        return RET_OK;
    }
}
