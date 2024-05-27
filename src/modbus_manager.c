/**
 * @file modbus_manager.c
 * @author niwciu (niwciu@gmail.com)
 * @brief 
 * @date 2024-05-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "modbus_type.h"
#include <stdint.h>

#define MAX_MODBUS_MSG_ITEMS = 10

typedef struct  
{
    modbus_buf_t buf[MODBUS_RTU_BUFFER_SIZE];
    uint8_t len;
}RTU_buf_t;

typedef struct 
{
    RTU_buf_t req_msg;
    RTU_buf_t resp_msg;
    modbus_device_ID_t slave_id;
}modbus_msg_t;

modbus_msg_t modbus_msg[MAX_MODBUS_MSG_ITEMS] = {0};

typedef struct 
{
    modbus_msg_t *data; 
    uint8_t head;
    uint8_t tail;
}modbus_queue_t;

modbus_queue_t modbus_msg_queue;


void modbus_queue_init(modbus_queue_t *q)
{
    q->head=0;
    q->tail=0;
}

void modbus_queue_push(modbus_queue_t *q, modbus_msg_t *data)
{
    int32_t new_head = (q->head + 1) % MAX_MODBUS_MSG_ITEMS;

    if (new_head == q->tail)
    {
        //queue full
        return;
    }

    q->items[q->head] = data;
    q->head = new_head;
    
}

void modbus_queue_pop(modbus_queue_t *q)
{
    if (q->head == q->tail)
    {
        //queue empty
        return NULL;
    }

    void * ret = q->items[q->tail];
    q->tail = (q->tail + 1) % ITEMS_MAX;

    return ret;
}