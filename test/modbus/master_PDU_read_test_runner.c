#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Master_PDU_read)
{
    /* Test cases to run */
    // master read write respond parse with null ptr passed tests
    RUN_TEST_CASE(Master_PDU_read, MasterReadCoilsRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterReadDiscreteInputsRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterReadInputRegRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterReadHoldingRegRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterWriteSingleCoilsRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterWriteSingleRegRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterWriteMultipleCoilsRespWithNullPtrPassedAsArgument);
    RUN_TEST_CASE(Master_PDU_read, MasterWriteMultipleRegRespWithNullPtrPassedAsArgument);

    // Poniżej wpoinny być testy na read in sigle byte read many byte ToDo
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue);
    // ToDo many bytes data tests
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue);
    // ToDo many bytes data tests
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveReadInputRegisterResponsRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue);
    // ToDo many bytes data tests
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveReadHoldingRegisterResponsRecivedWhenMasterReadSlaveRespondThenMasterHolRegUpdateToSlaveHolRegValue);
    // ToDo many bytes data tests

    // // Master Read Write funcitons respons test
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveWriteSingleCoilResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectOutputAdrAndIncorrectCoilValueWhenMasterWriteSignleCoilThenMasterWriteSingleCoilRetValueError);
    RUN_TEST_CASE(Master_PDU_read, GivenSlaveWriteSingleReglResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsIncorrectThenMasterWriteSingleRegisterRespRetErrorOutAddr);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectRegValueWhenMasterWriteSignleRegThenMasterWriteSingleRegRetValueError);

    RUN_TEST_CASE(Master_PDU_read, GivenSlaveWriteMultipleCoilResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetErrorFuncCode);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutAdrError);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutQtyError);

    RUN_TEST_CASE(Master_PDU_read, GivenSlaveWriteMultipleReglResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetErrorFuncCode);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutAdrError);
    // RUN_TEST_CASE(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutQtyError);
    
    // master recive exception code tests
    RUN_TEST_CASE(Master_PDU_read, GivenMasterReadCoilReqSendedAndWhenFunctionCodeWithExceptionCodeMaskRecivedThenModbusMasterReadSlaveReturnExceptionCodeRecivedStatus);
    RUN_TEST_CASE(Master_PDU_read, GivenMasterReadDisInReqSendedWhenFunctionCodeWithExceptionCodeMaskRecivedThenModbusMasterReadSlaveReturnExceptionCodeRecivedStatus);
    // RUN_TEST_CASE(Master_PDU_read, );
}
