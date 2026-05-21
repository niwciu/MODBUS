#pragma once

#include <avr/io.h>

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_PIN   PB5

void LED_init(void);
void LED_on(void);
void LED_off(void);
