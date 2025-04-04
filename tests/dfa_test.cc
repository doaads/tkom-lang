#include <gtest/gtest.h>
#include "handler_manager.h"
#include "dfa_states.h"

// Demonstrate some basic assertions.
TEST(DFATestHandler, HandlerManagerBasicWhitespace) {
  HandlerManager manager;
  std::variant<std::string, int, double> lexeme = "";
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state(' ', lexeme), DFAState::IN_WHITESPACE);
}

TEST(DFATestHandler2, HandlerManagerBasicIdentifier) {
  HandlerManager manager;
  std::variant<std::string, int, double> lexeme2 = "";
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('a', lexeme2), DFAState::IN_IDENTIFIER);
  EXPECT_EQ(std::get<std::string>(lexeme2), "a");
}

TEST(DFATestHandlerInt1, HandlerManagerBasicInt) {
  HandlerManager manager;
  std::variant<std::string, int, double> lexeme = "";
  EXPECT_EQ(manager[DFAState::IN_WHITESPACE]->next_state('1', lexeme), DFAState::IN_NUMBER);
  EXPECT_EQ(std::get<int>(lexeme), 1);
}

TEST(DFATestHandlerInt2, HandlerManagerBasicInt) {
  HandlerManager manager;
  std::variant<std::string, int, double> lexeme = 1;
  EXPECT_EQ(manager[DFAState::IN_NUMBER]->next_state('2', lexeme), DFAState::IN_NUMBER);
  EXPECT_EQ(std::get<int>(lexeme), 12);
}
