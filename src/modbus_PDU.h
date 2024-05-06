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
#define READ_DISCRETE_INPUT 0x02
#define READ_COILS 0x01
#define WRITE_SINGLE_COIL 0x05
#define WRITE_MULTIPLE_COILS 0x0F

#define READ_INPUT_REGISTER 0x04
#define READ_HOLDING_REGISTERS 0x03
#define WRITE_SINGLE_REGISTER 0x06
#define WRITE_MULTIPLE_REGISTER 0x10



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PDU_H_ */