#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Slave_PDU_exception_code)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01);
   
   //  RUN_TEST_CASE(Slave_PDU_resp, );
   // RUN_TEST_CASE(Slave_PDU_resp, );
   // RUN_TEST_CASE(Slave_PDU_resp, );
   // RUN_TEST_CASE(Slave_PDU_resp, );
   // RUN_TEST_CASE(Slave_PDU_resp, );
   // RUN_TEST_CASE(Slave_PDU_resp, );
}
