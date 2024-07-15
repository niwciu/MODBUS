/**
 * @file modbus_slave_PDU.c
 * @brief Implementation of Modbus slave protocol data unit (PDU) handling functions.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_slave_PDU.h"
#include "modbus_PDU_common.h"
#include "buf_rw.h"
#include <stdio.h>

/* Static function declarations for Modbus slave PDU handling*/
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

/* Array of function pointers mapping Modbus function codes to corresponding handling functions */
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

/**
 * @brief Parses the Modbus master request and prepares the response.
 *
 * This function processes the Modbus master request received in `rx_msg`. It checks for
 * NULL pointers in the message structure and verifies the integrity of the request and
 * response data buffers. If any of these conditions are not met, it returns RET_NULL_PTR_ERROR.
 *
 * Upon successful validation, it identifies the function code from the request and searches
 * for a corresponding function handler in the `modbus_slave_function_mapper`. If a matching
 * handler is found, it invokes the associated function to process the request and prepares
 * the response accordingly.
 *
 * If no matching function handler is found in the mapper, it sets the response function code
 * to indicate an illegal function error and sets an appropriate exception code in the response.
 * It then returns RET_ERROR_UNKNOWN_MAPPER_FUN_CODE.
 *
 * @param rx_msg Pointer to the Modbus message structure containing the request and response data.
 * @return modbus_ret_t Status of the request parsing and response preparation.
 *         @retval - RET_OK if the request was parsed and response prepared successfully.
 *         @retval - RET_NULL_PTR_ERROR if rx_msg, rx_msg->req.data, or rx_msg->resp.data is NULL.
 *         @retval - RET_ERROR_UNKNOWN_MAPPER_FUN_CODE if an unsupported function code is detected.
 *         @retval - RET_ERROR If an error occurred during validation or handling.
 */
modbus_ret_t parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg)
{

    if ((NULL == rx_msg) || (NULL == rx_msg->req.data) || (NULL == rx_msg->resp.data))
    {
        return RET_NULL_PTR_ERROR;
    }

    modbus_ret_t msg_parse_status;
    modbus_fun_code_t req_fun_code = rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
    uint32_t mapper_size = MODBUS_SLAVE_FUNCTION_MAPPER_SIZE;
    modbus_status_flag_t UNSUPORTED_FUNC_CODE_FLAG = MODBUS_FLAG_SET;
    for (uint32_t i = 0; i < mapper_size; i++)
    {
        if (req_fun_code == modbus_slave_function_mapper[i].fun_code)
        {
            msg_parse_status = modbus_slave_function_mapper[i].fun_code_action(rx_msg);
            UNSUPORTED_FUNC_CODE_FLAG = MODBUS_FLAG_CLEARED;
            break;
        }
    }
    if (MODBUS_FLAG_SET == UNSUPORTED_FUNC_CODE_FLAG)
    {
        rx_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] = rx_msg->req.data[MODBUS_FUNCTION_CODE_IDX];
        set_exception_code_resp(rx_msg, MODBUS_ILLEGAL_FUNCTION_ERROR);
        msg_parse_status = RET_ERROR_UNKNOWN_MAPPER_FUN_CODE;
    }

    return msg_parse_status;
}

/**
 * @brief Handles the Modbus function code for reading coils.
 *
 * This function prepares the response message with the appropriate function code,
 * validates the request data, and invokes the service handler to process the read
 * coils operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read coils operation.
 *         - @retval - RET_OK Operation successful.
 *         - @retval - RET_ERROR If an error occurred during validation or handling.
 */
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
/**
 * @brief Handles the Modbus read coils service.
 *
 * This function reads coil states from the slave's memory based on the address and quantity
 * specified in the Modbus request. It prepares the response message with the read coil states
 * and sets an exception code in the response if any errors occur during the read operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read coils service operation.
 *         - @retval - RET_OK Operation successful.
 *         - @retval - RET_ERROR If an error occurred during the read operation.
 */
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
            return RET_ERROR;
        }
        modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (read_coil_val << (i % 8));
    }
    return update_msg_len_and_ret_status(modbus_msg, read_coil_val, byte_cnt);
}

/**
 * @brief Handles the Modbus request to read discrete inputs.
 *
 * This function processes the Modbus request to read discrete inputs from the slave device.
 * It first verifies the correctness of the request data and then delegates the actual read
 * operation to a service function. Upon successful completion of the read operation, it prepares
 * the appropriate response with the read discrete input states.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested discrete inputs were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */

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

/**
 * @brief Handles the Modbus request to read discrete inputs.
 *
 * This function processes the Modbus request to read discrete inputs starting
 * from the specified address and for the given quantity. It verifies the correctness
 * of the request data and reads the state of discrete inputs from the slave device's
 * discrete input array. Upon successful read operation, it prepares the response with
 * the read discrete input states.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested discrete inputs were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */

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

/**
 * @brief Handles the Modbus request to read holding registers.
 *
 * This function initiates the Modbus read request to read holding registers from the
 * slave device. It verifies the correctness of the read request data and then delegates
 * the actual reading of holding registers to the service function.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested holding registers were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */

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

/**
 * @brief Handles the Modbus request to read holding registers.
 *
 * This function processes the Modbus request to read holding registers with the specified
 * starting address and quantity. It reads the registers from the slave device's holding
 * register array and prepares the response with the read register values. It also updates
 * the response length based on the number of registers read.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested holding registers were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */

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
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], (modbus_reg_t)(read_hreg_val));
        }
    }
    return update_msg_len_and_ret_status(modbus_msg, read_hreg_val, byte_cnt);
}

/**
 * @brief Handles the Modbus request to read input registers.
 *
 * This function processes the Modbus request to read input registers with the specified
 * address and quantity. It first checks the correctness of the request data. If the data
 * is valid, it proceeds to handle the read operation by calling the appropriate service function.
 * Upon successful read operation, it prepares the response with the read register values
 * and updates the response length.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested input registers were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */

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

/**
 * @brief Handles the Modbus request to read input registers.
 *
 * This function processes the Modbus request to read input registers starting from
 * the specified address (`adr`) and reads the number of registers (`reg_qty`) requested.
 * It retrieves the state of each register from the slave device's input registers array
 * and writes the values into the response buffer of the provided Modbus message structure.
 * If any read operation fails, it sets an appropriate exception code in the response.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the read operation.
 *         @retval - RET_OK if all requested input registers were read successfully.
 *         @retval - RET_ERROR if there was an error during the read operation.
 */
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
            write_u16_to_buf(&modbus_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + (i * 2)], (modbus_reg_t)(read_input_reg_val));
        }
    }
    return update_msg_len_and_ret_status(modbus_msg, read_input_reg_val, byte_cnt);
}

/**
 * @brief Handles the Modbus request to write a single coil.
 *
 * This function processes the Modbus request to write a single coil with the specified
 * address and state. It verifies the correctness of the request data and sets the coil
 * state accordingly in the slave device's coil array. Upon successful write operation,
 * it prepares the appropriate response with the updated coil address and state.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the write operation.
 * @retval - RET_OK if the write operation was successful.
 * @retval - RET_ERROR if there was an error during the write operation.
 */
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

/**
 * @brief Checks the correctness of the Modbus request data for writing a single coil.
 *
 * This function validates the Modbus request data to ensure that the coil address is within
 * the valid range and the coil state is either COIL_OFF or COIL_ON. It sets the appropriate
 * exception code in the response if any validation fails.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @param coil_adr Address of the coil to be written.
 * @param coil_state State to set for the coil (COIL_OFF or COIL_ON).
 * @return modbus_ret_t Status of the validation.
 *         @retval RET_OK Request data is valid and ready for processing.
 *         @retval RET_ERROR Request data contains invalid parameters or out-of-range address.
 */
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
    {
        return RET_OK;
    }
}

/**
 * @brief Handles the service for writing a single coil based on Modbus message data.
 *
 * This function sets the state of a single coil in the Slave Coils based on the provided
 * coil address and state, then prepares the response with the updated coil address and state.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @param coil_adr Address of the coil to write.
 * @param coil_state State to set for the coil (0 or 1).
 * @return modbus_ret_t Status of the operation.
 *         @retval RET_OK Successfully set the coil state and prepared the response.
 *         @retval RET_ERROR Failed to set the coil state or encountered an error during processing.
 */
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

/**
 * @brief Handles the Modbus slave's multiple coils write operation.
 *
 * This function validates the Modbus request for writing multiple coils,
 * sets the coil states based on the request data, and prepares the response
 * in the Modbus message structure accordingly.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the request data.
 * @return Status of the multiple coils write operation:
 *         - @retval RET_OK if all coils were successfully written.
 *         - @retval RET_ERROR if an error occurred during the write operation.
 */
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

/**
 * @brief Handles the Modbus slave's single register write service.
 *
 * This function reads the address and register value from the Modbus request message,
 * attempts to set the specified register in the slave's holding registers, and prepares
 * the appropriate response in the Modbus response message buffer.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the request data.
 * @return Status of the write operation:
 *         - @retval RET_OK if the register was successfully written.
 *         - @retval RET_ERROR if an error occurred during the write operation.
 */
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

/**
 * @brief Handles the Modbus slave's single register write service.
 *
 * This function reads the address and register value from the Modbus request message,
 * attempts to set the specified register in the slave's holding registers, and prepares
 * the appropriate response in the Modbus response message buffer.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the request data.
 * @return Status of the write operation:
 *         - @retval - RET_OK if the register was successfully written.
 *         - @retval - RET_ERROR if an error occurred during the write operation.
 */
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

/**
 * @brief Handles the Modbus write multiple holding registers request.
 *
 * This function checks the correctness of the write multiple holding registers request,
 * validates the data, and then processes the request by calling the service function that
 * performs the actual writing operation. It sets the function code in the response message
 * and returns the status of the write operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the write operation.
 *         @retval - RET_OK if all holding registers were written successfully.
 *         @retval - RET_ERROR if there was an error during the write process.
 */
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

/**
 * @brief Handles the Modbus write multiple holding registers service.
 *
 * This function processes the Modbus write multiple holding registers request
 * by setting the specified number of holding registers starting from the given
 * address with the data provided in the request message. It updates the response
 * message with the address and quantity of registers written if successful, or
 * sets an exception code in case of failure.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request and response data.
 * @return modbus_ret_t Status of the write operation.
 *         @retval - RET_OK if all holding registers were written successfully.
 *         @retval - RET_ERROR if there was an error during the write process.
 */
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

/**
 * @brief Sets the Modbus exception code in the response message.
 *
 * This function sets the Modbus exception code in the response message
 * at the appropriate index and updates the response length accordingly.
 *
 * @param modbus_msg Pointer to the Modbus message structure.
 * @param exception_code Modbus exception code to be set in the response.
 */
static void set_exception_code_resp(modbus_msg_t *modbus_msg, modbus_exception_code_t exception_code)
{
    modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] |= MODBUS_EXCEPTION_CODE_MASK;
    modbus_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX] = exception_code;
    modbus_msg->resp.len = MODBUS_PDU_EXCEPTION_CODE_LEN;
}

/**
 * @brief Updates the Modbus response length and returns the operation status.
 *
 * This function updates the length of the Modbus response based on the byte count
 * and returns the status of the operation.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the response data.
 * @param last_ret_val Last return value from the preceding operation.
 * @param byte_cnt Number of bytes to be included in the response.
 * @return modbus_ret_t Status of the operation.
 *         - @retval - RET_OK If the operation succeeded and the response length was updated.
 *         - @retval - RET_ERROR If there was an error and the response length could not be updated.
 */
static modbus_ret_t update_msg_len_and_ret_status(modbus_msg_t *modbus_msg, modbus_ret_t last_ret_val, modbus_byte_count_t byte_cnt)
{
    if (RET_ERROR != last_ret_val)
    {
        modbus_msg->resp.len = MODBUS_READ_RESP_LEN + byte_cnt;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}
/**
 * @brief Checks the correctness of read request data based on the Modbus message.
 *
 * This function verifies the correctness of read request data by validating the address range
 * and quantity of data requested against the specified maximum allowed data quantity and
 * application-specific data quantity.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing the request data.
 * @param max_read_data_qty Maximum allowable data quantity for the read operation.
 * @param app_data_qty Application-specific data quantity related to the operation.
 * @return modbus_ret_t Status of the read request data correctness check.
 *         - @retval - RET_OK If the request data is valid and within allowable ranges.
 *         - @retval - RET_ERROR If the request data is invalid, with an appropriate exception
 *           code set in the response.
 */
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
    {
        status = RET_OK;
    }
    return status;
}

/**
 * @brief Checks the correctness of write request data based on the Modbus function code.
 *
 * This function verifies the correctness of write request data by first checking the validity
 * of the overall request data using the specified maximum and application-specific data quantities.
 * Depending on the Modbus function code in the request, it further validates specific aspects:
 * for multiple coils write requests, it checks the byte count correctness; for other types,
 * it checks the register byte count correctness.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request data.
 * @param max_write_data_qty Maximum allowable data quantity for the write operation.
 * @param app_data_qty Application-specific data quantity related to the operation.
 * @return modbus_ret_t Status of the write request data correctness check.
 *         - @retval - RET_OK If all checks pass successfully.
 *         - @retval - RET_ERROR If any check fails, with an appropriate exception code set in the response.
 */
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

/**
 * @brief Checks the correctness of byte count against the expected count for write operations.
 *
 * This function verifies if the byte count provided in the Modbus request matches the
 * expected number of bytes needed for writing coils or discrete inputs. It calculates
 * the expected byte count based on the quantity of coils or discrete inputs provided
 * in the request.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request data.
 * @return modbus_ret_t Status of the byte count correctness check.
 *         - @retval - RET_OK If the byte count matches the expected size.
 *         - @retval - RET_ERROR If the byte count does not match the expected size,
 *           and an exception code is set in the response.
 */
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

/**
 * @brief Checks the correctness of byte count against the number of registers to be written.
 *
 * This function verifies if the byte count provided in the Modbus request matches the
 * expected number of bytes needed to write the specified quantity of registers.
 * If the byte count matches, it returns RET_OK. If not, it sets an exception code
 * in the Modbus response and returns RET_ERROR.
 *
 * @param modbus_msg Pointer to the Modbus message structure containing request data.
 * @return modbus_ret_t Status of the byte count correctness check.
 *         - @retval - RET_OK If the byte count matches the expected size.
 *         - @retval - RET_ERROR If the byte count does not match the expected size,
 *           and an exception code is set in the response.
 */
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

/**
 * @brief Sets the coil or discrete input values based on the Modbus message data.
 *
 * This function iterates through the specified number of coil or discrete input
 * values and sets their states based on the corresponding bits in the provided
 * data state buffer.
 *
 * @param data_state_ptr Pointer to the Modbus data state buffer.
 * @param start_adr Starting address of the coils or discrete inputs.
 * @param coil_din_qty Number of coils or discrete inputs to set.
 * @param data_tab Pointer to the array of coil or discrete input data structures.
 * @return modbus_ret_t Status of the operation.
 *         @retval - RET_OK if all coil/discrete input states were set successfully.
 *         @retval - RET_ERROR if there was an error during the setting process.
 */
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
        {
            break;
        }
    }
    return status;
}