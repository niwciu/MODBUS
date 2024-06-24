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
}

void modbus_queue_push(modbus_queue_t *q, modbus_msg_t *data)
{
    int32_t new_head = (q->head + 1) % MAX_MODBUS_MSG_QUEUE_ITEMS;

    if (new_head == q->tail)
    {
        // queue full
        return;
    }
    q->modbus_msg[q->head] = data;

    q->head = new_head;
}

modbus_msg_t *modbus_queue_pop(modbus_queue_t *q)
{
    if (q->head == q->tail)
    {
        // queue empty
        return NULL;
    }

    modbus_msg_t *ret = q->modbus_msg[q->tail];
    q->tail = (q->tail + 1) % MAX_MODBUS_MSG_QUEUE_ITEMS;

    return ret;
}