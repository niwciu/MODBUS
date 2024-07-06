/**
 * @file modbus_config.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_CONFIG_H_
#define _MODBUS_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    // clang-format off

#define MAX_MODBUS_MSG_QUEUE_ITEMS  2                     /**< Maximum Modbus message queue items. */
#define MODBUS_MASTER_RESP_TIME_OUT_MS 1000               /**< Modbus master response timeout in milliseconds. */
#define MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR 0           /**< Number of retries on any Modbus master error. */

#ifdef UNIT_TEST
#define MAIN_APP_COILS_QTY              MODBUS_MAX_READ_COILS_QTY + 1            /**< Quantity of coils for unit testing. */
#define MAIN_APP_DISCRET_INPUTS_QTY     MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1  /**< Quantity of discrete inputs for unit testing. */
#define MAIN_APP_INPUT_REG_QTY          MODBUS_MAX_READ_REG_QTY + 1              /**< Quantity of input registers for unit testing. */
#define MAIN_APP_HOLDING_REG_QTY        MODBUS_MAX_READ_REG_QTY + 1              /**< Quantity of holding registers for unit testing. */
#endif

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