#include "unity/fixture/unity_fixture.h"
#include "modbus_RTU.h"
#include "memory.h"
#include "buf_rw.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_RTU);

modbus_buf_t buf[MODBUS_RTU_BUFFER_SIZE] = {0};

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

    modbus_RTU_send(buf,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX8(slave_ID,buf[MODBUS_SLAVE_ADR_IDX]);
}


TEST(Modbus_RTU, AddCRCToSendBuf )
{
    
    modbus_buf_t PDU_frame[] ={0x00,0x03,0x00,0x01,0x00,0x05}; // read 5 holding resiters from adr 0x0001 
    modbus_device_ID_t slave_ID=0x02;
    modbus_CRC_t expected_CRC = 0xD43A;
    modbus_buf_size_t buf_data_len = sizeof(PDU_frame)/sizeof(modbus_buf_t);

    memcpy(buf,PDU_frame,sizeof(PDU_frame)/sizeof(modbus_buf_t));

    modbus_RTU_send(buf,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX16(expected_CRC, read_u16_from_buf(buf+buf_data_len));
}

TEST(Modbus_RTU, ModbusRtuSendWitnNullPtrAsBuffer)
{
    modbus_device_ID_t slave_ID=0x02;
    modbus_buf_size_t buf_data_len = 15;
    modbus_ret_t send_status;

    send_status= modbus_RTU_send(NULL,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR,send_status);

}

TEST(Modbus_RTU, ModbusRtuSendWitnBufLenEqualToMaxPduLen)
{
    modbus_device_ID_t slave_ID=0x02;
    modbus_buf_size_t buf_data_len = MODBUS_PDU_MAX_LEN;
    modbus_ret_t send_status;

    send_status= modbus_RTU_send(buf,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX8(RET_OK,send_status);
}

TEST(Modbus_RTU, ModbusRtuSendWitnBufLenEqualToMaxPduLenPlus1)
{
    modbus_device_ID_t slave_ID=0x02;
    modbus_buf_size_t buf_data_len = MODBUS_PDU_MAX_LEN+1;
    modbus_ret_t send_status;

    send_status= modbus_RTU_send(buf,buf_data_len,slave_ID);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR,send_status);
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
