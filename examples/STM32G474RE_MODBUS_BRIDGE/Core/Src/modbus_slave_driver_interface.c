/**
 * @file modbus_slave_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus slave RTU driver interface for STM32G474RE BRIDGE.
 *        Hardware: USART3 (PB10/PB11/PB14) + TIM6.
 *        GPIO clocks via RCC->AHB2ENR (G474 differs from G070 IOPENR).
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#define N_USART_SEND_BITS_DELAY 17 // 11bit * 1,5char

#include "modbus_driver_interface.h"
#include "modbus_slave_driver_config.h"
#include <stddef.h>

typedef struct
{
    volatile const modbus_buf_t *cur_byte_ptr;
    volatile const modbus_buf_t *last_byte_ptr;
} tx_buf_t;

typedef enum
{
    WAITING_FOR_FRAME,
    FRAME_RECEIVED,
} driver_timer_status_t;

static driver_subscr_cb_t slave_msg_tx_complete_cb = NULL;
static driver_subscr_cb_t slave_t_1_5_char_break_cb = NULL;
static driver_subscr_cb_t slave_t_3_5_char_break_cb = NULL;
static driver_subscr_cb_t slave_frame_error_cb = NULL;

static volatile driver_timer_status_t FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;

static modbus_req_resp_t *rx_msg = NULL;
#if SLAVE_USE_DMA == OFF
static volatile tx_buf_t tx_buf;
#endif



static void slave_usart_init(baud_t baud, parity_t parity);
static void slave_usart_send(volatile const modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void slave_disable_usart_rx_interrupt(void);
static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static void USART_GPIO_init(void);
static void USART_module_config(baud_t Baud, parity_t parity);
static uint32_t get_RTOR_value(baud_t baud);
static void USART_set_and_enable_interrupts(void);
static void slave_TIMER_init(baud_t baud);
static uint32_t get_ARR_value(baud_t baud);
#if SLAVE_USE_DMA == ON
static void USART_DMA_init(void);
#endif

static const modbus_RTU_driver_struct_t slave_RTU_driver_interface = {
    .init = slave_usart_init,
    .send = slave_usart_send,
    .enable_rcev = slave_enable_usart_rx_interrupt,
    .disable_rcev = slave_disable_usart_rx_interrupt,
    .subscribe_t_1_5_char_expired_cb = slave_t_1_5_char_expired_callback_subscribe,
    .subscribe_msg_tx_done_cb = slave_msg_tx_done_callback_subscribe,
    .subscribe_t_3_5_char_expired_cb = slave_t_3_5_char_expired_callback_subscribe,
    .subscribe_modbus_frame_error_cb = slave_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void)
{
    return &slave_RTU_driver_interface;
}

static void slave_usart_init(uint32_t Baud, parity_t parity)
{
    USART_GPIO_init();
#if SLAVE_USE_DMA == ON
    USART_DMA_init();
#endif
    USART_module_config(Baud, parity);
    USART_set_and_enable_interrupts();
    slave_TIMER_init(Baud);
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
}

static void slave_usart_send(volatile const modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
#if SLAVE_USART_DE_HW_CONTROLL == OFF
        MODBUS_SLAVE_USART_DE_PORT->BSRR |= MODBUS_SLAVE_USART_DE_SET_PIN;
#endif
#if SLAVE_USE_DMA == OFF
        tx_buf.cur_byte_ptr = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        MODBUS_SLAVE_USART->TDR = *tx_msg;
        MODBUS_SLAVE_USART->CR1 |= (USART_CR1_TXEIE_TXFNFIE);
#else
        MODBUS_SLAVE_DMA_chanel->CPAR  = (uint32_t)&(MODBUS_SLAVE_USART->TDR);
        MODBUS_SLAVE_DMA_chanel->CMAR  = (uint32_t)(tx_msg);
        MODBUS_SLAVE_DMA_chanel->CNDTR = msg_len;
        MODBUS_SLAVE_DMA_chanel->CCR  |= DMA_CCR_TCIE | DMA_CCR_TEIE | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_PL;
        MODBUS_SLAVE_USART->ICR       |= USART_ICR_TCCF;
        MODBUS_SLAVE_DMA_chanel->CCR  |= DMA_CCR_EN;
#endif
    }
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    MODBUS_SLAVE_TIMER->CR1 &= ~TIM_CR1_CEN;
    MODBUS_SLAVE_USART->ICR |= USART_ICR_RTOCF;
}

static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_msg = recv_buf;
    MODBUS_SLAVE_USART->CR1 |= USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE;
}

static void slave_disable_usart_rx_interrupt(void)
{
    rx_msg = NULL;
    MODBUS_SLAVE_USART->CR1 &= ~(USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE);
}

static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    slave_t_1_5_char_break_cb = callback;
}

static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback)
{
    slave_msg_tx_complete_cb = callback;
}

static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    slave_t_3_5_char_break_cb = callback;
}

static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback)
{
    slave_frame_error_cb = callback;
}

void MODBUS_SLAVE_USART_IRQHandler(void)
{
    uint32_t modbus_slave_usart_ISR = MODBUS_SLAVE_USART->ISR;

    // ======= TIMEOUT (1.5T Modbus) =======
    if (modbus_slave_usart_ISR & USART_ISR_RTOF)
    {
        MODBUS_SLAVE_USART->ICR |= USART_ICR_RTOCF;
        MODBUS_SLAVE_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_SLAVE_TIMER->CR1 |= TIM_CR1_CEN;
        if (NULL != slave_t_1_5_char_break_cb)
            slave_t_1_5_char_break_cb();
        FRAME_DETECTION_FLAG = FRAME_RECEIVED;
    }

    // ======= RECEIVER ERROR DETECTION =======
    if (modbus_slave_usart_ISR & (USART_ISR_FE | USART_ISR_NE | USART_ISR_PE | USART_ISR_ORE))
    {
        if (modbus_slave_usart_ISR & USART_ISR_FE)
            MODBUS_SLAVE_USART->ICR = USART_ICR_FECF;
        if (modbus_slave_usart_ISR & USART_ISR_NE)
            MODBUS_SLAVE_USART->ICR = USART_ICR_NECF;
        if (modbus_slave_usart_ISR & USART_ISR_PE)
            MODBUS_SLAVE_USART->ICR = USART_ICR_PECF;
        if (modbus_slave_usart_ISR & USART_ISR_ORE)
        {
            (void)MODBUS_SLAVE_USART->RDR;
            MODBUS_SLAVE_USART->ICR = USART_ICR_ORECF;
        }
        if (slave_frame_error_cb != NULL)
            slave_frame_error_cb();
        MODBUS_SLAVE_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_SLAVE_TIMER->CR1 |= TIM_CR1_CEN;
    }

    // ======= DATA RECEIVED =======
    if (modbus_slave_usart_ISR & USART_ISR_RXNE_RXFNE)
    {
        if (rx_msg != NULL)
        {
            rx_msg->data[rx_msg->len] = MODBUS_SLAVE_USART->RDR;
            rx_msg->len++;
        }
        else
        {
            (void)MODBUS_SLAVE_USART->RDR;
        }
        if (FRAME_RECEIVED == FRAME_DETECTION_FLAG)
        {
            if (slave_frame_error_cb != NULL)
                slave_frame_error_cb();
        }
        MODBUS_SLAVE_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_SLAVE_TIMER->CR1 |= TIM_CR1_CEN;
    }

#if SLAVE_USE_DMA == OFF
    // ======= TX BUFFER EMPTY =======
    if (modbus_slave_usart_ISR & USART_ISR_TXE_TXFNF)
    {
        if (tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
        {
            tx_buf.cur_byte_ptr++;
            MODBUS_SLAVE_USART->TDR = *tx_buf.cur_byte_ptr;
        }
        else
        {
            MODBUS_SLAVE_USART->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE);
            MODBUS_SLAVE_USART->ICR |= USART_ICR_TCCF;
            MODBUS_SLAVE_USART->CR1 |= USART_CR1_TCIE;
        }
    }
#endif

    // ======= TX TRANSFER COMPLETED =======
    if (modbus_slave_usart_ISR & USART_ISR_TC)
    {
        if (NULL != slave_msg_tx_complete_cb)
            slave_msg_tx_complete_cb();
        MODBUS_SLAVE_USART->CR1 &= ~(USART_CR1_TCIE);
        MODBUS_SLAVE_USART->ICR |= USART_ICR_TCCF;
#if SLAVE_USART_DE_HW_CONTROLL == OFF
        MODBUS_SLAVE_USART_DE_PORT->BSRR |= MODBUS_SLAVE_USART_DE_RESET_PIN;
#endif
    }
}

void MODBUS_SLAVE_TIMER_IRQHandler(void)
{
    if (MODBUS_SLAVE_TIMER->SR & TIM_SR_UIF)
    {
        if (NULL != slave_t_3_5_char_break_cb)
            slave_t_3_5_char_break_cb();
        FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
        MODBUS_SLAVE_TIMER->SR &= ~TIM_SR_UIF;
    }
}

static void USART_GPIO_init(void)
{
    // G474: GPIO clocks are on AHB2 (G070 used IOPENR)
    RCC->MODBUS_SLAVE_GPIO_CLK_REG |= MODBUS_SLAVE_USART_GPIO_RX_TX_CLK_EN;
    RCC->MODBUS_SLAVE_GPIO_CLK_REG |= MODBUS_SLAVE_USART_GPIO_DE_CLK_EN;

    MODBUS_SLAVE_USART_RX_TX_PORT->MODER &= ~(MODBUS_SLAVE_USART_TX_MODE_Msk | MODBUS_SLAVE_USART_RX_MODE_Msk);
    MODBUS_SLAVE_USART_RX_TX_PORT->MODER |= (MODBUS_SLAVE_USART_TX_MODE | MODBUS_SLAVE_USART_RX_MODE);
    MODBUS_SLAVE_USART_DE_PORT->MODER &= ~(MODBUS_SLAVE_USART_DE_MODE_Msk);
    MODBUS_SLAVE_USART_DE_PORT->MODER |= (MODBUS_SLAVE_USART_DE_MODE);
    MODBUS_SLAVE_USART_RX_TX_PORT->AFR[MODBUS_SLAVE_USART_RX_TX_AF_REG] &= ~(MODBUS_SLAVE_USART_TX_AF_Msk | MODBUS_SLAVE_USART_RX_AF_Msk);
    MODBUS_SLAVE_USART_RX_TX_PORT->AFR[MODBUS_SLAVE_USART_RX_TX_AF_REG] |= (MODBUS_SLAVE_USART_TX_AF | MODBUS_SLAVE_USART_RX_AF);
#if SLAVE_USART_DE_HW_CONTROLL == ON
    MODBUS_SLAVE_USART_DE_PORT->AFR[MODBUS_SLAVE_USART_DE_AF_REG] &= ~(MODBUS_SLAVE_USART_DE_AF_Msk);
    MODBUS_SLAVE_USART_DE_PORT->AFR[MODBUS_SLAVE_USART_DE_AF_REG] |= (MODBUS_SLAVE_USART_DE_AF);
#else
    MODBUS_SLAVE_USART_DE_PORT->PUPDR |= MODBUS_SLAVE_USART_DE_PUPDR;
#endif
}

static void USART_module_config(uint32_t Baud, parity_t parity)
{
    RCC->MODBUS_SLAVE_USART_CLK |= MODBUS_SLAVE_USART_CLK_EN;
    MODBUS_SLAVE_USART->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_RTOIE);
    if (NONE == parity)
    {
        MODBUS_SLAVE_USART->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
        MODBUS_SLAVE_USART->CR2 |= USART_CR2_STOP_1;
    }
    else if (ODD == parity)
    {
        MODBUS_SLAVE_USART->CR1 |= (USART_CR1_PCE | USART_CR1_PS | USART_CR1_M0);
    }
    else
    {
        MODBUS_SLAVE_USART->CR1 &= ~(USART_CR1_PS);
        MODBUS_SLAVE_USART->CR1 |= USART_CR1_PCE | USART_CR1_M0;
    }
    MODBUS_SLAVE_USART->CR1 |= USART_CR1_DEAT_3 | USART_CR1_DEDT_3;
    MODBUS_SLAVE_USART->CR2 |= USART_CR2_RTOEN;
#if SLAVE_USART_DE_HW_CONTROLL == ON
    MODBUS_SLAVE_USART->CR3 |= USART_CR3_OVRDIS | USART_CR3_DEM;
#else
    MODBUS_SLAVE_USART->CR3 |= USART_CR3_OVRDIS;
#endif
    MODBUS_SLAVE_USART->BRR = MODBUS_SLAVE_USART_CLK_FREQ / Baud;
    MODBUS_SLAVE_USART->RTOR |= get_RTOR_value(Baud);
#if SLAVE_USE_DMA == ON
    MODBUS_SLAVE_USART->CR3 |= USART_CR3_DMAT;
#endif
    MODBUS_SLAVE_USART->CR1 |= (USART_CR1_UE);
}

static uint32_t get_RTOR_value(baud_t baud)
{
    if (baud <= 19200)
        return N_USART_SEND_BITS_DELAY;
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
    NVIC_SetPriority(MODBUS_SLAVE_USART_IRQN, MODBUS_SLAVE_USART_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_SLAVE_USART_IRQN);
}

static void slave_TIMER_init(baud_t baud)
{
    MODBUS_SLAVE_TIMER_APBENR |= MODBUS_SLAVE_TIMER_CLK_EN;
    MODBUS_SLAVE_TIMER->CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    MODBUS_SLAVE_TIMER->DIER = TIM_DIER_UIE;
    MODBUS_SLAVE_TIMER->PSC = MODBUS_SLAVE_TIMER_CLK_FREQ / 1000000 - 1;
    MODBUS_SLAVE_TIMER->ARR = get_ARR_value(baud);
    NVIC_SetPriority(MODBUS_SLAVE_TIMER_IRQN, MODBUS_SLAVE_TIMER_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_SLAVE_TIMER_IRQN);
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
        q = 1750;
    }
    return q;
}

#if SLAVE_USE_DMA == ON
void MODBUS_SLAVE_DMA_IRQHandler(void)
{
    uint32_t dma_ISR = MODBUS_SLAVE_DMA->ISR;

    // ======= DMA TRANSMISSION FINISHED WITH NO ERROR =======
    if (dma_ISR & MODBUS_SLAVE_DMA_ISR_TCIF)
    {
        MODBUS_SLAVE_DMA_chanel->CCR &= ~(DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE);
        MODBUS_SLAVE_DMA->IFCR |= MODBUS_SLAVE_DMA_ISR_TCIF;
        MODBUS_SLAVE_USART->ICR |= USART_ICR_TCCF;
        MODBUS_SLAVE_USART->CR1 |= USART_CR1_TCIE;
    }

    // ======= DMA TEIF ERROR DETECTED =======
    if (dma_ISR & MODBUS_SLAVE_DMA_ISR_TEIF)
    {
        MODBUS_SLAVE_DMA_chanel->CCR &= ~(DMA_CCR_EN | DMA_CCR_TCIE | DMA_CCR_TEIE);
        MODBUS_SLAVE_DMA->IFCR |= MODBUS_SLAVE_DMA_ISR_TEIF;
#if SLAVE_USART_DE_HW_CONTROLL == OFF
        MODBUS_SLAVE_USART_DE_PORT->BSRR |= MODBUS_SLAVE_USART_DE_RESET_PIN;
#endif
        if (NULL != slave_msg_tx_complete_cb)
            slave_msg_tx_complete_cb();
    }
}

static void USART_DMA_init(void)
{
    // G474: DMA clock on AHB1
    RCC->MODBUS_SLAVE_DMA_CLK_REG |= MODBUS_SLAVE_DMA_CLK_EN;
    NVIC_SetPriority(MODBUS_SLAVE_DMA_IRQN, 0);
    NVIC_EnableIRQ(MODBUS_SLAVE_DMA_IRQN);
    MODBUS_SLAVE_DMAMUX_Chanel->CCR &= ~(DMAMUX_CxCR_DMAREQ_ID);
    MODBUS_SLAVE_DMAMUX_Chanel->CCR |= (MODBUS_SLAVE_USART_TX_RESOURCE);
}
#endif
