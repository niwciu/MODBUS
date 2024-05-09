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
#include "buf_rw.h"
// #include "modbus_data.h"  // dodane w hederze modsbus_PDU.h
#include <stdio.h>

static modbus_ret_t read_reg_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len);
static void write_single_reg_coil_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t data);
static modbus_data_t modbus_get_max_len(modbus_req_t req_code);
static modbus_byte_count_t get_coil_read_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(uint8_t *buf, modbus_data_qty_t qty);

static modbus_ret_t read_reg_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len)
{
    if ((len <= modbus_get_max_len(req_code)) && (len >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[MODBUS_FUNCTION_CODE_IDX] = req_code;
        write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, len);
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

static void write_single_reg_coil_request(uint8_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_reg_t data)
{
    send_buf[MODBUS_FUNCTION_CODE_IDX] = req_code;
    write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
    write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, data);
}

static modbus_data_t modbus_get_max_len(modbus_req_t req_code)
{
    modbus_data_t max_len = 0;
    switch (req_code)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        max_len = MODBUS_MAX_COILS_QTY;
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        max_len = MODBUS_MAX_DISCRETE_INPUTS_QTY;
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        max_len = MODBUS_MAX_REG_QTY;
        break;
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
    default:
        max_len = MODBUS_MIN_REG_COIL_QTY;
        break;
    }
    return max_len;
}

static modbus_byte_count_t get_coil_read_byte_count(modbus_data_qty_t coil_qty)
{
    modbus_byte_count_t byte_count;

    byte_count = (coil_qty / 8);
    if (coil_qty % 8)
    {
        byte_count++;
    }
    return byte_count;
}

static void clear_coil_din_status_byte(uint8_t *buf, modbus_data_qty_t qty)
{
    for (uint8_t i = 0; i < qty; i++)
    {
        *(buf + i) = 0;
    }
}

// Master API functions
modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty);
}

modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty);
}

modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty);
}

modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty);
}

void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val)
{
    write_single_reg_coil_request(send_buf, MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr, val);
}

void modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_w_coil_t coil_state)
{
    write_single_reg_coil_request(send_buf, MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr, coil_state);
}

modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, const modbus_reg_t *data_buf)
{
    if ((reg_qty <= MODBUS_MAX_REG_QTY) && (reg_qty >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, reg_qty);
        send_buf[MODBUS_REQUEST_BYTE_CNT_IDX] = reg_qty * 2;

        for (modbus_data_qty_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(send_buf + (MODBUS_WRITE_MULTI_DATA_IDX + (i * 2)), data_buf[i]);
        }
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

// Slave API functions

void register_app_data_to_coils_table(modbus_adr_t coil_adr, modbus_coil_t *app_data_ptr)
{
    Coils[coil_adr]=app_data_ptr;
}
void register_app_data_to_discrete_inputs_table (modbus_adr_t din_adr, modbus_coil_t *app_data_ptr)
{
    Discrete_Inputs[din_adr]=app_data_ptr;
}
void register_app_data_to_input_registers_table (modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr)
{
    Input_Registers[input_reg_adr]=app_data_ptr;
}
void register_app_data_to_holding_registers_table (modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr)
{
    Holding_Registers[hreg_reg_adr]=app_data_ptr;
}

void modbus_slave_read_coils(uint8_t *resp_buf, const uint8_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(req_buf + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t coil_qty = read_u16_from_buf(req_buf + MODBUS_REQUEST_LEN_IDX);
    modbus_byte_count_t byte_cnt = get_coil_read_byte_count(coil_qty);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;
    resp_buf[MODBUS_RESP_BYTE_CNT_IDX] = byte_cnt;

    clear_coil_din_status_byte(&resp_buf[MODBUS_RESP_DATA_IDX], byte_cnt);

    for (modbus_byte_count_t i = 0; i < coil_qty; i++)
    {
        resp_buf[MODBUS_RESP_DATA_IDX + i / 8] |= (get_coil_state(adr + i) << (i % 8));
    }
}

void modbus_slave_read_discrete_inputs(uint8_t *resp_buf, const uint8_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t din_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);
    modbus_byte_count_t byte_cnt = get_coil_read_byte_count(din_qty);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
    resp_buf[MODBUS_RESP_BYTE_CNT_IDX] = byte_cnt;

    clear_coil_din_status_byte(&resp_buf[MODBUS_RESP_DATA_IDX], byte_cnt);

    for (modbus_byte_count_t i = 0; i < din_qty; i++)
    {
        resp_buf[MODBUS_RESP_DATA_IDX + (i / 8)] |= (get_discrete_input_state(adr + i) << (i % 8));
    }
}



// void modbus_slave_read_holdin_reg(uint8_t *resp_buf, const uint8_t *req_buf)
// {
//     modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
//     modbus_data_qty_t reg_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);
//     modbus_byte_count_t byte_cnt = 2*reg_qty;

//     resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
//     resp_buf[MODBUS_RESP_BYTE_CNT_IDX] = byte_cnt;

//     // for (modbus_byte_count_t i = 0; i < reg_qty; i++)
//     // {
//     //     temp_reg_value = get_reg_state(adr + i);
//     //     if (temp_din_value != READ_DIS_IN_ERROR) // np nullpointer pod adresem
//     //     {
//     //         resp_buf[MODBUS_RESP_DATA_IDX + (i / 8)] |= (temp_din_value << (i % 8));
//     //     }
//     //     else
//     //     {
//     //         // gen_error_resp(4);
//     //         break;
//     //     }
//     // }
// }
