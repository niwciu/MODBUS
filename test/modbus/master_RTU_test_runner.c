#include "unity/fixture/unity_fixture.h"
#include "modbus_config.h"

TEST_GROUP_RUNNER(Master_RTU_test)
{
    /* Test cases to run */

    // Modbus Master Manager tests
    RUN_TEST_CASE(Master_RTU_test, WhenTestFunctionRegisterdAsModbusErrorCallbackThenModbusErrorCallbackPtrEqualToTestFunctionAdres);
    // MODBUS_MASTER_IDLE state tests
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxStatusIsEqualToUsartSending);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxBufPtrIsEqualToMsgPtr);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartRxAndTxIrqEnabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenGivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterRxBufferPtrSetToMsgRespBufData);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenSetMODBUS_MASTER_REQ_TRANSMITION_FLAG);

    // MODBUS_MASTER_TRANSMITTING_REQ state tests,
    // ToDo - >decide what to do with two tests bellow that looks like not form this layer
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabled); // Not sure if It should be here whiel it's driver responsibility
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared); // Not sure if It should be here whiel it's driver responsibility
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenResponseTimeoutIsEnabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenModbusMasterManagerStateMachineEqualToModbusMasterRespWaiting);
    // MODBUS_MASTER_RESP_RECIVED
    // zerowanie flag
    // wystapienie bledu crc to...
    // wystapienie  RET_ERROR_SLAVE_ID bledu slave id przy parsowaniu to smg_buf len ==0
    // wystopienie RET_OK przy parsowaniu RTU to wylacz resp timer i idz do nastepnego stani
    //  MODBUS_MASTER_RESP_WAITING state tests
    //  resp msg recived correct
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenResponsTimeOutTimerDisabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespContainExceptionCodeThenReportExceptionCodeError);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedThenPushMsgBuferPtrToFreeQueue);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState);
    // resp msg with frame error
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter);
    // resp msg CRC RTU error
    if ( MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR >=1 )
    {
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterLessThanMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualToMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState); 
        
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenResponsTimeOutTimerDisabled);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenRepeatCounterEqual0);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamThenReportError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamAndErrorReportedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThenRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState);
    }
    else
    {
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharReportError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharGoToIdleState);

    }

    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatcheFirstTimeAndRepeatRequestTransmisionWhen SecondFrameErrorCatchedAfterT3_5CharIncMsgRepeatCounterAndSetRepeatMsgRequestState);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedThenAfterT3_5CharIncMsgRepeatCounterAndSetRepeatMsgRequestState);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedSecondTimeThenFrameErrorCatchAndRepeatRequestAfter3_5Char);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charNotExpiredAndCharRecivedThirdTimeThenFrameErrorReport);

    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedWhenTimeOutTimerExpiredThenTimeOutErrorReported);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedTwiceWhenTimeOutTimerExpiredThenTimeOutErrorReported);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedWhenTimeOutTimerNotExpiredAndCorrectSlaveIdRecivedThenRespProcessed);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedTwiceWhenTimeOutTimerNotExpiredAndCorrectSlaveIdRecivedThenRespProcessed);
    // złe slave ID złe CRC złe slave id złe CRC -> czy złe złe CRC powinno się kasować? nie powtórki kasuje tylko poprawnie odebrana dana
    // test na powtórzenie jakichkolwiek błędów jednego po drugim 3 razy
    // test na zapis wystąpionych błędów po 3 krotnym wystąpieniujakiegokolwiek
    // test na wielokrotne użycie już wykorzystaneo bufora -> odpowiedź czy trzeba go zerować przed zwolnieniem
    //
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhen);

    // ToDo Modbus RTU manager error report generation and error callbcak calling
    // test na to czy manager wysle jak head bedzie rowny tail ale beda dostepne dane do wyslania
}
