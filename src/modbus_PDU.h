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

#define READ_DISCRETE_INPUTS 0x02
#define READ_COILS 0x01
#define WRITE_SINGLE_COIL 0x05
#define WRITE_MULTIPLE_COILS 0x0F

#define READ_INPUT_REGISTERS 0x04
#define READ_HOLDING_REGISTERS 0x03
#define WRITE_SINGLE_REGISTER 0x06
#define WRITE_MULTIPLE_REGISTER 0x10

#define MODBUS_PDU_FRAME_LEN 128

typedef uint8_t modbus_fun_code_t;
typedef uint16_t modbus_adr_t; 
typedef uint16_t modbus_reg_t;
typedef uint16_t modbus_data_qty_t;
typedef uint8_t modbus_byte_count_t;
typedef uint8_t modbus_error_code_t;
typedef uint8_t modbus_exeption_code_t;


void modbus_master_read_holding_reg(uint8_t *send_buf,modbus_adr_t adr, modbus_data_qty_t len);
void modbus_master_read_input_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len);
void modbus_master_read_discrete_inputs(uint8_t *send_buf, modbus_adr_t adr, modbus_data_qty_t len);


void modbus_master_write_single_reg(uint8_t *send_buf, modbus_adr_t adr, modbus_reg_t val);
void modbus_master_write_multiple_reg(uint8_t *send_buf,modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_reg_t *data_buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */