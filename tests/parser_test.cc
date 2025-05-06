#include <gtest/gtest.h>
#include "parser_test_counter.h"
#include "lexer.h"
#include "parser.h"

#include <memory>

#define VERBOSE false

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<std::stringstream> input = std::make_unique<std::stringstream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}

std::unique_ptr<Parser> get_parser(std::string string, std::shared_ptr<ParserTestCounter> visitor) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string(string));
    return std::make_unique<Parser>(std::move(lexer), visitor);
}

TEST(ParserTest, ParsesAssignmentStatement) {
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser("int main { 10 => int x; }", counter);
    auto program = parser->parse();

    ASSERT_NE(program, nullptr);
    EXPECT_EQ(counter->assign_stmt_count, 1);
    EXPECT_EQ(counter->literal_expr_count, 1);
    EXPECT_EQ(counter->identifier_expr_count, 1);
    EXPECT_EQ(counter->var_type_count, 2);  // func def & assign
    EXPECT_EQ(counter->function_count, 1);
    EXPECT_EQ(counter->func_signature_count, 1);
}

TEST(ParserTest, ParsesFunctionCall) {
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser("int main { (10, 20)->foo; }", counter);
    auto program = parser->parse();

    ASSERT_NE(program, nullptr);
    EXPECT_EQ(counter->call_stmt_count, 1); // One function call
    EXPECT_EQ(counter->call_expr_count, 1); // One call expression
    EXPECT_EQ(counter->literal_expr_count, 2); // Two literals: `10` and `20`
    EXPECT_EQ(counter->function_count, 1); // One function (main)
    EXPECT_EQ(counter->func_signature_count, 1); // One function signature for main
}

TEST(ParserTest, ParsesWhileLoop) {
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser("int main { while (i < 10) { (i)->increment; } }", counter);
    auto program = parser->parse();

    ASSERT_NE(program, nullptr);
    EXPECT_EQ(counter->while_stmt_count, 1); // One while loop statement
    EXPECT_EQ(counter->call_stmt_count, 1); // One assignment in the loop: `i++`
    EXPECT_EQ(counter->binary_expr_count, 1); // One binary expression: `i < 10`
    EXPECT_EQ(counter->literal_expr_count, 1); // One literal: `10`
}

TEST(ParserTest, ParsesReturnStatement) {
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser("int main { ret 42; }", counter);
    auto program = parser->parse();

    ASSERT_NE(program, nullptr);
    EXPECT_EQ(counter->ret_stmt_count, 1); // One return statement
    EXPECT_EQ(counter->literal_expr_count, 1); // One literal: `42`
}

TEST(ParserTest, ParsesConditionalStatement) {
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser("int main { if (x > 10) { 20 => int y; } }", counter);
    auto program = parser->parse();

    ASSERT_NE(program, nullptr);
    EXPECT_EQ(counter->conditional_stmt_count, 1); // One conditional statement (if)
    EXPECT_EQ(counter->binary_expr_count, 1); // One binary expression: `x > 10`
    EXPECT_EQ(counter->assign_stmt_count, 1); // One assignment in the if block: `20 => int y`
    EXPECT_EQ(counter->literal_expr_count, 2); // Two literals: `10` and `20`
    EXPECT_EQ(counter->identifier_expr_count, 2); // Two identifiers: `x` and `y`
    EXPECT_EQ(counter->function_count, 1); // One function (main)
    EXPECT_EQ(counter->func_signature_count, 1); // One function signature for main
    EXPECT_EQ(counter->block_count, 2); // One function signature for main
}

TEST(ParserTest, ParserLongProgramTest) {
    static const std::string program_str =
            "int main {"
            "    (\"Enter a\")->stdin => flt a;"
            "    (\"Enter b\")->stdin => flt b;"
            "    (\"Enter c\")->stdin => flt c;"
            "    b * b - 4 * a * c => flt delta;"
            "    if (delta > 0) {"
            "          (-b - (delta)->sqrt) / (2 * a) => flt x1;\n"
            "          (-b + (delta)->sqrt) / (2 * a) => flt x2;"
            "          (\"Result: \") -> stdout;"
            "          (x1 + \" and \" + x2) -> stdout;"
            "      } elif (delta == 0) {"
            "          -b / 2 * a => flt x1;"
            "          (\"Result: \") -> stdout;"
            "          (x1) -> stdout;\n"
            "      } else {"
            "          (\"No real solution\") -> stdout;"
            "    }"
            "}";

    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser(program_str, counter);
    auto program = parser->parse();

    EXPECT_NE(program, nullptr);
    EXPECT_EQ(counter->conditional_stmt_count, 2); // two conditional statements (if, else if)
    EXPECT_EQ(counter->else_stmt_count, 1);
    EXPECT_EQ(counter->call_expr_count, 10);
    EXPECT_EQ(counter->assign_stmt_count, 7);
    EXPECT_EQ(counter->block_count, 4);
    EXPECT_EQ(counter->identifier_expr_count, 34);
}
