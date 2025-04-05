cmake_minimum_required(VERSION 3.16)
project(Compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(position src/position.cpp)
add_library(handlers src/handlers.cpp)
add_library(lexeme_context src/lexeme_context.cpp)
add_library(handler_manager src/handler_manager.cpp)
add_library(operator_map src/operator_map.cpp)
target_include_directories(position PUBLIC include)
target_include_directories(handlers PUBLIC include)
target_include_directories(lexeme_context PUBLIC include)
target_include_directories(handler_manager PUBLIC include)
target_include_directories(operator_map PUBLIC include)

# Enable testing
enable_testing()

# Add GoogleTest
include(cmake/FetchGoogleTest.cmake)

# Add test directory
add_subdirectory(tests)
