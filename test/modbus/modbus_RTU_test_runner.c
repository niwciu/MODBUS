#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_RTU_test)
{
    /* Test cases to run */
    RUN_TEST_CASE(Modbus_RTU_test, AddSlaveIdToSendBuf);
    RUN_TEST_CASE(Modbus_RTU_test, AddCRCToSendBuf);
    RUN_TEST_CASE(Modbus_RTU_test, ModbusRtuSendWitnNullPtrAsBuffer);
    RUN_TEST_CASE(Modbus_RTU_test, ModbusRtuSendWitnBufLenEqualToMaxPduLen);
    RUN_TEST_CASE(Modbus_RTU_test, ModbusRtuSendWitnBufLenEqualToMaxPduLenPlus1);
    RUN_TEST_CASE(Modbus_RTU_test, SlaveIdIsCorrectInRecvBuffer);
    RUN_TEST_CASE(Modbus_RTU_test, SlaveIdIsNotCorrectInRecvBuffer);
    RUN_TEST_CASE(Modbus_RTU_test, CrcIsCorrectInRecivedBuffer);
    RUN_TEST_CASE(Modbus_RTU_test, CrcIsIncorrectInRecivedBuffer);
    // RUN_TEST_CASE(Modbus_RTU_test, );
    // RUN_TEST_CASE(Modbus_RTU_test, );
    // RUN_TEST_CASE(Modbus_RTU_test, );
}
