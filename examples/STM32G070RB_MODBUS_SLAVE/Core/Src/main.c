/**
 * @file main.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus slave entry point for STM32G070RB.
 *        Initialises the MCU core, then delegates to main_app_slave().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core_init.h"
#include "main_app_slave.h"

int main(void)
{
    core_init();
    main_app_slave(115200);
    return 0;
}
