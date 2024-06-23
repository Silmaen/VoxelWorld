#
#
#
add_subdirectory(owl)
if (${PRJPREFIX}_BUILD_NEST)
    add_subdirectory(owlnest)
endif ()
if (${PRJPREFIX}_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif ()
if (${PRJPREFIX}_BUILD_DRONE)
    add_subdirectory(owldrone)
endif ()
if (${PRJPREFIX}_BUILD_CAST)
    add_subdirectory(owlcast)
endif ()
