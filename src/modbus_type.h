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
    typedef enum
    {
        MODBUS_READ_COILS_FUNCTTION_CODE = 0x01U,
        READ_DISCRETE_INPUTS = 0x02U,
        READ_HOLDING_REGISTERS = 0x03U,
        READ_INPUT_REGISTERS = 0x04U,
        WRITE_SINGLE_COIL = 0x05U,
        WRITE_SINGLE_REGISTER = 0x06U,
        WRITE_MULTIPLE_COILS = 0x0FU,
        WRITE_MULTIPLE_REGISTER = 0x10U,
    } modbus_fun_code_t;

    typedef enum
    {
        COIL_OFF = 0x0000,
        COIL_ON = 0xFF00,
    } modbus_coil_t;

    typedef enum
    {
        RET_ERROR = -1,
        RET_OK = 0,
    } modbus_ret_t;

    typedef uint16_t modbus_adr_t;
    typedef uint16_t modbus_reg_t;
    typedef uint16_t modbus_data_qty_t;
    typedef uint16_t modbus_data_t;
    typedef uint8_t modbus_req_t;
    typedef uint8_t modbus_byte_count_t;
    // typedef uint8_t modbus_byte_count_t;
    // typedef uint8_t modbus_error_code_t;
    // typedef uint8_t modbus_exeption_code_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_TYPE_H_ */