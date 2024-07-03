#include "../unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
    int test_resoult = UnityMain(argc, argv, run_all_tests);

    return test_resoult;
}

static void run_all_tests(void)
{
    RUN_TEST_GROUP(Modbus_Data_test);
    RUN_TEST_GROUP(Master_PDU_req);
    RUN_TEST_GROUP(Slave_PDU_resp);
    RUN_TEST_GROUP(Master_PDU_read);
    RUN_TEST_GROUP(Modbus_CRC_test);
    RUN_TEST_GROUP(Modbus_RTU_test);
    RUN_TEST_GROUP(Modbus_Queue_test);
    // RUN_TEST_GROUP(master_RTU_init_test);
    // RUN_TEST_GROUP(master_RTU_test);
    // RUN_TEST_GROUP(Slave_RTU_init_test);
    // RUN_TEST_GROUP(Slave_RTU_test);
    // RUN_TEST_GROUP(Slave_PDU_exception_code);
}