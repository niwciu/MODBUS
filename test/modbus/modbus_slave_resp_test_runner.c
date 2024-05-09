#include "unity/fixture/unity_fixture.h"
#include "modbus_data.h"

TEST_GROUP_RUNNER(Modbus_Slave_Resp)
{
/* Test cases to run */
// Slave Read Coils request tests
#if 5 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5Coils);
#endif
#if 8 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8Coils);
#endif
#if 9 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9Coils);
#endif
#if 16 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16Coils);
#endif
#if 17 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17Coils);
#endif
// Slave Read Discrete Inputs tests
#if 5 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5DiscreteInputs);
#endif
#if 8 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8DiscreteInputs);
#endif
#if 9 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9DiscreteInputs);
#endif
#if 16 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16DiscreteInputs);
#endif
#if 17 <= DISCRET_INPUT_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17DiscreteInputs);
#endif
// Slave Read Holdeing Registers tests
#if 3 <= HOLDING_REG_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveReadOneHoldingRegister);
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead3HoldingRegisters);
#endif
// Slave Read Input Registers tests
#if 3 <= INPUT_REG_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveReadOneInputRegister);
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead3InputRegisters);
#endif
    // Slave Write Coils tests
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteSingleCoil);
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteSingleCoilToOnAndCheckCoilStatus);
#if 5 <= COILS_QTY
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils5Coils);
    RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils5CoilsAndCheckCoilsStatus);
#endif
    // #if 8 <= COILS_QTY
    // RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils8Coils);
    // #endif
    // #if 9 <= COILS_QTY
    // RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils9Coils);
    // #endif
    // #if 16 <= COILS_QTY
    // RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils16Coils);
    // #endif
    // #if 17 <= COILS_QTY
    // RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteMultipleCoils17Coils);
    // #endif
    // Slave Write Register tests (Holding Registers are R/w)
    // RUN_TEST_CASE(Modbus_Slave_Resp, SlaveWriteSingleRegister);

    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
    // RUN_TEST_CASE(Modbus_Slave_Resp, );
}
