/**
 * @file modbus_master_PDU.c
 * @brief Implementation file for Modbus master protocol data unit (PDU) functionality.
 *
 * This file implements functions for generating Modbus master protocol data unit (PDU) requests
 * for various Modbus operations including reading holding registers, input registers, discrete inputs,
 * and coils, as well as writing single and multiple registers and coils. It also includes a function
 * for processing responses received from a Modbus slave.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-30
 * @version 1.0.0
 *
 * @copyright Copyright (c) 2024
 *
 * @remark The implementation assumes proper configuration and usage of the Modbus protocol
 *         with functions optimized for generating requests and handling responses in master mode.
 *         It includes support for various Modbus function codes and ensures robust error handling
 *         for null pointer checks and parameter validation.
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
static modbus_data_t modbus_get_max_len_2_read(modbus_req_t req_code);

static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static void update_master_coils_frmo_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr);
static void update_master_dis_in_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr);
static void update_input_reg_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr);
static void update_holding_reg_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr);

static modbus_ret_t report_write_operation_confirmation(const modbus_req_resp_t *resp, const modbus_req_resp_t *req);
static modbus_ret_t check_null_ptr_correctness(const modbus_msg_t *modbus_msg);

static bool modbus_response_contain_exception_code(const modbus_msg_t *modbus_msg);
static modbus_ret_t process_modbus_exception_code(modbus_msg_t *modbus_msg);
static modbus_ret_t send_exception_code_report_data(modbus_read_data_t *ex_code_data);
static modbus_ret_t process_modbus_response(modbus_msg_t *modbus_msg);

/**
 * @brief Mapping of Modbus function codes to their corresponding response handling functions.
 *
 * This array maps each Modbus function code to a function pointer that handles the response
 * for that particular function code. It provides a centralized mechanism for dispatching
 * Modbus responses based on their function codes.
 */
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

/**
 * @brief Constructs a Modbus request to read holding registers.
 *
 * Constructs a Modbus request message to read holding registers based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 */
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

/**
 * @brief Constructs a Modbus request to read input registers.
 *
 * Constructs a Modbus request message to read input registers based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 */
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

/**
 * @brief Constructs a Modbus request to read discrete inputs.
 *
 * Constructs a Modbus request message to read discrete inputs based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 */
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
 * @brief Constructs a Modbus request to read multiple coils.
 *
 * Constructs a Modbus request message to read multiple coils based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
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

/**
 * @brief Constructs a Modbus request to write a single register.
 *
 * Constructs a Modbus request message to write a single register based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 */
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
 * @brief Constructs a Modbus request to write a single coil.
 *
 * Constructs a Modbus request message to write a single coil based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * prepares the request data accordingly, and sets the appropriate Modbus function code.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
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
 * @brief Constructs a Modbus request to write multiple registers.
 *
 * Constructs a Modbus request message to write multiple registers based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * validates the quantity of registers to be written, and prepares the request data
 * accordingly.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 * @retval RET_ERROR Invalid quantity of registers to write.
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
 * @brief Constructs a Modbus request to write multiple coils.
 *
 * Constructs a Modbus request message to write multiple coils based on the
 * parameters provided. Checks for null pointer correctness in the message buffer,
 * validates the quantity of coils to be written, and prepares the request data
 * accordingly.
 *
 * @param msg_buf Pointer to the Modbus message structure where the request will be stored.
 * @param req_param Pointer to the structure containing input parameters for the request.
 * @return modbus_ret_t The status of the request construction.
 * @retval RET_OK Request constructed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
 * @retval RET_ERROR Invalid quantity of coils to write.
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
 * @brief Processes the Modbus message response.
 *
 * This function checks for null pointer correctness in the Modbus message.
 * If no null pointer error is detected, it checks if the response contains
 * an exception code. If an exception code is present, it processes the exception.
 * If no exception code is found, it proceeds to process the main response based
 * on the function code.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing response data.
 * @return modbus_ret_t The status of the response processing.
 * @retval RET_OK Response processed successfully.
 * @retval RET_NULL_PTR_ERROR Null pointer detected in the Modbus message structure.
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
/**
 * @brief Processes the response for a Modbus "Read Coils" request.
 *
 * This function updates the master data with the coils' values received from the Modbus response.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the response.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK The response was successfully processed and the master data updated.
 * @retval RET_ERROR The function code in the response is unrecognized, or an error occurred during processing.
 */
static modbus_ret_t modbus_master_read_coils_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Processes the response for a Modbus "Read Discrete Inputs" request.
 *
 * This function updates the master data with the discrete inputs' values received from the Modbus response.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the response.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK The response was successfully processed and the master data updated.
 * @retval RET_ERROR The function code in the response is unrecognized, or an error occurred during processing.
 */
static modbus_ret_t modbus_master_read_discrete_inputs_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Processes the response for a Modbus "Read Input Registers" request.
 *
 * This function updates the master data with the input registers' values received from the Modbus response.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the response.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK The response was successfully processed and the master data updated.
 * @retval RET_ERROR The function code in the response is unrecognized, or an error occurred during processing.
 */
static modbus_ret_t modbus_master_read_input_reg_resp(modbus_msg_t *modbus_msg)
{

    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Processes the response for a Modbus "Read Holding Registers" request.
 *
 * This function updates the master data with the holding registers' values received from the Modbus response.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the response.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK The response was successfully processed and the master data updated.
 * @retval RET_ERROR The function code in the response is unrecognized, or an error occurred during processing.
 */
static modbus_ret_t modbus_master_read_holding_reg_resp(modbus_msg_t *modbus_msg)
{
    return update_master_data_from_modbus_msg(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Handles the response for writing a single coil in Modbus communication.
 *
 * This function processes the response message from the Modbus slave after a request
 * to write a single coil. It reports the completion of the write operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing response data.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK Always returns RET_OK to indicate that the write operation response was processed.
 */
static modbus_ret_t modbus_master_write_single_coil_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Handles the response for writing a single register in the Modbus master mode.
 *
 * This function confirms the success of the write operation based on the response.
 *
 * @param modbus_msg Pointer to the Modbus message containing the response.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK Always returns RET_OK to indicate that the write operation response was processed.
 */
static modbus_ret_t modbus_master_write_single_reg_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Handles the response for writing multiple coils in Modbus communication.
 *
 * This function processes the response message from the Modbus slave after a request
 * to write multiple coils. It reports the completion of the write operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing response data.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK Always returned to indicate that the write operation response was processed.
 */
static modbus_ret_t modbus_master_write_multiple_coils_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}
/**
 * @brief Handles the response for writing multiple registers in Modbus communication.
 *
 * This function processes the response message from the Modbus slave after a request
 * to write multiple registers. It reports the completion of the write operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing response data.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK Always returned to indicate that the write operation response was processed.
 */
static modbus_ret_t modbus_master_write_multiple_reg_resp(modbus_msg_t *modbus_msg)
{
    return report_write_operation_confirmation(&modbus_msg->resp, &modbus_msg->req);
}

/**
 * @brief Constructs a Modbus read request for registers.
 *
 * This function prepares a Modbus request to read registers by populating the request structure
 * with the appropriate function code, address, and quantity of data to read.
 *
 * @param req Pointer to the Modbus request structure to be populated.
 * @param req_code The Modbus function code for the read request.
 * @param adr The starting address for the read operation.
 * @param data_len The quantity of data to read.
 * @return modbus_ret_t The status of the operation.
 * @retval RET_OK The request was successfully constructed.
 * @retval RET_ERROR The data length is out of the allowed range.
 */
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

/**
 * @brief Gets the maximum length of data that can be read for a given Modbus request code.
 *
 * This function determines the maximum number of data elements that can be read based on the Modbus request code.
 * It supports reading coils, discrete inputs, holding registers, and input registers.
 *
 * @param req_code The Modbus request function code.
 * @return modbus_data_t The maximum length of data that can be read.
 */
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

/**
 * @brief Updates the Modbus master's data from a Modbus message.
 *
 * This function processes the response message to update the various types of data
 * (coils, discrete inputs, input registers, and holding registers) of the Modbus master device.
 * It reads the necessary information from the request and response messages and calls the appropriate
 * update functions based on the function code in the response.
 *
 * @param resp Pointer to the Modbus response message structure.
 * @param req Pointer to the Modbus request message structure.
 * @return modbus_ret_t
 * @retval RET_OK The data was successfully updated.
 * @retval RET_ERROR The function code in the response is unrecognized.
 */
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_req_resp_t *resp, const modbus_req_resp_t *req)
{
    modbus_ret_t status = RET_OK;
    modbus_device_ID_t slave_adr = resp->data[MODBUS_SLAVE_ADR_IDX];
    modbus_fun_code_t resp_fun_code = resp->data[MODBUS_FUNCTION_CODE_IDX];
    modbus_adr_t data_adr = read_u16_from_buf(req->data + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t data_qty = read_u16_from_buf(req->data + MODBUS_REQUEST_QTY_IDX);
    switch (resp_fun_code)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
        update_master_coils_frmo_modbus_msg(data_qty, resp, slave_adr, data_adr);
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:

        update_master_dis_in_from_modbus_msg(data_qty, resp, slave_adr, data_adr);
        break;

    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        update_input_reg_from_modbus_msg(data_qty, resp, slave_adr, data_adr);
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        update_holding_reg_from_modbus_msg(data_qty, resp, slave_adr, data_adr);
        break;
    default:
        status = RET_ERROR;
        break;
    }
    return status;
}

/**
 * @brief Updates the Modbus master's coils from a Modbus message.
 *
 * This function processes the response message to update the coils
 * of the Modbus master device. It reads the data from the response message
 * and updates each coil based on the received data.
 *
 * @param data_qty Quantity of coils to update.
 * @param resp Pointer to the Modbus response message structure.
 * @param slave_adr The address of the Modbus slave device.
 * @param data_adr The starting address of the coils in the Modbus slave device.
 */
static void update_master_coils_frmo_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr)
{
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
}

/**
 * @brief Updates discrete inputs (coils) from data received in a Modbus response message.
 *
 * This function reads `data_qty` amount of data from the response (`resp`) starting at index
 * `MODBUS_RESP_READ_DATA_IDX`. For each bit in the data, it determines the state of the coil
 * (ON or OFF) and updates the discrete inputs (`modbus_master_disin_read`) at consecutive
 * addresses starting from `data_adr` on the Modbus device with address `slave_adr`.
 *
 * @param[in] data_qty The quantity of data (number of coils) to update.
 * @param[in] resp Pointer to the Modbus response structure containing the response data.
 * @param[in] slave_adr Modbus device address of the slave.
 * @param[in] data_adr Starting address in the discrete inputs to update.
 */
static void update_master_dis_in_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr)
{
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
}

/**
 * @brief Updates input registers with data received from a Modbus response message.
 *
 * This function reads `data_qty` amount of data from the response (`resp`) starting at index
 * `MODBUS_RESP_READ_DATA_IDX`, and updates the input registers (`modbus_master_inreg_read`)
 * at consecutive addresses starting from `data_adr` on the Modbus device with address `slave_adr`.
 *
 * @param[in] data_qty The quantity of data (number of registers) to update.
 * @param[in] resp Pointer to the Modbus response structure containing the response data.
 * @param[in] slave_adr Modbus device address of the slave.
 * @param[in] data_adr Starting address in the input registers to update.
 */
static void update_input_reg_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr)
{
    for (modbus_data_qty_t i = 0; i < data_qty; i++)
    {
        modbus_data_t reg_data = read_u16_from_buf(&(resp->data[MODBUS_RESP_READ_DATA_IDX + (i * 2)]));
        modbus_master_inreg_read(slave_adr, data_adr + i, reg_data);
    }
}

/**
 * @brief Updates holding registers with data received from a Modbus response message.
 *
 * This function reads `data_qty` amount of data from the response (`resp`) starting at index
 * `MODBUS_RESP_READ_DATA_IDX`, and updates the holding registers (`modbus_master_hreg_read`)
 * at consecutive addresses starting from `data_adr` on the Modbus device with address `slave_adr`.
 *
 * @param[in] data_qty The quantity of data (number of registers) to update.
 * @param[in] resp Pointer to the Modbus response structure containing the response data.
 * @param[in] slave_adr Modbus device address of the slave.
 * @param[in] data_adr Starting address in the holding registers to update.
 */
static void update_holding_reg_from_modbus_msg(modbus_data_qty_t data_qty, const modbus_req_resp_t *resp, modbus_device_ID_t slave_adr, modbus_adr_t data_adr)
{
    for (modbus_data_qty_t i = 0; i < data_qty; i++)
    {
        modbus_data_t reg_data = read_u16_from_buf(&(resp->data[MODBUS_RESP_READ_DATA_IDX + (i * 2)]));
        modbus_master_hreg_read(slave_adr, data_adr + i, reg_data);
    }
}

/**
 * @brief Reports confirmation of a Modbus write operation based on the response received.
 *
 * This function processes the response (`resp`) received from a Modbus device after a write operation
 * request (`req`). Depending on the function code (`resp_fun_code`) in the response, it confirms the
 * write operation by invoking the appropriate write function for coils or holding registers.
 *
 * @param[in] resp Pointer to the Modbus response structure containing the response data.
 * @param[in] req Pointer to the Modbus request structure containing the request data.
 *
 * @retval RET_OK Indicates that the write operation confirmation was successfully reported.
 */
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

/**
 * @brief Checks the correctness of pointers in the Modbus message structure.
 *
 * This function verifies the validity of pointers within the provided Modbus message
 * structure (`modbus_msg`). It checks if the `modbus_msg` pointer itself, the request
 * data pointer (`modbus_msg->req.data`), and the response data pointer (`modbus_msg->resp.data`)
 * are not NULL.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure to be checked.
 *
 * @retval RET_OK           If all pointers within `modbus_msg` are valid.
 * @retval RET_NULL_PTR_ERROR If any of the pointers (`modbus_msg`, `modbus_msg->req.data`,
 *                           `modbus_msg->resp.data`) are NULL.
 */
static modbus_ret_t check_null_ptr_correctness(const modbus_msg_t *modbus_msg)
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
 * @brief Checks if the Modbus response message contains an exception code.
 *
 * This function examines the Modbus response message (`modbus_msg`) to determine
 * if it includes an exception code. It evaluates the function code field of the
 * response against the MODBUS_EXCEPTION_CODE_MASK to detect the presence of an
 * exception code.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure containing the response data.
 *
 * @retval true  If the response message contains an exception code.
 * @retval false If the response message does not contain an exception code.
 */
static bool modbus_response_contain_exception_code(const modbus_msg_t *modbus_msg)
{
    return (modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] & MODBUS_EXCEPTION_CODE_MASK) == MODBUS_EXCEPTION_CODE_MASK;
}

/**
 * @brief Processes the Modbus exception code received in a response message.
 *
 * This function extracts and interprets the Modbus exception code from the
 * response message in `modbus_msg`. It initializes a `modbus_read_data_t` structure
 * (`resp`) with relevant details from the response, including slave ID, function code,
 * exception code, data address, and quantity. Finally, it invokes
 * `send_exception_code_report_data` to handle the exception code and report it if necessary.
 *
 * @param[in] modbus_msg Pointer to the Modbus message structure containing the response data.
 *
 * @retval RET_OK The Modbus exception code was processed successfully.
 * @retval Any other return status indicates an error occurred during processing such us unrecognized function code.
 */
static modbus_ret_t process_modbus_exception_code(modbus_msg_t *modbus_msg)
{
    modbus_ret_t status = RET_OK;
    static modbus_read_data_t resp;
    resp.slave_ID = modbus_msg->resp.data[MODBUS_SLAVE_ADR_IDX];
    resp.fun_conde = (modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] & (~MODBUS_EXCEPTION_CODE_MASK));
    resp.exception_code = modbus_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX];
    resp.data_adr = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_ADR_IDX]);
    resp.data_qty = read_u16_from_buf(&modbus_msg->req.data[MODBUS_REQUEST_QTY_IDX]);
    send_exception_code_report_data(&resp);
    return status;
}

/**
 * @brief Sends an exception code report based on the Modbus function code.
 *
 * This function processes the Modbus function code in `ex_code_data` and sends
 * an exception code report based on the type of function code. Depending on the
 * function code, specific exception handling functions are called to process the exception.
 *
 * @param[in] ex_code_data Pointer to the Modbus read data structure containing
 *                        the function code and associated data.
 *
 * @retval RET_OK The exception code report was sent successfully.
 * @retval RET_ERROR An error occurred during processing, such as an unrecognized
 *                   function code.
 */
static modbus_ret_t send_exception_code_report_data(modbus_read_data_t *ex_code_data)
{
    modbus_ret_t status = RET_OK;
    switch (ex_code_data->fun_conde)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        modbus_master_coil_exception(ex_code_data);
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        modbus_master_disin_exception(ex_code_data);
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        modbus_master_hreg_exception(ex_code_data);
        break;
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        modbus_master_inreg_exception(ex_code_data);
        break;
    default:
        status = RET_ERROR;
    }
    return status;
}
/**
 * @brief Sends an exception code report based on the Modbus function code.
 *
 * This function sends an exception code report based on the Modbus function code
 * contained in `ex_code_data`. Depending on the function code, specific exception
 * handling functions are called to process the exception.
 *
 * @param[in] ex_code_data Pointer to the Modbus read data structure containing
 *                        the function code and associated data.
 * @retval RET_OK The exception code report was sent successfully.
 * @retval RET_ERROR The function code is unrecognized, and no report was sent.
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