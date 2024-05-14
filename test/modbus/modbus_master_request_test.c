#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "modbus_type.h"
#include "buf_rw.h"

uint8_t PDU_frame[MODBUS_RTU_BUFFER_SIZE];

TEST_GROUP(Modbus_Master_Requests);

TEST_SETUP(Modbus_Master_Requests)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_Master_Requests)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Requests, ReadSingleHoldingRegisterRequest)
{
    modbus_adr_t adr = 0x0030;
    modbus_data_qty_t len = 1;

    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(len, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadMaxQtyHoldingRegisterRequest)
{
    modbus_adr_t adr = 0x0030;
    modbus_data_qty_t len = MODBUS_MAX_REG_QTY;
    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadHoldingRegisterMaxQtyPlus1Request)
{
    modbus_adr_t adr = 0x0030;
    modbus_data_qty_t len = MODBUS_MAX_REG_QTY + 1;
    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, Read0QtyHoldingRegisterRequest)
{
    modbus_adr_t adr = 0x0030;
    modbus_data_qty_t len = 0;
    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadSingleInputRegisterRequest)
{
    modbus_adr_t adr = 0x0005;
    modbus_data_qty_t len = 1;

    modbus_reg_t status = modbus_master_read_input_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(len, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadMaxQtyInputRegisterRequest)
{
    modbus_adr_t adr = 0x0005;
    modbus_data_qty_t len = MODBUS_MAX_REG_QTY;
    modbus_ret_t status = modbus_master_read_input_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadInputRegisterMaxQtyPlus1Request)
{
    modbus_adr_t adr = 0x0005;
    modbus_data_qty_t len = MODBUS_MAX_REG_QTY + 1;
    modbus_ret_t status = modbus_master_read_input_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadZeroInputRegisterRequest)
{
    modbus_adr_t adr = 0x0005;
    modbus_data_qty_t len = 0;
    modbus_ret_t status = modbus_master_read_input_reg(PDU_frame, adr, len);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadDiscreteInputsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = 5;

    modbus_ret_t status = modbus_master_read_discrete_inputs(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadMaxQtyDiscreteInputsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = MODBUS_MAX_DISCRETE_INPUTS_QTY;

    modbus_ret_t status = modbus_master_read_discrete_inputs(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadMaxQtPlus1DiscreteInputsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = MODBUS_MAX_DISCRETE_INPUTS_QTY + 1;
    modbus_ret_t status = modbus_master_read_discrete_inputs(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadZeroDiscreteInputsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = 0;
    modbus_ret_t status = modbus_master_read_discrete_inputs(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadCoilsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = 5;

    modbus_ret_t status = modbus_master_read_coils(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(input_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadCoilsMaxQtyRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = MODBUS_MAX_COILS_QTY;
    modbus_ret_t status = modbus_master_read_coils(PDU_frame, adr, input_qty);
    TEST_ASSERT_EQUAL_INT16(MODBUS_READ_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, ReadCoilsMaxQtyPlus1Request)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = MODBUS_MAX_COILS_QTY + 1;
    modbus_ret_t status = modbus_master_read_coils(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, ReadZeroCoilsRequest)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t input_qty = 0;
    modbus_ret_t status = modbus_master_read_coils(PDU_frame, adr, input_qty);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, WriteSingleRegister)
{
    modbus_adr_t adr = 0x0009;
    modbus_reg_t value = 0x0012;

    modbus_ret_t status= modbus_master_write_single_reg(PDU_frame, adr, value);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(value, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_WRITE_SINGLE_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, WriteSingleCoilOn)
{
    modbus_adr_t adr = 0x0009;
    modbus_w_coil_t coil_state = COIL_ON;

    modbus_ret_t status= modbus_master_write_single_coil(PDU_frame, adr, coil_state);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(coil_state, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_INT16(MODBUS_WRITE_SINGLE_REQUEST_LEN, status);
}

TEST(Modbus_Master_Requests, WriteMultipleRegisters)
{
    modbus_adr_t adr = 0x0080;
    modbus_reg_t values[5] = {1, 2, 3, 4, 5};
    modbus_data_qty_t reg_qty = 5;

    modbus_ret_t status = modbus_master_write_multiple_reg(PDU_frame, adr, reg_qty, values);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(reg_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_UINT8((reg_qty * 2), PDU_frame[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_UINT16(values[0], read_u16_from_buf(PDU_frame + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX));
    TEST_ASSERT_EQUAL_UINT16(values[1], read_u16_from_buf(PDU_frame + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 2));
    TEST_ASSERT_EQUAL_UINT16(values[2], read_u16_from_buf(PDU_frame + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 4));
    TEST_ASSERT_EQUAL_UINT16(values[3], read_u16_from_buf(PDU_frame + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 6));
    TEST_ASSERT_EQUAL_UINT16(values[4], read_u16_from_buf(PDU_frame + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 8));
    TEST_ASSERT_EQUAL_INT16(MODBUS_WRITE_MULTI_REQUEST_LEN+(reg_qty*2), status);
}

TEST(Modbus_Master_Requests, WriteMaxQtyMultipleRegisters)
{
    modbus_adr_t adr = 0x0080;
    modbus_reg_t values[MODBUS_MAX_REG_QTY] = {0x5A5A};
    modbus_data_qty_t reg_qty = MODBUS_MAX_REG_QTY;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg(PDU_frame, adr, reg_qty, values);
    TEST_ASSERT_EQUAL_INT16(MODBUS_WRITE_MULTI_REQUEST_LEN+(reg_qty*2), status);
}

TEST(Modbus_Master_Requests, WriteMultipleRegistersMaxQtyPlus1)
{
    modbus_adr_t adr = 0x0080;
    modbus_reg_t values[MODBUS_MAX_REG_QTY + 1] = {0x5A5A};
    modbus_data_qty_t reg_qty = MODBUS_MAX_REG_QTY + 1;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg(PDU_frame, adr, reg_qty, values);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, WriteZeroMultipleRegisters)
{
    modbus_adr_t adr = 0x0080;
    modbus_reg_t values[MODBUS_MAX_REG_QTY] = {0x5A5A};
    modbus_data_qty_t reg_qty = 0;
    modbus_ret_t status;

    status = modbus_master_write_multiple_reg(PDU_frame, adr, reg_qty, values);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Modbus_Master_Requests, Write5MultipleCoils)
{
    modbus_adr_t adr = 0x0009;
    modbus_data_qty_t coils_qty=5;
    modbus_coil_reg_t coil_states[1] = {0x15};
    modbus_byte_count_t expected_byte_cnt = 1;
    modbus_ret_t status;

    status=modbus_master_write_multiple_coils(PDU_frame, adr,coils_qty, coil_states);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(coils_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt,PDU_frame[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_UINT8 (coil_states[0],PDU_frame[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX]);
    TEST_ASSERT_EQUAL (RET_OK,status);
}

TEST(Modbus_Master_Requests, WriteMaxQtyMultipleCoils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coils_qty=MODBUS_MAX_COILS_QTY;
    modbus_coil_reg_t coil_states[250] = {0x55};
    modbus_byte_count_t expected_byte_cnt = 250;
    modbus_ret_t status;

    status=modbus_master_write_multiple_coils(PDU_frame, adr,coils_qty, coil_states);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, PDU_frame[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(adr, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(coils_qty, read_u16_from_buf(PDU_frame + MODBUS_REQUEST_LEN_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt,PDU_frame[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL (RET_OK,status);
}

TEST(Modbus_Master_Requests, WriteMultipleCoilsMaxQtyPlus1)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coils_qty=MODBUS_MAX_COILS_QTY+1;
    modbus_coil_reg_t coil_states[251] = {0x55};
    modbus_ret_t status;

    status=modbus_master_write_multiple_coils(PDU_frame, adr,coils_qty, coil_states);
    TEST_ASSERT_EQUAL (RET_ERROR,status);
}

TEST(Modbus_Master_Requests, WriteZeroMultipleCoils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coils_qty=0;
    modbus_coil_reg_t coil_states[1] = {0};
    modbus_ret_t status;

    status=modbus_master_write_multiple_coils(PDU_frame, adr,coils_qty, coil_states);
    TEST_ASSERT_EQUAL (RET_ERROR,status);
}
//
// testy na zerową ilość rejestrów coili do odczytu zapisu.