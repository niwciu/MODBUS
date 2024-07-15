#include "unity/fixture/unity_fixture.h"
#include "modbus_queue.h"

// #include "tested_module.h"

#define MAX_HEAD_INDEX (MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS - 1)
TEST_GROUP(Modbus_Queue_test);
static modbus_queue_t test_queue;
static modbus_queue_t *q;
static modbus_buf_t req_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS + 3];
static modbus_buf_t resp_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS + 3];
static modbus_msg_t msg_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS + 3];
static modbus_msg_t *pop_msg;

TEST_SETUP(Modbus_Queue_test)
{
    /* Init before every test */
    q = &test_queue;
    for (uint8_t i = 0; i < 5; i++)
    {
        msg_buf[i].req.data = &req_buf[i];
        msg_buf[i].resp.data = &resp_buf[i];
    }
}

TEST_TEAR_DOWN(Modbus_Queue_test)
{
    /* Cleanup after every test */
}

TEST(Modbus_Queue_test, WhenModbusQueueInitThenQueueHeadAndTailIsEqualToZeroAndLastQueuePosStatusIsEpmty)
{
    modbus_queue_init(q);
    TEST_ASSERT_EQUAL(0, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
    TEST_ASSERT_EQUAL(LAST_QUEUE_POS_EMPTY, q->last_queue_pos_status);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenHeadisEqualTo1AndTailIsEqualTo0)
{
    modbus_msg_t *msg_ptr = &msg_buf[0];
    modbus_queue_init(q);
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(1, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenTailQueueMsgPtrIsEqualToPushedModbusMsgAdress)
{
    modbus_msg_t *msg_ptr = &msg_buf[0];
    modbus_queue_init(q);
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(&msg_buf[0], q->modbus_msg[q->tail]);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMasgPushToModbusQueueThenMsgNotPlacedInQueue)
{
    modbus_msg_t *msg_ptr;
    modbus_queue_init(q);
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        msg_ptr = &msg_buf[i];
        modbus_queue_push(q, &msg_ptr);
    }
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
    msg_ptr = &msg_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS];
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMasgPushToModbusQueueThenMsgPtrNotSetToNull)
{
    modbus_msg_t *msg_ptr;
    modbus_queue_init(q);
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        msg_ptr = &msg_buf[i];
        modbus_queue_push(q, &msg_ptr);
    }
    msg_ptr = &msg_buf[MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS];
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_NOT_NULL(msg_ptr);
}
TEST(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenModbusMsgPtrToPushedModbusMsgEqualToNull)
{
    modbus_msg_t *msg_ptr = &msg_buf[0];
    modbus_queue_init(q);

    TEST_ASSERT_EQUAL(&msg_buf[0], msg_ptr);
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_NULL(msg_ptr);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenTailisEqualToHead)
{
    modbus_msg_t *msg_ptr = &msg_buf[0];
    modbus_queue_init(q);
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(1, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
    pop_msg = modbus_queue_pop(q);
    TEST_ASSERT_EQUAL(1, q->head);
    TEST_ASSERT_EQUAL(1, q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenPopMsgPtrisEqualToMsgBufferAddresPopedFromQueue)
{
    modbus_msg_t *msg_ptr = &msg_buf[0];
    modbus_queue_init(q);
    modbus_queue_push(q, &msg_ptr);
    pop_msg = modbus_queue_pop(q);
    TEST_ASSERT_EQUAL(&msg_buf[0], pop_msg);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndEmptyAndPopMsgPtrEqualNullWhenModbusMsgPopFromQueueToPopMsgThenPopMsgEqualNull)
{
    modbus_queue_init(q);
    pop_msg = NULL;
    pop_msg = modbus_queue_pop(q);
    TEST_ASSERT_NULL(pop_msg);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMsgPopAndModbusMsgPushThenTailisEqualTo1AndHeadIsEqualTo0)
{
    // modbus_queue_init(q);
    // for (uint8_t i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    // {
    //     modbus_queue_push(q, &msg_buf[i]);
    // }

    modbus_msg_t *msg_ptr;
    modbus_queue_init(q);
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        msg_ptr = &msg_buf[i];
        modbus_queue_push(q, &msg_ptr);
    }
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);

    pop_msg = modbus_queue_pop(q);
    msg_ptr = &msg_buf[0];
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(0, q->head);
    TEST_ASSERT_EQUAL(1, q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullAndNextPopAllModbusMsgAndModbusMsgPushWhenModbusMsgPopThenTailisEqualTo0AndHeadIsEqualTo0)
{
    modbus_msg_t *msg_ptr;
    modbus_queue_init(q);
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        msg_ptr = &msg_buf[i];
        modbus_queue_push(q, &msg_ptr);
    }
    for (uint8_t i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(q);
    }
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->head);
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->tail);
    msg_ptr = &msg_buf[0];
    modbus_queue_push(q, &msg_ptr);
    TEST_ASSERT_EQUAL(0, q->head);
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX, q->tail);

    pop_msg = modbus_queue_pop(q);
    TEST_ASSERT_EQUAL(0, q->head);
    TEST_ASSERT_EQUAL(0, q->tail);
}

// TEST(Modbus_Queue_test, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }