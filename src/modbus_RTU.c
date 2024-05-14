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

void modbus_RTU_send(modbus_buf_t * buf, modbus_buf_size_t data_len, modbus_device_ID_t slave_ID)
{
    modbus_CRC_t CRC;
    buf[MODBUS_SLAVE_ADR_IDX]=slave_ID;
    
    CRC = calculate_CRC(buf,data_len);
    buf[data_len]=CRC & 0x00FF;
    buf[data_len+1]= CRC >>8;
}