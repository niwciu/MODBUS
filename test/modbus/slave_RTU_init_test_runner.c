#include "unity/fixture/unity_fixture.h"


TEST_GROUP_RUNNER(Slave_RTU_init_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_RTU_init_test, WhenModbusSlavenitInRTUmodeThenRtuReqAndRespBuffersAreRegistered);
}