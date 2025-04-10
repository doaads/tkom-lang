#include <gtest/gtest.h>
#include "lexer.h"

#define VERBOSE true

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<std::stringstream> input = std::make_unique<std::stringstream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}

TEST(LexerIdentifier, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test");
    Token token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test");
}

TEST(LexerIdentifiers, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test          test2");
    Token token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test");
    token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test2");
}

TEST(LexerIdentifierNewline, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test\ntest2");
    Token token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test");
    token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test2");
}

TEST(LexerOperator, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("->");
    Token token = lexer.get_token();
    EXPECT_EQ(token.get_type(), TokenType::T_CALL);
}
