#include <gtest/gtest.h>
#include "exceptions.h"
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

std::unique_ptr<Parser> get_parser(std::string string, bool skip = false) {
    auto lexer = std::make_shared<Lexer>(get_lexer_for_string(string));
    auto parser = std::make_unique<Parser>(std::move(lexer));
    if (skip) parser->next_token();
    return parser;
}

struct ExprCount {
    std::string program;
    int binary_count = 0;
    int literal_count = 0;
    int identifier_count = 0;
    int call_count = 0;
    int unary_count = 0;
    int bindfrt_count = 0;
};

class ParserParametrizedExpr : public ::testing::TestWithParam<ExprCount> {};

TEST_P(ParserParametrizedExpr, HandlerManagerOperator) {
    const auto& param = GetParam();
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser(param.program, true);
    auto expr = parser->parse_expression();
    std::cout << param.program << std::endl;
    ASSERT_NE(expr, nullptr);
    expr->accept(*counter);
    EXPECT_EQ(counter->binary_expr_count, param.binary_count);
    EXPECT_EQ(counter->literal_expr_count, param.literal_count);
    EXPECT_EQ(counter->identifier_expr_count, param.identifier_count);
    EXPECT_EQ(counter->call_expr_count, param.call_count);
    EXPECT_EQ(counter->unary_expr_count, param.unary_count);
    EXPECT_EQ(counter->bind_frt_expr_count, param.bindfrt_count);
}

INSTANTIATE_TEST_SUITE_P(
    AllExprs,
    ParserParametrizedExpr,
    ::testing::Values(
        ExprCount{"5 + 2", 1, 2},
        ExprCount{"5 + 2 - 2", 2, 3},
        ExprCount{"5 * 3", 1, 2},
        ExprCount{"3 / 2", 1, 2},
        ExprCount{"5 - 2", 1, 2},
        ExprCount{"5 || 2", 1, 2},
        ExprCount{"10 && 0", 1, 2},
        ExprCount{"10 == 0", 1, 2},
        ExprCount{"10 >= 0", 1, 2},
        ExprCount{"10 <= 0", 1, 2},
        ExprCount{"10 > 0", 1, 2},
        ExprCount{"10 < 0", 1, 2},
        ExprCount{"a + 0", 1, 1, 1},
        ExprCount{"!true", 0, 1, 0, 0, 1},
        ExprCount{"-5", 0, 1, 0, 0, 1},
        ExprCount{"!5 + 3", 1, 2, 0, 0, 1},
        ExprCount{"-(1 + 2)", 1, 2, 0, 0, 1},
        ExprCount{"!(!false)", 0, 1, 0, 0, 2},
        ExprCount{"-(-10)", 0, 1, 0, 0, 2},
        ExprCount{"(1, 2) -> print", 0, 2, 1, 1},
        ExprCount{"(x, y) -> doSomething", 0, 0, 3, 1},
        ExprCount{"() -> run", 0, 0, 1, 1},
        ExprCount{"(1 + 2) -> calc", 1, 2, 1, 1},
        ExprCount{"(5) ->> capture", 0, 1, 1, 0, 0, 1},
        ExprCount{"(a, b) ->> bindIt", 0, 0, 3, 0, 0, 1},
        ExprCount{"(1, x + y) ->> bindMath", 1, 1, 3, 0, 0, 1},
        ExprCount{"(((1)))", 0, 1, 0, 0, 0, 0},
        ExprCount{"(1 + 2) * (3 + 4)", 3, 4},
        ExprCount{"(1 + 2) * -(3 + 4)", 3, 4, 0, 0, 1},
        ExprCount{"((1) -> fn) * 2", 1, 2, 1, 1, 0, 0},
        ExprCount{"!((1, 2) ->> bound)", 0, 2, 1, 0, 1, 1},
        ExprCount{"-((1, 2) -> call)", 0, 2, 1, 1, 1},
        ExprCount{"(x) ->> bind", 0, 0, 2, 0, 0, 1},
        ExprCount{"!((a + b) * (c - d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"(1, 2) -> call", 0, 2, 1, 1, 0, 0},
        ExprCount{"(a + b, c) -> call", 1, 0, 4, 1, 0, 0},
        ExprCount{"!(x + y)", 1, 0, 2, 0, 1, 0},
        ExprCount{"-((a > b) == (c < d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"(1, 2 + 3) -> compute", 1, 3, 1, 1, 0, 0},
        ExprCount{"((1, 2) -> f) + 3", 1, 3, 1, 1, 0, 0},
        ExprCount{"(1) ->> bf", 0, 1, 1, 0, 0, 1},
        ExprCount{"(x + 1, y - 1) ->> bind", 2, 2, 3, 0, 0, 1},
        ExprCount{"(1, 2, 3) -> sum", 0, 3, 1, 1, 0, 0},
        ExprCount{"-((a + b) * (c - d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"!((x, y) -> compute)", 0, 0, 3, 1, 1, 0},
        ExprCount{"(((1 + 2) * 3) - 4)", 3, 4, 0, 0, 0, 0},
        ExprCount{"((5)) + ((6))", 1, 2, 0, 0, 0, 0},
        ExprCount{"(1, (x)) -> doThing", 0, 1, 2, 1, 0, 0},
        ExprCount{"(a, b) -> go * 2", 1, 1, 3, 1, 0, 0},
        ExprCount{"(1 + 2, 3 + 4) -> total", 2, 4, 1, 1, 0, 0},
        ExprCount{"((a)) ->> keep", 0, 0, 2, 0, 0, 1},
        ExprCount{"(1 + (2 + 3))", 2, 3, 0, 0, 0, 0},
        ExprCount{"!(x)", 0, 0, 1, 0, 1, 0},
        ExprCount{"-(y)", 0, 0, 1, 0, 1, 0},
        ExprCount{"(x) -> process", 0, 0, 2, 1, 0, 0},
        ExprCount{"(1 + x) -> run", 1, 1, 2, 1, 0, 0},
        ExprCount{"((x)) ->> store", 0, 0, 2, 0, 0, 1},
        ExprCount{"!((1 + 2) -> fn)", 1, 2, 1, 1, 1, 0},
        ExprCount{"-((1, 2) -> call)", 0, 2, 1, 1, 1, 0},
        ExprCount{"(x, y + z) -> doIt", 1, 0, 4, 1, 0, 0},
        ExprCount{"((1 + 2) * (3 - 4)) + 5", 4, 5, 0, 0, 0, 0},
        ExprCount{"(((a, b) -> fn))", 0, 0, 3, 1, 0, 0},
        ExprCount{"(a + b) * ((c, d) -> fun)", 2, 0, 5, 1, 0, 0},
        ExprCount{"(1 + 2) ->> keep", 1, 2, 1, 0, 0, 1},
        ExprCount{"!(a + b * c)", 2, 0, 3, 0, 1, 0},
        ExprCount{"-((x + y) == z)", 2, 0, 3, 0, 1, 0},
        ExprCount{"((x, y + z) -> op) + 0", 2, 1, 4, 1, 0, 0},
        ExprCount{"(1 + 2, a - b) ->> bind", 2, 2, 3, 0, 0, 1},
        ExprCount{"(x * (y + z))", 2, 0, 3, 0, 0, 0},
        ExprCount{"(x) ->> bind", 0, 0, 2, 0, 0, 1},
        ExprCount{"(a, b + c, d) -> combine", 1, 0, 5, 1, 0, 0},
        ExprCount{"((a + b) -> fn) * (1 + 2)", 3, 2, 3, 1, 0, 0},
        ExprCount{"!((a, b) ->> grab)", 0, 0, 3, 0, 1, 1},
        ExprCount{"-((x + 1, y + 2) -> action)", 2, 2, 3, 1, 1, 0},
        ExprCount{"5.0 + 2.0", 1, 2},
        ExprCount{"5.0 + 2.0 - 2.0", 2, 3},
        ExprCount{"5.0 * 3.0", 1, 2},
        ExprCount{"3.0 / 2.0", 1, 2},
        ExprCount{"5.0 - 2.0", 1, 2},
        ExprCount{"5.0 || 2.0", 1, 2},
        ExprCount{"10.0 && 0.0", 1, 2},
        ExprCount{"10.0 == 0.0", 1, 2},
        ExprCount{"10.0 >= 0.0", 1, 2},
        ExprCount{"10.0 <= 0.0", 1, 2},
        ExprCount{"10.0 > 0.0", 1, 2},
        ExprCount{"10.0 < 0.0", 1, 2},
        ExprCount{"a + 0.0", 1, 1, 1},
        ExprCount{"!true", 0, 1, 0, 0, 1},
        ExprCount{"-5.0", 0, 1, 0, 0, 1},
        ExprCount{"!5.0 + 3.0", 1, 2, 0, 0, 1},
        ExprCount{"-(1.0 + 2.0)", 1, 2, 0, 0, 1},
        ExprCount{"!(!false)", 0, 1, 0, 0, 2},
        ExprCount{"-(-10.0)", 0, 1, 0, 0, 2},
        ExprCount{"(1.0, 2.0) -> print", 0, 2, 1, 1},
        ExprCount{"(x, y) -> doSomething", 0, 0, 3, 1},
        ExprCount{"() -> run", 0, 0, 1, 1},
        ExprCount{"(1.0 + 2.0) -> calc", 1, 2, 1, 1},
        ExprCount{"(5.0) ->> capture", 0, 1, 1, 0, 0, 1},
        ExprCount{"(a, b) ->> bindIt", 0, 0, 3, 0, 0, 1},
        ExprCount{"(1.0, x + y) ->> bindMath", 1, 1, 3, 0, 0, 1},
        ExprCount{"(((1.0)))", 0, 1, 0, 0, 0, 0},
        ExprCount{"(1.0 + 2.0) * (3.0 + 4.0)", 3, 4},
        ExprCount{"(1.0 + 2.0) * -(3.0 + 4.0)", 3, 4, 0, 0, 1},
        ExprCount{"((1.0) -> fn) * 2.0", 1, 2, 1, 1, 0, 0},
        ExprCount{"!((1.0, 2.0) ->> bound)", 0, 2, 1, 0, 1, 1},
        ExprCount{"-((1.0, 2.0) -> call)", 0, 2, 1, 1, 1},
        ExprCount{"(x) ->> bind", 0, 0, 2, 0, 0, 1},
        ExprCount{"!((a + b) * (c - d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"(1.0, 2.0) -> call", 0, 2, 1, 1, 0, 0},
        ExprCount{"(a + b, c) -> call", 1, 0, 4, 1, 0, 0},
        ExprCount{"!(x + y)", 1, 0, 2, 0, 1, 0},
        ExprCount{"-((a > b) == (c < d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"(1.0, 2.0 + 3.0) -> compute", 1, 3, 1, 1, 0, 0},
        ExprCount{"((1.0) -> f) + 3.0", 1, 2, 1, 1, 0, 0},
        ExprCount{"(1.0) ->> bf", 0, 1, 1, 0, 0, 1},
        ExprCount{"(x + 1.0, y - 1.0) ->> bind", 2, 2, 3, 0, 0, 1},
        ExprCount{"(1.0, 2.0, 3.0) -> sum", 0, 3, 1, 1, 0, 0},
        ExprCount{"-((a + b) * (c - d))", 3, 0, 4, 0, 1, 0},
        ExprCount{"!((x, y) -> compute)", 0, 0, 3, 1, 1, 0},
        ExprCount{"(((1.0 + 2.0) * 3.0) - 4.0)", 3, 4, 0, 0, 0, 0},
        ExprCount{"((5.0)) + ((6.0))", 1, 2, 0, 0, 0, 0},
        ExprCount{"(1.0, (x)) -> doThing", 0, 1, 2, 1, 0, 0},
        ExprCount{"(a, b) -> go * 2.0", 1, 1, 3, 1, 0, 0},
        ExprCount{"(1.0 + 2.0, 3.0 + 4.0) -> total", 2, 4, 1, 1, 0, 0},
        ExprCount{"((a)) ->> keep", 0, 0, 2, 0, 0, 1},
        ExprCount{"(1.0 + (2.0 + 3.0))", 2, 3, 0, 0, 0, 0},
        ExprCount{"!(x)", 0, 0, 1, 0, 1, 0},
        ExprCount{"-(y)", 0, 0, 1, 0, 1, 0},
        ExprCount{"(x) -> process", 0, 0, 2, 1, 0, 0},
        ExprCount{"(1.0 + x) -> run", 1, 1, 2, 1, 0, 0},
        ExprCount{"((x)) ->> store", 0, 0, 2, 0, 0, 1},
        ExprCount{"!((1.0 + 2.0) -> fn)", 1, 2, 1, 1, 1, 0},
        ExprCount{"-((1.0, 2.0) -> call)", 0, 2, 1, 1, 1, 0},
        ExprCount{"(x, y + z) -> doIt", 1, 0, 4, 1, 0, 0},
        ExprCount{"((1.0 + 2.0) * (3.0 - 4.0)) + 5.0", 4, 5, 0, 0, 0, 0},
        ExprCount{"(((a, b) -> fn))", 0, 0, 3, 1, 0, 0},
        ExprCount{"(a + b) * ((c, d) -> fun)", 2, 0, 5, 1, 0, 0},
        ExprCount{"(1.0 + 2.0) ->> keep", 1, 2, 1, 0, 0, 1},
        ExprCount{"!(a + b * c)", 2, 0, 3, 0, 1, 0},
        ExprCount{"-((x + y) == z)", 2, 0, 3, 0, 1, 0},
        ExprCount{"((x, y + z) -> op) + 0.0", 2, 1, 4, 1, 0, 0},
        ExprCount{"(1.0 + 2.0, a - b) ->> bind", 2, 2, 3, 0, 0, 1},
        ExprCount{"(x * (y + z))", 2, 0, 3, 0, 0, 0},
        ExprCount{"(x) ->> bind", 0, 0, 2, 0, 0, 1},
        ExprCount{"(a, b + c, d) -> combine", 1, 0, 5, 1, 0, 0},
        ExprCount{"((a + b) -> fn) * (1.0 + 2.0)", 3, 2, 3, 1, 0, 0},
        ExprCount{"!((a, b) ->> grab)", 0, 0, 3, 0, 1, 1},
        ExprCount{"a @ b", 1, 0, 2, 0, 0, 0},
        ExprCount{"(5) -> a @ b", 1, 1, 2, 1, 0, 0},
        ExprCount{"(1) -> (5) ->> a @ b", 1, 2, 2, 1, 0, 1}
    )
);

enum class StmtType {
    FOR,
    WHILE,
    ASSIGN,
    CALL,
    CONDITIONAL,
    ELSE,
    RET,
};

struct StmtCount {
    std::string program;
    StmtType type = StmtType::ASSIGN;
    int num = 1;
};

class ParserParametrizedStmt : public ::testing::TestWithParam<StmtCount> {};

TEST_P(ParserParametrizedStmt, HandlerManagerOperator) {
    const auto& param = GetParam();
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser(param.program, true);
    auto stmt = parser->parse_statement();
    std::cout << param.program << std::endl;
    ASSERT_NE(stmt, nullptr);
    stmt->accept(*counter);
    int count;
    switch (param.type) {
        case StmtType::FOR: count = counter->for_stmt_count; break;
        case StmtType::WHILE: count = counter->while_stmt_count; break;
        case StmtType::ASSIGN: count = counter->assign_stmt_count; break;
        case StmtType::CALL: count = counter->call_stmt_count; break;
        case StmtType::CONDITIONAL: count = counter->conditional_stmt_count; break;
        case StmtType::ELSE: count = counter->else_stmt_count; break;
        case StmtType::RET: count = counter->ret_stmt_count; break;
    }
    EXPECT_EQ(param.num, count);
};

INSTANTIATE_TEST_SUITE_P(
    AllExprs,
    ParserParametrizedStmt,
    ::testing::Values(
        StmtCount{"5 => int a;"},
        StmtCount{"5 => mut int b;"},
        StmtCount{"func => [int::int] a;"},
        StmtCount{"ret a;", StmtType::RET},
        StmtCount{"for (i;i<10) {} -> increment;", StmtType::FOR},
        StmtCount{"while (i<10) {};", StmtType::WHILE},
        StmtCount{"if (a > 5) {}", StmtType::CONDITIONAL},
        StmtCount{"if (a > 5) {} elif (b > 1) {}", StmtType::CONDITIONAL, 2},
        StmtCount{"if (a > 10) {} else {}", StmtType::ELSE},
        StmtCount{"(5)->a;", StmtType::CALL},
        StmtCount{"for (x; x < 100) {} -> step;", StmtType::FOR},
        StmtCount{"for (i; i <= max) {} -> update;", StmtType::FOR},
        StmtCount{"while (done != true) { x => mut int temp; };", StmtType::WHILE},
        StmtCount{"while (counter < limit) {};", StmtType::WHILE},
        StmtCount{"value => flt rate;", StmtType::ASSIGN},
        StmtCount{"x => int total;", StmtType::ASSIGN},
        StmtCount{"func => [int::int] doStuff;", StmtType::ASSIGN},
        StmtCount{"(10.5)->compute;", StmtType::CALL},
        StmtCount{"(user_input)->process;", StmtType::CALL},
        StmtCount{"if (score >= passing) {}", StmtType::CONDITIONAL},
        StmtCount{"if (active) {} elif (timeout) {}", StmtType::CONDITIONAL, 2},
        StmtCount{"if (has_data) {} elif (has_backup) {} elif (has_default) {}", StmtType::CONDITIONAL, 3},
        StmtCount{"if (isReady) {} else {}", StmtType::ELSE},
        StmtCount{"if (user) {} elif (admin) {} else {}", StmtType::ELSE},
        StmtCount{"ret compute_sum;", StmtType::RET},
        StmtCount{"ret;", StmtType::RET}
    )
);

struct InvalidProgram {
    std::string program;
};

class ParserInvalidPrograms : public ::testing::TestWithParam<InvalidProgram> {};

TEST_P(ParserInvalidPrograms, ThrowsOnInvalidCode) {
    const auto& param = GetParam();
    std::shared_ptr<ParserTestCounter> counter = std::make_shared<ParserTestCounter>();
    auto parser = get_parser(param.program, true);
    std::cout << "Testing: " << param.program << std::endl;
    EXPECT_THROW({
        parser->parse();
    }, ParserError);
}

INSTANTIATE_TEST_SUITE_P(
    InvalidPrograms,
    ParserInvalidPrograms,
    ::testing::Values(
        // Missing function body
        InvalidProgram{"int main;"},

        // Bad argument list syntax (double colons)
        InvalidProgram{"int main ::: int a { ret a; }"},

        // Unknown return type
        InvalidProgram{"nonsense main { ret 0; }"},

        // Invalid statement in body
        InvalidProgram{"int main { 5 +; }"},

        // Missing return type
        InvalidProgram{"main :: int a { ret a; }"},

        // Invalid function type return
        InvalidProgram{"[int->int] func { }"}, // wrong arrow

        // Incomplete argument declaration
        InvalidProgram{"flt compute :: int { ret 3.14; }"},

        // Missing closing brace
        InvalidProgram{"string main :: int c { ret \"hi\";"},

        // Invalid keyword in function body
        InvalidProgram{"int main { return a; }"}, // should be `ret`

        // Incorrect function signature nesting
        InvalidProgram{"[[int::int]] func { }"},

        // Statement with broken assignment
        InvalidProgram{"int main { 42 =>; }"},

        // For loop with missing arrow call
        InvalidProgram{"int main { for (i; i < 10) {} }"}, // needs -> identifier

        // While with garbage inside
        InvalidProgram{"int main { while (x <) { } }"},

        // If condition is invalid
        InvalidProgram{"int main { if () {} }"},

        // Elif with no matching if
        InvalidProgram{"int main { elif (x) {} }"},

        // Else without if
        InvalidProgram{"int main { else {} }"},

        // Chained bind front without parens
        InvalidProgram{"int main { (x) ->> y -> z; }"},

        // Random junk in function body
        InvalidProgram{"int main { ??? }"},

        // Unclosed function returning function type
        InvalidProgram{"[int::int some_func { ret (x) -> x; }"},

        // Invalid float literal syntax
        InvalidProgram{"int main { 3.14.15 => flt pi; }"},

        // Valid-looking but missing return type
        InvalidProgram{"main :: int a { ret a; }"},

        // Function header ends in comma (invalid arg list)
        InvalidProgram{"int main :: int a, { ret a; }"},

        // Two return statements without a block separator
        InvalidProgram{"int main { ret 1; ret 2; ret 3 }"},

        // Block with valid syntax but no actual statements
        InvalidProgram{"int main { ;; }"},

        // Assignment to undeclared type
        InvalidProgram{"int main { 5 => banana x; }"},

        // Mixed scope return (illegal early return)
        InvalidProgram{"int main { if (a) ret a; }"},

        // Chained assignment without type
        InvalidProgram{"int main { 1 => x => y; }"},

        // Function declared inside another function
        InvalidProgram{"int main { int inner :: int a { ret a; } }"},

        // Function returns a function, but body uses bad lambda
        InvalidProgram{"[int::int] get_fn { ret x => x + 1; }"},

        // Invalid call syntax (extra parens)
        InvalidProgram{"int main { ((5)) -> a; }"},

        // If statement not followed by block
        InvalidProgram{"int main { if (x) ret x; }"},

        // No function name
        InvalidProgram{"int { ret 0; }"},

        // Missing type in argument list
        InvalidProgram{"int main :: a { ret 0; }"},

        // Double colons in function header
        InvalidProgram{"int main ::::: int a { ret a; }"},

        // Mismatched brackets in return type
        InvalidProgram{"[int::int main { ret (x) -> x; }"}, // opening [ but no closing ]

        // Using ret with no value in non-void function
        InvalidProgram{"int main { ret; }"},

        // Argument list uses semicolon instead of comma
        InvalidProgram{"int main :: int a; int b { ret a; }"},

        // Using `=>` with no left-hand side
        InvalidProgram{"int main { => int x; }"},

        // Invalid call syntax in statement
        InvalidProgram{"int main { (5) => a; }"},

        // FOR missing parentheses
        InvalidProgram{"int main { for i; i < 5 {} -> loop; }"},

        // ELSE with no preceding IF
        InvalidProgram{"int main { else { ret 0; } }"},

        // ELIF directly after ELSE
        InvalidProgram{"int main { if (a) {} else elif (b) {} }"},

        // RETURN statement inside function returning a function, but returning wrong thing
        InvalidProgram{"[int::int] gen { ret 5; }"}, // should return a function, not int

        // FOR without body
        InvalidProgram{"int main { for (i; i < 10) -> go; }"},

        // WHILE with trailing semicolon and no body
        InvalidProgram{"int main { while (i < 3); }"},

        // IF with no condition
        InvalidProgram{"int main { if {} }"},

        // Function return type is valid, but name is missing
        InvalidProgram{"[int::int] { ret (x)->x; }"},

        // Bind expression with missing right-hand side
        InvalidProgram{"int main { 42 => ; }"},

        // Function body has rogue statement
        InvalidProgram{"int main { int x => 5; }"}, // invalid syntax for declaration

        // Random arrow at top level
        InvalidProgram{"-> doSomething;"}
        )
);
