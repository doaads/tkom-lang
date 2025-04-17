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
using StatementType = std::optional<StatementPtr>;
using ExprType = std::optional<ExprPtr>;
using BlockType = std::optional<std::unique_ptr<Block>>;
using BlockPtr = std::unique_ptr<Block>;

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
        BlockType parse_block();
        StatementType parse_statement();
        StatementType parse_assign();
        StatementType parse_conditional_statement(TokenType st_type);
        StatementType parse_else_statement();
        StatementType parse_for_loop();
        StatementType parse_while_loop();
        StatementType parse_ret_statement();
        StatementType parse_call_statement();
        ExprType parse_condition();
        ExprType parse_or_expression();
        ExprType parse_and_expression();
        ExprType parse_comp_expression();
        ExprType parse_additive_expression();
        ExprType parse_term();
        ExprType parse_unary();
        ExprType parse_factor();
        ExprType parse_func_call();
        std::optional<std::vector<ExprPtr>> parse_func_args();

        bool is_comparative(TokenType type) const;
        bool is_factor(TokenType type) const;
        bool is_type(TokenType type) const;
        bool is_type_or_void(TokenType type) const;
        std::optional<Type> translate_token_to_type(TokenType type) const;

        void next_token();
};
