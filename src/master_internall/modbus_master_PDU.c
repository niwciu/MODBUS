/**
 * @file modbus_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_master_PDU.h"
#include "buf_rw.h"
#include <stdio.h>

static modbus_ret_t read_reg_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, modbus_data_qty_t data_len);
static modbus_ret_t write_single_reg_coil_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr);
static modbus_data_t modbus_get_max_len(modbus_req_t req_code);
static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab);
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void **data_tab);
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void **data_tab);
static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty);
static modbus_ret_t check_write_slave_resp_vs_req(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_null_ptr_correctness(modbus_msg_t *modbus_msg);

// Master API functions

modbus_ret_t modbus_master_read_holding_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return read_reg_request(&modbus_msg->req, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_input_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return read_reg_request(&modbus_msg->req, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{  
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return read_reg_request(&modbus_msg->req, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_coils_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return read_reg_request(&modbus_msg->req, MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_single_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return write_single_reg_coil_request(&modbus_msg->req, MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_single_coil_req(modbus_msg_t *modbus_msg, modbus_adr_t adr)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return write_single_reg_coil_request(&modbus_msg->req, MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_multiple_reg_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        if ((reg_qty <= MODBUS_MAX_WRITE_REG_QTY) && (reg_qty >= MODBUS_MIN_REG_COIL_QTY))
        {
            modbus_msg->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
            write_u16_to_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX, adr);
            write_u16_to_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX, reg_qty);
            modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX] = reg_qty * 2;

            for (modbus_data_qty_t i = 0; i < reg_qty; i++)
            {
                write_u16_to_buf(modbus_msg->req.data + (MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)), get_register_state(Master_Holding_Registers, adr + i));
            }
            modbus_msg->req.len = MODBUS_WRITE_MULTI_REQUEST_LEN + modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];
            return RET_OK;
        }
        else
        {
            return RET_ERROR;
        }
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_multiple_coils_req(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {   
        modbus_byte_count_t byte_count = get_coil_din_byte_count(coils_qty);
        if ((coils_qty <= MODBUS_MAX_WRITE_COILS_QTY) && (coils_qty >= MODBUS_MIN_REG_COIL_QTY))
        {
            modbus_msg->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
            write_u16_to_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX, adr);
            write_u16_to_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX, coils_qty);
            modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX] = byte_count;

            clear_coil_din_status_byte(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], byte_count);
            for (modbus_data_qty_t i = 0; i < coils_qty; i++)
            {

                modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i / 8)] |= (get_coil_din_state(Master_Coils, adr + i) << (i % 8));
            }
            modbus_msg->req.len = MODBUS_WRITE_MULTI_REQUEST_LEN + modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];
            return RET_OK;
        }
        else
        {
            return RET_ERROR;
        }
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_coils_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_COILS_FUNC_CODE, (void **)(Master_Coils));
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, (void **)(Master_Discrete_Inputs));
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_input_reg_resp(modbus_msg_t *modbus_msg)
{

    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, (void **)(Master_Input_Registers));
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_holding_reg_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, (void **)(Master_Holding_Registers));
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_single_coil_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_single_reg_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_multiple_coils_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}
modbus_ret_t modbus_master_write_multiple_reg_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

// Master internall functions
static modbus_ret_t read_reg_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, modbus_data_qty_t data_len)
{
    if ((data_len <= modbus_get_max_len(req_code)) && (data_len >= MODBUS_MIN_REG_COIL_QTY))
    {
        req->data[MODBUS_FUNCTION_CODE_IDX] = req_code;
        write_u16_to_buf(req->data + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(req->data + MODBUS_REQUEST_QTY_IDX, data_len);
        req->len = MODBUS_READ_REQUEST_LEN;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

static modbus_ret_t write_single_reg_coil_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr)
{
    if (NULL != req)
    {
        req->data[MODBUS_FUNCTION_CODE_IDX] = req_code;
        write_u16_to_buf(req->data + MODBUS_REQUEST_ADR_IDX, adr);
        if (MODBUS_WRITE_SINGLE_COIL_FUNC_CODE == req_code)
        {
            write_u16_to_buf(req->data + MODBUS_REQUEST_QTY_IDX, (get_coil_din_state(Master_Coils, adr) * COIL_ON));
        }
        else
        {
            write_u16_to_buf(req->data + MODBUS_REQUEST_QTY_IDX, get_register_state(Master_Holding_Registers, adr));
        }
        req->len = MODBUS_WRITE_SINGLE_REQUEST_LEN;
        return RET_OK;
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

static modbus_data_t modbus_get_max_len(modbus_req_t req_code)
{
    modbus_data_t max_len = 0;
    switch (req_code)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_COILS_QTY;
        break;
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        max_len = MODBUS_MAX_WRITE_COILS_QTY;
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_REG_QTY;
        break;
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        max_len = MODBUS_MAX_WRITE_REG_QTY;
        break;
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
    default:
        max_len = MODBUS_MIN_REG_COIL_QTY;
        break;
    }
    return max_len;
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
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab)
{
    for (modbus_data_qty_t i = 0; i < reg_qty; i++)
    {
        modbus_data_t reg_data = read_u16_from_buf(msg_data_ptr + (i * 2));
        set_register_value(data_tab, start_adr + i, reg_data);
    }
}
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void **data_tab)
{

    modbus_byte_count_t slave_resp_byte_cnt = resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX];
    modbus_data_qty_t requested_data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_QTY_IDX);
    modbus_data_qty_t expected_byte_cnt;
    modbus_reg_t status;
    expected_byte_cnt = get_expected_byte_cnt(fun_code, requested_data_qty);
    if (resp->data[MODBUS_FUNCTION_CODE_IDX] == fun_code)
    {
        if (slave_resp_byte_cnt == expected_byte_cnt)
        {
            if (NULL != data_tab)
            {
                update_master_specific_data_type_from_modbus_msg(resp->data, req->data, fun_code, data_tab);
                status = RET_OK;
            }
            else
                status = RET_ERROR_DATA_TAB_NULL_PTR;
        }
        else
        {
            status = RET_ERROR_BYTE_CNT;
        }
    }
    else
    {
        status = RET_ERROR_FUN_CODE;
    }
    return status;
}
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void **data_tab)
{
    modbus_data_qty_t data_qty = read_u16_from_buf(req + MODBUS_REQUEST_QTY_IDX);
    modbus_adr_t data_start_adr = read_u16_from_buf(req + MODBUS_REQUEST_ADR_IDX);
    switch (fun_code)
    {
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        set_inreg_hreg_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_reg_t **)(data_tab));
        break;
    default:
        set_coil_din_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_coil_disin_t **)(data_tab));

        break;
    }
}
static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty)
{
    modbus_byte_count_t byte_cnt;
    switch (func_code)
    {
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        byte_cnt = 2 * req_data_qty;
        break;
    default:
        byte_cnt = get_coil_din_byte_count(req_data_qty);
        break;
    }
    return byte_cnt;
}

static modbus_ret_t check_write_slave_resp_vs_req(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
{
    modbus_ret_t status = RET_OK;

    if (resp->data[MODBUS_FUNCTION_CODE_IDX] == req->data[MODBUS_FUNCTION_CODE_IDX])
    {
        if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_ADR_IDX]) == read_u16_from_buf(&req->data[MODBUS_REQUEST_ADR_IDX]))
        {
            status = check_out_val_or_out_qty_correctnes(resp, req);
        }
        else
        {
            status = RET_ERROR_WRITE_OUT_ADR;
        }
    }
    else
    {
        status = RET_ERROR_FUN_CODE;
    }
    return status;
}

static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
{
    modbus_ret_t status = RET_ERROR;
    switch (resp->data[MODBUS_FUNCTION_CODE_IDX])
    {
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
        if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]) == read_u16_from_buf(&req->data[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX]))
            status = RET_OK;
        else
            status = RET_ERROR_WRITE_SINGLE_OUT_VAL;
        break;
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]) == read_u16_from_buf(&req->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]))
            status = RET_OK;
        else
            status = RET_ERROR_WRITE_MULTI_OUT_QTY;
        break;
    default:
        status = RET_ERROR;
        break;
    }
    return status;
}

static modbus_ret_t check_null_ptr_correctness(modbus_msg_t *modbus_msg)
{
    if ((NULL == modbus_msg) || (NULL == modbus_msg->req.data) || (NULL == modbus_msg->resp.data))
    {

        return RET_NULL_PTR_ERROR;
    }
    else
    {
        return RET_OK;
    }
}
