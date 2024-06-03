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
extern modbus_msg_t *slave_msg_ptr;

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

TEST(Slave_RTU_test, GivenModbusSlaveInRTUmodeInitWhenRegisterAppDataToSlaveCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t app_data_coil = !!COIL_OFF;

    register_app_data_to_modbus_slave_coils_table(coil_adr, &app_data_coil);

    TEST_ASSERT_EQUAL(&app_data_coil, Slave_Coils[coil_adr]);
}

TEST(Slave_RTU_test, GivenModbusSlaveInRTUmodeInitWhenRegisterAppDataToSlaveDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t disin_adr = 0x0003;
    modbus_coil_disin_t app_data_disin;

    register_app_data_to_modbus_slave_din_table(disin_adr, &app_data_disin);

    TEST_ASSERT_EQUAL(&app_data_disin, Slave_Discrete_Inputs[disin_adr]);
}

TEST(Slave_RTU_test, GivenModbusSlaveInRTUmodeInitWhenRegisterAppDataToSlaveInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t inreg_adr = 0x0003;
    modbus_reg_t app_data_inreg;

    register_app_data_to_modbus_slave_inreg_table(inreg_adr, &app_data_inreg);

    TEST_ASSERT_EQUAL(&app_data_inreg, Slave_Input_Registers[inreg_adr]);
}

// TEST(Slave_RTU_test, GivenModbusSlaveInRTUmodeInitWhenRegisterAppDataToSlaveHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr)
// {
//     modbus_adr_t hreg_adr = 0x0003;
//     modbus_reg_t app_data_hreg;

//     register_app_data_to_modbus_slave_hreg_table(hreg_adr, &app_data_hreg);

//     TEST_ASSERT_EQUAL(&app_data_hreg, Slave_Holding_Registers[hreg_adr]);
// }


TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqRecivedWhenTimer1_5CharTrigerThenModbusManagerGoToModbusSlaveMsgRecived)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    modbus_device_ID_t msg_slave_ID = 0x99;

    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, msg_slave_ID);
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

    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, msg_slave_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    TEST_ASSERT_EQUAL(0,slave_msg_ptr->req.len);
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedWhenTimer1_5CharTrigerThenGoToModbusSlaveReciverSilancePendingState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;

    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);

}


TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcAndReciverTimer1_5CharTrigerWhenRxCharBeforeT3_5timerTriegerdThenIgnoreFrameAndGoToIdleState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;

    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
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
   
    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    modify_CRC_in_msg_frame();
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request

    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine); 
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
}



TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedAndTimer1_5CharTrigerAndTimer3_5CharTrigerThenSlavePrepareRespond)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    modbus_coil_disin_t coil_1= !!COIL_ON;
    modbus_coil_disin_t coil_2= !!COIL_ON;
    modbus_buf_t expected_resp[]={0x12, 0x01, 0x01, 0x03, 0x15, 0x0d};

    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr,&coil_1);
    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr+1,&coil_2);
   
    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request
    
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_resp,slave_msg_ptr->resp.data,slave_msg_ptr->resp.len);

}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedAndTimer1_5CharTrigerAndTimer3_5CharTrigerThenSlaveSendRespAndGoToModbuSlaveTransmitingResp)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    modbus_coil_disin_t coil_1= !!COIL_ON;
    modbus_coil_disin_t coil_2= !!COIL_ON;

    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr,&coil_1);
    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr+1,&coil_2);
   
    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request
    
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(USART_SENDING_DATA, USART_Tx_status);
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_TRANSMITING_RESP, slave_manager_state_machine);
}


TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithProperSlaveIdAndProperCrcRecivedAndTimer1_5CharTrigerAndTimer3_5CharTrigerAndSlaveSendingRespWhenWholeRespSendThenInitMsgBuffAndGoToIdleState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    modbus_coil_disin_t coil_1= !!COIL_ON;
    modbus_coil_disin_t coil_2= !!COIL_ON;

    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr,&coil_1);
    register_app_data_to_modbus_coils_din_table(Slave_Coils,coil_adr+1,&coil_2);
   
    modbus_master_read_coils_req(slave_msg_ptr,coil_adr,coils_qty);
    modbus_RTU_send(slave_msg_ptr->req.data,&slave_msg_ptr->req.len, device_ID);
    mock_USART_RX_IRQ(); // To simulate that some char was recived after using modbus_RTU_send func to generate recived request
    
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    mock_1_5_char_timer_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_MSG_RECIVED, slave_manager_state_machine);
    check_modbus_request();
    check_modbus_request();
    mock_3_5_char_timer_IRQ();
    check_modbus_request();
    mock_USART_Tx_Done_IRQ();
    check_modbus_request();
    TEST_ASSERT_EQUAL(0,slave_msg_ptr->req.len);
    TEST_ASSERT_EQUAL(0,slave_msg_ptr->resp.len);
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
static void modify_CRC_in_msg_frame(void)
{
    modbus_CRC_t CRC;

    CRC= read_u16_from_buf(&slave_msg_ptr->req.data[slave_msg_ptr->req.len -2]);
    CRC++;
    write_u16_to_buf(&slave_msg_ptr->req.data[slave_msg_ptr->req.len -2],CRC);

}