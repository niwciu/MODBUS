#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"


#include "mock_modbus_driver_interface.h"
// #include "mock_master_app_data.h"
#include "mock_slave_app_data.h"


TEST_GROUP(master_RTU);

// extern modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];
// extern modbus_queue_t master_msg_queue;
// extern struct modbus_RTU_driver_struct *RTU_driver;
// extern modbus_master_state_t master_manager_state_machine;



TEST_SETUP(master_RTU)
{
    /* Init before every test */
    modbus_master_init(RTU,9600,ODD);

}

TEST_TEAR_DOWN(master_RTU)
{
    /* Cleanup after every test */
}

TEST(master_RTU,GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t app_data_coil= !!COIL_OFF; 

    register_app_data_to_modbus_master_coils_table(coil_adr,&app_data_coil);
    
    TEST_ASSERT_EQUAL(&app_data_coil,Master_Coils[coil_adr]);
    // TEST_FAIL_MESSAGE("Implement your test!"); 
}

// TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU,GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU,GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersAndGetAnswerBeforTimeOutThenMasterHoldingRegistersAreUpdatedWithSlaveValue)
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

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!"); 
// }