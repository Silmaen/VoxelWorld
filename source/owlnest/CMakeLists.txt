#
#  Main Executable
#
file(GLOB_RECURSE SRCS
        *.cpp
        )
file(GLOB_RECURSE HDRS
        *.h
        )
add_executable(${CMAKE_PROJECT_NAME}nest
        ${SRCS} ${HDRS})
set_target_properties(${CMAKE_PROJECT_NAME}nest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${CMAKE_PROJECT_NAME}nest PRIVATE
        ${ENGINE_NAME}
        )

target_compile_definitions(${CMAKE_PROJECT_NAME}nest PRIVATE OWL_ASSETS_LOCATION="source/owlnest/assets")

# magic_enum
target_link_libraries(${CMAKE_PROJECT_NAME}nest PRIVATE owl::MagicEnum)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}nest PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.ini"
        VERBATIM)
install(TARGETS ${CMAKE_PROJECT_NAME}nest)
