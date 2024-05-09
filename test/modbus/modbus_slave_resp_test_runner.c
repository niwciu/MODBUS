#include "unity/fixture/unity_fixture.h"
#include "modbus_data.h"

TEST_GROUP_RUNNER(Modbus_Slave_Resp)
{
   /* Test cases to run */
   #if COILS_QTY> 5
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5Coils);
   #endif
   #if COILS_QTY> 8
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8Coils);
   #endif
   #if COILS_QTY> 9
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9Coils);
   #endif
   #if COILS_QTY> 16
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16Coils);
   #endif
   #if COILS_QTY> 17
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17Coils);
   #endif
   #if DISCRET_INPUT_QTY> 5
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5DiscreteInputs);
   #endif
   #if DISCRET_INPUT_QTY> 8
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8DiscreteInputs);
   #endif
   #if DISCRET_INPUT_QTY> 9
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9DiscreteInputs);
   #endif
   #if DISCRET_INPUT_QTY> 16
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16DiscreteInputs);
   #endif
   #if DISCRET_INPUT_QTY> 17
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17DiscreteInputs);
   #endif
   #if HOLDING_REG_QTY> 3
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveReadOneHoldingRegister);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead3HoldingRegisters);
   #endif
   #if INPUT_REG_QTY> 3
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveReadOneInputRegister);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead3InputRegisters);
   #endif
   // RUN_TEST_CASE(Modbus_Slave_Resp, );
   // RUN_TEST_CASE(Modbus_Slave_Resp, );
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
