
find_program(EDEPMANAGER depmanager)
if (${EDEPMANAGER} STREQUAL EDEPMANAGER-NOTFOUND)
    message(FATAL_ERROR "Dependency manager not found.")
else ()
    execute_process(COMMAND ${EDEPMANAGER} info cmakedir
            OUTPUT_VARIABLE depmanager_path)
    string(STRIP ${depmanager_path} depmanager_path)
    string(REPLACE "\\" "/" depmanager_path ${depmanager_path})
    list(PREPEND CMAKE_MODULE_PATH ${depmanager_path})
    include(DepManager)
endif ()

if (${${PRJPREFIX}_BUILD_SHARED})
    set(LOCAL_KIND shared)
    message(STATUS "Loading SHARED third parties")
else ()
    set(LOCAL_KIND static)
    message(STATUS "Loading STATIC third parties")
endif ()

dm_load_environment(KIND ${LOCAL_KIND})

dm_get_glibc(GLIBC_VERSION)
if (${GLIBC_VERSION})
    set(${PRJPREFIX}_GLIBC_STR "glibc_${GLIBC_VERSION}")
endif ()
