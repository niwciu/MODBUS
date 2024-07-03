#include "unity/fixture/unity_fixture.h"
#include "modbus_config.h"

TEST_GROUP_RUNNER(master_RTU_test)
{
    /* Test cases to run */
    // Master Request Generation and placing to queue tests
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr);

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
    // // IDLE state tests
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxStatusIsEqualToUsartSendingAndMasterUsartTxBufPtrIsEqualToMsgPtr);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartRxAndTxIrqEnabled);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenGivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterRxBufferPtrSetToMsgRespBufData);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenSetMODBUS_MASTER_REQ_TRANSMITION_FLAG);
    // // RUN_TEST_CASE(master_RTU_test, );

    // // MODBUS_MASTER_TRANSMITTING_REQ state tests,
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabledAndMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenResponseTimeoutIsEnabled);
    
    // //  MODBUS_MASTER_RESP_WAITING state tests
    //     //  resp msg recived correct
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenResponsTimeOutTimerDisabled);
    //     // resp msg with frame error
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter);
    // // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedThenErrorLogStructUpdated);
    // if (1 <= MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR)
    // {
    //     RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualOrLessForMsgRepeatOnErrorParamThenAfterT3_5CharSetRepeatMsgRequestStateAndFrameErrorFlagCleared);
    //     RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThenRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed);
    //     RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThenRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0);
    //     RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeThenRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessedAndResponsTimeOutTimerDisabledAndRepeatCounterEqual0);
        
        
        
        
        
        // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThenRepeatOnErrorParamThenReportError);
        // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThenRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState);
        // Curr_Msg_error_catched jako tymczasowa struktura do której raportuje błędy a następnie tą strukturę przepisuję do raportu błędu na końcu
    // }
    // else
    // {

    // }

    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatcheFirstTimeAndRepeatRequestTransmisionWhen SecondFrameErrorCatchedAfterT3_5CharIncMsgRepeatCounterAndSetRepeatMsgRequestState);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedThenAfterT3_5CharIncMsgRepeatCounterAndSetRepeatMsgRequestState);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedSecondTimeThenFrameErrorCatchAndRepeatRequestAfter3_5Char);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedThirdTimeThenFrameErrorReport);

    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedWhenTimeOutTimerExpiredThenTimeOutErrorReported);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedTwiceWhenTimeOutTimerExpiredThenTimeOutErrorReported);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedWhenTimeOutTimerNotExpiredAndCorrectSlaveIdRecivedThenRespProcessed);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedTwiceWhenTimeOutTimerNotExpiredAndCorrectSlaveIdRecivedThenRespProcessed);
    // złe slave ID złe CRC złe slave id złe CRC -> czy złe złe CRC powinno się kasować? nie powtórki kasuje tylko poprawnie odebrana dana
    // test na powtórzenie jakichkolwiek błędów jednego po drugim 3 razy
    // test na zapis wystąpionych błędów po 3 krotnym wystąpieniujakiegokolwiek
    //
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);

    // test na to czy manager wysle jak head bedzie rowny tail ale beda dostepne dane do wyslania
}
