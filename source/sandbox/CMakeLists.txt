#
#  Main Executable
#
file(GLOB_RECURSE SRCS
        *.cpp
)
file(GLOB_RECURSE HDRS
        *.h
)
add_executable(${CMAKE_PROJECT_NAME}_sandbox
        ${SRCS} ${HDRS})
set_target_properties(${CMAKE_PROJECT_NAME}_sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(${CMAKE_PROJECT_NAME}_sandbox
        ${ENGINE_NAME}
)
if (${PRJPREFIX}_PACKAGE_ENGINE)
    foreach (config IN ITEMS Debug Release)
        install(TARGETS ${CMAKE_PROJECT_NAME}_sandbox
                CONFIGURATIONS ${config}
                LIBRARY DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                ARCHIVE DESTINATION ${${PRJPREFIX}_INSTALL_LIB}/${config}
                RUNTIME DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                FRAMEWORK DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                COMPONENT Engine
        )
    endforeach ()
else ()
    install(TARGETS ${CMAKE_PROJECT_NAME}_sandbox
            LIBRARY DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
            RUNTIME DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
            FRAMEWORK DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
    )
endif ()