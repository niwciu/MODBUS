#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "modbus_RTU.h"
#include "mock_slave_app_data.h"
#include "buf_rw.h" 
#include <memory.h>

static modbus_ret_t status;

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
static modbus_msg_t *null_ptr_msg;

// modbus_ret_t status;

TEST_GROUP(Master_PDU_read);

TEST_SETUP(Master_PDU_read)
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

TEST_TEAR_DOWN(Master_PDU_read)
{
    /* Cleanup after every test */
}

TEST(Master_PDU_read, MasterReadCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadDiscreteInputsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadInputRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadHoldingRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteSingleCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteSingleRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteMultipleCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteMultipleRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    modbus_coil_disin_t readed_coil_val[coils_qty];
    RTU_msg->rw_data_ptr = (void *)(readed_coil_val);

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], readed_coil_val, coils_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    modbus_coil_disin_t expected_master_coil_value[4] = {!!COIL_ON};
    modbus_coil_disin_t master_coil_2_update[4] = {!!COIL_ON};
    mock_set_all_slave_cails_to_off();
    RTU_msg->rw_data_ptr = (void *)(expected_master_coil_value);

    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_master_coil_value, master_coil_2_update, coils_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterReadCoilReturnErroFuncCode)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    modbus_coil_disin_t readed_coils[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_coils);

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);

    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;
    modbus_coil_disin_t readed_disin[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_disin);

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, readed_disin, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;
    modbus_coil_disin_t readed_disin[4] = {0};
    modbus_coil_disin_t expected_dis_in[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_disin);

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_dis_in, readed_disin, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterReadDiscreteInputsReturnErroFuncCode)
{
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;
    modbus_coil_disin_t readed_disin[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_disin);

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, disin_adr, disin_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_reg_t readed_inreg[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_inreg);

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, readed_inreg, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_reg_t expected_inreg_val[4] = {0};
    modbus_reg_t readed_inreg[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_inreg);

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val, readed_inreg, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterReadInputRegistersReturnErroFuncCode)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_reg_t readed_inreg[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_inreg);

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_reg_t expected_inreg_val[4] = {0};
    modbus_reg_t readed_inreg[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_inreg);

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val, readed_inreg, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterReadInputRegistersReturnErrorByteCntCode)
{
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    modbus_reg_t readed_inreg[4] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_inreg);

    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, in_reg_adr, in_reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersThenMasterHoldingRegistersUpdateToHoldingRegistersValue)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 6;
    modbus_reg_t readed_hreg[6] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_hreg);

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_hreg + hreg_adr, readed_hreg, hreg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_reg_t expected_hreg_val[7] = {0};
    modbus_reg_t readed_hreg[7] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_hreg);

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val, readed_hreg, hreg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersReturnErroFuncCode)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_reg_t readed_hreg[7] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_hreg);

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_reg_t expected_hreg_val[7] = {0};
    modbus_reg_t readed_hreg[7] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_hreg);

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 7;
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val, readed_hreg, hreg_qty);
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterReadHoldingRegistersReturnErrorByteCntCode)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 7;
    modbus_reg_t readed_hreg[7] = {0};
    RTU_msg->rw_data_ptr = (void *)(readed_hreg);

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, hreg_adr, hreg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX] = 9;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleCoilRespAndOutputAddressIsCorrectThenMasterWriteSingleCoilRespRetOk)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectOutputAdrAndIncorrectCoilValueWhenMasterWriteSignleCoilThenMasterWriteSingleCoilRetValueError)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);

    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], !!COIL_OFF);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

    modbus_master_write_single_coil_req(RTU_msg, coil_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsCorrectThenMasterWriteSingleRegisterRespRetOk)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    RTU_msg->rw_data_ptr = (void *)(&reg_val);

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsIncorrectThenMasterWriteSingleRegisterRespRetErrorOutAddr)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    RTU_msg->rw_data_ptr = (void *)(&reg_val);

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    RTU_msg->rw_data_ptr = (void *)(&reg_val);

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // its 2 byte data so im incrementing LSB

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectRegValueWhenMasterWriteSignleRegThenMasterWriteSingleRegRetValueError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    RTU_msg->rw_data_ptr = (void *)(&reg_val);

    modbus_master_write_single_reg_req(RTU_msg, reg_adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOk)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetErrorFuncCode)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutAdrError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutQtyError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coil_qty + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOk)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetErrorFuncCode)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutAdrError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
}

TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutQtyError)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    RTU_msg->rw_data_ptr = (void *)(coils_2_write);

    modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], reg_qty + 1);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_read_slave_resp(RTU_msg));
}

// TEST(Master_PDU_read_exception_code, WhenRegisterModbusErrorCbCalledThenModbusErrorCbEqualToRegisteredFunctionPointer)
// {

//     TEST_ASSERT_EQUAL(&error_test_func, modbus_error_callback);
// }

TEST(Master_PDU_read, GivenMasterReadCoilReqSendedAndWhenFunctionExceptionCodeMaskRecivedThenModbusMasterReadSlaveReturnExceptionCodeRecivedStatus)
{
    modbus_device_ID_t Slave_ID = 0x03;
    modbus_buf_t read_coil_ex_code_01_resp[] = {0x03, 0x81, 0x01, 0x20, 0x50};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_01_resp) / sizeof(modbus_buf_t);
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    // modbus_coil_disin_t readed_coil_val[coils_qty];
    // RTU_msg->rw_data_ptr = (void *)(readed_coil_val);

    // generating correct req RTU_msg
    modbus_master_read_coils_req(RTU_msg, coil_adr, coils_qty);
    modbus_RTU_send(RTU_msg->req.data, &RTU_msg->req.len, Slave_ID);
    // generate fun code 01 resp RTU msg
    memcpy(RTU_msg->resp.data, read_coil_ex_code_01_resp, buf_len);
    RTU_msg->resp.len = buf_len;

    status = modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL(RET_ERROR_EXCEPTION_CODE_RECIVED, status);
}
// // TEST(Master_PDU_read, )
// // {
// //     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// // }
// //
// //