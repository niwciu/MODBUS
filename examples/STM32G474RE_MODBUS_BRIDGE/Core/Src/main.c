/**
 * @file main.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus bridge entry point for STM32G474RE.
 *        Slave: USART3 (upstream network), Master: USART1 (downstream subnet).
 *        SysTick fires at 1 ms and forwards to main_app_bridge_1ms_tick().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core_init.h"
#include "main_app_bridge.h"

int main(void)
{
    core_init();
    main_app_bridge(115200, 115200);
    return 0;
}

void SysTick_Handler(void)
{
    main_app_bridge_1ms_tick();
}
