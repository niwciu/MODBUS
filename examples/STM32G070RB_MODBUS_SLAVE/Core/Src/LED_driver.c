/**
 * @file LED_driver.c
 * @author niwciu (niwciu@gmail.com)
 * @brief simple module for driving LED on nucleo board. Allowe to init turn on and off the LED
 * @version 0.0.1
 * @date 2024-08-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "LED_driver.h"

void LED_init(void)
{
    RCC->IOPENR |= LED_PORT_CLK_EN;
    LED_PORT->MODER &= (~MODER_LED_Msk);
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