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

PRIVATE modbus_master_state_t modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
PRIVATE const modbus_RTU_driver_struct_t *RTU_driver = NULL;
PRIVATE modbus_master_error_cb_t modbus_error_callback = NULL;

static modbus_queue_t master_free_queue;
static modbus_queue_t master_tx_rx_queue;

PRIVATE modbus_queue_t *free_q = &master_free_queue;
PRIVATE modbus_queue_t *tx_rx_q = &master_tx_rx_queue;
PRIVATE modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];
PRIVATE modbus_msg_t *msg_buf = NULL;
PRIVATE modbus_timer_t modbus_master_resp_timeout = 0;
PRIVATE uint8_t modbus_master_msg_repeat_couter = 0;

PRIVATE modbus_status_flag_t MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_UNKNOWN;

static void modbus_master_send_req_from_msg_buf(void);
static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void push_all_available_msg_buffer_to_free_queue(void);
static modbus_master_req_ret_t generate_request(req_input_param_struct_t *req_param);
static modbus_ret_t generate_request_PDU_data(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param);
static modbus_ret_t modbus_master_write_single_coil_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);
static modbus_ret_t modbus_master_write_single_reg_req_wrapper(modbus_msg_t *modbus_msg, modbus_adr_t adr, modbus_data_qty_t coils_qty);
static void modbus_master_enable_resp_timeout_timer(void);
static void modbus_master_disable_resp_timeout_timer(void);

static void modbus_master_req_sended_callback(void);
static void modbus_master_T_1_5_char_expired_callback(void);
static void modbus_master_T_3_5_char_expired_callback(void);
static void modbus_master_frame_error_callback(void);

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

void register_modbus_master_error_cb(modbus_master_error_cb_t error_callback)
{
    modbus_error_callback=error_callback;
}

modbus_master_req_ret_t modbus_master_read_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
}

modbus_master_req_ret_t modbus_master_read_discrete_inputs(modbus_adr_t adr, modbus_data_qty_t discrete_input_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
}

modbus_master_req_ret_t modbus_master_read_input_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
}

modbus_master_req_ret_t modbus_master_read_holding_reg(modbus_adr_t adr, modbus_data_qty_t hreg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
}
modbus_master_req_ret_t modbus_master_write_single_coil(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_coil_disin_t coil_2_write)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr, 0, slave_ID, (void *)(&coil_2_write)};
    return generate_request(&req_input_param);
}
modbus_master_req_ret_t modbus_master_write_single_reg(modbus_adr_t adr, modbus_device_ID_t slave_ID, modbus_reg_t reg_2_write)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr, 0, slave_ID, (void *)(&reg_2_write)};
    return generate_request(&req_input_param);
}
modbus_master_req_ret_t modbus_master_write_multiple_reg(modbus_adr_t adr, modbus_data_qty_t reg_qty, modbus_device_ID_t slave_ID, modbus_reg_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, adr, reg_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
}
modbus_master_req_ret_t modbus_master_write_multiple_coils(modbus_adr_t adr, modbus_data_qty_t coils_qty, modbus_device_ID_t slave_ID, modbus_coil_disin_t *rw_data_ptr)
{
    req_input_param_struct_t req_input_param = {MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, adr, coils_qty, slave_ID, (void *)(rw_data_ptr)};
    return generate_request(&req_input_param);
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
    RTU_driver->subscribe_t_1_5_char_expired_cb(modbus_master_T_1_5_char_expired_callback);
    RTU_driver->subscribe_t_3_5_char_expired_cb(modbus_master_T_3_5_char_expired_callback);
    RTU_driver->subscribe_modbus_frame_error_cb(modbus_master_frame_error_callback);
    // ToDo setting up all flags ans modus_master_stateMachine
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    // set all internall variable to its default values
    modbus_master_manager_state_machine = MODBUS_MASTER_IDLE;
    modbus_master_msg_repeat_couter = 0;        // ToDo init test for this var
    modbus_master_disable_resp_timeout_timer(); // ToDo init test for this var
}
void check_modbus_master_manager(void)
{
    switch (modbus_master_manager_state_machine)
    {
    case MODBUS_MASTER_IDLE:
        if ((tx_rx_q->head != tx_rx_q->tail) || (LAST_QUEUE_POS_STORE_DATA == tx_rx_q->last_queue_pos_status)) // ToDo refacotr for check condition function
        {
            msg_buf = modbus_queue_pop(tx_rx_q);
            modbus_master_send_req_from_msg_buf();
        }
        break;
    case MODBUS_MASTER_REPEAT_REQUEST:
        modbus_master_send_req_from_msg_buf();
        break;
    case MODBUS_MASTER_TRANSMITTING_REQ:
        if (MODBUS_FLAG_CLEARED == MODBUS_MASTER_REQ_TRANSMITION_FLAG)
        {
            modbus_master_enable_resp_timeout_timer();
            modbus_master_manager_state_machine = MODBUS_MASTER_RESP_WAITING;
        }
        break;
    case MODBUS_MASTER_RESP_WAITING:
        if (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_1_5_CHAR_FLAG)
        {
            modbus_ret_t RTU_status;
            modbus_device_ID_t modbus_req_slave_ID = msg_buf->req.data[MODBUS_SLAVE_ADR_IDX];
            MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
            MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
            RTU_status = modbus_RTU_recv(msg_buf->req.data, msg_buf->req.len, modbus_req_slave_ID);
            if (RET_ERROR_CRC == RTU_status)
            {
                // update repetetion anu error counter
                // if couter > then defined max req repetition then proced to err else repeta request
                // stop response time out timer

                /****  for happy path we are setting error flag and going to next state*/
                // MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
                // modbus_master_manager_state_machine = MODBUS_MASTER_RESP_RECIVED;
                /**** happy path do not write test for this scenario **/
            }
            else if (RET_ERROR_SLAVE_ID == RTU_status)
            {
                // if slave id is wrong device stay in this state and wait for proper resp or resp timeout
                // stay in MODBUS_MASTER_RECEIVING_RESP
                // To Do Do in the future -> log event that for specific req slave id resp was catched

                // RTU_driver->disable_T3,5_timer ->  for now T_3_5_Timer_Flag is cleared when T 1_5Flag is seted in this state ->
                msg_buf->resp.len = 0; // set resp data buf ptr to first char
            }
            else // RET_OK
            {
                modbus_master_disable_resp_timeout_timer();
                modbus_master_manager_state_machine = MODBUS_MASTER_RESP_RECIVED;
            }
        }
        else if (0 == modbus_master_resp_timeout)
        {
        }
        break;
    case MODBUS_MASTER_RESP_RECIVED:
        if ((MODBUS_FLAG_SET == MODBUS_MASTER_FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_3_5_CHAR_FLAG))
        {   // frame error catched
            modbus_master_msg_repeat_couter++;
            if (MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR >= modbus_master_msg_repeat_couter)
            {
                modbus_master_manager_state_machine = MODBUS_MASTER_REPEAT_REQUEST;
                MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
            }
            else
            {
                // report error, 
                if (NULL != modbus_error_callback)
                {
                    static modbus_error_rep_t error_rep;
                    error_rep.slave_ID = modbus_msg->resp.data[MODBUS_SLAVE_ADR_IDX];
                    error_rep.fun_conde = (modbus_msg->resp.data[MODBUS_FUNCTION_CODE_IDX] & (~MODBUS_ERROR_CODE_MASK));
                    error_rep.resp_read_error = MODBUS_MASTER_RESP_FRAME_ERR;
                    modbus_error_callback(&error_rep);
                }
                // push msg to free queueue buffer and go to idle
                // memset(modbus_msg,0,sizoef(modbus_msg));
                // modbus_queue_push(free_q,modbus_msg);
            }
        }
        else if ((MODBUS_FLAG_CLEARED == MODBUS_MASTER_FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == MODBUS_MASTER_TIMER_3_5_CHAR_FLAG))
        {
            // parse slave_resp_msg
            modbus_master_read_slave_resp(msg_buf);
            modbus_master_msg_repeat_couter = 0;
            // push msg to free queue buffer
        }
        else
        {
            // do nothing untill 3_5TFlag is not set
        }
        break;
        // case MODBUS_MASTER_RESP_PROCESING:
        //     break;

        // case MODBUS_MASTER_ERROR_SERVICE:
        //     // W zależności od tego co było błędem wykonuję jego obsługę
        //     break;
    }
}

static void modbus_master_send_req_from_msg_buf(void)
{
    RTU_driver->send(msg_buf->req.data, msg_buf->req.len);
    RTU_driver->enable_rcev(&msg_buf->resp);
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_SET;
    modbus_master_manager_state_machine = MODBUS_MASTER_TRANSMITTING_REQ;
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

static modbus_master_req_ret_t generate_request(req_input_param_struct_t *req_param)
{
    msg_buf = modbus_queue_pop(free_q);
    modbus_ret_t modbus_lib_ret;
    if (NULL == msg_buf)
    {
        return MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR;
    }
    msg_buf->rw_data_ptr = NULL;
    modbus_lib_ret = generate_request_PDU_data(msg_buf, req_param);
    if (0 > modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_PDU_REQ_ERROR;
    }
    modbus_lib_ret = modbus_RTU_send(msg_buf->req.data, &msg_buf->req.len, req_param->slave_ID);
    if (RET_ERROR == modbus_lib_ret)
    {
        return MODBUS_MASTER_LIB_RTU_SEND_ERROR;
    }
    msg_buf->rw_data_ptr = req_param->rw_data_ptr;
    modbus_queue_push(tx_rx_q, msg_buf);
    return MODBUS_MASTER_REQUEST_SEND_TO_QUEUE;
}

static modbus_ret_t generate_request_PDU_data(modbus_msg_t *msg_buf, req_input_param_struct_t *req_param)
{
    modbus_ret_t PDU_ret_status = RET_ERROR_UNKNOWN_MAPPER_FUN_CODE;
    uint32_t master_mapper_size = MODBUS_MASTER_FUNCTIONS_MAPPER_SIZE;
    for (uint32_t i = 0; i < master_mapper_size; i++)
    {
        if (master_functions_mapper[i].fun_code == req_param->fun_code)
        {
            msg_buf->rw_data_ptr = req_param->rw_data_ptr;
            PDU_ret_status = master_functions_mapper[i].fun_code_action(msg_buf, req_param->adr, req_param->obj_qty);
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

static void modbus_master_enable_resp_timeout_timer(void)
{
    modbus_master_resp_timeout = MODBUS_MASTER_RESP_TIME_OUT_MS;
}
static void modbus_master_disable_resp_timeout_timer(void)
{
    modbus_master_resp_timeout = 0;
}

static void modbus_master_req_sended_callback(void)
{
    MODBUS_MASTER_REQ_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

static void modbus_master_T_1_5_char_expired_callback(void)
{
    MODBUS_MASTER_TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

static void modbus_master_T_3_5_char_expired_callback(void)
{
    MODBUS_MASTER_TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

static void modbus_master_frame_error_callback(void)
{
    MODBUS_MASTER_FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
}
