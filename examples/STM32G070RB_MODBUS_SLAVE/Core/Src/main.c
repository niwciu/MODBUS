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

#include "main.h"
#include "core_init.h"
#include "modbus_slave.h"
#include "LED_driver.h"

#include <stdio.h>
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define EXAMPLE_DATA_QTY 20

modbus_coil_disin_t coil_data[EXAMPLE_DATA_QTY] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
modbus_coil_disin_t din_data[EXAMPLE_DATA_QTY] = {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
modbus_reg_t hreg_data[EXAMPLE_DATA_QTY] = {2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010};
modbus_reg_t inreg_data[EXAMPLE_DATA_QTY] = {60, 61, 62, 4204, 4205, 4206, 4207, 4208, 4209, 4210, 4301, 4302, 4303, 4304, 4305, 4306, 4307, 4308, 4309, 4310};
modbus_device_ID_t Slave_ID = 0x01;

modbus_coil_disin_t LED_status = 0;

static void register_modbus_data(void);
static void update_LED(void);


int main(void)
{
    core_init();
    LED_init();
    modbus_slave_init(RTU, 115200, NONE, Slave_ID);
    register_modbus_data();

    /* Loop forever */
    while (1)
    {
        check_modbus_request();

        update_LED();
    }
}

static void register_modbus_data(void)
{
    for (int i = 0; i < 20; i++)
    {
        register_app_data_to_modbus_slave_coils_table(i, &coil_data[i]);
        register_app_data_to_modbus_slave_din_table(i, din_data + i);
        register_app_data_to_modbus_slave_hreg_table(i, &hreg_data[i]);
        register_app_data_to_modbus_slave_inreg_table(i, &inreg_data[i]);
    }
    register_app_data_to_modbus_slave_coils_table(21,&LED_status);
}
static void update_LED(void)
{
    if (LED_status == 1)
    {
      LED_on();
    }
    else
    {
      LED_off();
    }
}
