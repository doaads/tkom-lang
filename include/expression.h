#pragma once

#include <memory>
#include <vector>
#include <variant>

#include "token.h"
#include "operators.h"

enum class BaseType;
class ParserPrinter;

class Expression {
    public:
        virtual ~Expression() = default;
        virtual void accept(ParserPrinter& visitor) const = 0;
};

using ValueType = std::variant<std::monostate, std::string, int, double, bool>;
class LiteralExpr : public Expression {
    private:
        BaseType type;
        ValueType value;
        Position position;
    public:
        template<typename T>
        T get_value() const {
            if (const T* pval = std::get_if<T>(value)) {
                return *pval;
            } else {
                throw std::runtime_error("Unable to get LiteralExpr value");
            }
        }
        LiteralExpr(Token token);
        void accept(ParserPrinter& visitor) const override;
        std::string get_value_string() const;
};

class IdentifierExpr : public Expression {
    private:
        std::string identifier;
    public:
        IdentifierExpr(std::string identifier);

        void accept(ParserPrinter& visitor) const override;
        std::string get_identifier() const;
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
