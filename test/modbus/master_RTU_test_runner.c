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

   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);
   
   RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
   // RUN_TEST_CASE(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

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
