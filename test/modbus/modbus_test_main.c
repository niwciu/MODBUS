#include "../unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
   int test_resoult =UnityMain(argc, argv, run_all_tests);

   return test_resoult;
}

static void run_all_tests(void)
{
   RUN_TEST_GROUP(Modbus_Data);
   RUN_TEST_GROUP(Modbus_Master_Requests);
   RUN_TEST_GROUP(Modbus_Slave_Resp);
   RUN_TEST_GROUP(Modbus_Master_Read);
   RUN_TEST_GROUP(Modbus_CRC);
}