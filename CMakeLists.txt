cmake_minimum_required(VERSION 3.10)
project("GravitySandbox")

set(UNIT_NAME GravitySandbox)

set(SRCS
    src/main.cpp
    src/bodies_generator.cpp
    src/drawable_body.cpp
    src/physics/barnes_hut_simulation.cpp
    src/physics/body.cpp
    src/physics/simulation_controller.cpp
)
set(HDRS
    src/bodies_generator.h
    src/drawable_body.h
    src/physics/barnes_hut_simulation.h
    src/physics/body.h
    src/physics/simulation_controller.h
    src/base/quadtree.h
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