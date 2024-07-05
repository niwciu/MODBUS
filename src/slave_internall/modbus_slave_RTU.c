/**
 * @file modbus_slave_RTU.c
 * @brief Implementation of Modbus RTU slave functionality.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-06-28
 * 
 * @copyright Copyright(c) 2024
 */
#include "modbus_slave_RTU.h"

modbus_buf_t slave_RTU_req_buf[MODBUS_RTU_BUFFER_SIZE] = {0};       /**< Buffer to store Modbus RTU request data */
modbus_buf_t slave_RTU_resp_buf[MODBUS_RTU_BUFFER_SIZE] = {0};      /**< Buffer to store Modbus RTU response data */