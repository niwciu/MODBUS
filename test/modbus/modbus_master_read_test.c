#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

// uint8_t PDU_frame[MODBUS_FRAME_BUFFER_SIZE];


TEST_GROUP(Modbus_Master_Read);

TEST_SETUP(Modbus_Master_Read)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_Master_Read)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Read, FirstTest)
{
    TEST_FAIL_MESSAGE("Aded First Test");
}


//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 