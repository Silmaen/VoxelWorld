#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME}_sandbox main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_sandbox
        ${CMAKE_PROJECT_NAME}_lib
        )
install(TARGETS ${CMAKE_PROJECT_NAME}_sandbox)
