#pragma once

#include "token.h"
#include <memory>
#include <vector>


class FunctionCall;
class Expression;

class Expression {
    private:
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    public:
        virtual ~Expression() = default;
};


class LiteralExpr : public Expression { // base
    public:
        LiteralExpr(FunctionCall call);
        LiteralExpr(Token token);
        LiteralExpr(Expression expr);
};

class UnaryExpr : public Expression {
    private:
        TokenType op_type;
        std::unique_ptr<Expression> right;
    public:
        UnaryExpr(TokenType unary_op, std::unique_ptr<Expression> right);
};


class Term : public Expression {
    public:
        Term(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
};

class BinaryExpr : public Expression {
    public:
        BinaryExpr(std::unique_ptr<Expression> left, TokenType op, std::unique_ptr<Expression> right);
};

class CallExpr : public Expression {
    private:
        std::string func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> args);
};
