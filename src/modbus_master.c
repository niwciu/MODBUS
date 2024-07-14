/**
 * @file modbus_master.c
 * @brief Implementation of a Modbus master protocol handler for embedded systems.
 *
 * This file provides robust functionalities to manage communication with Modbus-compatible
 * slave devices. It supports reading and writing coils, discrete inputs, holding registers,
 * and input registers, adhering to the Modbus RTU (Remote Terminal Unit) protocol over a
 * serial interface.
 *
 * @version 0.0.1
 * @date 2024-05-27
 *
 * @note
 * - This implementation assumes the usage of Modbus RTU for communication with slave devices.
 * - It includes error handling mechanisms, timeout management, and CRC checks for reliable
 *   data transmission.
 *
 * @see modbus_master.h for function declarations and data type definitions used in this file.
 * @see modbus_master_PDU.h and modbus_master_RTU.h for definitions related to protocol data units
 *      and the RTU driver interface.
 * @see modbus_queue.h for the implementation of queue structures used in message management.
 * @see modbus_RTU.h for functions specific to handling the Modbus RTU protocol.
 * @see modbus_type.h for Modbus-specific data type definitions used throughout the implementation.
 * @see modbus_driver_interface.h for the driver interface facilitating interaction with hardware.
 * @see modbus_master_data_interface.h for data interface functions related to Modbus master operations.
 * @see buf_rw.h for utility functions used for buffer read and write operations.
 */

#include "modbus_master.h"
#include "modbus_master_PDU.h"
#include "modbus_master_RTU.h"
#include "modbus_queue.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"
#include "modbus_master_data_interface.h"
#include "buf_rw.h"

#include <stdio.h>

#ifdef UNIT_TEST
#define PRIVATE
#else
#define PRIVATE static
#endif

PRIVATE modbus_master_state_t modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
PRIVATE const modbus_RTU_driver_struct_t *RTU_driver = NULL;
PRIVATE modbus_master_error_cb_t modbus_error_callback = NULL;

static modbus_queue_t master_free_queue;
static modbus_queue_t master_tx_rx_queue;

PRIVATE modbus_queue_t *free_q = &master_free_queue;
PRIVATE modbus_queue_t *tx_rx_q = &master_tx_rx_queue;
PRIVATE modbus_msg_t modbus_msg[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS];
PRIVATE modbus_msg_t *msg_buf = NULL;
PRIVATE modbus_timer_t modbus_master_resp_timeout_timer = 0;
PRIVATE uint8_t modbus_master_msg_repeat_couter = 0;

PRIVATE modbus_status_flag_t MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_RTU_CRC_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;

static void modbus_master_send_req_from_msg_buf(void);
static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void push_all_available_msg_buffer_to_free_queue(void);
static modbus_master_req_ret_t generate_request(req_input_param_struct_t *req_param);
static modbus_ret_t generate_request_PDU_data(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
static void modbus_master_enable_resp_timeout_timer(void);
static void modbus_master_disable_resp_timeout_timer(void);

static void modbus_master_req_sended_callback(void);
static void modbus_master_T_1_5_char_expired_callback(void);
static void modbus_master_T_3_5_char_expired_callback(void);
static void modbus_master_frame_error_callback(void);

static bool check_data_to_send_availability(const modbus_queue_t *q);
static void modbus_master_idle_state_handling(void);
static void modbus_master_transmitting_state_handling(void);
static void modbus_master_resp_waiting_state_handling(void);
static void modbus_master_resp_timeout_handle();
static void modbus_master_RTU_recv_state_handle();
static void modbus_master_resp_recived_state_handling(void);

static void modbus_master_msg_recived_correctly_state_handle();

static void modbus_master_RTU_CRC_error_state_handling(void);
static void modbus_master_frame_error_state_handling(void);

static void modbus_master_msg_process_end(void);

/**
 * @brief Function pointer type for handling Modbus master function codes.
 *
 * This typedef defines a function pointer type for functions that handle Modbus master function codes.
 *
 * @param msg_buf Pointer to the Modbus message buffer.
 * @param req_param Pointer to the request input parameters.
 * @return Status of the Modbus operation.
 */
typedef modbus_ret_t (*modbus_master_fun_code_handler_t)(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);

/**
 * @brief Structure mapping Modbus function codes to their corresponding handlers.
 *
 * This structure maps Modbus function codes to their corresponding handler functions.
 */
struct modbus_master_functions_mapper
{
    modbus_fun_code_t fun_code;                       /**< Modbus function code */
    modbus_master_fun_code_handler_t fun_code_action; /**< Function handler for the Modbus function code */
};

/**
 * @brief Array mapping Modbus function codes to their corresponding handlers.
 *
 * This constant array maps Modbus function codes to their corresponding handler functions.
 */
const struct modbus_master_functions_mapper master_functions_mapper[] = {
    {MODBUS_READ_COILS_FUNC_CODE, modbus_master_read_coils_req},
    {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, modbus_master_read_discrete_inputs_req},
    {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, modbus_master_read_holding_reg_req},
    {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, modbus_master_read_input_reg_req},
    {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, modbus_master_write_single_coil_req},
    {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, modbus_master_write_single_reg_req},
    {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, modbus_master_write_multiple_coils_req},
    {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, modbus_master_write_multiple_reg_req},
};

#define MODBUS_MASTER_FUNCTIONS_MAPPER_SIZE (sizeof(master_functions_mapper) / sizeof(master_functions_mapper[0]));

/**
 * @brief Reads coils from a slave device.
 *
 * This function creates a request to read coils from a specified address on a slave device.
 *
 * @param adr The starting address to read coils from.
 * @param coils_qty The quantity of coils to read.
 * @param slave_ID The ID of the slave device to read from.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty, slave_ID, 0, NULL, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Reads discrete inputs from a slave device.
 *
 * This function creates a request to read discrete inputs from a specified address on a slave device.
 *
 * @param adr The starting address to read discrete inputs from.
 * @param discrete_input_qty The quantity of discrete inputs to read.
 * @param slave_ID The ID of the slave device to read from.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty, modbus_device_ID_t slave_ID)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty, slave_ID, 0, NULL, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Reads input registers from a slave device.
 *
 * This function creates a request to read input registers from a specified address on a slave device.
 *
 * @param adr The starting address to read input registers from.
 * @param reg_qty The quantity of input registers to read.
 * @param slave_ID The ID of the slave device to read from.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty, slave_ID, 0, NULL, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Reads holding registers from a slave device.
 *
 * This function creates a request to read holding registers from a specified address on a slave device.
 *
 * @param adr The starting address to read holding registers from.
 * @param hreg_qty The quantity of holding registers to read.
 * @param slave_ID The ID of the slave device to read from.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty, modbus_device_ID_t slave_ID)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty, slave_ID, 0, NULL, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Writes a single coil to a slave device.
 *
 * This function creates a request to write a single coil to a specified address on a slave device.
 *
 * @param adr The address to write the coil to.
 * @param slave_ID The ID of the slave device to write to.
 * @param coil_2_write The value of the coil to write.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_write_single_coil(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_coil_disin_t coil_2_write)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr, 0, slave_ID, coil_2_write, NULL, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Writes a single register to a slave device.
 *
 * This function creates a request to write a single register to a specified address on a slave device.
 *
 * @param adr The address to write the register to.
 * @param slave_ID The ID of the slave device to write to.
 * @param reg_2_write The value of the register to write.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_write_single_reg(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_reg_t reg_2_write)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr, 0, slave_ID, 0, NULL, reg_2_write, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Writes multiple registers to a slave device.
 *
 * This function creates a request to write multiple registers to a specified address on a slave device.
 *
 * @param adr The starting address to write the registers to.
 * @param reg_qty The quantity of registers to write.
 * @param slave_ID The ID of the slave device to write to.
 * @param rw_data_ptr A pointer to the data to write to the registers.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, adr, reg_qty, slave_ID, 0, NULL, 0, rw_data_ptr};
    return generate_request(&req_input_param);
}

/**
 * @brief Writes multiple coils to a slave device.
 *
 * This function creates a request to write multiple coils to a specified address on a slave device.
 *
 * @param adr The starting address to write the coils to.
 * @param coils_qty The quantity of coils to write.
 * @param slave_ID The ID of the slave device to write to.
 * @param rw_data_ptr A pointer to the data to write to the coils.
 * @return The status of the Modbus master request.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR If the free queue is empty.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR If there's an error generating the PDU request.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR If there's an error sending the RTU message.
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE If the request was successfully sent to the queue.
 */
modbus_master_req_ret_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, adr, coils_qty, slave_ID, 0, rw_data_ptr, 0, NULL};
    return generate_request(&req_input_param);
}

/**
 * @brief Initializes the Modbus master.
 *
 * This function sets up the Modbus master by registering message request/response data buffers,
 * initializing queues, and pushing all available message buffers to the free queue.
 * It also initializes the RTU driver with the provided baud rate and parity settings, and registers
 * the necessary callbacks for handling various Modbus events.
 *
 * @param mode The mode of Modbus communication (e.g., RTU).
 * @param baud_rate The baud rate for the RTU communication.
 * @param parity The parity setting for the RTU communication.
 *
 * @note This function sets up all internal flags and the Modbus master state machine to their default values.
 */
void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    register_msg_req_resp_data_buffers(mode);
    modbus_queue_init(free_q);
    modbus_queue_init(tx_rx_q);
    push_all_available_msg_buffer_to_free_queue();

    RTU_driver = get_master_RTU_driver_interface();
    RTU_driver->init(baud_rate, parity);

    // ToDo registration of all callbacks
    RTU_driver->subscribe_msg_tx_done_cb(modbus_master_req_sended_callback);
    RTU_driver->subscribe_t_1_5_char_expired_cb(modbus_master_T_1_5_char_expired_callback);
    RTU_driver->subscribe_t_3_5_char_expired_cb(modbus_master_T_3_5_char_expired_callback);
    RTU_driver->subscribe_modbus_frame_error_cb(modbus_master_frame_error_callback);
    // ToDo setting up all flags ans modus_master_stateMachine
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_RTU_CRC_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    // set all internall variable to its default values
    modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
    modbus_master_msg_repeat_couter = 0;        // ToDo init test for this var
    modbus_master_disable_resp_timeout_timer(); // ToDo init test for this var
}

/**
 * @brief Updates the Modbus master state machine.
 *
 * This function manages the state transitions of the Modbus master state machine.
 * Depending on the current state, it invokes the appropriate handler function:
 * - `MODBUS_MASTER_IDLE`: Calls `modbus_master_idle_state_handling` to handle the idle state.
 * - `MODBUS_MASTER_REPEAT_REQUEST`: Calls `modbus_master_send_req_from_msg_buf` to resend the request.
 * - `MODBUS_MASTER_TRANSMITTING_REQ`: Calls `modbus_master_transmitting_state_handling` to handle the transmitting state.
 * - `MODBUS_MASTER_RESP_WAITING`: Calls `modbus_master_resp_waiting_state_handling` to handle the response waiting state.
 * - `MODBUS_MASTER_RESP_RECIVED`: Calls `modbus_master_resp_recived_state_handling` to handle the received response state.
 *
 * If the state does not match any of the above, it defaults to setting the state machine to `MODBUS_MASTER_IDLE`.
 */
void update_modbus_master_manager(void)
{
    switch (modbus_master_manager_state_machine)
    {
    case MODBUS_MASTER_IDLE:
        modbus_master_idle_state_handling();
        break;
    case MODBUS_MASTER_REPEAT_REQUEST:
        modbus_master_send_req_from_msg_buf();
        break;
    case MODBUS_MASTER_TRANSMITTING_REQ:
        modbus_master_transmitting_state_handling();
        break;
    case MODBUS_MASTER_RESP_WAITING:
        modbus_master_resp_waiting_state_handling();
        break;
    case MODBUS_MASTER_RESP_RECIVED:
        modbus_master_resp_recived_state_handling();
        break;
    default:
        modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
        break;
    }
}

/**
 * @brief Sends the Modbus request from the message buffer.
 *
 * This function sends the Modbus request stored in `msg_buf` using the RTU driver.
 * It sets up the RTU driver to receive the response, updates the transmission flag,
 * and transitions the state machine to the transmitting request state.
 *
 * @note Assumes `RTU_driver` is properly initialized and `msg_buf` contains the request to be sent.
 */
static void modbus_master_send_req_from_msg_buf(void)
{
    RTU_driver->send(msg_buf->req.data, msg_buf->req.len);
    RTU_driver->enable_rcev(&msg_buf->resp);
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_SET;
    modbus_master_manager_state_machine = MODBUS_MASTER_TRANSMITTING_REQ;
}

/**
 * @brief Registers request and response data buffers for Modbus messages based on the mode.
 *
 * This function assigns the appropriate data buffers to each Modbus message in the `modbus_msg` array
 * depending on the specified Modbus mode. For the RTU mode, it assigns the `master_RTU_req_buf` and
 * `master_RTU_resp_buf` buffers. If a different mode is provided, it sets the data pointers to `NULL`.
 *
 * @param mode The Modbus mode to configure (e.g., RTU).
 *
 * @note The function assumes that `modbus_msg` is an array containing Modbus message structures,
 *       and `master_RTU_req_buf` and `master_RTU_resp_buf` are arrays of buffers for the RTU mode.
 */
static void register_msg_req_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
        {
            modbus_msg[i].req.data = &master_RTU_req_buf[i][0];
            modbus_msg[i].resp.data = &master_RTU_resp_buf[i][0];
        }
    }
    // else if( OTHER_MODE == mode)
    else
    {
        for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
        {
            modbus_msg[i].req.data = NULL;
            modbus_msg[i].resp.data = NULL;
        }
    }
}

/**
 * @brief Pushes all available Modbus message buffers to the free queue.
 *
 * This function iterates through all Modbus message buffers defined in `modbus_msg` and
 * pushes each buffer into the free queue (`free_q`). This is typically used during
 * initialization to ensure all message buffers are available for future use.
 *
 * @note This function assumes that `modbus_msg` is an array containing the available Modbus message buffers,
 *       and `free_q` is a queue intended to store these buffers for future allocation.
 */
static void push_all_available_msg_buffer_to_free_queue(void)
{
    modbus_msg_t *msg_ptr;
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        msg_ptr = &modbus_msg[i];
        modbus_queue_push(free_q, &msg_ptr);
    }
}

/**
 * @brief Generates a Modbus request message and sends it to the specified slave device.
 *
 * This function performs the following operations:
 * - Retrieves a Modbus message buffer (`msg_buf`) from the free queue (`free_q`).
 * - Generates the Protocol Data Unit (PDU) data for the request based on the provided
 *   request parameters (`req_param`) using the `generate_request_PDU_data` function.
 * - Sends the generated request using the Modbus RTU transmission function (`modbus_RTU_send`).
 * - Pushes the Modbus message buffer (`msg_buf`) into the transmission/reception queue (`tx_rx_q`).
 *
 * If any operation encounters an error or if the free queue is empty, appropriate error codes
 * are returned to indicate the failure reason.
 *
 * @param req_param Pointer to the structure containing input parameters for the request,
 *                 including function code, slave ID, and data pointers.
 *
 * @retval MODBUS_MASTER_REQUEST_SEND_TO_QUEUE Request successfully generated and sent to queue.
 * @retval MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR Failed to pop a message buffer from the free queue.
 * @retval MODBUS_MASTER_LIB_PDU_REQ_ERROR Error occurred during PDU data generation.
 * @retval MODBUS_MASTER_LIB_RTU_SEND_ERROR Error occurred during Modbus RTU transmission.
 */
static modbus_master_req_ret_t generate_request(req_input_param_struct_t *req_param)
{
    msg_buf = modbus_queue_pop(free_q);
    modbus_ret_t modbus_lib_ret;
    if (NULL == msg_buf)
    {
        return MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR;
    }
    modbus_lib_ret = generate_request_PDU_data(msg_buf, req_param);
    if (0 > modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_PDU_REQ_ERROR;
    }
    modbus_lib_ret = modbus_RTU_send(msg_buf->req.data, &msg_buf->req.len, req_param->slave_ID);
    if (RET_ERROR == modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_RTU_SEND_ERROR;
    }
    modbus_queue_push(tx_rx_q, &msg_buf);
    return MODBUS_MASTER_REQUEST_SEND_TO_QUEUE;
}

/**
 * @brief Generates Modbus Protocol Data Unit (PDU) data based on the provided request parameters.
 *
 * This function iterates through the master functions mapper to find a match for the given
 * function code (`req_param->fun_code`). Once a matching function is found, it invokes the
 * corresponding action function from the mapper to populate the Modbus message buffer (`msg_buf`)
 * with the required PDU data.
 *
 * @param msg_buf Pointer to the Modbus message buffer where the PDU data will be generated.
 * @param req_param Pointer to the structure containing input parameters for the request,
 *                 including function code and data pointers.
 * @return modbus_ret_t Returns the status of the PDU generation process. If a matching function
 *         code is found in the mapper, it returns the status of the action function execution.
 *         If no match is found, it returns RET_ERROR_UNKNOWN_MAPPER_FUN_CODE.
 */
static modbus_ret_t generate_request_PDU_data(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    modbus_ret_t PDU_ret_status = RET_ERROR_UNKNOWN_MAPPER_FUN_CODE;
    uint32_t master_mapper_size = MODBUS_MASTER_FUNCTIONS_MAPPER_SIZE;
    for (uint32_t i = 0; i < master_mapper_size; i++)
    {
        if (master_functions_mapper[i].fun_code == req_param->fun_code)
        {
            PDU_ret_status = master_functions_mapper[i].fun_code_action(msg_buf, req_param);
        }
    }
    return PDU_ret_status;
}

/**
 * @brief Enables the Modbus response timeout timer with a specified timeout value.
 *
 * This function sets the Modbus response timeout timer (`modbus_master_resp_timeout_timer`)
 * to a predefined timeout value (`MODBUS_MASTER_RESP_TIME_OUT_MS`). This timer is used
 * to detect when a response from the Modbus slave device is overdue.
 */
static void modbus_master_enable_resp_timeout_timer(void)
{
    modbus_master_resp_timeout_timer = MODBUS_MASTER_RESP_TIME_OUT_MS;
}

/**
 * @brief Disables the Modbus response timeout timer.
 *
 * This function sets the Modbus response timeout timer (`modbus_master_resp_timeout_timer`)
 * to zero, effectively disabling it. This is typically called when a Modbus response
 * is received or handled, ensuring that no timeout occurs during active communication.
 */
static void modbus_master_disable_resp_timeout_timer(void)
{
    modbus_master_resp_timeout_timer = 0;
}

/**
 * @brief Notifies that a Modbus request has been successfully sent.
 *
 * This function is called by the driver to indicate that a Modbus request
 * has been successfully transmitted over the communication channel. It clears
 * the `MODBUS_MASTER_REQ_TRANSMITION_FLAG` to `MODBUS_FLAG_CLEARED`, allowing
 * the system to proceed with subsequent operations.
 *
 * @note This callback function is typically used to synchronize actions after
 * sending a Modbus request.
 */
static void modbus_master_req_sended_callback(void)
{
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

/**
 * @brief Sets the Modbus 1.5 character timeout flag upon timer expiration.
 *
 * This function sets the `MODBUS_MASTER_TIMER_1_5_CHAR_FLAG` to `MODBUS_FLAG_SET`.
 * It serves as a callback function used to indicate that the 1.5 character timeout period
 * has expired during Modbus communication.
 *
 * @note This function is typically registered with a timer or scheduler to handle the timeout
 * condition required by the Modbus protocol for communication synchronization.
 */
static void modbus_master_T_1_5_char_expired_callback(void)
{
    MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

/**
 * @brief Sets the Modbus 3.5 character timeout flag upon timer expiration.
 *
 * This function sets the `MODBUS_MASTER_TIMER_3_5_CHAR_FLAG` to `MODBUS_FLAG_SET`.
 * It is used as a callback function to indicate that the 3.5 character timeout period
 * has expired during Modbus communication.
 *
 * @note This function is typically registered with a timer or scheduler to handle the timeout
 * condition required by the Modbus protocol for communication synchronization.
 */
static void modbus_master_T_3_5_char_expired_callback(void)
{
    MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

/**
 * @brief Sets the Modbus frame error flag to indicate a detected frame error.
 *
 * This function sets the `MODBUS_MASTER_FRAME_ERROR_FLAG` to `MODBUS_FLAG_SET`.
 * It serves as a callback function invoked by the USART driver upon detecting a frame error
 * during Modbus communication.
 *
 * @note This function is typically registered with the USART driver to handle frame errors
 * encountered while transmitting or receiving Modbus messages.
 */
static void modbus_master_frame_error_callback(void)
{
    MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
}
/**
 * @brief Checks the availability of data to send in a Modbus queue.
 *
 * This function checks if there is data available to send in the specified Modbus queue (`q`).
 * It returns true if either of the following conditions is met:
 * - The queue's head pointer (`head`) is not equal to its tail pointer (`tail`), indicating data is present.
 * - The queue's last position status (`last_queue_pos_status`) equals `LAST_QUEUE_POS_STORE_DATA`.
 *
 * @param q Pointer to the Modbus queue structure (`modbus_queue_t`) to check.
 * @return true if data is available to send, false otherwise.
 */
static bool check_data_to_send_availability(const modbus_queue_t *q)
{
    return ((q->head != q->tail) || (LAST_QUEUE_POS_STORE_DATA == q->last_queue_pos_status));
}

/**
 * @brief Handles the idle state in the Modbus master manager.
 *
 * This function checks if there is data available to send in the transmit/receive queue (`tx_rx_q`).
 * If data is available:
 * - It retrieves a message buffer (`msg_buf`) from the queue using `modbus_queue_pop`.
 * - It sends the request stored in `msg_buf` by calling `modbus_master_send_req_from_msg_buf`.
 *
 * This function is responsible for managing the transition from the idle state to transmitting
 * a Modbus request when data is available in the queue.
 */
static void modbus_master_idle_state_handling(void)
{
    if (check_data_to_send_availability(tx_rx_q))
    {
        msg_buf = modbus_queue_pop(tx_rx_q);
        modbus_master_send_req_from_msg_buf();
    }
}
/**
 * @brief Handles the state during Modbus message transmission.
 *
 * This function enables the response timeout timer and updates the state machine
 * to indicate that the system is now waiting for a response (`MODBUS_MASTER_RESP_WAITING`).
 *
 * It checks if the transmission flag (`MODBUS_MASTER_REQ_TRANSMITION_FLAG`) is cleared,
 * indicating that the system is ready to transmit a Modbus request.
 * Upon clearing the transmission flag:
 * - The response timeout timer is enabled using `modbus_master_enable_resp_timeout_timer`.
 * - The state machine transitions to `MODBUS_MASTER_RESP_WAITING`.
 */
static void modbus_master_transmitting_state_handling(void)
{
    if (MODBUS_FLAG_CLEARED == MODBUS_MASTER_REQ_TRANSMITION_FLAG)
    {
        modbus_master_enable_resp_timeout_timer();
        modbus_master_manager_state_machine = MODBUS_MASTER_RESP_WAITING;
    }
}

/**
 * @brief Handles the state when waiting for a Modbus response.
 *
 * This function determines the appropriate action based on the state of timers
 * and flags related to Modbus communication:
 * - If `MODBUS_MASTER_TIMER_1_5_CHAR_FLAG` is set, it calls
 *   `modbus_master_RTU_recv_state_handle` to handle the reception of Modbus data.
 * - If `modbus_master_resp_timeout_timer` equals 1, indicating a response timeout,
 *   it calls `modbus_master_resp_timeout_handle` to handle the timeout error
 *   and subsequently ends the message processing using `modbus_master_msg_process_end`.
 */
static void modbus_master_resp_waiting_state_handling(void)
{
    if (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_1_5_CHAR_FLAG)
    {
        modbus_master_RTU_recv_state_handle();
    }
    else if (1 == modbus_master_resp_timeout_timer)
    {
        modbus_master_resp_timeout_handle();
        modbus_master_msg_process_end();
    }
}

/**
 * @brief Handles the timeout event for a Modbus master response.
 *
 * This function is called when a timeout occurs while waiting for a response
 * from a Modbus slave device:
 * - Initializes a static instance of `modbus_master_error_report_t` for timeout error reporting.
 * - Retrieves relevant data from `msg_buf` to populate `timeout_error`:
 *   - `slave_ID`: Modbus slave address extracted from the request data.
 *   - `data_adr`: Address field from the Modbus request.
 *   - `data_qty`: Quantity field from the Modbus request.
 *   - `fun_conde`: Function code from the Modbus response (if available).
 *   - `resp_read_error`: Indicates a timeout error (`MODBUS_MASTER_RESP_TIMEOUT_ERR`).
 * - Invokes `modbus_master_data_timeout_error` to handle and report the timeout error.
 */
void modbus_master_resp_timeout_handle()
{
    static modbus_master_error_report_t timeout_error;
    timeout_error.slave_ID = msg_buf->req.data[MODBUS_SLAVE_ADR_IDX];
    timeout_error.data_adr = read_u16_from_buf(&msg_buf->req.data[MODBUS_REQUEST_ADR_IDX]);
    timeout_error.data_qty = read_u16_from_buf(&msg_buf->req.data[MODBUS_REQUEST_QTY_IDX]);
    timeout_error.fun_conde = msg_buf->resp.data[MODBUS_FUNCTION_CODE_IDX];
    timeout_error.resp_read_error = MODBUS_MASTER_RESP_TIMEOUT_ERR;
    modbus_master_data_timeout_error(&timeout_error);
}

/**
 * @brief Handles the state when a Modbus RTU response is received.
 *
 * This function processes the response received from the Modbus RTU interface:
 * - Resets the MODBUS_MASTER_TIMER_1_5_CHAR_FLAG and MODBUS_MASTER_TIMER_3_5_CHAR_FLAG
 *   indicating the end of the response reception timing.
 * - Calls modbus_RTU_recv to interpret the response and determines the RTU_status.
 * - If RTU_status indicates a CRC error (RET_ERROR_CRC):
 *   - Disables the Modbus response timeout timer.
 *   - Sets MODBUS_MASTER_RTU_CRC_ERROR_FLAG to indicate a CRC error occurred.
 *   - Updates modbus_master_manager_state_machine to MODBUS_MASTER_RESP_RECIVED.
 * - If RTU_status indicates an invalid slave ID (RET_ERROR_SLAVE_ID):
 *   - Resets the response length (msg_buf->resp.len) to 0.
 * - If RTU_status is successful (RET_OK):
 *   - Disables the Modbus response timeout timer.
 *   - Updates modbus_master_manager_state_machine to MODBUS_MASTER_RESP_RECIVED.
 */
static void modbus_master_RTU_recv_state_handle()
{
    modbus_ret_t RTU_status;
    modbus_device_ID_t modbus_req_slave_ID = msg_buf->req.data[MODBUS_SLAVE_ADR_IDX];
    MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    RTU_status = modbus_RTU_recv(msg_buf->resp.data, msg_buf->resp.len, modbus_req_slave_ID);
    if (RET_ERROR_CRC == RTU_status)
    {
        modbus_master_disable_resp_timeout_timer();
        MODBUS_MASTER_RTU_CRC_ERROR_FLAG = MODBUS_FLAG_SET;
        modbus_master_manager_state_machine = MODBUS_MASTER_RESP_RECIVED;
    }
    else if (RET_ERROR_SLAVE_ID == RTU_status)
    {
        msg_buf->resp.len = 0;
    }
    else // RET_OK
    {
        modbus_master_disable_resp_timeout_timer();
        modbus_master_manager_state_machine = MODBUS_MASTER_RESP_RECIVED;
    }
}

/**
 * @brief Handles the state when a Modbus response is received.
 *
 * This function determines the appropriate action based on the flags and timers
 * related to Modbus communication:
 * - If both `MODBUS_MASTER_FRAME_ERROR_FLAG` and `MODBUS_MASTER_TIMER_3_5_CHAR_FLAG` are set,
 *   it calls `modbus_master_frame_error_state_handling` to handle frame errors.
 * - If both `MODBUS_MASTER_RTU_CRC_ERROR_FLAG` and `MODBUS_MASTER_TIMER_3_5_CHAR_FLAG` are set,
 *   it calls `modbus_master_RTU_CRC_error_state_handling` to handle CRC errors.
 * - If `MODBUS_MASTER_FRAME_ERROR_FLAG` and `MODBUS_MASTER_RTU_CRC_ERROR_FLAG` are cleared,
 *   and `MODBUS_MASTER_TIMER_3_5_CHAR_FLAG` is set, it calls
 *   `modbus_master_msg_recived_correctly_state_handle` to process the received message correctly.
 * - Otherwise, it takes no action until `MODBUS_MASTER_TIMER_3_5_CHAR_FLAG` is set. This flag is set
 *   after 3.5 character times of silence on the Modbus line.
 */
static void modbus_master_resp_recived_state_handling(void)
{
    if ((MODBUS_FLAG_SET == MODBUS_MASTER_FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_3_5_CHAR_FLAG))
    {
        modbus_master_frame_error_state_handling();
    }
    else if ((MODBUS_FLAG_SET == MODBUS_MASTER_RTU_CRC_ERROR_FLAG) && (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_3_5_CHAR_FLAG))
    {
        modbus_master_RTU_CRC_error_state_handling();
    }
    else if ((MODBUS_FLAG_CLEARED == MODBUS_MASTER_FRAME_ERROR_FLAG) && (MODBUS_FLAG_CLEARED == MODBUS_MASTER_RTU_CRC_ERROR_FLAG) && (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_3_5_CHAR_FLAG))
    {
        modbus_master_msg_recived_correctly_state_handle();
    }
    else
    {
        // do nothing untill 3_5TFlag is not set
    }
}

/**
 * @brief Handles the correct reception of a Modbus response message.
 *
 * This function is invoked when a Modbus response message is received correctly
 * without any frame or CRC errors. It calls `modbus_master_read_slave_resp` to
 * process the received response stored in `msg_buf`. After processing, it finalizes
 * the message handling by calling `modbus_master_msg_process_end` to push the message
 * buffer back to the free queue and transition the state machine to idle state (`MODBUS_MASTER_IDLE`).
 */
static void modbus_master_msg_recived_correctly_state_handle(void)
{
    modbus_master_read_slave_resp(msg_buf);
    modbus_master_msg_process_end();
}

/**
 * @brief Handles CRC error during Modbus master communication.
 *
 * This function is called when a CRC error is detected in the Modbus response during
 * master communication. It increments the message repeat counter and checks if the maximum
 * retry count (`MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR`) has been exceeded. If retries are within
 * the limit, it transitions the state machine to repeat the request (`MODBUS_MASTER_REPEAT_REQUEST`)
 * and clears the CRC error flag (`MODBUS_MASTER_RTU_CRC_ERROR_FLAG`).
 *
 * If retries exceed the limit, it constructs an error report (`error_rep`) containing
 * details such as slave ID, function code, and error type (`MODBUS_MASTER_RESP_RTU_CRC_ERR`).
 * The function then calls `modbus_master_communication_error` to handle the communication error
 * and invokes `modbus_master_msg_process_end` to finalize the message processing.
 */
static void modbus_master_RTU_CRC_error_state_handling(void)
{
    modbus_master_msg_repeat_couter++;
    if (MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR >= modbus_master_msg_repeat_couter)
    {
        modbus_master_manager_state_machine = MODBUS_MASTER_REPEAT_REQUEST;
        MODBUS_MASTER_RTU_CRC_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else
    {

        static modbus_master_error_report_t error_rep;
        error_rep.slave_ID = msg_buf->resp.data[MODBUS_SLAVE_ADR_IDX];
        error_rep.fun_conde = (msg_buf->resp.data[MODBUS_FUNCTION_CODE_IDX] & (~MODBUS_EXCEPTION_CODE_MASK));
        error_rep.resp_read_error = MODBUS_MASTER_RESP_RTU_CRC_ERR;
        modbus_master_communication_error(&error_rep);
        modbus_master_msg_process_end();
    }
}

/**
 * @brief Handles frame error during Modbus master communication.
 *
 * This function is called when a frame error is detected during Modbus master communication.
 * It increments the message repeat counter and checks if the maximum retry count
 * (`MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR`) has been exceeded. If retries are within the limit,
 * it transitions the state machine to repeat the request (`MODBUS_MASTER_REPEAT_REQUEST`)
 * and clears the frame error flag (`MODBUS_MASTER_FRAME_ERROR_FLAG`).
 *
 * If retries exceed the limit, it constructs an error report (`error_rep`) containing
 * details such as slave ID, function code, and error type (`MODBUS_MASTER_RESP_FRAME_ERR`).
 * The function then calls `modbus_master_communication_error` to handle the communication error
 * and invokes `modbus_master_msg_process_end` to finalize the message processing.
 */
static void modbus_master_frame_error_state_handling(void)
{
    modbus_master_msg_repeat_couter++;
    if (MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR >= modbus_master_msg_repeat_couter)
    {
        modbus_master_manager_state_machine = MODBUS_MASTER_REPEAT_REQUEST;
        MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else
    {

        static modbus_master_error_report_t error_rep;
        error_rep.slave_ID = msg_buf->resp.data[MODBUS_SLAVE_ADR_IDX];
        error_rep.fun_conde = (msg_buf->resp.data[MODBUS_FUNCTION_CODE_IDX] & (~MODBUS_EXCEPTION_CODE_MASK));
        error_rep.resp_read_error = MODBUS_MASTER_RESP_FRAME_ERR;
        modbus_master_communication_error(&error_rep);
        modbus_master_msg_process_end();
    }
}

/**
 * @brief Finalizes the Modbus master message processing.
 *
 * This function finalizes the processing of the Modbus master message by performing
 * the following actions:
 * 1. Resets the Modbus master message repeat counter (`modbus_master_msg_repeat_couter`) to zero.
 * 2. Pushes the Modbus message buffer (`msg_buf`) back into the free queue (`free_q`).
 * 3. Transitions the Modbus master manager state machine (`modbus_master_manager_state_machine`)
 *    to the idle state (`MODBUS_MASTER_IDLE`), indicating that the Modbus master is ready
 *    for the next communication operation.
 */
static void modbus_master_msg_process_end(void)
{
    modbus_master_msg_repeat_couter = 0;
    modbus_queue_push(free_q, &msg_buf);
    modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
}
