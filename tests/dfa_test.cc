#include <gtest/gtest.h>
#include "handler_manager.h"
#include "dfa_states.h"
#include "lexeme_context.h"

TEST(DFATestHandler, HandlerManagerBasicWhitespace) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state(' ', context), DFAState::IN_WHITESPACE);
}

TEST(DFATestHandler2, HandlerManagerBasicIdentifier) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('a', context), DFAState::IN_IDENTIFIER);
  EXPECT_EQ(context.get_lexeme_as<std::string>(), "a");
}

TEST(DFATestHandlerInt1, HandlerManagerBasicInt) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('1', context), DFAState::IN_NUMBER);
  EXPECT_EQ(context.get_lexeme_as<int>(), 1);
}

TEST(DFATestHandlerInt2, HandlerManagerBasicInt) {
  HandlerManager manager;
  LexemeContext context;
  context.add_int(1);
  EXPECT_EQ(manager[DFAState::IN_NUMBER]->next_state('2', context), DFAState::IN_NUMBER);
  EXPECT_EQ(context.get_lexeme_as<int>(), 12);
}

TEST(DFATestHandlerFlt, HandlerManagerBasicFlt) {
  HandlerManager manager;
  LexemeContext context;
  context.add_int(1);
  EXPECT_EQ(manager[DFAState::IN_NUMBER]->next_state('.', context), DFAState::IN_FLT);
  EXPECT_EQ(context.get_lexeme_as<int>(), 1);
}

TEST(DFATestHandlerFltEmpty, HandlerManagerBasicFlt) {
  HandlerManager manager;
  LexemeContext context;
  context.add_int(1);
  EXPECT_EQ(manager[DFAState::IN_NUMBER]->next_state('.', context), DFAState::IN_FLT);
  EXPECT_EQ(context.convert_to_double(), 1.0);
}

TEST(DFATestHandlerFlt2, HandlerManagerBasicFlt) {
  HandlerManager manager;
  LexemeContext context;
  context.add_int(1);
  EXPECT_EQ(manager[DFAState::IN_NUMBER]->next_state('.', context), DFAState::IN_FLT);
  EXPECT_EQ(manager[DFAState::IN_FLT]->next_state('1', context), DFAState::IN_FLT);
  EXPECT_EQ(context.convert_to_double(), 1.1);
}

TEST(DFATestHandlerStringBasic, HandlerManagerString) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('"', context), DFAState::IN_STRING);
  EXPECT_EQ(manager[DFAState::IN_STRING]->next_state('1', context), DFAState::IN_STRING);
  EXPECT_EQ(context.get_lexeme_as<std::string>(), "1");
}

TEST(DFATestHandlerStringEscape, HandlerManagerString) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('"', context), DFAState::IN_STRING);
  EXPECT_EQ(manager[DFAState::IN_STRING]->next_state('\\', context), DFAState::IN_ESCAPE);
  EXPECT_EQ(manager[DFAState::IN_ESCAPE]->next_state('\"', context), DFAState::IN_STRING);
  EXPECT_EQ(context.get_lexeme_as<std::string>(), "\"");
}

TEST(DFATestHandlerOperator, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('-', context), DFAState::IN_OPERATOR);
}

TEST(DFATestHandlerLongOperator, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('-', context), DFAState::IN_OPERATOR);
  EXPECT_EQ(manager[DFAState::IN_OPERATOR]->next_state('>', context), DFAState::IN_LONG_OPERATOR);
}

TEST(DFATestHandlerLongOperatorToken, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('-', context), DFAState::IN_OPERATOR);
  EXPECT_EQ(manager[DFAState::IN_OPERATOR]->next_state('>', context), DFAState::IN_LONG_OPERATOR);
  EXPECT_EQ(manager[DFAState::IN_LONG_OPERATOR]->next_state(' ', context), DFAState::END);
  EXPECT_EQ(context.get_token_type(), TokenType::T_CALL);
}

TEST(DFATestHandlerLongOperatorAnd, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('&', context), DFAState::IN_FIRST_CHAR_LONG_OP);
  EXPECT_EQ(manager[DFAState::IN_FIRST_CHAR_LONG_OP]->next_state('&', context), DFAState::IN_LONG_OPERATOR);
  EXPECT_EQ(manager[DFAState::IN_LONG_OPERATOR]->next_state(' ', context), DFAState::END);
  EXPECT_EQ(context.get_token_type(), TokenType::T_AND);
}

TEST(DFATestHandlerSpecialChar, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('(', context), DFAState::IN_OPERATOR);
  EXPECT_EQ(context.get_token_type(), TokenType::T_LPAREN);
}

TEST(DFATestHandlerSpecialChar2, HandlerManagerOperator) {
  HandlerManager manager;
  LexemeContext context;
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state(';', context), DFAState::IN_OPERATOR);
  EXPECT_EQ(context.get_token_type(), TokenType::T_SEMICOLON);
}
