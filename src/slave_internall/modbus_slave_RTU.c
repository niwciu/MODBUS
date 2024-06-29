/**
 * @file modbus_slave_RTU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_slave_RTU.h"

modbus_buf_t slave_RTU_req_buf[MODBUS_RTU_BUFFER_SIZE] = {0};
modbus_buf_t slave_RTU_resp_buf[MODBUS_RTU_BUFFER_SIZE] = {0};