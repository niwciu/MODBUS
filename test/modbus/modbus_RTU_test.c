#include "unity/fixture/unity_fixture.h"
#include "modbus_crc.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_RTU);

TEST_SETUP(Modbus_RTU)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_RTU)
{
    /* Cleanup after every test */
}

TEST(Modbus_RTU, AddSlaveIdToSendBuf)
{
    uint8_t slave_ID=0x02;
    uint8_t buf_data_len = 15;
    modbus_RTU_send(buf,buf_len,slave_ID);

    TEST_FAIL_MESSAGE("ADDED FIRST TEST")
}


// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU, )
// {


//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }
