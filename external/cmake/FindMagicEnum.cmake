
add_library(MagicEnum INTERFACE IMPORTED)

set(MagicEnum_DIR ${PROJECT_SOURCE_DIR}/external/magic_enum)
set(MagicEnum_INCLUDE_DIR ${MagicEnum_DIR}/include)

target_include_directories(MagicEnum SYSTEM INTERFACE ${MagicEnum_INCLUDE_DIR})

