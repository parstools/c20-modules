cmake_minimum_required(VERSION 3.24)
project(make_projects)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(make_projects boost_filesystem)