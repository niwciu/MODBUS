#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(Master_PDU_req)
{
    /* Test cases to run */
    RUN_TEST_CASE(Master_PDU_req, ReadSingleHoldingRegisterRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, ReadSingleHoldingRegisterRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadMaxQtyHoldingRegisterRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadHoldingRegisterMaxQtyPlus1Request);
    RUN_TEST_CASE(Master_PDU_req, Read0QtyHoldingRegisterRequest);

    RUN_TEST_CASE(Master_PDU_req, ReadSingleInputRegisterRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, ReadSingleInputRegisterRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadMaxQtyInputRegisterRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadInputRegisterMaxQtyPlus1Request);
    RUN_TEST_CASE(Master_PDU_req, ReadZeroInputRegisterRequest);

    RUN_TEST_CASE(Master_PDU_req, ReadDiscreteInputsRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, ReadDiscreteInputsRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadMaxQtyDiscreteInputsRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadMaxQtPlus1DiscreteInputsRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadZeroDiscreteInputsRequest);

    RUN_TEST_CASE(Master_PDU_req, ReadCoilsRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, ReadCoilsRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadCoilsMaxQtyRequest);
    RUN_TEST_CASE(Master_PDU_req, ReadCoilsMaxQtyPlus1Request);
    RUN_TEST_CASE(Master_PDU_req, ReadZeroCoilsRequest);

    RUN_TEST_CASE(Master_PDU_req, WriteSingleRegisterRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, WriteSingleRegister);

    RUN_TEST_CASE(Master_PDU_req, WriteSingleCoilRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, WriteSingleCoilOn);

    RUN_TEST_CASE(Master_PDU_req, WriteMultipleRegistersRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, WriteMultipleRegisters);
    RUN_TEST_CASE(Master_PDU_req, WriteMaxQtyMultipleRegisters);
    RUN_TEST_CASE(Master_PDU_req, WriteMultipleRegistersMaxQtyPlus1);
    RUN_TEST_CASE(Master_PDU_req, WriteZeroMultipleRegisters);

    RUN_TEST_CASE(Master_PDU_req, Write5MultipleCoilsRequestWithNullPtrModbusMasgPassed);
    RUN_TEST_CASE(Master_PDU_req, Write5MultipleCoils);
    RUN_TEST_CASE(Master_PDU_req, WriteMaxQtyMultipleCoils);
    RUN_TEST_CASE(Master_PDU_req, WriteMultipleCoilsMaxQtyPlus1);
    RUN_TEST_CASE(Master_PDU_req, WriteZeroMultipleCoils);

    // RUN_TEST_CASE(Master_PDU_req, );
}
