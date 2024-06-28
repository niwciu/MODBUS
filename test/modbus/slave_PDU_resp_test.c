#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "buf_rw.h"
#include "mock_slave_app_data.h"
#include "mock_master_app_data.h"

modbus_ret_t status;

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;

TEST_GROUP(Slave_PDU_resp);

TEST_SETUP(Slave_PDU_resp)
{
    /* Init before every test */
    mock_register_slave_coils_data();
    mock_register_slave_discrete_inputs_data();
    mock_register_slave_input_registers_data();
    mock_register_slave_holding_registers_data();

    mock_set_all_slave_cails_to_off();
    mock_set_all_slave_din_to_off();

    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;
}

TEST_TEAR_DOWN(Slave_PDU_resp)
{
    /* Cleanup after every test */
}

TEST(Slave_PDU_resp, SlaveParseMsgWhenRtuMsgPassAsNullPtrArgumentToParse)
{
    static modbus_msg_t *RTU_NULL_msg;
    modbus_ret_t status= parse_master_request_and_prepare_resp(RTU_NULL_msg);
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, status);
}

TEST(Slave_PDU_resp, SlaveParseMsgWhenReqDataBufferPassAsNullPtrArgumentToParse)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    RTU_msg->req.data=NULL;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, parse_master_request_and_prepare_resp(RTU_msg));
}
    // TEST_FAIL_MESSAGE("ADDED new TEST");

TEST(Slave_PDU_resp, SlaveRead5Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x15};   // 0b0001 0101

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead8Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x55};

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead9Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55, 0x01};

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead16Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55, 0x55};

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead17Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 17;
    modbus_byte_count_t expected_byte_count = 3; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[3] = {0x55, 0x55, 0x01};

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[2], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 2]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead5DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x15};    // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, adr, din_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead8DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x55};    // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, adr, din_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead9DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 9;
    modbus_byte_count_t expected_byte_count = 2;    // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55, 0x01}; // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, adr, din_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead16DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 16;
    modbus_byte_count_t expected_byte_count = 2;    // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55, 0x55}; // 0b0001 0101

    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, adr, din_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead17DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 17;
    modbus_byte_count_t expected_byte_count = 3;          // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[3] = {0x55, 0x55, 0x01}; // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, adr, din_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[2], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 2]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveReadOneHoldingRegister)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A};

    mock_set_expected_slave_hreg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_holding_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead3HoldingRegisters)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 3;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[3] = {0xA55A, 0x5AA5, 0xA55A};

    mock_set_expected_slave_hreg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_holding_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[1], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 2]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[2], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 4]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveReadOneInputRegister)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A};

    mock_set_expected_slave_input_reg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_input_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead3InputRegisters)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 3;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[3] = {0xA55A, 0x5AA5, 0xA55A};

    mock_set_expected_slave_input_reg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_input_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[1], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 2]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[2], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 4]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteSingleCoil)
{
    modbus_adr_t adr = 0x0000;
    modbus_w_coil_t resp_buf_coil_expected_value = COIL_ON;

    set_coil_state(Master_Coils, adr, !!COIL_ON);
    modbus_master_write_single_coil_req(RTU_msg, adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(resp_buf_coil_expected_value, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteSingleCoilToOnAndCheckCoilStatus)
{
    modbus_adr_t adr = 0x0001;
    // modbus_w_coil_t RTU_msg->resp.data_coil_expected_value = COIL_ON;

    mock_set_all_slave_cails_to_off();
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr]);
    set_coil_state(Master_Coils, adr, !!COIL_ON);
    modbus_master_write_single_coil_req(RTU_msg, adr);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils5Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    // modbus_coil_disin_t data_coil_states[1] = {0x15};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils5CoilsAndCheckCoilsStatus)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    // modbus_coil_disin_t data_coil_states[1] = {0x15};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);

    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 1]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 2]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 3]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 4]);
    // below should be zeros due the mock_set_all_calis_to_off
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 5]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 6]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 7]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils8Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    // modbus_coil_disin_t data_coil_states[1] = {0x55};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils8CoilsAndCheckCoilsStatus)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    // modbus_coil_disin_t data_coil_states[] = {1,0,1,0,1,0,1,0};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 1]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 2]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 3]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 4]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 5]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 6]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 7]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils9Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    // modbus_coil_disin_t data_coil_states[2] = {0x55, 0x01};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils9CoilsAndCheckCoilsStatus)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    // modbus_coil_disin_t data_coil_states[2] = {0x55, 0x01};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 1]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 2]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 3]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 4]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 5]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 6]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 7]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 8]);
    // below should be zeros due the mock_set_all_calis_to_off
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 9]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 10]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 11]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 12]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 13]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 14]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 15]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils16Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    // modbus_coil_disin_t data_coil_states[2] = {0x55, 0x55};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils16CoilsAndCheckCoilsStatus)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    // modbus_coil_disin_t data_coil_states[2] = {0x55, 0x55};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 1]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 2]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 3]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 4]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 5]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 6]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 7]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 8]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 9]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 10]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 11]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 12]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 13]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 14]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 15]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils17Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    // modbus_coil_disin_t data_coil_states[3] = {0x55, 0x55, 0x01};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]);
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils17CoilsAndCheckCoilsStatus)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 17;
    // modbus_coil_disin_t data_coil_states[3] = {0x55, 0x55, 0x01};

    mock_set_all_slave_cails_to_off();
    mock_set_expected_master_coils_alternately(adr, coil_qty, !!COIL_ON);
    modbus_master_write_multiple_coils_req(RTU_msg, adr, coil_qty);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 1]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 2]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 3]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 4]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 5]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 6]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 7]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 8]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 9]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 10]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 11]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 12]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 13]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 14]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 15]);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr + 16]);
    // // below should be zeros due the mock_set_all_calis_to_off
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 17]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 18]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 19]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 21]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 22]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 23]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 24]);
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr + 25]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteSingleRegister)
{
    modbus_adr_t adr = 0x0000;
    modbus_reg_t reg_val = 0x5A5A;

    mock_master_hreg[adr] = reg_val;
    modbus_master_write_single_reg_req(RTU_msg, adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(RTU_msg->resp.data + MODBUS_RESP_WRITE_ADR_IDX));
    TEST_ASSERT_EQUAL_HEX16(reg_val, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}
TEST(Slave_PDU_resp, SlaveWriteSingleRegisterAndCheckRegisterValue)
{
    modbus_adr_t adr = 0x0001;
    modbus_reg_t reg_val = 0x5A5A;
    mock_reset_all_slave_hreg_value();
    mock_master_hreg[adr] = reg_val;
    modbus_master_write_single_reg_req(RTU_msg, adr);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(reg_val, mock_slave_hreg[adr]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleRegister3Reg)
{
    modbus_adr_t adr = 0x0001;
    // modbus_reg_t reg_val[3] = {0x5A5A, 0xA5A5, 0x5A5A};
    modbus_data_qty_t reg_qty = 3;

    mock_reset_all_slave_hreg_value();
    mock_set_expected_master_hreg_alternately(adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(RTU_msg->resp.data + MODBUS_RESP_WRITE_ADR_IDX));
    TEST_ASSERT_EQUAL_HEX16(reg_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleRegister3regAndCheckRegValue)
{
    modbus_adr_t adr = 0x0001;
    modbus_reg_t exp_reg_val[3] = {0x5A5A, 0xA5A5, 0x5A5A};
    modbus_data_qty_t reg_qty = 3;

    mock_reset_all_slave_hreg_value();
    mock_set_expected_master_hreg_alternately(adr, reg_qty, 0x5A5A);
    modbus_master_write_multiple_reg_req(RTU_msg, adr, reg_qty);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[0], mock_slave_hreg[adr]);
    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[1], mock_slave_hreg[adr + 1]);
    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[2], mock_slave_hreg[adr + 2]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

//
// ToDo odczyt zbyt duzej ilosci coili, disin i rej po strnie slave ilosc obiektow okresla config
