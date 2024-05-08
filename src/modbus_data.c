/**
 * @file modbus_data.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_type.h"
#include "modbus_config.h"
#include "modbus_PDU.h"

#if MAIN_APP_HOLDING_REG_QTY < MODBUS_MAX_REG_QTY
#define HOLDING_REG_QTY MAIN_APP_HOLDING_REG_QTY
#else
#define HOLDING_REG_QTY MODBUS_MAX_REG_QTY
#endif

#if MAIN_APP_INPUT_REG_QTY < MODBUS_MAX_REG_QTY
#define INPUT_REG_QTY (MAIN_APP_INPUT_REG_QTY)
#else
#define INPUT_REG_QTY (MODBUS_MAX_REG_QTY)
#endif

#if MAIN_APP_COILS_QTY < MODBUS_MAX_COILS_QTY
#define COILS_QTY (MAIN_APP_COILS_QTY)
#else
#define COILS_QTY (MODBUS_MAX_COILS_QTY)
#endif

#if MAIN_APP_DISCRET_INPUTS_QTY < MODBUS_MAX_DISCRETE_INPUTS_QTY
#define DISCRET_INPUT_QTY (MAIN_APP_DISCRET_INPUTS_QTY)
#else
#define DISCRET_INPUT_QTY (MODBUS_MAX_DISCRETE_INPUTS_QTY)
#endif

modbus_reg_t *Holding_Registers[HOLDING_REG_QTY] = {NULL};
modbus_reg_t *Input_Registers[INPUT_REG_QTY] = {NULL};
modbus_coil_t *Coils[COILS_QTY] = {NULL};
modbus_disin_t *Discrete_Inputs[DISCRET_INPUT_QTY] = {NULL};