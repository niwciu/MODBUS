#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Queue_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_Queue_test, WhenModbusQueueInitThenQueueHeadAndTailIsEqualToZero);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenHeadisEqualTo1AndTailIsEqualTo0);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitWhenModbusMasgPushToModbusQueueThenTailQueueMsgPtrIsEqualToPushedModbusMsgAdress);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitAndFullWhenModbusMasgPushToModbusQueueThenMsgNotPlacedInQueue);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenTailisEqualToHead);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitAndModbusMasgPushToModbusQueueWhenModbusMsgPopFromQueueThenPopMsgPtrisEqualToMsgBufferAddresPopedFromQueue);
   RUN_TEST_CASE(Modbus_Queue_test, GivenModbusQueueInitAndEmptyAndPopMsgPtrEqualNullWhenModbusMsgPopFromQueueToPopMsgThenPopMsgEqualNull);
   // test na zawijanie kolejki przy wrzucaniu
   // test jak zawijam kolejkę przy zdejmowaniu

   // RUN_TEST_CASE(Modbus_Queue_test, );
}
