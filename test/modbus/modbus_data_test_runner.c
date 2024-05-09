#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Data)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Data, RegisterCoilData);
   RUN_TEST_CASE(Modbus_Data, ReadRegisteredCoil);
   RUN_TEST_CASE(Modbus_Data, ReadUnregisteredCoil);

   RUN_TEST_CASE(Modbus_Data, RegisterDiscreteInputData);
   // RUN_TEST_CASE(Modbus_Data, ReadRegisteredDiscreteInput);
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );
   // RUN_TEST_CASE(Modbus_Data, );

   
}
