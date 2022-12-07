
add_library(DebugBreak INTERFACE IMPORTED)

set(DebugBreak_DIR ${PROJECT_SOURCE_DIR}/external/debugbreak)
set(DebugBreak_INCLUDE_DIR ${DebugBreak_DIR})

target_include_directories(DebugBreak SYSTEM INTERFACE ${DebugBreak_INCLUDE_DIR})

