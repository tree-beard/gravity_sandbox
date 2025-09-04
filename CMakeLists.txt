cmake_minimum_required(VERSION 3.10)
project("GravitySandbox")

set(UNIT_NAME GravitySandbox)

set(SRCS
    src/main.cpp
    src/body.cpp
    src/simulation.cpp
)
set(HDRS
    src/body.h
    src/simulation.h
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
    glm)