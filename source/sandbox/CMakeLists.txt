#
#  Main Executable
#
set(OWL_PROJECT ${CMAKE_PROJECT_NAME}Sandbox)

file(GLOB_RECURSE SRCS
        sources/*.cpp
)
file(GLOB_RECURSE HDRS
        sources/*.h
)
add_executable(${OWL_PROJECT}
        ${SRCS} ${HDRS})
set_target_properties(${OWL_PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_include_directories(${OWL_PROJECT} PRIVATE sources)

target_link_libraries(${OWL_PROJECT} PRIVATE
        ${ENGINE_NAME}
)

target_import_so_files(${OWL_PROJECT})
# ----==== third party ====----
# ----==== third party ====----

if (${PRJPREFIX}_BUILD_SHARED AND WIN32)
    add_custom_command(TARGET ${OWL_PROJECT} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:${OWL_PROJECT}>" "$<TARGET_FILE_DIR:${OWL_PROJECT}>"
            COMMAND_EXPAND_LISTS
    )
endif ()

if (${PRJPREFIX}_PACKAGE_ENGINE)
    foreach (config IN ITEMS Debug Release)
        install(TARGETS ${OWL_PROJECT}
                CONFIGURATIONS ${config}
                LIBRARY DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                ARCHIVE DESTINATION ${${PRJPREFIX}_INSTALL_LIB}/${config}
                RUNTIME DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                FRAMEWORK DESTINATION ${${PRJPREFIX}_INSTALL_BIN}/${config}
                COMPONENT Engine
        )
    endforeach ()
else ()
    install(TARGETS ${OWL_PROJECT}
            LIBRARY DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
            RUNTIME DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
            FRAMEWORK DESTINATION ${${PRJPREFIX}_INSTALL_BIN}
    )
endif ()