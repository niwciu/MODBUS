/**
 * @file modbus_slave_driver_interface.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2025-08-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "modbus_driver_interface.h"
#include "modbus_slave_driver_config.h"
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

static driver_subscr_cb_t slave_msg_tx_complete_cb = NULL;
static driver_subscr_cb_t slave_t_1_5_char_break_cb = NULL;
static driver_subscr_cb_t slave_t_3_5_char_break_cb = NULL;
static driver_subscr_cb_t slave_frame_error_cb = NULL;

static volatile driver_timer_status_t FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;

static modbus_req_resp_t *rx_msg = NULL;
static volatile tx_buf_t  tx_buf;

static uint16_t ticks_1_5T;
static uint16_t ticks_2T;

static void slave_usart_init(baud_t baud, parity_t parity);
static void slave_usart_send(volatile const modbus_buf_t *tx_msg, volatile modbus_buf_size_t msg_len);
static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf);
static void slave_disable_usart_rx_interrupt(void);
static void slave_t_1_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_tx_done_callback_subscribe(driver_subscr_cb_t callback);
static void slave_t_3_5_char_expired_callback_subscribe(driver_subscr_cb_t callback);
static void slave_msg_frame_erroro_callback_subscribe(driver_subscr_cb_t callback);

static void USART_GPIO_init(void);
static void USART_module_config(baud_t baud, parity_t parity);
static void slave_TIMER_init(baud_t baud);
static uint16_t get_1_5T_ticks(baud_t baud);
static uint16_t get_2T_ticks(baud_t baud);

static const modbus_RTU_driver_struct_t slave_RTU_driver_interface = {
    .init                          = slave_usart_init,
    .send                          = slave_usart_send,
    .enable_rcev                   = slave_enable_usart_rx_interrupt,
    .disable_rcev                  = slave_disable_usart_rx_interrupt,
    .subscribe_t_1_5_char_expired_cb  = slave_t_1_5_char_expired_callback_subscribe,
    .subscribe_msg_tx_done_cb         = slave_msg_tx_done_callback_subscribe,
    .subscribe_t_3_5_char_expired_cb  = slave_t_3_5_char_expired_callback_subscribe,
    .subscribe_modbus_frame_error_cb  = slave_msg_frame_erroro_callback_subscribe,
};

const modbus_RTU_driver_struct_t *get_slave_RTU_driver_interface(void)
{
    return &slave_RTU_driver_interface;
}

static void slave_usart_init(baud_t baud, parity_t parity)
{
    USART_GPIO_init();
    USART_module_config(baud, parity);
    slave_TIMER_init(baud);
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
}

static void slave_usart_send(volatile const modbus_buf_t *tx_msg, volatile modbus_buf_size_t msg_len)
{
    if ((tx_msg != NULL) && (msg_len > 0))
    {
#if SLAVE_USART_DE_HW_CONTROLL == OFF
        MODBUS_SLAVE_USART_DE_PORT |= (1 << MODBUS_SLAVE_USART_DE_PIN);
#endif
        tx_buf.cur_byte_ptr  = tx_msg;
        tx_buf.last_byte_ptr = tx_msg + (msg_len - 1);
        UDR0 = *tx_msg;
        UCSR0B |= (1 << UDRIE0);
    }
    // disable timer and clear all flags -> needed for repeat request handle
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    TCCR1B &= ~(MODBUS_SLAVE_TIMER_STOP_MASK);
}

static void slave_enable_usart_rx_interrupt(modbus_req_resp_t *recv_buf)
{
    rx_msg = recv_buf;
    UCSR0B |= (1 << RXCIE0);
}

static void slave_disable_usart_rx_interrupt(void)
{
    rx_msg = NULL;
    UCSR0B &= ~(1 << RXCIE0);
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

// ======= USART RX COMPLETE =======
ISR(USART_RX_vect)
{
    uint8_t status = UCSR0A;
    uint8_t data   = UDR0; // must read to clear RXC0; also clears FE0/DOR0/UPE0

    if (status & ((1 << FE0) | (1 << UPE0) | (1 << DOR0)))
    {
        if (slave_frame_error_cb != NULL)
            slave_frame_error_cb();
    }
    else
    {
        if (rx_msg != NULL)
        {
            rx_msg->data[rx_msg->len] = data;
            rx_msg->len++;
        }
        else
        {
            (void)data;
        }
        if (FRAME_RECEIVED == FRAME_DETECTION_FLAG)
        {
            if (slave_frame_error_cb != NULL)
                slave_frame_error_cb();
        }
    }

    // reset 1.5T timer on every byte (CTC: TCNT1 resets on match, so just reload OCR1A and restart)
    TCNT1 = 0;
    OCR1A = ticks_1_5T;
    FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
    TCCR1B |= MODBUS_SLAVE_TIMER_START_MASK;
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
#if SLAVE_USART_DE_HW_CONTROLL == OFF
    MODBUS_SLAVE_USART_DE_PORT &= ~(1 << MODBUS_SLAVE_USART_DE_PIN);
#endif
    if (slave_msg_tx_complete_cb != NULL)
        slave_msg_tx_complete_cb();
}

// ======= TIMER1 COMPARE MATCH A (1.5T / 3.5T two-phase) =======
ISR(TIMER1_COMPA_vect)
{
    if (WAITING_FOR_FRAME == FRAME_DETECTION_FLAG)
    {
        // 1.5T elapsed — reload for 2T more to reach 3.5T total
        FRAME_DETECTION_FLAG = FRAME_RECEIVED;
        OCR1A = ticks_2T;
        if (slave_t_1_5_char_break_cb != NULL)
            slave_t_1_5_char_break_cb();
    }
    else
    {
        // 3.5T elapsed — stop timer
        TCCR1B &= ~(MODBUS_SLAVE_TIMER_STOP_MASK);
        FRAME_DETECTION_FLAG = WAITING_FOR_FRAME;
        if (slave_t_3_5_char_break_cb != NULL)
            slave_t_3_5_char_break_cb();
    }
}

static void USART_GPIO_init(void)
{
    // TX (PD1) and RX (PD0) direction is set automatically by TXEN0/RXEN0 in USART
    MODBUS_SLAVE_USART_DE_DDR  |= (1 << MODBUS_SLAVE_USART_DE_PIN);
    MODBUS_SLAVE_USART_DE_PORT &= ~(1 << MODBUS_SLAVE_USART_DE_PIN); // DE low (RX mode)
}

static void USART_module_config(baud_t baud, parity_t parity)
{
    uint16_t ubrr = (uint16_t)(MODBUS_SLAVE_USART_CLK_FREQ / (16UL * baud) - 1);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    uint8_t ucsrc = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits
    if (NONE == parity)
    {
        UCSR0C = ucsrc | (1 << USBS0); // 8N2 (Modbus: 2 stop bits when no parity)
    }
    else if (ODD == parity)
    {
        UCSR0C = ucsrc | (1 << UPM01) | (1 << UPM00); // 8O1
    }
    else
    {
        UCSR0C = ucsrc | (1 << UPM01); // 8E1
    }

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

static void slave_TIMER_init(baud_t baud)
{
    ticks_1_5T = get_1_5T_ticks(baud);
    ticks_2T   = get_2T_ticks(baud);

    TCCR1A = 0;
    TCCR1B = (1 << WGM12); // CTC mode, timer stopped (no clock source yet)
    TCNT1  = 0;
    OCR1A  = ticks_1_5T;
    TIMSK1 = (1 << OCIE1A); // enable compare A interrupt
}

static uint16_t get_1_5T_ticks(baud_t baud)
{
    uint32_t time_us;
    if (baud <= 19200)
        time_us = 16500000UL / baud; // 11 bits * 1.5 char-times in µs
    else
        time_us = 750; // fixed minimum per Modbus spec

    return (uint16_t)(time_us * (MODBUS_SLAVE_TIMER_CLK_FREQ / 1000000UL) / MODBUS_SLAVE_TIMER_PRESCALER);
}

static uint16_t get_2T_ticks(baud_t baud)
{
    uint32_t time_us;
    if (baud <= 19200)
        time_us = 22000000UL / baud; // 11 bits * 2 char-times in µs
    else
        time_us = 1000; // fixed minimum per Modbus spec

    return (uint16_t)(time_us * (MODBUS_SLAVE_TIMER_CLK_FREQ / 1000000UL) / MODBUS_SLAVE_TIMER_PRESCALER);
}
