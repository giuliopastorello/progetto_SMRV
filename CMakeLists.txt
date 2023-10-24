cmake_minimum_required(VERSION 3.16)

project(infection CXX)
enable_testing()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

include(FetchContent)

FetchContent_Declare(
        matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master
    )

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(infection_tty infection.cpp rk.cpp display.cpp main.cpp)
target_compile_options(infection_tty PRIVATE -Wall -Wextra -g -std=c++17)
target_link_libraries(infection_tty PUBLIC matplot)
target_link_options(infection_tty PRIVATE)