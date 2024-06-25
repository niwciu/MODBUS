#include "unity/fixture/unity_fixture.h"
#include "modbus_queue.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_Queue_test);
modbus_queue_t test_queue;
modbus_queue_t *q;

TEST_SETUP(Modbus_Queue_test)
{
    /* Init before every test */
    q=&test_queue;
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

// TEST(Modbus_Queue_test, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Modbus_Queue_test, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }
