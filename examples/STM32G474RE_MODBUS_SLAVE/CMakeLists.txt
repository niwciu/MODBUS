#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit MCPU-Variable to suit STM32 project requirements.
#        For debug build using Unix Makefiles:
#          cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug all (optional with VERBOSE=1  and -jxx ->xx numnber of cores)
#        For debug build using Ninja:
#          cmake -S ./ -B Debug -G"Ninja" -DCMAKE_BUILD_TYPE=Debug
#          ninja -C Debug (optional with -V  and -jxx ->xx numnber of cores)
# 
#        For release build Unix Makefiles:
#          cmake -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make all -C Release VERBOSE=1 -j12 (optional with VERBOSE=1  and -jxx ->xx numnber of cores)
#        For Release build using Ninja:
#          cmake -S ./ -B Release -G"Ninja" -DCMAKE_BUILD_TYPE=Release
#          ninja -C Release (optional -V  and -jxx ->xx numnber of cores)
#############################################################################################################################
cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../config/STM32/Toolchain-arm-gcc.cmake)

enable_language(ASM)

project(STM32G474RE_MODBUS_SLAVE)

set(INCLUDE_DIRS
    Core/Inc
    Core/Startup
    Drivers/CMSIS/Device
    Drivers/CMSIS/Include

    ../src
    ../../lib/modbus
    ../../lib/modbus/common
    ../../lib/modbus/slave_internall
    .
)

set(C_SRCS
    Core/Src/main.c
    Core/Src/core_init.c
    Core/Src/modbus_driver_interface.c
    Core/Src/LED_driver.c
    ../src/main_app_slave.c
    ../STM32G070RB_MODBUS_SLAVE/Core/Src/syscalls.c
    ../STM32G070RB_MODBUS_SLAVE/Core/Src/sysmem.c

    ../../lib/modbus/slave_internall/modbus_slave_PDU.c
    ../../lib/modbus/slave_internall/modbus_slave_RTU.c
    ../../lib/modbus/common/modbus_RTU.c
    ../../lib/modbus/slave_internall/modbus_slave_data.c
    ../../lib/modbus/common/modbus_crc.c
    ../../lib/modbus/common/buf_rw.c
    ../../lib/modbus/modbus_slave.c
    ../../lib/modbus/common/modbus_PDU_common.c
)

set(ASM_SRCS
    Core/Startup/startup_stm32g474retx.s
)

set(GLOBAL_DEFINES
    -DSTM32G474xx
)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

set(MCPU "-mcpu=cortex-m4")
include(../config/STM32/compiler_flags.cmake)

add_arm_executable(${CMAKE_PROJECT_NAME} ${C_SRCS} ${ASM_SRCS})
arm_link_libraries(${CMAKE_PROJECT_NAME})
