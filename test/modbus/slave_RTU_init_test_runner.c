#include "unity/fixture/unity_fixture.h"


TEST_GROUP_RUNNER(Slave_RTU_init_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlavenitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenTxRxRTUmsgQueueInitialized);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFull);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenDriverInterfaceIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToIdle);
   // RUN_TEST_CASE(Slave_RTU_init_test, );
}