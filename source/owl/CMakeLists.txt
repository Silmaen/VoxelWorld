#
# base library
#
option(${PRJPREFIX}_BUILD_SHARED "If the Library must be build as shared library" OFF)
file(GLOB_RECURSE SRCS
        *.cpp
        )
file(GLOB_RECURSE HDRS
        *.h
        )
list(REMOVE_ITEM
        SRCS
        owlpch.cpp)
list(REMOVE_ITEM
        HDRS
        owlpch.h)
if (${PRJPREFIX}_BUILD_SHARED)
    add_library(${CMAKE_PROJECT_NAME}_lib
            SHARED
            ${SRCS} ${HDRS})
    target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PUBLIC ${PRJPREFIX}_BUILD_SHARED)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PRIVATE ${PRJPREFIX}_BUILD_DLL)
    set_target_properties(${CMAKE_PROJECT_NAME}_lib PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else ()
    add_library(${CMAKE_PROJECT_NAME}_lib
            STATIC
            ${SRCS} ${HDRS})
endif ()
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_PROJECT_NAME}_Base)
install(TARGETS ${CMAKE_PROJECT_NAME}_lib)
# precompiled headers
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PRJPREFIX_LOWER}pch.h")
  target_precompile_headers(${CMAKE_PROJECT_NAME}_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${PRJPREFIX_LOWER}pch.h)
endif()

# ----==== third party ====----
# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE OpenGL::GL)

# glad
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE glad)

# GLFW
find_package(glfw3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE glfw)

# glm
find_package(glm REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE glm)

# ImGui
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC imgui)
#target_link_libraries(${CMAKE_PROJECT_NAME}_lib INTERFACE imgui_client)

# debugbreak
find_package(debugbreak 8 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC DebugBreak)

# json
find_package(nlohmann_json 3.10 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC nlohmann_json::nlohmann_json)
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