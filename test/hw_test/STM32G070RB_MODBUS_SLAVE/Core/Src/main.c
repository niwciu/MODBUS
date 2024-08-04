/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "main.h"
#include "core_init.h"
#include "modbus_slave.h"
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

modbus_coil_disin_t coils[20] = {1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1,0,0,1};
modbus_reg_t hreg[20] = {2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,3001,3002,3003,3004,3005,3006,3007,3008,3009,3010};
modbus_reg_t inreg[20] = {4201, 4202, 4203, 4204, 4205, 4206, 4207, 4208, 4209, 4210, 4301, 4302, 4303, 4304, 4305, 4306, 4307, 4308, 4309, 4310};
modbus_device_ID_t Slave_ID = 0x01;

int main(void)
{

  core_init();
  modbus_slave_init(RTU, 115200, NONE, Slave_ID);
  for (int i = 0; i < EXAMPLE_DATA_QTY; i++)
  {
      register_app_data_to_modbus_slave_coils_table(i, &coil[i]);
      register_app_data_to_modbus_slave_hreg_table(i, &hreg[i]);
      register_app_data_to_modbus_slave_inreg_table(i, &inreg[i]);
  }

  /* Loop forever */
  while (1)
  {
      check_modbus_request();
  }

  /* Loop forever */
  while (1)
  {
    check_modbus_request();

    update_LED();
  }
}

