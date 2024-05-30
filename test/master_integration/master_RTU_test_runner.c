#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_RTU)
{
   /* Test cases to run */
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr);

   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);
   // RUN_TEST_CASE(master_RTU, );
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhen);

}
