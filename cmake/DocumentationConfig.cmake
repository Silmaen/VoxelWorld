#
# ---=== documentation ===---
#
find_package(Doxygen
        REQUIRED dot)
if (DOXYGEN_FOUND)
    message(STATUS "found doxygen version: ${DOXYGEN_VERSION}")
    if (${DOXYGEN_VERSION} VERSION_LESS 1.9.1)
        message(FATAL_ERROR "Doxygen version 1.9.1 or higher is required")
    endif ()
    add_custom_target(documentation)
    add_custom_command(
            TARGET documentation PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Copying Doxyfile to binary dir")
    add_custom_command(TARGET documentation POST_BUILD
            COMMAND ${DOXYGEN_EXECUTABLE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation"
            VERBATIM)
    add_custom_command(TARGET documentation POST_BUILD
            COMMAND echo "look at the result: file:///${CMAKE_CURRENT_BINARY_DIR}/Documentation/html/index.html")
else (DOXYGEN_FOUND)
    message(ERROR no doxygen found)
endif (DOXYGEN_FOUND)