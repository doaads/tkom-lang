#include "expression.h"

LiteralExpr::LiteralExpr(std::unique_ptr<Expression> expr) :
    value(std::move(expr)) {}

LiteralExpr::LiteralExpr(Token token) :
    value(token) {}

UnaryExpr::UnaryExpr(TokenType unary_op, std::unique_ptr<Expression> right) :
    op_type(unary_op), right(std::move(right)) {}

BinaryExpr::BinaryExpr(
        std::unique_ptr<Expression> left,
        TokenType op,
        std::unique_ptr<Expression> right) :
    left(std::move(left)), op(op), right(std::move(right)) {}

CallExpr::CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> args) : func_name(name), args(std::move(args)) {}
