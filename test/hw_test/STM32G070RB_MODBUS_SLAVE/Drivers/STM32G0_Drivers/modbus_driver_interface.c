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

driver_subscr_cb_t msg_tx_done_cb = NULL;
driver_subscr_cb_t t_1_5_char_break_cb = NULL;
driver_subscr_cb_t t_3_5_char_break_cb = NULL;
driver_subscr_cb_t frame_error_cb = NULL;

driver_timer_status_t FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;

modbus_req_resp_t *rx_buf = NULL;
tx_buf_t tx_buf;

modbus_buf_t mock_slave_tx_buffer[MODBUS_RTU_BUFFER_SIZE];

static void slave_usart_init(baud_t baud, parity_t parity);
static void slave_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static uint32_t get_RTOR_value(baud_t baud);
static void TIMER7_init(baud_t baud);
static uint32_t get_ARR_value(baud_t baud);
static void USART_GPIO_init(void);
static void USART_module_config(baud_t Baud, parity_t parity);
// static void USART_data_init(void);
static void USART_set_and_enable_interrupts(void);

static const modbus_RTU_driver_struct_t slave_RTU_driver_interface = {
    slave_usart_init,
    slave_usart_send,
    slave_enable_usart_rx_interrupt,
    NULL,
    slave_t_1_5_char_expired_callback_subscribe,
    slave_msg_tx_done_callback_subscribe,
    slave_t_3_5_char_expired_callback_subscribe,
    slave_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void)
{
    return &slave_RTU_driver_interface;
}

static void slave_usart_init(uint32_t Baud, parity_t parity)
{
    USART_GPIO_init();
    USART_module_config(Baud, parity);
    // USART_data_init();
    USART_set_and_enable_interrupts();
    TIMER7_init(Baud);
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
}

static void slave_usart_send(modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
        tx_buf.cur_byte_ptr = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        MODBUS_USART->TDR = *tx_msg;
        MODBUS_USART->CR1 |= (USART_CR1_TXEIE_TXFNFIE);
    }
}
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_buf = recv_buf;
    MODBUS_USART->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
}
static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    t_1_5_char_break_cb = callback;
}
static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback)
{
    msg_tx_done_cb = callback;
}
static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback)
{
    t_3_5_char_break_cb = callback;
}
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback)
{
    frame_error_cb = callback;
}

void MODBUS_USART_IRQHandler(void)
{
    // ToDo check in documentation which interrupt need to be also take cared -> errors overrun etc
    if ((MODBUS_USART->ISR) & USART_ISR_RXNE_RXFNE)
    {

        rx_buf->data[rx_buf->len] = MODBUS_USART->RDR;
        rx_buf->len++;

        if ((FRAME_RECEIVED == FRAME_DETECTION_FLAG)) // timer 3,5 odliczyl
        {
            frame_error_cb();
        }
        MODBUS_TIMER->EGR |= TIM_EGR_UG;
        MODBUS_TIMER->CR1 |= TIM_CR1_CEN;
    }
    if ((MODBUS_USART->ISR) & USART_ISR_TXE_TXFNF)
    {
        if (tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
        {
            tx_buf.cur_byte_ptr++;
            MODBUS_USART->TDR = *tx_buf.cur_byte_ptr;
        }
        else
        {
            // koniec nadawania
            MODBUS_USART->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE);
            msg_tx_done_cb();
        }
    }
    if ((MODBUS_USART->ISR) & USART_ISR_RTOF)
    {
        if (NULL != t_1_5_char_break_cb)
        {
            t_1_5_char_break_cb();
        }
        FRAME_DETECTION_FLAG = FRAME_RECEIVED;
        // MODBUS_USART->ICR |= USART_ICR_RTOCF; // jakby to przerwanie kasować dopiero w timerze 7
    }
}

void MODBUS_TIMER_IRQHandler(void)
{
    if (MODBUS_TIMER->SR & TIM_SR_UIF)
    {
        t_3_5_char_break_cb();
        FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
        MODBUS_USART->ICR |= USART_ICR_RTOCF; // dopiero tutaj odblokowuje przerwanie od t1,5char
        MODBUS_TIMER->SR &= ~TIM_SR_UIF;
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
        MODBUS_USART->CR1 |= (USART_CR1_PCE | USART_CR1_M0);
    }
    MODBUS_USART->CR1 |= (USART_CR1_DEAT_3 | USART_CR1_DEDT_3); // set DEAT and DEDT to 8* sampling time
    MODBUS_USART->CR2 |= USART_CR2_RTOEN;
    // set all required bits in CR3 register
    MODBUS_USART->CR3 |= (USART_CR3_OVRDIS | USART_CR3_DEM);
    // set expected baudrate
    MODBUS_USART->BRR = MODBUS_USART_CLK_FREQ / Baud; // for over8=1 CLk_FRE must be multiply by 2
    // set modbus delay for recognizing end of frame
    MODBUS_USART->RTOR |= get_RTOR_value(Baud);
    // enable usart module
    MODBUS_USART->CR1 |= (USART_CR1_UE);
}
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
// static void USART_data_init(void)
// {
// }

static void USART_set_and_enable_interrupts(void)
{
    NVIC_SetPriority(MODBUS_USART_IRQN, MODBUS_USART_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_USART_IRQN);
}
static void TIMER7_init(baud_t baud)
{
    /* Timer configuration for measuring 3,5 char delay*/
    RCC->APBENR1 |= MODBUS_TIMER_CLK_EN;
    MODBUS_TIMER->CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    MODBUS_TIMER->DIER = TIM_DIER_UIE;
    MODBUS_TIMER->PSC = MODBUS_TIMER_CLK_FREQ / 1000000 - 1;
    MODBUS_TIMER->ARR = get_ARR_value(baud);

    NVIC_SetPriority(MODBUS_TIMER_IRQN, MODBUS_TIMER_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_TIMER_IRQN);
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
