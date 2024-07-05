/**
 * @file modbus_slave_PDU.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Header file for Modbus Slave Protocol Data Unit (PDU) handling.
 * @date 2024-05-30
 *
 * This header file defines functions and data structures related to handling
 * Modbus Slave Protocol Data Units (PDUs). It provides functions to parse Modbus
 * master requests and prepare appropriate responses based on the received request.
 *
 * The main function declared in this file is `parse_master_request_and_prepare_resp`,
 * which processes the Modbus master request and prepares the response. It checks for
 * NULL pointers in the message structure and validates the integrity of the request
 * and response data buffers. Upon successful validation, it identifies the function
 * code from the request and invokes the corresponding function handler to process
 * the request and prepare the response.
 *
 * If the function code in the request is not supported (not found in the function
 * mapper), it sets the appropriate exception code in the response and returns an
 * error status.
 *
 * @note This header file depends on `modbus_slave_data.h` for data structure definitions
 *       related to Modbus slave operations.
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
#include "modbus_slave_data.h"

    modbus_ret_t parse_master_request_and_prepare_resp(modbus_msg_t *rx_msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_SLAVE_PDU_H_ */