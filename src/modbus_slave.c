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

PRIVATE modbus_slave_state_t slave_manager_state_machine = MODBUS_SLAVE_IDLE;
PRIVATE const modbus_RTU_driver_struct_t *slave_RTU_driver = NULL;

static modbus_queue_t slave_free_queue;
static modbus_queue_t slave_tx_rx_queue;

PRIVATE modbus_queue_t *slave_free_q = &slave_free_queue;
PRIVATE modbus_queue_t *slave_tx_rx_q = &slave_tx_rx_queue;
PRIVATE modbus_msg_t slave_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

PRIVATE modbus_msg_t *slave_msg_buf = NULL;

static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void push_all_available_msg_buffer_to_free_queue(void);

void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    register_msg_req_resp_data_buffers(RTU);
    modbus_queue_init(slave_tx_rx_q);
    modbus_queue_init(slave_free_q);
    push_all_available_msg_buffer_to_free_queue();

    slave_RTU_driver= get_slave_RTU_driver_interface();
    slave_RTU_driver->init(38400,ODD);
}

void parse_modbus_request(void)
{

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