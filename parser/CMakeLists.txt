# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

SET (CMAKE_CXX_STANDARD 17)

# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.12.0-complete.jar)
# add external build for antlrcpp
include( ExternalAntlr4Cpp )
# add antrl4cpp artifacts to project environment
include_directories( thirdparty/cpp_runtime/runtime/src )
link_directories( thirdparty/cpp_runtime/dist )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")

# Call macro to add grammar to your build dependencies.
antlr4cpp_process_grammar_one(demo cmake_parser
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake.g4)

# include generated files in project environment
include_directories(${antlr4cpp_include_dirs_cmake_parser})

# add generated grammar to demo binary target
add_executable(cmake_parser main.cpp ${antlr4cpp_src_files_cmake_parser})
add_dependencies(cmake_parser antlr4cpp_generation_cmake_parser)
target_link_libraries(cmake_parser antlr4-runtime)
