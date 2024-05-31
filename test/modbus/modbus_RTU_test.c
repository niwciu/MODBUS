#include "unity/fixture/unity_fixture.h"
#include "modbus_RTU.h"
#include "memory.h"
#include "buf_rw.h"

// #include "tested_module.h"

TEST_GROUP(Modbus_RTU_test);

modbus_buf_t buf[MODBUS_RTU_BUFFER_SIZE] = {0};
// modbus_req_resp_t req_resp_struct;
// modbus_req_resp_t *msg =&req_resp_struct;

#define SLAVE_ID_1 0x02U


TEST_SETUP(Modbus_RTU_test)
{
    /* Init before every test */
    // req_resp_struct.data=buf;
}

TEST_TEAR_DOWN(Modbus_RTU_test)
{
    /* Cleanup after every test */
}

TEST(Modbus_RTU_test, AddSlaveIdToSendBuf)
{
    modbus_buf_size_t buf_data_len = 15;

    modbus_RTU_send(buf, &buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(SLAVE_ID_1, buf[MODBUS_SLAVE_ADR_IDX]);
}

TEST(Modbus_RTU_test, AddCRCToSendBuf)
{
    modbus_buf_t PDU_frame[] = {0x00, 0x03, 0x00, 0x01, 0x00, 0x05}; // read 5 holding resiters from adr 0x0001
    modbus_CRC_t expected_CRC = 0xD43A;
    modbus_buf_size_t buf_data_len = sizeof(PDU_frame) / sizeof(modbus_buf_t);
    size_t PDU_frame_size = (sizeof(PDU_frame) / sizeof(modbus_buf_t));

    memcpy(buf, PDU_frame, PDU_frame_size);

    modbus_RTU_send(buf, &buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX16(expected_CRC, read_u16_from_buf(buf + (buf_data_len-2)));
}

TEST(Modbus_RTU_test, ModbusRtuSendWitnNullPtrAsBuffer)
{

    modbus_buf_size_t buf_data_len = 15;
    modbus_ret_t send_status;

    send_status = modbus_RTU_send(NULL, &buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR, send_status);
}

TEST(Modbus_RTU_test, ModbusRtuSendWitnBufLenEqualToMaxPduLen)
{
    modbus_buf_size_t buf_data_len = MODBUS_PDU_MAX_LEN;
    modbus_ret_t send_status;

    send_status = modbus_RTU_send(buf, &buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_OK, send_status);
}

TEST(Modbus_RTU_test, ModbusRtuSendWitnBufLenEqualToMaxPduLenPlus1)
{
    modbus_buf_size_t buf_data_len = MODBUS_PDU_MAX_LEN + 1;
    modbus_ret_t send_status;

    send_status = modbus_RTU_send(buf, &buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR, send_status);
}

TEST(Modbus_RTU_test, SlaveIdIsCorrectInRecvBuffer)
{
    modbus_buf_size_t buf_data_len = 2;
    modbus_ret_t recv_status;

    modbus_RTU_send(buf, &buf_data_len, SLAVE_ID_1);
    recv_status = modbus_RTU_recv(buf, buf_data_len, SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_OK, recv_status);
}

TEST(Modbus_RTU_test, SlaveIdIsNotCorrectInRecvBuffer)
{
    modbus_device_ID_t expected_slave_ID = 0x03;
    modbus_device_ID_t sended_slave_ID = SLAVE_ID_1;
    modbus_buf_size_t buf_data_len = 2;
    modbus_ret_t recv_status;

    modbus_RTU_send(buf, &buf_data_len, sended_slave_ID);
    recv_status = modbus_RTU_recv(buf, buf_data_len, expected_slave_ID);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR_SLAVE_ID, recv_status);
}

TEST(Modbus_RTU_test, CrcIsCorrectInRecivedBuffer)
{
    modbus_buf_t recv_RTU_msg[] = {SLAVE_ID_1, 0x03, 0x00, 0x01, 0x00, 0x05, 0xD4, 0x3A}; // message with correct CRC
    modbus_ret_t recv_status;

    recv_status = modbus_RTU_recv(recv_RTU_msg, sizeof(recv_RTU_msg) / sizeof(modbus_buf_t), SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_OK, recv_status);
}

TEST(Modbus_RTU_test, CrcIsIncorrectInRecivedBuffer)
{
    modbus_buf_t recv_RTU_msg[] = {SLAVE_ID_1, 0x03, 0x00, 0x01, 0x00, 0x05, 0xAA, 0xAA}; // message with incorrect CRC
    modbus_ret_t recv_status;

    recv_status = modbus_RTU_recv(recv_RTU_msg, sizeof(recv_RTU_msg) / sizeof(modbus_buf_t), SLAVE_ID_1);

    TEST_ASSERT_EQUAL_HEX8(RET_ERROR_CRC, recv_status);
}

// TEST(Modbus_RTU_test, )
// {

//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }

// TEST(Modbus_RTU_test, )
// {

//     TEST_FAIL_MESSAGE("ADDED NEW TEST")
// }
