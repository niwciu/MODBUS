/**
 * @file modbus_unit_test_config.h
 * @brief Defines configuration constants for a Modbus master application.
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-07
 * @copyright Copyright (c) 2024
 *
 *
 * @note Modify these constants according to your specific application requirements and testing needs.
 */
#ifndef _MODBUS_UNIT_TEST_CONFIG_H_
#define _MODBUS_UNIT_TEST_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    // clang-format off

#ifdef UNIT_TEST
#define MAIN_APP_COILS_QTY MODBUS_MAX_READ_COILS_QTY + 1                    /**< Quantity of coils for unit testing. */
#define MAIN_APP_DISCRET_INPUTS_QTY MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1 /**< Quantity of discrete inputs for unit testing. */
#define MAIN_APP_INPUT_REG_QTY MODBUS_MAX_READ_REG_QTY + 1                  /**< Quantity of input registers for unit testing. */
#define MAIN_APP_HOLDING_REG_QTY MODBUS_MAX_READ_REG_QTY + 1                /**< Quantity of holding registers for unit testing. */
#endif

    // clang-format on

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_UNIT_TEST_CONFIG_H_ */