cmake_minimum_required(VERSION 3.10)
project("GravitySandbox")

set(UNIT_NAME GravitySandbox)

set(SRCS
    src/main.cpp
    src/base/quadtree.ipp
    src/controllers/simulation_controller.cpp
    src/physics/body.cpp
    src/physics/nbody_simulation.cpp
    src/graphics/drawable_body.cpp
    src/utils/bodies_generator.cpp
)
set(HDRS
    src/base/quadtree.h
    src/controllers/simulation_controller.h
    src/physics/body.h
    src/physics/nbody_simulation.h
    src/graphics/drawable_body.h
    src/utils/bodies_generator.h
)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Setting a default.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(${UNIT_NAME} ${HDRS} ${SRCS})

add_subdirectory(raylib)
add_subdirectory(glm)
target_link_libraries(${UNIT_NAME} PUBLIC 
    raylib 
    glm::glm)
target_include_directories(${UNIT_NAME} PUBLIC 
    raylib/src 
    glm
    src)

macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            GroupSources(${curdir}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        endif()
    endforeach()
endmacro()

GroupSources(/)