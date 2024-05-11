/**
 * @file modbus_crc.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _MODBUS_CRC_H_
#define _MODBUS_CRC_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"

uint16_t calculate_CRC(uint8_t *data_buf, uint32_t data_buf_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_CRC_H_ */