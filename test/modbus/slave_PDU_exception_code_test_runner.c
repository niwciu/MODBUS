#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Slave_PDU_exception_code)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01);
   
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddressAndQuantitiofOutputsThenSlaveRespondWithExceptionCode02);
   // Don't see place for adopting exception code 04 while if coil is not registered it will retunr 0
   // RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestAndCatchErrorDuringProcessingOfRequestThenSlaveRespondWithExceptionCode04);

   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithDiscreteInputsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithDiscreteInputsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestWithIncorrectStartingAddressAndQuantitiofOutputsThenSlaveRespondWithExceptionCode02);
   // Don't see place for adopting exception code 04 while if coil is not registered it will retunr 0
   // RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDicreteInputsRequestAndCatchErrorDuringProcessingOfRequestThenSlaveRespondWithExceptionCode04);
   
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   // RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithIncorrectStartingAddressAndQuantitiofOutputsThenSlaveRespondWithExceptionCode02);
   // Don't see place for adopting exception code 04 while if coil is not registered it will retunr 0
   // RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestAndCatchErrorDuringProcessingOfRequestThenSlaveRespondWithExceptionCode04);

   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );

}
