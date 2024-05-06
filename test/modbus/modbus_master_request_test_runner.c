#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Master_Requests)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_Master_Requests, ReadSingleHoldingRegisterRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadMaxQtyHoldingRegisterRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadHoldingRegisterMaxQtyPlus1Request);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadSingleInputRegisterRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadMaxQtyInputRegisterRequest);
   // RUN_TEST_CASE(Modbus_Master_Requests, ReadInputRegisterMaxQtyPlus1Request);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadDiscreteInputsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadMaxQtyDiscreteInputsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadMaxQtPlus1DiscreteInputsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadCoilsRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadCoilsMaxQtyRequest);
   RUN_TEST_CASE(Modbus_Master_Requests, ReadCoilsMaxQtyPlus1Request);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteSingleRegister);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteSingleCoilOn);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteMultipleRegisters);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteMaxQtyMultipleRegisters);
   RUN_TEST_CASE(Modbus_Master_Requests, WriteMultipleRegistersMaxQtyPlus1); 

   // 
   // RUN_TEST_CASE(Modbus_Master_Requests, );
   // RUN_TEST_CASE(Modbus_Master_Requests, );
   // RUN_TEST_CASE(Modbus_Master_Requests, );
   // RUN_TEST_CASE(Modbus_Master_Requests, );

   
}
