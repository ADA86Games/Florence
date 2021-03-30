cmake_minimum_required(VERSION 3.10)


# set the project name
project(Florence)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

# add the executable
add_executable(florence src/florence.cpp)
target_link_libraries(florence fscompiler futil)
