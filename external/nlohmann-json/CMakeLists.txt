#
#
add_library(nlohmann_json INTERFACE)
add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)

option(nlohmann_json_SINGLE_INCLUDE "If you want the nlohman single incle version" ON)

set(NlohmannJson_DIR ${PROJECT_SOURCE_DIR}/external/nlohmann-json)
if (nlohmann_json_SINGLE_INCLUDE)
    set(NlohmannJson_INCLUDE_DIR ${NlohmannJson_DIR}/single_include)
else ()
    set(NlohmannJson_INCLUDE_DIR ${NlohmannJson_DIR}/include)
endif ()

target_include_directories(nlohmann_json SYSTEM INTERFACE ${NlohmannJson_INCLUDE_DIR})
