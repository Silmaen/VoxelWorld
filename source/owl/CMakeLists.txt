#
# base library
#
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
    add_library(${ENGINE_NAME}
            SHARED
            ${SRCS} ${HDRS})
    target_compile_definitions(${ENGINE_NAME} PUBLIC ${PRJPREFIX}_BUILD_SHARED)
    target_compile_definitions(${ENGINE_NAME} PRIVATE ${PRJPREFIX}_BUILD_DLL)
    set_target_properties(${ENGINE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else ()
    add_library(${ENGINE_NAME}
            STATIC
            ${SRCS} ${HDRS})
endif ()
target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${ENGINE_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_Base)
install(TARGETS ${ENGINE_NAME})
# precompiled headers
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PRJPREFIX_LOWER}pch.h")
    target_precompile_headers(${ENGINE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${PRJPREFIX_LOWER}pch.h)
endif ()

# ----==== third party ====----
#  ---- external folder ----
# debugbreak
find_package(DebugBreak 1.0 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC owl::DebugBreak)

# Entt
find_package(Entt 3.11 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC owl::Entt)

# fmt
target_link_libraries(${ENGINE_NAME} PUBLIC fmt::fmt)

# glad
target_link_libraries(${ENGINE_NAME} PRIVATE glad)

# GLFW
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

# glm
target_link_libraries(${ENGINE_NAME} PRIVATE glm_shared)

# ImGui
target_link_libraries(${ENGINE_NAME} PUBLIC imgui)
find_package(ImGuizmo REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC owl::ImGuizmo)

# magic_enum
find_package(MagicEnum REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE owl::MagicEnum)

# yaml
target_link_libraries(${ENGINE_NAME} PRIVATE yaml-cpp::yaml-cpp)

# spdlog
target_link_libraries(${ENGINE_NAME} PUBLIC spdlog::spdlog)

# magic_enum
find_package(StbImage REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE owl::StbImage)

# NativeFile Dialog
target_link_libraries(${ENGINE_NAME} PRIVATE nfd)

# Spir-V
target_link_libraries(${ENGINE_NAME} PRIVATE spirv-cross-cpp)
target_include_directories(${ENGINE_NAME} PRIVATE ${${PRJPREFIX}_EXTERNAL_DIR}/SPIRV-Cross)
# shaderc
target_link_libraries(${ENGINE_NAME} PRIVATE shaderc_shared)

