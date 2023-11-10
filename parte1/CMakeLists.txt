cmake_minimum_required(VERSION 3.27.6)

project(infection CXX)
enable_testing()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_subdirectory(matplotplusplus)

add_executable(infection_tty infection.cpp display.cpp main.cpp)
target_compile_options(infection_tty PRIVATE  -Wall -Wextra -g -std=c++17) #-fsanitize=address 
target_link_libraries(infection_tty PUBLIC matplot) # -fsanitize=address)

add_executable(infection.test infection.cpp infection.test.cpp)
target_compile_options(infection.test PRIVATE -Wall -Wextra -g) #-fsanitize=address 
target_link_options(infection.test PRIVATE) #-fsanitize=address)
add_test(NAME infection.test COMMAND infection.test)