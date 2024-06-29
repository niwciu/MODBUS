/**
 * @file modbus_slave_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_SLAVE_PDU_H_
#define _MODBUS_SLAVE_PDU_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_data.h"

    // Slave Functionality
    modbus_ret_t parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_PDU_H_ */