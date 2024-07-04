#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Master_PDU_read_exception_code)
{
    /* Test cases to run */
    RUN_TEST_CASE(Master_PDU_read_exception_code, WhenRegisterModbusErrorCbCalledThenModbusErrorCbEqualToRegisteredFunctionPointer)
    RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadDisInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadDisInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadDisInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadDisInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadInRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadInRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadInRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadInRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadHoldingRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadHoldingRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadHoldingRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterReadHoldingRegInReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleCoilReqSendedAndModbusErrorCbRegisteredWhenExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleRegReqSendedAndExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleRegReqSendedAndExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleRegReqSendedAndExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteSingleRegReqSendedAndExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleCoilsReqSendedAndExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleCoilsReqSendedAndExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleCoilsReqSendedAndExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleCoilsReqSendedAndExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleRegReqSendedAndExceptionCode01RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleRegReqSendedAndExceptionCode02RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleRegReqSendedAndExceptionCode03RecivedThen);
    // RUN_TEST_CASE(Master_PDU_read_exception_code, GivenMasterWriteMultipleRegReqSendedAndExceptionCode03RecivedThen);

    // RUN_TEST_CASE(Master_PDU_read_exception_code, );
}
