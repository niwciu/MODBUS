
#include "unity/fixture/unity_fixture.h"
TEST_GROUP_RUNNER(Slave_RTU_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithIncorectSlaveIdRecivedWhenTimer1_5CharTrigerThenModbusManagerGoBackToIdleStateWithEmptyMsgReqBuffer);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdRecivedWhenTimer1_5CharTrigerThenStartSilenceTimerAndGoToSilenceBrakeState);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndReciverTimer1_5CharTrigerWhenRxCharBeforeT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState);
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
}