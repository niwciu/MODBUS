#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_RTU)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_RTU, AddSlaveIdToSendBuf);
   RUN_TEST_CASE(Modbus_RTU, AddCRCToSendBuf);
   // RUN_TEST_CASE(Modbus_RTU, );
   // RUN_TEST_CASE(Modbus_RTU, );
   // RUN_TEST_CASE(Modbus_RTU, );
   // RUN_TEST_CASE(Modbus_RTU, );
   // RUN_TEST_CASE(Modbus_RTU, );
}
