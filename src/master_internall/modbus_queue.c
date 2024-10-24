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
    // q->last_queue_pos_status = LAST_QUEUE_POS_EMPTY;
    q->items_in_queue = 0;
}

/**
 * @brief Pushes a Modbus message onto the queue.
 *
 * This function adds a Modbus message to the end of the queue. If the queue is full,
 * the function will do nothing. The function takes a pointer to a pointer to the
 * Modbus message and sets the data pointer to NULL after pushing it onto the queue.
 *
 * @param[in] q Pointer to the Modbus message queue structure.
 * @param[in,out] data Pointer to a pointer to the Modbus message to push onto the queue.
 *                     After pushing the message onto the queue, the pointer is set to NULL.
 * @details The function ensures that the data pointer is set to NULL after the message
 *          is pushed onto the queue, to prevent potential reuse of the same message.
 *          If the queue is full, the function does nothing and the data pointer remains unchanged.
 * @note If the data pointer is not NULL after calling this function, the operation did not succeed.
 */
void modbus_queue_push(modbus_queue_t *q, modbus_msg_t **data)
{
    int32_t new_head = (q->head + 1) % MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS;

    if (q->head == q->tail)
    {
        if (q->items_in_queue == MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS)
        {
            return;
        }
        else
        {
            q->modbus_msg[q->head] = *data;
            q->items_in_queue++;
            q->head = new_head;
            *data = NULL;
        }
    }
    else
    {
        q->modbus_msg[q->head] = *data;
        q->head = new_head;
        q->items_in_queue++;
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
        if (q->items_in_queue == 0)
        {
            return NULL;
        }
        else
        {
            ret = q->modbus_msg[q->tail];
            q->modbus_msg[q->tail] = NULL;
            q->items_in_queue--;
            q->tail = (q->tail + 1) % MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS;
        }
    }
    else
    {
        ret = q->modbus_msg[q->tail];
        q->modbus_msg[q->tail] = NULL;
        q->items_in_queue--;
        q->tail = (q->tail + 1) % MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS;
    }

    return ret;
}