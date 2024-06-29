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
#include "modbus_master_RTU.h"
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
PRIVATE const modbus_RTU_driver_struct_t *RTU_driver = NULL;

static modbus_queue_t master_free_queue;
static modbus_queue_t master_tx_rx_queue;

PRIVATE modbus_queue_t *free_q = &master_free_queue;
PRIVATE modbus_queue_t *tx_rx_q = &master_tx_rx_queue;
PRIVATE modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];
PRIVATE modbus_msg_t *msg_buf = NULL;

// PRIVATE modbus_status_flag_t TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
// PRIVATE modbus_status_flag_t TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
// PRIVATE modbus_status_flag_t FRAME_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_UNKNOWN;

static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void push_all_available_msg_buffer_to_free_queue(void);
static modbus_master_error_t generate_request(modbus_fun_code_t fun_code, modbus_adr_t adr, modbus_data_qty_t obj_qty, modbus_device_ID_t slave_ID);
static modbus_ret_t add_PDU_request_data(modbus_msg_t *msg_buf, modbus_fun_code_t fun_code, modbus_adr_t adr, modbus_data_qty_t obj_qty);
static modbus_ret_t modbus_master_write_single_coil_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);
static modbus_ret_t modbus_master_write_single_reg_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);

static void modbus_master_req_sended_callback(void);

typedef modbus_ret_t (*modbus_master_fun_code_handler_t)(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);
struct modbus_master_functions_mapper
{
    modbus_fun_code_t fun_code;
    modbus_master_fun_code_handler_t fun_code_action;
};
const struct modbus_master_functions_mapper master_functions_mapper[] = {
    {MODBUS_READ_COILS_FUNC_CODE, modbus_master_read_coils_req},
    {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, modbus_master_read_discrete_inputs_req},
    {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, modbus_master_read_holding_reg_req},
    {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, modbus_master_read_input_reg_req},
    {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, modbus_master_write_single_coil_req_wrapper},
    {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, modbus_master_write_single_reg_req_wrapper},
    {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, modbus_master_write_multiple_coils_req},
    {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, modbus_master_write_multiple_reg_req},
};

#define MODBUS_MASTER_FUNCTIONS_MAPPER_SIZE (sizeof(master_functions_mapper) / sizeof(master_functions_mapper[0]));

void register_app_data_to_modbus_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Master_Coils, coil_adr, app_data_ptr);
}
void register_app_data_to_modbus_master_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Master_Discrete_Inputs, disin_adr, app_data_ptr);
}
void register_app_data_to_modbus_master_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Master_Input_Registers, reg_adr, app_data_ptr);
}
void register_app_data_to_modbus_master_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Master_Holding_Registers, reg_adr, app_data_ptr);
}

modbus_master_error_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty, slave_ID);
}

modbus_master_error_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty, slave_ID);
}

modbus_master_error_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty, slave_ID);
}

modbus_master_error_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty, slave_ID);
}
modbus_master_error_t modbus_master_write_single_coil(modbus_adr_t adr, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr, 0, slave_ID);
}
modbus_master_error_t modbus_master_write_single_reg(modbus_adr_t adr, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr, 0, slave_ID);
}
modbus_master_error_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, adr, reg_qty, slave_ID);
}
modbus_master_error_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID)
{
    return generate_request(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, adr, coils_qty, slave_ID);
}
void modbus_master_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    register_msg_req_resp_data_buffers(mode);
    modbus_queue_init(free_q);
    modbus_queue_init(tx_rx_q);
    push_all_available_msg_buffer_to_free_queue();

    RTU_driver = get_master_RTU_driver_interface();
    RTU_driver->init(baud_rate, parity);

    // ToDo registration of all callbacks
    RTU_driver->subscribe_msg_tx_done_cb(modbus_master_req_sended_callback);
    // ToDo setting up all flags ans modus_master_stateMachine
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}
void check_modbus_master_manager(void)
{
    switch (master_manager_state_machine)
    {
    case MODBUS_MASTER_IDLE:
        if ((tx_rx_q->head != tx_rx_q->tail) || (LAST_QUEUE_POS_STORE_DATA == tx_rx_q->last_queue_pos_status)) //ToDo refacotr for check condition function
        {
            msg_buf = modbus_queue_pop(tx_rx_q);
            RTU_driver->send(msg_buf->req.data, msg_buf->req.len);
            RTU_driver->enable_rcev(&msg_buf->resp);
            MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_SET;
            master_manager_state_machine = MODBUS_MASTER_TRANSMITTING_REQ;
        }
        break;
    case MODBUS_MASTER_TRANSMITTING_REQ:
        // sprawdz czy driver przestal wysylac i jesli tak to przejdz do odbioru
        // realizacja przez zdefiniowanie flai i callbacka który ustawia tą flagę przez driver który skończył wysyłać.
        // jak flaga zmieni status to przechodzę do następnego stanu
        break;
    case MODBUS_MASTER_RECEIVING_RESP:
        break;
        //     // pytanie czy tu nie powinno być stanu przejściowego na oczekiwania poprawności zależności czasowej
        //     // case MODBUS_WAIT_3_5_CHAR:

        // case MODBUS_MASTER_RESP_ANALYSE:
        //     // analizuje ramkę i wykonuje zadanie
        //     // jesli dostałem error lub jest coś nie tak idę do error service
        //     break;
        // case MODBUS_MASTER_ERROR_SERVICE:
        //     // W zależności od tego co było błędem wykonuję jego obsługę
        //     break;
    }
}

static void register_msg_req_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
        {
            modbus_msg[i].req.data = &master_RTU_req_buf[i][0];
            modbus_msg[i].resp.data = &master_RTU_resp_buf[i][0];
        }
    }
    // else if( OTHER_MODE == mode)
    else
    {
        for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
        {
            modbus_msg[i].req.data = NULL;
            modbus_msg[i].resp.data = NULL;
        }
    }
}

static void push_all_available_msg_buffer_to_free_queue(void)
{
    for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_push(free_q, &modbus_msg[i]);
    }
}

static modbus_master_error_t generate_request(modbus_fun_code_t fun_code, modbus_adr_t adr, modbus_data_qty_t obj_qty, modbus_device_ID_t slave_ID)
{
    msg_buf = modbus_queue_pop(free_q);
    modbus_ret_t modbus_lib_ret;

    if (NULL == msg_buf)
    {
        return MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR;
    }
    modbus_lib_ret = add_PDU_request_data(msg_buf, fun_code, adr, obj_qty);
    if (0 > modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_REQ_ERROR;
    }
    modbus_lib_ret = modbus_RTU_send(msg_buf->req.data, &msg_buf->req.len, slave_ID);
    if (RET_ERROR == modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_RTU_SEND_ERROR;
    }
    modbus_queue_push(tx_rx_q, msg_buf);
    return MODBUS_MASTER_REQUEST_SEND;
}

static modbus_ret_t add_PDU_request_data(modbus_msg_t *msg_buf, modbus_fun_code_t fun_code, modbus_adr_t adr, modbus_data_qty_t obj_qty)
{
    modbus_ret_t PDU_ret_status = RET_ERROR_UNKNOWN_MAPPER_FUN_CODE;
    uint32_t master_mapper_size = MODBUS_MASTER_FUNCTIONS_MAPPER_SIZE;
    for (uint32_t i = 0; i < master_mapper_size; i++)
    {
        if (master_functions_mapper[i].fun_code == fun_code)
        {
            PDU_ret_status = master_functions_mapper[i].fun_code_action(msg_buf, adr, obj_qty);
        }
    }
    return PDU_ret_status;
}

static modbus_ret_t modbus_master_write_single_coil_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    (void)(coils_qty);
    return modbus_master_write_single_coil_req(modbus_msg, adr);
}
static modbus_ret_t modbus_master_write_single_reg_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    (void)(coils_qty);
    return modbus_master_write_single_reg_req(modbus_msg, adr);
}

static void modbus_master_req_sended_callback(void)
{
    // MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

// static void modbus_T_1_5_char_expired_callback(void)
// {
//     TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
// }

// static void modbus_T_3_5_char_expired_callback(void)
// {
//     TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
// }

// static void modbus_frame_error_callback(void)
// {
//     FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
// }
