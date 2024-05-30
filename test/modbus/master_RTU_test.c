#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_queue.h"

#include "mock_modbus_driver_interface.h"
// #include "mock_master_app_data.h"
#include "mock_slave_app_data.h"

TEST_GROUP(master_RTU);
extern modbus_queue_t *tx_rx_q;
extern modbus_queue_t *free_q;

TEST_SETUP(master_RTU)
{
    /* Init before every test */
    modbus_master_init(RTU, 9600, ODD);
}

TEST_TEAR_DOWN(master_RTU)
{
    /* Cleanup after every test */
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t app_data_coil = !!COIL_OFF;

    register_app_data_to_modbus_master_coils_table(coil_adr, &app_data_coil);

    TEST_ASSERT_EQUAL(&app_data_coil, Master_Coils[coil_adr]);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t disin_adr = 0x0003;
    modbus_coil_disin_t app_data_disin;

    register_app_data_to_modbus_master_din_table(disin_adr, &app_data_disin);

    TEST_ASSERT_EQUAL(&app_data_disin, Master_Discrete_Inputs[disin_adr]);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t inreg_adr = 0x0003;
    modbus_reg_t app_data_inreg;

    register_app_data_to_modbus_master_inreg_table(inreg_adr, &app_data_inreg);

    TEST_ASSERT_EQUAL(&app_data_inreg, Master_Input_Registers[inreg_adr]);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_reg_t app_data_hreg;

    register_app_data_to_modbus_master_hreg_table(hreg_adr, &app_data_hreg);

    TEST_ASSERT_EQUAL(&app_data_hreg, Master_Holding_Registers[hreg_adr]);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    modbus_buf_t expected_master_request[] = {0x05, 0x03, 0x00, 0x02, 0x00, 0x01, 0x24, 0x4E};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t hreg_adr = 0x0000;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t inreg_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x02;
    modbus_data_qty_t inreg_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    modbus_buf_t expected_master_request[] = {0x02, 0x04, 0x00, 0x05, 0x00, 0x03, 0xA0, 0x39};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t inreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x02;
    modbus_data_qty_t inreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t inreg_adr = 0x0000;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t inreg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
 
    modbus_buf_t expected_master_request[] = {0x08, 0x01, 0x00, 0x05, 0x00, 0x03, 0x6c, 0x93};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len + 2);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    
    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }