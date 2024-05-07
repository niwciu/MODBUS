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
#include <stdio.h>

static void write_u16_to_buf(uint8_t *buf, uint16_t data);
static modbus_ret_t read_reg_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len);
static void write_single_reg_coil_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t data);
static modbus_data_t modbus_get_max_len(modbus_req_t req_code);

static void write_u16_to_buf(uint8_t *buf, uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);
    buf[1] = (uint8_t)(data & 0xFF);
}

static modbus_ret_t read_reg_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len)
{
    if ((len <= modbus_get_max_len(req_code)) && (len >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[0] = req_code;
        write_u16_to_buf(send_buf + 1, adr);
        write_u16_to_buf(send_buf + 3, len);
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

static void write_single_reg_coil_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_reg_t data)
{
    send_buf[0] = req_code;
    write_u16_to_buf(send_buf + 1, adr);
    write_u16_to_buf(send_buf + 3, data);
}

static modbus_data_t modbus_get_max_len(modbus_req_t req_code)
{
    modbus_data_t max_len = 0;
    switch (req_code)
    {
    case READ_COILS:
    case WRITE_MULTIPLE_COILS:
        max_len = MODBUS_MAX_COILS_READ_QTY;
        break;
    case READ_DISCRETE_INPUTS:
        max_len = MODBUS_MAX_DISCRETE_INPUTS_READ_QTY;
        break;
    case READ_HOLDING_REGISTERS:
    case READ_INPUT_REGISTERS:
    case WRITE_MULTIPLE_REGISTER:
        max_len = MODBUS_MAX_REG_RW_QTY;
        break;
    case WRITE_SINGLE_COIL:
    case WRITE_SINGLE_REGISTER:
    default:
        max_len = MODBUS_MIN_REG_COIL_QTY;
        break;
    }
    return max_len;
}

modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    return read_reg_request(send_buf, READ_HOLDING_REGISTERS, adr, hreg_qty);
}

modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return read_reg_request(send_buf, READ_INPUT_REGISTERS, adr, reg_qty);
}

modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    return read_reg_request(send_buf, READ_DISCRETE_INPUTS, adr, discrete_input_qty);
}

modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return read_reg_request(send_buf, READ_COILS, adr, coils_qty);
}


void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val)
{
    write_single_reg_coil_request(send_buf,WRITE_SINGLE_REGISTER,adr, val);
}

void modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_coil_t coil_state)
{
    write_single_reg_coil_request(send_buf,WRITE_SINGLE_COIL,adr, coil_state);
}

modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, const modbus_reg_t *data_buf)
{
    if ((reg_qty <= MODBUS_MAX_REG_RW_QTY)&&(reg_qty>=MODBUS_MIN_REG_COIL_QTY))
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
    else
    {
        return RET_ERROR;
    }
}
