cmake_minimum_required(VERSION 3.16)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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

target_include_directories(tokenizer PUBLIC include)
target_include_directories(token PUBLIC include)
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
    position
    exceptions
)

# Enable testing
enable_testing()

# Add GoogleTest
include(cmake/FetchGoogleTest.cmake)

# Add test directory
add_subdirectory(tests)
