#############################################################################################################################
# file:  CMakeLists.txt
# brief: CMakeLists.txt for ATmega328P MODBUS Slave example
#
# usage: build using Unix Makefiles:
#          cmake -S ./ -B build -G"Unix Makefiles"
#          make -C build all
#        Ninja build:
#          cmake -S ./ -B build -G"Unix Makefiles"
#          make -C build all
#############################################################################################################################
cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../config/AVR/Toolchain-AVR-gcc.cmake)

project(ATmega328P_MODBUS_SLAVE)

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
    ../../lib/modbus/slave_internall
)

set(C_SRCS
    Core/Src/main.c
    Core/Src/LED_driver.c
    Core/Src/modbus_driver_interface.c
    ../src/main_app_slave.c

    ../../lib/modbus/slave_internall/modbus_slave_PDU.c
    ../../lib/modbus/slave_internall/modbus_slave_RTU.c
    ../../lib/modbus/common/modbus_RTU.c
    ../../lib/modbus/slave_internall/modbus_slave_data.c
    ../../lib/modbus/common/modbus_crc.c
    ../../lib/modbus/common/buf_rw.c
    ../../lib/modbus/modbus_slave.c
    ../../lib/modbus/common/modbus_PDU_common.c
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
