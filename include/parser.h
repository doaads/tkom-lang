#pragma once

#include "lexer.h"
#include "token.h"
#include "tokens.h"

/* Temporary production definitions */
class Program;
class Function;
class TypeMut;
class FuncParams;
class Statement;
class IfStatement;
class ForLoop;
class WhileLoop;
class RetStatement;
class Expression;

class Parser {
    private:
        std::shared_ptr<Lexer> lexer;
        Token current_token;
        // add function table here (std::map?)
    public:
        Parser(std::shared_ptr<Lexer> lexer);

        std::variant<std::monostate, Program> parse();
        std::variant<std::monostate, TypeMut> parse_type_mut();
        std::variant<std::monostate, FuncParams> parse_func_params();
        std::variant<std::monostate, Statement> parse_statement();
        std::variant<std::monostate, Statement> parse_if_statement();
        std::variant<std::monostate, Statement> parse_else_statement();
        std::variant<std::monostate, Statement> parse_elif_statement();
        std::variant<std::monostate, Statement> parse_for_loop();
        std::variant<std::monostate, Statement> parse_while_loop();
        std::variant<std::monostate, Statement> parse_ret_statement();
        std::variant<std::monostate, Expression> parse_or_expression();
        std::variant<std::monostate, Expression> parse_and_expression();
        std::variant<std::monostate, Expression> parse_comp_expression();
        std::variant<std::monostate, Expression> parse_additive();
        std::variant<std::monostate, Expression> parse_term();
        std::variant<std::monostate, Expression> parse_unary();
        std::variant<std::monostate, Expression> parse_factor();
        std::variant<std::monostate, Expression> parse_func_call();

        bool is_comparative(TokenType type) const;
};


std::variant<std::monostate, Expression> Parser::parse_or_expression() {
    std::variant<std::monostate, Expression> left = parse_and_expression();
    if (!left.index())) {
        return std::monostate();
    }
    current_token = lexer->get_token();
    TokenType type = current_token.get_type();
    while (current_token.get_type() == TokenType::T_OR) {
        current_token = lexer->get_token();
        std::variant<std::monostate, Expression> right = parse_and_expression();
        // throw if index 0
        left = BinaryExpr(left, type, right)
        type = current_token.get_type();
    }
    return left;
}


std::variant<std::monostate, Expression> Parser::parse_and_expression() {
    std::variant<std::monostate, Expression> left = parse_comp_expression();
    if (!left.index())) {
        return std::monostate();
    }
    current_token = lexer->get_token();
    TokenType type = current_token.get_type();
    while (current_token.get_type() == TokenType::T_AND) {
        current_token = lexer->get_token();
        std::variant<std::monostate, Expression> right = parse_comp_expression();
        // throw if index 0
        left = BinaryExpr(left, TokenType::T_AND, right)
        type = current_token.get_type();
    }
    return left;
}


std::variant<std::monostate, Expression> Parser::parse_comp_expression() {
    std::variant<std::monostate, Expression> left = parse_additive_expression();
    if (!left.index())) {
        return std::monostate();
    }
    current_token = lexer->get_token();
    TokenType type = current_token.get_type();
    if (is_comparative(type)) {
        current_token = lexer->get_token();
        std::variant<std::monostate, Expression> right = parse_additive_expression();
        // throw if index 0
        left = BinaryExpr(left, type, right)
    }
    return left;
}


std::variant<std::monostate, Expression> Parser::parse_additive_expression() {
    std::variant<std::monostate, Expression> left = parse_term();
    if (!left.index())) {
        return std::monostate();
    }
    current_token = lexer->get_token();
    TokenType op_type = current_token.get_type();
    while (type == TokenType::T_PLUS || type == TokenType::T_MINUS) {
        current_token = lexer->get_token();
        std::variant<std::monostate, Expression> right = parse_term();
        // throw if index 0
        left = BinaryExpr(left, op_type, right)
        op_type = current_token.get_type();
    }
    return left;
}

std::variant<std::monostate, Expression> Parser::parse_term() {
    std::variant<std::monostate, Expression> left = parse_unary();
    if (!left.index())) {
        return std::monostate();
    }
    current_token = lexer->get_token();
    TokenType op_type = current_token.get_type();
    if (type == TokenType::T_MULT || type == TokenType::T_DIV) {
        current_token = lexer->get_token();
        std::variant<std::monostate, Expression> right = parse_unary();
        // throw if index 0
        left = BinaryExpr(left, op_type, right)
        op_type = current_token.get_type();
    }
    return left;
}


std::variant<std::monostate, Expression> Parser::parse_unary() {
    TokenType type = current_token.get_type();
    std::variant<std::monostate, TokenType> unary_op;
    if (type == T_MINUS || type == T_NOT) {
        current_token = lexer->get_token();
        unary_op = type;
    }
    std::variant<std::monostate, Expression> right = parse_factor();
    // throw if right index is 0
    return unary_op.index() ? UnaryExpr(unary_op, right) : right;
}

std::variant<std::monostate, Expression> Parser::parse_factor() {
    std::variant<std::monostate, Expression> factor = parse_function_call();
    if (factor.index()) return factor

    TokenType type = current_token.get_type();

    if (type == TokenType::LPAREN) {
        current_token = lexer->get_token();
        factor = parse_or_expression();
        // throw if monostate?
        current_token = lexer->get_token();
        // throw if not rparen
        return factor;
    }

    if (is_factor(type)) {
        return LiteralExpr(current_token);
    }
}


bool is_comparative(TokenType type) const {
    switch (type) {
        case TokenType::T_NEQ: return true;
        case TokenType::T_EQ: return true;
        case TokenType::T_LTE: return true;
        case TokenType::T_GTE: return true;
        case TokenType::T_LT: return true;
        case TokenType::T_GT: return true;
        defualt: return false;
    }
}

bool is_factor(TokenType type) const {
    switch (type) {
        case TokenType::T_INT: return true;
        case TokenType::T_INT: return true;
        case TokenType::T_FLT: return true;
        case TokenType::T_IDENTIFIER: return true;
        case TokenType::T_STRING: return true;
    }
}
