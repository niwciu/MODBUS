#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_slave_PDU.h"
#include "modbus_RTU.h"
#include "mock_slave_app_data.h"
#include "buf_rw.h"
#include <memory.h>

typedef struct
{
    modbus_adr_t data_adr;
    modbus_data_qty_t coil_qty;
} confirmation_struct_t;

static modbus_ret_t status;

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
static modbus_msg_t *null_ptr_msg;

extern modbus_coil_disin_t mock_master_coil_data[100];
extern modbus_coil_disin_t mock_master_dis_in[100];
extern modbus_reg_t mock_master_holding_reg[100];
extern modbus_reg_t mock_master_inreg[100];

extern confirmation_struct_t mock_colis_write_confirmation;
extern confirmation_struct_t mock_reg_write_confirmation;

// modbus_ret_t status;

TEST_GROUP(Master_PDU_read);

TEST_SETUP(Master_PDU_read)
{

    mock_register_slave_coils_data();
    mock_register_slave_discrete_inputs_data();
    mock_register_slave_input_registers_data();
    mock_register_slave_holding_registers_data();

    mock_set_all_slave_cails_to_off();
    mock_set_all_slave_din_to_off();
    mock_reset_all_slave_inreg_value();
    mock_reset_all_slave_hreg_value();

    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;
}

TEST_TEAR_DOWN(Master_PDU_read)
{
    /* Cleanup after every test */
}

TEST(Master_PDU_read, MasterReadCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadDiscreteInputsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadInputRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterReadHoldingRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteSingleCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteSingleRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteMultipleCoilsRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, MasterWriteMultipleRegRespWithNullPtrPassedAsArgument)
{
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_slave_resp(null_ptr_msg));
}

TEST(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedFor4CoilsWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 4;
    req.adr=coil_adr;
    req.obj_qty=coils_qty;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    // TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], &mock_master_coil_data[coil_adr], coils_qty);
}

TEST(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedFor8CoilsWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 8;
    req.adr = coil_adr;
    req.obj_qty = coils_qty;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    // TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], &mock_master_coil_data[coil_adr], coils_qty);
}

TEST(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedFor9CoilsWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 9;
    req.adr = coil_adr;
    req.obj_qty = coils_qty;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    // TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], &mock_master_coil_data[coil_adr], coils_qty);
}
TEST(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedFor16CoilsWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 16;
    req.adr = coil_adr;
    req.obj_qty = coils_qty;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    // TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], &mock_master_coil_data[coil_adr], coils_qty);
}

TEST(Master_PDU_read, GivenSlaveReadCoilsResponsRecivedFor17CoilsWhenMasterReadSlaveRespondThenMasterCoilsUpdateToSlaveCoilsValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0001;
    modbus_data_qty_t coils_qty = 17;
    req.adr = coil_adr;
    req.obj_qty = coils_qty;

    mock_set_expected_slave_coils_alternately(coil_adr, coils_qty, !!COIL_ON);
    modbus_master_read_coils_req(RTU_msg, &req);

    parse_master_request_and_prepare_resp(RTU_msg);
    status = modbus_master_read_slave_resp(RTU_msg);

    // TEST_ASSERT_EQUAL(RET_OK, status);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&mock_slave_coil[coil_adr], &mock_master_coil_data[coil_adr], coils_qty);
}

TEST(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWith4DisInWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 4;
    req.adr=disin_adr;
    req.obj_qty=disin_qty;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, mock_master_dis_in + disin_adr, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWith8DisInWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 8;
    req.adr = disin_adr;
    req.obj_qty = disin_qty;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, mock_master_dis_in + disin_adr, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWith9DisInWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 9;
    req.adr = disin_adr;
    req.obj_qty = disin_qty;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, mock_master_dis_in + disin_adr, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWith16DisInWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 16;
    req.adr = disin_adr;
    req.obj_qty = disin_qty;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, mock_master_dis_in + disin_adr, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveReadDisInResponsRecivedWith17DisInWhenMasterReadSlaveRespondThenMasterDisInUpdateToDisInValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t disin_adr = 0x0001;
    modbus_data_qty_t disin_qty = 17;
    req.adr = disin_adr;
    req.obj_qty = disin_qty;

    mock_set_expected_slave_disc_in_alternately(disin_adr, disin_qty);

    modbus_master_read_discrete_inputs_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(mock_slave_dis_in + disin_adr, mock_master_dis_in + disin_adr, disin_qty);
}

TEST(Master_PDU_read, GivenSlaveReadInputRegisterResponsWith4InRegRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 4;
    req.adr=in_reg_adr;
    req.obj_qty=in_reg_qty;
    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, mock_master_inreg + in_reg_adr, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveReadInputRegisterResponsWith8InRegRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 8;
    req.adr = in_reg_adr;
    req.obj_qty = in_reg_qty;
    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, mock_master_inreg + in_reg_adr, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveReadInputRegisterResponsWith9InRegRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 9;
    req.adr = in_reg_adr;
    req.obj_qty = in_reg_qty;
    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, mock_master_inreg + in_reg_adr, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveReadInputRegisterResponsWith16InRegRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 16;
    req.adr = in_reg_adr;
    req.obj_qty = in_reg_qty;
    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, mock_master_inreg + in_reg_adr, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveReadInputRegisterResponsWith17InRegRecivedWhenMasterReadSlaveRespondThenMasterInRegUpdateToSlaveInRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t in_reg_adr = 0x0001;
    modbus_data_qty_t in_reg_qty = 17;
    req.adr = in_reg_adr;
    req.obj_qty = in_reg_qty;
    mock_set_expected_slave_input_reg_alternately(in_reg_adr, in_reg_qty, 0x5A5A);

    modbus_master_read_input_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_inreg + in_reg_adr, mock_master_inreg + in_reg_adr, in_reg_qty);
}

TEST(Master_PDU_read, GivenSlaveReadHoldingRegisterResponsWith6HolRegRecivedWhenMasterReadSlaveRespondThenMasterHolRegUpdateToSlaveHolRegValue)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t hreg_adr = 0x0003;
    modbus_data_qty_t hreg_qty = 6;
    req.adr=hreg_adr;
    req.obj_qty=hreg_qty;

    mock_set_expected_slave_hreg_alternately(hreg_adr, hreg_qty, 0x5A5A);

    modbus_master_read_holding_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16_ARRAY(mock_slave_hreg + hreg_adr, mock_master_holding_reg + hreg_adr, hreg_qty);
}

TEST(Master_PDU_read, GivenSlaveWriteSingleCoilResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect)
{
    static req_input_param_struct_t req= {0};
    modbus_adr_t coil_adr = 0x0003;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    // RTU_msg->rw_data_ptr = (void *)(&coil_2_write);
    req.adr=coil_adr;
    req.coil_2_write=coil_2_write;

    modbus_master_write_single_coil_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);

    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(coil_adr,mock_colis_write_confirmation.data_adr );
    TEST_ASSERT_EQUAL_INT16( 1,mock_colis_write_confirmation.coil_qty);
}

// TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeAndWrongOutputAdressWhenMasterWriteSingleCoilRespThenMasterWriteSingleCoilReturnErrorOutputAddress)
// {
//     modbus_adr_t coil_adr = 0x0003;
//     modbus_coil_disin_t coil_2_write = !!COIL_ON;
//     RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

//     modbus_master_write_single_coil_req(RTU_msg, coil_adr);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);
//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectOutputAdrAndIncorrectCoilValueWhenMasterWriteSignleCoilThenMasterWriteSingleCoilRetValueError)
// {
//     modbus_adr_t coil_adr = 0x0003;
//     modbus_coil_disin_t coil_2_write = !!COIL_ON;
//     RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

//     modbus_master_write_single_coil_req(RTU_msg, coil_adr);

//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], !!COIL_OFF);
//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleCoilThenMasterWriteSingleCoilReturnErrorFuncCode)
// {
//     modbus_adr_t coil_adr = 0x0003;
//     modbus_coil_disin_t coil_2_write = !!COIL_ON;
//     RTU_msg->rw_data_ptr = (void *)(&coil_2_write);

//     modbus_master_write_single_coil_req(RTU_msg, coil_adr);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++;
//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
// }

TEST(Master_PDU_read, GivenSlaveWriteSingleReglResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect)
{
    modbus_adr_t reg_adr = 0x0005;
    modbus_reg_t reg_val = 0x1234;
    // RTU_msg->rw_data_ptr = (void *)(&reg_val);
    static req_input_param_struct_t req = {0};
    req.adr = reg_adr;
    req.reg_2_write = reg_val;

    modbus_master_write_single_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(reg_adr, mock_reg_write_confirmation.data_adr);
    TEST_ASSERT_EQUAL_INT16(1, mock_reg_write_confirmation.coil_qty);
}

// TEST(Master_PDU_read, GivenSlaveRespondWithCorrectFunctionCodeWhenMasterWriteSingleRegisterRespAndAddresIsIncorrectThenMasterWriteSingleRegisterRespRetErrorOutAddr)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_reg_t reg_val = 0x1234;
//     RTU_msg->rw_data_ptr = (void *)(&reg_val);

//     modbus_master_write_single_reg_req(RTU_msg, reg_adr);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);
//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeWhenMasterWriteSingleRegisterThenMasterWriteSingleRegisterReturnErrorFuncCode)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_reg_t reg_val = 0x1234;
//     RTU_msg->rw_data_ptr = (void *)(&reg_val);

//     modbus_master_write_single_reg_req(RTU_msg, reg_adr);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // its 2 byte data so im incrementing LSB

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithIncorectFunctionCodeAndCorrectOutputAdrAndIncorrectRegValueWhenMasterWriteSignleRegThenMasterWriteSingleRegRetValueError)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_reg_t reg_val = 0x1234;
//     RTU_msg->rw_data_ptr = (void *)(&reg_val);

//     modbus_master_write_single_reg_req(RTU_msg, reg_adr);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val + 1);

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_SINGLE_OUT_VAL, modbus_master_read_slave_resp(RTU_msg));
// }

TEST(Master_PDU_read, GivenSlaveWriteMultipleCoilResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t coil_adr = 0x0005;
    modbus_data_qty_t coil_qty = 10;
    modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr = coil_adr;
    req.obj_qty = coil_qty;
    req.coils_2_write = coils_2_write;

    modbus_master_write_multiple_coils_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_INT16(coil_adr, mock_colis_write_confirmation.data_adr);
    TEST_ASSERT_EQUAL_INT16(coil_qty, mock_colis_write_confirmation.coil_qty);
    // TEST_ASSERT_EQUAL_INT16(RET_OK, modbus_master_read_slave_resp(RTU_msg));
}

// TEST(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetErrorFuncCode)
// {
//     modbus_adr_t coil_adr = 0x0005;
//     modbus_data_qty_t coil_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutAdrError)
// {
//     modbus_adr_t coil_adr = 0x0005;
//     modbus_data_qty_t coil_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], coil_adr + 1);

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiCoilsRespThenRetOutQtyError)
// {
//     modbus_adr_t coil_adr = 0x0005;
//     modbus_data_qty_t coil_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_coils_req(RTU_msg, coil_adr, coil_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coil_qty + 1);

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_read_slave_resp(RTU_msg));
// }

TEST(Master_PDU_read, GivenSlaveWriteMultipleReglResponsRecivedWhenMasterReadSlaveRespondThenWrieConfirmationDataAreCorrect)
{
    static req_input_param_struct_t req = {0};
    modbus_adr_t reg_adr = 0x0005;
    modbus_data_qty_t reg_qty = 10;
    modbus_reg_t reg_2_write[10] = {0x5A5A,0xA5A5,0x5A5A,0xA5A5,0x5A5A,0xA5A5,0x5A5A,0xA5A5,0x5A5A,0xA5A5};
    // RTU_msg->rw_data_ptr = (void *)(coils_2_write);
    req.adr=reg_adr;
    req.obj_qty=reg_qty;
    req.regs_2_write = reg_2_write;

    modbus_master_write_multiple_reg_req(RTU_msg, &req);
    parse_master_request_and_prepare_resp(RTU_msg);
    modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL_HEX16(reg_adr, mock_reg_write_confirmation.data_adr);
    TEST_ASSERT_EQUAL_INT16(reg_qty, mock_reg_write_confirmation.coil_qty);
}

// TEST(Master_PDU_read, GivenSlaveRespondWithIncorrectFunctionCodeAndCorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetErrorFuncCode)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_data_qty_t reg_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     RTU_msg->resp.data[MODBUS_FUNCTION_CODE_IDX]++; // change Function Code in resp

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_REQ_RESP_FUN_CODE_MISMATCH, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndIncorrectStartAdrAndCorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutAdrError)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_data_qty_t reg_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_ADR_IDX], reg_adr + 1);

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_OUT_ADR, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read, GivenSlaveRespondWithCorectFunctionCodeAndCorrectStartAdrAndIncorrectOutputQtyWhenMasterWriteMultiRegRespThenRetOutQtyError)
// {
//     modbus_adr_t reg_adr = 0x0005;
//     modbus_data_qty_t reg_qty = 10;
//     modbus_coil_disin_t coils_2_write[10] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
//     RTU_msg->rw_data_ptr = (void *)(coils_2_write);

//     modbus_master_write_multiple_reg_req(RTU_msg, reg_adr, reg_qty);
//     parse_master_request_and_prepare_resp(RTU_msg);

//     write_u16_to_buf(&RTU_msg->resp.data[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], reg_qty + 1);

//     TEST_ASSERT_EQUAL_INT16(RET_ERROR_WRITE_MULTI_OUT_QTY, modbus_master_read_slave_resp(RTU_msg));
// }

// TEST(Master_PDU_read_exception_code, WhenRegisterModbusErrorCbCalledThenModbusErrorCbEqualToRegisteredFunctionPointer)
// {

//     TEST_ASSERT_EQUAL(&error_test_func, modbus_error_callback);
// }

TEST(Master_PDU_read, GivenMasterReadCoilReqSendedAndWhenFunctionCodeWithExceptionCodeMaskRecivedThenModbusMasterReadSlaveReturnExceptionCodeRecivedStatus)
{
    static req_input_param_struct_t req = {0};
    modbus_device_ID_t Slave_ID = 0x03;
    modbus_buf_t read_coil_ex_code_01_resp[] = {0x03, 0x81, 0x01, 0x20, 0x50};
    modbus_buf_size_t buf_len = sizeof(read_coil_ex_code_01_resp) / sizeof(modbus_buf_t);
    // modbus_adr_t coil_adr = 0x0001;
    // modbus_data_qty_t coils_qty = 4;
    req.adr = 0x0001;
    req.obj_qty = 4;
    // generating correct req RTU_msg
    modbus_master_read_coils_req(RTU_msg, &req);
    modbus_RTU_send(RTU_msg->req.data, &RTU_msg->req.len, Slave_ID);
    // generate fun code 01 resp RTU msg
    memcpy(RTU_msg->resp.data, read_coil_ex_code_01_resp, buf_len);
    RTU_msg->resp.len = buf_len;

    status = modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL(RET_ERROR_EXCEPTION_CODE_RECIVED, status);
}

TEST(Master_PDU_read, GivenMasterReadDisInReqSendedWhenFunctionCodeWithExceptionCodeMaskRecivedThenModbusMasterReadSlaveReturnExceptionCodeRecivedStatus)
{
    static req_input_param_struct_t req ={0};
    modbus_device_ID_t Slave_ID = 0x03;
    modbus_buf_t read_dis_in_ex_code_02_resp[] = {0x03, 0x82, 0x02, 0x60, 0xA1};
    modbus_buf_size_t buf_len = sizeof(read_dis_in_ex_code_02_resp) / sizeof(modbus_buf_t);
    // modbus_adr_t coil_adr = 0x0001;
    // modbus_data_qty_t coils_qty = 4;
    req.adr=0x0001;
    req.obj_qty=4;

    // generating correct req RTU_msg
    modbus_master_read_coils_req(RTU_msg, &req);
    modbus_RTU_send(RTU_msg->req.data, &RTU_msg->req.len, Slave_ID);
    // generate fun code 02 resp RTU msg
    memcpy(RTU_msg->resp.data, read_dis_in_ex_code_02_resp, buf_len);
    RTU_msg->resp.len = buf_len;

    status = modbus_master_read_slave_resp(RTU_msg);

    TEST_ASSERT_EQUAL(RET_ERROR_EXCEPTION_CODE_RECIVED, status);
}

// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//
// TEST(Master_PDU_read, )
// {
//     TEST_FAIL_MESSAGE ("ADDED NEW TEST");
// }
//
//