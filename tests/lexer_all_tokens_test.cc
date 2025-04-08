#include <gtest/gtest.h>
#include "lexer.h"

#define VERBOSE true

struct LexerTokenTestCase {
    std::string text;
    TokenType expected_token;
};

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<InputStream> input = std::make_unique<StringInputStream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}

class DFATestHandlerOperator : public ::testing::TestWithParam<LexerTokenTestCase> {};

TEST_P(DFATestHandlerOperator, HandlerManagerOperator) {
    const auto& param = GetParam();
    Lexer lexer = get_lexer_for_string(param.text);
    Token token = lexer.get_token();
    EXPECT_EQ(token.type, param.expected_token);
}

INSTANTIATE_TEST_SUITE_P(
    AllOperators,
    DFATestHandlerOperator,
    ::testing::Values(
        LexerTokenTestCase{"test", TokenType::T_IDENTIFIER},
        LexerTokenTestCase{"int", TokenType::T_INT_TYPE},
        LexerTokenTestCase{"flt", TokenType::T_FLT_TYPE},
        LexerTokenTestCase{"string", TokenType::T_STRING_TYPE},
        LexerTokenTestCase{"void", TokenType::T_VOID_TYPE},
        LexerTokenTestCase{"ret", TokenType::T_RET},
        LexerTokenTestCase{"while", TokenType::T_WHILE},
        LexerTokenTestCase{"for", TokenType::T_FOR},
        LexerTokenTestCase{"if", TokenType::T_IF},
        LexerTokenTestCase{"elif", TokenType::T_ELIF},
        LexerTokenTestCase{"else", TokenType::T_ELSE},
        LexerTokenTestCase{"mut", TokenType::T_MUT},
        LexerTokenTestCase{"->>", TokenType::T_BINDFRT},
        LexerTokenTestCase{"@", TokenType::T_DECORATE},
        LexerTokenTestCase{"->", TokenType::T_CALL},
        LexerTokenTestCase{"=>", TokenType::T_ASSIGN},
        LexerTokenTestCase{"!", TokenType::T_NOT},
        LexerTokenTestCase{"+", TokenType::T_PLUS},
        LexerTokenTestCase{"-", TokenType::T_MINUS},
        LexerTokenTestCase{"*", TokenType::T_MULT},
        LexerTokenTestCase{"/", TokenType::T_DIV},
        LexerTokenTestCase{"&&", TokenType::T_AND},
        LexerTokenTestCase{"||", TokenType::T_OR},
        LexerTokenTestCase{"<=", TokenType::T_LTE},
        LexerTokenTestCase{">=", TokenType::T_GTE},
        LexerTokenTestCase{"==", TokenType::T_EQ},
        LexerTokenTestCase{"!=", TokenType::T_NEQ},
        LexerTokenTestCase{"<", TokenType::T_LT},
        LexerTokenTestCase{">", TokenType::T_GT},
        LexerTokenTestCase{"(", TokenType::T_LPAREN},
        LexerTokenTestCase{")", TokenType::T_RPAREN},
        LexerTokenTestCase{"{", TokenType::T_LBLOCK},
        LexerTokenTestCase{"}", TokenType::T_RBLOCK},
        LexerTokenTestCase{";", TokenType::T_SEMICOLON},
        LexerTokenTestCase{",", TokenType::T_COMMA},
        LexerTokenTestCase{"::", TokenType::T_FUNC_SIGN},
        LexerTokenTestCase{"[", TokenType::T_LFTYPE},
        LexerTokenTestCase{"]", TokenType::T_RFTYPE},
        LexerTokenTestCase{"//", TokenType::T_COMMENT}
    )
);
