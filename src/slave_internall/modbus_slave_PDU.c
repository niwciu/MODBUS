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
#include "modbus_PDU.h"
#include "buf_rw.h"
// #include "modbus_type.h"
#include <stdio.h>

static modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_read_coil_service(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_read_discrete_inputs_service(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_read_holding_reg_service(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_read_input_reg_service(modbus_msg_t *modbus_msg);

static modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_write_single_coil_service(modbus_msg_t *modbus_msg, modbus_adr_t coil_adr, modbus_w_coil_t coil_state);
static modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_write_single_reg_service(modbus_msg_t *modbus_msg);
static modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg);
static modbus_ret_t handle_slave_write_multiple_reg_service(modbus_msg_t *modbus_msg);

static void set_exception_code_resp(modbus_msg_t *modbus_msg, modbus_exception_code_t exception_code);
static modbus_ret_t update_msg_len_and_ret_status(modbus_msg_t *modbus_msg, modbus_ret_t last_ret_val, modbus_byte_count_t byte_cnt);
static modbus_ret_t check_read_req_data_correcntess(modbus_msg_t *modbus_msg, modbus_data_qty_t max_read_data_qty, modbus_data_qty_t app_data_qty);
static modbus_ret_t check_write_req_data_correctness(modbus_msg_t *modbus_msg, modbus_data_qty_t max_write_data_qty, modbus_data_qty_t app_data_qty);
static modbus_ret_t check_write_req_byte_count_correctenss(modbus_msg_t *modbus_msg);
static modbus_ret_t check_write_req_reg_byte_count_correctenss(modbus_msg_t *modbus_msg);
static modbus_ret_t check_write_single_coil_req_data_correctness(modbus_msg_t *modbus_msg, modbus_adr_t coil_adr, modbus_w_coil_t coil_state);
static modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);
static const modbus_function_mapper_t modbus_slave_function_mapper[] = {
    {MODBUS_READ_COILS_FUNC_CODE, modbus_slave_read_coils},
    {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, modbus_slave_read_discrete_inputs},
    {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, modbus_slave_read_holding_reg},
    {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, modbus_slave_read_input_reg},
    {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, modbus_slave_write_single_coil},
    {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, modbus_slave_write_single_reg},
    {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, modbus_slave_write_multiple_coils},
    {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, modbus_slave_write_multiple_reg},
};

#define MODBUS_SLAVE_FUNCTION_MAPPER_SIZE (sizeof(modbus_slave_function_mapper) / sizeof(modbus_slave_function_mapper[0]));

modbus_ret_t parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg)
{
    modbus_ret_t msg_parse_status = RET_ERROR;
    if ((NULL == rx_msg) || (NULL == rx_msg->req.data) || (NULL == rx_msg->resp.data))
    {
        msg_parse_status = RET_NULL_PTR_ERROR;
    }
    else
    {
        modbus_fun_code_t req_fun_code = rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
        uint32_t mapper_size = MODBUS_SLAVE_FUNCTION_MAPPER_SIZE;
        bool UNSUPORTED_FUNC_CODE_FLAG = MODBUS_FLAG_SET;
        for (uint32_t i = 0; i < mapper_size; i++)
        {
            if (req_fun_code == modbus_slave_function_mapper[i].fun_code)
            {
                msg_parse_status = modbus_slave_function_mapper[i].fun_code_action(rx_msg);
                UNSUPORTED_FUNC_CODE_FLAG = MODBUS_FLAG_CLEARED;
            }
        }
        if (MODBUS_FLAG_SET == UNSUPORTED_FUNC_CODE_FLAG)
        {
            rx_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
            set_exception_code_resp(rx_msg, MODBUS_ILLEGAL_FUNCTION_ERROR);
        }
    }
    return msg_parse_status;
}

static modbus_ret_t modbus_slave_read_coils(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;
    status = check_read_req_data_correcntess(modbus_msg, MODBUS_MAX_READ_COILS_QTY, MAIN_APP_COILS_QTY);
    if (RET_ERROR != status)
    {
        status = handle_slave_read_coil_service(modbus_msg);
    }

    return status;
}
static modbus_ret_t handle_slave_read_coil_service(modbus_msg_t *modbus_msg)
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
    return update_msg_len_and_ret_status(modbus_msg, read_coil_val, byte_cnt);
}

static modbus_ret_t modbus_slave_read_discrete_inputs(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
    status = check_read_req_data_correcntess(modbus_msg, MODBUS_MAX_READ_DISCRETE_INPUTS_QTY, MAIN_APP_DISCRET_INPUTS_QTY);
    if (RET_OK == status)
    {
        status = handle_slave_read_discrete_inputs_service(modbus_msg);
    }

    return status;
}
static modbus_ret_t handle_slave_read_discrete_inputs_service(modbus_msg_t *modbus_msg)
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
    return update_msg_len_and_ret_status(modbus_msg, read_discrete_input_val, byte_cnt);
}

static modbus_ret_t modbus_slave_read_holding_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
    status = check_read_req_data_correcntess(modbus_msg, MODBUS_MAX_READ_REG_QTY, MAIN_APP_HOLDING_REG_QTY);
    if (RET_OK == status)
    {
        status = handle_slave_read_holding_reg_service(modbus_msg);
    }

    return status;
}

static modbus_ret_t handle_slave_read_holding_reg_service(modbus_msg_t *modbus_msg)
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
    return update_msg_len_and_ret_status(modbus_msg, read_hreg_val, byte_cnt);
}

static modbus_ret_t modbus_slave_read_input_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_INPUT_REGISTERS_FUNC_CODE;
    status = check_read_req_data_correcntess(modbus_msg, MODBUS_MAX_READ_REG_QTY, MAIN_APP_INPUT_REG_QTY);
    if (RET_OK == status)
    {
        status = handle_slave_read_input_reg_service(modbus_msg);
    }

    return status;
}

static modbus_ret_t handle_slave_read_input_reg_service(modbus_msg_t *modbus_msg)
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
    return update_msg_len_and_ret_status(modbus_msg, read_input_reg_val, byte_cnt);
}

static modbus_ret_t modbus_slave_write_single_coil(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    modbus_w_coil_t coil_state = read_u16_from_buf(&modbus_msg->req.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]);
    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_COIL_FUNC_CODE;
    status = check_write_single_coil_req_data_correctness(modbus_msg, adr, coil_state);
    if (status == RET_OK)
    {
        status = handle_slave_write_single_coil_service(modbus_msg, adr, coil_state);
    }

    return status;
}

static modbus_ret_t check_write_single_coil_req_data_correctness(modbus_msg_t *modbus_msg, modbus_adr_t coil_adr, modbus_w_coil_t coil_state)
{
    if ((coil_state != COIL_OFF) && (coil_state != COIL_ON))
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
        return RET_ERROR;
    }
    else if (coil_adr >= MAIN_APP_COILS_QTY)
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
        return RET_ERROR;
    }
    else
        return RET_OK;
}

static modbus_ret_t handle_slave_write_single_coil_service(modbus_msg_t *modbus_msg, modbus_adr_t coil_adr, modbus_w_coil_t coil_state)
{
    modbus_ret_t status = RET_OK;
    status = set_coil_state(Slave_Coils, coil_adr, !!coil_state);
    if (RET_OK == status)
    {
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], coil_state);
        modbus_msg->resp.len = MODBUS_WRITE_SINGLE_RESP_LEN;
    }
    else
    {
        set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
    }
    return status;
}

static modbus_ret_t modbus_slave_write_multiple_coils(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status = RET_OK;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
    status = check_write_req_data_correctness(modbus_msg, MODBUS_MAX_WRITE_COILS_QTY, MAIN_APP_COILS_QTY);
    if (RET_OK == status)
    {
        modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
        modbus_data_qty_t coils_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

        status = set_coil_din_value_from_modbus_msg(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], adr, coils_qty, Slave_Coils);
        if (RET_OK == status)
        {
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coils_qty);
            modbus_msg->resp.len = MODBUS_WRITE_MULTI_RESP_LEN;
        }
        else
        {
            set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
        }
    }
    return status;
}

static modbus_ret_t modbus_slave_write_single_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
    if (MAIN_APP_HOLDING_REG_QTY <= adr)
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
        status = RET_ERROR;
    }
    else
    {
        status = handle_slave_write_single_reg_service(modbus_msg);
    }
    return status;
}

static modbus_ret_t handle_slave_write_single_reg_service(modbus_msg_t *modbus_msg)
{
    modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    modbus_reg_t reg_val_to_save = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
    modbus_ret_t status = set_register_value(Slave_Holding_Registers, adr, reg_val_to_save);
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
    return status;
}

static modbus_ret_t modbus_slave_write_multiple_reg(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status;

    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
    status = check_write_req_data_correctness(modbus_msg, MODBUS_MAX_WRITE_REG_QTY, MAIN_APP_HOLDING_REG_QTY);
    if (RET_OK == status)
    {
        status = handle_slave_write_multiple_reg_service(modbus_msg);
    }
    return status;
}

static modbus_ret_t handle_slave_write_multiple_reg_service(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status = RET_OK;
    modbus_adr_t adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t hreg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

    for (modbus_data_qty_t i = 0; i < hreg_qty; i++)
    {
        status = set_register_value(Slave_Holding_Registers, adr + i, read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)]));
        if (RET_ERROR == status)
            break;
    }
    if (RET_OK == status)
    {
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], adr);
        write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], hreg_qty);
        modbus_msg->resp.len = MODBUS_WRITE_MULTI_RESP_LEN;
    }
    else
    {
        set_exception_code_resp(modbus_msg, MODBUS_SERVER_DEVICE_FAILURE_ERROR);
    }
    return status;
}

static void set_exception_code_resp(modbus_msg_t *modbus_msg, modbus_exception_code_t exception_code)
{
    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] |= MODBUS_EXCEPTION_CODE_MASK;
    modbus_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX] = exception_code;
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
static modbus_ret_t check_read_req_data_correcntess(modbus_msg_t *modbus_msg, modbus_data_qty_t max_read_data_qty, modbus_data_qty_t app_data_qty)
{
    modbus_adr_t adr = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t data_qty = read_u16_from_buf(modbus_msg->req.data + MODBUS_REQUEST_QTY_IDX);
    modbus_ret_t status = RET_OK;
    if ((0 == data_qty) || (max_read_data_qty < data_qty))
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
        status = RET_ERROR;
    }
    else if (((adr + data_qty) > app_data_qty))
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_ADDRESS_ERROR);
        status = RET_ERROR;
    }
    else
        status = RET_OK;
    return status;
}
static modbus_ret_t check_write_req_data_correctness(modbus_msg_t *modbus_msg, modbus_data_qty_t max_write_data_qty, modbus_data_qty_t app_data_qty)
{
    modbus_fun_code_t func_code = modbus_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
    modbus_ret_t status = check_read_req_data_correcntess(modbus_msg, max_write_data_qty, app_data_qty);
    if (RET_OK == status)
    {
        if (MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE == func_code)
        {
            status = check_write_req_byte_count_correctenss(modbus_msg);
        }
        else
        {
            status = check_write_req_reg_byte_count_correctenss(modbus_msg);
        }
    }
    return status;
}

static modbus_ret_t check_write_req_byte_count_correctenss(modbus_msg_t *modbus_msg)
{
    modbus_byte_count_t byte_count = modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];
    modbus_data_qty_t coils_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);

    if (byte_count == get_coil_din_byte_count(coils_qty))
    {
        return RET_OK;
    }
    else
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
        return RET_ERROR;
    }
}
static modbus_ret_t check_write_req_reg_byte_count_correctenss(modbus_msg_t *modbus_msg)
{
    modbus_byte_count_t byte_count = modbus_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];
    modbus_data_qty_t reg_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
    if (byte_count == (reg_qty * 2))
    {
        return RET_OK;
    }
    else
    {
        set_exception_code_resp(modbus_msg, MODBUS_ILLEGAL_DATA_VALUE_ERROR);
        return RET_ERROR;
    }
}

static modbus_ret_t set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab)
{
    modbus_ret_t status = RET_OK;
    for (modbus_data_qty_t i = 0; i < coil_din_qty; i++)
    {
        if (0 != (*(data_state_ptr + (i / 8)) & (1 << (i % 8))))
        {
            status = set_coil_state(data_tab, (start_adr + i), 1);
        }
        else
        {
            status = set_coil_state(data_tab, (start_adr + i), 0);
        }
        if (status == RET_ERROR)
            break;
    }
    return status;
}