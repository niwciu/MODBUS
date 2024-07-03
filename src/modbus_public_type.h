/**
 * @file modbus_public_type.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_PUBLIC_TYPE_H_
#define _MODBUS_PUBLIC_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include <stdint.h>
#include <stdbool.h>
    typedef enum
    {
        RTU,
        UNKNOWN_MODE
    } modbus_mode_t;

    typedef enum
    {
        NONE,
        ODD,
        EVEN
    } parity_t;
    typedef enum
    {
        MODBUS_MASTER_REQUEST_SEND,
        MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR,
        MODBUS_MASTER_LIB_REQ_ERROR,
        MODBUS_MASTER_LIB_RTU_SEND_ERROR,
    } modbus_master_error_t;

    typedef enum
    {
        MODBUS_FLAG_CLEARED,
        MODBUS_FLAG_SET,
        MODBUS_FLAG_UNKNOWN,

    } modbus_status_flag_t;

    typedef enum
    {
        MODBUS_ILLEGAL_FUNCTION_ERROR = 0x01U,
        MODBUS_ILLEGAL_DATA_ADDRESS_ERROR = 0x02U,
        MODBUS_ILLEGAL_DATA_VALUE_ERROR = 0x03U,
        MODBUS_SERVER_DEVICE_FAILURE_ERROR = 0x04U,
    } modbus_exception_code_t;

    typedef uint16_t modbus_adr_t;
    typedef bool modbus_coil_disin_t;
    typedef uint16_t modbus_reg_t;
    typedef uint16_t modbus_data_qty_t;
    typedef uint8_t modbus_device_ID_t;
    typedef uint32_t baud_t;
   

    typedef struct
    {
        modbus_exception_code_t read_coil;
        modbus_exception_code_t read_dis_in;
        modbus_exception_code_t read_in_reg;
        modbus_exception_code_t read_holding_reg;
        modbus_exception_code_t write_coil;
        modbus_exception_code_t write_multiple_coil;
        modbus_exception_code_t write_reg;
        modbus_exception_code_t write_multiple_reg;
    } modbus_device_exception_t;

    typedef struct
    {
        modbus_status_flag_t MODBUS_DEVICE_NO_RESPOND_ERROR_FLAG;
        modbus_status_flag_t MODBUS_DEVICE_FRAME_ERROR_FLAG;
        modbus_status_flag_t MODBUS_DEVICE_RTU_CRC_ERROR_FLAG;
        modbus_device_exception_t exception_code;
    } modbus_device_error_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PUBLIC_TYPE_H_ */