
set(ImGuizmo_DIR ${PROJECT_SOURCE_DIR}/external/ImGuizmo)
set(SRCS
        ${ImGuizmo_DIR}/GraphEditor.cpp
        ${ImGuizmo_DIR}/ImCurveEdit.cpp
        ${ImGuizmo_DIR}/ImGradient.cpp
        ${ImGuizmo_DIR}/ImGuizmo.cpp
        ${ImGuizmo_DIR}/ImSequencer.cpp)
set(HDRS
        ${ImGuizmo_DIR}/GraphEditor.h
        ${ImGuizmo_DIR}/ImCurveEdit.h
        ${ImGuizmo_DIR}/ImGradient.h
        ${ImGuizmo_DIR}/ImGuizmo.h
        ${ImGuizmo_DIR}/ImSequencer.h
        ${ImGuizmo_DIR}/ImZoomSlider.h)

if (${PRJPREFIX}_BUILD_SHARED)
    add_library(ImGuizmo_lib SHARED ${SRCS} ${HDRS})
else ()
    add_library(ImGuizmo_lib STATIC ${SRCS} ${HDRS})
endif ()
target_include_directories(ImGuizmo_lib PUBLIC ${ImGuizmo_DIR})
target_link_libraries(ImGuizmo_lib PRIVATE imgui)


add_library(owl::ImGuizmo INTERFACE IMPORTED)
target_link_libraries(owl::ImGuizmo INTERFACE ImGuizmo_lib)
