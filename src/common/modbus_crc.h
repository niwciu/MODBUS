/**
 * @file modbus_crc.h
 * @brief Header file for Modbus CRC calculation.
 *
 * This header file contains the declaration of the CRC calculation function
 * for Modbus protocol.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-11
 * @copyright Copyright (c) 2024
 */

#ifndef _MODBUS_CRC_H_
#define _MODBUS_CRC_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>

    uint16_t calculate_CRC(uint8_t *data_buf, uint32_t data_buf_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MODBUS_CRC_H_ */