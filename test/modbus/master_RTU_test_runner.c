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
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenResponseTimeoutIsEnabled);

    //  MODBUS_MASTER_RESP_WAITING state tests
    //  resp msg recived correct
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenResponsTimeOutTimerDisabled);
    // resp msg with frame error
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter);

    if (1 <= MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR)
    {
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterLessThanMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualToMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedRespProcessedAndNoExceptionCodeRecivedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedRespProcessedAndNoExceptionCodeRecivedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState);
        // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedRespProcessedAndExceptionCodeRecivedThen.. co tutaj robię z msg buforem? raportuje i wypycham czy zostawiam do obróbki?);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessedAndResponsTimeOutTimerDisabledAndRepeatCounterEqual0);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamThenReportError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamAndErrorReportedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThenRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState);
        // Curr_Msg_error_catched jako tymczasowa struktura do której raportuje błędy a następnie tą strukturę przepisuję do raportu błędu na końcu
    }
    else
    {
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
