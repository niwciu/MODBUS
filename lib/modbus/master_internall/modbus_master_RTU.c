/**
 * @file modbus_master_RTU.c
 * @brief Implementation file for Modbus Master using RTU (Remote Terminal Unit) protocol.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-06-28
 * @copyright Copyright (c) 2024
 *
 * This file contains the implementation of a Modbus master for communication over RTU,
 * a serial communication protocol widely used in industrial automation. It provides
 * functions to construct Modbus requests, handle Modbus responses, and manage
 * communication buffers for both requests and responses.
 *
 * The implementation includes support for Modbus function codes such as reading coils,
 * discrete inputs, holding registers, and input registers. It also includes functions
 * for writing single coils and single registers, as well as multiple coils and registers.
 *
 * @note This implementation assumes adherence to the Modbus RTU protocol specifications.
 *       It includes error handling for null pointer checks, parameter validation, and
 *       processing of Modbus exceptions returned by slave devices.
 *
 * @remark The file defines static buffers `master_RTU_req_buf` and `master_RTU_resp_buf`
 *         for storing Modbus messages in RTU format. These buffers facilitate asynchronous
 *         message handling and management of multiple Modbus transactions.
 */

#include "modbus_master_RTU.h"

modbus_buf_t master_RTU_req_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE] = {0};
modbus_buf_t master_RTU_resp_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS][MODBUS_RTU_BUFFER_SIZE] = {0};