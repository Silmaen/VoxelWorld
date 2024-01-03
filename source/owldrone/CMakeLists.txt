#
#  Main Executable
#
file(GLOB_RECURSE SRCS
        sources/*.cpp
)
file(GLOB_RECURSE HDRS
        sources/*.h
)
add_executable(${CMAKE_PROJECT_NAME}drone
        ${SRCS} ${HDRS})
set_target_properties(${CMAKE_PROJECT_NAME}drone PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_include_directories(${CMAKE_PROJECT_NAME}drone PRIVATE sources)

target_link_libraries(${CMAKE_PROJECT_NAME}drone PRIVATE
        ${ENGINE_NAME}
)

target_compile_definitions(${CMAKE_PROJECT_NAME}drone PRIVATE OWL_ASSETS_LOCATION="source/owldrone/assets")

# ----==== third party ====----

# OpenCV
#find_package(OpenCV 4.5.0 REQUIRED)
#target_link_libraries(${CMAKE_PROJECT_NAME}drone PUBLIC opencv_videoio)

# yaml
find_package(yaml-cpp REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}drone PRIVATE yaml-cpp)

if (OWL_PLATFORM_WINDOWS)
    # for web cam identification
    target_link_libraries(${CMAKE_PROJECT_NAME}drone PRIVATE strmiids)
endif ()

# ----==== third party ====----

if (${PRJPREFIX}_BUILD_SHARED AND WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}drone POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}drone>" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}drone>"
            COMMAND_EXPAND_LISTS
    )
endif ()

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}drone PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.ini"
        VERBATIM)
install(TARGETS ${CMAKE_PROJECT_NAME}drone)
