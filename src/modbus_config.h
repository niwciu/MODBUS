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
#define MAIN_APP_COILS_QTY              MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define MAIN_APP_DISCRET_INPUTS_QTY     MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define MAIN_APP_INPUT_REG_QTY          MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY
#define MAIN_APP_HOLDING_REG_QTY        MODBUS_MAX_SPECIFIC_DATA_ITEMS_QTY

// clang-format on

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_CONFIG_H_ */