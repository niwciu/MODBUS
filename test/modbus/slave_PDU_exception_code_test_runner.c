#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Slave_PDU_exception_code)
{
   /* Test cases to run */
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveRequestWithUnsuportedFunctionCodeThenRespondWithExceptionCode01);
   
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCoilsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadCoilsRequestAndGetErrorWhenReadingCoilsThenSlaveRespondWithExceptionCode04);

   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithDiscreteInputsQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithDiscreteInputsQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadDiscreteInputsRequestAndGetErrorWhenReadingDiscreteInputsThenSlaveRespondWithExceptionCode04);
   
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithInputRegisterQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadInputRegisterRequestAndGetErrorWhenReadingInputRegisterThenSlaveRespondWithExceptionCode04);

   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithHoldingRegisterQtyToReadAboveAllowedValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithHoldingRegisterQtyToReadEqual0ThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithIncorrectStartingAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestWithCorrectStartingAddressAndIncorrectQuantitiOfOutputsThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveReadHoldingRegisterRequestAndGetErrorWhenReadingHoldingRegisterThenSlaveRespondWithExceptionCode04);
   
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestWithIncorrectCoilValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestWithIncorrectAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleCoilRequestAndGetErrorStatusWhenWritingCoilThenSlaveRespondWithExceptionCode04);

   // RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleRegisterRequestWithIncorrectRegisterValueThenSlaveRespondWithExceptionCode03);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleRegisterRequestWithIncorrectAddresThenSlaveRespondWithExceptionCode02);
   RUN_TEST_CASE(Slave_PDU_exception_code, WhenSlaveReciveWriteSingleRegisterRequestAndGetErrorStatusWhenWritingRegisterThenSlaveRespondWithExceptionCode04);
   
   
   // RUN_TEST_CASE(Slave_PDU_exception_code, );
   // RUN_TEST_CASE(Slave_PDU_exception_code, );

}
