/**
 * @file modbus_master.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 0.0.1
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_master.h"
#include "modbus_master_PDU.h"
#include "modbus_queue.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"

#include <stdio.h>

#ifdef UNIT_TEST
#define PRIVATE
#else
#define PRIVATE static
#endif

PRIVATE modbus_master_state_t master_manager_state_machine = MODBUS_MASTER_IDLE;

PRIVATE const struct modbus_RTU_driver_struct *RTU_driver = NULL;

static modbus_queue_t master_msg_queue;
PRIVATE modbus_queue_t *msg_q=&master_msg_queue;
PRIVATE modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

PRIVATE modbus_msg_t * msg_buf = NULL;

static void register_msg_resq_resp_data_buffers(modbus_mode_t mode);
static void register_master_msg_queue(void);

void register_app_data_to_modbus_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Master_Coils,coil_adr,app_data_ptr);
}
void register_app_data_to_modbus_master_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Master_Discrete_Inputs,disin_adr,app_data_ptr);
}
void register_app_data_to_modbus_master_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Master_Input_Registers,reg_adr,app_data_ptr);
}
void register_app_data_to_modbus_master_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Master_Holding_Registers,reg_adr,app_data_ptr);
}


// modbus_master_error_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
// {
//     return 0;
// }
// modbus_master_error_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
// {
//     return 0;
// }
// modbus_master_error_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
// {
//     return 0;
// }

modbus_master_error_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty,modbus_device_ID_t slave_ID)
{
    msg_buf =  modbus_queue_pop(msg_q);
    modbus_ret_t modbus_lib_ret;

    // if(NULL == msg_buf)
    // {
    //     return MODBUS_MASTER_QUEUE_FULL_ERR;
    // }
    modbus_lib_ret = modbus_master_read_holding_reg_req(msg_buf,adr,hreg_qty);
    // if(0 >= lib_ret)
    // {

    //     return MODBUS_MASTER_REQ_LIB_ERROR;
    // }
    printf("\r\n test\r\n");
    uint8_t len = msg_buf->req.len;
    printf("msg_len = %d",len);
    // modbus_lib_ret= modbus_RTU_send(&msg_buf->req.data,msg_buf->req.len,slave_ID);
    // modbus_RTU_send(&msg_buf->req.data,5,slave_ID);
    // if(RET_ERROR == modbus_lib_ret)
    // {
    //     return MODBUS_MASTER_LIB_RTU_SEND_ERROR;
    // }
    // RTU_driver->send(msg_buf->req.data,msg_buf->req.len);
    // RTU_driver->enable_rcev(msg_buf->resp.data);
    return MODBUS_MASTER_REQUEST_SEND;
}
// modbus_master_error_t modbus_master_write_single_reg(modbus_adr_t adr)
// {
//     return 0;
// }
// modbus_master_error_t modbus_master_write_single_coil(modbus_adr_t adr)
// {
//     return 0;
// }
// modbus_master_error_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
// {
//     return 0;
// }
// modbus_master_error_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
// {
//     return 0;
// }
void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    register_msg_resq_resp_data_buffers(mode);
    register_master_msg_queue();
    
    RTU_driver=get_RTU_driver_interface();
    RTU_driver->init(baud_rate,parity);

    // ToDo rcv_callback ?? do we need this
}
void modbus_update_modbus_manager(void)
{
    // siwtch (master_manager_state)
    // {
    // case MODBUS_MASTER_IDLE:
    //     // sprawdz czy na kolejce jest bufor do wyslania 
    //     // jeśli nie to nie break
    //     // jeśli tak wyslij bufor i przejdź do nastepnego stanu
    //     break;
    // case MODBUS_MASTER_TRANSMISION:
    //     // sprawdz czy drier przestal wysylac i jesli tak to przejdz do odbioru
    //     break;
    // case MODBUS_MASTER_RECEIVES:
    //     break; 
    // // pytanie czy tu nie powinno być stanu przejściowego na oczekiwania poprawności zależności czasowej
    // case MODBUS_MASTER_RESP_ANALYSE:
    //     // analizuje ramkę i wykonuje zadanie
    //     // jesli dostałem error lub jest coś nie tak idę do error service
    //     break;
    // case MODBUS_MASTER_ERROR_SERVICE:
    //     // W zależności od tego co było błędem wykonuję jego obsługę
    //     break;

    // }
}

static void register_msg_resq_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        for (uint8_t i=0; i<MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
        {
            modbus_msg[i].req.data= &RTU_req_buf[i][0];
            modbus_msg[i].resp.data= &RTU_resp_buf[i][0];
        }
    }
}

static void register_master_msg_queue(void)
{
    for (uint8_t i=0; i<MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        master_msg_queue.modbus_msg[i]=&modbus_msg[i];
    }
}