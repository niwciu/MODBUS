#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "mock_slave_app_data.h"
#include "buf_rw.h"
#include "modbus_master.h"
#include <memory.h>

static modbus_ret_t status;

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
static modbus_error_rep_t test_error_rep;
// static modbus_msg_t *null_ptr_msg;
extern modbus_master_error_cb_t modbus_error_callback;

static void error_test_func(modbus_error_rep_t *error_info);
static void clr_test_error_rep(void);

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

    register_modbus_master_error_cb(error_test_func);
}

TEST_TEAR_DOWN(Master_PDU_read_exception_code)
{
    /* Cleanup after every test */
}
TEST(Master_PDU_read_exception_code, WhenRegisterModbusErrorCbCalledThenModbusErrorCbEqualToRegisteredFunctionPointer)
{

    TEST_ASSERT_EQUAL(&error_test_func, modbus_error_callback);
}

TEST(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThenExceptionCodeAddedToErrorReport)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    modbus_coil_disin_t readed_coil_val[coils_qty];
    RTU_msg->rw_data_ptr = (void *)(readed_coil_val);

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg); // zle to musza byc testy integracyjne bo samo pdu odebralo kod?

    TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], readed_coil_val, coils_qty);
}

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

static void error_test_func(modbus_error_rep_t *error_info)
{
    clr_test_error_rep();
    test_error_rep = *error_info;
}

static void clr_test_error_rep(void)
{
    memset(&test_error_rep,0,sizeof(test_error_rep));
}