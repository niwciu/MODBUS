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
    mock_set_all_slave_cails_to_off();
    mock_set_all_slave_din_to_off();
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


//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 