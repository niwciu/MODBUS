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
 * @brief Generates a Modbus RTU message ready to send.
 *
 * This function constructs a Modbus RTU message by appending the Modbus slave
 * address and CRC to the message buffer. It also sets the correct length
 * of the entire RTU message, ensuring it is ready for transmission.
 *
 * @param[in,out] buf Pointer to the buffer containing the Modbus PDU message.
 *                    The buffer is updated with the slave address and CRC.
 * @param[in,out] msg_len Pointer to the current size of the message buffer,
 *                        updated to include the CRC.
 * @param[in] slave_ID ID of the Modbus slave device.
 * @return modbus_ret_t The status of the message construction.
 *
 * @retval RET_OK The message was successfully constructed and is ready to send.
 * @retval RET_ERROR An error occurred, such as a NULL buffer pointer or an
 *                   invalid message length.
 *
 * @note This function requires the additional `modbus_crc` module for calculating
 *       the CRC to be placed in the message.
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
 * This function receives a Modbus RTU message and validates it by checking the CRC
 * and the slave ID. It ensures the integrity and correctness of the received message.
 *
 * @param[in] buf Pointer to the buffer containing the received Modbus message.
 * @param[in] msg_len Length of the received Modbus message, including the CRC.
 * @param[in] slave_ID ID of the Modbus slave device to validate against.
 * @return modbus_ret_t The result of the validation.
 *
 * @retval RET_OK The message was successfully received and validated.
 * @retval RET_ERROR_SLAVE_ID The received message contains an incorrect slave ID / slave address.
 * @retval RET_ERROR_CRC The calculated CRC from the received message is different from the CRC in the received message.
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