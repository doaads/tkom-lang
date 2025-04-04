add_executable(dfa_test dfa_test.cc)

# Link GoogleTest
#target_link_libraries(dfa_test PRIVATE GTest::gtest_main)
target_link_libraries(dfa_test PRIVATE
    handler_manager
    handlers
    position
    GTest::gtest_main
)

# Register tests
add_test(NAME CompilerTests COMMAND dfa_test)
