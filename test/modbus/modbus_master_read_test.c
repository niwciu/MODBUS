#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_master_app_data.h"
#include "mock_slave_app_data.h"
#include "buf_rw.h"


static uint8_t req_buf[MODBUS_RTU_BUFFER_SIZE] = {0};
static uint8_t resp_buf[MODBUS_RTU_BUFFER_SIZE] = {0};
static modbus_req_resp_t req;
static modbus_req_resp_t resp;

static modbus_ret_t status;

static modbus_req_resp_t *RTU_req_frame=&req;
static modbus_req_resp_t *RTU_resp_frame=&resp;

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;

// modbus_ret_t status;

TEST_GROUP(Modbus_Master_Read);

TEST_SETUP(Modbus_Master_Read)
{
    /* Init before every test */
    mock_register_master_coils_data();
    mock_register_master_discrete_inputs_data();
    mock_register_master_input_registers_data();
    mock_register_master_holding_registers_data();

    mock_register_slave_coils_data();
    mock_register_slave_discrete_inputs_data();
    mock_register_slave_input_registers_data();
    mock_register_slave_holding_registers_data();

    mock_set_all_master_cails_to_off();
    mock_set_all_master_din_to_off();
    mock_reset_all_master_inreg_value();
    mock_reset_all_master_hreg_value();

    mock_set_all_slave_cails_to_off();
    mock_set_all_slave_din_to_off();
    mock_reset_all_slave_inreg_value();
    mock_reset_all_slave_hreg_value();

    req.data=req_buf;
    resp.data=resp_buf;
    modbus_msg.req=RTU_req_frame;
    modbus_msg.resp=RTU_resp_frame;
}

TEST_TEAR_DOWN(Modbus_Master_Read)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);

    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    modbus_slave_read_coils(RTU_msg);

    status=modbus_master_read_coils_resp(RTU_msg);
    
    TEST_ASSERT_EQUAL(RET_OK,status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_coil, mock_master_coil, coil_adr + coils_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    modbus_coil_disin_t expected_master_coil_value[MODBUS_MAX_READ_COILS_QTY] = {0};

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);

    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    modbus_slave_read_coils(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;

    modbus_master_read_coils_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_master_coil_value, mock_master_coil, coil_adr + coils_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterReadCoilReturnErroFuncCode)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);

    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    modbus_slave_read_coils(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_read_coils_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    modbus_slave_read_discrete_inputs(RTU_msg);

    modbus_master_read_discrete_inputs_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in, mock_master_dis_in, disin_adr + disin_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;
    modbus_coil_disin_t expected_master_din_value[MODBUS_MAX_READ_COILS_QTY] = {0};

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    modbus_slave_read_discrete_inputs(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_discrete_inputs_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_master_din_value, mock_master_dis_in, disin_adr + disin_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterReadDiscreteInputsReturnErroFuncCode)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    modbus_slave_read_discrete_inputs(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_read_discrete_inputs_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    modbus_slave_read_input_reg(RTU_msg);

    modbus_master_read_input_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg, mock_master_inreg, in_reg_adr + in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_buf_t expected_inreg_val[INPUT_REG_QTY] = {0};

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    modbus_slave_read_input_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_input_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val, mock_master_inreg, in_reg_adr + in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterReadInputRegistersReturnErroFuncCode)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    modbus_slave_read_input_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_read_input_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_buf_t expected_inreg_val[INPUT_REG_QTY] = {0};

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    modbus_slave_read_input_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;
    modbus_master_read_input_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val, mock_master_inreg, in_reg_adr + in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterReadInputRegistersReturnErrorByteCntCode)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    modbus_slave_read_input_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT, modbus_master_read_input_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersThenMasterHoldingRegistersUpdateToHoldingRegistersValue)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 6;

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    modbus_slave_read_holding_reg(RTU_msg);

    modbus_master_read_holding_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_hreg, mock_master_hreg, hreg_adr + hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_buf_t expected_hreg_val[INPUT_REG_QTY] = {0};

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    modbus_slave_read_holding_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_holding_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val, mock_master_hreg, hreg_adr + hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersReturnErroFuncCode)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    modbus_slave_read_holding_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_read_holding_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_buf_t expected_hreg_val[INPUT_REG_QTY] = {0};

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    modbus_slave_read_holding_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;
    modbus_master_read_holding_reg_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val, mock_master_hreg, hreg_adr + hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterReadHoldingRegistersReturnErrorByteCntCode)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    modbus_slave_read_holding_reg(RTU_msg);
    RTU_msg->resp->data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 9;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT, modbus_master_read_holding_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleCoilRespAndOutputAddressIsCorrectThenMasterWriteSingleCoilRespRetOk)
{
    modbus_adr_t coil_adr = 0x0003;

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    modbus_slave_write_single_coil(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_write_single_coil_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress)
{
    modbus_adr_t coil_adr = 0x0003;

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    modbus_slave_write_single_coil(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_write_single_coil_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode)
{
    modbus_adr_t coil_adr = 0x0003;

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    modbus_slave_write_single_coil(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++;
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_write_single_coil_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectcoilValueWhenMasterWriteSignleCoilThenMasterWriteSingleCoilRetValueError)
{
    modbus_adr_t coil_adr = 0x0003;

    mock_master_coil[coil_adr] = !!COIL_ON;
    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    ;
    modbus_slave_write_single_coil(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], !!COIL_OFF);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_write_single_coil_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsCorrectThenMasterWriteSingleRegisterRespRetOk)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    mock_master_hreg[reg_adr] = reg_val;

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    modbus_slave_write_single_reg(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_write_single_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsIncorrectThenMasterWriteSingleRegisterRespRetErrorOutAddr)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    mock_master_hreg[reg_adr] = reg_val;

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    modbus_slave_write_single_reg(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_write_single_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    mock_master_hreg[reg_adr] = reg_val;

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    modbus_slave_write_single_reg(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++; // its 2 byte data so im incrementing LSB

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_write_single_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectRegValueWhenMasterWriteSignleRegThenMasterWriteSingleRegRetValueError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    mock_master_hreg[reg_adr] = reg_val;

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    modbus_slave_write_single_reg(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_write_single_reg_resp(RTU_msg));
}
TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOk)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 30;

    mock_set_expected_master_coils_alternately(coil_adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    modbus_slave_write_multiple_coils(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_write_multiple_coils_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetErrorFuncCode)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 30;

    mock_set_expected_master_coils_alternately(coil_adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    modbus_slave_write_multiple_coils(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_write_multiple_coils_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutAdrError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 30;

    mock_set_expected_master_coils_alternately(coil_adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    modbus_slave_write_multiple_coils(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_write_multiple_coils_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutQtyError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 30;

    mock_set_expected_master_coils_alternately(coil_adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    modbus_slave_write_multiple_coils(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coil_qty + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_write_multiple_coils_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOk)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 30;

    mock_set_expected_master_hreg_alternately(reg_adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    modbus_slave_write_multiple_reg(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_write_multiple_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetErrorFuncCode)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 30;

    mock_set_expected_master_hreg_alternately(reg_adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    modbus_slave_write_multiple_reg(RTU_msg);

    RTU_msg->resp->data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE, modbus_master_write_multiple_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutAdrError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 30;

    mock_set_expected_master_hreg_alternately(reg_adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    modbus_slave_write_multiple_reg(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_write_multiple_reg_resp(RTU_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutQtyError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 30;

    mock_set_expected_master_hreg_alternately(reg_adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    modbus_slave_write_multiple_reg(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp->data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], reg_qty + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_write_multiple_reg_resp(RTU_msg));
}

// TEST(Modbus_Master_Read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//