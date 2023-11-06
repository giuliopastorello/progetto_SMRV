cmake_minimum_required(VERSION 3.16)

project(cell CXX)
enable_testing()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

find_package(SFML COMPONENTS graphics REQUIRED)

add_executable(cell_sfml visual.cpp world.cpp main.cpp)
<<<<<<< HEAD
target_compile_options(cell_sfml PRIVATE -Wall -Wextra -fsanitize=address -g)
=======
target_compile_options(cell_sfml PRIVATE -Wall -Wextra -g) #-fsanitize=address
>>>>>>> e0475911350e6b6f273991ce904aea9bb8558cb5
target_link_libraries(cell_sfml PRIVATE sfml-graphics)
target_link_options(cell_sfml PRIVATE ) #-fsanitize=address 