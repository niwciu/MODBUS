#include "unity/fixture/unity_fixture.h"
#include "modbus_type.h"
#include "modbus_slave_PDU.h"

// #include "tested_module.h"

TEST_GROUP(Slave_PDU_exception_code);

TEST_SETUP(Slave_PDU_exception_code)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Slave_PDU_exception_code)
{
    /* Cleanup after every test */
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01)
{
    modbus_fun_code_t fun_code = 0x07;
    modbus_buf_t master_request[]= {0x01, fun_code, 0x41, 0xe2}; //07 (0x07) Read Exception Status (Serial Line only)
    modbus_msg_t msg;
    
    msg.req.data=master_request;
    msg.req.len= sizeof(master_request)/sizeof(modbus_buf_t);

    parse_master_request_and_prepare_resp(&msg);

    TEST_ASSERT_EQUAL(fun_code,msg.resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_FUNCTION_CODE_NOT_SUPPORTED_ERROR, msg.resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
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


// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

