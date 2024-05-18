#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Master_Read)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputstayUnchanged);
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );

   
}
