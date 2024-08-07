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
 */
#ifndef _MODBUS_CONFIG_H_
#define _MODBUS_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    // clang-format off

#define MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS  5              /**< Maximum Modbus message queue items. */
#define MODBUS_MASTER_RESP_TIME_OUT_MS 1000               /**< Modbus master response timeout in milliseconds. */
#define MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR 3           /**< Number of retries on any Modbus master error. */

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