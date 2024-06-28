#include "unity/fixture/unity_fixture.h"
#include "modbus_data.h"

TEST_GROUP_RUNNER(Slave_PDU_resp)
{
/* Test cases to run */
// Slave Read Slave_Coils request tests
RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenRtuMsgPassAsNullPtrArgumentToParse);
RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenReqDataBufferPassAsNullPtrArgumentToParse);
RUN_TEST_CASE(Slave_PDU_resp, SlaveParseMsgWhenRespDataBufferPassAsNullPtrArgumentToParse);
#if 5 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead5Coils);
#endif
#if 8 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead8Coils);
#endif
#if 9 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead9Coils);
#endif
#if 16 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead16Coils);
#endif
#if 17 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead17Coils);
#endif
// Slave Read Discrete Inputs tests
#if 5 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead5DiscreteInputs);
#endif
#if 8 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead8DiscreteInputs);
#endif
#if 9 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead9DiscreteInputs);
#endif
#if 16 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead16DiscreteInputs);
#endif
#if 17 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead17DiscreteInputs);
#endif
// Slave Read Holdeing Registers tests
#if 3 <= HOLDING_REG_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveReadOneHoldingRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead3HoldingRegisters);
#endif
// Slave Read Input Registers tests
#if 3 <= INPUT_REG_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveReadOneInputRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveRead3InputRegisters);
#endif
    // Slave Write Slave_Coils tests
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleCoil);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleCoilToOnAndCheckCoilStatus);
#if 5 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils5Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils5CoilsAndCheckCoilsStatus);
#endif
#if 8 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils8Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils8CoilsAndCheckCoilsStatus);
#endif
#if 9 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils9Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils9CoilsAndCheckCoilsStatus);
#endif
#if 16 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils16Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils16CoilsAndCheckCoilsStatus);
#endif
#if 17 <= COILS_QTY
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils17Coils);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleCoils17CoilsAndCheckCoilsStatus);
#endif
    // Slave Write Register tests (Holding Registers are R/w)
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleRegister);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteSingleRegisterAndCheckRegisterValue);

    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleRegister3Reg);
    RUN_TEST_CASE(Slave_PDU_resp, SlaveWriteMultipleRegister3regAndCheckRegValue);

    // RUN_TEST_CASE(Slave_PDU_resp, );
}
