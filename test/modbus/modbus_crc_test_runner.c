#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_CRC)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_CRC, CalculateCRCFrom5Bytes);
}
