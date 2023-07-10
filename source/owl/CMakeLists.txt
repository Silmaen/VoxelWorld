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
find_package(debugbreak 1.0 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC debugbreak::debugbreak)

# Entt
find_package(EnTT 3.11 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC EnTT::EnTT)

# fmt
find_package(fmt 9.1 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC fmt::fmt)

# glad
find_package(glad 0.1.36 REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE glad::glad)

# GLFW
find_package(glfw3 3.3.8 REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

# glm
find_package(glm 0.9.9 REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC glm::glm)

# ImGui
find_package(imgui REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC imgui::imgui)
find_package(imguizmo REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC imguizmo::imguizmo)

# magic_enum
find_package(magic_enum REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE magic_enum::magic_enum)

# yaml
find_package(yaml-cpp REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE yaml-cpp)

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(${ENGINE_NAME} PUBLIC spdlog::spdlog)

# stb_image
find_package(stb_image REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE stb_image::stb_image)

# NativeFile Dialog
find_package(nfd REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE nfd::nfd)

# Spir-V cross
if (${PRJPREFIX}_BUILD_SHARED)
    find_package(spirv_cross_c_shared REQUIRED)
    target_link_libraries(${ENGINE_NAME} PRIVATE spirv-cross-c-shared)
else ()
    find_package(spirv_cross_core REQUIRED)
    find_package(spirv_cross_glsl REQUIRED)
    find_package(spirv_cross_hlsl REQUIRED)
    find_package(spirv_cross_msl REQUIRED)
    find_package(spirv_cross_cpp REQUIRED)
    find_package(spirv_cross_reflect REQUIRED)
    find_package(spirv_cross_c REQUIRED)
    target_link_libraries(${ENGINE_NAME} PRIVATE spirv-cross-c)
endif ()
# shaderc
find_package(SPIRV-Tools REQUIRED)
find_package(SPIRV-Tools-opt REQUIRED)
find_package(glslang REQUIRED)
find_package(shaderc REQUIRED)
if (${PRJPREFIX}_BUILD_SHARED OR WIN32)
    target_link_libraries(${ENGINE_NAME} PRIVATE shaderc::shaderc_shared)
    if (WIN32)
        add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${shaderc_DIR}/../../../bin/libshaderc_shared.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                COMMAND_EXPAND_LISTS
                )
    endif ()
else ()
    target_link_libraries(${ENGINE_NAME} PRIVATE shaderc::shaderc)
endif ()

if (${PRJPREFIX}_BUILD_SHARED AND WIN32)
    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:${ENGINE_NAME}>" "$<TARGET_FILE_DIR:${ENGINE_NAME}>"
            COMMAND_EXPAND_LISTS
            )
endif ()
