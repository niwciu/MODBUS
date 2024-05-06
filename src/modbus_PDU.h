/**
 * @file modbus_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_PDU_H_
#define _MODBUS_PDU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>

#define MODBUS_PDU_FRAME_LEN 256
#define MODBUS_MAX_DISCRETE_INPUTS_READ_QTY 0x7D0
#define MODBUS_MAX_COILS_READ_QTY MODBUS_MAX_DISCRETE_INPUTS_READ_QTY
#define MODBUS_MAX_REG_RW_QTY 0x7B

    typedef enum
    {
        READ_COILS = 0x01,
        READ_DISCRETE_INPUTS = 0x02,
        READ_HOLDING_REGISTERS = 0x03,
        READ_INPUT_REGISTERS = 0x04,
        WRITE_SINGLE_COIL = 0x05,
        WRITE_SINGLE_REGISTER = 0x06,
        WRITE_MULTIPLE_COILS = 0x0F,
        WRITE_MULTIPLE_REGISTER = 0x10,
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
    // typedef uint8_t modbus_byte_count_t;
    // typedef uint8_t modbus_error_code_t;
    // typedef uint8_t modbus_exeption_code_t;

    modbus_ret_t modbus_master_read_holding_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty);
    modbus_ret_t modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty);
    modbus_ret_t modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty);
    modbus_ret_t modbus_master_read_coils(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty);

    void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val);
    void modbus_master_write_single_coil(uint8_t *send_buf, modbus_adr_t adr, modbus_coil_t coil_state);
    modbus_ret_t modbus_master_write_multiple_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_reg_t *data_buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */