cmake_minimum_required(VERSION 3.20)

set (LIB_INTERNALL_INC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/slave_internall
  ${CMAKE_CURRENT_SOURCE_DIR}/master_internall
  )

add_library(modbus_lib STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/modbus_master.c
  ${CMAKE_CURRENT_SOURCE_DIR}/modbus_slave.c
  ${CMAKE_CURRENT_SOURCE_DIR}/slave_internall/modbus_slave_data.c
  ${CMAKE_CURRENT_SOURCE_DIR}/slave_internall/modbus_slave_PDU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/slave_internall/modbus_slave_RTU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/master_internall/modbus_master_PDU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/master_internall/modbus_master_RTU.c
  ${CMAKE_CURRENT_SOURCE_DIR}/master_internall/modbus_queue.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/master_internall/modbus_master_data_interface_weak.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/common/buf_rw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/common/modbus_crc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/common/modbus_PDU_common.c
  ${CMAKE_CURRENT_SOURCE_DIR}/common/modbus_RTU.c

)

# set_target_properties(modbus_lib PROPERTIES OUTPUT_NAME "lib_lcd_hd44780")
set_target_properties(modbus_lib PROPERTIES 
  OUTPUT_NAME "modbus_lib"
  PREFIX ""
  )

target_include_directories(modbus_lib PRIVATE ${LIB_INTERNALL_INC})
target_include_directories(modbus_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

