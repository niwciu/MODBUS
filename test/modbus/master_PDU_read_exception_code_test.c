#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "mock_slave_app_data.h"
#include "buf_rw.h"

// static modbus_ret_t status;

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
// static modbus_msg_t *null_ptr_msg;
extern modbus_master_error_cb_t modbus_error_callback;

static void test_func(modbus_error_rep_t error_info);

TEST_GROUP(Master_PDU_read_exception_code);

TEST_SETUP(Master_PDU_read_exception_code)
{

    mock_register_slave_coils_data();
    mock_register_slave_discrete_inputs_data();
    mock_register_slave_input_registers_data();
    mock_register_slave_holding_registers_data();

    mock_set_all_slave_cails_to_off();
    mock_set_all_slave_din_to_off();
    mock_reset_all_slave_inreg_value();
    mock_reset_all_slave_hreg_value();

    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;
}

TEST_TEAR_DOWN(Master_PDU_read_exception_code)
{
    /* Cleanup after every test */
}
TEST(Master_PDU_read_exception_code, WhenRegisterModbusErrorCbCalledThenModbusErrorCbEqualToRegisteredFunctionPointer)
{

    register_modbus_master_error_cb(test_func);

    TEST_ASSERT_EQUAL(&test_func, modbus_error_callback);
}

// TEST(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen)
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

// TEST(Master_PDU_read_exception_code, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }

static void test_func(modbus_error_rep_t error_info)
{
    (void)(error_info);
}