#pragma once

#include "operators.h"
#include "token.h"
#include <memory>
#include <vector>


class FunctionCall;

class Expression {
    protected:
        static std::string indent_str(int amount);
    public:
        virtual void print(std::ostream& os, int indent = 0) const = 0;
        static const std::string expr_type;
        virtual ~Expression() = default;
        friend std::ostream& operator<<(std::ostream& os, const Expression& op);
};


class LiteralExpr : public Expression {
    private:
        std::variant<std::unique_ptr<Expression>, Token> value;
    public:
        void print(std::ostream& os, int indent) const;
        static const std::string expr_type;
        LiteralExpr(std::unique_ptr<Expression> expr);
        LiteralExpr(Token token);
};


class UnaryExpr : public Expression {
    private:
        UnaryOp op_type;
        std::unique_ptr<Expression> right;
    public:
        void print(std::ostream& os, int indent) const;
        static const std::string expr_type;
        UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right);
        UnaryOp get_operator() const;
};


class BinaryExpr : public Expression {
    private:
        std::unique_ptr<Expression> left;
        BinaryOp op;
        std::unique_ptr<Expression> right;
    public:
        void print(std::ostream& os, int indent) const;
        static const std::string expr_type;
        BinaryExpr(std::unique_ptr<Expression> left, BinaryOp op, std::unique_ptr<Expression> right);
        BinaryOp get_operator() const;
};


class CallExpr : public Expression {
    private:
        static const std::string expr_type;
        std::string func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        void print(std::ostream& os, int indent) const;
        CallExpr(std::string name, std::vector<std::unique_ptr<Expression>> args);
};
