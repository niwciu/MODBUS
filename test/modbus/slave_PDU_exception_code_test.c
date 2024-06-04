#include "unity/fixture/unity_fixture.h"
#include "modbus_type.h"
#include "modbus_slave_PDU.h"
#include "modbus_master_PDU.h"
#include "modbus_RTU.h"
#include "buf_rw.h"

// #include "tested_module.h"

TEST_GROUP(Slave_PDU_exception_code);

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];


static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;

TEST_SETUP(Slave_PDU_exception_code)
{
    /* Init before every test */
    RTU_msg->req.data=req_RTU_buf;
    RTU_msg->resp.data=resp_RTU_buf;
}

TEST_TEAR_DOWN(Slave_PDU_exception_code)
{
    /* Cleanup after every test */
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01)
{
    modbus_fun_code_t fun_code = 0x07;
    modbus_buf_t master_request[] = {0x01, fun_code, 0x41, 0xe2}; // 07 (0x07) Read Exception Status (Serial Line only)

    RTU_msg->req.data = master_request;
    RTU_msg->req.len = sizeof(master_request) / sizeof(modbus_buf_t);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(fun_code, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_FUNCTION_CODE_NOT_SUPPORTED_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithMoreCoilsToReadThanSlaveSupportsThenSlaveRespondWithExceptionCode03)
{
    modbus_adr_t coil_adr = 0x0000;
    modbus_data_qty_t coil_qty;

    modbus_master_read_coils_req(RTU_msg, coil_adr, MODBUS_MAX_READ_COILS_QTY);
    
    coil_qty = read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX);
    coil_qty++;
    write_u16_to_buf(RTU_msg->req.data+MODBUS_REQUEST_QTY_IDX,coil_qty);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
    
}

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }
