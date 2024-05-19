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
// 
// testy na zerową ilość rejestrów coili do odczytu zapisu. 