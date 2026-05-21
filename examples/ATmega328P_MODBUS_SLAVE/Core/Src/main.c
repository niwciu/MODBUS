/**
 * @file main.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus slave entry point for ATmega328P @ 16 MHz.
 *        Enables global interrupts, then delegates to main_app_slave().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <avr/interrupt.h>
#include "main_app_slave.h"

int main(void)
{
    sei();
    main_app_slave(9600);
    return 0;
}
