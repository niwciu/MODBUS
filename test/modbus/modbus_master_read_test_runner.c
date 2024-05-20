#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Master_Read)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterReadCoilReturnErroFuncCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterReadDiscreteInputsReturnErroFuncCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterReadInputRegistersReturnErroFuncCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterInputRegistersStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterReadInputRegistersReturnErrorByteCntCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersThenMasterHoldingRegistersUpdateToHoldingRegistersValue);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersReturnErroFuncCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterHoldingRegistersStayUnchanged);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterReadHoldingRegistersReturnErrorByteCntCode);
   // Master Read Write funcitons respons test
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleCoilRespAndOutputAddressIsCorrectThenMasterWriteSingleCoilRespRetOk);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode);
   RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsCorrectThenMasterWriteSingleRegisterRespRetOk);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleRegisterRespThenMasterWriteSingleRegisterReturnErrorOutputAddress);
   // RUN_TEST_CASE(Modbus_Master_Read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode);
   // RUN_TEST_CASE(Modbus_Master_Read, );
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
