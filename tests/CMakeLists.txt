add_executable(dfa_test dfa_test.cc)
add_executable(operator_dfa_test operator_dfa_test.cc)

# Link GoogleTest
target_link_libraries(dfa_test PRIVATE
    handler_manager
    lexeme_context
    handlers
    position
    operator_map
    GTest::gtest_main
)

target_link_libraries(operator_dfa_test PRIVATE
    handler_manager
    lexeme_context
    handlers
    position
    operator_map
    GTest::gtest_main
)

# Register tests
add_test(NAME DFATest COMMAND dfa_test)
add_test(NAME OperatorInDFATest COMMAND operator_dfa_test)
