#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "mock_slave_app_data.h"

TEST_GROUP(Modbus_Data);

TEST_SETUP(Modbus_Data)
{
    /* Init before every test */
    mock_clear_modbus_slave_coils_data_table();
    mock_clear_modbus_slave_discrete_inputs_data_table();
    mock_clear_modbus_slave_input_register_data_table();
    mock_clear_modbus_slave_holding_resgister_data_table();
}

TEST_TEAR_DOWN(Modbus_Data)
{
    /* Cleanup after every test */
}

// Slave_Coils Data tests
TEST(Modbus_Data, RegisterCoilData)
{
    modbus_coil_disin_t app_coil_data = 1;
    modbus_adr_t coil_adr = 0x0001;
    // register_app_data_to_slave_coils_table(coil_adr, &app_coil_data);
    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr,&app_coil_data);

    TEST_ASSERT_EQUAL(&app_coil_data, Slave_Coils[coil_adr]);
}

TEST(Modbus_Data, ReadRegisteredCoil)
{
    modbus_coil_disin_t app_coil_data = 1;
    modbus_adr_t coil_adr = 0x0001;
    // register_app_data_to_slave_coils_table(coil_adr, &app_coil_data);
    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr,&app_coil_data);

    TEST_ASSERT_EQUAL(app_coil_data, get_coil_state(Slave_Coils, coil_adr));
}

TEST(Modbus_Data, ReadUnregisteredCoil)
{
    modbus_adr_t coil_adr = 0x0001;
    TEST_ASSERT_EQUAL(0, get_coil_state(Slave_Coils, coil_adr));
}

TEST(Modbus_Data, WriteRegisteredCoil)
{
    modbus_coil_disin_t app_coil_data = 1;
    modbus_adr_t coil_adr = 0x0001;
    modbus_ret_t status;

    mock_register_slave_coils_data();

    status = set_coil_state(Slave_Coils, coil_adr, app_coil_data);

    TEST_ASSERT_EQUAL(app_coil_data, mock_slave_coil[coil_adr]);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Modbus_Data, WriteUnregisteredCoil)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_coil_disin_t app_coil_data = 1;
    mock_set_all_slave_cails_to_off();
    TEST_ASSERT_EQUAL(0, mock_slave_coil[coil_adr]);
    TEST_ASSERT_EQUAL(RET_ERROR, set_coil_state(Slave_Coils, coil_adr, app_coil_data));
}

// Discrete Inputs Data tests
TEST(Modbus_Data, RegisterDiscreteInputData)
{
    modbus_coil_disin_t app_din_data = 1;
    modbus_adr_t din_adr = 0x0001;
    // register_app_data_to_slave_discrete_inputs_table(din_adr, &app_din_data);
    register_app_data_to_modbus_coils_din_table(Slave_Discrete_Inputs,din_adr,&app_din_data);

    TEST_ASSERT_EQUAL(&app_din_data, Slave_Discrete_Inputs[din_adr]);
}

TEST(Modbus_Data, ReadRegisteredDiscreteInput)
{
    modbus_coil_disin_t app_din_data = 1;
    modbus_adr_t din_adr = 0x0001;
    // register_app_data_to_slave_discrete_inputs_table(din_adr, &app_din_data);
    register_app_data_to_modbus_coils_din_table(Slave_Discrete_Inputs,din_adr,&app_din_data);

    TEST_ASSERT_EQUAL(app_din_data, get_discrete_input_state(Slave_Discrete_Inputs, din_adr));
}

TEST(Modbus_Data, ReadUnegisteredDiscreteInput)
{
    modbus_adr_t din_adr = 0x0001;

    TEST_ASSERT_EQUAL(0, get_discrete_input_state(Slave_Discrete_Inputs, din_adr));
}

// Input Registers Data tests
TEST(Modbus_Data, RegisterInputRegisterData)
{
    modbus_reg_t app_inreg_data = 0x5A5A;
    modbus_adr_t inreg_adr = 0x0001;
    // register_app_data_to_slave_input_registers_table(inreg_adr, &app_inreg_data);
    register_app_data_to_modbus_reg_table(Slave_Input_Registers,inreg_adr,&app_inreg_data);

    TEST_ASSERT_EQUAL(&app_inreg_data, Slave_Input_Registers[inreg_adr]);
}

TEST(Modbus_Data, ReadRegisteredInputRegister)
{
    modbus_reg_t app_inreg_data = 0x5A5A;
    modbus_adr_t inreg_adr = 0x0001;
    // register_app_data_to_slave_input_registers_table(inreg_adr, &app_inreg_data);
    register_app_data_to_modbus_reg_table(Slave_Input_Registers,inreg_adr,&app_inreg_data);

    TEST_ASSERT_EQUAL_HEX16(app_inreg_data, get_input_register_state(Slave_Input_Registers, inreg_adr));
}

TEST(Modbus_Data, ReadUnregisteredInputRegister)
{
    modbus_adr_t inreg_adr = 0x0001;

    TEST_ASSERT_EQUAL(0, get_input_register_state(Slave_Input_Registers, inreg_adr));
}

// Holding Registers Data tests
TEST(Modbus_Data, RegisterHoldingRegisterData)
{
    modbus_reg_t app_hreg_data = 0xA5A5;
    modbus_adr_t hreg_adr = 0x0001;
    // register_app_data_to_slave_holding_registers_table(hreg_adr, &app_hreg_data);
    register_app_data_to_modbus_reg_table(Slave_Holding_Registers,hreg_adr,&app_hreg_data);

    TEST_ASSERT_EQUAL(&app_hreg_data, Slave_Holding_Registers[hreg_adr]);
}

TEST(Modbus_Data, ReadRegisteredHoldingRegister)
{
    modbus_reg_t app_hreg_data = 0xA5A5;
    modbus_adr_t hreg_adr = 0x0001;
    // register_app_data_to_slave_holding_registers_table(hreg_adr, &app_hreg_data);
    register_app_data_to_modbus_reg_table(Slave_Holding_Registers,hreg_adr,&app_hreg_data);

    TEST_ASSERT_EQUAL_HEX16(app_hreg_data, get_holding_register_value(Slave_Holding_Registers, hreg_adr));
}

TEST(Modbus_Data, ReadUnregisteredHoldingRegister)
{
    modbus_adr_t hreg_adr = 0x0001;

    TEST_ASSERT_EQUAL(0, get_holding_register_value(Slave_Holding_Registers, hreg_adr));
}
TEST(Modbus_Data, WriteRegisteredHoldingRegister)
{
    modbus_adr_t hreg_adr = 0x0001;
    modbus_reg_t hreg_val = 0x5a5a;
    modbus_ret_t status;
    mock_reset_all_slave_hreg_value();
    mock_register_slave_holding_registers_data();
    status = set_register_value(Slave_Holding_Registers, hreg_adr, hreg_val);

    TEST_ASSERT_EQUAL_HEX16(hreg_val, mock_slave_hreg[hreg_adr]);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Modbus_Data, WriteUnregisteredHoldingRegister)
{
    modbus_adr_t hreg_adr = 0x0001;
    modbus_reg_t hreg_val = 0x5a5a;

    TEST_ASSERT_EQUAL(RET_ERROR, set_register_value(Slave_Holding_Registers, hreg_adr, hreg_val));
}

//
// testy na zerową ilość rejestrów coili do odczytu zapisu.