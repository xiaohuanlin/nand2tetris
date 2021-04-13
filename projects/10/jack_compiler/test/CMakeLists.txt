message(STATUS "Enter test cmake file")
file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

# valgrind
find_program(VALGRIND_BIN valgrind)
if ("${VALGRIND_BIN}" STREQUAL "VALGRIND_BIN-NOTFOUND")
    message(WARNING "BusTub/test couldn't find valgrind.")
else()
    message(STATUS "BusTub/test found valgrind at ${VALGRIND_BIN}")
endif()

set(VALGRIND_OPTIONS
        --error-exitcode=1                      # if leaks are detected, return nonzero value
        # --gen-suppressions=all  # uncomment for leak suppression syntax
        --leak-check=full                       # detailed leak information
        --soname-synonyms=somalloc=*jemalloc*   # also intercept jemalloc
        --trace-children=yes                    # trace child processes
        --track-origins=yes                     # track origin of uninitialized values
        )
set(VALGRIND_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/build_support/valgrind.supp")

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##### make XYZ_test #####
foreach (test_source ${TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})
    add_dependencies(build-tests ${test_name})
    add_dependencies(check-tests ${test_name})

    target_link_libraries(${test_name} gtest gmock_main ${PROJECT_NAME})

    # Set test target properties and dependencies.
    set_target_properties(${test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
        COMMAND ${test_name}
    )

    # Add the test under CTest.
    message(STATUS "Create new test case ${test_name}")

    add_test(${test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
    --suppressions=${VALGRIND_SUPPRESSIONS_FILE}
    ${CMAKE_BINARY_DIR}/test/${test_name} --gtest_color=yes)

    add_test(${test_name} ${CMAKE_BINARY_DIR}/test/${test_name} --gtest_color=yes)
endforeach(test_source ${TEST_SOURCES})
