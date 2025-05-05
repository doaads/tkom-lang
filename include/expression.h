#pragma once

#include <memory>
#include <vector>
#include <variant>

#include "node.h"
#include "token.h"
#include "operators.h"

enum class BaseType;
class ParserVisitor;

class Expression : public Node {
    public:
        Expression(const Position pos) : Node(pos) {}
        virtual ~Expression() = default;
        virtual void accept(ParserVisitor& visitor) const = 0;
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
        LiteralExpr(const Position pos, Token token);
        void accept(ParserVisitor& visitor) const override;
        std::string get_value_string() const;
};

class IdentifierExpr : public Expression {
    private:
        std::string identifier;
    public:
        IdentifierExpr(const Position pos, std::string identifier);

        void accept(ParserVisitor& visitor) const override;
        std::string get_identifier() const;
};


class UnaryExpr : public Expression {
    private:
        UnaryOp op_type;
        std::unique_ptr<Expression> right;
    public:
        UnaryExpr(const Position pos, UnaryOp unary_op, std::unique_ptr<Expression> right);
        UnaryOp get_operator() const;
        void accept(ParserVisitor& visitor) const;

        const Expression* get_right() const;
};


class BinaryExpr : public Expression {
    private:
        std::unique_ptr<Expression> left;
        BinaryOp op;
        std::unique_ptr<Expression> right;
    public:
        BinaryExpr(const Position pos, std::unique_ptr<Expression> left, BinaryOp op, std::unique_ptr<Expression> right);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_left() const;
        BinaryOp get_operator() const;
        const Expression* get_right() const;
};


class CallExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        CallExpr(const Position pos, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};


class BindFrtExpr : public Expression {
    private:
        std::unique_ptr<Expression> func_name;
        std::vector<std::unique_ptr<Expression>> args;
    public:
        BindFrtExpr(const Position pos, std::unique_ptr<Expression> name, std::vector<std::unique_ptr<Expression>> args);
        void accept(ParserVisitor& visitor) const;

        const Expression* get_func_name() const;
        const std::vector<const Expression*> get_args() const;
};
