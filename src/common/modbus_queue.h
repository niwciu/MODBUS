/**
 * @file modbus_queue.h
 * @brief Header file for Modbus message queue management.
 *
 * This header file defines structures and functions for managing a queue of Modbus messages.
 * It provides operations to initialize the queue, push messages onto the queue, and pop messages
 * from the queue.
 *
 * @author niwciu (niwciu@gmail.com)
 * @date 2024-05-27
 * @version 0.0.1
 *
 * @copyright Copyright (c) 2024
 */

#ifndef _MODBUS_QUEUE_H_
#define _MODBUS_QUEUE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "modbus_type.h"
    // #include "modbus_public_type.h"

    /**
     * @struct modbus_queue_t
     * @brief Structure representing a Modbus message queue.
     *
     * This structure defines the elements of a Modbus message queue, including head, tail,
     * status of the last queue position, and an array of pointers to Modbus message structures.
     */
    typedef struct
    {
        uint8_t head;                                         /**< Head index of the queue. */
        uint8_t tail;                                         /**< Tail index of the queue. */
        last_q_pos_status_t last_queue_pos_status;            /**< Status of the last queue position. */
        modbus_msg_t *modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS]; /**< Array of pointers to Modbus messages. */
    } modbus_queue_t;

    void modbus_queue_init(modbus_queue_t *q);
    void modbus_queue_push(modbus_queue_t *q, modbus_msg_t **data);
    modbus_msg_t *modbus_queue_pop(modbus_queue_t *q);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_QUEUE_H_ */