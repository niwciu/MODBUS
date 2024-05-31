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
    modbus_slave_init(RTU, 0, 0);
    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_req_buf[i][0], slave_msg[i].req.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_resp_buf[i][0], slave_msg[i].resp.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
    }
}


TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenTxRxRTUmsgQueueInitialized )
{
    modbus_slave_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(0,slave_tx_rx_q->head);
    TEST_ASSERT_EQUAL(0,slave_tx_rx_q->tail);
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFullAndOneMsgBuffPop)
{
    modbus_slave_init(RTU, 0, 0);
    TEST_ASSERT_EQUAL(1,slave_free_q->tail);
    TEST_ASSERT_EQUAL((MAX_MODBUS_MSG_QUEUE_ITEMS - 1),slave_free_q->head);
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenDriverInterfaceIsRegistered)
{
    modbus_slave_init(RTU, 0, 0);
    TEST_ASSERT_NOT_NULL(slave_RTU_driver);
}

TEST(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity);

    TEST_ASSERT_EQUAL(baud, mock_slave_USART.baud_rate);
    TEST_ASSERT_EQUAL(parity, mock_slave_USART.parity);
    TEST_ASSERT_EQUAL(DRIVER_INITIALIZED, mock_slave_USART.init_status);
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenModbusSlaveManagerStateMachineIsSetToIdle)
{
    modbus_slave_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
}

TEST(Slave_RTU_init_test, GivenBaudAndParitySetWhenModbusSlaveInitInRTUmodeThenRxInterruptEnable)
{
    
    modbus_slave_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_slave_USART.Rx_IRQ);

}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgTxDoneCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity);
    TEST_ASSERT_NOT_NULL (mock_msg_tx_done_cb);
    
}

TEST(Slave_RTU_init_test, WhenModbusSlaveInitInRTUmodeThenMsgRxDoneCallbackIsRegistered)
{
    baud_t baud = 38400;
    parity_t parity = ODD;

    modbus_slave_init(RTU, baud, parity);
    TEST_ASSERT_NOT_NULL (mock_msg_rx_done_cb);
    
}

// TEST(Slave_RTU_init_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED NEW TEST !!!");
// }