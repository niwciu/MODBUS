/**
 * @file modbus_config.h
 * @brief ATmega328P-specific Modbus configuration.
 *
 * This file shadows the library default modbus_config.h.
 * Values are tuned to fit within ATmega328P's 2 KB RAM.
 * RTU buffer footprint: MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS * 2 * 256 = 2 * 2 * 256 = 1024 B
 */
#ifndef _MODBUS_CONFIG_H_
#define _MODBUS_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS     2  /**< Reduced for ATmega328P 2 KB RAM. */
#define MODBUS_MASTER_RESP_TIME_OUT_MS       20  /**< Modbus master response timeout in milliseconds. */
#define MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR 1  /**< Number of retries on any Modbus master error. */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_CONFIG_H_ */
