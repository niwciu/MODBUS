#include "unity/fixture/unity_fixture.h"
#include "modbus_slave.h"
// #include "tested_module.h"

extern modbus_event_cb_t req_msg_recived_event_cb;
extern modbus_event_cb_t resp_msg_send_event_cb;

static void mock_fun_1(void);

TEST_GROUP(Slave_RTU_event_subscr);

TEST_SETUP(Slave_RTU_event_subscr)
{
    /* Init before every test */
}

TEST_TEAR_DOWN(Slave_RTU_event_subscr)
{
    /* Cleanup after every test */
}

TEST(Slave_RTU_event_subscr, WhenRegisterSlaveReqRecivedEvenCbCalledWithMockFun1AsArgumentThenReqMsgRecivedEventCbIsEqualToMockFun1)
{
    // Given
    // When
    register_slave_req_recived_event_cb(mock_fun_1);
    // Then
    TEST_ASSERT_EQUAL(mock_fun_1,req_msg_recived_event_cb);
}

TEST(Slave_RTU_event_subscr, WhenRegisterSlaveRespSendEvenCbCalledWithMockFun1AsArgumentThenReqMsgRecivedEventCbIsEqualToMockFun1)
{

}

static void mock_fun_1(void)
{

}