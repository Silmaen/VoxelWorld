#
# the third parties
#

#
# include targets for googletest
add_subdirectory(GoogleTest)

#
# include targets for glad
add_subdirectory(glad)

#
# include targets for imgui
option(IMGUI_USE_DOCKING "Use experimental version of ImGui that has Docking system" ON)
if (IMGUI_USE_DOCKING)
    add_subdirectory(imgui/docking)
else ()
    add_subdirectory(imgui/1.89.1)
endif ()
