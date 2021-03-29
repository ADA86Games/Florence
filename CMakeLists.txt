cmake_minimum_required(VERSION 3.10)

# set the project name
project(Florence)

add_subdirectory(src)

# add the executable
add_executable(florence src/florence.cpp)
target_link_libraries(florence fscompiler futil)
