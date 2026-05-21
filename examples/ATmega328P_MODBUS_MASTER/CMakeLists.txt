#############################################################################################################################
# file:  CMakeLists.txt
# brief: CMakeLists.txt for ATmega328P MODBUS Master example
#
# usage: Debug build using Unix Makefiles:
#          cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug all
#        Release build:
#          cmake -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release all
#############################################################################################################################
cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../config/AVR/Toolchain-AVR-gcc.cmake)

project(ATmega328P_MODBUS_MASTER)

set(MMCU     "atmega328p")
set(PROG_TYPE usbasp)
set(L_FUSE   0xff)   # 16 MHz external crystal
set(H_FUSE   0xde)
set(E_FUSE   0xff)

set(INCLUDE_DIRS
    Core/Inc
    ../src
    ../../lib/modbus
    ../../lib/modbus/common
    ../../lib/modbus/master_internall
)

set(C_SRCS
    Core/Src/main.c
    Core/Src/modbus_driver_interface.c
    Core/Src/modbus_master_data_interface.c
    ../src/main_app_master.c

    ../../lib/modbus/master_internall/modbus_master_PDU.c
    ../../lib/modbus/master_internall/modbus_master_RTU.c
    ../../lib/modbus/master_internall/modbus_queue.c
    ../../lib/modbus/master_internall/modbus_master_data_interface_weak.c
    ../../lib/modbus/common/modbus_RTU.c
    ../../lib/modbus/common/modbus_crc.c
    ../../lib/modbus/common/buf_rw.c
    ../../lib/modbus/common/modbus_PDU_common.c
    ../../lib/modbus/modbus_master.c
)

set(GLOBAL_DEFINES
    -DF_CPU=16000000UL
)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

include(../config/AVR/AVR_compiler_flags.cmake)
add_compile_options(${COMPILE_OPTIONS})

add_AVR_executable(${CMAKE_PROJECT_NAME} ${C_SRCS})
AVR_link_libraries(${CMAKE_PROJECT_NAME})
