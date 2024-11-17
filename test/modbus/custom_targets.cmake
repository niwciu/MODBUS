# PROJECT CUSTOM TARGETS FILE
#  here you can define custom targets for the project so all team member can use it in the same way
#  some example of custo targets are shown bello those are targets for:
# 		1. Running unit tests
# 		2. Code Complexity Metrics
# 		3. CppCheck static analize of specific folder
# 		4. Code Coverage report generation.


#TARGETS FOR RUNNING UNIT TESTS
add_custom_target(run modbus_test)

# TARGET FOR CHECKING CODE COMPLEXITY METRICS"
# check if lizard software is available 
find_program(lizard_program lizard)
if(lizard_program)
	message(STATUS "Lizard was found, you can use predefined targets for src folder Code Complexity Metrics: \r\n\tccm,\r\n\tccmr,")
else()
	message(STATUS "Lizard was not found. \r\n\tInstall Lizard to get predefined targets for src folder Code Complexity Metrics")
endif()
# Prints CCM for src folder in the console
add_custom_target(ccm lizard ../../../src/ 
							--CCN 12 
							-Tnloc=30 
							-a 4 
							--languages cpp 
							-V 
							-i 1)
# Create CCM report in reports/Cylcomatic_Complexity/
add_custom_target(ccmr lizard ../../../src/ 
							--CCN 12 
							-Tnloc=30 
							-a 4 
							--languages cpp 
							-V 
							-o ../../../reports/Cyclomatic_Complexity/Lizard_report.html)

# TARGET FOR MAKING STATIC ANALYSIS OF THE SOURCE CODE AND UNIT TEST CODE
# check if cppchec software is available 
find_program(cppcheck_program cppcheck)
if(cppcheck_program)
	message(STATUS "CppCheck was found, you can use predefined targets for static analize : \r\n\tcppcheck_src,\r\n\tcppcheck_test,")
else()
	message(STATUS "CppCheck was not found. \r\n\tInstall CppCheck to get predefined targets for static analize")
endif()
# Prints cppcheck static analize output for src folder in the console
add_custom_target(cppcheck_src cppcheck 
										../../../src 
										--enable=all
										# --inconclusive
										--std=c99
										# --inline-suppr 
										# --platform=win64 
										--suppress=missingIncludeSystem 
										--suppress=missingInclude
										--suppress=unusedFunction:../../../src/modbus_slave.c
										--suppress=unusedFunction:../../../src/modbus_master.c
										--checkers-report=cppcheck_checkers_report.txt
										)
# Prints cppcheck static analize output for unit tests build configuration
add_custom_target(cppcheck_test cppcheck 
										--project=../../../test/modbus/out/compile_commands.json
										--enable=all
										# --inconclusive
										--force
										--std=c99
										# --inline-suppr 
										# --platform=win64 
										--suppress=unusedFunction:*/master_PDU_req_test_runner.c
										--suppress=missingInclude
										--suppress=missingIncludeSystem 
										--checkers-report=cppcheck_checkers_report.txt
										)
# TARGET FOR CHECKING CODE COVERAGE AND CREATING CODE COVERAGE REPORTS
# check if python 3 and gcovr are available 
find_program(GCOVR gcovr)
find_program(PYTHON3 python3)
if(PYTHON3)
	if(GCOVR)
		message(STATUS "python 3 and gcovr was found, you can use predefined targets for uint tests code coverage report generation : \r\n\tccr,")
	else()
		message(STATUS "pyton 3 was found but gcovr was not found. \r\n\tInstall gcovr to get predefined targets for uint tests code coverage report generation")
	endif()
else()
	if(GCOVR)
		message(STATUS "python3 was not found. \r\n\tInstall python 3 to get predefined targets for uint tests code coverage report generation")
	else()
		message(STATUS "python3 and gcovr were not found. \r\n\tInstall python 3 and gcovr to get predefined targets for uint tests code coverage report generation")
	endif()
endif()

# CODE COVERAGE REPORT
add_custom_target(ccr python3 -m gcovr CMakeFiles/modbus_test.dir/D_/EMBEDDED/LIBRARIES/C_libraries/MODBUS/src 
									   -r ../../.. 
									   --html-details ../../../reports/Code_Coverage/modbus_gcov_report.html)
# CODE COVERAGE CHECK
add_custom_target(ccc python3 -m gcovr CMakeFiles/modbus_test.dir/D_/EMBEDDED/LIBRARIES/C_libraries/MODBUS/src 
									   -r ../../.. 
									   --fail-under-line 90)