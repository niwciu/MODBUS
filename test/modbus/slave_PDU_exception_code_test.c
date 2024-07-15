#include "unity/fixture/unity_fixture.h"
#include "modbus_type.h"
#include "modbus_slave_PDU.h"
#include "modbus_master_PDU.h"
#include "modbus_RTU.h"
#include "buf_rw.h"
#include "mock_slave_app_data.h"

// #include "tested_module.h"

TEST_GROUP(Slave_PDU_exception_code);

#define EXPECTED_PDU_EXCEPTION_CODE_MSG_LED 3

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;

static void increase_obj_qty_in_req_frame(modbus_msg_t *msg);
static void set_zero_obj_qty_in_req_frame(modbus_msg_t *msg);
static void set_out_of_range_obj_adress(modbus_msg_t *msg, modbus_data_qty_t max_obj_adr_by_one);
static void increase_byte_count_in_req_frame(modbus_msg_t *msg);

TEST_SETUP(Slave_PDU_exception_code)
{
    /* Init before every test */
    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;
}

TEST_TEAR_DOWN(Slave_PDU_exception_code)
{
    /* Cleanup after every test */
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01)
{
    // static req_input_param_struct_t req={0};
    modbus_fun_code_t fun_code = 0x07;

    modbus_buf_t master_request[] = {0x01, fun_code, 0x41, 0xe2}; // 07 (0x07) Read Exception Status (Serial Line only)

    RTU_msg->req.data = master_request;
    RTU_msg->req.len = sizeof(master_request) / sizeof(modbus_buf_t);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((fun_code | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_FUNCTION_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Read Coils exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    req.adr=coil_adr;
    req.obj_qty = MODBUS_MAX_READ_COILS_QTY;

    modbus_master_read_coils_req(RTU_msg, &req);

    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    req.adr = coil_adr;
    req.obj_qty = MODBUS_MAX_READ_COILS_QTY;

    modbus_master_read_coils_req(RTU_msg, &req);

    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    req.adr = coil_adr;
    req.obj_qty = 1;

    modbus_master_read_coils_req(RTU_msg, &req);

    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_COILS_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = MAIN_APP_COILS_QTY - 1;
    req.adr = coil_adr;
    req.obj_qty = 2;

    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestAndGetErrorWhenReadingCoilsThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coil_qty = 4;
    req.adr = coil_adr;
    req.obj_qty = coil_qty;

    modbus_master_read_coils_req(RTU_msg, &req);

    mock_clear_modbus_slave_coils_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Read Discrete Inputs exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithDiscreteInputsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t din_adr = 0x0000;
    req.adr=din_adr;
    req.obj_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithDiscreteInputsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t din_adr = 0x0000;

    req.adr = din_adr;
    req.obj_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t din_adr = 0x0000;
    req.adr = din_adr;
    req.obj_qty = 1;

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_DISCRET_INPUTS_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t din_adr = MAIN_APP_DISCRET_INPUTS_QTY - 1;
    req.adr = din_adr;
    req.obj_qty = 2;

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestAndGetErrorWhenReadingDiscreteInputsThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t din_adr = 0x0001;
    modbus_data_qty_t din_qty = 4;
    req.adr = din_adr;
    req.obj_qty = din_qty;

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
 
    mock_clear_modbus_slave_discrete_inputs_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Read Input Register exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t in_reg_adr = 0x0000;
    req.adr = in_reg_adr;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;

    modbus_master_read_input_reg_req(RTU_msg, &req);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_INPUT_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t in_reg_adr = 0x0000;
    req.adr = in_reg_adr;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;

    modbus_master_read_input_reg_req(RTU_msg, &req);
   
    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_INPUT_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t in_reg_adr = 0x0000;
    req.adr = in_reg_adr;
    req.obj_qty = 1;

    modbus_master_read_input_reg_req(RTU_msg, &req);
    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_INPUT_REG_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_INPUT_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t in_reg_adr = MAIN_APP_INPUT_REG_QTY - 1;
    req.adr = in_reg_adr;
    req.obj_qty = 2;

    modbus_master_read_input_reg_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_INPUT_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestAndGetErrorWhenReadingInputRegisterThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    req.adr = in_reg_adr;
    req.obj_qty = in_reg_qty;

    modbus_master_read_input_reg_req(RTU_msg, &req);

    mock_clear_modbus_slave_input_register_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_INPUT_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Read Holding Register exception tests

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithHoldingRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t hreg_adr = 0x0000;
    req.adr = hreg_adr;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;

    modbus_master_read_holding_reg_req(RTU_msg, &req);

    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithHoldingRegisterQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t hreg_adr = 0x0000;
    req.adr = hreg_adr;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;

    modbus_master_read_holding_reg_req(RTU_msg, &req);

    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t hreg_adr = 0x0000;
    req.adr = hreg_adr;
    req.obj_qty = 1;

    modbus_master_read_holding_reg_req(RTU_msg, &req);

    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_INPUT_REG_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t hreg_adr = MAIN_APP_INPUT_REG_QTY - 1;
    req.adr = hreg_adr;
    req.obj_qty = 2;

    modbus_master_read_holding_reg_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestAndGetErrorWhenReadingHoldingRegisterThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t hreg_adr = 0x0001;
    modbus_data_qty_t hreg_qty = 4;
    req.adr = hreg_adr;
    req.obj_qty = hreg_qty;

    modbus_master_read_holding_reg_req(RTU_msg, &req);
 
    mock_clear_modbus_slave_holding_resgister_data_table();
    
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Write Signle Coil exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestWithIncorrectCoilValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_data_t incorrect_msg_coil_value = 0xFFFF;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    req.adr=coil_adr;
    req.coil_2_write=coil_2_write;
    // RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

    modbus_master_write_single_coil_req(RTU_msg, &req);

    write_u16_to_buf(&RTU_msg->req.data[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX], incorrect_msg_coil_value);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestWithIncorrectAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    req.adr = coil_adr;
    req.coil_2_write = coil_2_write;
    // RTU_msg->rw_data_ptr = (void *)(&coil_2_write);    
    modbus_master_write_single_coil_req(RTU_msg, &req);
    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_COILS_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestAndGetErrorStatusWhenWritingCoilThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    req.adr = coil_adr;
    req.coil_2_write = coil_2_write;
    // RTU_msg->rw_data_ptr = (void *)(&coil_2_write);
    modbus_master_write_single_coil_req(RTU_msg, &req);
    mock_clear_modbus_slave_coils_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Write Signle Register exception tests
// TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestWithIncorrectCoilValueThenSlaveRespondWithExceptionCode03)
// {
//     modbus_adr_t reg_adr = 0x0000;
//     modbus_data_t incorrect_msg_coil_value = 0xFFFF;
//     modbus_master_write_single_coil_req(RTU_msg, reg_adr);

//     write_u16_to_buf (&RTU_msg->req.data[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX],incorrect_msg_coil_value);

//     parse_master_request_and_prepare_resp(RTU_msg);

//     TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_COIL_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK) ,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
//     TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
//     TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED,RTU_msg->resp.len);
// }

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleRegisterRequestWithIncorrectAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t reg_2_write = 0x5a5a;
    // RTU_msg->rw_data_ptr = (void *)(&reg_2_write);
    req.adr=reg_adr;
    req.reg_2_write=reg_2_write;

    modbus_master_write_single_reg_req(RTU_msg, &req);
    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_HOLDING_REG_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleRegisterRequestAndGetErrorStatusWhenWritingRegisterThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0001;
    modbus_reg_t reg_2_write = 0x5a5a;
    // RTU_msg->rw_data_ptr = (void *)(&reg_2_write);
    req.adr = reg_adr;
    req.reg_2_write = reg_2_write;

    modbus_master_write_single_reg_req(RTU_msg, &req);
    mock_clear_modbus_slave_holding_resgister_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Write Multiple Coils exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestWithCoilsQtyToWriteAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write[MODBUS_MAX_WRITE_COILS_QTY] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr=coil_adr;
    req.obj_qty = MODBUS_MAX_WRITE_COILS_QTY;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestWithCoilsQtyToWriteEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write[MODBUS_MAX_WRITE_COILS_QTY] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr = coil_adr;
    req.obj_qty = MODBUS_MAX_WRITE_COILS_QTY;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);

    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestWithWrongByteCountThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write[20] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr = coil_adr;
    req.obj_qty = 20;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);

    increase_byte_count_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write[20] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr = coil_adr;
    req.obj_qty = 20;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_COILS_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = MAIN_APP_COILS_QTY - 1;
    modbus_coil_disin_t coil_2_write[2] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr = coil_adr;
    req.obj_qty = 2;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleCoilsRequestAndGetErrorWhenReadingCoilsThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t coil_adr = 0x0000;
    modbus_coil_disin_t coil_2_write[2] = {!!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coil_2_write);
    req.adr = coil_adr;
    req.obj_qty = 2;
    req.coils_2_write = coil_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);

    mock_clear_modbus_slave_coils_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// Write Multiple Register exception tests
TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestWithRegisterQtyToWriteAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t hreg_2_write[MODBUS_MAX_WRITE_REG_QTY] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr=reg_adr;
    req.obj_qty = MODBUS_MAX_WRITE_REG_QTY;
    req.regs_2_write=hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestWithRegisterQtyToWriteEqual0ThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t hreg_2_write[MODBUS_MAX_WRITE_REG_QTY] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr = reg_adr;
    req.obj_qty = MODBUS_MAX_WRITE_REG_QTY;
    req.regs_2_write = hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);

    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestWithWrongByteCountThenSlaveRespondWithExceptionCode03)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t hreg_2_write[20] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr = reg_adr;
    req.obj_qty = 20;
    req.regs_2_write = hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);

    increase_byte_count_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_VALUE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t hreg_2_write[20] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr = reg_adr;
    req.obj_qty = 20;
    req.regs_2_write = hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
    set_out_of_range_obj_adress(RTU_msg, MAIN_APP_HOLDING_REG_QTY);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = MAIN_APP_HOLDING_REG_QTY - 1;
    modbus_reg_t hreg_2_write[2] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr = reg_adr;
    req.obj_qty = 2;
    req.regs_2_write = hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveWriteMultipleRegistersRequestAndGetErrorWhenReadingCoilsThenSlaveRespondWithExceptionCode04)
{
    static req_input_param_struct_t req={0};
    modbus_adr_t reg_adr = 0x0000;
    modbus_reg_t hreg_2_write[2] = {0x5a5a};
    // RTU_msg->rw_data_ptr = (void *)(&hreg_2_write);
    req.adr = reg_adr;
    req.obj_qty = 2;
    req.regs_2_write = hreg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
   
    mock_clear_modbus_slave_holding_resgister_data_table();
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL((MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE | MODBUS_EXCEPTION_CODE_MASK), RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_SERVER_DEVICE_FAILURE_ERROR, RTU_msg->resp.data[MODBUS_RESP_EXCEPTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(EXPECTED_PDU_EXCEPTION_CODE_MSG_LED, RTU_msg->resp.len);
}

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("ADDED NEW TEST !!!");
// }

static void increase_obj_qty_in_req_frame(modbus_msg_t *msg)
{
    modbus_data_qty_t coil_qty;
    coil_qty = read_u16_from_buf(msg->req.data + MODBUS_REQUEST_QTY_IDX);
    coil_qty++;
    write_u16_to_buf(msg->req.data + MODBUS_REQUEST_QTY_IDX, coil_qty);
}

static void set_zero_obj_qty_in_req_frame(modbus_msg_t *msg)
{
    write_u16_to_buf(msg->req.data + MODBUS_REQUEST_QTY_IDX, 0);
}

static void set_out_of_range_obj_adress(modbus_msg_t *msg, modbus_data_qty_t max_obj_adr_by_one)
{
    write_u16_to_buf(msg->req.data + MODBUS_REQUEST_ADR_IDX, max_obj_adr_by_one);
}
static void increase_byte_count_in_req_frame(modbus_msg_t *msg)
{
    modbus_byte_count_t byte_count;
    byte_count = msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX];
    byte_count++;
    msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX] = byte_count;
}