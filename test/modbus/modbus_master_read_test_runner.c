#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Master_Read)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead);
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );
   // RUN_TEST_CASE(Modbus_Master_Read, );

   
}
