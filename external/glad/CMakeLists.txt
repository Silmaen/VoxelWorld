#
# CmakeLists for Glad
#
add_library(glad src/glad.c include/glad/glad.h include/KHR/khrplatform.h)

set_source_files_properties(src/glad.c LANGUAGE C)

target_include_directories(glad PUBLIC include)
