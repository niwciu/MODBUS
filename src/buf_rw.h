/**
 * @file buf_rw.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _BUF_RW_H_
#define _BUF_RW_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
// #include "modbus_type.h"
#include <stdint.h>

void write_u16_to_buf(uint8_t *buf, uint16_t data);
uint16_t read_u16_from_buf(const uint8_t *buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _BUF_RW_H_ */