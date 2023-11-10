cmake_minimum_required(VERSION 3.16)

project(cell CXX)
enable_testing()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

find_package(SFML COMPONENTS graphics REQUIRED)

add_executable(virus_sfml visual.cpp world.cpp main.cpp)
target_compile_options(virus_sfml PRIVATE -Wall -Wextra -g -fsanitize=address -std=c++17)
target_link_libraries(virus_sfml PRIVATE sfml-graphics)
target_link_options(virus_sfml PRIVATE -fsanitize=address) 

add_executable(virus.test world.cpp world.test.cpp)
target_compile_options(virus.test PRIVATE -Wall -Wextra -g -fsanitize=address -std=c++17)
target_link_options(virus.test PRIVATE -fsanitize=address)
add_test(NAME virus.test COMMAND virus.test)