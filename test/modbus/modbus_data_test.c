#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_app_data.h"



TEST_GROUP(Modbus_Data);

TEST_SETUP(Modbus_Data)
{
    /* Init before every test */
    mock_clear_modbus_coil_data_teble();
}

TEST_TEAR_DOWN(Modbus_Data)
{
    /* Cleanup after every test */
}

TEST(Modbus_Data, RegisterCoilData)
{
    modbus_coil_t app_coil_data=1;
    modbus_adr_t coil_adr=0x0001;
    register_app_data_to_coil_table(coil_adr,&app_coil_data);
    
    TEST_ASSERT_EQUAL(&app_coil_data,Coils[coil_adr]);
}

TEST(Modbus_Data, ReadRegisteredCoil)
{
    modbus_coil_t app_coil_data=1;
    modbus_adr_t coil_adr=0x0001;
    register_app_data_to_coil_table(coil_adr,&app_coil_data);
    
    TEST_ASSERT_EQUAL(app_coil_data,get_coil_state(coil_adr));
}

TEST(Modbus_Data, ReadUnregisteredCoil)
{
    modbus_adr_t coil_adr=0x0001;
    TEST_ASSERT_EQUAL(0,get_coil_state(coil_adr));
}

//
// testy na zerową ilość rejestrów coili do odczytu zapisu. 