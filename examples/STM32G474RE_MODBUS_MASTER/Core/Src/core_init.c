/**
 * @file core_init.c
 * @brief STM32G474RE core clock init: HSI → PLL → 64 MHz SYSCLK.
 *        SysTick enabled and generates 1 ms interrupt.
 */

#include "stm32g474xx.h"
#include "core_init.h"

#define SYS_FREQ          64000000U
#define SYS_TICK_MAX_CNT  1000U

void core_init(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    (void)(RCC->APB1ENR1 & RCC_APB1ENR1_PWREN);

    /* Flash latency: 3 WS for 64 MHz, range 1 */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_3WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_3WS)
        ;

    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        ;

    /* PLL: HSI, M=2 (/2→8MHz), N=16 (×16→128MHz VCO), R=0 (/2→64MHz) */
    RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSI)
                 | (1UL << RCC_PLLCFGR_PLLM_Pos)
                 | (16UL << RCC_PLLCFGR_PLLN_Pos)
                 | (0UL << RCC_PLLCFGR_PLLR_Pos);
    RCC->CR |= RCC_CR_PLLON;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        ;

    SysTick->LOAD = (uint32_t)(SYS_FREQ / SYS_TICK_MAX_CNT - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
