/**
 * @file modbus_config.h
 * @brief Defines configuration constants for a Modbus master application.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-07
 * @copyright Copyright (c) 2024
 *
 * This header file defines various configuration constants used by a Modbus master application,
 * including maximum message queue items, response timeout, and retry behavior on errors.
 *
 * The configuration constants include default quantities for coils, discrete inputs, input registers,
 * and holding registers. For unit testing purposes, these quantities are adjusted when UNIT_TEST
 * is defined.
 *
 * @note Modify these constants according to your specific application requirements and testing needs.
 *
 * @warning RAM footprint estimate with default values on a 32-bit target:
 *
 *   Master-only build:
 *     MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS * 2 * 256 = 10 * 2 * 256 = 5120 B  (RTU buffers)
 *     Minimum recommended target RAM: 8 KB.
 *     To reduce: lower MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS.
 *
 *   Slave-only build:
 *     (COILS + DIN + INREG + HREG) * sizeof(void*) = 4 * 100 * 4 = 1600 B  (pointer tables)
 *     2 * 256                                                      =  512 B  (RTU buffers)
 *                                                                  --------
 *                                                                  ~2.1 KB total static RAM
 *     Minimum recommended target RAM: 4 KB.
 *     To reduce: lower MAIN_APP_*_QTY values.
 *
 *   Smaller targets (e.g. ATmega328P with 2 KB RAM) are supported provided
 *   MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS and MAIN_APP_*_QTY are reduced accordingly.
 *   With default values above, neither build fits in 2 KB RAM.
 */
#ifndef _MODBUS_CONFIG_H_
#define _MODBUS_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    // clang-format off

#define MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS  10              /**< Maximum Modbus message queue items. */
#define MODBUS_MASTER_RESP_TIME_OUT_MS 20               /**< Modbus master response timeout in milliseconds. */
#define MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR 1           /**< Number of retries on any Modbus master error. */

#ifndef UNIT_TEST
#define MAIN_APP_COILS_QTY              100                                      /**< Default quantity of coils. */
#define MAIN_APP_DISCRET_INPUTS_QTY     100                                      /**< Default quantity of discrete inputs. */
#define MAIN_APP_INPUT_REG_QTY          100                                      /**< Default quantity of input registers. */
#define MAIN_APP_HOLDING_REG_QTY        100                                      /**< Default quantity of holding registers. */
#endif

    // clang-format on

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_CONFIG_H_ */