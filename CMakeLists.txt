cmake_minimum_required(VERSION 3.16)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Boost REQUIRED COMPONENTS program_options)

include_directories(include)

add_library(tokenizer src/tokenizer/tokenizer.cpp)
add_library(token src/token/token.cpp src/token/tokens.cpp)
add_library(input src/input/input_manager.cpp)
add_library(position src/position/position.cpp)
add_library(lexer src/lexer.cpp)
add_library(exceptions src/exceptions/exceptions.cpp)

add_library(expression src/parser/expression.cpp)
add_library(function src/parser/function.cpp)
add_library(statement src/parser/statement.cpp)
add_library(type src/parser/type.cpp)
add_library(variable src/parser/variable.cpp)
add_library(block src/parser/block.cpp)
add_library(operators src/parser/operators.cpp)
add_library(program src/parser/program.cpp)
add_library(visitor src/parser/visitor.cpp)
add_library(print_error src/exceptions/print_error.cpp)
add_library(parser src/parser/parser.cpp)

add_library(local_function src/interpreter/local_function.cpp)
add_library(builtins
    src/interpreter/builtins.cpp
    src/interpreter/builtin_helpers.cpp
)
add_library(interpreter
    src/interpreter/interpreter_expr.cpp
    src/interpreter/interpreter.cpp
    src/interpreter/interpreter_helpers.cpp
    src/interpreter/tkom_interpreter.cpp
)

target_include_directories(tokenizer PUBLIC include)
target_include_directories(token PUBLIC include)
target_include_directories(input PUBLIC include)
target_include_directories(position PUBLIC include)
target_include_directories(lexer PUBLIC include)
target_include_directories(exceptions PUBLIC include)

target_include_directories(expression PUBLIC include)
target_include_directories(function PUBLIC include)
target_include_directories(statement PUBLIC include)
target_include_directories(type PUBLIC include)
target_include_directories(variable PUBLIC include)
target_include_directories(operators PUBLIC include)
target_include_directories(block PUBLIC include)
target_include_directories(program PUBLIC include)
target_include_directories(visitor PUBLIC include)
target_include_directories(print_error PUBLIC include)
target_include_directories(parser PUBLIC include)

target_include_directories(interpreter PUBLIC include)
target_include_directories(local_function PUBLIC include)
target_include_directories(builtins PUBLIC include)

add_library(tkom-parser-lib INTERFACE)
add_executable(tkom-parser src/parser_main.cpp)
target_include_directories(tkom-parser PRIVATE include)
target_link_libraries(tkom-parser-lib INTERFACE
    Boost::program_options
    lexer
    parser
    print_error
    visitor
    program
    block
    tokenizer
    token
    input
    position
    exceptions
    expression
    function
    statement
    type
    variable
    operators
)
target_link_libraries(tkom-parser PRIVATE tkom-parser-lib)

add_executable(tkom-lexer src/lexer_main.cpp)
target_include_directories(tkom-lexer PRIVATE include)
target_link_libraries(tkom-lexer PRIVATE
    Boost::program_options
    lexer
    parser
    tokenizer
    token
    input
    position
    exceptions
)

add_executable(tkom src/tkom.cpp)
target_include_directories(tkom PRIVATE include)
target_link_libraries(tkom PRIVATE
    interpreter
    builtins
    local_function
    tkom-parser-lib
)

# Enable testing
enable_testing()

# Add GoogleTest
include(cmake/FetchGoogleTest.cmake)

# Add test directory
add_subdirectory(tests)
