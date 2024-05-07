#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Slave_Resp)
{
   /* Test cases to run */

   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5Coils);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8Coils);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9Coils);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16Coils);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17Coils);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead5DiscreteInputs);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead8DiscreteInputs);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead9DiscreteInputs);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead16DiscreteInputs);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveRead17DiscreteInputs);
   RUN_TEST_CASE(Modbus_Slave_Resp, SlaveReadOneHoldingRegister);
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
   // RUN_TEST_CASE(Modbus_Slave_Resp, );

   
}
