
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

add_custom_target(All_Tests)
# googletest
find_package(GTest 1.13 REQUIRED)

foreach (child ${children})
    if (${child} MATCHES "test_helper")
        continue()
    endif ()
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        file(GLOB_RECURSE
                SRCS
                ${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.cpp
                )
        if ("${SRCS} " STREQUAL " ")
            message(STATUS "Test folder '${child}' is empty, skipping")
        else ()
            add_executable(${PRJPREFIX_LOWER}_${child}_unit_test ${SRCS})
            set_target_properties(${PRJPREFIX_LOWER}_${child}_unit_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
            target_include_directories(${PRJPREFIX_LOWER}_${child}_unit_test PUBLIC test_helper)
            target_link_libraries(${PRJPREFIX_LOWER}_${child}_unit_test GTest::gtest GTest::gtest_main)
            target_link_libraries(${PRJPREFIX_LOWER}_${child}_unit_test ${ENGINE_NAME})
            add_test(${PRJPREFIX_LOWER}_${child}_UTests ./${PRJPREFIX_LOWER}_${child}_unit_test --gtest_output=xml:test/${child}_UTest_Report.xml)
            set_tests_properties(${PRJPREFIX_LOWER}_${child}_UTests PROPERTIES TIMEOUT 3600)
            add_dependencies(All_Tests ${PRJPREFIX_LOWER}_${child}_unit_test)
            add_custom_command(TARGET All_Tests POST_BUILD
                    COMMAND ${PRJPREFIX_LOWER}_${child}_unit_test
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        endif ()
    endif ()
endforeach ()

if (${PRJPREFIX}_COVERAGE)

    set(COVERAGE_RESULT_DIR ${CMAKE_BINARY_DIR}/Coverage)
    file(MAKE_DIRECTORY ${COVERAGE_RESULT_DIR})
    set(COVERAGE_TITLE "Owl: tests coverage")
    set(COVERAGE_HTML --html --html-details --html-details-syntax-highlighting --html-title ${COVERAGE_TITLE} --source-encoding utf-8)
    set(COVERAGE_FILTER "-e\".*external/.*\" -e\".*test/.*\" -e\".*owlnest.*\"  -e\".*sandbox.*\" -e\".*autogen.*\"")
    set(COVERAGE_OPTION "-buds -j 4 --exclude-throw-branches --exclude-unreachable-branches --exclude-lines-by-pattern \".*---UNCOVER---.*\"")
    if ("d${${PRJPREFIX}_GCOV}" STREQUAL "d")
        set(COVERAGE_GCOV)
    else ()
        set(COVERAGE_GCOV "--gcov-executable \"llvm-cov gcov\" --gcov-ignore-parse-errors")
    endif ()
    add_custom_command(TARGET All_Tests POST_BUILD
            COMMAND ${GCOVR} ${COVERAGE_OPTION} -r ${CMAKE_SOURCE_DIR} -o ${COVERAGE_RESULT_DIR}/index.html ${COVERAGE_GCOV} ${COVERAGE_HTML} ${COVERAGE_FILTER} .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            USES_TERMINAL)
    add_custom_command(TARGET All_Tests POST_BUILD
            COMMAND echo "look at the coverage result: file:///${COVERAGE_RESULT_DIR}/index.html")
endif ()

