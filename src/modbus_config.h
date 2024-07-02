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

#define MAX_MODBUS_MSG_QUEUE_ITEMS  2
#define MODBUS_MASTER_RESP_TIME_OUT_MS 1000
#define MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR 3

#ifdef  UNIT_TEST
// belo values cavering conditions to run all tests
#define MAIN_APP_COILS_QTY              MODBUS_MAX_READ_COILS_QTY+1
#define MAIN_APP_DISCRET_INPUTS_QTY     MODBUS_MAX_READ_DISCRETE_INPUTS_QTY+1
#define MAIN_APP_INPUT_REG_QTY          MODBUS_MAX_READ_REG_QTY+1
#define MAIN_APP_HOLDING_REG_QTY        MODBUS_MAX_READ_REG_QTY+1
#endif

#ifndef  UNIT_TEST
#define MAIN_APP_COILS_QTY              100 
#define MAIN_APP_DISCRET_INPUTS_QTY     100 
#define MAIN_APP_INPUT_REG_QTY          100 
#define MAIN_APP_HOLDING_REG_QTY        100 
#endif

    // clang-format on

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_CONFIG_H_ */