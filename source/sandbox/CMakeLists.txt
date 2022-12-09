#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME}_sandbox main.cpp)
set_target_properties(${CMAKE_PROJECT_NAME}_sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${CMAKE_PROJECT_NAME}_sandbox
        ${ENGINE_NAME}
        )
install(TARGETS ${CMAKE_PROJECT_NAME}_sandbox)
