/**
 * @file main.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus master entry point for STM32G474RE.
 *        Initialises the MCU core, then delegates to main_app_master().
 *        SysTick fires at 1 ms and forwards to main_app_master_1ms_tick().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core_init.h"
#include "main_app_master.h"

int main(void)
{
    core_init();
    main_app_master(115200);
    return 0;
}

void SysTick_Handler(void)
{
    main_app_master_1ms_tick();
}
