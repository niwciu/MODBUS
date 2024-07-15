#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "buf_rw.h"
#include "mock_slave_app_data.h"

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
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, parse_master_request_and_prepare_resp(RTU_NULL_msg));
}

TEST(Slave_PDU_resp, SlaveParseMsgWhenReqDataBufferPassAsNullPtrArgumentToParse)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    req.adr=adr;
    req.obj_qty=coil_qty;

    modbus_master_read_coils_req(RTU_msg, &req);
    RTU_msg->req.data = NULL;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, parse_master_request_and_prepare_resp(RTU_msg));
}
TEST(Slave_PDU_resp, SlaveParseMsgWhenRespDataBufferPassAsNullPtrArgumentToParse)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    req.adr = adr;
    req.obj_qty = coil_qty;

    modbus_master_read_coils_req(RTU_msg, &req);
    RTU_msg->resp.data = NULL;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, parse_master_request_and_prepare_resp(RTU_msg));
}

TEST(Slave_PDU_resp, SlaveRead5Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x15};   // 0b0001 0101
    req.adr = adr;
    req.obj_qty = coil_qty;

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead8Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x55};
    req.adr = adr;
    req.obj_qty = coil_qty;

    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead9Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55, 0x01};
    req.adr = adr;
    req.obj_qty = coil_qty;
    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead16Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55, 0x55};
    req.adr = adr;
    req.obj_qty = coil_qty;
    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead17Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 17;
    modbus_byte_count_t expected_byte_count = 3; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[3] = {0x55, 0x55, 0x01};
    req.adr = adr;
    req.obj_qty = coil_qty;
    mock_set_expected_slave_coils_alternately(adr, coil_qty, 1);

    modbus_master_read_coils_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x15};    // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);
    req.adr = adr;
    req.obj_qty = din_qty;
    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead8DiscreteInputs)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x55};    // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);
    req.adr = adr;
    req.obj_qty = din_qty;
    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead9DiscreteInputs)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 9;
    modbus_byte_count_t expected_byte_count = 2;    // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55, 0x01}; // 0b0001 0101
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);
    req.adr = adr;
    req.obj_qty = din_qty;
    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead16DiscreteInputs)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 16;
    modbus_byte_count_t expected_byte_count = 2;    // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55, 0x55}; // 0b0001 0101
    req.adr = adr;
    req.obj_qty = din_qty;
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX + 1]);
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead17DiscreteInputs)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 17;
    modbus_byte_count_t expected_byte_count = 3;          // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[3] = {0x55, 0x55, 0x01}; // 0b0001 0101
    req.adr = adr;
    req.obj_qty = din_qty;
    mock_set_expected_slave_disc_in_alternately(adr, din_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A};
    req.adr = adr;
    req.obj_qty = reg_qty;
    mock_set_expected_slave_hreg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_holding_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead3HoldingRegisters)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 3;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[3] = {0xA55A, 0x5AA5, 0xA55A};
    req.adr = adr;
    req.obj_qty = reg_qty;

    mock_set_expected_slave_hreg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_holding_reg_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A};
    req.adr = adr;
    req.obj_qty = reg_qty;
    mock_set_expected_slave_input_reg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, RTU_msg->resp.data[MODBUS_RESP_READ_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_READ_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_READ_RESP_LEN + expected_byte_count, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveRead3InputRegisters)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 3;
    modbus_byte_count_t expected_byte_count = 2 * reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[3] = {0xA55A, 0x5AA5, 0xA55A};
    req.adr = adr;
    req.obj_qty = reg_qty;

    mock_set_expected_slave_input_reg_alternately(adr, reg_qty, exp_readed_reg_value[0]);

    modbus_master_read_input_reg_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_w_coil_t resp_buf_coil_expected_value = COIL_ON;
    modbus_coil_disin_t coil_2_write= !!COIL_ON;
    // RTU_msg->rw_data_ptr=(void*)(&coil_2_write);
    req.adr = adr;
    req.coil_2_write=coil_2_write;

    modbus_master_write_single_coil_req(RTU_msg,&req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(resp_buf_coil_expected_value, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteSingleCoilToOnAndCheckCoilStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0001;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    // RTU_msg->rw_data_ptr = (void *)(&coil_2_write);
    req.adr = adr;
    req.coil_2_write = coil_2_write;

    mock_set_all_slave_cails_to_off();
    TEST_ASSERT_EQUAL(0, mock_slave_coil[adr]);

    modbus_master_write_single_coil_req(RTU_msg,&req);
    parse_master_request_and_prepare_resp(RTU_msg);
    TEST_ASSERT_EQUAL(1, mock_slave_coil[adr]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils5Coils)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    modbus_coil_disin_t coils_2_write[5] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    
    req.adr = adr;
    req.obj_qty=coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils5CoilsAndCheckCoilsStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    modbus_coil_disin_t coils_2_write[5] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);

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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    modbus_coil_disin_t coils_2_write[8] = { !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils8CoilsAndCheckCoilsStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    modbus_coil_disin_t coils_2_write[8] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    modbus_coil_disin_t coils_2_write[9] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();

    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils9CoilsAndCheckCoilsStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    modbus_coil_disin_t coils_2_write[9] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_coil_disin_t coils_2_write[16] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF,
                                             !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils16CoilsAndCheckCoilsStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_coil_disin_t coils_2_write[16] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF,
                                             !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_coil_disin_t coils_2_write[16] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF,
                                             !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(coil_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleCoils17CoilsAndCheckCoilsStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 17;
    modbus_coil_disin_t coils_2_write[17] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF,
                                             !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    mock_set_all_slave_cails_to_off();
    modbus_master_write_multiple_coils_req(RTU_msg, &req);
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
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0000;
    modbus_reg_t reg_val = 0x5A5A;
    // RTU_msg->rw_data_ptr=(void*)(&reg_val);
    req.adr = adr;
    req.reg_2_write=reg_val;

    modbus_master_write_single_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX]));
    TEST_ASSERT_EQUAL_HEX16(reg_val, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}
TEST(Slave_PDU_resp, SlaveWriteSingleRegisterAndCheckRegisterValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0001;
    modbus_reg_t reg_val = 0x5A5A;
    // RTU_msg->rw_data_ptr = (void *)(&reg_val);
    req.adr = adr;
    req.reg_2_write = reg_val;

    mock_reset_all_slave_hreg_value();
    
    modbus_master_write_single_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(reg_val, mock_slave_hreg[adr]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_SINGLE_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleRegister3Reg)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0001;
    modbus_reg_t reg_val[3] = {0x5A5A, 0xA5A5, 0x5A5A};
    modbus_data_qty_t reg_qty = 3;
    // RTU_msg->rw_data_ptr = (void *)(reg_val);
    req.adr = adr;
    req.obj_qty =reg_qty;
    req.regs_2_write = reg_val;

    mock_reset_all_slave_hreg_value();

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_HEX16(adr, read_u16_from_buf(RTU_msg->resp.data + MODBUS_RESP_WRITE_ADR_IDX));
    TEST_ASSERT_EQUAL_HEX16(reg_qty, read_u16_from_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX]));
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

TEST(Slave_PDU_resp, SlaveWriteMultipleRegister3regAndCheckRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t adr = 0x0001;
    modbus_reg_t exp_reg_val[3] = {0x5A5A, 0xA5A5, 0x5A5A};
    modbus_data_qty_t reg_qty = 3;
    // RTU_msg->rw_data_ptr = (void *)(exp_reg_val);
    req.adr=adr;
    req.obj_qty=reg_qty;
    req.regs_2_write=exp_reg_val;

    mock_reset_all_slave_hreg_value();

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[0], mock_slave_hreg[adr]);
    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[1], mock_slave_hreg[adr + 1]);
    TEST_ASSERT_EQUAL_HEX16(exp_reg_val[2], mock_slave_hreg[adr + 2]);
    TEST_ASSERT_EQUAL(MODBUS_WRITE_MULTI_RESP_LEN, RTU_msg->resp.len);
}

//
// ToDo odczyt zbyt duzej ilosci coili, disin i rej po strnie slave ilosc obiektow okresla config
