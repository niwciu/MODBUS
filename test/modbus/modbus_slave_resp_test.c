#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

// uint8_t PDU_frame[MODBUS_PDU_FRAME_LEN];

// static uint16_t read_u16_from_buf(const uint8_t *buf);

TEST_GROUP(Modbus_Slave_Resp);

TEST_SETUP(Modbus_Slave_Resp)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_Slave_Resp)
{
    /* Cleanup after every test */
}

TEST(Modbus_Slave_Resp, FirstTest)
{
    TEST_FAIL_MESSAGE("Aded First Test");
}


//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 