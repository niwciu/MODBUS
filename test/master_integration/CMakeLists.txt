#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building Unit test modules.
#
# usage:
#        For build using Unix Makefiles:
#          	1. cmake -S./ -B out -G"Unix Makefiles"
#			2. enter the "out" folder
#          	3. make all -o lcd_hd44780_test.o (-jXX additionaly to speed up)
#        For build using Ninja:
#          	1. cmake -S./ -B out -G"Ninja"
# 			2. enter the "out" folder
#         	3. ninja -C out -o lcd_hd44780_test.o (optional with -V  and -jxx ->xx numnber of cores)
# additional custom targets for this project:
# 		If lizard is installed and you are in the out folder:
# 			1. make ccm -> code complexity metrix print in console
# 			2. make ccmr -> code complexity metrics report generation
# 		If cppcheck is installed and you are in the out folder
# 			1. make cppcheck_src -> static analize  for src folder printed in console
# 			2. make cppcheck_test-> static analize  for src folder printed in console
# 		If gcovr is installed and you are in the out folder
# 			1. make ccr -> code coverage report generation 
# 
#############################################################################################################################
cmake_minimum_required(VERSION 3.10)

project(modbus_master_integration)

add_subdirectory(../unity unity)

set(CUSTOM_TARGETS_FILE_DIR ./custom_targets.cmake)

set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
endif()

set(INCLUDE_DIRS
	../../src
	../../src/master_internall
	../../src/slave_internall
	../../src/common
)

set(TEST_INCLUDE_DIRS
	..
	.
	../modbus
	
)

set(SRCS
	../../src/master_internall/modbus_queue.c
	../../src/master_internall/modbus_master_PDU.c
	../../src/master_internall/modbus_master_RTU.c
	../../src/slave_internall/modbus_slave_PDU.c
	../../src/slave_internall/modbus_slave_RTU.c
	../../src/slave_internall/modbus_slave_data.c
	../../src/common/buf_rw.c
	../../src/common/modbus_crc.c
	../../src/common/modbus_RTU.c
	../../src/common/modbus_PDU_common.c
	../../src/modbus_master.c

	../../src/modbus_slave.c
)

set(TEST_SRCS
    master_integration_test_main.c
    ../modbus/master_RTU_init_test_runner.c
    ../modbus/master_RTU_init_test.c
	../modbus/master_RTU_req_gen_test_runner.c
    ../modbus/master_RTU_req_gen_test.c
	../modbus/master_RTU_test_runner.c
	../modbus/master_RTU_test.c
	
	../modbus/mock_slave_driver_interface.c
	../modbus/mock_slave_app_data.c
	../modbus/mock_master_driver_interface.c
	../modbus/mock_master_data_interface.c

	#mock_module.c
)

set(GLOBAL_DEFINES
	-DUNIT_TEST
)

include(${CUSTOM_TARGETS_FILE_DIR})

include_directories(${INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

link_libraries(unity)
add_executable(${PROJECT_NAME} ${SRCS} ${TEST_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
target_link_options(${PROJECT_NAME} PRIVATE -fprofile-arcs)
