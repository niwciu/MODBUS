/**
 * @file buf_rw.c
 * @brief Implementation file for buffer read and write operations.
 *
 * This file contains the implementations of functions declared in buf_rw.h.
 * These functions facilitate reading and writing 16-bit unsigned integers to/from buffers.
 *
 * @author niwciu (niwciu@gmail.com)
 * @version 0.0.1
 * @date 2024-05-07
 * @copyright Copyright (c) 2024
 */

#include "buf_rw.h"

/**
 * @brief Writes a 16-bit unsigned integer to a buffer.
 *
 * Writes the given 16-bit unsigned integer `data` to the buffer `buf`.
 * The buffer `buf` is assumed to be an array of uint8_t.
 *
 * @param buf Pointer to the buffer where data will be written.
 * @param data 16-bit unsigned integer to be written to the buffer.
 */
void write_u16_to_buf(uint8_t *buf, uint16_t data)
{
    buf[0] = (uint8_t)(data >> 8);   // Most significant byte
    buf[1] = (uint8_t)(data & 0xFF); // Least significant byte
}

/**
 * @brief Reads a 16-bit unsigned integer from a buffer.
 *
 * Reads a 16-bit unsigned integer from the buffer `buf` and returns it.
 * The buffer `buf` is assumed to be an array of const uint8_t.
 *
 * @param buf Pointer to the buffer from which data will be read.
 * @return uint16_t The 16-bit unsigned integer read from the buffer.
 */
uint16_t read_u16_from_buf(const uint8_t *buf)
{
    return (uint16_t)((buf[0] << 8) | buf[1]); // Combine bytes to form uint16_t
}