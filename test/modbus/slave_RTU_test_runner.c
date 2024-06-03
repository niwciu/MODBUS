
#include "unity/fixture/unity_fixture.h"
TEST_GROUP_RUNNER(Slave_RTU_test)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqRecivedWhenTimer1_5CharTrigerThenModbusManagerGoToModbusSlaveMsgRecived);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithIncorectSlaveIdRecivedWhenTimer1_5CharTrigerThenModbusManagerGoBackToIdleStateWithEmptyMsgReqBuffer);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedWhenTimer1_5CharTrigerThenGoToModbusSlaveReciverSilancePendingState);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcAndReciverTimer1_5CharTrigerWhenRxCharBeforeT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndIncorrectCrcRecivedAndReciverTimer1_5CharTrigerWhenT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState);
//    
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedAndTimer1_5CharTrigerAndTimer3_5CharTrigerThenSlavePrepareRespond);
   RUN_TEST_CASE(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedAndTimer1_5CharTrigerAndTimer3_5CharTrigerThenSlaveSendRespAndGoToModbuSlaveTransmitingResp);
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
//    RUN_TEST_CASE(Slave_RTU_test, );
}