#include "unity/fixture/unity_fixture.h"
#include "modbus_master_PDU.h"
#include "modbus_type.h"
#include "buf_rw.h"
// #include "mock_master_app_data.h"

static modbus_buf_t req_RTU_buf[MODBUS_RTU_BUFFER_SIZE];
static modbus_buf_t resp_RTU_buf[MODBUS_RTU_BUFFER_SIZE];

static modbus_msg_t modbus_msg;
static modbus_msg_t *RTU_msg = &modbus_msg;
static modbus_msg_t *null_ptr_msg = NULL;

TEST_GROUP(Master_PDU_req);

TEST_SETUP(Master_PDU_req)
{
    /* Init before every test */
    RTU_msg->req.data = req_RTU_buf;
    RTU_msg->resp.data = resp_RTU_buf;

    // mock_register_master_coils_data();
    // mock_register_master_discrete_inputs_data();
    // mock_register_master_input_registers_data();
    // mock_register_master_holding_registers_data();
}

TEST_TEAR_DOWN(Master_PDU_req)
{

    /* Cleanup after every test */
}

TEST(Master_PDU_req, ReadSingleHoldingRegisterRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0001;
    req.obj_qty=1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_holding_reg_req(null_ptr_msg, &req));
}

TEST(Master_PDU_req, ReadSingleHoldingRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0030;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0030;
    req.obj_qty = 1;

    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
}

TEST(Master_PDU_req, ReadMaxQtyHoldingRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0030;
    // static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY;
    req.adr = 0x0030;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg,&req);

    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
}

TEST(Master_PDU_req, ReadHoldingRegisterMaxQtyPlus1Request)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0030;
    // static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY + 1;
    req.adr = 0x0030;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY+1;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, Read0QtyHoldingRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0030;
    // static modbus_data_qty_t len = 0;
    req.adr = 0x0030;
    req.obj_qty = 0;
    modbus_reg_t status = modbus_master_read_holding_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadSingleInputRegisterRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0001;
    req.obj_qty = 1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_input_reg_req(null_ptr_msg, &req));
}

TEST(Master_PDU_req, ReadSingleInputRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0005;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0005;
    req.obj_qty = 1;
    modbus_reg_t status = modbus_master_read_input_reg_req(RTU_msg,&req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtyInputRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0005;
    // static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY;
    req.adr = 0x0005;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadInputRegisterMaxQtyPlus1Request)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0005;
    // static modbus_data_qty_t len = MODBUS_MAX_READ_REG_QTY + 1;
    req.adr = 0x0005;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY+1;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroInputRegisterRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0005;
    // static modbus_data_qty_t len = 0;
    req.adr = 0x0005;
    req.obj_qty = 0;
    modbus_ret_t status = modbus_master_read_input_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadDiscreteInputsRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0001;
    req.obj_qty = 1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_discrete_inputs_req(null_ptr_msg, &req));
}

TEST(Master_PDU_req, ReadDiscreteInputsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = 5;
    req.adr = 0x0003;
    req.obj_qty = 5;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtyDiscreteInputsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;
    req.adr = 0x0003;
    req.obj_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadMaxQtPlus1DiscreteInputsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY + 1;
    req.adr = 0x0003;
    req.obj_qty = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY+1;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroDiscreteInputsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = 0;
    req.adr = 0x0003;
    req.obj_qty = 0;
    modbus_ret_t status = modbus_master_read_discrete_inputs_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadCoilsRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    req.adr = 0x0001;
    req.obj_qty = 1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_read_coils_req(null_ptr_msg, &req));
}

TEST(Master_PDU_req, ReadCoilsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = 5;
    req.adr = 0x0003;
    req.obj_qty = 5;

    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadCoilsMaxQtyRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = MODBUS_MAX_READ_COILS_QTY;
    req.adr = 0x0003;
    req.obj_qty = MODBUS_MAX_READ_COILS_QTY;
    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, &req);
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, ReadCoilsMaxQtyPlus1Request)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    req.adr = 0x0003;
    req.obj_qty = MODBUS_MAX_READ_COILS_QTY+1;

    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, ReadZeroCoilsRequest)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_data_qty_t input_qty = 0;
    req.adr=0x0003;
    req.obj_qty=0;
    modbus_ret_t status = modbus_master_read_coils_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteSingleRegisterRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    // static modbus_reg_t reg = 0;
    // RTU_msg->rw_data_ptr = &reg;
    req.adr=0x0003;
    req.reg_2_write=0;
    modbus_ret_t status = modbus_master_write_single_reg_req(null_ptr_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_NULL_PTR_ERROR, status);
}

TEST(Master_PDU_req, WriteSingleRegister)
{
    static req_input_param_struct_t req = {0};
    static modbus_adr_t adr = 0x0009;
    modbus_reg_t reg_value = 0x0012;
    modbus_ret_t status;
    req.adr=adr;
    req.reg_2_write=reg_value;
    // RTU_msg->rw_data_ptr=(void*)(&reg_value);
    status = modbus_master_write_single_reg_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.reg_2_write, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteSingleCoilRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0003;
    req.adr=0x0003;
    modbus_ret_t status = modbus_master_write_single_coil_req(null_ptr_msg, &req);

    TEST_ASSERT_EQUAL_INT16(RET_NULL_PTR_ERROR, status);
}

TEST(Master_PDU_req, WriteSingleCoilOn)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0009;
    modbus_ret_t status;
    modbus_coil_disin_t coil_2_write = !!COIL_ON;
    req.adr=0x0009;
    req.coil_2_write=coil_2_write;
    // RTU_msg->rw_data_ptr = &coil_2_write;
    status = modbus_master_write_single_coil_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(COIL_ON, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_INT8(MODBUS_READ_REQUEST_LEN, RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleRegistersRequestWithNullPtrModbusMasgPassed)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    req.adr=0x0001;
    req.obj_qty=1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_write_multiple_reg_req(null_ptr_msg, &req));
}

TEST(Master_PDU_req, WriteMultipleRegisters)
{
    static req_input_param_struct_t req = {0};
    // static modbus_adr_t adr = 0x0008;
    // static modbus_data_qty_t reg_qty = 5;
    modbus_reg_t reg_values[5] = {0x5A5A, 0xA5A5, 0x5A5A, 0xA5A5, 0x5A5A};
    req.adr=0x0008;
    req.obj_qty=5;
    req.regs_2_write=reg_values;
    // RTU_msg->rw_data_ptr = reg_values;
    modbus_ret_t status = modbus_master_write_multiple_reg_req(RTU_msg, &req);
    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8((req.obj_qty * 2), RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX));
    TEST_ASSERT_EQUAL_HEX16(0xA5A5, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 2));
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 4));
    TEST_ASSERT_EQUAL_HEX16(0xA5A5, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 6));
    TEST_ASSERT_EQUAL_HEX16(0x5A5A, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + 8));
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + (req.obj_qty * 2), RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMaxQtyMultipleRegisters)
{
    static modbus_adr_t adr = 0x0080;
    static modbus_data_qty_t reg_qty = MODBUS_MAX_WRITE_REG_QTY;
    static req_input_param_struct_t req = {0};
    modbus_reg_t reg_2_write[MODBUS_MAX_WRITE_REG_QTY] = {0};
    modbus_ret_t status;
    req.adr = adr;
    req.obj_qty = reg_qty;
    req.regs_2_write = reg_2_write;
    // RTU_msg->rw_data_ptr = (void *)(reg_2_write);
    status = modbus_master_write_multiple_reg_req(RTU_msg, &req);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + (req.obj_qty * 2), RTU_msg->req.len);
    TEST_ASSERT_EQUAL_INT16(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleRegistersMaxQtyPlus1)
{
    // static modbus_adr_t adr = 0x0080;
    // static modbus_data_qty_t reg_qty = MODBUS_MAX_READ_REG_QTY + 1;
    static req_input_param_struct_t req = {0};
    modbus_reg_t reg_2_write[MODBUS_MAX_WRITE_REG_QTY + 1] = {0};
    modbus_ret_t status;
    req.adr = 0x0080;
    req.obj_qty = MODBUS_MAX_READ_REG_QTY + 1;
    req.regs_2_write = reg_2_write;
    // RTU_msg->rw_data_ptr = (void*)(reg_2_write);

    status = modbus_master_write_multiple_reg_req(RTU_msg, &req);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteZeroMultipleRegisters)
{
    // static modbus_adr_t adr = 0x0080;
    // static modbus_data_qty_t reg_qty = 0;
    static req_input_param_struct_t req = {0};
    modbus_ret_t status;
    req.adr = 0x0080;
    req.obj_qty = 0;
    req.regs_2_write=NULL;
    // RTU_msg->rw_data_ptr = NULL;
    status = modbus_master_write_multiple_reg_req(RTU_msg,&req);
    TEST_ASSERT_EQUAL_INT16(RET_ERROR, status);
}

TEST(Master_PDU_req, Write5MultipleCoilsRequestWithNullPtrModbusMasgPassed)
{
    // static modbus_adr_t adr = 0x0001;
    // static modbus_data_qty_t len = 1;
    static req_input_param_struct_t req={0};
    req.adr=0x0001;
    req.obj_qty=1;
    TEST_ASSERT_EQUAL(RET_NULL_PTR_ERROR, modbus_master_write_multiple_coils_req(null_ptr_msg,&req));
}

TEST(Master_PDU_req, Write5MultipleCoils)
{
    req_input_param_struct_t req;

    modbus_buf_t expected_coil_states_in_PDU[1] = {0x15};
    modbus_byte_count_t expected_byte_cnt = 1;
    modbus_ret_t status;
    modbus_coil_disin_t coils_2_write[5] = {!!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON};
    req.adr = 0x0009;
    req.obj_qty = 5;
    req.coils_2_write = coils_2_write;
    
    status = modbus_master_write_multiple_coils_req(RTU_msg,&req );

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt, RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_coil_states_in_PDU, RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX, expected_byte_cnt);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + expected_byte_cnt, RTU_msg->req.len);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Master_PDU_req, Write16MultipleCoils)
{
    req_input_param_struct_t req;

    modbus_buf_t expected_coil_states_in_PDU[2] = {0x55,0x55};
    modbus_byte_count_t expected_byte_cnt = 2;
    modbus_ret_t status;
    modbus_coil_disin_t coils_2_write[16] = {   !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF,
                                                !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF, !!COIL_ON, !!COIL_OFF};
    req.adr = 0x0009;
    req.obj_qty = 16;
    req.coils_2_write = coils_2_write;

    status = modbus_master_write_multiple_coils_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt, RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_coil_states_in_PDU, RTU_msg->req.data + MODBUS_REQUEST_WRITE_MULTI_DATA_IDX, expected_byte_cnt);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + expected_byte_cnt, RTU_msg->req.len);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Master_PDU_req, WriteMaxQtyMultipleCoils)
{
    static req_input_param_struct_t req;
    modbus_byte_count_t expected_byte_cnt = 246;
    modbus_ret_t status;
    modbus_coil_disin_t coils_2_write[MODBUS_MAX_WRITE_COILS_QTY] = {!!COIL_ON};
    req.adr = 0x0000;
    req.obj_qty = MODBUS_MAX_WRITE_COILS_QTY;
    req.coils_2_write = coils_2_write;
    status = modbus_master_write_multiple_coils_req(RTU_msg, &req);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE, RTU_msg->req.data[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL_UINT16(req.adr, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_ADR_IDX));
    TEST_ASSERT_EQUAL_UINT16(req.obj_qty, read_u16_from_buf(RTU_msg->req.data + MODBUS_REQUEST_QTY_IDX));
    TEST_ASSERT_EQUAL_UINT8(expected_byte_cnt, RTU_msg->req.data[MODBUS_REQUEST_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_INT8(MODBUS_WRITE_MULTI_REQUEST_LEN + expected_byte_cnt, RTU_msg->req.len);
    TEST_ASSERT_EQUAL(RET_OK, status);
}

TEST(Master_PDU_req, WriteMultipleCoilsMaxQtyPlus1)
{
    static modbus_adr_t adr = 0x0000;
    static modbus_data_qty_t coils_qty = MODBUS_MAX_READ_COILS_QTY + 1;
    static req_input_param_struct_t req;
    modbus_ret_t status;
    modbus_coil_disin_t coils_2_write[MODBUS_MAX_WRITE_COILS_QTY + 1] = {!!COIL_ON};
    req.adr = adr;
    req.obj_qty = coils_qty;
    req.coils_2_write = coils_2_write;
    status = modbus_master_write_multiple_coils_req(RTU_msg, &req );

    TEST_ASSERT_EQUAL(RET_ERROR, status);
}

TEST(Master_PDU_req, WriteZeroMultipleCoils)
{
    // static modbus_adr_t adr = 0x0000;
    // static modbus_data_qty_t coils_qty = 0;
    static req_input_param_struct_t req ={0};
    modbus_ret_t status;

    status = modbus_master_write_multiple_coils_req(RTU_msg, &req);
    TEST_ASSERT_EQUAL(RET_ERROR, status);
}
// //
// // testy na zerową ilość rejestrów coili do odczytu zapisu.