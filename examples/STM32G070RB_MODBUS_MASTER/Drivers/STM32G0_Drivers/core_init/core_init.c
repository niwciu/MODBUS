#include "stm32g070xx.h"

#include "core_init.h"

#define SYS_FREQ            64000000U
#define SYS_TICK_MAX_CNT    1000U

void core_init(void)
{
    __IO uint32_t tmpreg;
    RCC->APBENR2 |=RCC_APBENR2_SYSCFGEN;
    /* Delay after an RCC peripheral clock enabling */
    tmpreg = RCC->APBENR2 & RCC_APBENR2_SYSCFGEN;
    (void)tmpreg;

    RCC->APBENR2 |=RCC_APBENR1_PWREN;
    /* Delay after an RCC peripheral clock enabling */
    tmpreg = RCC->APBENR2 & RCC_APBENR1_PWREN;
    (void)tmpreg;

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, 3);

    /* Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral*/
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE;

    /* set flash latency for 64Mhz */
    FLASH ->ACR &=~(FLASH_ACR_LATENCY); //clear latency flag
    FLASH ->ACR |=FLASH_ACR_LATENCY_2;
    while((FLASH->ACR & FLASH_ACR_LATENCY)!=FLASH_ACR_LATENCY_2);

    /* HSI configuration and activation */
    RCC->CR|=RCC_CR_HSION;
    while((RCC->CR & RCC_CR_HSIRDY)!=RCC_CR_HSIRDY);

    /* Main PLL configuration and activation  form max sysclk 64Mhz*/
    RCC->PLLCFGR &=~( RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLR);
    // ((RCC_PLLCFGR_PLLSRC_HSI) | (0x00000000U) | (8 << RCC_PLLCFGR_PLLN_Pos) | (RCC_PLLCFGR_PLLR_0));
    RCC->PLLCFGR |= ((RCC_PLLCFGR_PLLSRC_HSI) |(8 << RCC_PLLCFGR_PLLN_Pos) | (RCC_PLLCFGR_PLLR_0));
    RCC->CR|= RCC_CR_PLLON;
    RCC->PLLCFGR|= RCC_PLLCFGR_PLLREN;


    while((RCC->CR & RCC_CR_PLLRDY)!=RCC_CR_PLLRDY);

    // /* Set AHB prescaler*/
    RCC->CFGR &= ~(RCC_CFGR_HPRE);
    RCC->CFGR |= 0x00000000U ;

    /* Sysclk activation on the main PLL */
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_1;
    while((RCC->CFGR & RCC_CFGR_SWS)!= RCC_CFGR_SWS_1);

    /* Set APB1 prescaler*/
    RCC->CFGR &= ~(RCC_CFGR_PPRE);
    RCC->CFGR |= 0; 
    SysTick->LOAD  = (uint32_t)((SYS_FREQ / SYS_TICK_MAX_CNT) - 1UL);               /* set reload register */
    SysTick->VAL   = 0UL;                                                           /* Load the SysTick Counter Value */  
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  
}