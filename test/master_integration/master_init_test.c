#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"

// #include "tested_module.h"

TEST_GROUP(master_init);

extern modbus_msg_t modbus_msg[MAX_MODBUS_MSG_ITEMS];
extern modbus_queue_t master_msg_queue;

TEST_SETUP(master_init)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(master_init)
{
    /* Cleanup after every test */
}

TEST(master_init, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered)
{
    modbus_master_init(RTU);
    for (int i = 0 ; i< MAX_MODBUS_MSG_ITEMS; i++)
    {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_req_buf[i][0],modbus_msg[i].req.data,MAX_MODBUS_MSG_ITEMS);
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_resp_buf[i][0],modbus_msg[i].resp.data,MAX_MODBUS_MSG_ITEMS);
    }
}

TEST(master_init,WhenModbusMasterInitInRTUmodeThenRTUmsgQueueInitialized)
{
    modbus_master_init(RTU);

    for (int i = 0 ; i< MAX_MODBUS_MSG_ITEMS; i++)
    {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&modbus_msg[i],master_msg_queue.modbus_msg[i],MAX_MODBUS_MSG_ITEMS);
    }
    TEST_FAIL_MESSAGE("Implement your test!"); 
}

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_init,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }