#include "unity/fixture/unity_fixture.h"
#include "modbus_PDU.h"
#include "mock_modbus_data_interface.h"

uint8_t req_PDU[MODBUS_PDU_FRAME_LEN] = {0};
uint8_t resp_PDU[MODBUS_PDU_FRAME_LEN] = {0};

static void set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty);

TEST_GROUP(Modbus_Slave_Resp);

TEST_SETUP(Modbus_Slave_Resp)
{
    /* Init before every test */
    mock_set_all_cails_to_off();
}

TEST_TEAR_DOWN(Modbus_Slave_Resp)
{
    /* Cleanup after every test */
}

TEST(Modbus_Slave_Resp, SlaveRead5Coils)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t coil_qty = 5;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported

    set_expected_coils_alternately(adr,coil_qty);

    modbus_data_t exp_readed_coil_value = 0x0015; // 0b10101

    modbus_master_read_coils(req_PDU, adr, coil_qty);
    modbus_slave_read_coils(resp_PDU, req_PDU);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_PDU[0]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_PDU[1]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value, resp_PDU[2]);
}

TEST(Modbus_Slave_Resp, SlaveRead8Coils)
{
    modbus_adr_t adr = 0x0003;
    modbus_data_qty_t coil_qty = 8;
    modbus_byte_count_t expected_byte_count = 1; // in each byte 8 coil status is reported

    set_expected_coils_alternately(adr,coil_qty);

    modbus_data_t exp_readed_coil_value = 0x0055;

    modbus_master_read_coils(req_PDU, adr, coil_qty);
    modbus_slave_read_coils(resp_PDU, req_PDU);

    TEST_ASSERT_EQUAL_UINT8(MODBUS_READ_COILS_FUNC_CODE, resp_PDU[0]);
    TEST_ASSERT_EQUAL(expected_byte_count, resp_PDU[1]);
    TEST_ASSERT_EQUAL_HEX16(exp_readed_coil_value, resp_PDU[2]);
}

//

static void set_expected_coils_alternately(modbus_adr_t start_adr, modbus_data_qty_t qty)
{
    modbus_r_coil_t current_state = 1;
    for (modbus_data_qty_t i = 0; i < qty; i++)
    {
        mock_coil[start_adr+i]=current_state;
        current_state=!current_state;
    }
}