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
#include "modbus_queue.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"

#ifdef UNIT_TEST
#define PRIVATE
#else
#define PRIVATE static
#endif
PRIVATE modbus_device_ID_t modbus_slave_ID;

PRIVATE modbus_slave_state_t slave_manager_state_machine = MODBUS_SLAVE_UNKNOWN;
PRIVATE const modbus_RTU_driver_struct_t *slave_RTU_driver = NULL;

static modbus_queue_t slave_free_queue;
static modbus_queue_t slave_tx_rx_queue;

PRIVATE modbus_queue_t *slave_free_q = &slave_free_queue;
PRIVATE modbus_queue_t *slave_tx_rx_q = &slave_tx_rx_queue;
PRIVATE modbus_msg_t slave_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

PRIVATE modbus_msg_t *slave_msg_buf = NULL;

modbus_status_flag_t TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
modbus_status_flag_t TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
modbus_status_flag_t FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;

static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void push_all_available_msg_buffer_to_free_queue(void);
static void modbus_resp_send_callback(void);
static void modbus_T_1_5_char_expired_callback(void);
static void modbus_T_3_5_char_expired_callback(void);

void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity, modbus_device_ID_t slave_ID)
{
    register_msg_req_resp_data_buffers(mode);
    modbus_queue_init(slave_tx_rx_q);
    modbus_queue_init(slave_free_q);
    push_all_available_msg_buffer_to_free_queue();

    slave_RTU_driver = get_slave_RTU_driver_interface();
    slave_RTU_driver->init(baud_rate, parity);
    slave_msg_buf = modbus_queue_pop(slave_free_q);
    slave_RTU_driver->enable_rcev(&slave_msg_buf->resp);
    slave_RTU_driver->subscribe_msg_tx_done_cb(modbus_resp_send_callback);
    slave_RTU_driver->subscribe_msg_rx_done_cb(modbus_T_1_5_char_expired_callback);
    slave_RTU_driver->subscribe_start_req_processing_cb(modbus_T_3_5_char_expired_callback);
    modbus_slave_ID = slave_ID;
    
}

void check_modbus_request(void)
{
    switch (slave_manager_state_machine)
    {
    case MODBUS_SLAVE_IDLE:
        if (TIMER_1_5_CHAR_FLAG == MODBUS_FLAG_SET)
        {
            slave_manager_state_machine = MODBUS_SLAVE_MSG_RECIVED;
        }
        break;
    case MODBUS_SLAVE_MSG_RECIVED:
        modbus_ret_t RTU_status = modbus_RTU_recv(slave_msg_buf->req.data, slave_msg->req.len, modbus_slave_ID);
        if (RET_ERROR_SLAVE_ID == RTU_status)
        {
            slave_msg_buf->req.len = 0;
            slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        }
        else if(RET_ERROR_CRC == RTU_status)
        {
        }
        else
        {
            slave_manager_state_machine = MODBUS_SLAVE_RECIVER_SILANCE_PENDING;
            slave_RTU_driver->enable_silence_timer();
        }
        break;
    case MODBUS_SLAVE_RECIVER_SILANCE_PENDING:
        // tutaj sprwdzam flagę ustawioną przez przerwanie 3,5_t bo jak to przerwanie by zmieniło stan a byłbym jeszczez w modbus_slave
        break;
    case MODBUS_SLAVE_TRANSMITING_RESP:
        // waiting for end of resp transmission
        break;
    default:
        slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        break;
    }
}
static void register_msg_req_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
        {
            slave_msg[i].req.data = &RTU_req_buf[i][0];
            slave_msg[i].resp.data = &RTU_resp_buf[i][0];
        }
    }
}

static void push_all_available_msg_buffer_to_free_queue(void)
{
    for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_push(slave_free_q, &slave_msg[i]);
    }
}

static void modbus_resp_send_callback(void)
{
    // modbus_queue_push(slave_free_q, slave_msg_buf);
    // slave_manager_state_machine = MODBUS_SLAVE_IDLE;
}

static void modbus_T_1_5_char_expired_callback(void)
{
    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

static void modbus_T_3_5_char_expired_callback(void)
{
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
}
