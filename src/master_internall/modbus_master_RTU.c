/**
 * @file modbus_master_RTU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_slave_RTU.h"

modbus_buf_t master_RTU_req_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE] = {0};
modbus_buf_t master_RTU_resp_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE] = {0};