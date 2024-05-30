#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"

#include "mock_modbus_driver_interface.h"


TEST_GROUP(master_RTU);

// extern modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];
// extern modbus_queue_t master_msg_queue;
// extern struct modbus_RTU_driver_struct *RTU_driver;
// extern modbus_master_state_t master_manager_state_machine;



TEST_SETUP(master_RTU)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(master_RTU)
{
    /* Cleanup after every test */
}



TEST(master_RTU, first_test)
{
   TEST_FAIL_MESSAGE("Implement your test!"); 
}

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }