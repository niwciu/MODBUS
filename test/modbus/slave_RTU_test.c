#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
#include "modbus_master_PDU.h"
#include "mock_slave_driver_interface.h"
// #include <mem.h>
// #include "modbus_public_type.h"

// #include "tested_module.h"

TEST_GROUP(Slave_RTU_test);

extern modbus_slave_state_t slave_manager_state_machine;
extern modbus_msg_t *slave_msg_buf;

modbus_device_ID_t device_ID = 0x12;
baud_t baud = 38400;
parity_t parity = ODD;
modbus_mode_t mode = RTU;

TEST_SETUP(Slave_RTU_test)
{
    /* Init before every test */
    modbus_slave_init(mode,baud,parity,device_ID);
}

TEST_TEAR_DOWN(Slave_RTU_test)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_test, GivenModbusSlaveInitAndReadCoilsReqWithIncorectSlaveIdRecivedWhenTimer1_5CharTrigerThenSkiMsgGoBackToIdleState)
{
    modbus_adr_t coil_adr=0x0001;
    modbus_data_qty_t coils_qty=2;
    
    modbus_master_read_coils_req(slave_msg_buf,coil_adr,coils_qty);

    mock_msg_rx_done_cb(); 
    TEST_ASSERT_EQUAL(MODBUS_SLAVE_IDLE, slave_manager_state_machine);
    TEST_ASSERT_EQUAL(0,slave_msg_buf->req.len);
}