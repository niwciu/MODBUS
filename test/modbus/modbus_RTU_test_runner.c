#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_RTU)
{
   /* Test cases to run */
   RUN_TEST_CASE(Modbus_RTU, AddSlaveIdToSendBuf);
   RUN_TEST_CASE(Modbus_RTU, AddCRCToSendBuf);
   RUN_TEST_CASE(Modbus_RTU, ModbusRtuSendWitnNullPtrAsBuffer);
   RUN_TEST_CASE(Modbus_RTU, ModbusRtuSendWitnBufLenEqualToMaxPduLen);
   RUN_TEST_CASE(Modbus_RTU, ModbusRtuSendWitnBufLenEqualToMaxPduLenPlus1);
   RUN_TEST_CASE(Modbus_RTU, SlaveIdIsCorrectInRecvBuffer);
   // RUN_TEST_CASE(Modbus_RTU, );
}
