#
# CmakeLists for Glad
#
set(GLAD_SOURCE src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
if(${PRJPREFIX}_BUILD_SHARED)
add_library(glad SHARED ${GLAD_SOURCE})
else()
add_library(glad STATIC ${GLAD_SOURCE})
endif()

set_source_files_properties(src/glad.c LANGUAGE C)

target_include_directories(glad PRIVATE include)
target_include_directories(glad SYSTEM INTERFACE include)
