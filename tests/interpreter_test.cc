#include <gtest/gtest.h>
#include <memory>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "builtin_defines.h"

#define VERBOSE false

Lexer get_lexer_for_string(std::string string) {
    std::shared_ptr<std::stringstream> input = std::make_unique<std::stringstream>(string);
    Lexer lexer(input, VERBOSE);
    return lexer;
}

std::unique_ptr<Parser> get_parser(std::string string, bool skip = false) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string(string));
    auto parser = std::make_unique<Parser>(std::move(lexer));
    if (skip) parser->next_token();
    return parser;
}

TEST(InterpreterTest, InterpreterAdd) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("1 + 2", true);
    auto expr = parser->parse_expression();
    expr->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 3);
}

TEST(InterpreterTest, InterpreterAddDouble) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("1.0 + 2.0", true);
    auto expr = parser->parse_expression();
    expr->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<double>(value));
    EXPECT_EQ(std::get<double>(value), 3.0);
}

TEST(InterpreterTest, InterpreterAddDoubleInt) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("1.0 + 2", true);
    auto expr = parser->parse_expression();
    expr->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<double>(value));
    EXPECT_EQ(std::get<double>(value), 3.0);
}

TEST(InterpreterTest, InterpreterAssignBasic) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("1 => int a;", true);
    interpreter->push_call_stack(CallStackFrame({}, {BlockScope{}}));
    auto stmt = parser->parse_statement();
    stmt->accept(*interpreter);

    std::weak_ptr<Variable> var = interpreter->find_var_in_frame("a");
    ValType value = var.lock()->value;

    VarType type = VarType(BaseType::INT, false);

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
    EXPECT_TRUE(type.is_equal_to(var.lock()->signature.get_type()));
}

TEST(InterpreterTest, InterpreterReAssignBasic) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser_assign = get_parser("1 => mut int a;", true);
    auto parser_reassign = get_parser("2 => a;", true);
    interpreter->push_call_stack(CallStackFrame({}, {BlockScope{}}));

    auto stmt = parser_assign->parse_statement();
    auto stmt_reassign = parser_reassign->parse_statement();

    stmt->accept(*interpreter);
    stmt_reassign->accept(*interpreter);

    std::weak_ptr<Variable> var = interpreter->find_var_in_frame("a");
    ValType value = var.lock()->value;

    VarType type = VarType(BaseType::INT, true);

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 2);
    EXPECT_TRUE(type.is_equal_to(var.lock()->signature.get_type()));
}

TEST(InterpreterTest, InterpreterWhileBasic) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser_assign = get_parser("1 => mut int i;", true);
    auto parser_reassign = get_parser("while (i < 5) { i + 1 => i; }", true);
    interpreter->push_call_stack(CallStackFrame({}, {BlockScope{}}));

    auto stmt = parser_assign->parse_statement();
    auto stmt_reassign = parser_reassign->parse_statement();

    stmt->accept(*interpreter);
    stmt_reassign->accept(*interpreter);

    std::weak_ptr<Variable> var = interpreter->find_var_in_frame("i");
    ValType value = var.lock()->value;

    VarType type = VarType(BaseType::INT, true);

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 5);
    EXPECT_TRUE(type.is_equal_to(var.lock()->signature.get_type()));
}

TEST(InterpreterTest, InterpreterProgram) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("int main { ret 5; }", false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 5);
}

TEST(InterpreterTest, InterpreterProgramReassign) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser("int main { 0 => mut int a; while (a < 5) { a + 1 => a; } ret a;}", false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 5);
}

TEST(InterpreterTest, InterpreterFuncTest) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
            "int main { (0) -> add_1 => int a; ret a;} "
            "int add_1 :: int a { ret a + 1; }",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}

TEST(InterpreterTest, InterpreterFuncTestReference) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
            "int main { 0 => mut int a; (a)->add_1; ret a;} "
            "int add_1 :: mut int a { a + 1 => a; }",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}

TEST(InterpreterTest, InterpreterFuncTestNoReference) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
            "int main { 1 => mut int a; (a + 1)->add_1; ret a;} "
            "int add_1 :: mut int a { a + 1 => a; }",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}

TEST(InterpreterTest, InterpreterBindFrontTest) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
            "(1) ->> add => [int::int] add_bound;"
            "ret (5) -> add_bound;"
        "}"

        "int add :: int a, int b {"
            "ret a + b;"
        "}",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 6);
}

TEST(InterpreterTest, InterpreterTestFlt) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "flt main {"
            "1.5 => mut flt a;"
            "(a) -> double;"
            "ret a;"
        "}"

        "void double :: mut flt a {"
            "2 * a => a;"
        "}",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<double>(value));
    EXPECT_EQ(std::get<double>(value), 3.0);
}

TEST(InterpreterTest, InterpreterBindFrontNoRefPass) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
            "1 => mut int a;"
            "(a) ->> add => [int::mut int] add_bound;"
            "(5) -> add_bound;"
            "ret a;"
        "}"

        "int add :: mut int a, mut int b {"
            "a + 2 => a;"
            "ret a + b;"
        "}",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}

TEST(InterpreterTest, InterpreterBindFrontReturnFunc) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
        "    (5) -> bind => [int::int] bound_returned;"
        "    ret (1) -> bound_returned;"
        "}"

        "[int::int] bind :: int a {"
            "(a) ->> add => [int::int] add_bound;"
            "ret add_bound;"
        "}"

        "int add :: int a, int b {"
            "ret a + b;"
        "}",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 6);
}

TEST(InterpreterTest, InterpreterDecoratorTest) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
        "    add_1 @ decorator => [int::] decorated;"
        "    ret () -> decorated;"
        "}"

        "int decorator :: [int::int] func {"
            "(5) -> func => int a;"
            "ret a + 1;"
        "}"

        "int add_1 :: int a {"
            "ret a + 1;"
        "}",
        false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 7);
}


TEST(InterpreterTest, InterpreterIfElseTest) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
            "1 => mut int a;"
            "if (a > 0) {"
            "   ret a;"
            "} elif (a == 0) {"
            "   ret 0;"
            "} else {"
            "   ret -1;"
            "}"
        "}",
    false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}
TEST(InterpreterTest, InterpreterBuiltinWhile) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>(builtins);
    auto parser = get_parser(
        "int main {"
        "    0 => mut int a;"
        "    while (a < 5) {"
        "        (a) -> increment;"
        "    }"
        "    ret a;"
        "}",
    false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 5);
}


TEST(InterpreterTest, InterpreterBuiltinPassValue) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>(builtins);
    auto parser = get_parser(
        "int main {"
        "    (0) -> increment;"
        "    ret 0;"
        "}",
    false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 0);
}


TEST(InterpreterTest, InterpreterBuiltin) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>(builtins);
    auto parser = get_parser(
        "int main {"
        "    1 => mut int a;"
        "    (a) -> increment;"
        "    ret a;"
        "}",
    false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 2);
}

TEST(InterpreterTest, InterpreterIfElseifTest) {
    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>();
    auto parser = get_parser(
        "int main {"
            "0 => int a;"
            "if (a > 0) {"
            "   ret a;"
            "} elif (a == 0) {"
            "   ret -10;"
            "} else {"
            "   ret -20;"
            "}"
        "}",
    false);

    auto program = parser->parse();

    program->accept(*interpreter);

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), -10);
}

TEST(InterpreterTest, InterpreterBuiltinStdout) {

    std::shared_ptr<InterpreterVisitor> interpreter = std::make_shared<InterpreterVisitor>(builtins);
    auto parser = get_parser(
        "int main {"
        "    1 => int a;"
        "    (\"HELLO WORLD!\") -> stdout;"
        "    ret a;"
        "}",
    false);

    auto program = parser->parse();

    std::streambuf* original_buf = std::cout.rdbuf();
    std::stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());

    program->accept(*interpreter);

    std::cout.rdbuf(original_buf);

    EXPECT_EQ(buffer.str(), "HELLO WORLD!");

    ValType value = interpreter->get_value();

    ASSERT_TRUE(std::holds_alternative<int>(value));
    EXPECT_EQ(std::get<int>(value), 1);
}

enum class ValKind {
    Int,
    Double,
    String,
    Bool
};

struct InterpreterAddParam {
    std::string program;
    std::variant<int, double, std::string, bool> expected;
    ValKind expected_kind = ValKind::Bool;
};

class InterpreterAddParamTest : public ::testing::TestWithParam<InterpreterAddParam> {};

TEST_P(InterpreterAddParamTest, AddExpressionProducesCorrectValue) {
    const auto& param = GetParam();

    auto interpreter = std::make_shared<InterpreterVisitor>();
    std::cout << "TESTING: " << param.program << std::endl;
    auto parser = get_parser(param.program, true);
    auto expr = parser->parse_expression();
    expr->accept(*interpreter);

    ValType value = interpreter->get_value();

    switch (param.expected_kind) {
        case ValKind::Int:
            ASSERT_TRUE(std::holds_alternative<int>(value));
            EXPECT_EQ(std::get<int>(value), std::get<int>(param.expected));
            break;
        case ValKind::Double:
            ASSERT_TRUE(std::holds_alternative<double>(value));
            EXPECT_DOUBLE_EQ(std::get<double>(value), std::get<double>(param.expected));
            break;
        case ValKind::String:
            ASSERT_TRUE(std::holds_alternative<std::string>(value));
            EXPECT_EQ(std::get<std::string>(value), std::get<std::string>(param.expected));
            break;
        case ValKind::Bool:
            ASSERT_TRUE(std::holds_alternative<bool>(value));
            EXPECT_EQ(std::get<bool>(value), std::get<bool>(param.expected));
            break;
        default:
            FAIL() << "Unknown expected kind";
    }
}

INSTANTIATE_TEST_SUITE_P(
    AddTests,
    InterpreterAddParamTest,
    ::testing::Values(
        InterpreterAddParam{"1 + 2", 3, ValKind::Int},
        InterpreterAddParam{"1.0 + 2.0", 3.0, ValKind::Double},
        InterpreterAddParam{"1 + 2.0", 3.0, ValKind::Double},
        InterpreterAddParam{"\"foo\" + \"bar\"", std::string("foobar"), ValKind::String},
        InterpreterAddParam{"true + false", 1, ValKind::Int},
        InterpreterAddParam{"true + 3", 4, ValKind::Int},
        InterpreterAddParam{"false + false", 0, ValKind::Int},
        InterpreterAddParam{"1 + \"test\"", std::string("1test"), ValKind::String},

        InterpreterAddParam{"5 - 1", 4, ValKind::Int},
        InterpreterAddParam{"5.0 - 1.0", 4.0, ValKind::Double},
        InterpreterAddParam{"5.0 - 1", 4.0, ValKind::Double},
        InterpreterAddParam{"5.0 - 1 * 2", 3.0, ValKind::Double},
        InterpreterAddParam{"(5.0 - 1) * 2", 8.0, ValKind::Double},

        InterpreterAddParam{"3 * 4", 12, ValKind::Int},
        InterpreterAddParam{"2.5 * 4.0", 10.0, ValKind::Double},
        InterpreterAddParam{"3 * 2.0", 6.0, ValKind::Double},
        InterpreterAddParam{"\"ha\" * 3", std::string("hahaha"), ValKind::String},
        InterpreterAddParam{"3 * \"ha\"", std::string("hahaha"), ValKind::String},
        InterpreterAddParam{"true * 5", 5, ValKind::Int},
        InterpreterAddParam{"false * 5.0", 0.0, ValKind::Double},

        InterpreterAddParam{"8 / 2", 4, ValKind::Int},
        InterpreterAddParam{"9.0 / 3.0", 3.0, ValKind::Double},
        InterpreterAddParam{"9 / 2.0", 4.5, ValKind::Double},

        InterpreterAddParam{"9 > 2.0", true},
        InterpreterAddParam{"9 < 2.0", false},
        InterpreterAddParam{"9 >= 2.0", true},
        InterpreterAddParam{"9 <= 2.0", false},
        InterpreterAddParam{"9 == 2.0", false},
        InterpreterAddParam{"9 == 9.0", true},

        InterpreterAddParam{"\"1234\" > \"12\"", true},
        InterpreterAddParam{"\"1234\" >= \"12\"", true},
        InterpreterAddParam{"\"1234\" != \"12\"", true},
        InterpreterAddParam{"\"1234\" == \"1234\"", true},
        InterpreterAddParam{"!5", false},
        InterpreterAddParam{"-5", -5, ValKind::Int}
    )
);

