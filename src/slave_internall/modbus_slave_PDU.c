/**
 * @file modbus_slave_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_slave_PDU.h"
#include "modbus_data.h"
#include "buf_rw.h"
// #include "modbus_type.h"
#include <stdio.h>

static modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg);

static modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg);
static void modbus_slave_unsupoerted_function_error(modbus_msg_t *rx_msg);
    
static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);

void parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg)
{
    switch(rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX])
    {
    case MODBUS_READ_COILS_FUNC_CODE:
        modbus_slave_read_coils(rx_msg);
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        modbus_slave_read_discrete_inputs(rx_msg);
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        modbus_slave_read_holding_reg(rx_msg);
        break;
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        modbus_slave_read_input_reg(rx_msg);
        break;
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
        modbus_slave_write_single_coil(rx_msg);
        break;
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
        modbus_slave_write_single_reg(rx_msg);
        break;
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        modbus_slave_write_multiple_coils(rx_msg);
        break;
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        modbus_slave_write_multiple_reg(rx_msg);
        break;
    default:
        modbus_slave_unsupoerted_function_error(rx_msg);
        break;
    }

}


static modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        status =  RET_NULL_PTR_ERROR; 
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX);
        modbus_data_qty_t coil_qty = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX);
        modbus_byte_count_t byte_cnt = get_coil_din_byte_count(coil_qty);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;

        if((0 == coil_qty )||(  MODBUS_MAX_READ_COILS_QTY < coil_qty))
        {
            modbus_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]= MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR;
            status = RET_ERROR;
        }
        else if((adr > MAIN_APP_COILS_QTY)||((adr+coil_qty)>MAIN_APP_COILS_QTY))
        {
            modbus_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]= MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_ADRES_RANGE_ERROR;
            status = RET_ERROR;
        }
        else
        {
            
            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

            clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);
            for (modbus_data_qty_t i = 0; i < coil_qty; i++)
            {
                modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_coil_state(Slave_Coils, adr + i) << (i % 8));
            }

            modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
            
            status = RET_OK;
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t din_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
        modbus_byte_count_t byte_cnt = get_coil_din_byte_count(din_qty);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
        if((MODBUS_MAX_READ_DISCRETE_INPUTS_QTY < din_qty))
        {
            modbus_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]= MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR;
            status = RET_ERROR;
        }
        else
        {
            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;
            clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);
            for (modbus_data_qty_t i = 0; i < din_qty; i++)
            {
                modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_discrete_input_state(Slave_Discrete_Inputs, adr + i) << (i % 8));
            }
            modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
            status = RET_OK;
        }

    }
    return status;
}

static modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
        modbus_byte_count_t byte_cnt = 2 * reg_qty;

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        for (modbus_data_qty_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_holding_register_value(Slave_Holding_Registers, adr + i));
        }
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
}

static modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
        modbus_byte_count_t byte_cnt = 2 * reg_qty;

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_INPUT_REGISTERS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        for (modbus_data_qty_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_input_register_state(Slave_Input_Registers, adr + i));
        }
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
}

static modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_w_coil_t coils_state = read_u16_from_buf(&modbus_msg->req.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_COIL_FUNC_CODE;

        set_coil_state(Slave_Coils, adr, !!coils_state); // double logic negation make 1 from var that is different then 0 and 0 from var equal 0

        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], coils_state);
        modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
        return RET_OK;
    }
}

static modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t coils_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

        set_coil_din_value_from_modbus_msg(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], adr, coils_qty, Slave_Coils);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coils_qty);
        modbus_msg->resp.len = MODBUS_WRITE_MULTI_RESP_LEN;
        return RET_OK;
    }
}

static modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_reg_t reg_val_to_save = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

        set_register_value(Slave_Holding_Registers, adr, reg_val_to_save);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val_to_save);
        modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
        return RET_OK;
    }
}

static modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg)||(NULL== modbus_msg->req.data) ||(NULL== modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t hreg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

        for (modbus_data_qty_t i = 0; i < hreg_qty; i++)
        {
            set_register_value(Slave_Holding_Registers, adr + i, read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)]));
        }

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], hreg_qty);

        modbus_msg->resp.len = MODBUS_WRITE_MULTI_RESP_LEN;
        return RET_OK;
    }
}

static void modbus_slave_unsupoerted_function_error(modbus_msg_t *rx_msg)
{
    rx_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]=rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
    rx_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]= MODBUS_ERROR_CODE_MASK|MODBUS_FUNCTION_CODE_NOT_SUPPORTED_ERROR;
}

static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty)
{
    modbus_byte_count_t byte_count;

    byte_count = (coil_qty / 8);
    if (coil_qty % 8)
    {
        byte_count++;
    }
    return byte_count;
}

static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty)
{
    for (uint8_t i = 0; i < qty; i++)
    {
        *(buf + i) = 0;
    }
}

static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab)
{
    for (modbus_data_qty_t i = 0; i < coil_din_qty; i++)
    {
        if (0 != (*(data_state_ptr + (i / 8)) & (1 << (i % 8))))
        {
            set_coil_state(data_tab, (start_adr + i), 1);
        }
        else
        {
            set_coil_state(data_tab, (start_adr + i), 0);
        }
    }
}