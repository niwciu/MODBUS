#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
#include "modbus_RTU.h"
#include "modbus_type.h"

// #include "tested_module.h"

TEST_GROUP(Slave_RTU_init_test);

extern modbus_msg_t slave_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

TEST_SETUP(Slave_RTU_init_test)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Slave_RTU_init_test)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_init_test, WhenModbusSlavenitInRTUmodeThenRtuReqAndRespBuffersAreRegistered)
{
    modbus_slave_init(RTU, 0, 0);
    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_req_buf[i][0], slave_msg[i].req.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_resp_buf[i][0], slave_msg[i].resp.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
    }
}