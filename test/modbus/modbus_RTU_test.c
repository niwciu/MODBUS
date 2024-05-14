#include "unity/fixture/unity_fixture.h"
#include "modbus_RTU.h"

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

    modbus_device_ID_t slave_ID=0x02;
    modbus_buf_size_t buf_data_len = 15;
    modbus_buf_t buf[MODBUS_FRAME_BUFFER_SIZE];

    modbus_RTU_send(buf,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX8(slave_ID,buf[MODBUS_SLAVE_ADR_IDX]);
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
