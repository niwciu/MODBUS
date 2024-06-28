#include "unity/fixture/unity_fixture.h"
#include "modbus_queue.h"

// #include "tested_module.h"

#define MAX_HEAD_INDEX (MAX_MODBUS_MSG_QUEUE_ITEMS-1)
TEST_GROUP(Modbus_Queue_test);
modbus_queue_t test_queue;
modbus_queue_t *q;
modbus_buf_t req_buf[MAX_MODBUS_MSG_QUEUE_ITEMS+3];
modbus_buf_t resp_buf[MAX_MODBUS_MSG_QUEUE_ITEMS+3];
modbus_msg_t msg_buf[MAX_MODBUS_MSG_QUEUE_ITEMS+3];
modbus_msg_t *pop_msg;

TEST_SETUP(Modbus_Queue_test)
{
    /* Init before every test */
    q=&test_queue;
    for(uint8_t i=0; i<5; i++)
    {
        msg_buf[i].req.data=&req_buf[i];
        msg_buf[i].resp.data=&resp_buf[i];
    }
}

TEST_TEAR_DOWN(Modbus_Queue_test)
{
    /* Cleanup after every test */
}

TEST(Modbus_Queue_test, WhenModbusQueueInitThenQueueHeadAndTailIsEqualToZero)
{
    modbus_queue_init(q);
    TEST_ASSERT_EQUAL(0,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenHeadisEqualTo1AndTailIsEqualTo0)
{
    modbus_queue_init(q);
    modbus_queue_push(q,&msg_buf[0]);
    TEST_ASSERT_EQUAL(1,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenTailQueueMsgPtrIsEqualToPushedModbusMsgAdress)
{
    modbus_queue_init(q);
    modbus_queue_push(q,&msg_buf[0]);
    TEST_ASSERT_EQUAL(&msg_buf[0],q->modbus_msg[q->tail]);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMasgPushToModbusQueueThenMsgNotPlacedInQueue)
{
    modbus_queue_init(q);
    for (uint8_t i=0; i<MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_push(q,&msg_buf[i]);
    }
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);

    modbus_queue_push(q,&msg_buf[MAX_MODBUS_MSG_QUEUE_ITEMS]);
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenTailisEqualToHead)
{
    modbus_queue_init(q);
    modbus_queue_push(q,&msg_buf[0]);
    TEST_ASSERT_EQUAL(1,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);
    pop_msg=modbus_queue_pop(q);
    TEST_ASSERT_EQUAL(1,q->head);
    TEST_ASSERT_EQUAL(1,q->tail);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenPopMsgPtrisEqualToMsgBufferAddresPopedFromQueue)
{
    modbus_queue_init(q);
    modbus_queue_push(q,&msg_buf[0]);
    pop_msg=modbus_queue_pop(q);
    TEST_ASSERT_EQUAL(&msg_buf[0],pop_msg);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndEmptyAndPopMsgPtrEqualNullWhenModbusMsgPopFromQueueToPopMsgThenPopMsgEqualNull)
{
    modbus_queue_init(q);
    pop_msg=NULL;
    pop_msg=modbus_queue_pop(q);
    TEST_ASSERT_NULL(pop_msg);
}

TEST(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMsgPopAndModbusMsgPushThenTailisEqualTo1AndHeadIsEqualTo0)
{
    modbus_queue_init(q);
    for (uint8_t i=0; i<MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_push(q,&msg_buf[i]);
    }
    TEST_ASSERT_EQUAL(MAX_HEAD_INDEX,q->head);
    TEST_ASSERT_EQUAL(0,q->tail);

    pop_msg=modbus_queue_pop(q);
    modbus_queue_push(q,&msg_buf[0]);
    TEST_ASSERT_EQUAL(0,q->head);
    TEST_ASSERT_EQUAL(1,q->tail);
}

// TEST(Modbus_Queue_test, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }