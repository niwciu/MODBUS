#############################################################################################################################
# FILE:    custom_targets.cmake
# BRIEF:   Defines project-specific custom build targets (unit tests, static analysis, complexity, coverage, formatting)
# NOTE:
#   Relative paths below are intentional for cleaner console output.
#   WORKING_DIRECTORY guarantees they resolve correctly at runtime.
#############################################################################################################################

#############################################################################################################################
# 0) TARGET CONFIGURATION
#    -> Replace the "template" values with the actual module names
#    -> Keep correct case sensitivity
#############################################################################################################################
set(SRC_ROOT_DIR lib)
set(SRC_MODULE_FOLDER_NAME modbus)
set(TEST_MODULE_FOLDER_NAME modbus)
set(REPORTS_PREFIX_NAME modbus)
set(GCOVR_REPORT_SUBFOLDER_NAME modbus)


#############################################################################################################################
# 1) RUN UNIT TESTS
#############################################################################################################################
message(STATUS "To run Unit Tests, you can use predefined target: \r\n\trun, \r\n\trun_ctest")

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo "Running ${PROJECT_NAME} unit tests..."
    COMMAND ${PROJECT_NAME} 
    COMMENT "Executing ${PROJECT_NAME} app"
    VERBATIM
)

add_custom_target(run_ctest
    COMMAND ${CMAKE_COMMAND} -E echo "Running ${PROJECT_NAME} unit tests..."
    COMMAND ${CMAKE_CTEST_COMMAND} 
    COMMENT "Executing ${PROJECT_NAME} via CTest"
    VERBATIM
)

#############################################################################################################################
# 2) CODE COMPLEXITY (LIZARD)
#############################################################################################################################
find_program(LIZARD_EXECUTABLE lizard)
if(LIZARD_EXECUTABLE)
    message(STATUS "Lizard found — predefined targets available: \r\n\tccm, \r\n\tccmr")

    add_custom_target(ccm
        COMMAND ${LIZARD_EXECUTABLE}
            ../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            --CCN 12 
			-Tnloc=30 
			-a 4
            --languages cpp
            -V 
			-i 0
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running Lizard complexity metrics (console output)"
        VERBATIM
    )

    add_custom_target(ccmr
        COMMAND ${CMAKE_COMMAND} -E make_directory ../../reports/CCM/
        COMMAND ${LIZARD_EXECUTABLE}
            ../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            --CCN 12 
			-Tnloc=30 
			-a 4
            --languages cpp
            -V 
			-o ../../reports/CCM/${REPORTS_PREFIX_NAME}.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating Lizard HTML complexity report"
        VERBATIM
    )
else()
    message(STATUS "Lizard not found — install it to enable ccm / ccmr targets.")
endif()

#############################################################################################################################
# 3) STATIC ANALYSIS (CPPCHECK)
#############################################################################################################################
find_program(CPPCHECK_EXECUTABLE cppcheck)
if(CPPCHECK_EXECUTABLE)
    message(STATUS "CppCheck found — predefined target available: \r\n\tcppcheck")

    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXECUTABLE}
            ../../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            ../../../test/${TEST_MODULE_FOLDER_NAME}
            -i../../test/${TEST_MODULE_FOLDER_NAME}/out
            -i../../test/${TEST_MODULE_FOLDER_NAME}/cmocks
            --enable=all
            --force
            --std=c99
            --suppress=missingIncludeSystem
            --suppress=missingInclude
			--check-level=exhaustive
			# --suppress=unusedFunction:../../../test/....
			--suppress=unusedFunction:../../../test/modbus/master_PDU_read_test_runner.c:3
			--suppress=unusedFunction:../../../lib/modbus/modbus_slave.c:135 ## ToDo Verify this ? -> shuldnt be reported
			--checkers-report=cppcheck_checkers_report.txt
			--error-exitcode=1
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running CppCheck static analysis for src & test directories"
        VERBATIM
    )
else()
    message(STATUS "CppCheck was not found — install it to enable cppcheck target.")
endif()

#############################################################################################################################
# 4) CODE COVERAGE (GCOVR)
#############################################################################################################################
find_program(GCOVR_EXECUTABLE gcovr)
if(GCOVR_EXECUTABLE)
    message(STATUS "Gcovr found — predefined targets available: \r\n\tccc, \r\n\tccr, \r\n\tccca, \r\n\tccra")

    set(REPORT_DIR ../../reports/CCR)
    set(REPORT_JSON_DIR ${REPORT_DIR}/JSON_ALL)
    set(REPORT_COMMON_HTML_DIR ${REPORT_JSON_DIR}/HTML_OUT)
    set(REPORT_MODULE_DIR ${REPORT_DIR}/${GCOVR_REPORT_SUBFOLDER_NAME})

    add_custom_target(ccr
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_JSON_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_MODULE_DIR}
        COMMAND ${GCOVR_EXECUTABLE}
            -r ../../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            --json ${REPORT_JSON_DIR}/coverage_${REPORTS_PREFIX_NAME}.json
            --json-base ${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            --html-details ${REPORT_MODULE_DIR}/${REPORTS_PREFIX_NAME}_report.html
            --html-theme github.dark-green
            .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating Code Coverage report for module"
        VERBATIM
    )

    add_custom_target(ccc
        COMMAND ${GCOVR_EXECUTABLE}
            -r ../../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}
            --fail-under-line 90
            .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Performing Code Coverage check (min 90%)"
        VERBATIM
    )

    add_custom_target(ccca
        COMMAND ${GCOVR_EXECUTABLE}
            -r ../../../
            --json-add-tracefile "${REPORT_JSON_DIR}/coverage_*.json"
            .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Aggregating Code Coverage data from all modules"
        VERBATIM
    )

    add_custom_target(ccra
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_JSON_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${REPORT_COMMON_HTML_DIR}
        COMMAND ${GCOVR_EXECUTABLE}
            -r ../../../
            --json-add-tracefile "${REPORT_JSON_DIR}/coverage_*.json"
            --html-details -o ${REPORT_COMMON_HTML_DIR}/project_coverage.html
            --html-theme github.dark-green
            .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating aggregated Code Coverage HTML report"
        VERBATIM
    )
	add_dependencies(ccc run)
    add_dependencies(ccra ccr)
    add_dependencies(ccca ccr)
else()
    message(STATUS "Gcovr not found — install it to enable code coverage targets.")
endif()

#############################################################################################################################
# 5) CODE FORMATTING (CLANG-FORMAT) — GLOB zamiast '*'
#############################################################################################################################
# CMake nie rozwija gwiazdek. Używamy file(GLOB ...) by zbudować listę plików,
# a potem wstrzykujemy ją do polecenia z COMMAND_EXPAND_LISTS (modern CMake).
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    message(STATUS "clang-format found — predefined targets: \r\n\tformat, \r\n\tformat_check")

    # Zbierz pliki źródłowe do formatowania (ścieżki RELATIVE dla krótszego outputu)
    file(GLOB FORMAT_SOURCES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}/*.c
        ../../${SRC_ROOT_DIR}/${SRC_MODULE_FOLDER_NAME}/*.h
    )

    file(GLOB FORMAT_TEST_SOURCES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ./*.c
        ./*.h
    )

    # Zbierz wszystkie pliki do sprawdzenia/formatowania
    set(ALL_FORMAT_FILES ${FORMAT_SOURCES} ${FORMAT_TEST_SOURCES})

    # format_check: sprawdza formatowanie bez modyfikowania plików
    # Uwaga: gdy clang-format dostanie 0 plików, potrafi czytać stdin i wygląda to jak "zawieszenie".
    # Drugi częsty problem to limity długości linii poleceń (zwłaszcza na Windows) — dlatego lecimy per plik.
    add_custom_target(format_check
        COMMENT "Checking formatting (sources + tests) using clang-format"
    )

    foreach(f IN LISTS ALL_FORMAT_FILES)
        add_custom_command(TARGET format_check PRE_BUILD
            COMMAND ${CLANG_FORMAT_EXECUTABLE}
                    --dry-run --Werror -style=file
                    "${f}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
        )
    endforeach()

    # (Opcjonalnie) jeżeli lista jest pusta, pokaż informację zamiast ryzykować uruchomienie clang-format bez argumentów
    if(NOT ALL_FORMAT_FILES)
        add_custom_command(TARGET format_check PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "format_check: no files matched (nothing to check)."
            VERBATIM
        )
    endif()

    # format: formatuje kod + testy (jeden target)
    # Zabezpieczenie: jeżeli nie ma plików (np. brak testów albo pusty src), nie wołamy clang-format bez argumentów.
    add_custom_target(format
        COMMENT "Formatting source + test files using clang-format"
    )

    foreach(f IN LISTS ALL_FORMAT_FILES)
        add_custom_command(TARGET format PRE_BUILD
            COMMAND ${CLANG_FORMAT_EXECUTABLE}
                    -i -style=file
                    "${f}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
        )
    endforeach()

    if(NOT ALL_FORMAT_FILES)
        add_custom_command(TARGET format PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "format: no files matched (nothing to format)."
            VERBATIM
        )
    endif()

else()
    message(STATUS "clang-format was not found — install it to enable code formatting targets.")
endif()
#############################################################################################################################
# END OF FILE
#############################################################################################################################
