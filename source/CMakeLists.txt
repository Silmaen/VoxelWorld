#
# base library
#
file(GLOB_RECURSE SRCS
        vw/*.cpp
        )
file(GLOB_RECURSE HDRS
        vw/*.h
        )
add_library(${CMAKE_PROJECT_NAME}_lib
        STATIC
        ${SRCS} ${HDRS})
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC vw)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_PROJECT_NAME}_Base)

# ----==== third party ====----
# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE OpenGL::GL)

find_package(GLEW REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE GLEW::glew_s)

#find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE glfw)


# json
find_package(nlohmann_json 3.11.1 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE nlohmann_json::nlohmann_json)
# header-only

#fmt
find_package(fmt 8 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC fmt::fmt-header-only)

#spdlog
find_package(spdlog REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC spdlog::spdlog_header_only)
target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PUBLIC SPDLOG_FMT_EXTERNAL)

# magic_enum
find_package(MagicEnum REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC MagicEnum)
#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${CMAKE_PROJECT_NAME}_lib
        )
