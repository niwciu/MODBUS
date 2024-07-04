/**
 * @file modbus_type.h
 * @brief Header file defining types and constants for Modbus protocol implementation.
 *
 * This header file defines various types, constants, enums, and structures used in the
 * implementation of the Modbus protocol. It includes definitions for Modbus RTU buffer sizes,
 * maximum data quantities, offsets for Modbus frame parameters, error codes, communication
 * states, flags, and data types like Modbus messages and function handlers.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-07
 * @version 1.0.0
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
    // taken from modsbus frame buffer size specification
    #define MODBUS_RTU_BUFFER_SIZE 256U
    #define MODBUS_PDU_MAX_LEN 253U

    // max and min allowed values taken from Modbus standard documentation
    #define MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY  65535U 

    #define MODBUS_MAX_READ_DISCRETE_INPUTS_QTY 0x7D0U
    #define MODBUS_MAX_READ_COILS_QTY           0x7D0U
    #define MODBUS_MAX_WRITE_COILS_QTY          0x7B0U
    #define MODBUS_MAX_READ_REG_QTY             0x7DU
    #define MODBUS_MAX_WRITE_REG_QTY            0x7BU
    #define MODBUS_MIN_REG_COIL_QTY             1U
    #define MODBUS_SLAVE_ADR_BYTE_LEN           1U

    // modbus_frame osffsets parameters
    #define MODBUS_SLAVE_ADR_IDX                    0U    
    #define MODBUS_PDU_BUF_OFFSET                   MODBUS_SLAVE_ADR_BYTE_LEN
    #define MODBUS_FUNCTION_CODE_IDX                MODBUS_PDU_BUF_OFFSET
    #define MODBUS_REQUEST_ADR_IDX                  (MODBUS_PDU_BUF_OFFSET + 1U)
    #define MODBUS_REQUEST_QTY_IDX                  (MODBUS_PDU_BUF_OFFSET + 3U)
    #define MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX    (MODBUS_PDU_BUF_OFFSET + 3U)
    #define MODBUS_REQUEST_BYTE_CNT_IDX             (MODBUS_PDU_BUF_OFFSET + 5U)
    #define MODBUS_REQUEST_WRITE_MULTI_DATA_IDX     (MODBUS_PDU_BUF_OFFSET + 6U)

    #define MODBUS_RESP_READ_BYTE_CNT_IDX           (MODBUS_PDU_BUF_OFFSET + 1U)
    #define MODBUS_RESP_READ_DATA_IDX               (MODBUS_PDU_BUF_OFFSET + 2U)
    
    #define MODBUS_RESP_WRITE_ADR_IDX               (MODBUS_PDU_BUF_OFFSET + 1U)
    #define MODBUS_RESP_WRITE_SINGLE_DATA_IDX       (MODBUS_PDU_BUF_OFFSET + 3U)
    #define MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX (MODBUS_PDU_BUF_OFFSET + 3U)
    #define MODBUS_RESP_EXCEPTION_CODE_IDX          (MODBUS_PDU_BUF_OFFSET + 1U)
    // #define MODBUS_WRITE_MULTI_DATA_IDX    

    #define MODBUS_READ_REQUEST_LEN            6U
    #define MODBUS_WRITE_SINGLE_REQUEST_LEN    6U
    #define MODBUS_WRITE_MULTI_REQUEST_LEN     7U 

    #define MODBUS_READ_RESP_LEN    3U
    #define MODBUS_WRITE_SINGLE_RESP_LEN  6U
    #define MODBUS_WRITE_MULTI_RESP_LEN   6U
    #define MODBUS_EXCEPTION_CODE_RESP_LEN 3U
    #define MODBUS_PDU_EXCEPTION_CODE_LEN 3U
    #define MODBUS_EXCEPTION_CODE_MASK  0x80U

    // clang-format on
    typedef enum
    {
        COIL_OFF = 0x0000,
        COIL_ON = 0xFF00,
    } modbus_w_coil_t;

    typedef enum
    {
        RET_ERROR_EXCEPTION_CODE_RECIVED = -12,
        RET_ERROR_UNKNOWN_MAPPER_FUN_CODE = -11,
        RET_NULL_PTR_ERROR = -10,
        RET_ERROR_WRITE_MULTI_OUT_QTY = -9,
        RET_ERROR_WRITE_SINGLE_OUT_VAL = -8,
        RET_ERROR_WRITE_OUT_ADR = -7,
        RET_ERROR_DATA_TAB_NULL_PTR = -6,
        RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH = -5, // used in modbusread slave resp
        RET_ERROR_BYTE_CNT = -4,
        RET_ERROR_CRC = -3,
        RET_ERROR_SLAVE_ID = -2,
        RET_ERROR = -1,
        RET_OK = 0,
    } modbus_ret_t;

    typedef enum
    {
        MODBUS_MASTER_IDLE,
        MODBUS_MASTER_REPEAT_REQUEST,
        MODBUS_MASTER_TRANSMITTING_REQ,
        MODBUS_MASTER_RESP_WAITING,
        MODBUS_MASTER_RESP_RECIVED,
    } modbus_master_state_t;

    typedef enum
    {
        MODBUS_SLAVE_IDLE,
        MODBUS_SLAVE_MSG_RECIVED,
        MODBUS_SLAVE_TRANSMITING_RESP,
        MODBUS_SLAVE_UNKNOWN,
    } modbus_slave_state_t;
    typedef enum
    {
        MODBUS_FLAG_CLEARED,
        MODBUS_FLAG_SET,
        MODBUS_FLAG_UNKNOWN,

    } modbus_status_flag_t;
    typedef enum
    {
        LAST_QUEUE_POS_EMPTY,
        LAST_QUEUE_POS_STORE_DATA,
    } last_q_pos_status_t;

    typedef uint16_t modbus_data_t;
    typedef uint8_t modbus_req_t;
    typedef uint8_t modbus_byte_count_t;
    typedef uint8_t modbus_buf_t;
    typedef uint8_t modbus_buf_size_t;
    typedef uint32_t modbus_timer_t;
    typedef uint16_t modbus_CRC_t;

    typedef struct
    {
        modbus_buf_t *data;
        modbus_buf_size_t len;
    } modbus_req_resp_t;

    typedef struct
    {
        modbus_req_resp_t req;
        modbus_req_resp_t resp;
        void *rw_data_ptr;
    } modbus_msg_t;

    typedef modbus_ret_t (*modbus_fun_code_handler_t)(modbus_msg_t *modbus_msg);
    typedef struct
    {
        modbus_fun_code_t fun_code;
        modbus_fun_code_handler_t fun_code_action;
    } modbus_function_mapper_t;

    typedef struct
    {
        modbus_fun_code_t fun_code;
        modbus_adr_t adr;
        modbus_data_qty_t obj_qty;
        modbus_device_ID_t slave_ID;
        void *rw_data_ptr;
    } req_input_param_struct_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_TYPE_H_ */