/**
 * @file usart.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#define N_USART_SEND_BITS_DELAY 17 // 11bit* 1,5char

#include "modbus_driver_interface.h"
#include "modbus_driver_config.h"
#include <stddef.h>

typedef struct
{
    modbus_buf_t *cur_byte_ptr;
    modbus_buf_t *last_byte_ptr;
} tx_buf_t;
typedef enum
{
    WAITING_FOR_FRAME,
    FRAME_RECEIVED,
} driver_timer_status_t;

static driver_subscr_cb_t master_msg_tx_complete_cb = NULL;
static driver_subscr_cb_t master_t_1_5_char_break_cb = NULL;
static driver_subscr_cb_t master_t_3_5_char_break_cb = NULL;
static driver_subscr_cb_t master_frame_error_cb = NULL;

static driver_timer_status_t FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;

static modbus_req_resp_t *rx_msg = NULL;
#if MASTER_USE_DMA == OFF
static tx_buf_t tx_buf;
#endif

static void master_usart_init(baud_t baud, parity_t parity);
static void master_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void master_disable_usart_rx_interrupt(void);
static void master_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void master_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void master_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void master_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static void USART_GPIO_init(void);
static void USART_module_config(baud_t Baud, parity_t parity);
static uint32_t get_RTOR_value(baud_t baud);
static void USART_set_and_enable_interrupts(void);
static void TIMER7_init(baud_t baud);
static uint32_t get_ARR_value(baud_t baud);
#if MASTER_USE_DMA == ON
static void USART_DMA_init(void);
#endif

static const modbus_RTU_driver_struct_t master_RTU_driver_interface = {
    master_usart_init,
    master_usart_send,
    master_enable_usart_rx_interrupt,
    master_disable_usart_rx_interrupt,
    master_t_1_5_char_expired_callback_subscribe,
    master_msg_tx_done_callback_subscribe,
    master_t_3_5_char_expired_callback_subscribe,
    master_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void)
{
    return &master_RTU_driver_interface;
}

static void master_usart_init(uint32_t Baud, parity_t parity)
{
    USART_GPIO_init();
#if MASTER_USE_DMA == ON
    USART_DMA_init();
#endif
    USART_module_config(Baud, parity);
    USART_set_and_enable_interrupts();
    TIMER7_init(Baud);
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
}

static void master_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
#if USART_DE_HW_CONTROLL == OFF
        // ustaw stan wysoki na DE
        MODBUS_MASTER_USART_DE_PORT->BSRR |= MODBUS_MASTER_USART_DE_SET_PIN;
#endif
#if MASTER_USE_DMA == OFF
        tx_buf.cur_byte_ptr = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        MODBUS_MASTER_USART->TDR = *tx_msg;
        MODBUS_MASTER_USART->CR1 |= (USART_CR1_TXEIE_TXFNFIE);

#else
        // According to RM0444 page 1039
        MODBUS_MASTER_DMA_chanell->CPAR = (uint32_t) & (MODBUS_MASTER_USART->TDR);
        MODBUS_MASTER_DMA_chanell->CMAR = (uint32_t)(tx_msg);
        MODBUS_MASTER_DMA_chanell->CNDTR = msg_len;
        MODBUS_MASTER_DMA_chanell->CCR |= DMA_CCR_TCIE | //
                                          DMA_CCR_TEIE | //
                                          DMA_CCR_DIR |  //
                                          DMA_CCR_MINC | //
                                          DMA_CCR_PL;    //
        MODBUS_MASTER_USART->ICR |= USART_ICR_TCCF;      // Clear TC Flag
        MODBUS_MASTER_DMA_chanell->CCR |= DMA_CCR_EN;    // Enable DMA transfer
#endif
    }
    // disable timer and clear all flags -> needed for repeat request handle
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    MODBUS_MASTER_TIMER->CR1 &= ~TIM_CR1_CEN;
    MODBUS_MASTER_USART->ICR |= USART_ICR_RTOCF; // enable T1,5 char timer interrupt
}

static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_msg = recv_buf;
    MODBUS_MASTER_USART->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
}
static void master_disable_usart_rx_interrupt(void)
{
    rx_msg = NULL;
    MODBUS_MASTER_USART->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
}

static void master_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    master_t_1_5_char_break_cb = callback;
}

static void master_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback)
{
    master_msg_tx_complete_cb = callback;
}

static void master_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    master_t_3_5_char_break_cb = callback;
}

static void master_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback)
{
    master_frame_error_cb = callback;
}

#if MASTER_USE_DMA == ON
void MODBUS_MASTER_DMA_IRQHandler(void)
{
    if ((MODBUS_MASTER_DMA->ISR) & MODBUS_MASTER_DMA_ISR_TCIF)
    {
        // wylacz przerwania
        MODBUS_MASTER_DMA_chanell->CCR &= ~(DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE);
        // skasuj flage przerwania
        MODBUS_MASTER_DMA->IFCR |= MODBUS_MASTER_DMA_ISR_TCIF;
        if(NULL != master_msg_tx_complete_cb)
        {
            master_msg_tx_complete_cb();
        }
        // włącz przerwanie Transfer completed
        MODBUS_MASTER_USART->CR1 |= USART_ISR_TC;

    }
    if ((MODBUS_MASTER_DMA->ISR) & MODBUS_MASTER_DMA_ISR_TEIF)
    {
#if USART_DE_HW_CONTROLL == OFF
        MODBUS_MASTER_USART_DE_PORT->BSRR |= MODBUS_MASTER_USART_DE_RESET_PIN;
#endif
        // ToDo obsługa błedu transmisji DMA do USART
    }
}
#endif
void MODBUS_MASTER_USART_IRQHandler(void)
{
    // ToDo check in documentation which interrupt need to be also take cared -> errors overrun etc
    if ((MODBUS_MASTER_USART->ISR) & USART_ISR_RXNE_RXFNE)
    {
        rx_msg->data[rx_msg->len] = MODBUS_MASTER_USART->RDR;
        rx_msg->len++;

        if ((FRAME_RECEIVED == FRAME_DETECTION_FLAG)) // timer 3,5 nie odliczyl
        {
            master_frame_error_cb();
        }
        MODBUS_MASTER_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_MASTER_TIMER->CR1 |= TIM_CR1_CEN;
    }
#if MASTER_USE_DMA == OFF
    if ((MODBUS_MASTER_USART->ISR) & USART_ISR_TXE_TXFNF)
    {
        if (tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
        {
            tx_buf.cur_byte_ptr++;
            MODBUS_MASTER_USART->TDR = *tx_buf.cur_byte_ptr;
        }
        else
        {
            // koniec nadawania wyłącz przerwanie TXE i włącz RX
            MODBUS_MASTER_USART->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE);
            // włącz przerwanie Transfer completed
            MODBUS_MASTER_USART->CR1 |= USART_ISR_TC;
        }
    }

#endif
    if ((MODBUS_MASTER_USART->ISR) & USART_ISR_TC)
    {
        if (NULL != master_msg_tx_complete_cb)
        {
            master_msg_tx_complete_cb(); 
        }
        // wyłącz przerwanie transfer compleated i skasuj flage przerwania
        MODBUS_MASTER_USART->CR1 &= ~(USART_ISR_TC);
        MODBUS_MASTER_USART->ICR |= USART_ICR_TCCF;
#if USART_DE_HW_CONTROLL == OFF
        // ustaw stan wysoki na DE
        MODBUS_MASTER_USART_DE_PORT->BSRR |= MODBUS_MASTER_USART_DE_RESET_PIN;
#endif
    }

    if ((MODBUS_MASTER_USART->ISR) & USART_ISR_RTOF)
    {
        if (NULL != master_t_1_5_char_break_cb)
        {
            master_t_1_5_char_break_cb();
        }
        FRAME_DETECTION_FLAG = FRAME_RECEIVED;
        MODBUS_MASTER_USART->ICR |= USART_ICR_RTOCF; // jakby to przerwanie kasować dopiero w timerze 7
    }
}

void MODBUS_MASTER_TIMER_IRQHandler(void)
{
    if (MODBUS_MASTER_TIMER->SR & TIM_SR_UIF)
    {
        if (NULL != master_t_3_5_char_break_cb)
        {
            master_t_3_5_char_break_cb();
        }
        FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
        MODBUS_MASTER_TIMER->SR &= ~TIM_SR_UIF;
    }
}

static void USART_GPIO_init(void)
{
    // enable clk for modbus_gpio
    RCC->IOPENR |= MODBUS_MASTER_USART_GPIO_RX_TX_CLK_EN;
    RCC->IOPENR |= MODBUS_MASTER_USART_GPIO_DE_CLK_EN;
    // config gpio mode
    MODBUS_MASTER_USART_RX_TX_PORT->MODER &= ~(MODBUS_MASTER_USART_TX_MODE_Msk | MODBUS_MASTER_USART_RX_MODE_Msk);
    MODBUS_MASTER_USART_RX_TX_PORT->MODER |= (MODBUS_MASTER_USART_TX_MODE | MODBUS_MASTER_USART_RX_MODE);
    MODBUS_MASTER_USART_DE_PORT->MODER &= ~(MODBUS_MASTER_USART_DE_MODE_Msk);
    MODBUS_MASTER_USART_DE_PORT->MODER |= (MODBUS_MASTER_USART_DE_MODE);
    // set alternate function for GPIO
    MODBUS_MASTER_USART_RX_TX_PORT->AFR[MODBUS_MASTER_USART_RX_TX_AF_REG] &= ~(MODBUS_MASTER_USART_TX_AF_Msk | MODBUS_MASTER_USART_TX_AF_Msk);
    MODBUS_MASTER_USART_RX_TX_PORT->AFR[MODBUS_MASTER_USART_RX_TX_AF_REG] |= (MODBUS_MASTER_USART_TX_AF | MODBUS_MASTER_USART_RX_AF);
#if USART_DE_HW_CONTROLL == ON
    MODBUS_MASTER_USART_DE_PORT->AFR[MODBUS_MASTER_USART_DE_AF_REG] &= ~(MODBUS_MASTER_USART_DE_AF_Msk);
    MODBUS_MASTER_USART_DE_PORT->AFR[MODBUS_MASTER_USART_DE_AF_REG] |= (MODBUS_MASTER_USART_DE_AF);
#else
    MODBUS_MASTER_USART_DE_PORT->PUPDR |= GPIO_PUPDR_PUPD3_0; // only for DE manual controll
#endif
}

static void USART_module_config(uint32_t Baud, parity_t parity)
{
    // enable clk
    RCC->MODBUS_MASTER_USART_CLK |= MODBUS_MASTER_USART_CLK_EN;
    // set all bits in CR1 register
    MODBUS_MASTER_USART->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_RTOIE);
    if (NONE == parity)
    {
        MODBUS_MASTER_USART->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
        MODBUS_MASTER_USART->CR2 |= USART_CR2_STOP_1;
    }
    else if (ODD == parity)
    {
        MODBUS_MASTER_USART->CR1 |= (USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
    }
    else
    {
        MODBUS_MASTER_USART->CR1 &= ~(USART_CR1_PS);
        MODBUS_MASTER_USART->CR1 |= USART_CR1_PCE | USART_CR1_M0;
    }
    MODBUS_MASTER_USART->CR1 |= USART_CR1_DEAT_3 |
                                USART_CR1_DEDT_3; // set DEAT and DEDT to 8* sampling time
    MODBUS_MASTER_USART->CR2 |= USART_CR2_RTOEN;  //
#if USART_DE_HW_CONTROLL == ON
    MODBUS_MASTER_USART->CR3 |= USART_CR3_OVRDIS | USART_CR3_DEM; // DEM -> hardware flow controll

#else
    MODBUS_MASTER_USART->CR3 |= USART_CR3_OVRDIS; //
#endif

    MODBUS_MASTER_USART->BRR = MODBUS_MASTER_USART_CLK_FREQ / Baud; // set expected baudrate. For over8=1 CLk_FREQ must be multiply by 2
    MODBUS_MASTER_USART->RTOR |= get_RTOR_value(Baud);              // set End Of Frame delay
#if MASTER_USE_DMA == ON
    MODBUS_MASTER_USART->CR3 |= USART_CR3_DMAT; // Enable DMA tx
#endif
    MODBUS_MASTER_USART->CR1 |= (USART_CR1_UE); // USART enable
}
//  CR1 -> TCIE
// event flag to TC w ISR
// TCCF

static uint32_t get_RTOR_value(baud_t baud)
{
    if (baud <= 19200)
        return N_USART_SEND_BITS_DELAY; // 11 bits * 1,5 char
    else
    {
        uint32_t q = baud / 9600;
        q = q * 83;
        q = q / 10;
        if (q > 0xFFFFFF)
            q = 0xFFFFFF;
        return q;
    }
}

static void USART_set_and_enable_interrupts(void)
{
    NVIC_SetPriority(MODBUS_MASTER_USART_IRQN, MODBUS_MASTER_USART_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_MASTER_USART_IRQN);
}

static void TIMER7_init(baud_t baud)
{
    /* Timer configuration for measuring 3,5 char delay*/
    RCC->APBENR1 |= MODBUS_MASTER_TIMER_CLK_EN;
    MODBUS_MASTER_TIMER->CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    MODBUS_MASTER_TIMER->DIER = TIM_DIER_UIE;
    MODBUS_MASTER_TIMER->PSC = MODBUS_MASTER_TIMER_CLK_FREQ / 1000000 - 1;
    MODBUS_MASTER_TIMER->ARR = get_ARR_value(baud);

    NVIC_SetPriority(MODBUS_MASTER_TIMER_IRQN, MODBUS_MASTER_TIMER_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_MASTER_TIMER_IRQN);
}

static uint32_t get_ARR_value(baud_t baud)
{
    uint32_t q;
    if (baud <= 19200)
    {
        q = baud / 2400;
        q = 16042 / q;
    }
    else
    {
        q = 1750; // value in ms
    }
    return q;
}

#if MASTER_USE_DMA == ON
static void USART_DMA_init(void)
{
    /* DMA controller clock enable */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* DMA1_Channel1_IRQn interrupt configuration */
    NVIC_SetPriority(MODBUS_MASTER_DMA_IRQN, 0); // ToDo ustalić priorytety
    NVIC_EnableIRQ(MODBUS_MASTER_DMA_IRQN);

    DMAMUX1_Channel1->CCR &= ~(DMAMUX_CxCR_DMAREQ_ID);
    DMAMUX1_Channel1->CCR |= (MODBUS_MASTER_USART_TX_RESOURCE); // 0x110101 ->53 -> USART2_TX_RESOURCE
}
#endif
