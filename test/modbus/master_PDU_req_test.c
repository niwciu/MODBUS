#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_type.h"
#include "buf_rw.h"
#include "mock_master_app_data.h"

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
static modbus_msg_t *null_ptr_msg = NULL;

TEST_GROUP(Master_PDU_req);

TEST_SETUP(Master_PDU_req)
{
    /* Init before every test */
    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;

    mock_register_master_coils_data();
    mock_register_master_discrete_inputs_data();
    mock_register_master_input_registers_data();
    mock_register_master_holding_registers_data();
}

TEST_TEAR_DOWN(Master_PDU_req)
{

    /* Cleanup after every test */
}

TEST (Master_PDU_req ,ReadSingleHoldingRegisterRequestWithNullPtrModbusMasgPassed)
{
    static modbus_adr_t adr = 0x0001;
    static modbus_data_qty_t len = 1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_holding_reg_req(null_ptr_msg, adr, len));
}

TEST(Master_PDU_req, ReadSingleHoldingRegisterRequest)
{
    static modbus_adr_t adr = 0x0030;
    static modbus_data_qty_t len = 1;

    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(len, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
}

TEST(Master_PDU_req, ReadMaxQtyHoldingRegisterRequest)
{
    static modbus_adr_t adr = 0x0030;
    static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
}

TEST(Master_PDU_req, ReadHoldingRegisterMaxQtyPlus1Request)
{
    static modbus_adr_t adr = 0x0030;
    static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, Read0QtyHoldingRegisterRequest)
{
    static modbus_adr_t adr = 0x0030;
    static modbus_data_qty_t len = 0;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadSingleInputRegisterRequest)
{
    static modbus_adr_t adr = 0x0005;
    static modbus_data_qty_t len = 1;

    modbus_reg_t status = modbus_master_read_input_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(len, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtyInputRegisterRequest)
{
    static modbus_adr_t adr = 0x0005;
    static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadInputRegisterMaxQtyPlus1Request)
{
    static modbus_adr_t adr = 0x0005;
    static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroInputRegisterRequest)
{
    static modbus_adr_t adr = 0x0005;
    static modbus_data_qty_t len = 0;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadDiscreteInputsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = 5;

    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtyDiscreteInputsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;

    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtPlus1DiscreteInputsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroDiscreteInputsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = 0;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadCoilsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = 5;

    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadCoilsMaxQtyRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = MODBUS_MAX_READ_COILS_QTY;
    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, adr, input_qty);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadCoilsMaxQtyPlus1Request)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroCoilsRequest)
{
    static modbus_adr_t adr = 0x0003;
    static modbus_data_qty_t input_qty = 0;
    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteSingleRegister)
{
    static modbus_adr_t adr = 0x0009;
    modbus_reg_t value = 0x0012;
    modbus_ret_t status;
    set_register_value(Master_Holding_Registers, adr, value);
    status = modbus_master_write_single_reg_req(RTU_msg, adr);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(value, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteSingleCoilOn)
{
    static modbus_adr_t adr = 0x0009;
    modbus_ret_t status;
    set_coil_state(Master_Coils, adr, !!COIL_ON);

    status = modbus_master_write_single_coil_req(RTU_msg, adr);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(COIL_ON, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleRegisters)
{
    static modbus_adr_t adr = 0x0008;
    // modbus_reg_t values[5] = {1, 2, 3, 4, 5};
    static modbus_data_qty_t reg_qty = 5;

    mock_set_expected_master_hreg_alternately(adr, reg_qty, 0x5A5A);
    modbus_ret_t status = modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(reg_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8((reg_qty * 2), RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX));
    TEST_ASSERT_EQUAL_HEX16(0xA5A5, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 2));
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 4));
    TEST_ASSERT_EQUAL_HEX16(0xA5A5, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 6));
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 8));
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + (reg_qty * 2), RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMaxQtyMultipleRegisters)
{
    static modbus_adr_t adr = 0x0080;
    static modbus_data_qty_t reg_qty = MODBUS_MAX_WRITE_REG_QTY;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + (reg_qty * 2), RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleRegistersMaxQtyPlus1)
{
    static modbus_adr_t adr = 0x0080;
    static modbus_data_qty_t reg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteZeroMultipleRegisters)
{
    static modbus_adr_t adr = 0x0080;
    static modbus_data_qty_t reg_qty = 0;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, Write5MultipleCoils)
{
    static modbus_adr_t adr = 0x0009;
    static modbus_data_qty_t coils_qty = 5;
    modbus_buf_t expected_coil_states_in_PDU[1] = {0x15};
    modbus_byte_count_t expected_byte_cnt = 1;
    modbus_ret_t status;

    mock_set_expected_master_coils_alternately(adr, coils_qty, !!COIL_ON);
    status = modbus_master_write_multiple_coils_req(RTU_msg, adr, coils_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(coils_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt, RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_coil_states_in_PDU, RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX, expected_byte_cnt);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + expected_byte_cnt, RTU_msg->req.len);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Master_PDU_req, WriteMaxQtyMultipleCoils)
{
    static modbus_adr_t adr = 0x0000;
    static modbus_data_qty_t coils_qty = MODBUS_MAX_WRITE_COILS_QTY;
    modbus_byte_count_t expected_byte_cnt = 246;
    modbus_ret_t status;

    mock_set_expected_master_coils_alternately(adr, coils_qty, !!COIL_ON);
    status = modbus_master_write_multiple_coils_req(RTU_msg, adr, coils_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(coils_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt, RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + expected_byte_cnt, RTU_msg->req.len);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleCoilsMaxQtyPlus1)
{
    static modbus_adr_t adr = 0x0000;
    static modbus_data_qty_t coils_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    modbus_ret_t status;

    status = modbus_master_write_multiple_coils_req(RTU_msg, adr, coils_qty);
    TEST_ASSERT_EQUAL(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteZeroMultipleCoils)
{
    static modbus_adr_t adr = 0x0000;
    static modbus_data_qty_t coils_qty = 0;
    modbus_ret_t status;

    status = modbus_master_write_multiple_coils_req(RTU_msg, adr, coils_qty);
    TEST_ASSERT_EQUAL(RET_ERROR, status);
}
// //
// // testy na zerową ilość rejestrów coili do odczytu zapisu.