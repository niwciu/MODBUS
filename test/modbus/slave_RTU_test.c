#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
#include "modbus_master_PDU.h"
#include "modbus_RTU.h"
#include "mock_slave_driver_interface.h"
// #include <mem.h>
// #include "modbus_public_type.h"

// #include "tested_module.h"

TEST_GROUP(Slave_RTU_test);

extern modbus_slave_state_t slave_manager_state_machine;
extern modbus_msg_t *slave_msg_buf;

modbus_device_ID_t device_ID = 0x12;
baud_t baud = 38400;
parity_t parity = ODD;
modbus_mode_t mode = RTU;

TEST_SETUP(Slave_RTU_test)
{
    /* Init before every test */
    modbus_slave_init(mode,baud,parity,device_ID);
}

TEST_TEAR_DOWN(Slave_RTU_test)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithIncorectSlaveIdRecivedWhenTimer1_5CharTrigerThenModbusManagerGoBackToIdleStateWithEmptyMsgReqBuffer)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    modbus_device_ID_t msg_slave_ID = 0x99;

    modbus_master_read_coils_req(slave_msg_buf,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_buf->req.data,&slave_msg_buf->req.len, msg_slave_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    TEST_ASSERT_EQUAL(0,slave_msg_buf->req.len);
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdRecivedWhenTimer1_5CharTrigerThenStartSilenceTimerAndGoToSilenceBrakeState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;

    modbus_master_read_coils_req(slave_msg_buf,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_buf->req.data,&slave_msg_buf->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_RECIVER_SILANCE_PENDING, slave_manager_state_machine);
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndReciverTimer1_5CharTrigerWhenRxCharBeforeT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;

    modbus_master_read_coils_req(slave_msg_buf,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_buf->req.data,&slave_msg_buf->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_RECIVER_SILANCE_PENDING, slave_manager_state_machine);
    mock_USART_RX_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
}

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }

// TEST(Slave_RTU_test, )
// {
//     TEST_FAIL_MESSAGE("ADDED_NEW_TEST")
// }