#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Data)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Data, RegisterCoilData);
   RUN_TEST_CASE(Modbus_Data, ReadRegisteredCoil);
   RUN_TEST_CASE(Modbus_Data, ReadUnregisteredCoil);
   RUN_TEST_CASE(Modbus_Data, WriteRegisteredCoil);
   RUN_TEST_CASE(Modbus_Data, WriteUnregisteredCoil);

   RUN_TEST_CASE(Modbus_Data, RegisterDiscreteInputData);
   RUN_TEST_CASE(Modbus_Data, ReadRegisteredDiscreteInput);
   RUN_TEST_CASE(Modbus_Data, ReadUnegisteredDiscreteInput);

   RUN_TEST_CASE(Modbus_Data, RegisterInputRegisterData);
   RUN_TEST_CASE(Modbus_Data, ReadRegisteredInputRegister);
   RUN_TEST_CASE(Modbus_Data, ReadUnregisteredInputRegister);

   RUN_TEST_CASE(Modbus_Data, RegisterHoldingRegisterData);
   RUN_TEST_CASE(Modbus_Data, ReadRegisteredHoldingRegister);
   RUN_TEST_CASE(Modbus_Data, ReadUnregisteredHoldingRegister);
   RUN_TEST_CASE(Modbus_Data, WriteRegisteredHoldingRegister);
   RUN_TEST_CASE(Modbus_Data, WriteUnregisteredHoldingRegister);
   
   
   // RUN_TEST_CASE(Modbus_Data, );

   
}
