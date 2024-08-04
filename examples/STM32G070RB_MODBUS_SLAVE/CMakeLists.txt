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

set(CUSTOM_TARGETS_FILE_DIR ./custom_targets.cmake)

# Allow assembler
enable_language(ASM)

project(STM32G070RB_MODBUS_SLAVE_HW_TEST)

set(INCLUDE_DIRS
Core
Core/Inc/
Core/Startup

Drivers
Drivers/CMSIS
Drivers/CMSIS/Device
Drivers/CMSIS/Include
# Drivers/CMSIS/STM32G0_Drivers
# Drivers/STM32G0_Drivers/core_init

../../src
../../src/common
../../src/slave_internall

.
)

set(C_SRCS
    Core/Src/main.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/core_init.c
    Core/Src/modbus_driver_interface.c
    Core/Src/LED_driver.c

	../../src/slave_internall/modbus_slave_PDU.c
    ../../src/slave_internall/modbus_slave_RTU.c
    ../../src/common/modbus_RTU.c
    ../../src/slave_internall/modbus_slave_data.c
    ../../src/common/modbus_crc.c
    ../../src/common/buf_rw.c
    ../../src/modbus_slave.c
    ../../src/common/modbus_PDU_common.c
	
)

set(ASM_SRCS
    Core/Startup/startup_stm32g070rbtx.s
)

set(GLOBAL_DEFINES
    -DSTM32G070xx
)

set(LINKED_LIBS

)

link_directories(

)

include(${CUSTOM_TARGETS_FILE_DIR})

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

set(MCPU "-mcpu=cortex-m0plus")
include(    ../config/STM32/compiler_flags.cmake)

# generate elf file
add_arm_executable(${CMAKE_PROJECT_NAME} ${C_SRCS} ${ASM_SRCS})

arm_link_libraries(${CMAKE_PROJECT_NAME} ${LINKED_LIBS})
