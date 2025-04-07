add_executable(dfa_test dfa_test.cc)
add_executable(operator_dfa_test operator_dfa_test.cc)
add_executable(tokenizer_test tokenizer_test.cc)
add_executable(lexer_test lexer_test.cc)

# Link GoogleTest
target_link_libraries(dfa_test PRIVATE
    lexeme_context
    handlers
    position
    dfa_maps
    token
    tokenizer
    GTest::gtest_main
)

target_link_libraries(operator_dfa_test PRIVATE
    lexeme_context
    handlers
    position
    dfa_maps
    token
    tokenizer
    GTest::gtest_main
)

target_link_libraries(tokenizer_test PRIVATE
    tokenizer
    lexeme_context
    handlers
    dfa_maps
    token
    input
    position
    keyword_map
    GTest::gtest_main
)

target_link_libraries(lexer_test PRIVATE
    lexer
    tokenizer
    visitor
    lexeme_context
    handlers
    dfa_maps
    token
    input
    position
    keyword_map
    GTest::gtest_main
)

# Register tests
add_test(NAME DFATest COMMAND dfa_test)
add_test(NAME OperatorInDFATest COMMAND operator_dfa_test)
add_test(NAME TokenizerTest COMMAND tokenizer_test)
add_test(NAME LexerTest COMMAND lexer_test)
