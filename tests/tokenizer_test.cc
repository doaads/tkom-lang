#include <gtest/gtest.h>
#include "tokenizer.h"
#include "input_manager.h"


Tokenizer get_tokenizer_for_string(std::string string) {
    std::shared_ptr<std::stringstream> input = std::make_unique<std::stringstream>(string);
    std::shared_ptr<Position> position = std::make_unique<Position>();
    std::shared_ptr<InputManager>manager = std::make_unique<InputManager>(position, input);
    Tokenizer tokenizer(manager);
    return tokenizer;
}

Token get_token_from_string(std::string string) {
    Tokenizer tokenizer = get_tokenizer_for_string(string);
    return tokenizer.get_token();
}

TEST(TokenizerIdentifier, TokenizerTestBasic) {
    Token token = get_token_from_string("test");
    EXPECT_EQ(token.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token.get_value<std::string>(), "test");
}

TEST(TokenizerKeyword, TokenizerTestBasic) {
    Token token = get_token_from_string("while");
    EXPECT_EQ(token.get_type(), TokenType::T_WHILE);
}

TEST(TokenizerInt, TokenizerTestBasic) {
    Token token = get_token_from_string("123");
    EXPECT_EQ(token.get_type(), TokenType::T_INT);
    EXPECT_EQ(token.get_value<int>(), 123);
}

TEST(TokenizerDouble, TokenizerTestBasic) {
    Token token = get_token_from_string("12.3456");
    EXPECT_EQ(token.get_type(), TokenType::T_FLT);
    EXPECT_EQ(token.get_value<double>(), 12.3456);
}

TEST(TokenizerOperator, TokenizerTestBasic) {
    Token token = get_token_from_string("->");
    EXPECT_EQ(token.get_type(), TokenType::T_CALL);
    EXPECT_FALSE(token.get_value().index() != 0);
}

// the only 3-sign op
TEST(TokenizerOperatorBindFrt, TokenizerTestBasic) {
    Token token = get_token_from_string("->>");
    EXPECT_EQ(token.get_type(), TokenType::T_BINDFRT);
    EXPECT_FALSE(token.get_value().index() != 0);
}


TEST(TokenizerTwoTokens, TokenizerTestBasic) {
    Tokenizer t = get_tokenizer_for_string("test test2");
    Token token1 = t.get_token();
    Token token2 = t.get_token();
    EXPECT_EQ(token1.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token1.get_value<std::string>(), "test");
    EXPECT_EQ(token2.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token2.get_value<std::string>(), "test2");
}


TEST(TokenizerComment, TokenizerTestBasic) {
    Tokenizer t = get_tokenizer_for_string("test // blablabla\nother");
    Token token1 = t.get_token();
    Token token2 = t.get_token();
    Token token3 = t.get_token();
    EXPECT_EQ(token1.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token1.get_value<std::string>(), "test");
    EXPECT_EQ(token2.get_type(), TokenType::T_COMMENT);
    EXPECT_EQ(token3.get_type(), TokenType::T_IDENTIFIER);
    EXPECT_EQ(token3.get_value<std::string>(), "other");
}
