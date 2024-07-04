#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_slave_PDU.h"
#include "modbus_slave.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_queue.h"

#include "mock_master_driver_interface.h"
// #include "mock_master_app_data.h"
#include "mock_slave_app_data.h"
#include <memory.h>

TEST_GROUP(Master_RTU_test);

#define mock_slave_coil_TABLE_SIZE (MASTER_COILS_TABLE_SIZE)

extern modbus_queue_t *tx_rx_q;
extern modbus_queue_t *free_q;
extern modbus_msg_t *msg_buf;

extern modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG;
extern modbus_master_state_t modbus_master_manager_state_machine;
extern modbus_timer_t modbus_master_resp_timeout;
extern uint8_t modbus_master_msg_repeat_couter;

extern modbus_status_flag_t MODBUS_MASTER_FRAME_ERROR_FLAG;

static void reset_all_RTU_buffers(void);
static void generate_resp_using_slave_lib(modbus_device_ID_t Slave_ID);
static void generate_send_req_sequence(void);
static void generate_msg_T_1_5_char_brake_sequence(void);
static void generate_msg_T_3_5_char_brake_sequence(void);

modbus_coil_disin_t debug_readed_coil_disin[2] = {!!COIL_OFF};

TEST_SETUP(Master_RTU_test)
{
    /* Init before every test */
    modbus_master_init(RTU, 9600, ODD);
    reset_all_RTU_buffers();
    for (uint8_t i = 0; i < MASTER_COILS_TABLE_SIZE; i++)
    {
        // _coils_table(i, &mock_master_coils[i]);
    }
    for (uint8_t i = 0; i < mock_slave_coil_TABLE_SIZE; i++)
    {
        register_app_data_to_modbus_slave_coils_table(i, &mock_slave_coil[i]);
    }
}

TEST_TEAR_DOWN(Master_RTU_test)
{
    /* Cleanup after every test */
}

// //  Modbus Master Manager tests
// //  IDLE state tests
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxStatusIsEqualToUsartSending)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(USART_SENDING_DATA, master_USART_Tx_status);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxBufPtrIsEqualToMsgPtr)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(msg_buf->req.data, mock_master_tx_buf_ptr);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartRxAndTxIrqEnabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_master_USART.Rx_IRQ);
    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_master_USART.Tx_IRQ);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenGivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterRxBufferPtrSetToMsgRespBufData)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(&msg_buf->resp, mock_master_rx_msg_ptr);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenSetMODBUS_MASTER_REQ_TRANSMITION_FLAG)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_SET, MODBUS_MASTER_REQ_TRANSMITION_FLAG);
}

//  MODBUS_MASTER_TRANSMITTING_REQ state tests,
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();

    mock_USART_req_msg_sended_EVENT();
    TEST_ASSERT_EQUAL(IRQ_DISABLED, mock_master_USART.Tx_IRQ);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();

    mock_USART_req_msg_sended_EVENT();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_REQ_TRANSMITION_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenResponseTimeoutIsEnabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();
    check_modbus_master_manager();
    check_modbus_master_manager();

    mock_USART_req_msg_sended_EVENT();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_TIME_OUT_MS, modbus_master_resp_timeout);
}

//  MODBUS_MASTER_RESP_WAITING state tests
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[2]={!!COIL_OFF};

    mock_slave_coil[coil_adr] = !!COIL_ON;
    mock_slave_coil[coil_adr+1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    TEST_ASSERT_EQUAL(!!COIL_OFF, readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(!!COIL_OFF, readed_coil_disin[1]);

    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();

    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenResponsTimeOutTimerDisabled)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();

    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    mock_USART_frame_error_EVENT();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(1, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterLessThanMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    mock_USART_frame_error_EVENT();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REPEAT_REQUEST, modbus_master_manager_state_machine);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_FRAME_ERROR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualToMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndFrameErrorFlagStatusToCleared)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    for (uint8_t i = 0; i < MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR; i++)
    {
        generate_send_req_sequence();
        generate_resp_using_slave_lib(slave_ID);
        generate_msg_T_1_5_char_brake_sequence();
        mock_USART_frame_error_EVENT();
        generate_msg_T_3_5_char_brake_sequence();
    }
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REPEAT_REQUEST, modbus_master_manager_state_machine);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_FRAME_ERROR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    mock_USART_frame_error_EVENT();
    generate_msg_T_3_5_char_brake_sequence();
    // repeat same request;
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    mock_USART_frame_error_EVENT();
    generate_msg_T_3_5_char_brake_sequence();
    // repeat same request;
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout);
    TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessedAndResponsTimeOutTimerDisabledAndRepeatCounterEqual0)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    for (uint8_t i=0; i< MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR; i++)
    {
        generate_send_req_sequence();
        generate_resp_using_slave_lib(slave_ID);
        generate_msg_T_1_5_char_brake_sequence();
        mock_USART_frame_error_EVENT();
        generate_msg_T_3_5_char_brake_sequence();
    }
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout);
    TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
}

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }
static void reset_all_RTU_buffers(void)
{
    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_msg_t *msg;
        msg = modbus_queue_pop(free_q);
        if (NULL != msg)
        {
            memset(msg->req.data, 0, MODBUS_RTU_BUFFER_SIZE);
            memset(msg->resp.data, 0, MODBUS_RTU_BUFFER_SIZE);
            modbus_queue_push(free_q, msg);
        }
    }
}
static void generate_resp_using_slave_lib(modbus_device_ID_t Slave_ID)
{
    parse_master_request_and_prepare_resp(msg_buf);
    modbus_RTU_send(msg_buf->resp.data, &msg_buf->resp.len, Slave_ID);
    check_modbus_master_manager();
    check_modbus_master_manager();
}
static void generate_send_req_sequence(void)
{
    check_modbus_master_manager();
    check_modbus_master_manager();
    mock_USART_req_msg_sended_EVENT();
    check_modbus_master_manager();
    check_modbus_master_manager();
}

static void generate_msg_T_1_5_char_brake_sequence(void)
{
    mock_USART_T_1_5_timeout_EVENT();
    check_modbus_master_manager();
    check_modbus_master_manager();
}

static void generate_msg_T_3_5_char_brake_sequence(void)
{
    mock_USART_T_3_5_timeout_EVENT();
    check_modbus_master_manager();
}
