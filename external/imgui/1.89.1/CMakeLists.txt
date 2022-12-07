#
# Cmake for ImGui
#

file(GLOB SRCS
        *.cpp
        )
file(GLOB HDRS
        *.h
        )

add_library(imgui ${SRCS} ${HDRS})


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_include_directories(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} backends)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# we are only using the glfw version
target_sources(imgui PRIVATE backends/imgui_impl_glfw.cpp backends/imgui_impl_glfw.h)
find_package(glfw3 REQUIRED)
target_link_libraries(imgui PRIVATE glfw)

target_sources(imgui PRIVATE backends/imgui_impl_opengl3.cpp backends/imgui_impl_opengl3.h backends/imgui_impl_opengl3_loader.h)


if(OWL_PLATFORM_WINDOWS)
    target_sources(imgui PRIVATE backends/imgui_impl_win32.cpp backends/imgui_impl_win32.h)
elseif(OWL_PLATFORM_LINUX)

endif()

add_library(imgui_client INTERFACE IMPORTED)
target_include_directories(imgui_client INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR})
