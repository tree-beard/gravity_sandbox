cmake_minimum_required(VERSION 3.10)
project("GravitySandbox")

set(UNIT_NAME GravitySandbox)

set(SRCS
    src/main.cpp
)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Setting a default.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(${UNIT_NAME} ${SRCS})

add_subdirectory(raylib)
target_link_libraries(${UNIT_NAME} PUBLIC raylib)
target_include_directories(${UNIT_NAME} PUBLIC src PUBLIC raylib/src)