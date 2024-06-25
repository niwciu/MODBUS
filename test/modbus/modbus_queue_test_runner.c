#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Queue_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_Queue_test, WhenModbusQueueInitThenQueueHeadAndTailIsEqualToZero);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenHeadisEqualTo1AndTailIsEqualTo0);
   // RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenQueueMsgPtrIsEqualToPushedModbusMsgAdress);
   // RUN_TEST_CASE(Modbus_Queue_test, );
}
