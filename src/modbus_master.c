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
#include "modbus_PDU.h"
#include "modbus_queue.h"
#include "modbus_RTU.h"

#include <stdio.h>


static modbus_master_state_t master_manager_state;
static modbus_queue_t master_msg_queue;

static modbus_msg_t modbus_msg[MAX_MODBUS_MSG_ITEMS]; // w inicie przypisuje do modbus_msg.req.data adres na bufor req RTU i modbus_msg.resp.data adres resp RTU
                                                      // do tego init kolejki ktory podpina wskaźniki modbus_msg



void register_app_data_to_modbus_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_din_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    
}
void register_app_data_to_modbus_master_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    
}

modbus_master_error_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty,modbus_device_ID_t slave_ID)
{
    modbus_master_error_t status;
    modbus_msg_t * msg_buf =  modbus_queue_pop();
    modbus_ret_t modbus_lib_ret;

    if(NULL == msg_buf)
    {
        return MODBUS_MASTER_QUEUE_FULL_ERR;
    }
    modbus_ret = modbus_master_read_holding_reg_req(msg_buf->req,adr,hreg_qty);
    if(0 >= lib_ret)
    {

        return MODBUS_MASTER_REQ_LIB_ERROR;
    }
    modbus_lib_ret= modbus_RTU_send(msg_buf->req,modbus_ret,slave_ID);
    if(RET_ERROR == modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_RTU_SEND_ERROR;
    }
    return status;
}
modbus_master_error_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return 0;
}
modbus_master_error_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    return 0;
}
modbus_master_error_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return 0;
}
modbus_master_error_t modbus_master_write_single_reg(modbus_adr_t adr)
{
    return 0;
}
modbus_master_error_t modbus_master_write_single_coil(modbus_adr_t adr)
{
    return 0;
}
modbus_master_error_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return 0;
}
modbus_master_error_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return 0;
}
void modbus_master_init(void)
{

}
void modbus_update_modbus_manager(void)
{
    siwtch (master_manager_state)
    {
    case MODBUS_MASTER_IDLE:
        // sprawdz czy na kolejce jest bufor do wyslania 
        // jeśli nie to nie break
        // jeśli tak wyslij bufor i przejdź do nastepnego stanu
        break;
    case MODBUS_MASTER_TRANSMISION:
        // sprawdz czy drier przestal wysylac i jesli tak to przejdz do odbioru
        break;
    case MODBUS_MASTER_RECEIVES:
        break; 
    // pytanie czy tu nie powinno być stanu przejściowego na oczekiwania poprawności zależności czasowej
    case MODBUS_MASTER_RESP_ANALYSE:
        // analizuje ramkę i wykonuje zadanie
        // jesli dostałem error lub jest coś nie tak idę do error service
        break;
    case MODBUS_MASTER_ERROR_SERVICE:
        // W zależności od tego co było błędem wykonuję jego obsługę
        break;

    }
}