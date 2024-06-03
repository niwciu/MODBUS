#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
#include "modbus_master_PDU.h"
#include "modbus_RTU.h"
#include "mock_slave_driver_interface.h"
#include "buf_rw.h"
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

static void modify_CRC_in_msg_frame(void);

TEST_SETUP(Slave_RTU_test)
{
    /* Init before every test */
    modbus_slave_init(mode,baud,parity,device_ID);
}

TEST_TEAR_DOWN(Slave_RTU_test)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqRecivedWhenTimer1_5CharTrigerThenModbusManagerGoToModbusSlaveMsgRecived)
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
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_RECIVER_SILANCE_PENDING, slave_manager_state_machine);
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    TEST_ASSERT_EQUAL(0,slave_msg_buf->req.len);
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedWhenTimer1_5CharTrigerThenGoToModbusSlaveReciverSilancePendingState)
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


TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcAndReciverTimer1_5CharTrigerWhenRxCharBeforeT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState)
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
    mock_USART_RX_IRQ(); //this is recived data before 3,5char time expired. According to modbus doc such event should trigger frame error flag and msg should be ignored after 3,5 char expired
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndIncorrectCrcRecivedAndReciverTimer1_5CharTrigerWhenT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
   

    modbus_master_read_coils_req(slave_msg_buf,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_buf->req.data,&slave_msg_buf->req.len, device_ID);
    modify_CRC_in_msg_frame();
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_RECIVER_SILANCE_PENDING, slave_manager_state_machine);
    mock_USART_RX_IRQ(); //this is recived data before 3,5char time expired. According to modbus doc such event should trigger frame error flag and msg should be ignored after 3,5 char expired
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
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
static void modify_CRC_in_msg_frame(void)
{
    modbus_CRC_t CRC;

    CRC= read_u16_from_buf(&slave_msg_buf->req.data[slave_msg_buf->req.len -2]);
    CRC++;
    write_u16_to_buf(&slave_msg_buf->req.data[slave_msg_buf->req.len -2],CRC);

}