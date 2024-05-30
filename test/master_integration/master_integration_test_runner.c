#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_init)
{
   /* Test cases to run */
   RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
   // RUN_TEST_CASE(master_init, WhenModbusMasterInitInRTUmodeThenRTUmsgQueueInitialized);
   // RUN_TEST_CASE(master_init, );
}
