/**
 * @file modbus_manager.c
 * @brief Implementation file for Modbus message queue management.
 *
 * This file implements functions declared in modbus_queue.h for managing a Modbus message queue.
 * It includes functions to initialize the queue, push messages onto the queue, and pop messages
 * from the queue.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-27
 * @version 0.0.1
 *
 * @copyright Copyright (c) 2024
 */
#include "modbus_queue.h"

#include <stdint.h>
#include <stddef.h>
/**
 * @brief Initializes a Modbus message queue.
 *
 * Initializes a Modbus message queue structure by setting the head and tail indices to 0
 * and setting the last queue position status to EMPTY.
 *
 * @param q Pointer to the Modbus message queue structure to initialize.
 */
void modbus_queue_init(modbus_queue_t *q)
{
    q->head = 0;
    q->tail = 0;
    q->last_queue_pos_status = LAST_QUEUE_POS_EMPTY; // ToDo test na inita
}

/**
 * @brief Pushes a Modbus message onto the queue.
 *
 * Pushes a Modbus message onto the end of the queue. If the queue is full,
 * it updates the last queue position status to indicate data storage.
 *
 * @param q Pointer to the Modbus message queue structure.
 * @param data Pointer to a pointer to the Modbus message to push onto the queue.
 */
void modbus_queue_push(modbus_queue_t *q, modbus_msg_t **data)
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
            q->modbus_msg[q->head] = *data;
            *data=NULL;
        }
    }
    else
    {
        q->modbus_msg[q->head] = *data;
        q->head = new_head;
        *data = NULL;
    }
}

/**
 * @brief Pops a Modbus message from the queue.
 *
 * Pops a Modbus message from the front of the queue. If the queue is empty,
 * it updates the last queue position status to indicate an empty queue.
 *
 * @param q Pointer to the Modbus message queue structure.
 * @return Pointer to the popped Modbus message, or NULL if the queue is empty.
 */
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