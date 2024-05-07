#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_modbus_data_interface.h"

uint8_t req_PDU[MODBUS_PDU_FRAME_LEN]={0};
uint8_t resp_PDU[MODBUS_PDU_FRAME_LEN]={0};

// static uint16_t read_u16_from_buf(const uint8_t *buf);

TEST_GROUP(Modbus_Slave_Resp);

TEST_SETUP(Modbus_Slave_Resp)
{
    /* Init before every test */
    mock_set_all_cails_to_off();
}

TEST_TEAR_DOWN(Modbus_Slave_Resp)
{
    /* Cleanup after every test */
}

TEST(Modbus_Slave_Resp, SlaveRead5Coils)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t coil_qty=5;
    modbus_byte_count_t expected_byte_count=1; // in each byte 8 coil status is reported

    mock_coil[adr]=COIL_ON;
    mock_coil[adr+1]=COIL_OFF;
    mock_coil[adr+2]=COIL_ON;
    mock_coil[adr+3]=COIL_OFF;
    mock_coil[adr+4]=COIL_ON;
    modbus_data_t exp_readed_coil_value=0x0015; //0b10101


    modbus_master_read_coils(req_PDU,adr,coil_qty);
    modbus_slave_read_coils(resp_PDU,req_PDU);
    
    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNCTTION_CODE, resp_PDU[0]);
    TEST_ASSERT_EQUAL(expected_byte_count,resp_PDU[1]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value,resp_PDU[2]);
}

TEST(Modbus_Slave_Resp, SlaveRead8Coils)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t coil_qty=8;
    modbus_byte_count_t expected_byte_count=1; // in each byte 8 coil status is reported

    mock_coil[adr]=1;
    mock_coil[adr+1]=0;
    mock_coil[adr+2]=1;
    mock_coil[adr+3]=0;
    mock_coil[adr+4]=1;
    mock_coil[adr+5]=0;
    mock_coil[adr+6]=1;
    mock_coil[adr+7]=0;
    modbus_data_t exp_readed_coil_value=0x0055; 


    modbus_master_read_coils(req_PDU,adr,coil_qty);
    modbus_slave_read_coils(resp_PDU,req_PDU);
    
    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNCTTION_CODE, resp_PDU[0]);
    TEST_ASSERT_EQUAL(expected_byte_count,resp_PDU[1]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value,resp_PDU[2]);
}


//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 