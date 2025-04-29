#include <gtest/gtest.h>
#include "lexer.h"
#include "parser.h"

#define VERBOSE false

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<std::stringstream> input = std::make_unique<std::stringstream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}
TEST(ParserTest, ParsesLiteralExpression) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("42"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<LiteralExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesBinaryExpression) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("1 + 2"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<BinaryExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesBinaryOrExpression) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("1 || 2"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<BinaryExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesBinaryAndExpression) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("1 && 2"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<BinaryExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionCallExpression) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("(1, 2)->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionNoArgs) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("()->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionOneArg) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("(1)->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionNestedExpr) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("(1 + 5)->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionNestedCall) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("((5)->inc)->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesFunctionNestedNoArgs) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("(()->inc)->foo"));
    Parser parser(lexer);
    parser.next_token();
    ExprPtr expr = parser.parse_expression();
    ASSERT_NE(expr, nullptr);
    ASSERT_NE(dynamic_cast<CallExpr*>(expr.get()), nullptr);
}

TEST(ParserTest, ParsesAssignStatement) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("5 => int a;"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<AssignStatement*>(stmt.get()), nullptr);
}

TEST(ParserTest, ParsesReturnStatement) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("ret 0;"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<RetStatement*>(stmt.get()), nullptr);
}

TEST(ParserTest, ParsesIfStatement) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("if (x < 5) { ret 5; }"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<ConditionalStatement*>(stmt.get()), nullptr);
}

TEST(ParserTest, ParsesWhileLoopStatement) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("while (x < 10) { ret 5; }"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<WhileLoopStatement*>(stmt.get()), nullptr);
}

TEST(ParserTest, ParsesAsignCall) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("(5)->increment => int b;"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<AssignStatement*>(stmt.get()), nullptr);
}

TEST(ParserTest, ParsesForLoop) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string("for (5 => int a; a < 10) { (a)->some_func; } -> increment;"));
    Parser parser(lexer);
    parser.next_token();
    StatementPtr stmt = parser.parse_statement();
    ASSERT_NE(stmt, nullptr);
    ASSERT_NE(dynamic_cast<ForLoopStatement*>(stmt.get()), nullptr);
}
