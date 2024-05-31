#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Master_PDU_read)
{
    /* Test cases to run */

    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsUpdateToSlaveCoilsValue);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterCoilsStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadCoilRespThenMasterReadCoilReturnErroFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDisInUpdateToDisInValue);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterDiscreteInputsStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadDiscreteInputsRespThenMasterReadDiscreteInputsReturnErroFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersUpdateToInputRegistersValue);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterInputRegistersStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadInputRegistersThenMasterReadInputRegistersReturnErroFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterInputRegistersStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadInputRegistersAndByteCountIsIncorrectThenMasterReadInputRegistersReturnErrorByteCntCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersThenMasterHoldingRegistersUpdateToHoldingRegistersValue);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterReadHoldingRegistersThenMasterReadHoldingRegistersReturnErroFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterHoldingRegistersStayUnchanged);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterReadHoldingRegistersAndByteCountIsIncorrectThenMasterReadHoldingRegistersReturnErrorByteCntCode);
    // Master Read Write funcitons respons test
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleCoilRespAndOutputAddressIsCorrectThenMasterWriteSingleCoilRespRetOk);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectcoilValueWhenMasterWriteSignleCoilThenMasterWriteSingleCoilRetValueError);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsCorrectThenMasterWriteSingleRegisterRespRetOk);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsIncorrectThenMasterWriteSingleRegisterRespRetErrorOutAddr);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectRegValueWhenMasterWriteSignleRegThenMasterWriteSingleRegRetValueError);

    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOk);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetErrorFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutAdrError);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutQtyError);

    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOk);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetErrorFuncCode);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutAdrError);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutQtyError);

    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterRead); // tego nie odpalaj tak było do zdabania po refactoringu
    // RUN_TEST_CASE(Master_PDU_read, );
    // RUN_TEST_CASE(Master_PDU_read, );
    // RUN_TEST_CASE(Master_PDU_read, );
    // RUN_TEST_CASE(Master_PDU_read, );
    // RUN_TEST_CASE(Master_PDU_read, );
}