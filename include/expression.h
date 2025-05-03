#pragma once

#include "operators.h"
#include "parser_visitor.h"
#include "token.h"
#include <memory>
#include <vector>


class FunctionCall;
class ParserPrinter;

class Expression {
    protected:
        static std::string indent_str(int amount);
    public:
        virtual ~Expression() = default;
        virtual void accept(ParserPrinter& visitor) const = 0;
};


class LiteralExpr : public Expression {
    private:
        std::variant<std::unique_ptr<Expression>, Token> value;
    public:
        LiteralExpr(std::unique_ptr<Expression> expr);
        LiteralExpr(Token token);

        void accept(ParserPrinter& visitor) const override;

        std::variant<const Expression*, Token> get_value() const;
};


class UnaryExpr : public Expression {
    private:
        UnaryOp op_type;
        std::unique_ptr<Expression> right;
    public:
        UnaryExpr(UnaryOp unary_op, std::unique_ptr<Expression> right);
        UnaryOp get_operator() const;
        void accept(ParserPrinter& visitor) const;

        const Expression* get_right() const;
};


class BinaryExpr : public Expression {
    private:
        std::unique_ptr<Expression> left;
        BinaryOp op;
        std::unique_ptr<Expression> right;
    public:
        BinaryExpr(std::unique_ptr<Expression> left, BinaryOp op, std::unique_ptr<Expression> right);
        void accept(ParserPrinter& visitor) const;

        const Expression* get_left() const;
        BinaryOp get_operator() const;
        const Expression* get_right() const;
};


class CallExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        CallExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserPrinter& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};


class BindFrtExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        BindFrtExpr(std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserPrinter& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};
