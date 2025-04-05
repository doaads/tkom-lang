#include <gtest/gtest.h>
#include "handler_manager.h"
#include "dfa_states.h"
#include "lexeme_context.h"

struct OperatorTestCase {
    char input_char;
    TokenType expected_token;
};

struct LongOperatorTestCase {
    char first_char;
    char second_char;
    TokenType expected_token;
};

class DFATestHandlerOperator : public ::testing::TestWithParam<OperatorTestCase> {};

TEST_P(DFATestHandlerOperator, HandlerManagerOperator) {
    const auto& param = GetParam();

    HandlerManager manager;
    LexemeContext context;

    EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state(param.input_char, context), DFAState::IN_OPERATOR);
    EXPECT_EQ(context.get_lexeme_string(), std::string(1, param.input_char));
    EXPECT_EQ(context.get_token_type(), param.expected_token);
}

INSTANTIATE_TEST_SUITE_P(
    AllOperators,
    DFATestHandlerOperator,
    ::testing::Values(
        OperatorTestCase{'+', TokenType::T_PLUS},
        OperatorTestCase{'-', TokenType::T_MINUS},
        OperatorTestCase{'*', TokenType::T_MULT},
        OperatorTestCase{'/', TokenType::T_DIV},
        OperatorTestCase{'<', TokenType::T_LT},
        OperatorTestCase{'>', TokenType::T_GT},
        OperatorTestCase{'!', TokenType::T_NOT}
    )
);


class DFATestHandlerLongOperator : public ::testing::TestWithParam<LongOperatorTestCase> {};

TEST_P(DFATestHandlerLongOperator, HandlerManagerOperator) {
    const auto& param = GetParam();

    HandlerManager manager;
    LexemeContext context;

    EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state(param.first_char, context), DFAState::IN_OPERATOR);
    EXPECT_EQ(manager[DFAState::IN_OPERATOR]->next_state(param.second_char, context), DFAState::IN_LONG_OPERATOR);
    EXPECT_EQ(context.get_lexeme_string(), std::string(1, param.first_char) + std::string(1, param.second_char));
    EXPECT_EQ(context.get_token_type(), param.expected_token);
}

INSTANTIATE_TEST_SUITE_P(
    AllOperators,
    DFATestHandlerLongOperator,
    ::testing::Values(
        LongOperatorTestCase{'-', '>', TokenType::T_CALL},
        LongOperatorTestCase{'>', '=', TokenType::T_GTE},
        LongOperatorTestCase{'<', '=', TokenType::T_LTE},
        LongOperatorTestCase{'!', '=', TokenType::T_NEQ}
    )
);
