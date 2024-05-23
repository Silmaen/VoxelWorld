
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    message(FATAL_ERROR "Coverage only works on debug builds")
endif ()
#set(GCOVR ${Python_EXECUTABLE} -u -m gcovr)
set(GCOVR gcovr)
# Get gcov version
execute_process(COMMAND ${GCOVR} --version
        OUTPUT_VARIABLE GCOVR_VERSION)
string(REPLACE "\n" ";" GCOVR_VERSION "${GCOVR_VERSION}")
list(SUBLIST GCOVR_VERSION 0 1 GCOVR_VERSION)
string(REPLACE " " ";" GCOVR_VERSION ${GCOVR_VERSION})
list(SUBLIST GCOVR_VERSION 1 1 GCOVR_VERSION)

message(STATUS "Found gcovr version ${GCOVR_VERSION}.")
if (${GCOVR_VERSION} VERSION_LESS 5.0)
    message(FATAL_ERROR "gcovr: Too old version of gcovr, minimum required is 5.0")
endif ()

target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE OWL_COVERAGE)

# options for coverage
if (${PRJPREFIX}_COMPILER_GCC)
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE --coverage)
    target_link_libraries(${CMAKE_PROJECT_NAME}_Base INTERFACE gcov)
elseif (${PRJPREFIX}_COMPILER_CLANG)
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE --coverage)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE --coverage)
    get_filename_component(COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(${PRJPREFIX}_GCOV "${COMPILER_PATH}/llvm-cov gcov")
    if (WIN32)
        string(REPLACE "/" "\\\\" ${PRJPREFIX}_GCOV ${${PRJPREFIX}_GCOV})
    endif ()
else ()
    message(FATAL_ERROR "Unsupported Compiler '${CMAKE_CXX_COMPILER_ID}' for coverage")
endif ()
