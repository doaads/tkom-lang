#pragma once

#include "token.h"
#include <memory>
#include <vector>


class FunctionCall;

class Expression {
    private:
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
    public:
        virtual ~Expression() = default;
};


class LiteralExpr : public Expression { // base
    private:
        std::variant<std::unique_ptr<Expression>, Token> value;
    public:
        LiteralExpr(std::unique_ptr<Expression> expr);
        LiteralExpr(Token token);
};

class UnaryExpr : public Expression {
    private:
        TokenType op_type;
        std::unique_ptr<Expression> right;
    public:
        UnaryExpr(TokenType unary_op, std::unique_ptr<Expression> right);
};

class BinaryExpr : public Expression {
    private:
        std::unique_ptr<Expression> left;
        TokenType op;
        std::unique_ptr<Expression> right;

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
