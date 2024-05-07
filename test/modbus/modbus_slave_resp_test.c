#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

uint8_t req_PDU[MODBUS_PDU_FRAME_LEN]={0};
uint8_t resp_PDU[MODBUS_PDU_FRAME_LEN]={0};

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

TEST(Modbus_Slave_Resp, SlaveRead5Coils)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=5;
    modbus_byte_count_t expected_byte_count=1;

    modbus_master_read_coils(req_PDU,adr,input_qty);
    modbus_slave_read_couils(resp_PDU,req_PDU);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNCTTION_CODE, &resp_PDU[0]);
    // TEST_ASSERT_EQUAL(expected_byte_count,&resp_PDU[1]);
    // ToDo how to check and generate coils status
    // TEST_ASSERT_EQUAL(expected_byte_count,resp_PDU[2]);
    TEST_FAIL_MESSAGE("Aded First Test");
}


//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 