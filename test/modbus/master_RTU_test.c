#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_queue.h"

#include "mock_master_driver_interface.h"
// #include "mock_master_app_data.h"
#include "mock_slave_app_data.h"
#include <mem.h>

TEST_GROUP(master_RTU_test);
extern modbus_queue_t *tx_rx_q;
extern modbus_queue_t *free_q;
extern modbus_msg_t *msg_buf;

extern modbus_master_state_t master_manager_state_machine;

    static void
    reset_all_RTU_buffers(void);

TEST_SETUP(master_RTU_test)
{
    /* Init before every test */
    modbus_master_init(RTU, 9600, ODD);
    reset_all_RTU_buffers();
}

TEST_TEAR_DOWN(master_RTU_test)
{
    /* Cleanup after every test */
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterCoilPtrTableThenCoilPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t app_data_coil = !!COIL_OFF;

    register_app_data_to_modbus_master_coils_table(coil_adr, &app_data_coil);

    TEST_ASSERT_EQUAL(&app_data_coil, Master_Coils[coil_adr]);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterDisInPtrTableThenDisInPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t disin_adr = 0x0003;
    modbus_coil_disin_t app_data_disin;

    register_app_data_to_modbus_master_din_table(disin_adr, &app_data_disin);

    TEST_ASSERT_EQUAL(&app_data_disin, Master_Discrete_Inputs[disin_adr]);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterInRegPtrTableThenInRegPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t inreg_adr = 0x0003;
    modbus_reg_t app_data_inreg;

    register_app_data_to_modbus_master_inreg_table(inreg_adr, &app_data_inreg);

    TEST_ASSERT_EQUAL(&app_data_inreg, Master_Input_Registers[inreg_adr]);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenRegisterAppDataToMasterHolRegPtrTableThenHolRegPtrIsEqualToRegisteredDataAdr)
{
    modbus_adr_t hreg_adr = 0x0003;
    modbus_reg_t app_data_hreg;

    register_app_data_to_modbus_master_hreg_table(hreg_adr, &app_data_hreg);

    TEST_ASSERT_EQUAL(&app_data_hreg, Master_Holding_Registers[hreg_adr]);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
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
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
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

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
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

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
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
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
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

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
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

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
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
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
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

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    modbus_buf_t expected_master_request[] = {0x08, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x93};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x01;
    modbus_coil_disin_t coil_state = !!COIL_ON;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    modbus_buf_t expected_master_request[] = {0x01, 0x05, 0x00, 0x05, 0xFF, 0x00, 0x9c, 0x3b};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    register_app_data_to_modbus_master_coils_table(coil_adr, &coil_state);

    ret_status = modbus_master_write_single_coil(coil_adr, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x01;
    modbus_coil_disin_t coil_state = !!COIL_ON;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    register_app_data_to_modbus_master_coils_table(coil_adr, &coil_state);

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_single_reg(coil_adr, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0025;
    modbus_device_ID_t slave_ID = 0x11;
    modbus_reg_t reg_state = 0x5a5a;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    modbus_buf_t expected_master_request[] = {0x11, 0x06, 0x00, 0x25, 0x5a, 0x5a, 0x20, 0x0a};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    register_app_data_to_modbus_master_hreg_table(hreg_adr, &reg_state);

    ret_status = modbus_master_write_single_reg(hreg_adr, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0025;
    modbus_device_ID_t slave_ID = 0x11;
    modbus_reg_t reg_state = 0x5a5a;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    register_app_data_to_modbus_master_hreg_table(hreg_adr, &reg_state);

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }

    ret_status = modbus_master_write_single_reg(hreg_adr, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    modbus_reg_t hreg_data[] = {0x2001, 0x2002, 0x2003, 0x2004};
    modbus_buf_t expected_master_request[] = {0x09, 0x10, 0x00, 0x02, 0x00, 0x04, 0x08, 0x20, 0x01, 0x20, 0x02, 0x20, 0x03, 0x20, 0x04, 0xc8, 0x00};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    for (uint8_t i = 0; i < hreg_qty; i++)
    {
        register_app_data_to_modbus_master_hreg_table(hreg_adr + i, hreg_data + i);
    }
    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = MODBUS_MAX_WRITE_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    modbus_coil_disin_t coils_data[] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_ON};
    modbus_buf_t expected_master_request[] = {0x09, 0x0F, 0x00, 0x02, 0x00, 0x04, 0x01, 0x0d, 0x87, 0x35};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    for (uint8_t i = 0; i < coils_qty; i++)
    {
        register_app_data_to_modbus_master_coils_table(coil_adr + i, coils_data + i);
    }
    ret_status = modbus_master_write_multiple_coils(coil_adr, coils_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SEND, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;

    for (int i = 0; i < MAX_MODBUS_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_multiple_coils(coil_adr, coils_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR, ret_status);
}

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = MODBUS_MAX_WRITE_COILS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_error_t ret_status;
    ret_status = modbus_master_write_multiple_reg(coil_adr, coils_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_REQ_ERROR, ret_status);
}

//Modbus Manager integration Tests

TEST(master_RTU_test, GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartTxStatusIsEqualToUsartSendingAndMasterUsartTxBufPtrIsEqualToMsgPtr)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(USART_SENDING_DATA, master_USART_Tx_status);
    TEST_ASSERT_EQUAL(msg_buf->req.data,mock_master_tx_buf_ptr);
}


TEST(master_RTU_test,GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterUsartRxAndTxIrqEnabled)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_master_USART.Rx_IRQ);
    TEST_ASSERT_EQUAL(IRQ_ENABLED, mock_master_USART.Tx_IRQ);
}

TEST(master_RTU_test,GivenModbusMasterInRTUmodeInitWhenGivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterRxBufferPtrSetToMsgRespBufData)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(&msg_buf->resp, mock_master_rx_msg_ptr);
}

TEST(master_RTU_test,GivenModbusMasterInRTUmodeInitAndAnyRequestPlacedInQueueWhenModbusMasterManagerCheckThenMasterManagerStateMachineSetToModbusMasterTransmittingReqAndSetMODBUS_MASTER_REQ_TRANSMITION_FLAG)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 2;

    modbus_master_read_coils(coil_adr, coils_qty, slave_ID);
    check_modbus_master_manager();
    TEST_ASSERT_EQUAL(MODBUS_FLAG_SET,MODBUS_MASTER_TRANSMITTING_REQ);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_TRANSMITTING_REQ, master_manager_state_machine);
}

// TEST(master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU_test,)
// {
//    TEST_FAIL_MESSAGE("Implement your test!");
// }

// TEST(master_RTU_test,)
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