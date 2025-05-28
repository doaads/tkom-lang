add_executable(tokenizer_test tokenizer_test.cc)
add_executable(lexer_test lexer_test.cc)
add_executable(lexer_all_tokens_test lexer_all_tokens_test.cc)
add_executable(parser_test parser_test.cc)
add_executable(parser_parametrized_test parser_parametrized_test.cc)
add_executable(interpreter_test interpreter_test.cc)

add_library(parser_test_lib INTERFACE)
add_library(interpreter_test_lib INTERFACE)

target_include_directories(parser_test_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(tokenizer_test PRIVATE
    tokenizer
    token
    input
    position
    exceptions
    GTest::gtest_main
)

target_link_libraries(lexer_test PRIVATE
    lexer
    tokenizer
    token
    input
    position
    exceptions
    GTest::gtest_main
)

target_link_libraries(lexer_all_tokens_test PRIVATE
    lexer
    tokenizer
    token
    input
    position
    exceptions
    GTest::gtest_main
)

target_link_libraries(parser_test_lib INTERFACE
    lexer
    parser
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
    GTest::gtest_main
)

target_link_libraries(interpreter_test_lib INTERFACE
    parser_test_lib
    interpreter
    local_function
    builtins
)

target_link_libraries(interpreter_test PRIVATE interpreter_test_lib)

target_link_libraries(parser_test PRIVATE parser_test_lib)

target_link_libraries(parser_parametrized_test PRIVATE parser_test_lib)

# Register tests
add_test(NAME TokenizerTest COMMAND tokenizer_test)
add_test(NAME LexerTest COMMAND lexer_test)
add_test(NAME LexerAllTokensTest COMMAND lexer_all_tokens_test)
add_test(NAME ParserTest COMMAND parser_test)
add_test(NAME ParserParametrizedTest COMMAND parser_parametrized_test)
add_test(NAME InterpreterTest COMMAND interpreter_test)
