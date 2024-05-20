#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_master_app_data.h"
#include "mock_slave_app_data.h"


uint8_t req_msg[MODBUS_RTU_BUFFER_SIZE] = {0};
uint8_t resp_msg[MODBUS_RTU_BUFFER_SIZE] = {0};

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
}

TEST_TEAR_DOWN(Modbus_Master_Read)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=4;

    mock_set_expected_slave_coils_alternately(coil_adr,coils_qty,!!COIL_ON);

    modbus_master_read_coils_req(req_msg,coil_adr,coils_qty);
    modbus_slave_read_coils(resp_msg,req_msg);

    modbus_master_read_coils_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_coil,mock_master_coil,coil_adr+coils_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=4;
    modbus_coil_disin_t expected_master_coil_value[MODBUS_MAX_COILS_QTY]={0};

    mock_set_expected_slave_coils_alternately(coil_adr,coils_qty,!!COIL_ON);

    modbus_master_read_coils_req(req_msg,coil_adr,coils_qty);
    modbus_slave_read_coils(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;

    modbus_master_read_coils_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_master_coil_value,mock_master_coil,coil_adr+coils_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterReadCoilReturnErroFuncCode)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=4;

    mock_set_expected_slave_coils_alternately(coil_adr,coils_qty,!!COIL_ON);

    modbus_master_read_coils_req(req_msg,coil_adr,coils_qty);
    modbus_slave_read_coils(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE,modbus_master_read_coils_resp(resp_msg,req_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue)
{
    modbus_adr_t disin_adr=0x0001;
    modbus_data_qty_t disin_qty=4;

    mock_set_expected_slave_disc_in_alternately(disin_adr,disin_qty);

    modbus_master_read_discrete_inputs_req(req_msg,disin_adr,disin_qty);
    modbus_slave_read_discrete_inputs(resp_msg,req_msg);

    modbus_master_read_discrete_inputs_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in,mock_master_dis_in,disin_adr+disin_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged)
{
    modbus_adr_t disin_adr=0x0001;
    modbus_data_qty_t disin_qty=4;
    modbus_coil_disin_t expected_master_din_value[MODBUS_MAX_COILS_QTY]={0};

    mock_set_expected_slave_disc_in_alternately(disin_adr,disin_qty);

    modbus_master_read_discrete_inputs_req(req_msg,disin_adr,disin_qty);
    modbus_slave_read_discrete_inputs(resp_msg,req_msg);

    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;
    modbus_master_read_discrete_inputs_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_master_din_value,mock_master_dis_in,disin_adr+disin_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterReadDiscreteInputsReturnErroFuncCode)
{
    modbus_adr_t disin_adr=0x0001;
    modbus_data_qty_t disin_qty=4;

    mock_set_expected_slave_disc_in_alternately(disin_adr,disin_qty);

    modbus_master_read_discrete_inputs_req(req_msg,disin_adr,disin_qty);
    modbus_slave_read_discrete_inputs(resp_msg,req_msg);

    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE,modbus_master_read_discrete_inputs_resp(resp_msg,req_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue)
{
    modbus_adr_t in_reg_adr=0x0001;
    modbus_data_qty_t in_reg_qty=4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr,in_reg_qty,0x5A5A);

    modbus_master_read_input_reg_req(req_msg,in_reg_adr,in_reg_qty);
    modbus_slave_read_input_reg(resp_msg,req_msg);

    modbus_master_read_input_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg,mock_master_inreg,in_reg_adr+in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr=0x0001;
    modbus_data_qty_t in_reg_qty=4;
    modbus_buf_t expected_inreg_val[INPUT_REG_QTY]={0};

    mock_set_expected_slave_input_reg_alternately(in_reg_adr,in_reg_qty,0x5A5A);

    modbus_master_read_input_reg_req(req_msg,in_reg_adr,in_reg_qty);
    modbus_slave_read_input_reg(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;
    modbus_master_read_input_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val,mock_master_inreg,in_reg_adr+in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterReadInputRegistersReturnErroFuncCode)
{
    modbus_adr_t in_reg_adr=0x0001;
    modbus_data_qty_t in_reg_qty=4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr,in_reg_qty,0x5A5A);

    modbus_master_read_input_reg_req(req_msg,in_reg_adr,in_reg_qty);
    modbus_slave_read_input_reg(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;
    
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE,modbus_master_read_input_reg_resp(resp_msg,req_msg));
}
 
TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterInputRegistersStayUnchanged)
{
    modbus_adr_t in_reg_adr=0x0001;
    modbus_data_qty_t in_reg_qty=4;
    modbus_buf_t expected_inreg_val[INPUT_REG_QTY]={0};

    mock_set_expected_slave_input_reg_alternately(in_reg_adr,in_reg_qty,0x5A5A);

    modbus_master_read_input_reg_req(req_msg,in_reg_adr,in_reg_qty);
    modbus_slave_read_input_reg(resp_msg,req_msg);
    resp_msg[MODBUS_RESP_READ_BYTE_CNT_IDX]=7;
    modbus_master_read_input_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_inreg_val,mock_master_inreg,in_reg_adr+in_reg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterReadInputRegistersReturnErrorByteCntCode)
{
    modbus_adr_t in_reg_adr=0x0001;
    modbus_data_qty_t in_reg_qty=4;

    mock_set_expected_slave_input_reg_alternately(in_reg_adr,in_reg_qty,0x5A5A);

    modbus_master_read_input_reg_req(req_msg,in_reg_adr,in_reg_qty);
    modbus_slave_read_input_reg(resp_msg,req_msg);
    resp_msg[MODBUS_RESP_READ_BYTE_CNT_IDX]=7;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT,modbus_master_read_input_reg_resp(resp_msg,req_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersThenMasterHoldingRegistersUpdateToHoldingRegistersValue)
{
    modbus_adr_t hreg_adr=0x0003;
    modbus_data_qty_t hreg_qty=6;

    mock_set_expected_slave_hreg_alternately(hreg_adr,hreg_qty,0x5A5A);

    modbus_master_read_holding_reg_req(req_msg,hreg_adr,hreg_qty);
    modbus_slave_read_holding_reg(resp_msg,req_msg);

    modbus_master_read_holding_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_hreg,mock_master_hreg,hreg_adr+hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr=0x0003;
    modbus_data_qty_t hreg_qty=7;
    modbus_buf_t expected_hreg_val[INPUT_REG_QTY]={0};

    mock_set_expected_slave_hreg_alternately(hreg_adr,hreg_qty,0x5A5A);

    modbus_master_read_holding_reg_req(req_msg,hreg_adr,hreg_qty);
    modbus_slave_read_holding_reg(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;
    modbus_master_read_holding_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val,mock_master_hreg,hreg_adr+hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersReturnErroFuncCode)
{
    modbus_adr_t hreg_adr=0x0003;
    modbus_data_qty_t hreg_qty=7;

    mock_set_expected_slave_hreg_alternately(hreg_adr,hreg_qty,0x5A5A);

    modbus_master_read_holding_reg_req(req_msg,hreg_adr,hreg_qty);
    modbus_slave_read_holding_reg(resp_msg,req_msg);
    resp_msg[MODBUS_FUNCTION_CODE_IDX]=0x95;
    
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE,modbus_master_read_holding_reg_resp(resp_msg,req_msg));
}
 
TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterHoldingRegistersStayUnchanged)
{
    modbus_adr_t hreg_adr=0x0003;
    modbus_data_qty_t hreg_qty=7;
    modbus_buf_t expected_hreg_val[INPUT_REG_QTY]={0};

    mock_set_expected_slave_hreg_alternately(hreg_adr,hreg_qty,0x5A5A);

    modbus_master_read_holding_reg_req(req_msg,hreg_adr,hreg_qty);
    modbus_slave_read_holding_reg(resp_msg,req_msg);
    resp_msg[MODBUS_RESP_READ_BYTE_CNT_IDX]=7;
    modbus_master_read_holding_reg_resp(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(expected_hreg_val,mock_master_hreg,hreg_adr+hreg_qty);
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterReadHoldingRegistersReturnErrorByteCntCode)
{
    modbus_adr_t hreg_adr=0x0003;
    modbus_data_qty_t hreg_qty=7;

    mock_set_expected_slave_hreg_alternately(hreg_adr,hreg_qty,0x5A5A);

    modbus_master_read_holding_reg_req(req_msg,hreg_adr,hreg_qty);
    modbus_slave_read_holding_reg(resp_msg,req_msg);
    resp_msg[MODBUS_RESP_READ_BYTE_CNT_IDX]=9;

    TEST_ASSERT_EQUAL_INT16(RET_ERROR_BYTE_CNT,modbus_master_read_holding_reg_resp(resp_msg,req_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleCoilRespThenOutputAddressInSlaveRespIsCorrect)
{
    modbus_adr_t coil_adr =0x0003;

    modbus_master_write_single_coil_req(req_msg,coil_adr,COIL_ON);
    modbus_slave_write_single_coil(resp_msg,req_msg);

    TEST_ASSERT_EQUAL_INT16(RET_OK,modbus_master_write_single_coil_resp(resp_msg,req_msg));

}

TEST(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress)
{
    modbus_adr_t coil_adr =0x0003;

    modbus_master_write_single_coil_req(req_msg,coil_adr,COIL_ON);
    modbus_slave_write_single_coil(resp_msg,req_msg);

    resp_msg[MODBUS_RESP_WRITE_ADR_IDX] = coil_adr+1;
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_ADR,modbus_master_write_single_coil_resp(resp_msg,req_msg));
}

TEST(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode)
{
    modbus_adr_t coil_adr =0x0003;

    modbus_master_write_single_coil_req(req_msg,coil_adr,COIL_ON);
    modbus_slave_write_single_coil(resp_msg,req_msg);

    resp_msg[MODBUS_FUNCTION_CODE_IDX] = resp_msg[MODBUS_FUNCTION_CODE_IDX]+1;
    TEST_ASSERT_EQUAL_INT16(RET_ERROR_FUN_CODE,modbus_master_write_single_coil_resp(resp_msg,req_msg));

}

// TEST(Modbus_Master_Read, )
// {
    
// TEST_FAIL_MESSAGE("ADDED NEW TEST!!!");
// }

// TEST(Modbus_Master_Read, )
// {
    
// TEST_FAIL_MESSAGE("ADDED NEW TEST!!!");
// }

// TEST(Modbus_Master_Read, )
// {
    
// TEST_FAIL_MESSAGE("ADDED NEW TEST!!!");
// }

// TEST(Modbus_Master_Read, )
// {
    
// TEST_FAIL_MESSAGE("ADDED NEW TEST!!!");
// }
// 
// 