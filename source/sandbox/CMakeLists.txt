#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME}_sandbox main.cpp)
set_target_properties(${CMAKE_PROJECT_NAME}_sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${CMAKE_PROJECT_NAME}_sandbox
        ${CMAKE_PROJECT_NAME}_lib
        )
install(TARGETS ${CMAKE_PROJECT_NAME}_sandbox)
