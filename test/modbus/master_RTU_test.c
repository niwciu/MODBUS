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
extern modbus_master_error_cb_t modbus_error_callback;

extern modbus_master_state_t modbus_master_manager_state_machine;
extern modbus_timer_t modbus_master_resp_timeout_timer;
extern uint8_t modbus_master_msg_repeat_couter;

extern modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG;
extern modbus_status_flag_t MODBUS_MASTER_FRAME_ERROR_FLAG;
extern modbus_status_flag_t MODBUS_MASTER_TIMER_1_5_CHAR_FLAG;
extern modbus_status_flag_t MODBUS_MASTER_TIMER_3_5_CHAR_FLAG;
extern modbus_status_flag_t MODBUS_MASTER_RTU_CRC_ERROR_FLAG;

static modbus_error_rep_t test_error_rep;

static void reset_all_RTU_buffers(void);
static void generate_resp_using_slave_lib(modbus_device_ID_t Slave_ID);
static void generate_send_req_sequence(void);
static void generate_msg_T_1_5_char_brake_sequence(void);
static void generate_msg_T_3_5_char_brake_sequence(void);
static void generate_read_frame_error_catch_sequance(modbus_device_ID_t slave_ID, uint8_t sequence_repeat);
static void generate_send_req_read_resp_msg_sequance(modbus_device_ID_t slave_ID);
static void error_report_test_function(modbus_error_rep_t *error_rep);
static void set_new_CRC_val(modbus_req_resp_t *req_res,modbus_CRC_t new_CRC);
static void generate_read_rtu_crc_error_catch_sequance(modbus_device_ID_t slave_ID, uint8_t sequence_repeat);

modbus_coil_disin_t debug_readed_coil_disin[2] = {!!COIL_OFF};

TEST_SETUP(Master_RTU_test)
{
    /* Init before every test */
    modbus_master_init(RTU, 9600, ODD);
    reset_all_RTU_buffers();
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
TEST(Master_RTU_test, WhenTestFunctionRegisterdAsModbusErrorCallbackThenModbusErrorCallbackPtrEqualToTestFunctionAdres)
{
    register_modbus_master_error_cb(error_report_test_function);
    TEST_ASSERT_EQUAL(&error_report_test_function, modbus_error_callback);
}
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
// First two commented tests related to callback actions trigered by driver ToDo set seperate section for all callback triger
// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAllDriverCallbackRegisteredAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenTxIrqDisabled)
// {
//     modbus_adr_t coil_adr = 0x0002;
//     modbus_device_ID_t slave_ID = 0x09;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     check_modbus_master_manager();

//     mock_USART_req_msg_sended_EVENT();
//     TEST_ASSERT_EQUAL(IRQ_DISABLED, mock_master_USART.Tx_IRQ);
// }
// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAllDriverCallbackRegisteredAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenMODBUS_MASTER_REQ_TRANSMITION_FLAG_Cleared)
// {
//     modbus_adr_t coil_adr = 0x0002;
//     modbus_device_ID_t slave_ID = 0x09;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     check_modbus_master_manager();

//     mock_USART_req_msg_sended_EVENT();
//     TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_REQ_TRANSMITION_FLAG);
// }

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
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_TIME_OUT_MS, modbus_master_resp_timeout_timer);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitingWhenWhloeRequestIsTransmittedThenModbusMasterManagerStateMachineEqualToModbusMasterRespWaiting)
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
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_WAITING, modbus_master_manager_state_machine);
}

// MODBUS_MASTER_RESP_RECIVED
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredThenModbusT1_5FlagCleared)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();

    mock_USART_req_msg_sended_EVENT();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_TIMER_1_5_CHAR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredThenModbusT3_5FlagCleared)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    check_modbus_master_manager();

    mock_USART_req_msg_sended_EVENT();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_TIMER_3_5_CHAR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenModbusCrcRtuErroFlagSet)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    // change CRC in resp to 0x0000
    msg_buf->resp.data[msg_buf->resp.len - 2] = 0;
    msg_buf->resp.data[msg_buf->resp.len - 1] = 0;
    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_SET, MODBUS_MASTER_RTU_CRC_ERROR_FLAG);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenModbusMasterRespRecivedStateSet)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    set_new_CRC_val(&msg_buf->resp, 0);
    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_RECIVED, modbus_master_manager_state_machine);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUCrcErrorCatchedThenRespTimoutTimerDisabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    set_new_CRC_val(&msg_buf->resp,0);

    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndRTUSlaveIdErrorCatchedThenMsgBufRespLenSetTo0)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    msg_buf->resp.data[MODBUS_SLAVE_ADR_IDX] = slave_ID+1;

    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(0, msg_buf->resp.len);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndNoRtuErrorInRespMsgThenRespTimoutTimerDisabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);

    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmittedAndMsgRecivedWhenTimer_1_5CharExpiredAndNoRtuErrorInRespMsgThenModbusMasterRespRecivedStateSet)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);

    generate_msg_T_1_5_char_brake_sequence();
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_RECIVED, modbus_master_manager_state_machine);
}

//  MODBUS_MASTER_RESP_WAITING state tests
//  resp msg recived correct
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredThenRespProcessed)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[2] = {!!COIL_OFF};

    mock_slave_coil[coil_adr] = !!COIL_ON;
    mock_slave_coil[coil_adr + 1] = !!COIL_ON;

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
    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespContainExceptionCodeThenReportExceptionCodeError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];
    modbus_buf_t read_coil_ex_code_02_resp[] = {0x03, 0x81, 0x02, 0x60, 0x51};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_02_resp) / sizeof(modbus_buf_t);

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    generate_send_req_sequence();
    // generate fun code 02 resp RTU msg
    memcpy(msg_buf->resp.data, read_coil_ex_code_02_resp, buf_len);
    msg_buf->resp.len = buf_len;
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.resp_read_error);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedThenPushMsgBuferPtrToFreeQueue)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_read_resp_msg_sequance(slave_ID);
    TEST_ASSERT_NULL(msg_buf);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndTimer3_5charExpiredAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_send_req_read_resp_msg_sequance(slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
}

// resp msg with frame error
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharIncMsgRepeatCounter)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, 1);

    TEST_ASSERT_EQUAL(1, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedWhenRespWithCrcErrorRecivedThenAfterT3_5CharIncMsgRepeatCounter)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);

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
    generate_read_frame_error_catch_sequance(slave_ID, 1);
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
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

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
    generate_read_frame_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

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
    generate_read_frame_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
    TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x02;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];
    modbus_buf_t read_coil_ex_code_02_resp[] = {0x02, 0x81, 0x02, 0x31, 0x91};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_02_resp) / sizeof(modbus_buf_t);

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_send_req_sequence();
    generate_read_frame_error_catch_sequance(slave_ID, 1);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    // generate fun code 02 resp RTU msg
    memcpy(msg_buf->resp.data, read_coil_ex_code_02_resp, buf_len);
    msg_buf->resp.len = buf_len;
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.resp_read_error);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenPushMsgBuferPtrToFreeQueue)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_NULL(msg_buf);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];
    modbus_buf_t read_coil_ex_code_02_resp[] = {0x03, 0x81, 0x02, 0x60, 0x51};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_02_resp) / sizeof(modbus_buf_t);

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_send_req_sequence();
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    // generate fun code 02 resp RTU msg
    memcpy(msg_buf->resp.data, read_coil_ex_code_02_resp, buf_len);
    msg_buf->resp.len = buf_len;
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.resp_read_error);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenResponsTimeOutTimerDisabled)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
    generate_send_req_read_resp_msg_sequance(slave_ID);
    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
}
TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenRepeatCounterEqual0)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
    generate_send_req_read_resp_msg_sequance(slave_ID);
    TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamThenReportError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    register_modbus_master_error_cb(error_report_test_function);
    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_FRAME_ERR, test_error_rep.resp_read_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThanRepeatOnErrorParamAndErrorReportedThenPushMsgBuferPtrToFreeQueue)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    register_modbus_master_error_cb(error_report_test_function);
    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);

    TEST_ASSERT_NULL(msg_buf);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndFrameErrorCatchedMoreTimeThenRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    register_modbus_master_error_cb(error_report_test_function);
    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterLessThanMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndRtuCrcErrorFlagStatusToCleared)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REPEAT_REQUEST, modbus_master_manager_state_machine);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_RTU_CRC_ERROR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedAndMsgRepeatCounterUpdatedWhenMsgRepeatCounterEqualToMsgRepeatOnErrorParamAndT3_5CharTimeExpiredThenSetRepeatMsgRequestStateAndRtuCrcErrorFlagStatusToCleared)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

    TEST_ASSERT_EQUAL(MODBUS_MASTER_REPEAT_REQUEST, modbus_master_manager_state_machine);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED, MODBUS_MASTER_RTU_CRC_ERROR_FLAG);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
    TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenResponsTimeOutTimerDisabledAndRepeatCounterEqual0)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);

    generate_send_req_read_resp_msg_sequance(slave_ID);

    TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
    TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x04;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];
    modbus_buf_t read_coil_ex_code_02_resp[] = {0x04, 0x81, 0x02, 0xD1, 0x90};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_02_resp) / sizeof(modbus_buf_t);

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

    generate_send_req_sequence();
    generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    // generate fun code 02 resp RTU msg
    memcpy(msg_buf->resp.data, read_coil_ex_code_02_resp, buf_len);
    msg_buf->resp.len = buf_len;
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.resp_read_error);
}

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenPushMsgBuferPtrToFreeQueue)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);

//     generate_send_req_read_resp_msg_sequance(slave_ID);

//     TEST_ASSERT_NULL(msg_buf);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedLessTimeThanRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndPushMsgBuferPtrToFreeQueueThenSetModbusMasterIdleState)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, 1);

//     generate_send_req_read_resp_msg_sequance(slave_ID);

//     TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedThenRespProcessed)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
//     generate_send_req_read_resp_msg_sequance(slave_ID);

//     TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr], readed_coil_disin[0]);
//     TEST_ASSERT_EQUAL(mock_slave_coil[coil_adr + 1], readed_coil_disin[1]);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedAndExceptionCodeRecivedThenReportExceptionCodeError)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];
//     modbus_buf_t read_coil_ex_code_02_resp[] = {0x03, 0x81, 0x02, 0x60, 0x51};
//     modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_02_resp) / sizeof(modbus_buf_t);

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

//     generate_send_req_sequence();
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
//     // generate fun code 02 resp RTU msg
//     memcpy(msg_buf->resp.data, read_coil_ex_code_02_resp, buf_len);
//     msg_buf->resp.len = buf_len;
//     generate_msg_T_1_5_char_brake_sequence();
//     generate_msg_T_3_5_char_brake_sequence();

//     TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
//     TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
//     TEST_ASSERT_EQUAL(MODBUS_ILLEGAL_DATA_ADDRESS_ERROR, test_error_rep.exception_code);
//     TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
//     TEST_ASSERT_EQUAL(0, test_error_rep.resp_read_error);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenResponsTimeOutTimerDisabled)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
//     generate_send_req_read_resp_msg_sequance(slave_ID);
//     TEST_ASSERT_EQUAL(0, modbus_master_resp_timeout_timer);
// }
// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedEqualTimeAsRepeatOnErrorParamAndReqMsgRepeatedAndCorrectResponsRecivedAndRespProcessedThenRepeatCounterEqual0)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);

//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);
//     generate_send_req_read_resp_msg_sequance(slave_ID);
//     TEST_ASSERT_EQUAL(0, modbus_master_msg_repeat_couter);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamThenReportError)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     register_modbus_master_error_cb(error_report_test_function);
//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);

//     TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
//     TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
//     TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_FRAME_ERR, test_error_rep.resp_read_error);
//     TEST_ASSERT_EQUAL(0, test_error_rep.exception_code);
//     TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndModbusErrorCbRegisteredWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThanRepeatOnErrorParamAndErrorReportedThenPushMsgBuferPtrToFreeQueue)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     register_modbus_master_error_cb(error_report_test_function);
//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);

//     TEST_ASSERT_NULL(msg_buf);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitWhenAndAnyRequestTransmitedAndRtuCrcErrorCatchedMoreTimeThenRepeatOnErrorParamAndSlaveErrorReportedThenGoToIdleState)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     register_modbus_master_error_cb(error_report_test_function);
//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_rtu_crc_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR + 1);
//     TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
// }


TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharReportError)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

    TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
    TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_FRAME_ERR, test_error_rep.resp_read_error);
    TEST_ASSERT_EQUAL(0, test_error_rep.exception_code);
    TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
}

TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRespWithCorrectIDandCRCRecivedAndFrameErrorCatchedThenAfterT3_5CharGoToIdleState)
{
    modbus_adr_t coil_adr = 0x0001;
    modbus_device_ID_t slave_ID = 0x03;
    modbus_data_qty_t coils_qty = 2;
    modbus_coil_disin_t readed_coil_disin[coils_qty];

    mock_slave_coil[0] = !!COIL_ON;
    mock_slave_coil[1] = !!COIL_ON;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
    generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

    TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
}

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRtuCrcErrorCatchedThenAfterT3_5CharReportError)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

//     TEST_ASSERT_EQUAL(slave_ID, test_error_rep.slave_ID);
//     TEST_ASSERT_EQUAL(MODBUS_READ_COILS_FUNC_CODE, test_error_rep.fun_conde);
//     TEST_ASSERT_EQUAL(MODBUS_MASTER_RESP_FRAME_ERR, test_error_rep.resp_read_error);
//     TEST_ASSERT_EQUAL(0, test_error_rep.exception_code);
//     TEST_ASSERT_EQUAL(0, test_error_rep.req_gen_error);
// }

// TEST(Master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestTransmitedWhenRespWithRtuCrcErrorCatchedThenAfterT3_5CharGoToIdleState)
// {
//     modbus_adr_t coil_adr = 0x0001;
//     modbus_device_ID_t slave_ID = 0x03;
//     modbus_data_qty_t coils_qty = 2;
//     modbus_coil_disin_t readed_coil_disin[coils_qty];

//     mock_slave_coil[0] = !!COIL_ON;
//     mock_slave_coil[1] = !!COIL_ON;

//     modbus_master_read_coils(coil_adr, coils_qty, slave_ID, readed_coil_disin);
//     generate_read_frame_error_catch_sequance(slave_ID, MODBUS_MASTER_REQ_REPEAT_ON_ANY_ERROR);

//     TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, modbus_master_manager_state_machine);
// }

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(Master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }
static void
reset_all_RTU_buffers(void)
{
    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_msg_t *msg;
        msg = modbus_queue_pop(free_q);
        if (NULL != msg)
        {
            memset(msg->req.data, 0, MODBUS_RTU_BUFFER_SIZE);
            memset(msg->resp.data, 0, MODBUS_RTU_BUFFER_SIZE);
            modbus_queue_push(free_q, &msg);
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

static void generate_read_frame_error_catch_sequance(modbus_device_ID_t slave_ID, uint8_t sequence_repeat)
{
    for (uint8_t i = 0; i < sequence_repeat; i++)
    {
        generate_send_req_sequence();
        generate_resp_using_slave_lib(slave_ID);
        generate_msg_T_1_5_char_brake_sequence();
        mock_USART_frame_error_EVENT();
        generate_msg_T_3_5_char_brake_sequence();
    }
}

static void generate_send_req_read_resp_msg_sequance(modbus_device_ID_t slave_ID)
{
    generate_send_req_sequence();
    generate_resp_using_slave_lib(slave_ID);
    generate_msg_T_1_5_char_brake_sequence();
    generate_msg_T_3_5_char_brake_sequence();
}

static void error_report_test_function(modbus_error_rep_t *error_rep)
{
    memset(&test_error_rep, 0, sizeof(test_error_rep));
    test_error_rep = *error_rep;
}

static void set_new_CRC_val(modbus_req_resp_t *req_res, modbus_CRC_t new_CRC)
{
    req_res->data[req_res->len - 2] = (modbus_buf_t)(new_CRC);
    req_res->data[req_res->len - 1] = (modbus_buf_t)(new_CRC >> 8);
}

static void generate_read_rtu_crc_error_catch_sequance(modbus_device_ID_t slave_ID, uint8_t sequence_repeat)
{
    for (uint8_t i = 0; i < sequence_repeat; i++)
    {
        generate_send_req_sequence();
        generate_resp_using_slave_lib(slave_ID);
        set_new_CRC_val(&msg_buf->resp,0);
        generate_msg_T_1_5_char_brake_sequence();
        generate_msg_T_3_5_char_brake_sequence();
    }
}