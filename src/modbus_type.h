/**
 * @file modbus_type.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_TYPE_H_
#define _MODBUS_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include <stdint.h>
#include <stdbool.h>
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
    #define MODBUS_SLAVE_ADR_BYTE_LEN           1

    // modbus_frame osffsets parameters
    #define MODBUS_SLAVE_ADR_IDX                0    
    #define MODBUS_PDU_BUF_OFFSET                   MODBUS_SLAVE_ADR_BYTE_LEN
    #define MODBUS_FUNCTION_CODE_IDX                MODBUS_PDU_BUF_OFFSET
    #define MODBUS_REQUEST_ADR_IDX                  (MODBUS_PDU_BUF_OFFSET + 1)
    #define MODBUS_REQUEST_LEN_IDX                  (MODBUS_PDU_BUF_OFFSET + 3)
    #define MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX    (MODBUS_PDU_BUF_OFFSET + 3)
    #define MODBUS_REQUEST_BYTE_CNT_IDX             (MODBUS_PDU_BUF_OFFSET + 5)
    #define MODBUS_REQUEST_WRITE_MULTI_DATA_IDX     (MODBUS_PDU_BUF_OFFSET + 6)

    #define MODBUS_RESP_READ_BYTE_CNT_IDX           (MODBUS_PDU_BUF_OFFSET + 1)
    #define MODBUS_RESP_READ_DATA_IDX               (MODBUS_PDU_BUF_OFFSET + 2)
    
    #define MODBUS_RESP_WRITE_ADR_IDX                   (MODBUS_PDU_BUF_OFFSET + 1)
    #define MODBUS_RESP_WRITE_SINGLE_DATA_IDX           (MODBUS_PDU_BUF_OFFSET + 3)
    #define MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX     (MODBUS_PDU_BUF_OFFSET + 3)
    // #define MODBUS_WRITE_MULTI_DATA_IDX    

    #define MODBUS_READ_REQUEST_LEN            6
    #define MODBUS_WRITE_SINGLE_REQUEST_LEN    6
    #define MODBUS_WRITE_MULTI_REQUEST_LEN     7 

    #define MODBUS_READ_RESP_LEN    3
    #define MODBUS_WRITE_SINGLE_RESP_LEN  6
    #define MODBUS_WRITE_MULTI_RESP_LEN   6
    #define MODBUS_ERROR_CODE_RESP_LEN    3
    // clang-format on
    typedef enum
    {
        MODBUS_READ_COILS_FUNC_CODE = 0x01U,
        MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE = 0x02U,
        MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE = 0x03U,
        MODBUS_READ_INPUT_REGISTERS_FUNC_CODE = 0x04U,
        MODBUS_WRITE_SINGLE_COIL_FUNC_CODE = 0x05U,
        MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE = 0x06U,
        MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE = 0x0FU,
        MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE = 0x10U,

        MODBUS_ERROR_CODE_MASK = 0x80U,
    } modbus_fun_code_t;

    typedef enum
    {
        COIL_OFF = 0x0000,
        COIL_ON = 0xFF00,
    } modbus_w_coil_t;

    typedef enum
    {
        RET_ERROR_WRITE_SINGLE_OUT_VAL = -8,
        RET_ERROR_WRITE_SINGLE_OUT_ADR = -7,
        RET_ERROR_DATA_TAB_NULL_PTR = -6,
        RET_ERROR_FUN_CODE= -5,
        RET_ERROR_BYTE_CNT= -4,
        RET_ERROR_CRC = -3,
        RET_ERROR_SLAVE_ID = -2,
        RET_ERROR = -1,
        RET_OK = 0,
    } modbus_ret_t;

    typedef uint16_t modbus_adr_t;
    typedef uint16_t modbus_reg_t;
    typedef uint16_t modbus_data_qty_t;
    typedef uint16_t modbus_data_t;
    typedef uint8_t modbus_req_t;
    typedef uint8_t modbus_byte_count_t;
    typedef uint8_t modbus_buf_t;
    typedef uint8_t modbus_buf_size_t;
    typedef uint8_t modbus_device_ID_t;
    typedef uint16_t modbus_CRC_t;
    typedef bool modbus_coil_disin_t;
    // typedef bool modbus_coil_disin_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_TYPE_H_ */