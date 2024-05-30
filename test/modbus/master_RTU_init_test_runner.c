#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_init)
{
   /* Test cases to run */
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenTxRxRTUmsgQueueInitialized);
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFull);
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenDriverInterfaceIsRegistered);
   RUN_TEST_CASE(master_init, GivenBaudAndParitySetWhenModbusMasterInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity );
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenModbusMasterManagerStateMachineIsSetToIdle);
   // RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThen);
   // RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThen);
}
