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

static void increase_obj_qty_in_req_frame(modbus_msg_t *msg);
static void set_zero_obj_qty_in_req_frame(modbus_msg_t *msg);
static void set_out_of_range_obj_adress(modbus_msg_t *msg);


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

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    modbus_adr_t coil_adr = 0x0000;

    modbus_master_read_coils_req(RTU_msg, coil_adr, MODBUS_MAX_READ_COILS_QTY);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
    
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    modbus_adr_t coil_adr = 0x0000;

    modbus_master_read_coils_req(RTU_msg, coil_adr, MODBUS_MAX_READ_COILS_QTY);
    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    modbus_adr_t coil_adr = 0x0000;

    modbus_master_read_coils_req(RTU_msg, coil_adr, 1);
    set_out_of_range_obj_adress(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_ADRES_RANGE_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddressAndQuantitiofOutputsThenSlaveRespondWithExceptionCode02)
{
    modbus_adr_t coil_adr = MAIN_APP_COILS_QTY;

    modbus_master_read_coils_req(RTU_msg, coil_adr, 2);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_ADRES_RANGE_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithDiscreteInputsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03 )
{
    modbus_adr_t din_adr = 0x0000;

    modbus_master_read_discrete_inputs_req(RTU_msg, din_adr, MODBUS_MAX_READ_DISCRETE_INPUTS_QTY);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithDiscreteInputsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03)
{
    modbus_adr_t din_adr = 0x0000;

    modbus_master_read_discrete_inputs_req(RTU_msg, din_adr, MODBUS_MAX_READ_DISCRETE_INPUTS_QTY);
    set_zero_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02)
{
    modbus_adr_t din_adr = 0x0000;

    modbus_master_read_discrete_inputs_req(RTU_msg, din_adr, 1);
    set_out_of_range_obj_adress(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_ADRES_RANGE_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithIncorrectStartingAddressAndQuantitiofOutputsThenSlaveRespondWithExceptionCode02)
{
    modbus_adr_t din_adr = MAIN_APP_DISCRET_INPUTS_QTY;

    modbus_master_read_discrete_inputs_req(RTU_msg, din_adr, 2);

    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_ADRES_RANGE_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

TEST(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03)
{
    modbus_adr_t in_reg = 0x0000;

    modbus_master_read_input_reg_req(RTU_msg, in_reg, MODBUS_MAX_READ_DISCRETE_INPUTS_QTY);
    increase_obj_qty_in_req_frame(RTU_msg);

    parse_master_request_and_prepare_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE,RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_INT16(MODBUS_ERROR_CODE_MASK | MODBUS_REQUEST_DATA_QUANTITY_ERROR, RTU_msg->resp.data[MODBUS_RESP_ERROR_CODE_IDX]);
}

// TEST(Slave_PDU_exception_code, )
// {
//     TEST_FAIL_MESSAGE("Implement your test!");
// }

static void increase_obj_qty_in_req_frame(modbus_msg_t *msg)
{
    modbus_data_qty_t coil_qty;
    coil_qty = read_u16_from_buf(msg->req.data + MODBUS_REQUEST_QTY_IDX);
    coil_qty++;
    write_u16_to_buf(msg->req.data+MODBUS_REQUEST_QTY_IDX,coil_qty);
}

static void set_zero_obj_qty_in_req_frame(modbus_msg_t *msg)
{
    write_u16_to_buf(msg->req.data+MODBUS_REQUEST_QTY_IDX,0);
}

static void set_out_of_range_obj_adress(modbus_msg_t *msg)
{
    write_u16_to_buf(msg->req.data+MODBUS_REQUEST_ADR_IDX,MAIN_APP_COILS_QTY+1);
}