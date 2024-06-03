#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"
#include "mock_slave_driver_interface.h"

// #include "tested_module.h"

TEST_GROUP(Slave_RTU_init_test);

extern modbus_msg_t slave_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

extern modbus_queue_t *slave_free_q;
extern modbus_queue_t *slave_tx_rx_q;

extern modbus_RTU_driver_struct_t *slave_RTU_driver;
extern modbus_slave_state_t slave_manager_state_machine;
extern modbus_device_ID_t modbus_slave_ID;

extern modbus_status_flag_t TIMER_1_5_CHAR_FLAG;
extern modbus_status_flag_t TIMER_3_5_CHAR_FLAG;
extern modbus_status_flag_t FRAME_ERROR_FLAG;
extern modbus_status_flag_t RESP_TRANSMITION_FLAG;

TEST_SETUP(Slave_RTU_init_test)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Slave_RTU_init_test)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_init_test, WhenModbusSlavenitInRTUmodeThenRtuReqAndRespBuffersAreRegistered)
{
    modbus_slave_init(RTU, 0, 0,0);
    TEST_ASSERT_EQUAL(&RTU_req_buf[0][0], slave_msg->req.data);
    TEST_ASSERT_EQUAL(&RTU_resp_buf[0][0], slave_msg->resp.data);

}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenDriverInterfaceIsRegistered)
{
    modbus_slave_init(RTU, 0, 0,0);
    TEST_ASSERT_NOT_NULL(slave_RTU_driver);
}

TEST(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity,0);

    TEST_ASSERT_EQUAL(baud, mock_slave_USART.baud_rate);
    TEST_ASSERT_EQUAL(parity, mock_slave_USART.parity);
    TEST_ASSERT_EQUAL(DRIVER_INITIALIZED, mock_slave_USART.init_status);
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToIdle)
{
    modbus_slave_init(RTU, 0, 0,0);

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
}

TEST(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenRxInterruptEnable)
{
    
    modbus_slave_init(RTU, 0, 0,0);

    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_slave_USART.Rx_IRQ);

}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgTxDoneCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity,0);
    TEST_ASSERT_NOT_NULL (mock_msg_tx_done_cb);
    
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenT1_5CharExpiredCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity,0);
    TEST_ASSERT_NOT_NULL (mock_1_5_char_break_cb);
    
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenT3_5CharExpiredCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity,0);
    TEST_ASSERT_NOT_NULL (mock_3_5_char_break_cb);
    
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgFrameErrorCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity,0);
    TEST_ASSERT_NOT_NULL (mock_frame_error_cb);
    
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeWithDefinedSlaveIdThenModbusSlaveIdIsAssigned)
{
    baud_t baud = 38400;
    parity_t parity = ODD;
    modbus_device_ID_t expected_slave_ID= 0x12;

    modbus_slave_init(RTU,baud,parity, expected_slave_ID);

    TEST_ASSERT_EQUAL(expected_slave_ID,modbus_slave_ID);
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToModbusSlaveIdle)
{
    baud_t baud = 38400;
    parity_t parity = ODD;
    modbus_device_ID_t expected_slave_ID= 0x12;

    modbus_slave_init(RTU,baud,parity, expected_slave_ID);

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE,slave_manager_state_machine);
}

TEST(Slave_RTU_init_test,WhenModbusSlaveInitInRTUmodeThenAllModbusStatusFlagsAreCleared )
{
    baud_t baud = 38400;
    parity_t parity = ODD;
    modbus_device_ID_t expected_slave_ID= 0x12;

    modbus_slave_init(RTU,baud,parity, expected_slave_ID);

    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED ,TIMER_1_5_CHAR_FLAG);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED ,TIMER_3_5_CHAR_FLAG);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED ,FRAME_ERROR_FLAG);
    TEST_ASSERT_EQUAL(MODBUS_FLAG_CLEARED ,RESP_TRANSMITION_FLAG);
}

// TEST(Slave_RTU_init_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED NEW TEST !!!");
// }

// TEST(Slave_RTU_init_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED NEW TEST !!!");
// }
