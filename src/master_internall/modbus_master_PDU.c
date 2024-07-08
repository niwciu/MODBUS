/**
 * @file modbus_master_PDU.c
 * @brief Implementation file for Modbus Master Protocol Data Unit (PDU) handling.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-06-28
 * @copyright Copyright (c) 2024
 */

#include "modbus_master_PDU.h"
#include "modbus_PDU_common.h"
#include "modbus_master_data_interface.h"
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
// static modbus_ret_t write_single_reg_coil_request(modbus_req_resp_t *req, modbus_req_t req_code, modbus_adr_t adr, void *rw_data_ptr);
static modbus_data_t modbus_get_max_len_2_read(modbus_req_t req_code);

static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);

static modbus_ret_t report_write_operation_confirmation(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
// static modbus_ret_t check_out_val_or_out_qty_correctnes(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_null_ptr_correctness(modbus_msg_t *modbus_msg);

static bool modbus_response_contain_exception_code(const modbus_msg_t *modbus_msg);
static modbus_ret_t process_modbus_exception_code(modbus_msg_t *modbus_msg);
static modbus_ret_t process_modbus_response(modbus_msg_t *modbus_msg);

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

modbus_ret_t modbus_master_read_holding_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        return read_reg_request(&msg_buf->req, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, req_param->adr, req_param->obj_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_input_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        return read_reg_request(&msg_buf->req, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, req_param->adr, req_param->obj_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        return read_reg_request(&msg_buf->req, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, req_param->adr, req_param->obj_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

/**
 * @brief Initiates a Modbus master request to read coils.
 *
 * This function prepares a Modbus master request to read coils at a specified address
 * with a specified quantity of coils.
 *
 * @param[in,out] modbus_msg Pointer to the Modbus message structure to be populated with the request data.
 * @param[in] adr Starting address of the coils to read.
 * @param[in] coils_qty Quantity of coils to read.
 * @return - RET_OK if the request is successfully prepared.
 * @return - RET_NULL_PTR_ERROR if the provided @p modbus_msg pointer is null.
 * @return - RET_ERROR if the quantity of coils requested exceeds the maximum allowed.
 */
modbus_ret_t modbus_master_read_coils_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        return read_reg_request(&msg_buf->req, MODBUS_READ_COILS_FUNC_CODE, req_param->adr, req_param->obj_qty);
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

modbus_ret_t modbus_master_write_single_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        // return write_single_reg_coil_request(&msg_buf->req, MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, req_param->adr, req_param->regs_2_write);
        msg_buf->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_ADR_IDX, req_param->adr);
        write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_QTY_IDX, req_param->reg_2_write);
        msg_buf->req.len = MODBUS_WRITE_SINGLE_REQUEST_LEN;
        return RET_OK;
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

/**
 * @brief Sends a Modbus request to write a single coil in a slave device.
 *
 * This function constructs a Modbus request message to write a single coil in a slave device
 * based on the provided address.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure where the request will be stored.
 * @param[in] adr Address of the coil to write.
 * @return - RET_OK if the request is constructed successfully.
 * @return - RET_NULL_PTR_ERROR if the provided @p modbus_msg pointer is null.
 * @return - RET_ERROR if there is an error in constructing the request.
 */
modbus_ret_t modbus_master_write_single_coil_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK == check_null_ptr_correctness(msg_buf))
    {
        // return write_single_reg_coil_request(&msg_buf->req, MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, req_param->adr, &req_param->coils_2_write);
        msg_buf->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_COIL_FUNC_CODE;
        write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_ADR_IDX, req_param->adr);
        write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_QTY_IDX, req_param->coil_2_write * COIL_ON);
        msg_buf->req.len = MODBUS_WRITE_SINGLE_REQUEST_LEN;
        return RET_OK;
    }
    else
    {
        return RET_NULL_PTR_ERROR;
    }
}

/**
 * @brief Sends a Modbus request to write multiple holding registers in a slave device.
 *
 * This function constructs a Modbus request message to write multiple holding registers in a slave device
 * based on the provided starting address and quantity of registers to write.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure where the request will be stored.
 * @param[in] adr Starting address of the holding registers to write.
 * @param[in] reg_qty Number of holding registers to write.
 * @return - RET_OK if the request is constructed successfully.
 * @return - RET_NULL_PTR_ERROR if the provided @p modbus_msg pointer is null.
 * @return - RET_ERROR if the provided register quantity is out of range or there is an error in constructing the request.
 */
modbus_ret_t modbus_master_write_multiple_reg_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK != check_null_ptr_correctness(msg_buf))
    {
        return RET_NULL_PTR_ERROR;
    }

    if (req_param->obj_qty > MODBUS_MAX_WRITE_REG_QTY || req_param->obj_qty < MODBUS_MIN_REG_COIL_QTY)
    {
        return RET_ERROR;
    }

    // Construct the Modbus request
    msg_buf->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
    write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_ADR_IDX, req_param->adr);
    write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_QTY_IDX, req_param->obj_qty);
    msg_buf->req.data[MODBUS_REQUEST_BYTE_CNT_IDX] = req_param->obj_qty * 2;

    // Copy register data to the request buffer
    for (modbus_data_qty_t i = 0; i < req_param->obj_qty; i++)
    {
        uint16_t data = *(req_param->regs_2_write + i);
        write_u16_to_buf(msg_buf->req.data + (MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)), data);
    }

    // Calculate request length
    msg_buf->req.len = MODBUS_WRITE_MULTI_REQUEST_LEN + msg_buf->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];

    return RET_OK;
}

/**
 * @brief Constructs a Modbus request to write multiple coils in a slave device.
 *
 * Constructs a Modbus request message to write multiple coils starting from
 * the specified address in a slave device.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure where the request will be stored.
 * @param[in] adr Starting address of the coils to write.
 * @param[in] coils_qty Number of coils to write.
 * @return RET_OK if the request is successfully constructed.
 * @return RET_NULL_PTR_ERROR if modbus_msg is NULL.
 * @return RET_ERROR if the coil quantity is out of range or there is an error in constructing the request.
 */
modbus_ret_t modbus_master_write_multiple_coils_req(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    if (RET_OK != check_null_ptr_correctness(msg_buf))
    {
        return RET_NULL_PTR_ERROR;
    }

    if (req_param->obj_qty > MODBUS_MAX_WRITE_COILS_QTY || req_param->obj_qty < MODBUS_MIN_REG_COIL_QTY)
    {
        return RET_ERROR;
    }

    modbus_byte_count_t byte_count = get_coil_din_byte_count(req_param->obj_qty);

    msg_buf->req.data[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
    write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_ADR_IDX, req_param->adr);
    write_u16_to_buf(msg_buf->req.data + MODBUS_REQUEST_QTY_IDX, req_param->obj_qty);
    msg_buf->req.data[MODBUS_REQUEST_BYTE_CNT_IDX] = byte_count;

    clear_coil_din_status_byte(&msg_buf->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], byte_count);

    for (modbus_data_qty_t i = 0; i < req_param->obj_qty; i++)
    {
        modbus_coil_disin_t data = *(req_param->coils_2_write + i);
        msg_buf->req.data[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i / 8)] |= (data << (i % 8));
    }
    msg_buf->req.len = MODBUS_WRITE_MULTI_REQUEST_LEN + msg_buf->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];

    return RET_OK;
}

/**
 * @brief Processes a Modbus response received from a slave device.
 *
 * This function checks for null pointer correctness, verifies if the response
 * contains an error exception code, and processes the Modbus response if
 * no errors are found.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure containing the response.
 * @return - RET_OK if the response is successfully processed.
 * @return - RET_NULL_PTR_ERROR if modbus_msg is NULL.
 */
modbus_ret_t modbus_master_read_slave_resp(modbus_msg_t *modbus_msg)
{
    if (RET_OK != check_null_ptr_correctness(modbus_msg))
    {
        return RET_NULL_PTR_ERROR;
    }

    if (modbus_response_contain_exception_code(modbus_msg))
    {

        return process_modbus_exception_code(modbus_msg);
    }

    return process_modbus_response(modbus_msg);
}

// Master internall functions
static modbus_ret_t modbus_master_read_coils_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_read_input_reg_resp(modbus_msg_t *modbus_msg)
{

    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_read_holding_reg_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_write_single_coil_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_write_single_reg_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}

static modbus_ret_t modbus_master_write_multiple_coils_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}
static modbus_ret_t modbus_master_write_multiple_reg_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
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
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
{
    modbus_device_ID_t slave_adr = resp->data[MODBUS_SLAVE_ADR_IDX];
    modbus_fun_code_t resp_fun_code = resp->data[MODBUS_FUNCTION_CODE_IDX];
    modbus_adr_t data_adr = read_u16_from_buf(req->data + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_QTY_IDX);
    switch (resp_fun_code)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
        for (modbus_data_qty_t i = 0; i < data_qty; i++)
        {
            if (0 != ((resp->data[MODBUS_RESP_READ_DATA_IDX + (i / 8)]) & (1 << (i % 8))))
            {
                modbus_master_coil_read(slave_adr, data_adr + i, !!COIL_ON);
            }
            else
            {
                modbus_master_coil_read(slave_adr, data_adr + i, !!COIL_OFF);
            }
        }
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:

        for (modbus_data_qty_t i = 0; i < data_qty; i++)
        {
            if (0 != ((resp->data[MODBUS_RESP_READ_DATA_IDX + (i / 8)]) & (1 << (i % 8))))
            {
                modbus_master_disin_read(slave_adr, data_adr + i, !!COIL_ON);
            }
            else
            {
                modbus_master_disin_read(slave_adr, data_adr + i, !!COIL_OFF);
            }
        }
        break;

    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        for (modbus_data_qty_t i = 0; i < data_qty; i++)
        {
            modbus_data_t reg_data = read_u16_from_buf(&(resp->data[MODBUS_RESP_READ_DATA_IDX + (i * 2)]));
            modbus_master_inreg_read(slave_adr, data_adr + i, reg_data);
        }
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        for (modbus_data_qty_t i = 0; i < data_qty; i++)
        {
            modbus_data_t reg_data = read_u16_from_buf(&(resp->data[MODBUS_RESP_READ_DATA_IDX + (i * 2)]));
            modbus_master_hreg_read(slave_adr, data_adr + i, reg_data);
        }
        break;
    default:
        break;
    }
    return RET_OK;
}

static modbus_ret_t report_write_operation_confirmation(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
{
    modbus_device_ID_t slave_adr = resp->data[MODBUS_SLAVE_ADR_IDX];
    modbus_adr_t data_adr = read_u16_from_buf(req->data + MODBUS_REQUEST_ADR_IDX);
    modbus_fun_code_t resp_fun_code = resp->data[MODBUS_FUNCTION_CODE_IDX];
    if ((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE == resp_fun_code))
    {
        modbus_master_coil_write(slave_adr, data_adr, 1);
    }
    else if (MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE == resp_fun_code)
    {
        modbus_master_hreg_write(slave_adr, data_adr, 1);
    }
    else if (MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE == resp_fun_code)
    {
        modbus_data_qty_t data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_QTY_IDX);
        modbus_master_coil_write(slave_adr, data_adr, data_qty);
    }
    else //  MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE
    {
        modbus_data_qty_t data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_QTY_IDX);
        modbus_master_hreg_write(slave_adr, data_adr, data_qty);
    }
    return RET_OK;
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

/**
 * @brief Checks if the Modbus response contains an error code.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure containing the response data.
 * @return True if an error code is present, false otherwise.
 */
static bool modbus_response_contain_exception_code(const modbus_msg_t *modbus_msg)
{
    return (modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] & MODBUS_EXCEPTION_CODE_MASK) == MODBUS_EXCEPTION_CODE_MASK;
}

static modbus_ret_t process_modbus_exception_code(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status = RET_OK;
    static modbus_read_data_t resp;
    resp.slave_ID = modbus_msg->resp.data[MODBUS_SLAVE_ADR_IDX];
    resp.fun_conde = (modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] & (~MODBUS_EXCEPTION_CODE_MASK));
    resp.exception_code = modbus_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX];
    resp.data_adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    resp.data_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
    switch (resp.fun_conde)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        modbus_master_coil_exception(&resp);
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        modbus_master_disin_exception(&resp);
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        modbus_master_hreg_exception(&resp);
        break;
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        modbus_master_inreg_exception(&resp);
        break;
    default:
        status = RET_ERROR;
    }
    return status;
}
/**
 * @brief Processes the Modbus response based on the request function code.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure containing the request and response data.
 * @return - The status of the response processing.
 * @retval - RET_OK
 * @retval - RET_ERROR
 * @retval - RET_ERROR_BYTE_CNT
 * @retval - RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH
 *
 */
static modbus_ret_t process_modbus_response(modbus_msg_t *modbus_msg)
{
    uint32_t mapper_size = MODBUS_MASTER_FUNCTION_MAPPER_SIZE;
    modbus_fun_code_t req_fun_code = modbus_msg->req.data[MODBUS_FUNCTION_CODE_IDX];

    for (uint32_t i = 0; i < mapper_size; i++)
    {
        if (modbus_master_function_mapper[i].fun_code == req_fun_code)
        {
            return modbus_master_function_mapper[i].fun_code_action(modbus_msg);
        }
    }

    return RET_ERROR;
}