/**
 * @file LED_driver.h
 * @author niwciu (niwciu@gmail.com)
 * @brief simple module for driving LED on nucleo board. Allowe to init turn on and off the LED
 * @version 0.0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "stm32g070xx.h"

// LED port and pin defines
#define LED_PORT (GPIOA)
#define LED_PORT_CLK_EN (RCC_IOPENR_GPIOAEN)
#define MODER_LED_Msk (GPIO_MODER_MODE5_Msk)
#define MODER_LED_0 (GPIO_MODER_MODE5_0)
#define LED_OUT_PIN (GPIO_ODR_OD5)

    void LED_init(void);
    void LED_on(void);
    void LED_off(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _LED_DRIVER_H_ */