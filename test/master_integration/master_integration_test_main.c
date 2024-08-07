#include "unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
    int test_resoult = UnityMain(argc, argv, run_all_tests);

    return test_resoult;
}

static void run_all_tests(void)
{
    RUN_TEST_GROUP(master_RTU_init_test);
    RUN_TEST_GROUP(Master_RTU_req_gen_test);
    RUN_TEST_GROUP(Master_RTU_test);
}