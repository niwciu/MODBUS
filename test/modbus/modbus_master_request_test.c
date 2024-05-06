#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_Master_Requests);

TEST_SETUP(Modbus_Master_Requests)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_Master_Requests)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Requests, ReadSingleHoldingRegisterRequest)
{
    modbus_adr_t adr=0x0030;
    modbus_reg_qty_t len=1;

    uint8_t PDU_frame[5];

    modbus_master_read_holding_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_UINT8(READ_HOLDING_REGISTERS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,(uint16_t)((PDU_frame[1]<<8) | PDU_frame[2]));
    TEST_ASSERT_EQUAL_UINT16(len,(uint16_t)((PDU_frame[3]<<8) | PDU_frame[4]));
}

// TEST(Modbus_Master_Requests, )
// {


//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Modbus_Master_Requests, )
// {


//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Modbus_Master_Requests, )
// {


//     TEST_FAIL_MESSAGE("Implement your test!");
// }
