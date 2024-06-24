#include "unity/fixture/unity_fixture.h"
TEST_GROUP_RUNNER(Slave_RTU_init_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlavenitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenDriverInterfaceIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToIdle);
   RUN_TEST_CASE(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenRxInterruptEnable);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgTxDoneCallbackIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenT1_5CharExpiredCallbackIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenT3_5CharExpiredCallbackIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgFrameErrorCallbackIsRegistered);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeWithDefinedSlaveIdThenModbusSlaveIdIsAssigned);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToModbusSlaveIdle);
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenAllModbusStatusFlagsAreCleared);
  
   // RUN_TEST_CASE(Slave_RTU_init_test, );
   // RUN_TEST_CASE(Slave_RTU_init_test, );
   // RUN_TEST_CASE(Slave_RTU_init_test, );
   // RUN_TEST_CASE(Slave_RTU_init_test, );
   // RUN_TEST_CASE(Slave_RTU_init_test, );

}