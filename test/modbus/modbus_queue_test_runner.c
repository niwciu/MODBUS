#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Queue_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_Queue_test, WhenModbusQueueInitThenQueueHeadAndTailIsEqualToZero);
}
