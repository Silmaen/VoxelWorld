cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

set(GLM_VERSION "0.9.9")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)
#enable_testing()

if (GLM_BUILD_SHARED_LIBS)
    SET(BUILD_SHARED_LIBS ON)
else ()
    SET(BUILD_STATIC_LIBS ON)
endif ()

add_subdirectory(glm)
add_library(glm::glm ALIAS glm)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

    #add_subdirectory(test)

endif (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
