#include <gtest/gtest.h>
#include "lexer.h"

#define VERBOSE true

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<InputStream> input = std::make_unique<StringInputStream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}

TEST(LexerIdentifier, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test");
    Token token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(token.value), "test");
}

TEST(LexerIdentifiers, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test          test2");
    Token token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(token.value), "test");
    token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(token.value), "test2");
}

TEST(LexerIdentifierNewline, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("test\ntest2");
    Token token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(token.value), "test");
    token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(token.value), "test2");
}

TEST(LexerOperator, LexerTestBasic) {
    Lexer lexer = get_lexer_for_string("->");
    Token token = lexer.get_token();
    EXPECT_EQ(token.type, TokenType::T_CALL);
}
