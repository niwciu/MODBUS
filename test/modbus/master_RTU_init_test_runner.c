#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_RTU_init_test)
{
    /* Test cases to run */
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInUnknownModeThenRtuReqAndRespBuffersAreNotRegistered);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenTxRxRTUmsgQueueInitialized);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFull);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenDriverInterfaceIsRegistered);
    RUN_TEST_CASE(master_RTU_init_test, GivenBaudAndParitySetWhenModbusMasterInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenModbusMasterManagerStateMachineIsSetToIdle);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenMOdbusSendingRequestFlagIsCleared);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenTimer1_5Char_FlagIsCleared);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenTimer3_5Char_FlagIsCleared);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenFrameErrorFlagIsCleared);
 

    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenRequestTransmiscionCompleateCallbackRegistered);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenT1_5CharTimeExpiredCallbackRegistered);
    RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenT3_5CharTimeExpiredCallbackRegistered);
    // RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenFrameErrorFlagCallbackRegistered);
    // RUN_TEST_CASE(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThen);
}
