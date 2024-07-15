#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_CRC_test)
{
    /* Test cases to run */
    RUN_TEST_CASE(Modbus_CRC_test, CalculateCRCFrom5Bytes);
    RUN_TEST_CASE(Modbus_CRC_test, CalculateCRCFrom7Bytes);
    // RUN_TEST_CASE(Modbus_CRC_test, );
    // RUN_TEST_CASE(Modbus_CRC_test, );
}
