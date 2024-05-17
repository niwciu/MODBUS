/**
 * @file modbus_data.h
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MODBUS_DATA_H_
#define _MODBUS_DATA_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include "modbus_type.h"
#include "modbus_config.h"

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

extern modbus_reg_t *Slave_Holding_Registers[HOLDING_REG_QTY];
extern modbus_reg_t *Slave_Input_Registers[INPUT_REG_QTY];
extern modbus_coil_t *Slave_Coils[COILS_QTY];
extern modbus_disin_t *Slave_Discrete_Inputs[DISCRET_INPUT_QTY];

extern modbus_reg_t *Master_Holding_Registers[HOLDING_REG_QTY];
extern modbus_reg_t *Master_Input_Registers[INPUT_REG_QTY];
extern modbus_coil_t *Master_Coils[COILS_QTY];
extern modbus_disin_t *Master_Discrete_Inputs[DISCRET_INPUT_QTY];

modbus_coil_t get_coil_state(modbus_coil_t **coils_tab_ptr,modbus_adr_t coil_adr);
modbus_ret_t set_coil_state(modbus_coil_t **coils_tab_ptr,modbus_adr_t coil_adr, modbus_coil_t coil_state);

modbus_disin_t get_discrete_input_state(modbus_disin_t **disin_tab_ptr,modbus_adr_t din_adr);

modbus_reg_t get_input_register_state(modbus_reg_t **inreg_tab_ptr,modbus_adr_t input_reg_adr);

modbus_reg_t get_holding_register_value(modbus_reg_t **hreg_tab_ptr,modbus_adr_t holding_reg_adr);
modbus_ret_t set_holding_register_value(modbus_reg_t **hreg_tab_ptr,modbus_adr_t holding_reg_adr, modbus_ret_t hreg_val);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_DATA_H_ */