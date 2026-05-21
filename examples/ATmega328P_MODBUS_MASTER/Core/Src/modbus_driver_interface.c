/**
 * @file modbus_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief Modbus master RTU driver interface for ATmega328P.
 *
 *  Timing strategy (no hardware RTOR):
 *    Timer1 CTC mode, prescaler 8 (0.5 µs/tick at 16 MHz).
 *    Each received byte resets TCNT1 and sets OCR1A = 1.5T ticks.
 *    First COMPA ISR → 1.5T event: call cb_1_5, reload OCR1A = 2T ticks.
 *    Second COMPA ISR → 3.5T event: call cb_3_5, stop timer.
 *
 *  TX: byte-by-byte via UDRE ISR, TC (USART_TX_vect) for DE deassertion.
 *  DE: manual GPIO (PD2), set high before TX, cleared in USART_TX_vect.
 *
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "modbus_driver_interface.h"
#include "modbus_driver_config.h"
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

static driver_subscr_cb_t master_msg_tx_complete_cb = NULL;
static driver_subscr_cb_t master_t_1_5_char_break_cb = NULL;
static driver_subscr_cb_t master_t_3_5_char_break_cb = NULL;
static driver_subscr_cb_t master_frame_error_cb = NULL;

static volatile driver_timer_status_t FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;

static modbus_req_resp_t *rx_msg = NULL;
static volatile tx_buf_t tx_buf;

static uint16_t ticks_1_5T;
static uint16_t ticks_2T;

static void master_usart_init(baud_t baud, parity_t parity);
static void master_usart_send(volatile const modbus_buf_t *tx_msg, modbus_buf_size_t msg_len);
static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void master_disable_usart_rx_interrupt(void);
static void master_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void master_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void master_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void master_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static void USART_GPIO_init(void);
static void USART_module_config(baud_t baud, parity_t parity);
static void master_TIMER_init(baud_t baud);
static uint16_t get_1_5T_ticks(baud_t baud);
static uint16_t get_2T_ticks(baud_t baud);

static const modbus_RTU_driver_struct_t master_RTU_driver_interface = {
    .init = master_usart_init,
    .send = master_usart_send,
    .enable_rcev = master_enable_usart_rx_interrupt,
    .disable_rcev = master_disable_usart_rx_interrupt,
    .subscribe_t_1_5_char_expired_cb = master_t_1_5_char_expired_callback_subscribe,
    .subscribe_msg_tx_done_cb = master_msg_tx_done_callback_subscribe,
    .subscribe_t_3_5_char_expired_cb = master_t_3_5_char_expired_callback_subscribe,
    .subscribe_modbus_frame_error_cb = master_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_master_RTU_driver_interface(void)
{
    return &master_RTU_driver_interface;
}

static void master_usart_init(baud_t baud, parity_t parity)
{
    USART_GPIO_init();
    USART_module_config(baud, parity);
    master_TIMER_init(baud);
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
}

static void master_usart_send(volatile const modbus_buf_t *tx_msg, modbus_buf_size_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
        MODBUS_MASTER_USART_DE_PORT |= (1 << MODBUS_MASTER_USART_DE_PIN);
        tx_buf.cur_byte_ptr = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        UDR0 = *tx_msg;
        UCSR0B |= (1 << UDRIE0);
    }
    // stop timer and reset flag — needed for repeat request handling
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    TCCR1B &= ~(MODBUS_MASTER_TIMER_STOP_MASK);
}

static void master_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_msg = recv_buf;
    UCSR0B |= (1 << RXCIE0);
}

static void master_disable_usart_rx_interrupt(void)
{
    rx_msg = NULL;
    UCSR0B &= ~(1 << RXCIE0);
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

// ======= USART RX COMPLETE =======
ISR(USART_RX_vect)
{
    uint8_t status = UCSR0A;
    uint8_t data   = UDR0; // must read to clear RXC0; also clears FE0/DOR0/UPE0

    if (status & ((1 << FE0) | (1 << UPE0) | (1 << DOR0)))
    {
        if (master_frame_error_cb != NULL)
            master_frame_error_cb();
    }
    else
    {
        if (rx_msg != NULL)
        {
            rx_msg->data[rx_msg->len] = data;
            rx_msg->len++;
        }
        if (FRAME_RECEIVED == FRAME_DETECTION_FLAG)
        {
            if (master_frame_error_cb != NULL)
                master_frame_error_cb();
        }
    }

    TCNT1 = 0;
    OCR1A = ticks_1_5T;
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    TCCR1B |= MODBUS_MASTER_TIMER_START_MASK;
}

// ======= USART DATA REGISTER EMPTY (TXE equivalent) =======
ISR(USART_UDRE_vect)
{
    if (tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
    {
        tx_buf.cur_byte_ptr++;
        UDR0 = *tx_buf.cur_byte_ptr;
    }
    else
    {
        UCSR0B &= ~(1 << UDRIE0);
        UCSR0B |= (1 << TXCIE0);
    }
}

// ======= USART TX COMPLETE (last bit shifted out) =======
ISR(USART_TX_vect)
{
    UCSR0B &= ~(1 << TXCIE0);
    MODBUS_MASTER_USART_DE_PORT &= ~(1 << MODBUS_MASTER_USART_DE_PIN);
    if (master_msg_tx_complete_cb != NULL)
        master_msg_tx_complete_cb();
}

// ======= TIMER1 COMPARE MATCH A (1.5T / 3.5T) =======
ISR(TIMER1_COMPA_vect)
{
    if (WAITING_FOR_FRAME == FRAME_DETECTION_FLAG)
    {
        FRAME_DETECTION_FLAG = FRAME_RECEIVED;
        OCR1A = ticks_2T;
        if (master_t_1_5_char_break_cb != NULL)
            master_t_1_5_char_break_cb();
    }
    else
    {
        TCCR1B &= ~(MODBUS_MASTER_TIMER_STOP_MASK);
        FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
        if (master_t_3_5_char_break_cb != NULL)
            master_t_3_5_char_break_cb();
    }
}

static void USART_GPIO_init(void)
{
    MODBUS_MASTER_USART_DE_DDR  |= (1 << MODBUS_MASTER_USART_DE_PIN);
    MODBUS_MASTER_USART_DE_PORT &= ~(1 << MODBUS_MASTER_USART_DE_PIN);
}

static void USART_module_config(baud_t baud, parity_t parity)
{
    uint16_t ubrr = (uint16_t)(MODBUS_MASTER_USART_CLK_FREQ / (16UL * baud) - 1);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    uint8_t ucsrc = (1 << UCSZ01) | (1 << UCSZ00);
    if (NONE == parity)
    {
        UCSR0C = ucsrc | (1 << USBS0);
    }
    else if (ODD == parity)
    {
        UCSR0C = ucsrc | (1 << UPM01) | (1 << UPM00);
    }
    else
    {
        UCSR0C = ucsrc | (1 << UPM01);
    }

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

static void master_TIMER_init(baud_t baud)
{
    ticks_1_5T = get_1_5T_ticks(baud);
    ticks_2T   = get_2T_ticks(baud);

    TCCR1A = 0;
    TCCR1B = (1 << WGM12); // CTC mode, stopped
    TCNT1  = 0;
    OCR1A  = ticks_1_5T;
    TIMSK1 = (1 << OCIE1A);
}

static uint16_t get_1_5T_ticks(baud_t baud)
{
    uint32_t time_us;
    if (baud <= 19200)
        time_us = 16500000UL / baud;
    else
        time_us = 750;

    return (uint16_t)(time_us * (MODBUS_MASTER_TIMER_CLK_FREQ / 1000000UL) / MODBUS_MASTER_TIMER_PRESCALER);
}

static uint16_t get_2T_ticks(baud_t baud)
{
    uint32_t time_us;
    if (baud <= 19200)
        time_us = 22000000UL / baud;
    else
        time_us = 1000;

    return (uint16_t)(time_us * (MODBUS_MASTER_TIMER_CLK_FREQ / 1000000UL) / MODBUS_MASTER_TIMER_PRESCALER);
}
