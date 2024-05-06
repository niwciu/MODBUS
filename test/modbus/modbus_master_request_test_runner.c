#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Master_Requests)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_Master_Requests, ReadSingleHoldingRegisterRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadSingleInputRegisterRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadDiscreteInputsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadMaxQtyDiscreteInputsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteSingleRegister);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteMultipleRegisters);
   // 
   // 
   // RUN_TEST_CASE(Modbus_Master_Requests, );
   // RUN_TEST_CASE(Modbus_Master_Requests, );
   // RUN_TEST_CASE(Modbus_Master_Requests, );

   
}
