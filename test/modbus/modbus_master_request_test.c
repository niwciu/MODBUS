#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"

uint8_t PDU_frame[MODBUS_PDU_FRAME_LEN];

static uint16_t read_u16_from_buf(uint8_t *buf);

TEST_GROUP(Modbus_Master_Requests);

TEST_SETUP(Modbus_Master_Requests)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Modbus_Master_Requests)
{
    /* Cleanup after every test */
}

TEST(Modbus_Master_Requests, ReadSingleHoldingRegisterRequest)
{
    modbus_adr_t adr=0x0030;
    modbus_data_qty_t len=1;

    modbus_master_read_holding_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_UINT8(READ_HOLDING_REGISTERS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(len,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadMaxQtyHoldingRegisterRequest)
{
    modbus_adr_t adr=0x0030;
    modbus_data_qty_t len=MODBUS_MAX_REG_RW_QTY;
    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL(RET_OK,status);
} 

TEST(Modbus_Master_Requests, ReadHoldingRegisterMaxQtyPlus1Request)
{
    modbus_adr_t adr=0x0030;
    modbus_data_qty_t len=MODBUS_MAX_REG_RW_QTY+1;
    modbus_reg_t status = modbus_master_read_holding_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_INT8(RET_ERROR,status);
}

TEST(Modbus_Master_Requests, ReadSingleInputRegisterRequest)
{
    modbus_adr_t adr=0x0005;
    modbus_data_qty_t len=1;

    modbus_master_read_input_reg(PDU_frame,adr,len);
  
    TEST_ASSERT_EQUAL_UINT8(READ_INPUT_REGISTERS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(len,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadMaxQtyInputRegisterRequest)
{
    modbus_adr_t adr=0x0005;
    modbus_data_qty_t len=MODBUS_MAX_REG_RW_QTY;
    modbus_ret_t status = modbus_master_read_input_reg(PDU_frame,adr,len);
    
    TEST_ASSERT_EQUAL_INT8(RET_OK,status);
}
TEST(Modbus_Master_Requests, ReadInputRegisterMaxQtyPlus1Request)
{
    modbus_adr_t adr=0x0005;
    modbus_data_qty_t len=MODBUS_MAX_REG_RW_QTY+1;
    modbus_ret_t status = modbus_master_read_input_reg(PDU_frame,adr,len);
    
    TEST_ASSERT_EQUAL_INT8(RET_ERROR,status);
}

TEST(Modbus_Master_Requests, ReadDiscreteInputsRequest)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=5;

    modbus_master_read_discrete_inputs(PDU_frame,adr,input_qty);
  
    TEST_ASSERT_EQUAL_UINT8(READ_DISCRETE_INPUTS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(input_qty,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadMaxQtyDiscreteInputsRequest)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=MODBUS_MAX_DISCRETE_INPUTS_READ_QTY;

    modbus_master_read_discrete_inputs(PDU_frame,adr,input_qty);
  
    TEST_ASSERT_EQUAL_UINT8(READ_DISCRETE_INPUTS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(input_qty,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadMaxQtPlus1DiscreteInputsRequest)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=MODBUS_MAX_DISCRETE_INPUTS_READ_QTY+1;
    modbus_ret_t status = modbus_master_read_discrete_inputs(PDU_frame,adr,input_qty);
  
    TEST_ASSERT_EQUAL(RET_ERROR,status);
}

TEST(Modbus_Master_Requests, ReadCoilsRequest)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=5;

    modbus_master_read_coils(PDU_frame,adr,input_qty);
  
    TEST_ASSERT_EQUAL_UINT8(READ_COILS,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(input_qty,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, ReadCoilsMaxQtyRequest)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=MODBUS_MAX_COILS_READ_QTY;
    modbus_ret_t status= modbus_master_read_coils(PDU_frame,adr,input_qty);
    TEST_ASSERT_EQUAL(RET_OK,status);
}


TEST(Modbus_Master_Requests, ReadCoilsMaxQtyPlus1Request)
{
    modbus_adr_t adr=0x0003;
    modbus_data_qty_t input_qty=MODBUS_MAX_COILS_READ_QTY+1;
    modbus_ret_t status=modbus_master_read_coils(PDU_frame,adr,input_qty);

    TEST_ASSERT_EQUAL(RET_ERROR,status);
}

TEST(Modbus_Master_Requests, WriteSingleRegister)
{
    modbus_adr_t adr=0x0009;
    modbus_reg_t value=0x0012;

    modbus_master_write_single_reg(PDU_frame,adr,value);
  
    TEST_ASSERT_EQUAL_UINT8(WRITE_SINGLE_REGISTER,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(value,read_u16_from_buf(PDU_frame+3));
}

TEST(Modbus_Master_Requests, WriteSingleCoilOn)
{
    modbus_adr_t adr=0x0009;
    modbus_coil_t coil_state=COIL_ON;

    modbus_master_write_single_coil(PDU_frame,adr,coil_state);
  
    TEST_ASSERT_EQUAL_UINT8(WRITE_SINGLE_REGISTER,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(coil_state,read_u16_from_buf(PDU_frame+3));

}

TEST(Modbus_Master_Requests, WriteMultipleRegisters)
{
    modbus_adr_t adr=0x0080;
    modbus_reg_t values[5]={1,2,3,4,5};
    modbus_data_qty_t reg_qty=5;

    modbus_master_write_multiple_reg(PDU_frame,adr,reg_qty,values);
  
    TEST_ASSERT_EQUAL_UINT8(WRITE_MULTIPLE_REGISTER,PDU_frame[0]);
    TEST_ASSERT_EQUAL_UINT16(adr,read_u16_from_buf(PDU_frame+1));
    TEST_ASSERT_EQUAL_UINT16(reg_qty,read_u16_from_buf(PDU_frame+3));
    TEST_ASSERT_EQUAL_UINT8((reg_qty*2),PDU_frame[5]);
    TEST_ASSERT_EQUAL_UINT16(values[0],read_u16_from_buf(PDU_frame+6));
    TEST_ASSERT_EQUAL_UINT16(values[1],read_u16_from_buf(PDU_frame+8));
    TEST_ASSERT_EQUAL_UINT16(values[2],read_u16_from_buf(PDU_frame+10));
    TEST_ASSERT_EQUAL_UINT16(values[3],read_u16_from_buf(PDU_frame+12));
    TEST_ASSERT_EQUAL_UINT16(values[4],read_u16_from_buf(PDU_frame+14));

}

TEST(Modbus_Master_Requests, WriteMaxQtyMultipleRegisters)
{
    modbus_adr_t adr=0x0080;
    modbus_reg_t values[MODBUS_MAX_REG_RW_QTY]={0x5A5A};
    modbus_data_qty_t reg_qty=MODBUS_MAX_REG_RW_QTY;
    modbus_ret_t status;

    status=modbus_master_write_multiple_reg(PDU_frame,adr,reg_qty,values);
    TEST_ASSERT_EQUAL(RET_OK,status);
}

TEST(Modbus_Master_Requests, WriteMultipleRegistersMaxQtyPlus1)
{
    modbus_adr_t adr=0x0080;
    modbus_reg_t values[MODBUS_MAX_REG_RW_QTY+1]={0x5A5A};
    modbus_data_qty_t reg_qty=MODBUS_MAX_REG_RW_QTY+1;
    modbus_ret_t status;

    status=modbus_master_write_multiple_reg(PDU_frame,adr,reg_qty,values);
    TEST_ASSERT_EQUAL(RET_ERROR,status);
}

static uint16_t read_u16_from_buf(uint8_t *buf)
{
    return (uint16_t)((buf[0]<<8) | buf[1]);
}

// zacznij od uzupełnienia testów dla max i ponad max ilości rejestrów dla read input register
// testy na zerową ilość rejestrów coili do odczytu zapisu. 