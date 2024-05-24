/**
 * @file usart.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stm32g070xx.h"
#include "modbus_driver_interface.h"
#include <stddef.h>

#define RX_DATA_BUF_SIZE 256
#define LOW 0
#define HIGH 1

/** modbus USART and Timer selection configuration*/
#define MODBUS_USART (USART1)
#define MODBUS_USART_CLK APBENR2
#define MODBUS_USART_CLK_EN (RCC_APBENR2_USART1EN)
#define MODBUS_USART_CLK_FREQ 64000000UL
#define MODBUS_USART_IRQN USART1_IRQn
#define MODBUS_USART_IRQHandler USART1_IRQHandler
#define MODBUS_USART_IRQ_PRIORITY 10

#define MODBUS_USART_PORT (GPIOB)
#define MODBUS_USART_RX_MODE (GPIO_MODER_MODE7_1)
#define MODBUS_USART_TX_MODE (GPIO_MODER_MODE6_1)
#define MODBUS_USART_DE_MODE (GPIO_MODER_MODE3_1)
#define MODBUS_USART_RX_MODE_Msk (GPIO_MODER_MODE7)
#define MODBUS_USART_TX_MODE_Msk (GPIO_MODER_MODE6)
#define MODBUS_USART_DE_MODE_Msk (GPIO_MODER_MODE3)
#define MODBUS_USART_RX_AF_Msk (GPIO_AFRL_AFSEL7)
#define MODBUS_USART_TX_AF_Msk (GPIO_AFRL_AFSEL6)
#define MODBUS_USART_DE_AF_Msk (GPIO_AFRL_AFSEL3)
#define MODBUS_USART_RX_AF (0)
#define MODBUS_USART_TX_AF (0)
#define MODBUS_USART_DE_AF (GPIO_AFRL_AFSEL3_2)
#define MODBUS_USART_AF_REG (LOW)
#define MODBUS_USART_GPIO_CLK_EN (RCC_IOPENR_GPIOBEN)

// #define MODBUS_USART    (USART3)
// #define MODBUS_USART_CLK_EN    (RCC_APBENR1_USART3EN)
// #define MODBUS_USART_IRQN   USART3_4_IRQn
// #define MODBUS_USART_IRQHandler USART3_USART4_LPUART1_IRQHandler
// #define MODBUS_USART_IRQ_PRIORITY 10

// #define MODBUS_USART_PORT   (GPIOD)
// #define MODBUS_USART_RX_MODE    (GPIO_MODER_MODE9_1)
// #define MODBUS_USART_TX_MODE    (GPIO_MODER_MODE8_1)
// #define MODBUS_USART_RX_MODE_Msk    (GPIO_MODER_MODE9)
// #define MODBUS_USART_TX_MODE_Msk    (GPIO_MODER_MODE8)
// #define MODBUS_USART_RX_AF_Msk  (GPIO_AFRL_AFSEL9)
// #define MODBUS_USART_TX_AF_Msk  (GPIO_AFRL_AFSEL8)
// #define MODBUS_USART_RX_AF  (0)
// #define MODBUS_USART_TX_AF  (0)
// #define MODBUS_USART_AF_REG (LOW)
// #define MODBUS_USART_GPIO_CLK_EN    (RCC_IOPENR_GPIODEN)

#define MODBUS_TIMER (TIM7)
#define MODBUS_TIMER_CLK_EN (RCC_APBENR1_TIM7EN)
#define MODBUS_TIMER_CLK_FREQ 64000000UL
#define MODBUS_TIMER_IRQN TIM7_IRQn
#define MODBUS_TIMER_IRQHandler TIM7_LPTIM2_IRQHandler
#define MODBUS_TIMER_IRQ_PRIORITY 10

typedef struct
{
    uint8_t *cur_byte_ptr;
    uint8_t *last_byte_ptr;
} tx_buf_t;
typedef struct
{
    uint8_t data[RX_DATA_BUF_SIZE];
    uint8_t data_idx;
} rx_buf_t;

rx_buf_t rx_buf;
tx_buf_t tx_buf;

static void usart_init(uint32_t Baud, parity_t parity);
static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void enable_usart_rx_interrupt(void);
static void disable_usart_rx_interrupt(void);
static void uasrt_subscribe_rx_callback(rx_cb_t callback);
const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void);

static void USART_GPIO_init(void);
static void USART_module_config(uint32_t Baud, parity_t parity);
static void USART_data_init(void);

static const struct modbus_RTU_driver_struct RTU_driver = {
    usart_init,
    usart_send,
    enable_usart_rx_interrupt,
    disable_usart_rx_interrupt,
    uasrt_subscribe_rx_callback,
};

const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void)
{
    return &RTU_driver;
}

static void usart_init(uint32_t Baud, parity_t parity) // parameters for init -> baud, pariti, stop bits
{

    USART_GPIO_init();

    USART_module_config(Baud, parity);

    NVIC_SetPriority(MODBUS_USART_IRQN, MODBUS_USART_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_USART_IRQN);
    // ToDo init rx tx buffers as a function
    rx_buf.data_idx = 0;

    /* Timer configuration for measuring 1,5 and 3,5 char*/
    // ToDo get ARR value fo 3,5 char depanding on Baud Rate
    RCC->APBENR1 |= MODBUS_TIMER_CLK_EN;
    MODBUS_TIMER->CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    MODBUS_TIMER->DIER = TIM_DIER_UIE;
    MODBUS_TIMER->PSC = MODBUS_TIMER_CLK_FREQ / 1000000 - 1;
    MODBUS_TIMER->ARR = 1750;

    NVIC_SetPriority(MODBUS_TIMER_IRQN, MODBUS_TIMER_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_TIMER_IRQN);
}

static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
        tx_buf.cur_byte_ptr = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        MODBUS_USART->TDR = *tx_msg;
        MODBUS_USART->CR1 |= (USART_CR1_TXEIE_TXFNFIE);
    }
}
static void enable_usart_rx_interrupt(void)
{
    MODBUS_USART->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
}
static void disable_usart_rx_interrupt(void)
{
    MODBUS_USART->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
}

static void uasrt_subscribe_rx_callback(rx_cb_t callback)
{
}
void MODBUS_USART_IRQHandler(void)
{
    // ToDo check in documentation which interrupt need to be also take cared -> errors overrun etc
    if ((MODBUS_USART->ISR) & USART_ISR_RXNE_RXFNE)
    {
        // ToDo check if you are not between 1,5 char and 3,5 char timing (frame error)
        // if frame error accure set error flag and skip collecting data in rx buf
        rx_buf.data[rx_buf.data_idx] = MODBUS_USART->RDR;
        rx_buf.data_idx++;
        MODBUS_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_TIMER->CR1 |= TIM_CR1_CEN;
    }
    else if ((MODBUS_USART->ISR) & USART_ISR_TXE_TXFNF)
    {

        if (tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
        {
            tx_buf.cur_byte_ptr++;
            MODBUS_USART->TDR = *tx_buf.cur_byte_ptr;
        }
        else
        {
            MODBUS_USART->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE);
        }
    }
}

void MODBUS_TIMER_IRQHandler(void)
{
    if (MODBUS_TIMER->SR & TIM_SR_UIF)
    {
        MODBUS_TIMER->SR &= ~TIM_SR_UIF;
        rx_buf.data_idx = 0;
        // if error error flag set do nothign
        //  else pass the rx buf to rsp msg or set some flag to indicate that there are some data to parse
    }
}

static void USART_GPIO_init(void)
{
    // enable clk for modbus_gpio
    RCC->IOPENR |= MODBUS_USART_GPIO_CLK_EN;
    // config gpio mode
    MODBUS_USART_PORT->MODER &= ~(MODBUS_USART_TX_MODE_Msk | MODBUS_USART_RX_MODE_Msk | MODBUS_USART_DE_MODE_Msk);
    MODBUS_USART_PORT->MODER |= (MODBUS_USART_TX_MODE | MODBUS_USART_RX_MODE | MODBUS_USART_DE_MODE);
    // set alternate function for GPIO
    MODBUS_USART_PORT->AFR[MODBUS_USART_AF_REG] &= ~(MODBUS_USART_TX_AF_Msk | MODBUS_USART_TX_AF_Msk | MODBUS_USART_DE_AF_Msk);
    MODBUS_USART_PORT->AFR[MODBUS_USART_AF_REG] |= (MODBUS_USART_TX_AF | MODBUS_USART_RX_AF | MODBUS_USART_DE_AF);
}
static void USART_module_config(uint32_t Baud, parity_t parity)
{
    // enable clk
    RCC->MODBUS_USART_CLK |= MODBUS_USART_CLK_EN;
    // set all bits in CR1 register
    MODBUS_USART->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_RTOIE);

    if (NONE == parity)
    {
        MODBUS_USART->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
        MODBUS_USART->CR2 |= USART_CR2_STOP_1;
    }
    else if (ODD == parity)
    {
        MODBUS_USART->CR1 |= (USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
    }
    else
    {
        MODBUS_USART->CR1 &= ~(USART_CR1_PS);
        MODBUS_USART->CR1 |= (USART_CR1_PCE | USART_CR1_M0 );
    }
    MODBUS_USART->CR1 |= (USART_CR1_DEAT_3 | USART_CR1_DEDT_3); // set DEAT and DEDT to 8* sampling time
 
    MODBUS_USART->CR2 |= USART_CR2_RTOEN;
    // set all required bits in CR3 register
    MODBUS_USART->CR3 |= (USART_CR3_OVRDIS | USART_CR3_DEM);
    // set expected baudrate
    MODBUS_USART->BRR = MODBUS_USART_CLK_FREQ / Baud; // for over8=1 CLk_FRE must be multiply by 2

    // enable usart module
    MODBUS_USART->CR1 |= (USART_CR1_UE);
}
static void USART_data_init(void)
{
}