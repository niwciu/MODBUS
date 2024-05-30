/**
 * @file modbus_slave_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "modbus_PDU.h"
#include "buf_rw.h"
#include <stdio.h>



static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);

// static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab);
// static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void **data_tab);
// static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void **data_tab);
// static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty);
// static modbus_ret_t check_write_slave_resp_vs_req(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);




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
// static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab)
// {
//     for (modbus_data_qty_t i = 0; i < reg_qty; i++)
//     {
//         modbus_data_t reg_data = read_u16_from_buf(msg_data_ptr + (i * 2));
//         set_register_value(data_tab, start_adr + i, reg_data);
//     }
// }
// static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req, modbus_fun_code_t fun_code, void **data_tab)
// {

//     modbus_byte_count_t slave_resp_byte_cnt = resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX];
//     modbus_data_qty_t requested_data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_LEN_IDX);
//     modbus_data_qty_t expected_byte_cnt;
//     modbus_reg_t status;
//     expected_byte_cnt = get_expected_byte_cnt(fun_code, requested_data_qty);
//     if (resp->data[MODBUS_FUNCTION_CODE_IDX] == fun_code)
//     {
//         if (slave_resp_byte_cnt == expected_byte_cnt)
//         {
//             if (NULL != data_tab)
//             {
//                 update_master_specific_data_type_from_modbus_msg(resp->data, req->data, fun_code, data_tab);
//                 status = RET_OK;
//             }
//             else
//                 status = RET_ERROR_DATA_TAB_NULL_PTR;
//         }
//         else
//         {
//             status = RET_ERROR_BYTE_CNT;
//         }
//     }
//     else
//     {
//         status = RET_ERROR_FUN_CODE;
//     }
//     return status;
// }
// static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp, const modbus_buf_t *req, modbus_fun_code_t fun_code, void **data_tab)
// {
//     modbus_data_qty_t data_qty = read_u16_from_buf(req+ MODBUS_REQUEST_LEN_IDX);
//     modbus_adr_t data_start_adr = read_u16_from_buf(req + MODBUS_REQUEST_ADR_IDX);
//     switch (fun_code)
//     {
//     case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
//     case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
//         set_inreg_hreg_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_reg_t **)(data_tab));
//         break;
//     default:
//         set_coil_din_value_from_modbus_msg(&resp[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_coil_disin_t **)(data_tab));
        
//         break;
//     }
// }
// static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty)
// {
//     modbus_byte_count_t byte_cnt;
//     switch (func_code)
//     {
//     case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
//     case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
//         byte_cnt = 2 * req_data_qty;
//         break;
//     default:
//         byte_cnt = get_coil_din_byte_count(req_data_qty);
//         break;
//     }
//     return byte_cnt;
// }

// static modbus_ret_t check_write_slave_resp_vs_req(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
// {
//     modbus_ret_t status = RET_OK;

//     if (resp->data[MODBUS_FUNCTION_CODE_IDX] == req->data[MODBUS_FUNCTION_CODE_IDX])
//     {
//         if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_ADR_IDX]) == read_u16_from_buf(&req->data[MODBUS_REQUEST_ADR_IDX]))
//         {
//             status = check_out_val_or_out_qty_correctnes(resp, req);
//         }
//         else
//         {
//             status = RET_ERROR_WRITE_OUT_ADR;
//         }
//     }
//     else
//     {
//         status = RET_ERROR_FUN_CODE;
//     }
//     return status;
// }

// static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
// {
//     modbus_ret_t status = RET_ERROR;
//     switch (resp->data[MODBUS_FUNCTION_CODE_IDX])
//     {
//     case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
//     case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
//         if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]) == read_u16_from_buf(&req->data[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX]))
//             status = RET_OK;
//         else
//             status = RET_ERROR_WRITE_SINGLE_OUT_VAL;
//         break;
//     case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
//     case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
//         if (read_u16_from_buf(&resp->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]) == read_u16_from_buf(&req->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]))
//             status = RET_OK;
//         else
//             status = RET_ERROR_WRITE_MULTI_OUT_QTY;
//         break;
//     default:
//         status = RET_ERROR;
//         break;
//     }
//     return status;
// }


// Slave API functions
modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX);
        modbus_data_qty_t coil_qty = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_LEN_IDX);
        modbus_byte_count_t byte_cnt = get_coil_din_byte_count(coil_qty);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);

        for (modbus_byte_count_t i = 0; i < coil_qty; i++)
        {
            modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_coil_state(Slave_Coils, adr + i) << (i % 8));
        }
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
    }
    return RET_OK;
}

modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t din_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);
        modbus_byte_count_t byte_cnt = get_coil_din_byte_count(din_qty);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        clear_coil_din_status_byte(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX], byte_cnt);

        for (modbus_byte_count_t i = 0; i < din_qty; i++)
        {
            modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_discrete_input_state(Slave_Discrete_Inputs, adr + i) << (i % 8));
        }
        modbus_msg->resp.len =MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);
        modbus_byte_count_t byte_cnt = 2 * reg_qty;

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        for (modbus_byte_count_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_holding_register_value(Slave_Holding_Registers, adr + i));
        }
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);
        modbus_byte_count_t byte_cnt = 2 * reg_qty;

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_INPUT_REGISTERS_FUNC_CODE;
        modbus_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

        for (modbus_byte_count_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_input_register_state(Slave_Input_Registers, adr + i));
        }
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
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
        modbus_msg->resp.len =MODBUS_WRITE_SINGLE_RESP_LEN;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t coils_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);

        set_coil_din_value_from_modbus_msg(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], adr, coils_qty, Slave_Coils);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coils_qty);
        modbus_msg->resp.len =MODBUS_WRITE_MULTI_RESP_LEN;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_reg_t reg_val_to_save = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);

        set_register_value(Slave_Holding_Registers, adr, reg_val_to_save);

        modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val_to_save);
        modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
        return RET_OK;
    }
}

modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg)
{
    if (NULL == modbus_msg)
    {
        return RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t hreg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_LEN_IDX]);

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
