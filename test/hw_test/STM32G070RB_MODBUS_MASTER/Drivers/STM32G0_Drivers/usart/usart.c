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

#define USART3_BAUD_RATE 9600U
#define APB1_CLK_FREQ 64000000UL
#define DEFAULT_IRQ_PRIOTITY 10

#define RX_DATA_BUF_SIZE 256

typedef struct 
{
    uint8_t *cur_byte_ptr;
    uint8_t *last_byte_ptr;
}tx_buf_t ;
typedef struct 
{
    uint8_t data[RX_DATA_BUF_SIZE];
    uint8_t data_idx;
}rx_buf_t;

rx_buf_t rx_buf;
tx_buf_t tx_buf;

static void usart_init(uint32_t Baud, parity_t parity, stop_bits_t stop_bits);
static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len);
static void enable_usart_rx_interrupt(void);
static void disable_usart_rx_interrupt(void);
static void uasrt_subscribe_rx_callback(rx_cb_t callback);
const struct modbus_RTU_driver_struct* get_RTU_driver_interface(void);

static const struct modbus_RTU_driver_struct RTU_driver = {
    usart_init,
    usart_send,
    enable_usart_rx_interrupt,
    disable_usart_rx_interrupt,
    uasrt_subscribe_rx_callback,
};

const struct modbus_RTU_driver_struct* get_RTU_driver_interface(void)
{
    return &RTU_driver;
}

static void usart_init(uint32_t Baud, parity_t parity, stop_bits_t stop_bits) //parameters for init -> baud, pariti, stop bits
{
    
    /* GPIOD for USART3 RX/TX CONFIGURATION */
    // enable clk for GPIOD
    RCC->IOPENR |= RCC_IOPENR_GPIODEN;
    // config gpiod mode
    GPIOD->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
    GPIOD->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
    // set alternate function for GPIOD8 & GPIO D9
    GPIOD->AFR[1] &= ~(GPIO_AFRH_AFSEL8_Msk | GPIO_AFRH_AFSEL9_Msk);

    /* USART 3 CONFIGURATION 9600 Bps , 8 bit data, 1 bit parity, 1 stop bit , pariti even*/
    RCC-> APBENR1 |=RCC_APBENR1_USART3EN; //clk enable
    USART3 -> CR1 |= (USART_CR1_M0 | USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_PCE ) ; //
    USART3 -> BRR = APB1_CLK_FREQ/USART3_BAUD_RATE;

    NVIC_SetPriority(USART3_4_IRQn,DEFAULT_IRQ_PRIOTITY);
    NVIC_EnableIRQ(USART3_4_IRQn);
    // ToDo init rx tx buffers as a function
    rx_buf.data_idx=0;

    /* Timer configuration for measuring 1,5 and 3,5 char*/
    //ToDo get ARR value fo 3,5 char depanding on Baud Rate
    RCC-> APBENR1 |= RCC_APBENR1_TIM7EN;
    TIM7 -> CR1 |= TIM_CR1_OPM | TIM_CR1_URS;
    TIM7 -> DIER  = TIM_DIER_UIE;
    TIM7 -> PSC = APB1_CLK_FREQ/1000000-1;
    TIM7 -> ARR = 1750; 

    NVIC_SetPriority(TIM7_IRQn,DEFAULT_IRQ_PRIOTITY);
    NVIC_EnableIRQ(TIM7_IRQn);
    
}

static void usart_send(modbus_buf_t *tx_msg, uint8_t msg_len)
{
    if((tx_msg != NULL)&&(msg_len>0))
    {
        tx_buf.cur_byte_ptr=tx_msg;
        tx_buf.last_byte_ptr=tx_msg+(msg_len-1);
        USART3 -> TDR= *tx_msg;
        USART3 -> CR1 |=(USART_CR1_TXEIE_TXFNFIE); 
    }
    
}
static void enable_usart_rx_interrupt(void)
{
    USART3 -> CR1 |=  USART_CR1_RXNEIE_RXFNEIE;
}
static void disable_usart_rx_interrupt(void)
{
    USART3 -> CR1 &=  ~USART_CR1_RXNEIE_RXFNEIE;
}

static void uasrt_subscribe_rx_callback(rx_cb_t callback)
{

}
void USART3_USART4_LPUART1_IRQHandler(void)
{
    //ToDo check in documentation which interrupt need to be also take cared -> errors overrun etc
    if((USART3 ->ISR )&USART_ISR_RXNE_RXFNE)
    {
        // ToDo check if you are not between 1,5 char and 3,5 char timing (frame error)
        //if frame error accure set error flag and skip collecting data in rx buf
        rx_buf.data[rx_buf.data_idx]= USART3 ->RDR;
        rx_buf.data_idx++;
        TIM7 ->EGR |= TIM_EGR_UG;
        TIM7 ->CR1 |= TIM_CR1_CEN;
        
    }
    else if((USART3 ->ISR)&USART_ISR_TXE_TXFNF)
    {
       
       if(tx_buf.cur_byte_ptr < tx_buf.last_byte_ptr)
       {
            tx_buf.cur_byte_ptr++;
            USART3 -> TDR= *tx_buf.cur_byte_ptr; 
       }
       else
       {
            USART3 -> CR1 &=~(USART_CR1_TXEIE_TXFNFIE); 
       }
    }
}

void TIM7_LPTIM2_IRQHandler(void)
{
    if(TIM7 ->SR & TIM_SR_UIF)
    {
        TIM7 ->SR &= ~TIM_SR_UIF;
        rx_buf.data_idx=0;
        //if error error flag set do nothign
        // else pass the rx buf to rsp msg or set some flag to indicate that there are some data to parse
    }
}