/**
 * @file modbus_RTU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_RTU.h"
#include "modbus_crc.h"
#include "buf_rw.h"
#include <stdio.h>

modbus_buf_t RTU_req_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE];
modbus_buf_t RTU_resp_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE];

modbus_ret_t modbus_RTU_send(modbus_buf_t *buf, modbus_buf_size_t msg_len, modbus_device_ID_t slave_ID)
{

    if ((NULL != buf) && (MODBUS_PDU_MAX_LEN >= msg_len))
    {
        modbus_CRC_t CRC;
        buf[MODBUS_SLAVE_ADR_IDX] = slave_ID;

        CRC = calculate_CRC(buf, msg_len);
        buf[msg_len] = CRC & 0x00FF;
        buf[msg_len + 1] = CRC >> 8;
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

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