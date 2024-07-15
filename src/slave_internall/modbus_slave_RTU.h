/**
 * @file modbus_slave_RTU.h
 * @brief Header file for Modbus RTU slave functionality.
 * @author niwciu (niwciu@gmail.com)
 * @version 0.0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_SLAVE_RTU_H_
#define _MODBUS_SLAVE_RTU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"

    extern modbus_buf_t slave_RTU_req_buf[MODBUS_RTU_BUFFER_SIZE];
    extern modbus_buf_t slave_RTU_resp_buf[MODBUS_RTU_BUFFER_SIZE];

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_RTU_H_ */