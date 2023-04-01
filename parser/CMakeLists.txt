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

function(get_arguments_cmake ARGS)
    return_([[cmake_parser]] ${CMAKE_CURRENT_SOURCE_DIR}/CMake.g4)
endfunction()

fun_expr(generated_files_cmake_space antlr4cpp_process_grammar_one(get_arguments_cmake()))
separate_arguments(generated_files_cmake UNIX_COMMAND ${generated_files_cmake_space})

# Call macro to add lexer and grammar to your build dependencies.
function(get_arguments_cpp ARGS)
    set(VAR cpp_parser
            ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Lexer.g4
            ${CMAKE_CURRENT_SOURCE_DIR}/CPP14Parser.g4)
    return_(${VAR})
endfunction()

fun_expr(generated_files_cpp_space antlr4cpp_process_grammar(get_arguments_cpp()))
separate_arguments(generated_files_cpp UNIX_COMMAND ${generated_files_cpp_space})

add_executable(cmake_parser cmake_driver.cpp ${generated_files_cmake})
target_include_directories(cmake_parser PRIVATE ${CMAKE_BINARY_DIR}/gen/cmake_parser)
add_dependencies(cmake_parser gen_cmake_parser)
target_link_libraries(cmake_parser antlr4-runtime)

add_executable(cpp_parser cpp_driver.cpp ${generated_files_cpp})
target_include_directories(cpp_parser PRIVATE ${CMAKE_BINARY_DIR}/gen/cpp_parser)
add_dependencies(cpp_parser gen_cpp_parser)
target_link_libraries(cpp_parser antlr4-runtime)
