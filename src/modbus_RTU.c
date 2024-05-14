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

void modbus_RTU_send(modbus_buf_t * buf, modbus_buf_size_t data_len, modbus_device_ID_t slave_ID)
{
    buf[MODBUS_SLAVE_ADR_IDX]=slave_ID;
}