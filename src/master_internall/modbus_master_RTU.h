/**
 * @file modbus_master_RTU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_MASTER_RTU_H_
#define _MODBUS_MASTER_RTU_H_

#ifdef __cplusplusr
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"

    extern modbus_buf_t master_RTU_req_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE];
    extern modbus_buf_t master_RTU_resp_buf[MAX_MODBUS_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_MASTER_RTU_H_ */