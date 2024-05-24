/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "main.h"
#include "usart.h"
#include "core_init.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

uint8_t test_buf[]={"hello world !!!\r\n"};
uint8_t test_buf_2[]={"test !!!\r\n"};
uint8_t test;
int main(void)
{
  core_init();

  usart_init();
  __enable_irq();
  usart_send(test_buf,sizeof(test_buf));
  while((USART3 -> CR1)&USART_CR1_TXEIE_TXFNFIE);
  usart_send(test_buf_2,sizeof(test_buf_2));
  enable_usart_rx_interrupt();
    /* Loop forever */
	while(1)
  {
    test++;
  }
}
