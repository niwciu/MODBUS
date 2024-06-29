/**
 * @file modbus_manager.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "modbus_queue.h"

#include <stdint.h>
#include <stddef.h>

void modbus_queue_init(modbus_queue_t *q)
{
    q->head = 0;
    q->tail = 0;
    q->last_queue_pos_status = LAST_QUEUE_POS_EMPTY; // ToDo test na inita
}

void modbus_queue_push(modbus_queue_t *q, modbus_msg_t *data)
{
    int32_t new_head = (q->head + 1) % MAX_MODBUS_MSG_QUEUE_ITEMS;

    if (new_head == q->tail)
    {
        // check if queue full
        if (LAST_QUEUE_POS_STORE_DATA == q->last_queue_pos_status)
        {
            return;
        }
        else
        {
            q->last_queue_pos_status = LAST_QUEUE_POS_STORE_DATA;
            q->modbus_msg[q->head] = data;
        }
    }
    else
    {
        q->modbus_msg[q->head] = data;
        q->head = new_head;
    }
}

modbus_msg_t *modbus_queue_pop(modbus_queue_t *q)
{
    modbus_msg_t *ret;
    if (q->head == q->tail)
    {
        // queue empty
        if (LAST_QUEUE_POS_EMPTY == q->last_queue_pos_status)
        {
            return NULL;
        }
        else
        {
            q->last_queue_pos_status = LAST_QUEUE_POS_EMPTY;
            ret = q->modbus_msg[q->tail];
        }
    }
    else
    {
        ret = q->modbus_msg[q->tail];
        q->tail = (q->tail + 1) % MAX_MODBUS_MSG_QUEUE_ITEMS;
    }

    return ret;
}