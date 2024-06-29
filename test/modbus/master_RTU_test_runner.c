#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(master_RTU_test)
{
    /* Test cases to run */
    // Master Request Generation and placing to queue tests
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError);

    // RUN_TEST_CASE(master_RTU_test, );

    // Modbus Master Manager tests
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxStatusIsEqualToUsartSendingAndMasterUsartTxBufPtrIsEqualToMsgPtr);
    RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartRxAndTxIrqEnabled);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterManagerStateMachineSetToModbusMasterTransmittingReq);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Set);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhen);

    // test na to czy manager wysle jak head bedzie rowny tail ale beda dostepne dane do wyslania
}
