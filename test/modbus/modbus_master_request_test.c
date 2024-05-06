#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

uint8_t PDU_frame[MODBUS_PDU_FRAME_LEN];

static uint16_t read_u16_from_buf(uint8_t *buf);

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

    modbus_master_read_holding_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_UINT8(READ_HOLDING_REGISTERS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(len,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadSingleInputRegisterRequest)
{
    modbus_adr_t adr=0x0005;
    modbus_reg_qty_t len=1;

    modbus_master_read_input_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_UINT8(READ_INPUT_REGISTERS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(len,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, WriteSingleHoldingRegister)
{
    modbus_adr_t adr=0x0009;
    modbus_reg_t value=0x0012;

    modbus_master_write_single_reg(PDU_frame,adr,value);
  
    TEST_ASSERT_EQUAL_UINT8(WRITE_SINGLE_REGISTER,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(value,read_u16_from_buf(PDU_frame+3));
}

// TEST(Modbus_Master_Requests, )
// {


//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Modbus_Master_Requests, )
// {


//     TEST_FAIL_MESSAGE("Implement your test!");
// }

static uint16_t read_u16_from_buf(uint8_t *buf)
{
    return (uint16_t)((buf[0]<<8) | buf[1]);
}