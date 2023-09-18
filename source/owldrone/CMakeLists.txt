#
#  Main Executable
#
file(GLOB_RECURSE SRCS
        *.cpp
)
file(GLOB_RECURSE HDRS
        *.h
)
add_executable(${CMAKE_PROJECT_NAME}drone
        ${SRCS} ${HDRS})
set_target_properties(${CMAKE_PROJECT_NAME}drone PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${CMAKE_PROJECT_NAME}drone PRIVATE
        ${ENGINE_NAME}
)

target_compile_definitions(${CMAKE_PROJECT_NAME}drone PRIVATE OWL_ASSETS_LOCATION="source/owldrone/assets")


add_custom_command(TARGET ${CMAKE_PROJECT_NAME}drone PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.ini"
        VERBATIM)
install(TARGETS ${CMAKE_PROJECT_NAME}drone)
