/**
 * @file main_app_master.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Common Modbus master application logic shared across all example platforms.
 *        Provides main_app_master() which initialises Modbus and runs the polling loop,
 *        and main_app_master_1ms_tick() which must be called from a 1 ms platform timer ISR.
 *        Platform-specific code (core_init, timer setup, sei, etc.) is called by the
 *        platform main() before invoking main_app_master().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "main_app_master.h"
#include "main.h"

modbus_coil_disin_t readed_coil   = 0;
modbus_coil_disin_t prev_coil_val = 0;
modbus_data_qty_t   coil_qty      = 1;

modbus_reg_t      readed_hreg   = 0;
modbus_reg_t      prev_hreg_val = 0;
modbus_data_qty_t reg_qty       = 1;

static volatile uint16_t update_timer = UPDATE_INTERVAL_MS;

static void update_modbus_data(void);

void main_app_master(baud_t baud)
{
    modbus_master_init(RTU, baud, NONE);

    while (1)
    {
        update_modbus_master_manager();
        update_modbus_data();
    }
}

void main_app_master_1ms_tick(void)
{
    if (update_timer)
        update_timer--;
    update_modbus_master_timout_timer();
}

static void update_modbus_data(void)
{
    if (update_timer == 0)
    {
        modbus_master_read_coils(READ_COIL_ADR, coil_qty, SLAVE_ADDRES_EXAMPLE_NODE);
        modbus_master_write_single_coil(WRITE_COIL_ADR, SLAVE_ADDRES_EXAMPLE_NODE, readed_coil);
        update_timer = UPDATE_INTERVAL_MS;
    }
}
