#include "unity/fixture/unity_fixture.h"
#include "modbus_slave_data.h"

TEST_GROUP_RUNNER(Slave_PDU_resp)
{
    /* Test cases to run */
    // Slave Read Slave_Coils request tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenRtuMsgPassAsNullPtrArgumentToParse);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenReqDataBufferPassAsNullPtrArgumentToParse);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenRespDataBufferPassAsNullPtrArgumentToParse);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead5Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead8Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead9Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead16Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead17Coils);
// Slave Read Discrete Inputs tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead5DiscreteInputs);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead8DiscreteInputs);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead9DiscreteInputs);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead16DiscreteInputs);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead17DiscreteInputs);
// Slave Read Holdeing Registers tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveReadOneHoldingRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead3HoldingRegisters);
// Slave Read Input Registers tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveReadOneInputRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead3InputRegisters);
// Slave Write Slave_Coils tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleCoil);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleCoilToOnAndCheckCoilStatus);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils5Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils5CoilsAndCheckCoilsStatus);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils8Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils8CoilsAndCheckCoilsStatus);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils9Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils9CoilsAndCheckCoilsStatus);

    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils16Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils16CoilsAndCheckCoilsStatus);

    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils17Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils17CoilsAndCheckCoilsStatus);
// Slave Write Register tests (Holding Registers are R/w)
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleRegisterAndCheckRegisterValue);

    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleRegister3Reg);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleRegister3regAndCheckRegValue);

    // RUN_TEST_CASE(Slave_PDU_resp, );
}
