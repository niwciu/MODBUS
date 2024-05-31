#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Data_test)
{
    /* Test cases to run */
    RUN_TEST_CASE(Modbus_Data_test, RegisterCoilData);
    RUN_TEST_CASE(Modbus_Data_test, ReadRegisteredCoil);
    RUN_TEST_CASE(Modbus_Data_test, ReadUnregisteredCoil);
    RUN_TEST_CASE(Modbus_Data_test, WriteRegisteredCoil);
    RUN_TEST_CASE(Modbus_Data_test, WriteUnregisteredCoil);

    RUN_TEST_CASE(Modbus_Data_test, RegisterDiscreteInputData);
    RUN_TEST_CASE(Modbus_Data_test, ReadRegisteredDiscreteInput);
    RUN_TEST_CASE(Modbus_Data_test, ReadUnegisteredDiscreteInput);

    RUN_TEST_CASE(Modbus_Data_test, RegisterInputRegisterData);
    RUN_TEST_CASE(Modbus_Data_test, ReadRegisteredInputRegister);
    RUN_TEST_CASE(Modbus_Data_test, ReadUnregisteredInputRegister);

    RUN_TEST_CASE(Modbus_Data_test, RegisterHoldingRegisterData);
    RUN_TEST_CASE(Modbus_Data_test, ReadRegisteredHoldingRegister);
    RUN_TEST_CASE(Modbus_Data_test, ReadUnregisteredHoldingRegister);
    RUN_TEST_CASE(Modbus_Data_test, WriteRegisteredHoldingRegister);
    RUN_TEST_CASE(Modbus_Data_test, WriteUnregisteredHoldingRegister);

    // RUN_TEST_CASE(Modbus_Data_test, );
}
