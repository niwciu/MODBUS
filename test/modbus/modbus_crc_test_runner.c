#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_CRC)
{
    /* Test cases to run */
    RUN_TEST_CASE(Modbus_CRC, CalculateCRCFrom5Bytes);
    RUN_TEST_CASE(Modbus_CRC, CalculateCRCFrom7Bytes);
    // RUN_TEST_CASE(Modbus_CRC, );
    // RUN_TEST_CASE(Modbus_CRC, );
}
