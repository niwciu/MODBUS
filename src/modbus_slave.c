/**
 * @file modbus_slave.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-31
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_slave.h"
#include "modbus_slave_PDU.h"
#include "modbus_slave_RTU.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"
#include <stddef.h>

#ifdef UNIT_TEST
#define PRIVATE
#else
#define PRIVATE static
#endif
PRIVATE modbus_device_ID_t modbus_slave_ID;
PRIVATE modbus_slave_state_t slave_manager_state_machine = MODBUS_SLAVE_UNKNOWN;
PRIVATE const modbus_RTU_driver_struct_t *slave_RTU_driver = NULL;
PRIVATE modbus_msg_t slave_msg;
PRIVATE modbus_msg_t *slave_msg_ptr = NULL;
PRIVATE modbus_status_flag_t TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t FRAME_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t RESP_TRANSMITION_FLAG = MODBUS_FLAG_UNKNOWN;

static void handle_modbus_slave_idle_state(void);
static void handle_modbus_slave_msg_recived_state(void);
static void handle_modbus_slave_transmitting_resp_state(void);
static void handle_modbus_slave_default_state(void);
static void init_modbus_data_buffers_and_queues(modbus_mode_t mode);
static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void init_modbus_driver(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
static void init_modbus_slave_internall_data(modbus_device_ID_t Slave_ID);
static void modbus_resp_send_callback(void);
static void modbus_T_1_5_char_expired_callback(void);
static void modbus_T_3_5_char_expired_callback(void);
static void modbus_frame_error_callback(void);

void register_app_data_to_modbus_slave_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Slave_Coils, coil_adr, app_data_ptr);
}
void register_app_data_to_modbus_slave_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Slave_Discrete_Inputs, disin_adr, app_data_ptr);
}
void register_app_data_to_modbus_slave_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Slave_Input_Registers, reg_adr, app_data_ptr);
}
void register_app_data_to_modbus_slave_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Slave_Holding_Registers, reg_adr, app_data_ptr);
}

void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity, modbus_device_ID_t slave_ID)
{
    init_modbus_data_buffers_and_queues(mode);
    init_modbus_driver(mode, baud_rate, parity);
    init_modbus_slave_internall_data(slave_ID);
}

void check_modbus_request(void)
{
    switch (slave_manager_state_machine)
    {
    case MODBUS_SLAVE_IDLE:
        handle_modbus_slave_idle_state();
        break;
    case MODBUS_SLAVE_MSG_RECIVED:
        handle_modbus_slave_msg_recived_state();
        break;
    case MODBUS_SLAVE_TRANSMITING_RESP:
        handle_modbus_slave_transmitting_resp_state();
        break;
    default:
        handle_modbus_slave_default_state();
        break;
    }
}

static void handle_modbus_slave_idle_state(void)
{
    if (TIMER_1_5_CHAR_FLAG == MODBUS_FLAG_SET)
    {
        modbus_ret_t RTU_status;
        TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
        RTU_status = modbus_RTU_recv(slave_msg_ptr->req.data, slave_msg_ptr->req.len, modbus_slave_ID);
        if (RTU_status != RET_OK)
        {
            FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
        }
        slave_manager_state_machine = MODBUS_SLAVE_MSG_RECIVED;
    }
}
static void handle_modbus_slave_msg_recived_state(void)
{
    if ((MODBUS_FLAG_SET == FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == TIMER_3_5_CHAR_FLAG))
    {
        slave_msg_ptr->req.len = 0;
        slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
        TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else if ((MODBUS_FLAG_CLEARED == FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == TIMER_3_5_CHAR_FLAG))
    {
        parse_master_request_and_prepare_resp(slave_msg_ptr);
        modbus_RTU_send(slave_msg_ptr->resp.data, &slave_msg_ptr->resp.len, modbus_slave_ID);
        slave_RTU_driver->send(slave_msg_ptr->resp.data, slave_msg_ptr->resp.len);
        RESP_TRANSMITION_FLAG = MODBUS_FLAG_SET;
        slave_manager_state_machine = MODBUS_SLAVE_TRANSMITING_RESP;
        TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else
    {
        // do nothing untill flags are set as expected
    }
}
static void handle_modbus_slave_transmitting_resp_state(void)
{
    if (MODBUS_FLAG_CLEARED == RESP_TRANSMITION_FLAG)
    {
        slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        slave_msg_ptr->resp.len = 0;
        slave_msg_ptr->req.len = 0;
    }
}
static void handle_modbus_slave_default_state(void)
{
    slave_manager_state_machine = MODBUS_SLAVE_IDLE;
    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
    slave_msg_ptr->resp.len = 0;
    slave_msg_ptr->req.len = 0;
}

static void init_modbus_data_buffers_and_queues(modbus_mode_t mode)
{
    register_msg_req_resp_data_buffers(mode);
    slave_msg_ptr = &slave_msg;
}
static void register_msg_req_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        slave_msg.req.data = slave_RTU_req_buf;
        slave_msg.resp.data = slave_RTU_resp_buf;
    }
}

static void init_modbus_driver(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    if (RTU == mode)
    {
        slave_RTU_driver = get_slave_RTU_driver_interface();
        slave_RTU_driver->init(baud_rate, parity);
        slave_RTU_driver->enable_rcev(&slave_msg_ptr->req);
        slave_RTU_driver->subscribe_msg_tx_done_cb(modbus_resp_send_callback);
        slave_RTU_driver->subscribe_t_1_5_char_expired_cb(modbus_T_1_5_char_expired_callback);
        slave_RTU_driver->subscribe_t_3_5_char_expired_cb(modbus_T_3_5_char_expired_callback);
        slave_RTU_driver->subscribe_modbus_frame_error_cb(modbus_frame_error_callback);
    }
    // else if (OTHER_MODE == mode)
    else
    {
        slave_RTU_driver = NULL;
    }
}

static void init_modbus_slave_internall_data(modbus_device_ID_t Slave_ID)
{
    modbus_slave_ID = Slave_ID;

    slave_manager_state_machine = MODBUS_SLAVE_IDLE;

    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

static void modbus_resp_send_callback(void)
{
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

static void modbus_T_1_5_char_expired_callback(void)
{
    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

static void modbus_T_3_5_char_expired_callback(void)
{
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

static void modbus_frame_error_callback(void)
{
    FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
}