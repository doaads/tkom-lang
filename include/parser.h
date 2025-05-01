#pragma once

#include "lexer.h"
#include "program.h"
#include "token.h"
#include "tokens.h"
#include "expression.h"
#include "statement.h"
#include "block.h"
#include "function.h"
#include <optional>

using ExprPtr = std::unique_ptr<Expression>;
using StatementPtr = std::unique_ptr<Statement>;
using BlockPtr = std::unique_ptr<Block>;
using ForLoopArgsPtr = std::unique_ptr<ForLoopArgs>;
using FuncPtr = std::unique_ptr<Function>;
using TypePtr = std::unique_ptr<Type>;
using FuncSignPtr = std::unique_ptr<FuncSignature>;
using ProgramPtr = std::unique_ptr<Program>;

class Parser {
    private:
        std::shared_ptr<Lexer> lexer;
        Token current_token;
    public:
        Parser(std::shared_ptr<Lexer> lexer);

        ProgramPtr parse();
        FuncPtr parse_func_def();
        FuncSignPtr parse_func_signature();
        BlockPtr parse_block();
        StatementPtr parse_statement();
        StatementPtr parse_assign_or_call();
        StatementPtr parse_conditional_statement(TokenType st_type);
        StatementPtr parse_else_statement();
        StatementPtr parse_for_loop();
        ForLoopArgsPtr parse_for_loop_args();
        StatementPtr parse_while_loop();
        StatementPtr parse_ret_statement();
        //StatementPtr parse_call_statement();
        ExprPtr parse_condition();
        ExprPtr parse_expression();
        ExprPtr parse_or_expression();
        ExprPtr parse_and_expression();
        ExprPtr parse_comp_expression();
        ExprPtr parse_additive_expression();
        ExprPtr parse_term();
        ExprPtr parse_unary();
        ExprPtr parse_factor();
        ExprPtr parse_func_call_or_parens();

        ExprPtr parse_bindfrt_or_call(std::vector<ExprPtr> args);
        ExprPtr parse_bind_front_right(std::vector<ExprPtr> args);
        ExprPtr parse_bind_front();
        ExprPtr parse_decorator();

        ExprPtr parse_identifier();
        ExprPtr parse_func_call(ExprPtr first = nullptr);
        TypePtr parse_type();
        TypePtr parse_var_type();
        TypePtr parse_func_type();
        std::optional<std::vector<ExprPtr>> parse_func_args(ExprPtr first = nullptr);

        bool is_comparative(TokenType type) const;
        bool is_factor(TokenType type) const;
        bool is_type(TokenType type) const;
        bool is_type_or_void(TokenType type) const;
        bool is_next_token(TokenType type);
        bool was_last_token(TokenType type);
        std::optional<BaseType> translate_token_to_type(TokenType type) const;

        void next_token();
};
