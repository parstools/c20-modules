# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(parser)

SET (CMAKE_CXX_STANDARD 17)

# set variable pointing to the antlr tool that supports C++
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.12.0-complete.jar)
# add external build for antlrcpp
INCLUDE(cmake/ExternalAntlr4Cpp.cmake)
# add antrl4cpp artifacts to project environment
include_directories( thirdparty/cpp_runtime/runtime/src )
link_directories( thirdparty/cpp_runtime/dist )
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")

# Call macro to add grammar to your build dependencies.
antlr4cpp_process_grammar_one(cmake_parser
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake.g4)

# Call macro to add lexer and grammar to your build dependencies.
antlr4cpp_process_grammar(cpp_parser
  ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Lexer.g4
  ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Parser.g4)

fun_expr(NAME format_name(get_first_and_last()))
#fun_expr(NAME antlr4cpp_process_grammar1(cpp_parser
#        ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Lexer.g4
#        ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Parser.g4))
message(WARNING ${NAME})
separate_arguments(out UNIX_COMMAND ${NAME})
foreach(generated_file ${out})
    message(STATUS "MyTests Variable:  ${generated_file}")
endforeach(generated_file)

# add generated grammar to demo binary target
add_executable(cmake_parser main.cpp ${antlr4cpp_src_files_cmake_parser})
# include generated files in project environment
target_include_directories(cmake_parser PRIVATE ${antlr4cpp_include_dirs_cmake_parser})
add_dependencies(cmake_parser antlr4cpp_generation_cmake_parser)
target_link_libraries(cmake_parser antlr4-runtime)

# add generated grammar to demo binary target
add_executable(cpp_parser main.cpp ${antlr4cpp_src_files_cmake_parser})
# include generated files in project environment
target_include_directories(cpp_parser PRIVATE ${antlr4cpp_include_dirs_cpp_parser})
add_dependencies(cpp_parser antlr4cpp_generation_cpp_parser)
target_link_libraries(cpp_parser antlr4-runtime)
