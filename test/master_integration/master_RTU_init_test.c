#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_queue.h"
#include "mock_master_driver_interface.h"

// #include "tested_module.h"

TEST_GROUP(master_RTU_init_test);

extern modbus_msg_t modbus_msg[MAX_MODBUS_MSG_QUEUE_ITEMS];

extern struct modbus_RTU_driver_struct *RTU_driver;
extern modbus_master_state_t master_manager_state_machine;
extern modbus_queue_t *free_q;
extern modbus_queue_t *tx_rx_q;

TEST_SETUP(master_RTU_init_test)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(master_RTU_init_test)
{
    /* Cleanup after every test */
}

TEST(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenRtuReqAndRespBuffersAreRegistered)
{
    modbus_master_init(RTU, 0, 0);
    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_req_buf[i][0], modbus_msg[i].req.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
        TEST_ASSERT_EQUAL_UINT32_ARRAY(&RTU_resp_buf[i][0], modbus_msg[i].resp.data, MAX_MODBUS_MSG_QUEUE_ITEMS);
    }
}

TEST(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenTxRxRTUmsgQueueInitialized)
{
    modbus_master_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(0, tx_rx_q->head);
    TEST_ASSERT_EQUAL(0, tx_rx_q->tail);
}

TEST(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenFreeRTUmsgQueueInitializedAndFull)
{
    modbus_master_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(0, free_q->tail);
    TEST_ASSERT_EQUAL((MAX_MODBUS_MSG_QUEUE_ITEMS - 1), free_q->head);
}

TEST(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenDriverInterfaceIsRegistered)
{
    modbus_master_init(RTU, 0, 0);
    TEST_ASSERT_NOT_NULL(RTU_driver);
}

TEST(master_RTU_init_test, GivenBaudAndParitySetWhenModbusMasterInitInRTUmodeThenDriverIsInitializedWithProperBaudAndParity)
{
    uint32_t baud = 38400;
    parity_t parity = ODD;

    modbus_master_init(RTU, baud, parity);

    TEST_ASSERT_EQUAL(baud, mock_USART.baud_rate);
    TEST_ASSERT_EQUAL(parity, mock_USART.parity);
    TEST_ASSERT_EQUAL(DRIVER_INITIALIZED, mock_USART.init_status);
}

TEST(master_RTU_init_test, WhenModbusMasterInitInRTUmodeThenModbusMasterManagerStateMachineIsSetToIdle)
{
    modbus_master_init(RTU, 0, 0);

    TEST_ASSERT_EQUAL(MODBUS_MASTER_IDLE, master_manager_state_machine);
}

// TEST(master_RTU_init_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }