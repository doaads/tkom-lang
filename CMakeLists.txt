cmake_minimum_required(VERSION 3.16)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Boost REQUIRED COMPONENTS program_options)

include_directories(include)

add_library(handlers src/handlers/handlers.cpp src/handlers/handler_manager.cpp)
add_library(lexeme_context src/lexeme_context.cpp)
add_library(dfa_maps src/maps/operator_map.cpp src/maps/special_sign_map.cpp)
add_library(keyword_map src/maps/keyword_map.cpp)
add_library(tokenizer src/tokenizer.cpp)
add_library(token src/token/token.cpp src/token/tokens.cpp)
add_library(visitor src/visitor.cpp)
add_library(input src/input/input_stream.cpp src/input/input_manager.cpp)
add_library(position src/position/position.cpp src/position/position_manager.cpp)
add_library(lexer src/lexer.cpp)
add_library(exceptions src/exceptions/exceptions.cpp src/exceptions/exception_handler.cpp)

target_include_directories(handlers PUBLIC include)
target_include_directories(lexeme_context PUBLIC include)
target_include_directories(dfa_maps PUBLIC include)
target_include_directories(keyword_map PUBLIC include)
target_include_directories(tokenizer PUBLIC include)
target_include_directories(token PUBLIC include)
target_include_directories(visitor PUBLIC include)
target_include_directories(input PUBLIC include)
target_include_directories(position PUBLIC include)
target_include_directories(lexer PUBLIC include)
target_include_directories(exceptions PUBLIC include)

add_executable(tkom src/lexer_main.cpp)
target_include_directories(tkom PRIVATE include)
target_link_libraries(tkom PRIVATE
    Boost::program_options
    lexer
    tokenizer
    token
    input
    handlers
    lexeme_context
    dfa_maps
    keyword_map
    visitor
    position
    exceptions
)

# Enable testing
enable_testing()

# Add GoogleTest
include(cmake/FetchGoogleTest.cmake)

# Add test directory
add_subdirectory(tests)
