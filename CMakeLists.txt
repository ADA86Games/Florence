cmake_minimum_required(VERSION 3.10)


# set the project name
project(Florence C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

# add the executable
add_executable(florence src/florence.cpp src/linker.cpp src/linker.hpp src/assembler.cpp src/assembler.hpp)
target_link_libraries(florence fscompiler futil)
