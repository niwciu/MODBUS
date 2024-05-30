/**
 * @file buf_rw.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "buf_rw.h"

void write_u16_to_buf(uint8_t *buf, uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);
    buf[1] = (uint8_t)(data & 0xFF);
}

uint16_t read_u16_from_buf(const uint8_t *buf)
{
    return (uint16_t)((buf[0] << 8) | buf[1]);
}