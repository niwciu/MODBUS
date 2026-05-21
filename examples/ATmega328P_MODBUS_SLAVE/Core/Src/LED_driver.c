#include "LED_driver.h"

void LED_init(void)
{
    LED_DDR  |=  (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);
}

void LED_on(void)
{
    LED_PORT |= (1 << LED_PIN);
}

void LED_off(void)
{
    LED_PORT &= ~(1 << LED_PIN);
}
