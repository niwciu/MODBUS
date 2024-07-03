/**
 * @file modbus_master_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_master_PDU.h"
#include "modbus_PDU.h"
#include "buf_rw.h"
#include <stdio.h>

static modbus_ret_t modbus_master_read_coils_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_read_input_reg_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_read_holding_reg_resp(modbus_msg_t *modbus_msg);

static modbus_ret_t modbus_master_write_single_coil_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_write_single_reg_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_write_multiple_coils_resp(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_master_write_multiple_reg_resp(modbus_msg_t *modbus_msg);

static modbus_ret_t read_reg_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, modbus_data_qty_t data_len);
static modbus_ret_t write_single_reg_coil_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, void *rw_data_ptr);
static modbus_data_t modbus_get_max_len_2_read(modbus_req_t req_code);
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_data_qty_t reg_qty, modbus_reg_t *rw_data_ptr);
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void *rw_data_ptr);
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void *rw_data_ptr);
static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty);
static modbus_ret_t check_write_slave_resp_vs_req(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_null_ptr_correctness(modbus_msg_t *modbus_msg);
static modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t *rw_data_ptr);
static modbus_ret_t set_master_coil_din_state(modbus_coil_disin_t *coil_din_ptr, modbus_coil_disin_t coil_state);
static modbus_ret_t set_master_register_value(modbus_reg_t *hreg_tab_ptr, modbus_reg_t hreg_val);

    static const modbus_function_mapper_t modbus_master_function_mapper[] = {
        {MODBUS_READ_COILS_FUNC_CODE, modbus_master_read_coils_resp},
        {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, modbus_master_read_discrete_inputs_resp},
        {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, modbus_master_read_holding_reg_resp},
        {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, modbus_master_read_input_reg_resp},
        {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, modbus_master_write_single_coil_resp},
        {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, modbus_master_write_single_reg_resp},
        {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, modbus_master_write_multiple_coils_resp},
        {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, modbus_master_write_multiple_reg_resp},
};

#define MODBUS_MASTER_FUNCTION_MAPPER_SIZE (sizeof(modbus_master_function_mapper) / sizeof(modbus_master_function_mapper[0]));

// Master PDU API functions

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
        return write_single_reg_coil_request(&modbus_msg->req, MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr, modbus_msg->rw_data_ptr);
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
        return write_single_reg_coil_request(&modbus_msg->req, MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr, modbus_msg->rw_data_ptr);
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
                uint16_t data = *((modbus_reg_t *)(modbus_msg->rw_data_ptr) + i);
                write_u16_to_buf(modbus_msg->req.data + (MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)), data);
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
                modbus_coil_disin_t data = *((modbus_coil_disin_t *)(modbus_msg->rw_data_ptr) + i);
                modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i / 8)] |= (data << (i % 8));
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

modbus_ret_t modbus_master_read_slave_resp(modbus_msg_t *modbus_msg)
{
    modbus_ret_t resp_processing_status = RET_ERROR;
    if (RET_OK == check_null_ptr_correctness(modbus_msg))
    {
        uint32_t mapper_size = MODBUS_MASTER_FUNCTION_MAPPER_SIZE;
        modbus_fun_code_t req_fun_code = modbus_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
        for (uint32_t i=0; i<mapper_size; i++)
        {
            if (modbus_master_function_mapper[i].fun_code == req_fun_code)
            {
                resp_processing_status= modbus_master_function_mapper[i].fun_code_action(modbus_msg);
                break;
            }
        }
    }
    else
    {
        resp_processing_status = RET_NULL_PTR_ERROR;
    }
    return resp_processing_status;
}
// Master internall functions
static modbus_ret_t modbus_master_read_coils_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_COILS_FUNC_CODE, modbus_msg->rw_data_ptr);
}

static modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, modbus_msg->rw_data_ptr);
}

static modbus_ret_t modbus_master_read_input_reg_resp(modbus_msg_t *modbus_msg)
{

    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, modbus_msg->rw_data_ptr);
}

static modbus_ret_t modbus_master_read_holding_reg_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, modbus_msg->rw_data_ptr);
}

static modbus_ret_t modbus_master_write_single_coil_resp(modbus_msg_t *modbus_msg)
{
    return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_write_single_reg_resp(modbus_msg_t *modbus_msg)
{
    return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_write_multiple_coils_resp(modbus_msg_t *modbus_msg)
{
    return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
}
static modbus_ret_t modbus_master_write_multiple_reg_resp(modbus_msg_t *modbus_msg)
{
    return check_write_slave_resp_vs_req(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t read_reg_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, modbus_data_qty_t data_len)
{
    if ((data_len <= modbus_get_max_len_2_read(req_code)) && (data_len >= MODBUS_MIN_REG_COIL_QTY))
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

static modbus_ret_t write_single_reg_coil_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, void *rw_data_ptr)
{
    req->data[MODBUS_FUNCTION_CODE_IDX] = req_code;
    write_u16_to_buf(req->data + MODBUS_REQUEST_ADR_IDX, adr);
    if (MODBUS_WRITE_SINGLE_COIL_FUNC_CODE == req_code)
    {
        write_u16_to_buf(req->data + MODBUS_REQUEST_QTY_IDX, *((modbus_coil_disin_t*)(rw_data_ptr)) * COIL_ON);
    }
    else
    {
        write_u16_to_buf(req->data + MODBUS_REQUEST_QTY_IDX, *((modbus_reg_t*)(rw_data_ptr)));
    }
    req->len = MODBUS_WRITE_SINGLE_REQUEST_LEN;
    return RET_OK;
}

static modbus_data_t modbus_get_max_len_2_read(modbus_req_t req_code)
{
    modbus_data_t max_len = 0;
    if (MODBUS_READ_COILS_FUNC_CODE == req_code)
    {
        max_len = MODBUS_MAX_READ_COILS_QTY;
    }
    else if (MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE == req_code)
    {
        max_len = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;
    }
    else
    {
        // MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE & MODBUS_READ_INPUT_REGISTERS_FUNC_CODE
        max_len = MODBUS_MAX_READ_REG_QTY;
    }
    return max_len;
}
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_data_qty_t reg_qty, modbus_reg_t *rw_data_ptr)
{
    for (modbus_data_qty_t i = 0; i < reg_qty; i++)
    {
        modbus_data_t reg_data = read_u16_from_buf(msg_data_ptr + (i * 2));
        set_master_register_value(rw_data_ptr+i, reg_data);
    }
}
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void *rw_data_ptr)
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
            if (NULL != rw_data_ptr)
            {
                update_master_specific_data_type_from_modbus_msg(resp->data, req->data, fun_code, rw_data_ptr);
            }
            status = RET_OK;
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
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void *rw_data_ptr)
{
    modbus_data_qty_t data_qty = read_u16_from_buf(req + MODBUS_REQUEST_QTY_IDX);
    // modbus_adr_t data_start_adr = read_u16_from_buf(req + MODBUS_REQUEST_ADR_IDX);
    switch (fun_code)
    {
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        set_inreg_hreg_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_qty, (modbus_reg_t *)(rw_data_ptr));
        break;
    default:
        set_coil_din_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_qty, (modbus_coil_disin_t*)(rw_data_ptr));
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
    modbus_fun_code_t resp_fun_code = resp->data[MODBUS_FUNCTION_CODE_IDX];
    if ((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE == resp_fun_code) || (MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE == resp_fun_code))
    {
        if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]) == read_u16_from_buf(&req->data[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX]))
            status = RET_OK;
        else
            status = RET_ERROR_WRITE_SINGLE_OUT_VAL;
    }
    else // MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE or MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE
    {
        if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]) == read_u16_from_buf(&req->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]))
            status = RET_OK;
        else
            status = RET_ERROR_WRITE_MULTI_OUT_QTY;
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

static modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr , modbus_data_qty_t coil_din_qty, modbus_coil_disin_t *rw_data_ptr)
{
    modbus_ret_t status = RET_OK;
    for (modbus_data_qty_t i = 0; i < coil_din_qty; i++)
    {
        if (0 != (*(data_state_ptr + (i / 8)) & (1 << (i % 8))))
        {
            status = set_master_coil_din_state( rw_data_ptr+i, 1);
        }
        else
        {
            status = set_master_coil_din_state(rw_data_ptr + i, 0);
        }
    }
    return status;
}
static modbus_ret_t set_master_coil_din_state(modbus_coil_disin_t *coil_din_ptr, modbus_coil_disin_t coil_state)
{
    if (NULL != coil_din_ptr)
    {
        *coil_din_ptr = coil_state;
        return RET_OK;
    }
    else
        return RET_ERROR;
}

static modbus_ret_t set_master_register_value(modbus_reg_t *hreg_tab_ptr, modbus_reg_t hreg_val)
{
    if (NULL != hreg_tab_ptr)
    {
        *hreg_tab_ptr = hreg_val;
        return RET_OK;
    }
    else
        return RET_ERROR;
}