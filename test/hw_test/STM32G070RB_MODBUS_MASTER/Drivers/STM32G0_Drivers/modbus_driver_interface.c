/**
 * @file usart.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */


#include "modbus_driver_interface.h"
#include "modbus_driver_config.h"
#include <stddef.h>

typedef struct
{
    modbus_buf_t *cur_byte_ptr;
    modbus_buf_t *last_byte_ptr;
} tx_buf_t;
typedef struct
{
    modbus_buf_t data[RX_DATA_BUF_SIZE];
    uint8_t data_idx;
} rx_buf_t;

rx_buf_t rx_buf;
tx_buf_t tx_buf;

rx_cb_t rx_callback = NULL;

static void usart_init(uint32_t Baud, parity_t parity);
static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void enable_usart_rx_interrupt(void);
static void disable_usart_rx_interrupt(void);
static void uasrt_subscribe_rx_callback(rx_cb_t callback);
const struct modbus_RTU_driver_struct *get_RTU_driver_interface(void);

static void USART_GPIO_init(void);
static void USART_module_config(uint32_t Baud, parity_t parity);
static uint32_t get_RTOR_value(uint32_t baud);
static void USART_data_init(void);
static void USART_set_and_enable_interrupts(void);

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
    USART_data_init();
    USART_set_and_enable_interrupts();
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
    rx_callback=callback;
}

void MODBUS_USART_IRQHandler(void)
{
    // ToDo check in documentation which interrupt need to be also take cared -> errors overrun etc
    if ((MODBUS_USART->ISR) & USART_ISR_RXNE_RXFNE)
    {
        rx_buf.data[rx_buf.data_idx] = MODBUS_USART->RDR;
        rx_buf.data_idx++;
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
            MODBUS_USART->CR1 &= ~(USART_CR1_TXEIE_TXFNFIE);
        }
    }
    if ((MODBUS_USART->ISR) & USART_ISR_RTOF)
    {
        if( NULL != rx_callback)
        {
            rx_callback(rx_buf.data,rx_buf.data_idx);
        }
        rx_buf.data_idx = 0;
        MODBUS_USART ->ICR |= USART_ICR_RTOCF;
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
    // set modbus delay for recognizing end of frame
    MODBUS_USART -> RTOR |= get_RTOR_value(Baud);
    // enable usart module
    MODBUS_USART->CR1 |= (USART_CR1_UE);
}
static uint32_t get_RTOR_value(uint32_t baud)
{
    if(baud<=19200)
        return 38;
    else
    {
        uint32_t q=baud/9600;
        q=q*168;
        q=q/10;
        if(q>0xFFFFFF) q=0xFFFFFF;
        return q;
    }
}
static void USART_data_init(void)
{
   rx_buf.data_idx = 0;
}

static void USART_set_and_enable_interrupts(void)
{
    NVIC_SetPriority(MODBUS_USART_IRQN, MODBUS_USART_IRQ_PRIORITY);
    NVIC_EnableIRQ(MODBUS_USART_IRQN); 
}