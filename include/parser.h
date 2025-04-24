#pragma once

#include "lexer.h"
#include "token.h"
#include "tokens.h"
#include "expression.h"
#include "statement.h"
#include "block.h"
#include <optional>

/* Temporary production definitions */
class Program;
class Function;
class TypeMut;
class FuncParams;

using ExprPtr = std::unique_ptr<Expression>;
using StatementPtr = std::unique_ptr<Statement>;
using BlockPtr = std::unique_ptr<Block>;
using ForLoopArgsPtr = std::unique_ptr<ForLoopArgs>;

class Parser {
    private:
        std::shared_ptr<Lexer> lexer;
        Token current_token;
        // add function table here (std::map?)
    public:
        Parser(std::shared_ptr<Lexer> lexer);

        std::optional<Program> parse();
        std::optional<TypeMut> parse_type_mut();
        std::optional<FuncParams> parse_func_params();
        BlockPtr parse_block();
        StatementPtr parse_statement();
        StatementPtr parse_assign();
        StatementPtr parse_conditional_statement(TokenType st_type);
        StatementPtr parse_else_statement();
        StatementPtr parse_for_loop();
        ForLoopArgsPtr parse_for_loop_args();
        StatementPtr parse_while_loop();
        StatementPtr parse_ret_statement();
        StatementPtr parse_call_statement();
        ExprPtr parse_condition();
        ExprPtr parse_or_expression();
        ExprPtr parse_and_expression();
        ExprPtr parse_comp_expression();
        ExprPtr parse_additive_expression();
        ExprPtr parse_term();
        ExprPtr parse_unary();
        ExprPtr parse_factor();
        ExprPtr parse_func_call();
        std::optional<std::vector<ExprPtr>> parse_func_args();

        bool is_comparative(TokenType type) const;
        bool is_factor(TokenType type) const;
        bool is_type(TokenType type) const;
        bool is_type_or_void(TokenType type) const;
        std::optional<Type> translate_token_to_type(TokenType type) const;

        void next_token();
};
