#include "unity/fixture/unity_fixture.h"
#include "modbus_crc.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_CRC);

TEST_SETUP(Modbus_CRC)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_CRC)
{
    /* Cleanup after every test */
}

TEST(Modbus_CRC, CalculateCRCFrom5Bytes)
{
    static uint8_t data_buf[] ={0x01,0x03,0x5A,0x5A,0x00,0x05}; //read 5 holding registers from slave 0x01 adr

    uint16_t expected_CRC = 0x2B7;
    uint8_t data_len=sizeof(data_buf);

    TEST_ASSERT_EQUAL_HEX16(expected_CRC,calculate_CRC(data_buf,data_len));
}

TEST(Modbus_CRC, CalculateCRCFrom7Bytes)
{
    static uint8_t data_buf[] ={0x01,0x03,0x5A,0x5A,0x00,0x05,0x23,0x54}; //read 5 holding registers from slave 0x01 adr

    uint16_t expected_CRC = 0xFEEE;
    uint8_t data_len=sizeof(data_buf)/sizeof(data_buf[0]);

    TEST_ASSERT_EQUAL_HEX16(expected_CRC,calculate_CRC(data_buf,data_len));
}

