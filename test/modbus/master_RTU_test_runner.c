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
    // First two commented tests related to callback actions trigered by driver ToDo set seperate section for all callback triger
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAllDriverCallbackRegisteredAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabled); 
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAllDriverCallbackRegisteredAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared); 
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenResponseTimeoutIsEnabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenModbusMasterManagerStateMachineEqualToModbusMasterRespWaiting);

    // MODBUS_MASTER_RESP_WAITING 
    // flag set tests
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredThenModbusT1_5FlagCleared);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredThenModbusT3_5FlagCleared);
    // rtu crc error tests
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenModbusCrcRtuErroFlagSet);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenModbusMasterRespRecivedStateSet);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenRespTimoutTimerDisabled);
    // rtu slave id error test
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUSlaveIdErrorCatchedThenMsgBufRespLenSetTo0);
    // no error tests
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndNoRtuErrorInRespMsgThenRespTimoutTimerDisabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndNoRtuErrorInRespMsgThenModbusMasterRespRecivedStateSet);

    //  MODBUS_MASTER_RESP_RECIVED state tests
    //  resp msg recived correctly with data to process
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenResponsTimeOutTimerDisabled);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespContainExceptionCodeThenReportExceptionCodeError);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedThenPushMsgBuferPtrToFreeQueue);
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState);
    // frame error in recived msg tests 
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter);
    // resp msg CRC RTU error
    RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCrcErrorRecivedThenAfterT3_5CharIncMsgRepeatCounter);
    if ( MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR >=1 )
    {
        //frame eeror repetition tests
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
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState);

        // rtu crc repetition tets      
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterLessThanMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndRtuCrcErrorFlagStatusToCleared);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualToMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndRtuCrcErrorFlagStatusToCleared);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed); 
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenResponsTimeOutTimerDisabled);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenRepeatCounterEqual0);

        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamThenReportError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamAndErrorReportedThenPushMsgBuferPtrToFreeQueue);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamThenModbusMsgCouterEqual);
    }
    else
    {
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharReportError);
        RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharGoToIdleState);

    }

    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedWhenTimeOutTimerExpiredThenTimeOutErrorReported); -> to ok
    // RUN_TEST_CASE(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRespWithWrongSlaveIdRecivedTwiceWhenTimeOutTimerExpiredThenTimeOutErrorReported); -> to niepotrzebne
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
