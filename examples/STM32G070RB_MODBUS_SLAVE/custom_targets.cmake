# PROJECT CUSTOM TARGETS FILE
#  here you can define custom targets for the project so all team member can use it in the same way
#  some example of custo targets are shown bello those are targets for:
# 		1. Running unit tests
# 		2. Code Complexity Metrics
# 		3. CppCheck static analize of specific folder
# 		4. Code Coverage report generation.


# TARGET FOR CHECKING CODE COMPLEXITY METRICS"
# check if lizard software is available 
find_program(lizard_program lizard)
if(lizard_program)
	message(STATUS "Lizard was found, you can use predefined targets for src folder Code Complexity Metrics: \r\n\tccm,\r\n\tccmr,")
else()
	message(STATUS "Lizard was not found. \r\n\tInstall Lizard to get predefined targets for src folder Code Complexity Metrics")
endif()
# Prints CCM for src folder in the console
add_custom_target(ccm lizard ../../../hw/STM32G070RB_MODBUS_SLAVE 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Include/*"
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Device/*"  
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Debug/*" 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Core/Src/syscalls.c" 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Core/Src/sysmem.c" 
							--CCN 12 
							-Tnloc=30 
							-a 4 
							--languages cpp 
							-V 
							-i 1)
# Create CCM report in reports/Cylcomatic_Complexity/
add_custom_target(ccmr lizard ../../../hw/ 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Include/*"
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Device/*"  
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Debug/*" 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Core/Src/syscalls.c" 
							--exclude="../../../hw/STM32G070RB_MODBUS_SLAVE/Core/Src/sysmem.c" 
							--CCN 12 -Tnloc=30 
							-a 4 
							--languages cpp 
							-V 
							-o ../../../reports/Cyclomatic_Complexity/Lizard_report.html)

# TARGET FOR MAKING STATIC ANALYSIS OF THE SOURCE CODE AND UNIT TEST CODE
# check if cppchec software is available 
find_program(cppcheck_program cppcheck)
if(cppcheck_program)
	message(STATUS "CppCheck was found, you can use predefined targets for static analize : \r\n\tcppcheck,")
else()
	message(STATUS "CppCheck was not found. \r\n\tInstall CppCheck to get predefined targets for static analize")
endif()
# Prints static analize output for src folder in the console
add_custom_target(cppcheck cppcheck ../../../hw/STM32G070RB_MODBUS_SLAVE 
									-i../../../hw/STM32G070RB_MODBUS_SLAVE/Debug 
									-i../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Include
									-I../../../hw/STM32G070RB_MODBUS_SLAVE/Core/Inc
									-I../../../hw/STM32G070RB_MODBUS_SLAVE/Drivers/CMSIS/Device
									--enable=all 
									--inconclusive 
									--force 
									--inline-suppr 
									--platform=win64 
									--suppress=missingIncludeSystem 
									--suppress=unusedFunction 
									--suppress=missingInclude)

