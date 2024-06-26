#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_RTU_init_test)
{
    /* Test cases to run */
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenTxRxRTUmsgQueueInitialized);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFull);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenDriverInterfaceIsRegistered);
    RUN_TEST_CASE(master_RTU_init_test, GivenBaudAndParitySetWhenModbusMasterInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenModbusMasterManagerStateMachineIsSetToIdle);
    // RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThen);
    // RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThen);
}
