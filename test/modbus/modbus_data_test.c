#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_app_data.h"



TEST_GROUP(Modbus_Data);

TEST_SETUP(Modbus_Data)
{
    /* Init before every test */
    mock_clear_modbus_coils_data_teble();
    mock_clear_modbus_discrete_inputs_data_teble();
}

TEST_TEAR_DOWN(Modbus_Data)
{
    /* Cleanup after every test */
}

// Coils Data tests
TEST(Modbus_Data, RegisterCoilData)
{
    modbus_coil_t app_coil_data=1;
    modbus_adr_t coil_adr=0x0001;
    register_app_data_to_coils_table(coil_adr,&app_coil_data);
    
    TEST_ASSERT_EQUAL(&app_coil_data,Coils[coil_adr]);
}

TEST(Modbus_Data, ReadRegisteredCoil)
{
    modbus_coil_t app_coil_data=1;
    modbus_adr_t coil_adr=0x0001;
    register_app_data_to_coils_table(coil_adr,&app_coil_data);
    
    TEST_ASSERT_EQUAL(app_coil_data,get_coil_state(coil_adr));
}

TEST(Modbus_Data, ReadUnregisteredCoil)
{
    modbus_adr_t coil_adr=0x0001;
    TEST_ASSERT_EQUAL(0,get_coil_state(coil_adr));
}

// Discrete Inputs Data tests
TEST(Modbus_Data, RegisterDiscreteInputData)
{
    modbus_coil_t app_din_data=1;
    modbus_adr_t din_adr=0x0001;
    register_app_data_to_discrete_inputs_table(din_adr,&app_din_data);
    
    TEST_ASSERT_EQUAL(&app_din_data,Discrete_Inputs[din_adr]);
}

TEST(Modbus_Data, ReadRegisteredDiscreteInput)
{
    modbus_coil_t app_din_data=1;
    modbus_adr_t din_adr=0x0001;
    register_app_data_to_discrete_inputs_table(din_adr,&app_din_data);
    
    TEST_ASSERT_EQUAL(app_din_data,get_discrete_input_state(din_adr));
}

TEST(Modbus_Data, ReadUnegisteredDiscreteInput)
{
    modbus_adr_t din_adr=0x0001;
    
    TEST_ASSERT_EQUAL(0,get_discrete_input_state(din_adr));
}

// Coils Data tests
TEST(Modbus_Data, RegisterInputRegisterData)
{
    modbus_reg_t app_inreg_data=0x5A5A;
    modbus_adr_t inreg_adr=0x0001;
    register_app_data_to_input_registers_table(inreg_adr,&app_inreg_data);
    
    TEST_ASSERT_EQUAL(&app_inreg_data,Input_Registers[inreg_adr]);
}

// TEST(Modbus_Data, ReadRegisteredDiscreteInput)
// {
//     modbus_reg_t app_inreg_data=1;
//     modbus_adr_t inreg_adr=0x0001;
//     register_app_data_to_discrete_input_table(inreg_adr,&app_inreg_data);
    
//     TEST_ASSERT_EQUAL(app_inreg_data,get_discrete_input_state(inreg_adr));
// }

// TEST(Modbus_Data, ReadUnegisteredDiscreteInput)
// {
//     modbus_adr_t inreg_adr=0x0001;
    
//     TEST_ASSERT_EQUAL(0,get_discrete_input_state(inreg_adr));
// }

// TEST(Modbus_Data, RegisterDiscreteInputData)
// {
//     modbus_reg_t app_din_data=1;
//     modbus_adr_t din_adr=0x0001;
//     register_app_data_to_discrete_input_table(din_adr,&app_din_data);
    
//     TEST_ASSERT_EQUAL(&app_din_data,Discrete_Inputs[din_adr]);
// }

// TEST(Modbus_Data, ReadRegisteredDiscreteInput)
// {
//     modbus_reg_t app_din_data=1;
//     modbus_adr_t din_adr=0x0001;
//     register_app_data_to_discrete_input_table(din_adr,&app_din_data);
    
//     TEST_ASSERT_EQUAL(app_din_data,get_discrete_input_state(din_adr));
// }

// TEST(Modbus_Data, ReadUnegisteredDiscreteInput)
// {
//     modbus_adr_t din_adr=0x0001;
    
//     TEST_ASSERT_EQUAL(0,get_discrete_input_state(din_adr));
// }

//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 