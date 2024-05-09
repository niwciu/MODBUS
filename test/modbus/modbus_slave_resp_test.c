#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "buf_rw.h"
#include "mock_app_data.h"

uint8_t req_buf[MODBUS_PDU_FRAME_LEN] = {0};
uint8_t resp_buf[MODBUS_PDU_FRAME_LEN] = {0};

TEST_GROUP(Modbus_Slave_Resp);

TEST_SETUP(Modbus_Slave_Resp)
{
    /* Init before every test */
    mock_register_coils_data();
    mock_register_discrete_inputs_data();
    mock_register_input_registers_data();
    mock_register_holding_registers_data();
    
    mock_set_all_cails_to_off();
    mock_set_all_din_to_off();
}

TEST_TEAR_DOWN(Modbus_Slave_Resp)
{
    /* Cleanup after every test */
}

TEST(Modbus_Slave_Resp, SlaveRead5Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x15}; //0b0001 0101
    mock_set_expected_coils_alternately(adr,coil_qty);

    modbus_master_read_coils(req_buf, adr, coil_qty);
    modbus_slave_read_coils(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
}

TEST(Modbus_Slave_Resp, SlaveRead8Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[1] = {0x55};
    mock_set_expected_coils_alternately(adr,coil_qty);

    

    modbus_master_read_coils(req_buf, adr, coil_qty);
    modbus_slave_read_coils(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
}

TEST(Modbus_Slave_Resp, SlaveRead9Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 9;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55,0x01};
    mock_set_expected_coils_alternately(adr,coil_qty);

    modbus_master_read_coils(req_buf, adr, coil_qty);
    modbus_slave_read_coils(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
}

TEST(Modbus_Slave_Resp, SlaveRead16Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 16;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[2] = {0x55,0x55};
    mock_set_expected_coils_alternately(adr,coil_qty);

    modbus_master_read_coils(req_buf, adr, coil_qty);
    modbus_slave_read_coils(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
}

TEST(Modbus_Slave_Resp, SlaveRead17Coils)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t coil_qty = 17;
    modbus_byte_count_t expected_byte_count = 3; // in each byte 8 coil status is reported
    uint8_t exp_readed_coil_value[3] = {0x55,0x55,0x01};
    mock_set_expected_coils_alternately(adr,coil_qty);

    modbus_master_read_coils(req_buf, adr, coil_qty);
    modbus_slave_read_coils(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_coil_value[2], resp_buf[MODBUS_RESP_DATA_IDX+2]);
}

TEST(Modbus_Slave_Resp, SlaveRead5DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x15}; //0b0001 0101
    mock_set_expected_disc_in_alternately(adr,din_qty);

    modbus_master_read_discrete_inputs(req_buf, adr, din_qty);
    modbus_slave_read_discrete_inputs(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
}

TEST(Modbus_Slave_Resp, SlaveRead8DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[1] = {0x55}; //0b0001 0101
    mock_set_expected_disc_in_alternately(adr,din_qty);

    modbus_master_read_discrete_inputs(req_buf, adr, din_qty);
    modbus_slave_read_discrete_inputs(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
}

TEST(Modbus_Slave_Resp, SlaveRead9DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 9;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55,0x01}; //0b0001 0101
    mock_set_expected_disc_in_alternately(adr,din_qty);

    modbus_master_read_discrete_inputs(req_buf, adr, din_qty);
    modbus_slave_read_discrete_inputs(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
}

TEST(Modbus_Slave_Resp, SlaveRead16DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 16;
    modbus_byte_count_t expected_byte_count = 2; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[2] = {0x55,0x55}; //0b0001 0101
    
    mock_set_expected_disc_in_alternately(adr,din_qty);

    modbus_master_read_discrete_inputs(req_buf, adr, din_qty);
    modbus_slave_read_discrete_inputs(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
}

TEST(Modbus_Slave_Resp, SlaveRead17DiscreteInputs)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t din_qty = 17;
    modbus_byte_count_t expected_byte_count = 3; // in each byte 8 coil status is reported
    uint8_t exp_readed_din_value[3] = {0x55,0x55,0x01}; //0b0001 0101
    mock_set_expected_disc_in_alternately(adr,din_qty);

    modbus_master_read_discrete_inputs(req_buf, adr, din_qty);
    modbus_slave_read_discrete_inputs(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[0], resp_buf[MODBUS_RESP_DATA_IDX]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[1], resp_buf[MODBUS_RESP_DATA_IDX+1]);
    TEST_ASSERT_EQUAL_HEX8(exp_readed_din_value[2], resp_buf[MODBUS_RESP_DATA_IDX+2]);
}

TEST(Modbus_Slave_Resp, SlaveReadOneHoldingRegister)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2*reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A}; 

    mock_set_expected_hreg_alternately(adr,reg_qty,exp_readed_reg_value[0]);

    modbus_master_read_holding_reg(req_buf, adr, reg_qty);
    modbus_slave_read_holdin_reg(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX]));
}

TEST(Modbus_Slave_Resp, SlaveRead3HoldingRegisters)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 3;
    modbus_byte_count_t expected_byte_count = 2*reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[3] = {0xA55A,0x5AA5,0xA55A}; 

    mock_set_expected_hreg_alternately(adr,reg_qty,exp_readed_reg_value[0]);

    modbus_master_read_holding_reg(req_buf, adr, reg_qty);
    modbus_slave_read_holdin_reg(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[1], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX+2]));
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[2], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX+4]));
}

TEST(Modbus_Slave_Resp, SlaveReadOneInputRegister)
{
    modbus_adr_t adr = 0x0000;
    modbus_data_qty_t reg_qty = 1;
    modbus_byte_count_t expected_byte_count = 2*reg_qty; // in each byte 8 coil status is reported
    modbus_reg_t exp_readed_reg_value[1] = {0xA55A}; 

    mock_set_expected_input_reg_alternately(adr,reg_qty,exp_readed_reg_value[0]);

    modbus_master_read_input_reg(req_buf, adr, reg_qty);
    modbus_slave_read_input_reg(resp_buf, req_buf);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX]));
}

// TEST(Modbus_Slave_Resp, )
// {
//     modbus_adr_t adr = 0x0000;
//     modbus_data_qty_t reg_qty = 3;
//     modbus_byte_count_t expected_byte_count = 2*reg_qty; // in each byte 8 coil status is reported
//     modbus_reg_t exp_readed_reg_value[3] = {0xA55A,0x5AA5,0xA55A}; 

//     mock_set_expected_input_reg_alternately(adr,reg_qty,exp_readed_reg_value[0]);

//     modbus_master_read_holding_reg(req_buf, adr, reg_qty);
//     modbus_slave_read_holdin_reg(resp_buf, req_buf);

//     TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, resp_buf[MODBUS_FUNCTION_CODE_IDX]);
//     TEST_ASSERT_EQUAL(expected_byte_count, resp_buf[MODBUS_RESP_BYTE_CNT_IDX]);
//     TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[0], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX]));
//     TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[1], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX+2]));
//     TEST_ASSERT_EQUAL_HEX16(exp_readed_reg_value[2], read_u16_from_buf(&resp_buf[MODBUS_RESP_DATA_IDX+4]));
// }

//ToDo odczyt zbyt duzej ilosci coili, disin i rej po strnie slave ilosc obiektow okresla config

//     for (int i=0; i<reg_qty;i++)
    // {
    //     printf("\r\n mock_hreg[%i]= 0x%x",i,mock_hreg[i]);
    // }