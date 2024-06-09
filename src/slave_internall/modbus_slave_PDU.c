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
// static void modbus_slave_unsupoerted_function_error(modbus_msg_t *rx_msg);

static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);

static void set_exception_code_resp(modbus_msg_t *modbus_msg, modbus_exception_code_t exception_code);
static modbus_ret_t update_msg_len_and_ret_status(modbus_msg_t *modbus_msg, modbus_ret_t last_ret_val, modbus_byte_count_t byte_cnt);
static modbus_ret_t check_read_data_qty(modbus_msg_t *modbus_msg,modbus_data_qty_t max_read_qty ,modbus_data_qty_t app_data_qty);

void parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg)
{
    switch (rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX])
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
        rx_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
        set_exception_code_resp(rx_msg, MODBUS_ILLEGAL_FUNCTION_ERROR);
        break;
    }
}

static modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;
        status = check_read_data_qty(modbus_msg,MODBUS_MAX_READ_COILS_QTY,MAIN_APP_COILS_QTY);
        if(RET_ERROR != status)
        {
            modbus_adr_t adr = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX);
            modbus_data_qty_t coil_qty = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX);
            modbus_byte_count_t byte_cnt = get_coil_din_byte_count(coil_qty);
            modbus_ret_t read_coil_val = RET_OK;
            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

            clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);
            for (modbus_data_qty_t i = 0; i < coil_qty; i++)
            {
                read_coil_val = get_coil_din_state(Slave_Coils, adr + i);
                if (RET_ERROR == read_coil_val)
                {
                    set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
                    break;
                }
                else
                {
                    modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (read_coil_val << (i % 8));
                }
            }
            status = update_msg_len_and_ret_status(modbus_msg, read_coil_val, byte_cnt);
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
        status = check_read_data_qty(modbus_msg,MODBUS_MAX_READ_DISCRETE_INPUTS_QTY,MAIN_APP_DISCRET_INPUTS_QTY);
        if(RET_OK == status)
        {
            modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
            modbus_data_qty_t din_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
            modbus_byte_count_t byte_cnt = get_coil_din_byte_count(din_qty);
            modbus_ret_t read_discrete_input_val = RET_OK;

            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;
            clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);
            for (modbus_data_qty_t i = 0; i < din_qty; i++)
            {
                read_discrete_input_val = get_coil_din_state(Slave_Discrete_Inputs, adr + i);
                if (RET_ERROR == read_discrete_input_val)
                {
                    set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
                    break;
                }
                else
                {
                    modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (read_discrete_input_val << (i % 8));
                }
            }
            status = update_msg_len_and_ret_status(modbus_msg, read_discrete_input_val, byte_cnt);
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
        status = check_read_data_qty(modbus_msg,MODBUS_MAX_READ_REG_QTY,MAIN_APP_HOLDING_REG_QTY);
        if(RET_OK == status)
        {
            modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
            modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
            modbus_byte_count_t byte_cnt = 2 * reg_qty;
            modbus_ret_t read_hreg_val = RET_OK;
            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

            for (modbus_data_qty_t i = 0; i < reg_qty; i++)
            {
                read_hreg_val = get_register_state(Slave_Holding_Registers, adr + i);
                if (RET_ERROR == read_hreg_val)
                {
                    set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
                    break;
                }
                else
                {
                    write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], read_hreg_val);
                }
            }
            status = update_msg_len_and_ret_status(modbus_msg, read_hreg_val, byte_cnt);
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_INPUT_REGISTERS_FUNC_CODE;
        status = check_read_data_qty(modbus_msg,MODBUS_MAX_READ_REG_QTY,MAIN_APP_INPUT_REG_QTY);
        if(RET_OK == status)
        {
            modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
            modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
            modbus_byte_count_t byte_cnt = 2 * reg_qty;
            modbus_ret_t read_input_reg_val = RET_OK;
            modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;
            for (modbus_data_qty_t i = 0; i < reg_qty; i++)
            {
                read_input_reg_val = get_register_state(Slave_Input_Registers, adr + i);
                if (RET_ERROR == read_input_reg_val)
                {
                    set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
                    break;
                }
                else
                {
                    write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], read_input_reg_val);
                }
            }
            status = update_msg_len_and_ret_status(modbus_msg, read_input_reg_val, byte_cnt);
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_w_coil_t coils_state = read_u16_from_buf(&modbus_msg->req.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_COIL_FUNC_CODE;
        if ((coils_state != COIL_OFF) && (coils_state != COIL_ON))
        {
            set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
            status = RET_ERROR;
        }
        else if (adr >= MAIN_APP_COILS_QTY)
        {
            set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
            status = RET_ERROR;
        }
        else
        {   status = set_coil_state(Slave_Coils, adr, !!coils_state);
            if (RET_OK == status)
            {
                write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
                write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], coils_state);
                modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
            }
            else
            {
                set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
            }
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
        status = check_read_data_qty(modbus_msg,MODBUS_MAX_WRITE_COILS_QTY,MAIN_APP_COILS_QTY);
        if(RET_OK == status)
        {
            modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
            modbus_data_qty_t coils_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

            set_coil_din_value_from_modbus_msg(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], adr, coils_qty, Slave_Coils);
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coils_qty);
            modbus_msg->resp.len = MODBUS_WRITE_MULTI_RESP_LEN;
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {
        status = RET_NULL_PTR_ERROR;
    }
    else
    {
        
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_reg_t reg_val_to_save = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
        if( MAIN_APP_HOLDING_REG_QTY<=adr)
        {
            set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
            status = RET_ERROR;
        }
        else
        {
            status = set_register_value(Slave_Holding_Registers, adr, reg_val_to_save);
            if (RET_OK == status)
            {
                write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
                write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val_to_save);
                modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
            }
            else
            {
                set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
            }

        }
    }
    return status;
}

static modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
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

static void set_exception_code_resp(modbus_msg_t *modbus_msg, modbus_exception_code_t exception_code)
{
    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] |= MODBUS_ERROR_CODE_MASK;
    modbus_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX] = exception_code;
    modbus_msg->resp.len = MODBUS_PDU_EXCEPTION_CODE_LEN;
}

static modbus_ret_t update_msg_len_and_ret_status(modbus_msg_t *modbus_msg, modbus_ret_t last_ret_val, modbus_byte_count_t byte_cnt)
{
    if (RET_ERROR != last_ret_val)
    {
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
    else
        return RET_ERROR;
}
static modbus_ret_t check_read_data_qty(modbus_msg_t *modbus_msg,modbus_data_qty_t max_read_qty ,modbus_data_qty_t app_data_qty)
{
    modbus_adr_t adr = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t data_qty = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX);
    modbus_ret_t status=RET_OK;
    if ((0 == data_qty) || (max_read_qty < data_qty))
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
        status = RET_ERROR;
    }
    else if (((adr + data_qty) > app_data_qty))
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
        status = RET_ERROR;
    }
    else status=RET_OK;
    return status;
}