cmake_minimum_required(VERSION 3.20)
project(gen-dummy_projects)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} boost_filesystem)