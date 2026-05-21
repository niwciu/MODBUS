/**
 * @file main_app_bridge.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Common Modbus bridge application logic shared across all dual-USART example platforms.
 *        The bridge acts as a Modbus slave on the upstream network and as a Modbus master on
 *        the downstream subnet.  Data read from downstream slaves is stored in bridge_coil_data[]
 *        and bridge_hreg_data[], which are also registered in the slave's data tables so that
 *        the upstream master can transparently read them.
 *        Platform-specific code (core_init, SysTick/timer setup) is done by the platform main()
 *        before calling main_app_bridge().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "main_app_bridge.h"
#include "main.h"

modbus_coil_disin_t bridge_coil_data[BRIDGE_DATA_QTY];
modbus_reg_t        bridge_hreg_data[BRIDGE_DATA_QTY];

static modbus_device_ID_t    bridge_slave_id = BRIDGE_SLAVE_ID;
static volatile uint16_t     update_timer    = BRIDGE_UPDATE_INTERVAL_MS;

static void register_bridge_slave_data(void);
static void update_bridge_requests(void);

void main_app_bridge(baud_t slave_baud, baud_t master_baud)
{
    modbus_slave_init(RTU, slave_baud, NONE, bridge_slave_id);
    register_bridge_slave_data();
    modbus_master_init(RTU, master_baud, NONE);

    while (1)
    {
        check_modbus_request();
        update_modbus_master_manager();
        update_bridge_requests();
    }
}

void main_app_bridge_1ms_tick(void)
{
    if (update_timer)
        update_timer--;
    update_modbus_master_timout_timer();
}

static void register_bridge_slave_data(void)
{
    for (int i = 0; i < BRIDGE_DATA_QTY; i++)
    {
        register_app_data_to_modbus_slave_coils_table(i, &bridge_coil_data[i]);
        register_app_data_to_modbus_slave_hreg_table(i, &bridge_hreg_data[i]);
    }
}

static void update_bridge_requests(void)
{
    if (update_timer == 0)
    {
        modbus_master_read_coils(0, BRIDGE_DATA_QTY, DOWNSTREAM_SLAVE_ID);
        modbus_master_read_holding_reg(0, BRIDGE_DATA_QTY, DOWNSTREAM_SLAVE_ID);
        update_timer = BRIDGE_UPDATE_INTERVAL_MS;
    }
}
