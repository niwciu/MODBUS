/**
 * @file main.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus master entry point for ATmega328P @ 16 MHz.
 *        Sets up Timer2 for the 1 ms tick, enables global interrupts,
 *        then delegates to main_app_master().
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main_app_master.h"

static void timer2_1ms_init(void);

int main(void)
{
    timer2_1ms_init();
    sei();
    main_app_master(9600);
    return 0;
}

static void timer2_1ms_init(void)
{
    // CTC mode, prescaler 128: 16 MHz / 128 = 125 kHz, OCR2A=124 -> 1 ms tick
    TCCR2A = (1 << WGM21);
    OCR2A  = 124;
    TIMSK2 = (1 << OCIE2A);
    TCCR2B = (1 << CS22) | (1 << CS20);
}

ISR(TIMER2_COMPA_vect)
{
    main_app_master_1ms_tick();
}
