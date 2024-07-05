/**
 * @file modbus_type.h
 * @brief Header file defining types and constants for Modbus protocol implementation.
 *
 * This header file defines various types, constants, enums, and structures used in the
 * implementation of the Modbus protocol. It includes definitions for Modbus RTU buffer sizes,
 * maximum data quantities, offsets for Modbus frame parameters, error codes, communication
 * states, flags, and data types like Modbus messages and function handlers.
 *
 * The defined constants ensure compatibility with Modbus standard specifications, facilitating
 * the development of Modbus-compliant applications. The file also includes enums for handling
 * various return codes and states, and data structures for message handling.
 *
 * @version 1.0.0
 * @date 2024-05-07
 * @author niwciu
 * @contact niwciu@gmail.com
 *
 * @copyright Copyright (c) 2024
 */
#ifndef _MODBUS_TYPE_H_
#define _MODBUS_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_public_type.h"
#include "modbus_config.h"
    // clang-format off

/// Modbus RTU buffer size specification
#define MODBUS_RTU_BUFFER_SIZE 256U

/// Maximum length of Modbus PDU
#define MODBUS_PDU_MAX_LEN 253U

/// Maximum allowed specific data items quantity
#define MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY  65535U 

/// Maximum quantity of discrete inputs to read
#define MODBUS_MAX_READ_DISCRETE_INPUTS_QTY 0x7D0U

/// Maximum quantity of coils to read
#define MODBUS_MAX_READ_COILS_QTY 0x7D0U

/// Maximum quantity of coils to write
#define MODBUS_MAX_WRITE_COILS_QTY 0x7B0U

/// Maximum quantity of registers to read
#define MODBUS_MAX_READ_REG_QTY 0x7DU

/// Maximum quantity of registers to write
#define MODBUS_MAX_WRITE_REG_QTY 0x7BU

/// Minimum quantity of registers/coils
#define MODBUS_MIN_REG_COIL_QTY 1U

/// Length of Modbus slave address byte
#define MODBUS_SLAVE_ADR_BYTE_LEN 1U

/// Indexes for Modbus frame parameters
#define MODBUS_SLAVE_ADR_IDX 0U
#define MODBUS_PDU_BUF_OFFSET MODBUS_SLAVE_ADR_BYTE_LEN
#define MODBUS_FUNCTION_CODE_IDX MODBUS_PDU_BUF_OFFSET
#define MODBUS_REQUEST_ADR_IDX (MODBUS_PDU_BUF_OFFSET + 1U)
#define MODBUS_REQUEST_QTY_IDX (MODBUS_PDU_BUF_OFFSET + 3U)
#define MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX (MODBUS_PDU_BUF_OFFSET + 3U)
#define MODBUS_REQUEST_BYTE_CNT_IDX (MODBUS_PDU_BUF_OFFSET + 5U)
#define MODBUS_REQUEST_WRITE_MULTI_DATA_IDX (MODBUS_PDU_BUF_OFFSET + 6U)
#define MODBUS_RESP_READ_BYTE_CNT_IDX (MODBUS_PDU_BUF_OFFSET + 1U)
#define MODBUS_RESP_READ_DATA_IDX (MODBUS_PDU_BUF_OFFSET + 2U)
#define MODBUS_RESP_WRITE_ADR_IDX (MODBUS_PDU_BUF_OFFSET + 1U)
#define MODBUS_RESP_WRITE_SINGLE_DATA_IDX (MODBUS_PDU_BUF_OFFSET + 3U)
#define MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX (MODBUS_PDU_BUF_OFFSET + 3U)
#define MODBUS_RESP_EXCEPTION_CODE_IDX (MODBUS_PDU_BUF_OFFSET + 1U)

// Lengths for various Modbus requests and responses
#define MODBUS_READ_REQUEST_LEN 6U
#define MODBUS_WRITE_SINGLE_REQUEST_LEN 6U
#define MODBUS_WRITE_MULTI_REQUEST_LEN 7U 
#define MODBUS_READ_RESP_LEN 3U
#define MODBUS_WRITE_SINGLE_RESP_LEN 6U
#define MODBUS_WRITE_MULTI_RESP_LEN 6U
#define MODBUS_EXCEPTION_CODE_RESP_LEN 3U
#define MODBUS_PDU_EXCEPTION_CODE_LEN 3U
#define MODBUS_EXCEPTION_CODE_MASK 0x80U

    // clang-format on

    /**
     * @brief Enum representing the states of a Modbus coil.
     *
     * This enumeration defines the possible states of a Modbus coil:
     * - COIL_OFF: Indicates that the coil is in the 'off' state (0x0000).
     * - COIL_ON: Indicates that the coil is in the 'on' state (0xFF00).
     */
    typedef enum
    {
        COIL_OFF = 0x0000, ///< Coil is off.
        COIL_ON = 0xFF00,  ///< Coil is on.
    } modbus_w_coil_t;

    /**
     * @brief Enum representing return codes for Modbus internall operations.
     */
    typedef enum
    {
        RET_ERROR_EXCEPTION_CODE_RECIVED = -12,    ///< Error: Exception code received
        RET_ERROR_UNKNOWN_MAPPER_FUN_CODE = -11,   ///< Error: Unknown mapper function code
        RET_NULL_PTR_ERROR = -10,                  ///< Error: Null pointer
        RET_ERROR_WRITE_MULTI_OUT_QTY = -9,        ///< Error: Write multiple out of quantity
        RET_ERROR_WRITE_SINGLE_OUT_VAL = -8,       ///< Error: Write single out of value
        RET_ERROR_WRITE_OUT_ADR = -7,              ///< Error: Write out of address
        RET_ERROR_DATA_TAB_NULL_PTR = -6,          ///< Error: Data table null pointer
        RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH = -5, ///< Error: Request/response function code mismatch
        RET_ERROR_BYTE_CNT = -4,                   ///< Error: Byte count mismatch
        RET_ERROR_CRC = -3,                        ///< Error: CRC error
        RET_ERROR_SLAVE_ID = -2,                   ///< Error: Invalid slave ID
        RET_ERROR = -1,                            ///< General error
        RET_OK = 0,                                ///< Operation successful
    } modbus_ret_t;

    /**
     * @brief Enum representing the states of a Modbus master.
     */
    typedef enum
    {
        MODBUS_MASTER_IDLE,             ///< Master is idle
        MODBUS_MASTER_REPEAT_REQUEST,   ///< Master is repeating a request
        MODBUS_MASTER_TRANSMITTING_REQ, ///< Master is transmitting a request
        MODBUS_MASTER_RESP_WAITING,     ///< Master is waiting for a response
        MODBUS_MASTER_RESP_RECIVED,     ///< Master has received a response
    } modbus_master_state_t;

    /**
     * @brief Enum representing the states of a Modbus slave.
     */
    typedef enum
    {
        MODBUS_SLAVE_IDLE,             ///< Slave is idle
        MODBUS_SLAVE_MSG_RECIVED,      ///< Slave has received a message
        MODBUS_SLAVE_TRANSMITTING_RESP, ///< Slave is transmitting a response
        MODBUS_SLAVE_UNKNOWN,          ///< Slave state is unknown
    } modbus_slave_state_t;

    /**
     * @brief Enum representing the status of Modbus flags.
     */
    typedef enum
    {
        MODBUS_FLAG_CLEARED, ///< Flag is cleared
        MODBUS_FLAG_SET,     ///< Flag is set
        MODBUS_FLAG_UNKNOWN, ///< Flag status is unknown
    } modbus_status_flag_t;

    /**
     * @brief Enum representing the status of the last queue position.
     */
    typedef enum
    {
        LAST_QUEUE_POS_EMPTY,      ///< Last queue position is empty
        LAST_QUEUE_POS_STORE_DATA, ///< Last queue position stores data
    } last_q_pos_status_t;

    typedef uint16_t modbus_data_t;
    typedef uint8_t modbus_req_t;
    typedef uint8_t modbus_byte_count_t;
    typedef uint8_t modbus_buf_t;
    typedef uint8_t modbus_buf_size_t;
    typedef uint32_t modbus_timer_t;
    typedef uint16_t modbus_CRC_t;

    /**
     * @brief Structure representing a Modbus request or response.
     *
     * This structure encapsulates the details of a Modbus request or response, including a
     * pointer to the data buffer and the length of the data.
     */
    typedef struct
    {
        modbus_buf_t *data;    ///< Pointer to the data buffer
        modbus_buf_size_t len; ///< Length of the data
    } modbus_req_resp_t;

    /**
     * @brief Structure representing a Modbus message.
     *
     * This structure encapsulates the components of a Modbus message, including the request,
     * response, and a pointer to the data to be read or written.
     *
     * - The `req` member contains the Modbus request details.
     * - The `resp` member holds the Modbus response details.
     * - The `rw_data_ptr` member is a pointer to the data that will be read from or written to the Modbus device.
     */
    typedef struct
    {
        modbus_req_resp_t req;  ///< Modbus request details
        modbus_req_resp_t resp; ///< Modbus response details
        void *rw_data_ptr;      ///< Pointer to the data to read/write
    } modbus_msg_t;

    /**
     * @brief Function pointer type for Modbus function code handlers.
     */
    typedef modbus_ret_t (*modbus_fun_code_handler_t)(modbus_msg_t *modbus_msg);

    /**
     * @brief Structure representing a mapping between Modbus function codes and their handlers.
     *
     * This structure is used to map Modbus function codes to their corresponding handler functions.
     * Each function code is associated with a function pointer that defines the action to be taken
     * when a message with that function code is received.
     *
     * The `fun_code` member holds the Modbus function code, and the `fun_code_action` member holds
     * the function pointer to the handler that processes the corresponding Modbus message.
     *
     * Example usage:
     * @code
     * modbus_function_mapper_t mapper = {
     *     .fun_code = READ_HOLDING_REGISTERS,
     *     .fun_code_action = handle_read_holding_registers
     * };
     * @endcode
     */
    typedef struct
    {
        modbus_fun_code_t fun_code;                ///< Modbus function code
        modbus_fun_code_handler_t fun_code_action; ///< Function pointer to the handler for the defined function code
    } modbus_function_mapper_t;

    /**
     * @brief Structure representing the parameters for a Modbus request.
     *
     * This structure encapsulates the parameters required to formulate a Modbus request. It includes
     * details such as the function code, address, quantity of data objects, slave ID, and a pointer to the data to be read or written.
     *
     * The `fun_code` member specifies the Modbus function code to be used for the request. The `adr` member indicates
     * the address of the Modbus object to be accessed. The `obj_qty` member denotes the quantity of Modbus data objects
     * to be read or written. The `slave_ID` member holds the ID of the Modbus slave device to communicate with.
     * The `rw_data_ptr` member is a pointer to the data that will be read from or written to the Modbus device.
     *
     * Example usage:
     * @code
     * req_input_param_struct_t request_params = {
     *     .fun_code = READ_HOLDING_REGISTERS,
     *     .adr = 0x0001,
     *     .obj_qty = 10,
     *     .slave_ID = 1,
     *     .rw_data_ptr = buffer
     * };
     * @endcode
     */
    typedef struct
    {
        modbus_fun_code_t fun_code;  ///< Modbus function code
        modbus_adr_t adr;            ///< Address of the Modbus object to read/write
        modbus_data_qty_t obj_qty;   ///< Quantity of Modbus data objects to read/write
        modbus_device_ID_t slave_ID; ///< ID of the Modbus slave device
        void *rw_data_ptr;           ///< Pointer to the data to read/write
    } req_input_param_struct_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_TYPE_H_ */