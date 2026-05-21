#############################################################################################################################
# file:  CMakeLists.txt
# brief: CMakeLists.txt for STM32G070RB MODBUS Bridge example.
#        The bridge acts as Modbus slave (USART2, upstream) and Modbus master (USART1, downstream)
#        simultaneously, forwarding data collected from the downstream subnet to the upstream network.
#
# NOTE: STM32G070xx CMSIS device headers are in the local Drivers/CMSIS tree:
#         Drivers/CMSIS/Device/  (stm32g070xx.h, stm32g0xx.h)
#         Drivers/CMSIS/Include/ (core_cm0plus.h, cmsis_gcc.h, etc.)
#
# usage: cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#        make -C Debug all
#############################################################################################################################
cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../config/STM32/Toolchain-arm-gcc.cmake)

enable_language(ASM)

project(STM32G070RB_MODBUS_BRIDGE)

set(INCLUDE_DIRS
    Core/Inc
    Drivers/CMSIS/Device
    Drivers/CMSIS/Include

    ../src
    ../../lib/modbus
    ../../lib/modbus/common
    ../../lib/modbus/slave_internall
    ../../lib/modbus/master_internall
    .
)

set(C_SRCS
    Core/Src/main.c
    Core/Src/modbus_slave_driver_interface.c
    Core/Src/modbus_master_driver_interface.c
    Core/Src/modbus_master_data_interface.c
    ../src/main_app_bridge.c
    ../STM32G070RB_MODBUS_SLAVE/Core/Src/syscalls.c
    ../STM32G070RB_MODBUS_SLAVE/Core/Src/sysmem.c
    ../STM32G070RB_MODBUS_SLAVE/Core/Src/core_init.c

    ../../lib/modbus/slave_internall/modbus_slave_PDU.c
    ../../lib/modbus/slave_internall/modbus_slave_RTU.c
    ../../lib/modbus/slave_internall/modbus_slave_data.c
    ../../lib/modbus/master_internall/modbus_master_PDU.c
    ../../lib/modbus/master_internall/modbus_master_RTU.c
    ../../lib/modbus/master_internall/modbus_queue.c
    ../../lib/modbus/master_internall/modbus_master_data_interface_weak.c
    ../../lib/modbus/common/modbus_RTU.c
    ../../lib/modbus/common/modbus_crc.c
    ../../lib/modbus/common/buf_rw.c
    ../../lib/modbus/common/modbus_PDU_common.c
    ../../lib/modbus/modbus_slave.c
    ../../lib/modbus/modbus_master.c
)

set(ASM_SRCS
    ../STM32G070RB_MODBUS_SLAVE/Core/Startup/startup_stm32g070rbtx.s
)

set(GLOBAL_DEFINES
    -DSTM32G070xx
)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

set(MCPU "-mcpu=cortex-m0plus")
include(../config/STM32/compiler_flags.cmake)

add_arm_executable(${CMAKE_PROJECT_NAME} ${C_SRCS} ${ASM_SRCS})
arm_link_libraries(${CMAKE_PROJECT_NAME})
