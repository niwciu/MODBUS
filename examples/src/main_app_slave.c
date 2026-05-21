/**
 * @file main_app_slave.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Common Modbus slave application logic shared across all example platforms.
 *        Provides main_app_slave() which initialises the LED, sets up Modbus, registers
 *        example data and runs the polling loop.
 *        Platform-specific code (core_init, sei, etc.) is called by the platform main()
 *        before invoking main_app_slave().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "main_app_slave.h"
#include "LED_driver.h"

#define EXAMPLE_DATA_QTY 20

static modbus_coil_disin_t coil_data[EXAMPLE_DATA_QTY] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
static modbus_coil_disin_t din_data[EXAMPLE_DATA_QTY] = {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1};
static modbus_reg_t hreg_data[EXAMPLE_DATA_QTY] = {2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                                                   3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010};
static modbus_reg_t inreg_data[EXAMPLE_DATA_QTY] = {60, 61, 62, 4204, 4205, 4206, 4207, 4208, 4209, 4210,
                                                    4301, 4302, 4303, 4304, 4305, 4306, 4307, 4308, 4309, 4310};

static modbus_device_ID_t Slave_ID = 0x01;
static modbus_coil_disin_t LED_status = 0;

static void register_modbus_data(void);
static void update_LED(void);

void main_app_slave(baud_t baud)
{
    LED_init();
    modbus_slave_init(RTU, baud, NONE, Slave_ID);
    register_modbus_data();

    while (1)
    {
        check_modbus_request();
        update_LED();
    }
}

static void register_modbus_data(void)
{
    for (int i = 0; i < EXAMPLE_DATA_QTY; i++)
    {
        register_app_data_to_modbus_slave_coils_table(i, &coil_data[i]);
        register_app_data_to_modbus_slave_din_table(i, &din_data[i]);
        register_app_data_to_modbus_slave_hreg_table(i, &hreg_data[i]);
        register_app_data_to_modbus_slave_inreg_table(i, &inreg_data[i]);
    }
    register_app_data_to_modbus_slave_coils_table(21, &LED_status);
}

static void update_LED(void)
{
    if (LED_status == 1)
        LED_on();
    else
        LED_off();
}
