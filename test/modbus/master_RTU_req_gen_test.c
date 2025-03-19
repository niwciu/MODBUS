#include "unity/fixture/unity_fixture.h"
#include "modbus_master.h"
#include "modbus_slave_PDU.h"
#include "modbus_slave.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_queue.h"

#include "mock_master_driver_interface.h"
#include "mock_master_data_interface.h"
#include "mock_slave_app_data.h"
#include <memory.h>

TEST_GROUP(Master_RTU_req_gen_test);

#define TEST_SLAVE_COILS_TABLE_SIZE (MOCK_MASTER_COILS_TABLE_SIZE)

extern modbus_queue_t *tx_rx_q;
extern modbus_queue_t *free_q;
extern modbus_msg_t *msg_buf;

extern modbus_status_flag_t MODBUS_MASTER_REQ_TRANSMITION_FLAG;
extern modbus_master_state_t modbus_master_manager_state_machine;
extern modbus_timer_t modbus_master_resp_timeout_timer;
extern uint8_t modbus_master_msg_repeat_couter;

extern modbus_status_flag_t MODBUS_MASTER_FRAME_ERROR_FLAG;

static modbus_coil_disin_t test_slave_coils[TEST_SLAVE_COILS_TABLE_SIZE] = {COIL_OFF};

static void reset_all_RTU_buffers(void);

TEST_SETUP(Master_RTU_req_gen_test)
{
    /* Init before every test */
    modbus_master_init(RTU, 9600, ODD);
    reset_all_RTU_buffers();
    for (uint8_t i = 0; i < TEST_SLAVE_COILS_TABLE_SIZE; i++)
    {
        register_app_data_to_modbus_slave_coils_table(i, &test_slave_coils[i]);
    }
}

TEST_TEAR_DOWN(Master_RTU_req_gen_test)
{
    /* Cleanup after every test */
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_buf_t expected_master_request[] = {0x05, 0x03, 0x00, 0x02, 0x00, 0x01, 0x24, 0x4E};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadHoldingRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t hreg_adr = 0x0000;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t hreg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    ret_status = modbus_master_read_holding_reg(hreg_adr, hreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t inreg_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x02;
    modbus_data_qty_t inreg_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    modbus_buf_t expected_master_request[] = {0x02, 0x04, 0x00, 0x05, 0x00, 0x03, 0xA0, 0x39};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t inreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x02;
    modbus_data_qty_t inreg_qty = 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadInputRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t inreg_adr = 0x0000;
    modbus_device_ID_t slave_ID = 0x05;
    modbus_data_qty_t inreg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    ret_status = modbus_master_read_input_reg(inreg_adr, inreg_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    modbus_buf_t expected_master_request[] = {0x08, 0x01, 0x00, 0x05, 0x00, 0x03, 0x6c, 0x93};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t coil_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    ret_status = modbus_master_read_coils(coil_adr, coil_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_buf_t expected_master_request[] = {0x08, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x93};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = 3;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusReadDisInWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t disin_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x08;
    modbus_data_qty_t disin_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    ret_status = modbus_master_read_discrete_inputs(disin_adr, disin_qty, slave_ID);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x01;
    modbus_coil_disin_t coil_state = !!COIL_ON;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    modbus_buf_t expected_master_request[] = {0x01, 0x05, 0x00, 0x05, 0xFF, 0x00, 0x9c, 0x3b};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_write_single_coil(coil_adr, slave_ID, coil_state);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleCoilWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0005;
    modbus_device_ID_t slave_ID = 0x01;
    modbus_coil_disin_t coil_state = !!COIL_ON;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_single_reg(coil_adr, slave_ID, coil_state);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0025;
    modbus_device_ID_t slave_ID = 0x11;
    modbus_reg_t reg_state = 0x5a5a;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    modbus_buf_t expected_master_request[] = {0x11, 0x06, 0x00, 0x25, 0x5a, 0x5a, 0x20, 0x0a};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_write_single_reg(hreg_adr, slave_ID, reg_state);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);

    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteSingleRegisterWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0025;
    modbus_device_ID_t slave_ID = 0x11;
    modbus_reg_t reg_state = 0x5a5a;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }

    ret_status = modbus_master_write_single_reg(hreg_adr, slave_ID, reg_state);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_reg_t hreg_data[] = {0x2001, 0x2002, 0x2003, 0x2004};
    modbus_buf_t expected_master_request[] = {0x09, 0x10, 0x00, 0x02, 0x00, 0x04, 0x08, 0x20, 0x01, 0x20, 0x02, 0x20, 0x03, 0x20, 0x04, 0xc8, 0x00};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID, hreg_data);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
    // TEST_ASSERT_EQUAL(hreg_data, tx_rx_msg_buf->rw_data_ptr);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_reg_t hreg_data[] = {0x2001, 0x2002, 0x2003, 0x2004};

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID, hreg_data);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleRegistersWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t hreg_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t hreg_qty = MODBUS_MAX_WRITE_REG_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_reg_t hreg_data[MODBUS_MAX_WRITE_REG_QTY + 1] = {0x2001};
    ret_status = modbus_master_write_multiple_reg(hreg_adr, hreg_qty, slave_ID, hreg_data);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndFreeMsgBuffersAreAvailableThenProperRequestSendToTxRxQueue)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_coil_disin_t coils_data[] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_ON};
    modbus_buf_t expected_master_request[] = {0x09, 0x0F, 0x00, 0x02, 0x00, 0x04, 0x01, 0x0d, 0x87, 0x35};
    uint8_t expected_msg_len = (sizeof(expected_master_request) / sizeof(modbus_buf_t));

    ret_status = modbus_master_write_multiple_coils(coil_adr, coils_qty, slave_ID, coils_data);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NOT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(expected_msg_len, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_master_request, tx_rx_msg_buf->req.data, tx_rx_msg_buf->req.len);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE, ret_status);
    // TEST_ASSERT_EQUAL(coils_data, tx_rx_msg_buf->rw_data_ptr);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithProperParametersAndNoFreeMsgBuffersAreAvailableThenReturnFreeQueueEmptyErr)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = 4;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_coil_disin_t coils_data[] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_ON};

    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
    {
        modbus_queue_pop(free_q);
    }
    ret_status = modbus_master_write_multiple_coils(coil_adr, coils_qty, slave_ID, coils_data);
    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR, ret_status);
}

TEST(Master_RTU_req_gen_test, GivenModbusMasterInRTUmodeInitWhenModbusWriteMultipleCoilsWithWrongParametersAndFreeMsgBuffersAreAvailableThenReturnMasterReqLibError)
{
    modbus_adr_t coil_adr = 0x0002;
    modbus_device_ID_t slave_ID = 0x09;
    modbus_data_qty_t coils_qty = MODBUS_MAX_WRITE_COILS_QTY + 1;
    modbus_msg_t *tx_rx_msg_buf;
    modbus_master_req_ret_t ret_status;
    modbus_coil_disin_t coils_data[MODBUS_MAX_WRITE_COILS_QTY + 1] = {!!COIL_ON};
    ret_status = modbus_master_write_multiple_coils(coil_adr, coils_qty, slave_ID, coils_data);

    tx_rx_msg_buf = modbus_queue_pop(tx_rx_q);
    TEST_ASSERT_NULL(tx_rx_msg_buf);
    TEST_ASSERT_EQUAL(MODBUS_MASTER_LIB_PDU_REQ_ERROR, ret_status);
}

static void reset_all_RTU_buffers(void)
{
    for (int i = 0; i < MODBUS_MASTER_MAX_MSG_QUEUE_ITEMS; i++)
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