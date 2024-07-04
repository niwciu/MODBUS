/**
 * @file modbus_RTU.c
 * @brief Implementation file for Modbus RTU communication protocol.
 *
 * This file implements functions declared in modbus_RTU.h for sending and receiving Modbus messages
 * using the RTU (Remote Terminal Unit) communication protocol. It includes functions to send a Modbus
 * RTU message with CRC and receive a Modbus RTU message with CRC validation.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-14
 * @version 0.0.1
 *
 * @copyright Copyright (c) 2024
 */
#include "modbus_RTU.h"
#include "modbus_crc.h"
#include "buf_rw.h"
#include <stdio.h>

/**
 * @brief Sends a Modbus RTU message.
 *
 * Constructs a Modbus RTU message by appending CRC to the message buffer.
 *
 * @param buf Pointer to the buffer containing the Modbus message.
 * @param msg_len Pointer to the size of the message buffer, updated to include CRC.
 * @param slave_ID ID of the Modbus slave device.
 * @return modbus_ret_t Returns RET_OK if successful, otherwise RET_ERROR.
 */
modbus_ret_t modbus_RTU_send(modbus_buf_t *buf, modbus_buf_size_t *msg_len, modbus_device_ID_t slave_ID)
{

    if ((NULL != buf) && (MODBUS_PDU_MAX_LEN >= *msg_len))
    {
        modbus_CRC_t CRC;
        buf[MODBUS_SLAVE_ADR_IDX] = slave_ID;

        CRC = calculate_CRC(buf, *msg_len);
        buf[*msg_len] = CRC & 0x00FF;
        buf[*msg_len + 1] = CRC >> 8;

        *msg_len = *msg_len + 2;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
    return RET_OK;
}

/**
 * @brief Receives and validates a Modbus RTU message.
 *
 * Receives and validates a Modbus RTU message by checking CRC and slave ID.
 *
 * @param buf Pointer to the buffer containing the received Modbus message.
 * @param msg_len Length of the received Modbus message including CRC.
 * @param slave_ID ID of the Modbus slave device.
 * @return - RET_OK - if successful,
 * @return - RET_ERROR_SLAVE_ID - if resp cointain wrong slave ID on RTU layer
 * @return - RET_ERROR_CRC - if Calculated CRC from the recv msg is different than CRC written in recv msg.
 */
modbus_ret_t modbus_RTU_recv(modbus_buf_t *buf, modbus_buf_size_t msg_len, modbus_device_ID_t slave_ID)
{
    if (slave_ID != buf[MODBUS_SLAVE_ADR_IDX])
    {
        return RET_ERROR_SLAVE_ID;
    }
    else if (0 != calculate_CRC(buf, msg_len))
    {
        return RET_ERROR_CRC;
    }
    else
        return RET_OK;
}