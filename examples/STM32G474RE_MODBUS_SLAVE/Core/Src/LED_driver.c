#include "LED_driver.h"

void LED_init(void)
{
    RCC->AHB2ENR |= LED_PORT_CLK_EN;
    LED_PORT->MODER &= ~MODER_LED_Msk;
    LED_PORT->MODER |= MODER_LED_0;
}

void LED_on(void)
{
    LED_PORT->ODR |= LED_OUT_PIN;
}

void LED_off(void)
{
    LED_PORT->ODR &= ~LED_OUT_PIN;
}
